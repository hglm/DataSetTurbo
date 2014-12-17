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
#include <dstVectorMathSIMD.h>
#include <dstMemory.h>


typedef uint64_t (*TestFunc)(dstThreadedTimeout *tt);

class TestData {
public :
	const char *description;
	TestFunc test_func;
};

static uint64_t SIMDCalculateDotProductsNx1Vector4D(dstThreadedTimeout *tt);
static uint64_t NonSIMDCalculateDotProductsNx1Vector4D(dstThreadedTimeout *tt);
static uint64_t SIMDCalculateDotProductsNx1Vector3D(dstThreadedTimeout *tt);
static uint64_t NonSIMDCalculateDotProductsNx1Vector3D(dstThreadedTimeout *tt);
static uint64_t SIMDCalculateDotProductsNxNVector4D(dstThreadedTimeout *tt);
static uint64_t NonSIMDCalculateDotProductsNxNVector4D(dstThreadedTimeout *tt);
static uint64_t SIMDCalculateDotProductsNxNVector3D(dstThreadedTimeout *tt);
static uint64_t NonSIMDCalculateDotProductsNxNVector3D(dstThreadedTimeout *tt);

TestData test_data[] = {
	{
		"SIMD CalculateDotProductsWithConstantVector4D",
		SIMDCalculateDotProductsNx1Vector4D,
	},
	{
		"Regular CalculateDotProductsWithConstantVector4D",
		NonSIMDCalculateDotProductsNx1Vector4D,
	},
	{
		"SIMD CalculateDotProductsWithConstantVector3D",
		SIMDCalculateDotProductsNx1Vector3D,
	},
	{
		"Regular CalculateDotProductsWithConstantVector3D",
		NonSIMDCalculateDotProductsNx1Vector3D,
	},

	{
		"SIMD CalculateDotProductsVector4D",
		SIMDCalculateDotProductsNxNVector4D,
	},
	{
		"Regular CalculateDotProductsVector4D",
		NonSIMDCalculateDotProductsNxNVector4D,
	},
	{
		"SIMD CalculateDotProductsVector3D",
		SIMDCalculateDotProductsNxNVector3D
	},
	{
		"Regular CalculateDotProductsVector3D",
		NonSIMDCalculateDotProductsNxNVector3D
	},
};

#define NU_TESTS (sizeof(test_data) / sizeof(test_data[0]))
#define VECTOR_ARRAY_SIZE 1027
#define NU_CORRECTNESS_ITERATIONS 1000

Vector4D *vector4D_array[2];
Vector3D *vector3D_array[2];
float *dot_product_array[2];
dstRNG *rng;

// N x 1 dot products

