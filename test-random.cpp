/*

Copyright (c) 2014 Harm Hanemaaijer <fgenfb@yahoo.com>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/

//
// Random number generator accuracy and performance tests.
//
// Compile with:
// g++ -Ofast test-random.cpp -o test-random -lm -lDataSetTurbo -lfgen
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>
#include <fgen.h>

#include "dstRandom.h"
#include "dstMatrixMath.h"

#define MAX_N (1 << 24)

#define NU_RNGS 3

enum {
	RNG_DST = 0,
	RNG_FGEN = 1,
	RNG_DST_STATE_1 = 2,
};

enum {
	MASK_DST = 1,
	MASK_FGEN = 2,
	MASK_DST_STATE_1 = 4,
};

#define NU_TESTS 13

enum {
	TEST_RANDOM_INT_POWER_OF_TWO = 0,
	TEST_RANDOM_POWER_OF_TWO = 1,
	TEST_RANDOM_INT_SMALL = 2,
	TEST_RANDOM_INT_MEDIUM = 3,
	TEST_RANDOM_INT_LARGE = 4,
        TEST_RANDOM_INT_GENERAL = 5,
        TEST_RANDOM_INT_GENERAL_EMPIRICAL = 6,
        TEST_RANDOM_INT_GENERAL_REMAINDER = 7,
        TEST_RANDOM_FLOAT = 8,
	TEST_RANDOM_FLOAT_LP = 9, 
	TEST_RANDOM_DOUBLE = 10,
	TEST_RANDOM_DOUBLE_LP = 11,
	TEST_RANDOM_DOUBLE_HP = 12
};

enum {
	MASK_RANDOM_INT_POWER_OF_TWO = 1,
	MASK_RANDOM_POWER_OF_TWO = 2,
	MASK_SMALL = 4,
	MASK_MEDIUM = 8,
	MASK_LARGE = 16,
        MASK_GENERAL = 32,
        MASK_GENERAL_EMPIRICAL = 64,
	MASK_GENERAL_REMAINDER = 128,
	MASK_FLOAT = 256,
	MASK_FLOAT_LP = 512,
	MASK_DOUBLE = 1024,
	MASK_DOUBLE_LP = 2048,
	MASK_DOUBLE_HP = 4096
};

static const int rng_test_mask[NU_RNGS] = {
	MASK_RANDOM_INT_POWER_OF_TWO | MASK_RANDOM_POWER_OF_TWO | MASK_SMALL |
        MASK_MEDIUM | MASK_LARGE | MASK_GENERAL_EMPIRICAL | MASK_GENERAL |
        MASK_GENERAL_REMAINDER | MASK_FLOAT | MASK_FLOAT_LP |
	MASK_DOUBLE | MASK_DOUBLE_LP | MASK_DOUBLE_HP,
	MASK_RANDOM_INT_POWER_OF_TWO | MASK_SMALL | MASK_MEDIUM |
	MASK_LARGE | MASK_GENERAL | MASK_FLOAT | MASK_FLOAT_LP |
	MASK_DOUBLE | MASK_DOUBLE_LP | MASK_DOUBLE_HP,
	MASK_RANDOM_INT_POWER_OF_TWO | MASK_RANDOM_POWER_OF_TWO | MASK_SMALL |
        MASK_MEDIUM | MASK_LARGE | MASK_GENERAL_EMPIRICAL | MASK_GENERAL |
        MASK_GENERAL_REMAINDER | MASK_FLOAT | MASK_FLOAT_LP |
	MASK_DOUBLE | MASK_DOUBLE_LP | MASK_DOUBLE_HP,
};

static int rng_mask, test_mask;
static int count[MAX_N];
static dstRNG *dst_rng[NU_RNGS];
static FgenRNG *fgen_rng;
static double total_M_ops_per_sec[NU_RNGS];
static int volatile_global;
static bool quiet;

static const char *library_name[NU_RNGS] = {
	"DST (defaults)", "libfgen", "DST (state size = 1)"
};

static const char *test_str[NU_RNGS][NU_TESTS] = {
	{
	"dstRNG::RandomInt(",
	"dstRNG::RandomIntPowerOfTwo(",
	"dstRNG::RandomInt(",
	"dstRNG::RandomInt(",
	"dstRNG::RandomInt(",
	"dstRNG::RandomInt(",
	"dstRNG::RandomIntEmpirical(",
	"dstRNG::RandomIntRemainder(",
        "dstRNG::RandomFloatHP(",
	"dstRNG::RandomFloat(",
        "dstRNG::RandomDouble(",
        "dstRNG::RandomDoubleLP(",
        "dstRNG::RandomDoubleHP(",
	},
	{
	"fgen_random_n(rng, ",
	"Invalid(rng, ",
	"fgen_random_n(rng, ",
	"fgen_random_n(rng, ",
	"fgen_random_n(rng, ",
	"fgen_random_n(rng, ",
        "Invalid(rng, ",
	"Invalid(rng, ",
        "fgen_random_f(rng, ",
        "fgen_random_f_low_precision(rng, ",
        "fgen_random_d(rng, ",
        "fgen_random_d_low_precision(rng, ",
        "fgen_random_d_high_precision(rng, ",
	},
	{
	"dstRNG::RandomInt(",
	"dstRNG::RandomIntPowerOfTwo(",
	"dstRNG::RandomInt(",
	"dstRNG::RandomInt(",
	"dstRNG::RandomInt(",
	"dstRNG::RandomInt(",
	"dstRNG::RandomIntEmpirical(",
	"dstRNG::RandomIntRemainder(",
        "dstRNG::RandomFloatHP(",
	"dstRNG::RandomFloat(",
        "dstRNG::RandomDouble(",
        "dstRNG::RandomDoubleLP(",
        "dstRNG::RandomDoubleHP(",
	},
};

static double GetCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 0.000001;
}

void PerformTestRun(int rng_index, int test_index, int n, int runs) {
	if (rng_index == RNG_FGEN)
		for (int i = 0; i < runs; i++) {
			int r = fgen_random_n(fgen_rng, n);
			count[r]++;
		}
	else {
		dstRNG *rng = dst_rng[rng_index];
		if (test_index == TEST_RANDOM_POWER_OF_TWO)
			for (int i = 0; i < runs; i++) {
				int r = rng->RandomIntPowerOfTwo(n);
				count[r]++;
			}
		else if (test_index == TEST_RANDOM_INT_GENERAL_EMPIRICAL)
			for (int i = 0; i < runs; i++) {
				int r = rng->RandomIntEmpirical(n);
				count[r]++;
			}
#ifdef DST_RANDOM_INT_INCLUDE_REMAINDER_STRATEGY
		else if (test_index == TEST_RANDOM_INT_GENERAL_REMAINDER)
			for (int i = 0; i < runs; i++) {
				int r = rng->RandomIntRemainder(n);
				count[r]++;
			}
#endif
		else
			for (int i = 0; i < runs; i++) {
				int r = rng->RandomInt(n);
				count[r]++;
			}
	}
}

float PerformTestRunFloat(int rng_index, int test_index, float range, int runs) {
	float sum = 0;
	if (rng_index == RNG_FGEN) {
		if (test_index == TEST_RANDOM_FLOAT)
			for (int i = 0; i < runs; i++) {
				float r = fgen_random_f(fgen_rng, range);
				sum += r;
			}
		else if (test_index == TEST_RANDOM_FLOAT_LP)
			for (int i = 0; i < runs; i++) {
				float r = fgen_random_f_low_precision(fgen_rng, range);
				sum += r;
			}
	}
	else {
		dstRNG *rng = dst_rng[rng_index];
		if (test_index == TEST_RANDOM_FLOAT)
			for (int i = 0; i < runs; i++) {
				// fgen_random_f is equivalent to RandomFloatHP.
				float r = rng->RandomFloatHP(range);
				sum += r;
			}
		else if (test_index == TEST_RANDOM_FLOAT_LP)
			for (int i = 0; i < runs; i++) {
				// fgen_random_f_low_precision is equivalent to RandomFloat.
				float r = rng->RandomFloat(range);
				sum += r;
			}
	}
	volatile_global = sum;
	return sum;
}

double PerformTestRunDouble(int rng_index, int test_index, double range, int runs) {
	double sum = 0;
	if (rng_index == RNG_FGEN) {
		if (test_index == TEST_RANDOM_DOUBLE)
			for (int i = 0; i < runs; i++) {
				double r = fgen_random_d(fgen_rng, range);
				sum += r;
			}
		else if (test_index == TEST_RANDOM_DOUBLE_LP)
			for (int i = 0; i < runs; i++) {
				double r = fgen_random_d_low_precision(fgen_rng, range);
				sum += r;
			}
		else if (test_index == TEST_RANDOM_DOUBLE_HP)
			for (int i = 0; i < runs; i++) {
				double r = fgen_random_d_high_precision(fgen_rng, range);
				sum += r;
			}
	}
	else {
		dstRNG *rng = dst_rng[rng_index];
		if (test_index == TEST_RANDOM_DOUBLE)
			for (int i = 0; i < runs; i++) {
				// fgen_random_d is equivalent to RandomDouble,
				double r = rng->RandomDouble(range);
				sum += r;
			}
		else if (test_index == TEST_RANDOM_DOUBLE_LP)
			for (int i = 0; i < runs; i++) {
				double r = rng->RandomDoubleLP(range);
				sum += r;
			}
		else if (test_index == TEST_RANDOM_DOUBLE_HP)
			for (int i = 0; i < runs; i++) {
				double r = rng->RandomDoubleHP(range);
				sum += r;
			}
	}
	volatile_global = sum;
	return sum;
}

static void TestRandomInt(int test_index, unsigned int n, int rng_index) {
		int runs;
		runs = 10000000;
		unsigned int k;
		for (k = 0; k <= 31; k++)
			if (n == ((unsigned int)1 << k))
				break;
		if (k == 32)
			runs /= 4;
		// Warm up.
		for (int i = 0; i < n; i++)
			count[i] = 0;
		PerformTestRun(rng_index, test_index, n, runs / 8);
		int sum = 0;
		for (int i = 0; i < n; i++) {
			sum += count[i];
			count[i] = 0;
		}
		volatile_global = sum;
		// Perform test.
		double start_time = GetCurrentTime();

		PerformTestRun(rng_index, test_index, n, runs);

		double end_time = GetCurrentTime();
		double ops_per_sec = runs / (end_time - start_time);
		// Calculate the standard deviation.
		uint64_t total = 0;
		for (int i = 0; i < n; i++)
			total += count[i] * i;
		double avg = (double)total / runs;
		double total_diff_squared = 0;
		for (int i = 0; i < n; i++)
			total_diff_squared += count[i] * pow(i - avg, 2.0);
		double sd = sqrt(total_diff_squared / runs);
		// Calculate the expected standard deviation.
		double avg_expected = (double)n * 0.5 - 0.5;
		total_diff_squared = 0;
		for (int i = 0; i < n; i++)
			total_diff_squared += pow(i - avg_expected, 2.0);
		double sd_expected = sqrt(total_diff_squared / n);
		if (!quiet)
			printf("%s%d) (%.2lfM ops/sec): SD = %lf (%lf expected)\n",
				test_str[rng_index][test_index], n,
				ops_per_sec / 1000000.0, sd, sd_expected);
		if ((fabs(sd - sd_expected) / sd_expected) > 0.01) {
			printf("Actual SD for n = %d deviates more than 1%% from expected SD.\n"
				"Average %lf vs %lf.\n", n, avg, avg_expected);
			if (!quiet) {
				printf("Distribution:\n");
				for (int i = 0; i < n; i++)
					printf("%d: %d ", i, count[i]);
				printf("\n");
			}
		}
		fflush(stdout);
#if 0
		// Calculate standard deviation of counts.
		total = 0;
		for (int i = 0; i < n; i++)
			total += count[i];
		avg = total / n;
		total_diff_squared = 0;
		for (int i = 0; i < n; i++)
			total_diff_squared += pow(count[i] - avg, 2.0);
		sd = sqrt(total_diff_squared / n);
		printf("SD of distibution of %d times RandomInt(%d) = %lf\n",
			runs, n, sd);
#endif
		total_M_ops_per_sec[rng_index] += ops_per_sec / 1000000.0;
}

static void ReportAverageOpsPerSecond(int test_index, int mask, int nu_tests) {
	for (int i = 0; i < NU_RNGS; i++)
		if (mask & (1 << i) && (rng_test_mask[i] & (1 << test_index))) {
			double avg_M_ops_per_sec = total_M_ops_per_sec[i] / nu_tests;
			printf("library %s: %sn): %.2lfM ops/sec.\n",
				library_name[i], test_str[i][test_index],
				avg_M_ops_per_sec);
		}
}

static void TestRandomFloat(int test_index, float range, int rng_index) {
	int runs;
	runs = 10000000;
	// Warm up.
	PerformTestRunFloat(rng_index, test_index, range, runs / 8);
	// Perform test.
	double start_time = GetCurrentTime();
	float avg = PerformTestRunFloat(rng_index, test_index, range, runs);
	double end_time = GetCurrentTime();
	double ops_per_sec = runs / (end_time - start_time);
	double sd = 0;
	double sd_expected = 0;
        avg /= runs;
	float avg_expected = range / 2;
	if (!quiet)
		printf("%s%f) (%.2lfM ops/sec): avg = %lf (%lf expected)\n",
			test_str[rng_index][test_index], range,
			ops_per_sec / 1000000.0, avg, avg_expected);
	if ((fabs(sd - sd_expected) / sd_expected) > 0.01) {
		printf("Actual SD for range = %f deviates more than 1%% from expected SD.\n"
			"Average %lf vs %lf.\n", range, avg, avg_expected);
	}
	fflush(stdout);
	total_M_ops_per_sec[rng_index] += ops_per_sec / 1000000.0;
}

static void TestRandomDouble(int test_index, double range, int rng_index) {
	int runs;
	runs = 10000000;
        if (test_index == TEST_RANDOM_DOUBLE_HP)
            runs /= 2;
	// Warm up.
	PerformTestRunDouble(rng_index, test_index, range, runs / 8);
	// Perform test.
	double start_time = GetCurrentTime();
	double avg = PerformTestRunDouble(rng_index, test_index, range, runs);
	double end_time = GetCurrentTime();
	double ops_per_sec = runs / (end_time - start_time);
	double sd = 0;
	double sd_expected = 0;
        avg /= runs;
	double avg_expected = range / 2;
	if (!quiet)
		printf("%s%lf) (%.2lfM ops/sec): avg = %lf (%lf expected)\n",
			test_str[rng_index][test_index], range,
			ops_per_sec / 1000000.0, avg, avg_expected);
	if ((fabs(sd - sd_expected) / sd_expected) > 0.01) {
		printf("Actual SD for range = %lf deviates more than 1%% from expected SD.\n"
			"Average %lf vs %lf.\n", range, avg, avg_expected);
	}
	fflush(stdout);
	total_M_ops_per_sec[rng_index] += ops_per_sec / 1000000.0;
}


// Test that include both power of two and general exponentially increasing
// values. Can be called as different tests.

static void TestGeneral(int test_mask, int rng_mask, int test_index) {
	for (int i = 0; i < NU_RNGS; i++)
	        total_M_ops_per_sec[i] = 0;
	if (test_mask & (1 << test_index)) {
	int n_count;
	for (int j = 0; j < NU_RNGS; j++)
		if ((rng_mask & (1 << j))  &&
		(rng_test_mask[j] & (1 << test_index))) {
			for (int i = 1; i <= 24; i++) {
				TestRandomInt(test_index,
                                (unsigned int)1 << i, j);
			}
                        n_count = 24;
			int n = 1;
			for (;;) {
				n_count++;
				TestRandomInt(test_index, n, j);
				n = ceil(1 + n * 1.04);
				if (n > MAX_N)
					break;
			}
		}
	printf("Power of two and general n:\n");
	ReportAverageOpsPerSecond(test_index, rng_mask, n_count);
	}
}

int main(int argc, char *argv[]) {
	rng_mask = 0;
	test_mask = 0;
	quiet = false;
	int argi = 1;
	while (argi < argc) {
		if (strcmp(argv[argi], "--dst") == 0)
			rng_mask |= MASK_DST;
		else if (strcmp(argv[argi], "--fgen") == 0)
			rng_mask |= MASK_FGEN;
		else if (strcmp(argv[argi], "--dst_state_1") == 0)
			rng_mask |= MASK_DST_STATE_1;
		else if (strcmp(argv[argi], "--random_int_power_of_two") == 0)
			test_mask |= MASK_RANDOM_INT_POWER_OF_TWO;
		else if (strcmp(argv[argi], "--small") == 0)
			test_mask |= MASK_SMALL;
		else if (strcmp(argv[argi], "--medium") == 0)
			test_mask |= MASK_MEDIUM;
		else if (strcmp(argv[argi], "--large") == 0)
			test_mask |= MASK_LARGE;
		else if (strcmp(argv[argi], "--random_power_of_two") == 0)
			test_mask |= MASK_RANDOM_POWER_OF_TWO;
		else if (strcmp(argv[argi], "--empirical") == 0)
			test_mask |= MASK_GENERAL_EMPIRICAL;
		else if (strcmp(argv[argi], "--general") == 0)
			test_mask |= MASK_GENERAL;
		else if (strcmp(argv[argi], "--remainder") == 0)
			test_mask |= MASK_GENERAL_REMAINDER;
		else if (strcmp(argv[argi], "--float") == 0)
			test_mask |= MASK_FLOAT;
		else if (strcmp(argv[argi], "--float_lp") == 0)
			test_mask |= MASK_FLOAT_LP;
		else if (strcmp(argv[argi], "--double") == 0)
			test_mask |= MASK_DOUBLE;
		else if (strcmp(argv[argi], "--double_lp") == 0)
			test_mask |= MASK_DOUBLE_LP;
		else if (strcmp(argv[argi], "--double_hp") == 0)
			test_mask |= MASK_DOUBLE_HP;
		else if (strcmp(argv[argi], "--quiet") == 0)
			quiet = true;
		else {
			printf("test-random: Unrecognized command-line argument %s.\n",
				argv[argi]);
			exit(1);
		}
		argi++;		
	}
	if (rng_mask == 0)
		// Run both libraries with standard settings by default.
		rng_mask = MASK_DST | MASK_FGEN;
	if (test_mask == 0)
		// Run all tests by default.
		test_mask = MASK_RANDOM_INT_POWER_OF_TWO | MASK_SMALL | MASK_MEDIUM |
			MASK_LARGE | MASK_RANDOM_POWER_OF_TWO |
                        MASK_GENERAL_EMPIRICAL | MASK_GENERAL | MASK_GENERAL_REMAINDER |
                        MASK_FLOAT | MASK_FLOAT_LP | MASK_DOUBLE | MASK_DOUBLE_LP |
			MASK_DOUBLE_HP;

	if (rng_mask & MASK_DST)
		dst_rng[RNG_DST] = new dstCMWCRNG();
	if (rng_mask & MASK_DST_STATE_1)
		dst_rng[RNG_DST_STATE_1] = new dstCMWCRNG(1);
	if (rng_mask & MASK_FGEN) {
		fgen_rng = fgen_random_create_rng();
	}

	for (int i = 0; i < NU_RNGS; i++)
	        total_M_ops_per_sec[i] = 0;
	if (test_mask & MASK_RANDOM_INT_POWER_OF_TWO) {
	for (int j = 0; j < NU_RNGS; j++)
		if ((rng_mask & (1 << j)) &&
		(rng_test_mask[j] & MASK_RANDOM_INT_POWER_OF_TWO)) {
			for (int i = 1; i <= 20; i++) {
				TestRandomInt(TEST_RANDOM_INT_POWER_OF_TWO, 1 << i, j);
			}
		}
	printf("Power of two using general RandomInt(n):\n");
	ReportAverageOpsPerSecond(TEST_RANDOM_INT_POWER_OF_TWO, rng_mask, 20);
	}

	for (int i = 0; i < NU_RNGS; i++)
	        total_M_ops_per_sec[i] = 0;
	if (test_mask & MASK_RANDOM_POWER_OF_TWO) {
	for (int j = 0; j < NU_RNGS; j++)
		if ((rng_mask & (1 << j))  &&
		(rng_test_mask[j] & MASK_RANDOM_POWER_OF_TWO)) {
			for (int i = 1; i <= floor(log2(MAX_N)); i++) {
				TestRandomInt(TEST_RANDOM_POWER_OF_TWO, 1 << i, j);
			}
		}
	printf("Power of two using specific RandomIntPowerOfTwo(n):\n");
	ReportAverageOpsPerSecond(TEST_RANDOM_POWER_OF_TWO, rng_mask, 20);
	}

	for (int i = 0; i < NU_RNGS; i++)
	        total_M_ops_per_sec[i] = 0;
	if (test_mask & MASK_SMALL) {
	int n_count;
		for (int j = 0; j < NU_RNGS; j++)
			if (rng_mask & (1 << j)) {
				n_count = 0;
				for (int i = 1; i < 256; i++) {
					TestRandomInt(TEST_RANDOM_INT_SMALL, i, j);
					n_count++;
					i += (i >= 64) + (i >= 128);
				}
			}
	printf("Overall n <= 255 using general RandomInt(n):\n");
	ReportAverageOpsPerSecond(TEST_RANDOM_INT_SMALL, rng_mask, n_count);
	}

	for (int i = 0; i < NU_RNGS; i++)
	        total_M_ops_per_sec[i] = 0;
	if (test_mask & MASK_MEDIUM) {
        int n_count;
	for (int j = 0; j < NU_RNGS; j++)
		if (rng_mask & (1 << j)) {
			n_count = 0;
			int n = 256;
			for (;;) {
				n_count++;
				TestRandomInt(TEST_RANDOM_INT_MEDIUM, n, j);
				n += ceil(pow((double)n / 65536, 1.3) * 10000.0);
				if (n >= 65536)
					break;
			}
		}
	printf("Overall n <= 65535 using general RandomInt(n):\n");
	ReportAverageOpsPerSecond(TEST_RANDOM_INT_MEDIUM, rng_mask, n_count);
	}

	for (int i = 0; i < NU_RNGS; i++)
	        total_M_ops_per_sec[i] = 0;
	if (test_mask & MASK_LARGE) {
        int n_count;
	for (int j = 0; j < NU_RNGS; j++)
		if (rng_mask & (1 << j)) {
			n_count = 0;
			int n = 65536;
			for (;;) {
				n_count++;
				TestRandomInt(TEST_RANDOM_INT_LARGE, n, j);
				n += ceil(pow(((double)n + MAX_N / 8) /
					(MAX_N + MAX_N / 8), 1.4) * 1000000.0);
				if (n > MAX_N)
					break;
			}
		}
	printf("Large n (65536 - %d) using general RandomInt(n):\n", MAX_N);
	ReportAverageOpsPerSecond(TEST_RANDOM_INT_LARGE, rng_mask, n_count);
	}

        TestGeneral(test_mask, rng_mask, TEST_RANDOM_INT_GENERAL);

        TestGeneral(test_mask, rng_mask, TEST_RANDOM_INT_GENERAL_EMPIRICAL);

#ifdef DST_RANDOM_INT_INCLUDE_REMAINDER_STRATEGY
        TestGeneral(test_mask, rng_mask, TEST_RANDOM_INT_GENERAL_REMAINDER);
#endif

	for (int i = 0; i < NU_RNGS; i++)
	        total_M_ops_per_sec[i] = 0;
	if (test_mask & MASK_FLOAT) {
	int n_count;
		for (int j = 0; j < NU_RNGS; j++)
			if (rng_mask & (1 << j)) {
				n_count = 0;
				for (int i = 0; i < 11; i++) {
					TestRandomFloat(TEST_RANDOM_FLOAT,
						powf(10.0f, i - 5), j);
					n_count++;
				}
			}
	printf("Float in ranges from 0.00001 to 100000.0 using RandomFloatHP(range):\n");
	ReportAverageOpsPerSecond(TEST_RANDOM_FLOAT, rng_mask, n_count);
	}

	for (int i = 0; i < NU_RNGS; i++)
	        total_M_ops_per_sec[i] = 0;
	if (test_mask & MASK_FLOAT_LP) {
	int n_count;
		for (int j = 0; j < NU_RNGS; j++)
			if (rng_mask & (1 << j)) {
				n_count = 0;
				for (int i = 0; i < 11; i++) {
					TestRandomFloat(TEST_RANDOM_FLOAT_LP,
						powf(10.0f, i - 5), j);
					n_count++;
				}
			}
	printf("Float in ranges from 0.00001 to 100000.0 using RandomFloat(range):\n");
	ReportAverageOpsPerSecond(TEST_RANDOM_FLOAT_LP, rng_mask, n_count);
	}

	for (int i = 0; i < NU_RNGS; i++)
	        total_M_ops_per_sec[i] = 0;
	if (test_mask & MASK_DOUBLE) {
	int n_count;
		for (int j = 0; j < NU_RNGS; j++)
			if (rng_mask & (1 << j)) {
				n_count = 0;
				for (int i = 0; i < 11; i++) {
					TestRandomDouble(TEST_RANDOM_DOUBLE,
						pow(10.0, i - 5), j);
					n_count++;
				}
			}
	printf("Double in ranges from 0.00001 to 100000.0 using RandomDouble(range):\n");
	ReportAverageOpsPerSecond(TEST_RANDOM_DOUBLE, rng_mask, n_count);
	}

	for (int i = 0; i < NU_RNGS; i++)
	        total_M_ops_per_sec[i] = 0;
	if (test_mask & MASK_DOUBLE_LP) {
	int n_count;
		for (int j = 0; j < NU_RNGS; j++)
			if (rng_mask & (1 << j)) {
				n_count = 0;
				for (int i = 0; i < 11; i++) {
					TestRandomDouble(TEST_RANDOM_DOUBLE_LP,
						pow(10.0, i - 5), j);
					n_count++;
				}
			}
	printf("Double in ranges from 0.00001 to 100000.0 using RandomDoubleLP(range):\n");
	ReportAverageOpsPerSecond(TEST_RANDOM_DOUBLE_LP, rng_mask, n_count);
	}

	for (int i = 0; i < NU_RNGS; i++)
	        total_M_ops_per_sec[i] = 0;
	if (test_mask & MASK_DOUBLE_HP) {
	int n_count;
		for (int j = 0; j < NU_RNGS; j++)
			if (rng_mask & (1 << j)) {
				n_count = 0;
				for (int i = 0; i < 11; i++) {
					TestRandomDouble(TEST_RANDOM_DOUBLE_HP,
						pow(10.0, i - 5), j);
					n_count++;
				}
			}
	printf("Double in ranges from 0.00001 to 100000.0 using RandomDoubleHP(range):\n");
	ReportAverageOpsPerSecond(TEST_RANDOM_DOUBLE_HP, rng_mask, n_count);
	}

	exit(0);
}
