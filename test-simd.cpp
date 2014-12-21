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

#include <dstDynamicArray.h>
#include <dstTimer.h>
#include <dstMemory.h>
#include <dstVectorMath.h>


typedef uint64_t (*TestFunc)(dstThreadedTimeout *tt);

class TestData {
public :
	const char *description;
	TestFunc test_func;
};

static uint64_t TestSIMDCalculateDotProductsNx1Vector4D(dstThreadedTimeout *tt);
static uint64_t TestNonSIMDCalculateDotProductsNx1Vector4D(dstThreadedTimeout *tt);
static uint64_t TestSIMDCalculateDotProductsNx1Vector3D(dstThreadedTimeout *tt);
static uint64_t TestNonSIMDCalculateDotProductsNx1Vector3D(dstThreadedTimeout *tt);
static uint64_t TestSIMDCalculateDotProductsNxNVector4D(dstThreadedTimeout *tt);
static uint64_t TestNonSIMDCalculateDotProductsNxNVector4D(dstThreadedTimeout *tt);
static uint64_t TestSIMDCalculateDotProductsNxNVector3D(dstThreadedTimeout *tt);
static uint64_t TestNonSIMDCalculateDotProductsNxNVector3D(dstThreadedTimeout *tt);

TestData test_data[] = {
	{
		"SIMD dstCalculateDotProductsNx1Vector4D",
		TestSIMDCalculateDotProductsNx1Vector4D,
	},
	{
		"Non-SIMD dstCalculateDotProductsNx1Vector4D",
		TestNonSIMDCalculateDotProductsNx1Vector4D,
	},
	{
		"SIMD dstCalculateDotProductsNx1Vector3D",
		TestSIMDCalculateDotProductsNx1Vector3D,
	},
	{
		"Non-SIMD dstCalculateDotProductsNx1Vector3D",
		TestNonSIMDCalculateDotProductsNx1Vector3D,
	},

	{
		"SIMD dstCalculateDotProductsVector4D",
		TestSIMDCalculateDotProductsNxNVector4D,
	},
	{
		"Non-SIMD dstCalculateDotProductsVector4D",
		TestNonSIMDCalculateDotProductsNxNVector4D,
	},
	{
		"SIMD dstCalculateDotProductsVector3D",
		TestSIMDCalculateDotProductsNxNVector3D
	},
	{
		"Non-SIMD dstCalculateDotProductsVector3D",
		TestNonSIMDCalculateDotProductsNxNVector3D
	},
};

#define NU_TESTS (sizeof(test_data) / sizeof(test_data[0]))
#define VECTOR_ARRAY_SIZE 1039
#define NU_CORRECTNESS_ITERATIONS 1000

Vector4D *vector4D_array[2];
Vector3D *vector3D_array[2];
float *dot_product_array[2];
dstRNG *rng;
int simd_type;

static void TypeSizeReport() {
	Vector3DPadded V;
        printf("sizeof(Vector3D): %u\n", (unsigned int)sizeof(Vector3D));
        printf("sizeof(Point3D): %u\n", (unsigned int)sizeof(Vector3D));
        printf("sizeof(Vector3DPadded): %u\n", (unsigned int)sizeof(Vector3DPadded));
        printf("sizeof(Point3DPadded): %u\n", (unsigned int)sizeof(Point3DPadded));
}

// N x 1 dot products