static uint64_t SIMDCalculateDotProductsNx1Vector4D(dstThreadedTimeout *tt) {
	uint64_t count = 0;
	for (;;) {
		SIMDCalculateDotProductsWithConstantVector(VECTOR_ARRAY_SIZE,
			vector4D_array[0], vector4D_array[1][0], dot_product_array[0]);
		count += VECTOR_ARRAY_SIZE;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static void NonSIMDCalculateDotProductsWithConstantVector(int n, const Vector4D *v1,
const Vector4D& v2, float *dot) {
	int i = 0;
	for (i = 0; i + 3 < n; i += 4) {
		dot[i] = Dot(v1[i], v2);
		dot[i + 1] = Dot(v1[i + 1], v2);
		dot[i + 2] = Dot(v1[i + 2], v2);
		dot[i + 3] = Dot(v1[i + 3], v2);
	}
	for (; i < n; i++)
		dot[i] = Dot(v1[i], v2);
}


static uint64_t NonSIMDCalculateDotProductsNx1Vector4D(dstThreadedTimeout *tt) {
	uint64_t count = 0;
	for (;;) {
		NonSIMDCalculateDotProductsWithConstantVector(VECTOR_ARRAY_SIZE,
			vector4D_array[0], vector4D_array[1][0], dot_product_array[0]);
		count += VECTOR_ARRAY_SIZE;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static uint64_t SIMDCalculateDotProductsNx1Vector3D(dstThreadedTimeout *tt) {
	uint64_t count = 0;
	for (;;) {
		SIMDCalculateDotProductsWithConstantVector(VECTOR_ARRAY_SIZE,
			vector3D_array[0], vector3D_array[1][0], dot_product_array[0]);
		count += VECTOR_ARRAY_SIZE;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static void NonSIMDCalculateDotProductsWithConstantVector(int n, const Vector3D *v1,
const Vector3D& v2, float *dot) {
	int i = 0;
	for (i = 0; i + 3 < n; i += 4) {
		dot[i] = Dot(v1[i], v2);
		dot[i + 1] = Dot(v1[i + 1], v2);
		dot[i + 2] = Dot(v1[i + 2], v2);
		dot[i + 3] = Dot(v1[i + 3], v2);
	}
	for (; i < n; i++)
		dot[i] = Dot(v1[i], v2);
}

static uint64_t NonSIMDCalculateDotProductsNx1Vector3D(dstThreadedTimeout *tt) {
	uint64_t count = 0;
	for (;;) {
		NonSIMDCalculateDotProductsWithConstantVector(VECTOR_ARRAY_SIZE,
			vector3D_array[0], vector3D_array[1][0], dot_product_array[0]);
		count += VECTOR_ARRAY_SIZE;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

// N x N dot products

static uint64_t SIMDCalculateDotProductsNxNVector4D(dstThreadedTimeout *tt) {
	uint64_t count = 0;
	for (;;) {
		CalculateDotProducts(VECTOR_ARRAY_SIZE,
			vector4D_array[0], vector4D_array[1], dot_product_array[0]);
		count += VECTOR_ARRAY_SIZE;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static void NonSIMDCalculateDotProducts(int n, const Vector4D * __restrict v1,
const Vector4D * __restrict v2, float * __restrict dot ) {
    int i = 0;
    for (; i + 3 < n; i += 4) {
        dot[i] = Dot(v1[i], v2[i]);
        dot[i + 1] = Dot(v1[i + 1], v2[i + 1]);
        dot[i + 2] = Dot(v1[i + 2], v2[i + 2]);
        dot[i + 3] = Dot(v1[i + 3], v2[i + 3]);
    }
    for (; i < n; i++)
        dot[i] = Dot(v1[i], v2[i]);
}

static uint64_t NonSIMDCalculateDotProductsNxNVector4D(dstThreadedTimeout *tt) {
	uint64_t count = 0;
	for (;;) {
		NonSIMDCalculateDotProducts(VECTOR_ARRAY_SIZE,
			vector4D_array[0], vector4D_array[1], dot_product_array[0]);
		count += VECTOR_ARRAY_SIZE;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static uint64_t SIMDCalculateDotProductsNxNVector3D(dstThreadedTimeout *tt) {
	uint64_t count = 0;
	for (;;) {
		CalculateDotProducts(VECTOR_ARRAY_SIZE,
			vector3D_array[0], vector3D_array[1], dot_product_array[0]);
		count += VECTOR_ARRAY_SIZE;
		if (tt->StopSignalled())
			break;
	}
	return count;
}

static void NonSIMDCalculateDotProducts(int n, const Vector3D * __restrict v1,
const Vector3D * __restrict v2, float * __restrict dot ) {
    int i = 0;
    for (; i + 3 < n; i += 4) {
        dot[i] = Dot(v1[i], v2[i]);
        dot[i + 1] = Dot(v1[i + 1], v2[i + 1]);
        dot[i + 2] = Dot(v1[i + 2], v2[i + 2]);
        dot[i + 3] = Dot(v1[i + 3], v2[i + 3]);
    }
    for (; i < n; i++)
        dot[i] = Dot(v1[i], v2[i]);
}

static uint64_t NonSIMDCalculateDotProductsNxNVector3D(dstThreadedTimeout *tt) {
	uint64_t count = 0;
	for (;;) {
		NonSIMDCalculateDotProducts(VECTOR_ARRAY_SIZE,
			vector3D_array[0], vector3D_array[1], dot_product_array[0]);
		count += VECTOR_ARRAY_SIZE;
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
	vector4D_array[0] = dstNewAligned <Vector4D>(VECTOR_ARRAY_SIZE, 16);
	vector4D_array[1] = dstNewAligned <Vector4D>(VECTOR_ARRAY_SIZE, 16);
	vector3D_array[0] = dstNewAligned <Vector3D>(VECTOR_ARRAY_SIZE, 16);
	vector3D_array[1] = dstNewAligned <Vector3D>(VECTOR_ARRAY_SIZE, 16);
	dot_product_array[0] = dstNewAligned <float>(VECTOR_ARRAY_SIZE, 16);
	dot_product_array[1] = dstNewAligned <float>(VECTOR_ARRAY_SIZE, 16);

        printf("Size of Vector3D: %u\n", (unsigned int)sizeof(Vector3D));

	rng = dstGetDefaultRNG();
	SetRandomVector4DArrays();
	SetRandomVector3DArrays();
	
	printf("Correctness of SIMD versions compared to non-SIMD:\n");
	double deviation, avg_deviation, max_deviation;

	deviation = 0.0d;
	max_deviation = 0.0d;
	for (int i = 0; i < NU_CORRECTNESS_ITERATIONS; i++) {
		SetRandomVector4DArrays();
		SIMDCalculateDotProductsWithConstantVector(VECTOR_ARRAY_SIZE,
			vector4D_array[0], vector4D_array[1][0], dot_product_array[0]);
		NonSIMDCalculateDotProductsWithConstantVector(VECTOR_ARRAY_SIZE, vector4D_array[0],
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
		SIMDCalculateDotProductsWithConstantVector(VECTOR_ARRAY_SIZE,
			vector3D_array[0], vector3D_array[1][0], dot_product_array[0]);
		NonSIMDCalculateDotProductsWithConstantVector(VECTOR_ARRAY_SIZE,
			vector3D_array[0], vector3D_array[1][0], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
	}
	avg_deviation = deviation / NU_CORRECTNESS_ITERATIONS;
        printf("CalculateDotProductsWithConstantVector3D: average deviation = %lE (%s)\n",
		avg_deviation, CorrectString(avg_deviation));

	deviation = 0.0d;
	for (int i = 0; i < NU_CORRECTNESS_ITERATIONS; i++) {
		SetRandomVector4DArrays();
		CalculateDotProducts(VECTOR_ARRAY_SIZE, vector4D_array[0], vector4D_array[1],
			dot_product_array[0]);
		NonSIMDCalculateDotProducts(VECTOR_ARRAY_SIZE, vector4D_array[0],
			vector4D_array[1], dot_product_array[1]);
		deviation += DotProductArraysDeviation();
	}
	avg_deviation = deviation / NU_CORRECTNESS_ITERATIONS;
        printf("CalculateDotProductsVector4D: average deviation = %lE (%s)\n", avg_deviation,
		CorrectString(avg_deviation));

	deviation = 0.0d;
	for (int i = 0; i < NU_CORRECTNESS_ITERATIONS; i++) {
		SetRandomVector3DArrays();
		CalculateDotProducts(VECTOR_ARRAY_SIZE, vector3D_array[0], vector3D_array[1],
			dot_product_array[0]);
		NonSIMDCalculateDotProducts(VECTOR_ARRAY_SIZE, vector3D_array[0],
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

