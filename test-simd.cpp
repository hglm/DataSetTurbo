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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>

#include <dstDynamicArray.h>
#include <dstRandom.h>
#include <dstTimer.h>
#include <dstMemory.h>
#include <dstVectorMath.h>


// Duration of each test in seconds.
#define TEST_DURATION 0.5f

typedef uint64_t (*TestFunc)(dstThreadedTimeout *tt, int test_mode);

class TestData {
public :
	const char *description;
	TestFunc test_func;
};

static uint64_t TestCalculateDotProductsNxNVector4D(dstThreadedTimeout *tt, int test_mode);
static uint64_t TestCalculateDotProductsNxNVector3DPadded(dstThreadedTimeout *tt, int test_mode);
static uint64_t TestCalculateDotProductsNxNVector3D(dstThreadedTimeout *tt, int test_mode);
static uint64_t TestCalculateDotProductsNx1Vector4D(dstThreadedTimeout *tt, int test_mode);
static uint64_t TestCalculateDotProductsNx1Vector3DPadded(dstThreadedTimeout *tt, int test_mode);
static uint64_t TestCalculateDotProductsNx1Vector3D(dstThreadedTimeout *tt, int test_mode);
static uint64_t TestCalculateDotProductsNx1Point3DVector4D(dstThreadedTimeout *tt, int test_mode);
static uint64_t TestCalculateDotProductsNx1Point3DPaddedVector4D(dstThreadedTimeout *tt, int test_mode);

TestData test_data[] = {
	{
		"SIMD dstCalculateDotProductsNxNVector4D",
		TestCalculateDotProductsNxNVector4D,
	},
	{
		"SIMD dstCalculateDotProductsNxNVector3DPadded",
		TestCalculateDotProductsNxNVector3DPadded
	},
	{
		"SIMD dstCalculateDotProductsNxNVector3D",
		TestCalculateDotProductsNxNVector3D
	},

	{
		"SIMD dstCalculateDotProductsNx1Vector4D",
		TestCalculateDotProductsNx1Vector4D,
	},
	{
		"SIMD dstCalculateDotProductsNx1Vector3DPadded",
		TestCalculateDotProductsNx1Vector3DPadded,
	},
	{
		"SIMD dstCalculateDotProductsNx1Vector3D",
		TestCalculateDotProductsNx1Vector3D,
	},
	{
		"SIMD dstCalculateDotProductsNx1Point3DVector4D",
		TestCalculateDotProductsNx1Point3DVector4D,
	},
	{
		"SIMD dstCalculateDotProductsNx1Point3DPaddedVector4D",
		TestCalculateDotProductsNx1Point3DPaddedVector4D,
	},
};

#define NU_TESTS (sizeof(test_data) / sizeof(test_data[0]))
#define DEFAULT_VECTOR_ARRAY_SIZE 1024

enum {
	TEST_MODE_NO_SIMD,
	TEST_MODE_SIMD_NO_STREAMING,
	TEST_MODE_SIMD_STREAMING
};

Vector4D *vector4D_array[2];
Vector3DPadded *vector3D_padded_array[2];
Vector3D *vector3D_array[2];
float *dot_product_array[2];
dstRNG *rng;
int simd_type;
int vector_array_size;
int fixed_nu_threads;

static void TypeSizeReport() {
	Vector3DPadded V;
        printf("sizeof(Vector3D): %u\n", (unsigned int)sizeof(Vector3D));
        printf("sizeof(Point3D): %u\n", (unsigned int)sizeof(Vector3D));
        printf("sizeof(Vector3DPadded): %u\n", (unsigned int)sizeof(Vector3DPadded));
        printf("sizeof(Point3DPadded): %u\n", (unsigned int)sizeof(Point3DPadded));
}

// N x N dot products