static uint64_t TestCalculateDotProductsNx1Vector4D(dstThreadedTimeout *tt) {
	uint64_t count = 0;
	for (;;) {
		dstCalculateDotProductsNx1(VECTOR_ARRAY_SIZE,
			vector4D_array[0], vector4D_array[1][0], dot_product_array[0]);
		count += VECTOR_ARRAY_SIZE;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static uint64_t TestSIMDCalculateDotProductsNx1Vector4D(dstThreadedTimeout *tt) {
	dstSetSIMDType(simd_type);
	return TestCalculateDotProductsNx1Vector4D(tt);
}

static uint64_t TestNonSIMDCalculateDotProductsNx1Vector4D(dstThreadedTimeout *tt) {
	dstSetSIMDType(DST_SIMD_NONE);
	return TestCalculateDotProductsNx1Vector4D(tt);
}

static uint64_t TestCalculateDotProductsNx1Vector3D(dstThreadedTimeout *tt) {
	uint64_t count = 0;
	for (;;) {
		dstCalculateDotProductsNx1(VECTOR_ARRAY_SIZE,
			vector3D_array[0], vector3D_array[1][0], dot_product_array[0]);
		count += VECTOR_ARRAY_SIZE;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static uint64_t TestSIMDCalculateDotProductsNx1Vector3D(dstThreadedTimeout *tt) {
	dstSetSIMDType(simd_type);
	return TestCalculateDotProductsNx1Vector3D(tt);
}

static uint64_t TestNonSIMDCalculateDotProductsNx1Vector3D(dstThreadedTimeout *tt) {
	dstSetSIMDType(DST_SIMD_NONE);
	return TestCalculateDotProductsNx1Vector3D(tt);
}

// N x N dot products

static uint64_t TestCalculateDotProductsNxNVector4D(dstThreadedTimeout *tt) {
	uint64_t count = 0;
	for (;;) {
		dstCalculateDotProductsNxN(VECTOR_ARRAY_SIZE,
			vector4D_array[0], vector4D_array[1], dot_product_array[0]);
		count += VECTOR_ARRAY_SIZE;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static uint64_t TestSIMDCalculateDotProductsNxNVector4D(dstThreadedTimeout *tt) {
	dstSetSIMDType(simd_type);
	return TestCalculateDotProductsNxNVector4D(tt);
}

static uint64_t TestNonSIMDCalculateDotProductsNxNVector4D(dstThreadedTimeout *tt) {
	dstSetSIMDType(DST_SIMD_NONE);
	return TestCalculateDotProductsNxNVector4D(tt);
}

static uint64_t TestCalculateDotProductsNxNVector3D(dstThreadedTimeout *tt) {
	uint64_t count = 0;
	for (;;) {
		dstCalculateDotProductsNxN(VECTOR_ARRAY_SIZE,
			vector3D_array[0], vector3D_array[1], dot_product_array[0]);
		count += VECTOR_ARRAY_SIZE;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static uint64_t TestSIMDCalculateDotProductsNxNVector3D(dstThreadedTimeout *tt) {
	dstSetSIMDType(simd_type);
	return TestCalculateDotProductsNxNVector3D(tt);
}

static uint64_t TestNonSIMDCalculateDotProductsNxNVector3D(dstThreadedTimeout *tt) {
	dstSetSIMDType(DST_SIMD_NONE);
	return TestCalculateDotProductsNxNVector3D(tt);
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
	for (int i = 0; i < VECTOR_ARRAY_SIZE; i++) {
		vector4D_array[0][i] = RandomVector4D();
		vector4D_array[1][i] = RandomVector4D();
	}
}

static void SetRandomVector3DArrays() {
	for (int i = 0; i < VECTOR_ARRAY_SIZE; i++) {
		vector3D_array[0][i] = RandomVector3D();
		vector3D_array[1][i] = RandomVector3D();
	}
}

static float DotProductArraysDeviation() {
	double deviation = 0.0f;
	for (int i = 0; i < VECTOR_ARRAY_SIZE; i++)
		deviation += fabs(dot_product_array[1][i] - dot_product_array[0][i]);
	return deviation / VECTOR_ARRAY_SIZE;
}

static float DotProductArraysMaxDeviation() {
	double max_deviation = 0.0d;
	for (int i = 0; i < VECTOR_ARRAY_SIZE; i++)
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

int main(int argc, char *argv[]) {
	dstInit();

	vector4D_array[0] = dstNewAligned <Vector4D>(VECTOR_ARRAY_SIZE, 16);
	vector4D_array[1] = dstNewAligned <Vector4D>(VECTOR_ARRAY_SIZE, 16);
	vector3D_array[0] = dstNewAligned <Vector3D>(VECTOR_ARRAY_SIZE, 16);
	vector3D_array[1] = dstNewAligned <Vector3D>(VECTOR_ARRAY_SIZE, 16);
	dot_product_array[0] = dstNewAligned <float>(VECTOR_ARRAY_SIZE, 16);
	dot_product_array[1] = dstNewAligned <float>(VECTOR_ARRAY_SIZE, 16);

	TypeSizeReport();
	simd_type = dstGetSIMDType();
	printf("SIMD type: %s\n", dstGetSIMDTypeString(simd_type));

	rng = dstGetDefaultRNG();
	SetRandomVector4DArrays();
	SetRandomVector3DArrays();
	
	printf("Correctness of SIMD versions compared to non-SIMD:\n");
	double deviation, avg_deviation, max_deviation;


	deviation = 0.0d;
	max_deviation = 0.0d;
	for (int i = 0; i < NU_CORRECTNESS_ITERATIONS; i++) {
		SetRandomVector4DArrays();
		dstSetSIMDType(simd_type);
		dstCalculateDotProductsNx1(VECTOR_ARRAY_SIZE,
			vector4D_array[0], vector4D_array[1][0], dot_product_array[0]);
		dstSetSIMDType(DST_SIMD_NONE);
		dstCalculateDotProductsNx1(VECTOR_ARRAY_SIZE, vector4D_array[0],
			vector4D_array[1][0], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
		max_deviation = maxd(max_deviation, DotProductArraysMaxDeviation());
	}
	avg_deviation = deviation / NU_CORRECTNESS_ITERATIONS;
        printf("CalculateDotProductsWithConstantVector4D: average deviation = %lE (%s), "
		"maximum deviation %lE (%s)\n",
		avg_deviation, CorrectString(avg_deviation),
		max_deviation, CorrectString(max_deviation));

	deviation = 0.0d;
	for (int i = 0; i < NU_CORRECTNESS_ITERATIONS; i++) {
		SetRandomVector3DArrays();
		dstSetSIMDType(simd_type);
		dstCalculateDotProductsNx1(VECTOR_ARRAY_SIZE,
			vector3D_array[0], vector3D_array[1][0], dot_product_array[0]);
		dstSetSIMDType(DST_SIMD_NONE);
		dstCalculateDotProductsNx1(VECTOR_ARRAY_SIZE,
			vector3D_array[0], vector3D_array[1][0], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
	}
	avg_deviation = deviation / NU_CORRECTNESS_ITERATIONS;
        printf("CalculateDotProductsWithConstantVector3D: average deviation = %lE (%s)\n",
		avg_deviation, CorrectString(avg_deviation));

	deviation = 0.0d;
	for (int i = 0; i < NU_CORRECTNESS_ITERATIONS; i++) {
		SetRandomVector4DArrays();
		dstSetSIMDType(simd_type);
		dstCalculateDotProductsNxN(VECTOR_ARRAY_SIZE, vector4D_array[0], vector4D_array[1],
			dot_product_array[0]);
		dstSetSIMDType(DST_SIMD_NONE);
		dstCalculateDotProductsNxN(VECTOR_ARRAY_SIZE, vector4D_array[0],
			vector4D_array[1], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
	}
	avg_deviation = deviation / NU_CORRECTNESS_ITERATIONS;
        printf("CalculateDotProductsVector4D: average deviation = %lE (%s)\n", avg_deviation,
		CorrectString(avg_deviation));

	deviation = 0.0d;
	for (int i = 0; i < NU_CORRECTNESS_ITERATIONS; i++) {
		SetRandomVector3DArrays();
		dstSetSIMDType(simd_type);
		dstCalculateDotProductsNxN(VECTOR_ARRAY_SIZE, vector3D_array[0], vector3D_array[1],
			dot_product_array[0]);
		dstSetSIMDType(DST_SIMD_NONE);
		dstCalculateDotProductsNxN(VECTOR_ARRAY_SIZE, vector3D_array[0],
			vector3D_array[1], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
	}
	avg_deviation = deviation / NU_CORRECTNESS_ITERATIONS;
        printf("CalculateDotProductsVector3D: average deviation = %lE (%s)\n", avg_deviation,
		CorrectString(avg_deviation));

	float timeout_secs = 1.0f;
	dstTimer timer;

	for (int i = 0; i < NU_TESTS; i++) {
		dstThreadedTimeout *tt;
		tt = new dstThreadedTimeout();
		tt->Start((uint64_t)(timeout_secs * 1000000));
		timer.Start();

		uint64_t count = test_data[i].test_func(tt);

		double elapsed_time = timer.Elapsed();
		double rate = count / elapsed_time;
		printf("Test: %s  Rate: %.3fM ops/sec\n", test_data[i].description,
			rate / 1000000);
		delete tt;
	}
}