static uint64_t TestCalculateDotProductsNxNVector4D(dstThreadedTimeout *tt, int test_mode) {
	if (test_mode == TEST_MODE_NO_SIMD)
		dstSetSIMDType(DST_SIMD_NONE);
	else if (test_mode == TEST_MODE_SIMD_NO_STREAMING)
		dstSetNonStreamingSIMDType(simd_type);
	else
		dstSetStreamingSIMDType(simd_type);
	uint64_t count = 0;
	for (;;) {
		dstCalculateDotProductsNxN(vector_array_size,
			vector4D_array[0], vector4D_array[1], dot_product_array[0]);
		count += vector_array_size;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static uint64_t TestCalculateDotProductsNxNVector3DPadded(dstThreadedTimeout *tt, int test_mode) {
	if (test_mode == TEST_MODE_NO_SIMD)
		dstSetSIMDType(DST_SIMD_NONE);
	else if (test_mode == TEST_MODE_SIMD_NO_STREAMING)
		dstSetNonStreamingSIMDType(simd_type);
	else
		dstSetStreamingSIMDType(simd_type);
	uint64_t count = 0;
	for (;;) {
		dstCalculateDotProductsNxN(vector_array_size,
			vector3D_padded_array[0], vector3D_padded_array[1], dot_product_array[0]);
		count += vector_array_size;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static uint64_t TestCalculateDotProductsNxNVector3D(dstThreadedTimeout *tt, int test_mode) {
	if (test_mode == TEST_MODE_NO_SIMD)
		dstSetSIMDType(DST_SIMD_NONE);
	else if (test_mode == TEST_MODE_SIMD_NO_STREAMING)
		dstSetNonStreamingSIMDType(simd_type);
	else
		dstSetStreamingSIMDType(simd_type);
	uint64_t count = 0;
	for (;;) {
		dstCalculateDotProductsNxN(vector_array_size,
			vector3D_array[0], vector3D_array[1], dot_product_array[0]);
		count += vector_array_size;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

// N x 1 dot products

static uint64_t TestCalculateDotProductsNx1Vector4D(dstThreadedTimeout *tt, int test_mode) {
	if (test_mode == TEST_MODE_NO_SIMD)
		dstSetSIMDType(DST_SIMD_NONE);
	else if (test_mode == TEST_MODE_SIMD_NO_STREAMING)
		dstSetNonStreamingSIMDType(simd_type);
	else
		dstSetStreamingSIMDType(simd_type);
	uint64_t count = 0;
	for (;;) {
		dstCalculateDotProductsNx1(vector_array_size,
			vector4D_array[0], vector4D_array[1][0], dot_product_array[0]);
		count += vector_array_size;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static uint64_t TestCalculateDotProductsNx1Vector3DPadded(dstThreadedTimeout *tt, int test_mode) {
	if (test_mode == TEST_MODE_NO_SIMD)
		dstSetSIMDType(DST_SIMD_NONE);
	else if (test_mode == TEST_MODE_SIMD_NO_STREAMING)
		dstSetNonStreamingSIMDType(simd_type);
	else
		dstSetStreamingSIMDType(simd_type);
	uint64_t count = 0;
	for (;;) {
		dstCalculateDotProductsNx1(vector_array_size,
			vector3D_padded_array[0], vector3D_array[1][0], dot_product_array[0]);
		count += vector_array_size;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static uint64_t TestCalculateDotProductsNx1Vector3D(dstThreadedTimeout *tt, int test_mode) {
	if (test_mode == TEST_MODE_NO_SIMD)
		dstSetSIMDType(DST_SIMD_NONE);
	else if (test_mode == TEST_MODE_SIMD_NO_STREAMING)
		dstSetNonStreamingSIMDType(simd_type);
	else
		dstSetStreamingSIMDType(simd_type);
	uint64_t count = 0;
	for (;;) {
		dstCalculateDotProductsNx1(vector_array_size,
			vector3D_array[0], vector3D_array[1][0], dot_product_array[0]);
		count += vector_array_size;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static uint64_t TestCalculateDotProductsNx1Point3DVector4D(dstThreadedTimeout *tt, int test_mode) {
	if (test_mode == TEST_MODE_NO_SIMD)
		dstSetSIMDType(DST_SIMD_NONE);
	else if (test_mode == TEST_MODE_SIMD_NO_STREAMING)
		dstSetNonStreamingSIMDType(simd_type);
	else
		dstSetStreamingSIMDType(simd_type);
	uint64_t count = 0;
	for (;;) {
		dstCalculateDotProductsNx1(vector_array_size,
			(const Point3D *)vector3D_array[0], vector4D_array[1][0], dot_product_array[0]);
		count += vector_array_size;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static uint64_t TestCalculateDotProductsNx1Point3DPaddedVector4D(dstThreadedTimeout *tt, int test_mode) {
	if (test_mode == TEST_MODE_NO_SIMD)
		dstSetSIMDType(DST_SIMD_NONE);
	else if (test_mode == TEST_MODE_SIMD_NO_STREAMING)
		dstSetNonStreamingSIMDType(simd_type);
	else
		dstSetStreamingSIMDType(simd_type);
	uint64_t count = 0;
	for (;;) {
		dstCalculateDotProductsNx1(vector_array_size,
			(const Point3DPadded *)vector4D_array[0], vector4D_array[1][0], dot_product_array[0]);
		count += vector_array_size;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

Vector4D RandomVector4D() {
	Vector4D v;
	v.x = rng->RandomFloat(1.0f);
	v.y = rng->RandomFloat(1.0f);
	v.z = rng->RandomFloat(1.0f);
	v.w = rng->RandomFloat(1.0f);
	return v;
}

static Vector3D RandomVector3D() {
	Vector3D v;
	v.x = rng->RandomFloat(1.0f);
	v.y = rng->RandomFloat(1.0f);
	v.z = rng->RandomFloat(1.0f);
	return v;
}

static void SetRandomVector4DArrays() {
	for (int i = 0; i < vector_array_size; i++) {
		vector4D_array[0][i] = RandomVector4D();
		vector4D_array[1][i] = RandomVector4D();
	}
}

static void SetRandomVector3DPaddedArrays() {
	for (int i = 0; i < vector_array_size; i++) {
		vector3D_padded_array[0][i] = RandomVector3D();
		vector3D_padded_array[1][i] = RandomVector3D();
	}
}

static void SetRandomVector3DArrays() {
	for (int i = 0; i < vector_array_size; i++) {
		vector3D_array[0][i] = RandomVector3D();
		vector3D_array[1][i] = RandomVector3D();
	}
}

static float DotProductArraysDeviation() {
	double deviation = 0.0f;
	for (int i = 0; i < vector_array_size; i++)
		deviation += fabs(dot_product_array[1][i] - dot_product_array[0][i]);
	return deviation / vector_array_size;
}

static float DotProductArraysMaxDeviation() {
	double max_deviation = 0.0d;
	for (int i = 0; i < vector_array_size; i++)
		max_deviation = maxd(max_deviation,
			fabsf(dot_product_array[1][i] - dot_product_array[0][i]));
	return max_deviation;
}

static const char *CorrectString(double deviation) {
	if (deviation == 0.0d)
		return "100% correct";
	else if (deviation < 0.00001d)
		return "nearly 100% correct";
	else if (deviation < 0.001d)
		return "not 100% correct";
	else
		return "not correct";
}

static void ErrorMessage(const char *s) {
	printf("%s\n", s);
	exit(1);
}

static void Usage() {
	printf(
		"test-simd -- Test SIMD functionality of DataSetTurbo library\n\n"
		"Options:\n"
		"-n <number>  Size of vector arrays (number of elements).\n"
		"-t <number>  Use a fixed number of threads for multi-threading benchmarks.\n"
		"-h           Display this text.\n"
		);
	exit(0);
}

int main(int argc, char *argv[]) {
	vector_array_size = DEFAULT_VECTOR_ARRAY_SIZE;
	fixed_nu_threads = 0;

 	for (int i = 1; i < argc; i++) {
		// Options that take no arguments.
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'h' :
				Usage();
			}
		}
		// Options that take one argument.
		if (argv[i][0] == '-' && i + 1 < argc) {
			int n = atoi(argv[i + 1]);
			switch (argv[i][1]) {
			case 'n' :
				if (n > 0 && n <= (1 << 30))
					vector_array_size = n;
				else
					ErrorMessage("Invalid option argument (vector array size).");
				i++;
				continue;
			case 't' :
				if (n > 0 && n <= 128)
					fixed_nu_threads = n;
				else
					ErrorMessage("Invalid option argument (fixed number of threads).");
				i++;
				continue;
			}
		}
		if (argv[i][0] == '-')
			ErrorMessage("Invalid option.");
		ErrorMessage("Invalid command-line argument (only options allowed).");
	}

	dstInit();
	if (fixed_nu_threads > 0) {
		dstSetFixedNumberOfThreads(fixed_nu_threads);
		dstSetFlag(DST_FLAG_FIXED_NU_THREADS);
	}
#if 0
	// Allocate 1GB of address space, so that huge pages will be used.
	uint8_t *buffer = (uint8_t *)malloc (1024 * 1024 * 1024);
	uintptr_t step = 1024 * 1024 * 4;
	vector4D_array[0] = (Vector4D *)&buffer[0 * step];
	vector4D_array[1] = (Vector4D *)&buffer[1 * step];
	vector3D_array[0] = (Vector3D *)&buffer[2 * step];
	vector3D_array[1] = (Vector3D *)&buffer[3 * step];
	vector3D_padded_array[0] = (Vector3DPadded *)&buffer[4 * step];
	vector3D_padded_array[1] = (Vector3DPadded *)&buffer[5 * step];
	dot_product_array[0] = (float *)&buffer[6 * step]; 
	dot_product_array[1] = (float *)&buffer[7 * step]; 
#else
	int page_size = sysconf(_SC_PAGESIZE);
	vector4D_array[0] = dstNewAligned <Vector4D>(vector_array_size, page_size);
	vector4D_array[1] = dstNewAligned <Vector4D>(vector_array_size, page_size);
	vector3D_array[0] = dstNewAligned <Vector3D>(vector_array_size, page_size);
	vector3D_array[1] = dstNewAligned <Vector3D>(vector_array_size, page_size);
	vector3D_padded_array[0] = dstNewAligned <Vector3DPadded>(vector_array_size, page_size);
	vector3D_padded_array[1] = dstNewAligned <Vector3DPadded>(vector_array_size, page_size);
	dot_product_array[0] = dstNewAligned <float>(vector_array_size, page_size);
	dot_product_array[1] = dstNewAligned <float>(vector_array_size, page_size);
#endif

	TypeSizeReport();
	simd_type = dstGetSIMDType();
	printf("SIMD type: %s\n", dstGetSIMDTypeString(simd_type));

	rng = dstGetDefaultRNG();
	SetRandomVector4DArrays();
	SetRandomVector3DPaddedArrays();
	SetRandomVector3DArrays();
	
	printf("Correctness of SIMD versions compared to non-SIMD:\n");
	double deviation, avg_deviation, max_deviation;
	int nu_correctness_iterations = ceil(1000000.0f / vector_array_size);

	deviation = 0.0d;
	for (int i = 0; i < nu_correctness_iterations; i++) {
		SetRandomVector4DArrays();
		dstSetSIMDType(simd_type);
		dstCalculateDotProductsNxN(vector_array_size, vector4D_array[0], vector4D_array[1],
			dot_product_array[0]);
		dstSetSIMDType(DST_SIMD_NONE);
		dstCalculateDotProductsNxN(vector_array_size, vector4D_array[0],
			vector4D_array[1], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
	}
	avg_deviation = deviation / nu_correctness_iterations;
        printf("CalculateDotProductsNxNVector4D: average deviation = %lE (%s)\n", avg_deviation,
		CorrectString(avg_deviation));

	deviation = 0.0d;
	for (int i = 0; i < nu_correctness_iterations; i++) {
		SetRandomVector3DPaddedArrays();
		dstSetSIMDType(simd_type);
		dstCalculateDotProductsNxN(vector_array_size, vector3D_padded_array[0],
			vector3D_padded_array[1], dot_product_array[0]);
		dstSetSIMDType(DST_SIMD_NONE);
		dstCalculateDotProductsNxN(vector_array_size, vector3D_padded_array[0],
			vector3D_padded_array[1], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
	}
	avg_deviation = deviation / nu_correctness_iterations;
        printf("CalculateDotProductNxNsVector3DPadded: average deviation = %lE (%s)\n", avg_deviation,
		CorrectString(avg_deviation));

	deviation = 0.0d;
	for (int i = 0; i < nu_correctness_iterations; i++) {
		SetRandomVector3DArrays();
		dstSetSIMDType(simd_type);
		dstCalculateDotProductsNxN(vector_array_size, vector3D_array[0], vector3D_array[1],
			dot_product_array[0]);
		dstSetSIMDType(DST_SIMD_NONE);
		dstCalculateDotProductsNxN(vector_array_size, vector3D_array[0],
			vector3D_array[1], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
	}
	avg_deviation = deviation / nu_correctness_iterations;
        printf("CalculateDotProductsNxNVector3D: average deviation = %lE (%s)\n", avg_deviation,
		CorrectString(avg_deviation));

	deviation = 0.0d;
	max_deviation = 0.0d;
	for (int i = 0; i < nu_correctness_iterations; i++) {
		SetRandomVector4DArrays();
		dstSetSIMDType(simd_type);
		dstCalculateDotProductsNx1(vector_array_size,
			vector4D_array[0], vector4D_array[1][0], dot_product_array[0]);
		dstSetSIMDType(DST_SIMD_NONE);
		dstCalculateDotProductsNx1(vector_array_size, vector4D_array[0],
			vector4D_array[1][0], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
		max_deviation = maxd(max_deviation, DotProductArraysMaxDeviation());
	}
	avg_deviation = deviation / nu_correctness_iterations;
        printf("CalculateDotProductsNx1Vector4D: average deviation = %lE (%s), "
		"maximum deviation %lE (%s)\n",
		avg_deviation, CorrectString(avg_deviation),
		max_deviation, CorrectString(max_deviation));

	deviation = 0.0d;
	for (int i = 0; i < nu_correctness_iterations; i++) {
		SetRandomVector3DPaddedArrays();
		SetRandomVector3DArrays();
		dstSetSIMDType(simd_type);
		dstCalculateDotProductsNx1(vector_array_size,
			vector3D_padded_array[0], vector3D_array[1][0], dot_product_array[0]);
		dstSetSIMDType(DST_SIMD_NONE);
		dstCalculateDotProductsNx1(vector_array_size,
			vector3D_padded_array[0], vector3D_array[1][0], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
	}
	avg_deviation = deviation / nu_correctness_iterations;
        printf("CalculateDotProductsNx1Vector3DPadded: average deviation = %lE (%s)\n",
		avg_deviation, CorrectString(avg_deviation));

	deviation = 0.0d;
	for (int i = 0; i < nu_correctness_iterations; i++) {
		SetRandomVector3DArrays();
		dstSetSIMDType(simd_type);
		dstCalculateDotProductsNx1(vector_array_size,
			vector3D_array[0], vector3D_array[1][0], dot_product_array[0]);
		dstSetSIMDType(DST_SIMD_NONE);
		dstCalculateDotProductsNx1(vector_array_size,
			vector3D_array[0], vector3D_array[1][0], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
	}
	avg_deviation = deviation / nu_correctness_iterations;
        printf("CalculateDotProductsNx1Vector3D: average deviation = %lE (%s)\n",
		avg_deviation, CorrectString(avg_deviation));

	deviation = 0.0d;
	for (int i = 0; i < nu_correctness_iterations; i++) {
		SetRandomVector3DArrays();
		SetRandomVector4DArrays();
		dstSetSIMDType(simd_type);
		dstCalculateDotProductsNx1(vector_array_size,
			(const Point3D *)vector3D_array[0], vector4D_array[1][0], dot_product_array[0]);
		dstSetSIMDType(DST_SIMD_NONE);
		dstCalculateDotProductsNx1(vector_array_size,
			(const Point3D *)vector3D_array[0], vector4D_array[1][0], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
	}
	avg_deviation = deviation / nu_correctness_iterations;
        printf("CalculateDotProductsNx1Point3DVector4D: average deviation = %lE (%s)\n",
		avg_deviation, CorrectString(avg_deviation));

	deviation = 0.0d;
	for (int i = 0; i < nu_correctness_iterations; i++) {
		SetRandomVector3DPaddedArrays();
		SetRandomVector4DArrays();
		dstSetSIMDType(simd_type);
		dstCalculateDotProductsNx1(vector_array_size,
			(const Point3DPadded *)vector3D_padded_array[0], vector4D_array[1][0],
			 dot_product_array[0]);
		dstSetSIMDType(DST_SIMD_NONE);
		dstCalculateDotProductsNx1(vector_array_size,
			(const Point3DPadded *)vector3D_padded_array[0], vector4D_array[1][0],
			dot_product_array[1]);
		deviation += DotProductArraysDeviation();
	}
	avg_deviation = deviation / nu_correctness_iterations;
        printf("CalculateDotProductsNx1Point3DPaddedVector4D: average deviation = %lE (%s)\n",
		avg_deviation, CorrectString(avg_deviation));

	printf("Array size %d\n", vector_array_size);
	printf("Number of threads for multi-threading: ");
	if (fixed_nu_threads > 0)
		printf("%d (fixed)\n", fixed_nu_threads);
	else
		printf("Dynamic (depends on data size)\n");
	float timeout_secs = TEST_DURATION;
	dstTimer timer;

	for (uint32_t i = 0; i < NU_TESTS; i++) {
		dstThreadedTimeout *tt;
		tt = new dstThreadedTimeout();

		dstClearFlag(DST_FLAG_THREADING);

		// Warm-up for 0.1s.
		tt->Start((uint64_t)(timeout_secs * 100000));
		timer.Start();
		test_data[i].test_func(tt, TEST_MODE_NO_SIMD);

		tt->Start((uint64_t)(timeout_secs * 1000000));
		timer.Start();
		uint64_t count = test_data[i].test_func(tt, TEST_MODE_NO_SIMD);
		double elapsed_time = timer.Elapsed();
		double rate_non_simd = count / elapsed_time;

		// Warm-up for 0.1s.
		tt->Start((uint64_t)(timeout_secs * 100000));
		timer.Start();
		test_data[i].test_func(tt, TEST_MODE_SIMD_NO_STREAMING);

		tt->Start((uint64_t)(timeout_secs * 1000000));
		timer.Start();
		count = test_data[i].test_func(tt, TEST_MODE_SIMD_NO_STREAMING);
		elapsed_time = timer.Elapsed();
		double rate_non_streaming = count / elapsed_time;

		dstSetStreamingSIMDType(simd_type);

		// Warm-up for 0.1s.
		tt->Start((uint64_t)(timeout_secs * 100000));
		timer.Start();
		test_data[i].test_func(tt, TEST_MODE_SIMD_STREAMING);

		tt->Start((uint64_t)(timeout_secs * 1000000));
		timer.Start();
		count = test_data[i].test_func(tt, TEST_MODE_SIMD_STREAMING);
		elapsed_time = timer.Elapsed();
		double rate_streaming = count / elapsed_time;

		dstSetFlag(DST_FLAG_THREADING);

		// Warm-up for 0.1s.
		tt->Start((uint64_t)(timeout_secs * 100000));
		timer.Start();
		test_data[i].test_func(tt, TEST_MODE_SIMD_NO_STREAMING);

		tt->Start((uint64_t)(timeout_secs * 1000000));
		timer.Start();
		count = test_data[i].test_func(tt, TEST_MODE_SIMD_NO_STREAMING);
		elapsed_time = timer.Elapsed();
		double rate_non_streaming_threaded = count / elapsed_time;

		// Warm-up for 0.1s.
		tt->Start((uint64_t)(timeout_secs * 100000));
		timer.Start();
		test_data[i].test_func(tt, TEST_MODE_SIMD_STREAMING);

		tt->Start((uint64_t)(timeout_secs * 1000000));
		timer.Start();
		count = test_data[i].test_func(tt, TEST_MODE_SIMD_STREAMING);
		elapsed_time = timer.Elapsed();
		double rate_streaming_threaded = count / elapsed_time;

		double rate_non_simd_threaded = 0.0d;
		printf("Test: %s\n"
			"Single:  Non-SIMD: %8.3lfM  SIMD: %8.3lfM  Streaming: %8.3lfM\n"
			"Multi:   Non-SIMD: %8.3lfM  SIMD: %8.3lfM  Streaming: %8.3lfM\n",
			test_data[i].description, rate_non_simd / 1000000.0d, rate_non_streaming / 1000000.0d,
			rate_streaming / 1000000.0d, rate_non_simd_threaded / 1000000.0d,
			rate_non_streaming_threaded / 1000000.0d, rate_streaming_threaded / 1000000.0d);
		delete tt;
	}
}

