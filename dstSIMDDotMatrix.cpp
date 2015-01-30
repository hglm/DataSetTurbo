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

// dstSIMD library functions for matrix and dot products, mostly implemented using
// inline functions defined in dstSIMD.h (more specially dstSIMDMatrix.h an dstSIMDDot.h)

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "dstSIMD.h"
#include "dstSIMDDot.h"
#include "dstSIMDMatrix.h"
#include "dstMisc.h"

#ifdef DST_SIMD_MODE_STREAM
// Support multi-threading in streaming store versions of SIMD functions.
#define DST_MULTI_THREADING
#else
// Support multi-threading also in non-streaming store versions of SIMD functions.
// This adds some overhead because the non-streaming versions are called for small
// sizes also (for which multi-threading is normally not used, but a flags check/
// cost calculation is performed).
#define DST_MULTI_THREADING
#endif

#ifdef DST_MULTI_THREADING
#include "dstThread.h"
#endif

#ifndef DST_NO_SIMD

// Matrix multiplication.

void SIMD_FUNC(dstMatrixMultiply4x4CM)(const float * DST_RESTRICT m1,
const float * DST_RESTRICT m2, float * DST_RESTRICT m3) {
    dstInlineMatrixMultiply4x4CM(m1, m2, m3);
}

void SIMD_FUNC(dstMatrixMultiply4x3RM)(const float * DST_RESTRICT m1,
const float * DST_RESTRICT m2, float * DST_RESTRICT m3) {
    dstInlineMatrixMultiply4x3RM(m1, m2, m3);
}

void SIMD_FUNC(dstMatrixMultiply4x4CM4x3RM)(const float * DST_RESTRICT m1,
const float * DST_RESTRICT m2, float * DST_RESTRICT m3) {
    dstInlineMatrixMultiply4x4CM4x3RM(m1, m2, m3);
}


// Multiply matrix with vectors.

void SIMD_FUNC(dstMatrixMultiplyVectors1x4M4x4CMV4)(const float * DST_RESTRICT m,
const float * DST_RESTRICT v, float * DST_RESTRICT v_result) {
	dstInlineMatrixMultiplyVectors1x4M4x4CMV4(m, v, v_result);
}

void SIMD_FUNC(dstMatrixMultiplyVectors1xNM4x4CMV4)(int n, const float * DST_RESTRICT m,
const float * DST_RESTRICT v, float * DST_RESTRICT v_result) {
	dstInlineMatrixMultiplyVectors1xNM4x4CMV4(n, m, v, v_result);
}

void SIMD_FUNC(dstMatrixMultiplyVectors1x4M4x4CMP3P)(const float * DST_RESTRICT m,
const float * DST_RESTRICT v, float * DST_RESTRICT v_result) {
	dstInlineMatrixMultiplyVectors1x4M4x4CMP3P(m, v, v_result);
}

void SIMD_FUNC(dstMatrixMultiplyVectors1xNM4x4CMP3P)(int n, const float * DST_RESTRICT m,
const float * DST_RESTRICT v, float * DST_RESTRICT v_result) {
	dstInlineMatrixMultiplyVectors1xNM4x4CMP3P(n, m, v, v_result);
}

// Dot products (4x4).

void SIMD_FUNC(dstCalculateFourDotProductsV4)(
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, __simd128_float& m_result) {
	dstInlineCalculateDotProducts4x4V4(f1, f2, m_result);
}

void SIMD_FUNC(dstCalculateDotProducts4x4V3P)(const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, __simd128_float& m_result) {
	dstInlineCalculateDotProducts4x4V3P(f1, f2, m_result);
}

void SIMD_FUNC(dstCalculateDotProducts4x4V3)(const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, __simd128_float& m_result) {
	dstInlineCalculateDotProducts4x4V3(f1, f2, m_result);
}

#ifdef DST_MULTI_THREADING

typedef void (*dstDotProductFuncType)(int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float * DST_RESTRICT dot);


static void dstCalculateDotProductsThread(dstThreadData *thread_data) {
	void **user_data = (void **)thread_data->user_data;
	float *f1 = (float *)user_data[0];
	float *f2 = (float *)user_data[1];
	float *dot = (float *)user_data[2];
	dstDotProductFuncType dot_product_func = (dstDotProductFuncType)user_data[3];
	uint32_t alignment_and_sizes = (uint32_t)(uint64_t)user_data[4];
	uint32_t size_f1 = (alignment_and_sizes >> 8) & 0xFF;
	uint32_t size_f2 = (alignment_and_sizes >> 16) & 0xFF;
	f1 += thread_data->subdivision.start_index * size_f1;
	f2 += thread_data->subdivision.start_index * size_f2;
	dot += thread_data->subdivision.start_index;
//	printf("Task begin start_index = %d, n = %d, sizes %d/%d\n",
//		thread_data->subdivision.start_index,
//		thread_data->subdivision.nu_elements,
//		size_f1, size_f2);
//	printf("Creation time = %.5lf\n", (double)thread_data->creation_time / 1000000.0d); 
//	fflush(stdout);
	dot_product_func(thread_data->subdivision.nu_elements, f1, f2, dot);
//	printf("Task end, start_index = %d\n", thread_data->subdivision.start_index);
//	fflush(stdout);
}


static void dstSubdivideDotProductFunction(int nu_threads,
const void (*dot_product_func)(int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float * DST_RESTRICT dot), uint32_t alignment_and_sizes, int n, const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	uint32_t alignment = alignment_and_sizes & 0xFF;
	dstTaskDivisionData division;
	division.size = n;
	division.nu_subdivisions = nu_threads;
	// Alignment in terms of number of elements.
	division.alignment = alignment;
//	printf("Number of threads hint: %d\n", nu_threads);
	void **user_data = (void **)malloc(sizeof(void *) * 5);
	user_data[0] = (void *)f1;
	user_data[1] = (void *)f2;
	user_data[2] = dot;
	user_data[3] = (void *)dot_product_func;
	user_data[4] = (void *)(uint64_t)alignment_and_sizes;
#define USE_TASK_GROUP
#ifdef USE_TASK_GROUP
	int group_index = dst_config.task_scheduler.AddSubdividedTaskGroup(0,
		dstCalculateDotProductsThread, (void *)user_data, division);
	dst_config.nu_tasks++;
	if (dst_config.nu_tasks >= dst_config.max_tasks) {
		// Wait for all tasks to finish.
		dst_config.task_scheduler.WaitUntilGroupFinished(group_index);
		dst_config.nu_tasks = 0;
	}
#else
	for (int i = 0; i < nu_threads; i++) {
		division.index = (uint32_t)i;
		dst_config.task_scheduler.AddTask(0, dstCalculateDotProductsThread,
			(void *)user_data, division);
	}
	dst_config.task_scheduler.WaitUntilFinished();
#endif
	free(user_data);
}

static DST_INLINE_ONLY bool dstDotProductFunctionMultiThreadCheck(const void (*dot_product_func)(
int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float * DST_RESTRICT dot), int cost, int alignment, int n, const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	if (dstCheckFlag(DST_FLAG_THREADING)) {
		int nu_threads = dstGetNumberOfThreadsHint(n, cost);
		if (nu_threads > 1) {
			dstSubdivideDotProductFunction(nu_threads, dot_product_func, alignment,
				n, f1, f2, dot);
			return true;
		}
	}
	return false;
}

#define DOT_PRODUCT_FUNC_MULTI_THREAD_CHECK(func, cost, alignment) \
	bool r = dstDotProductFunctionMultiThreadCheck(func, cost, alignment, n, f1, f2, dot); \
	if (r) \
		return;

#else

#define DOT_PRODUCT_FUNC_MULTI_THREAD_CHECK(func, cost, alignment)

#endif

// Create integer with information about alignment in terms of number of elements and
// size of each element of f1 and f2 in terms of floats.
#define ALIGNMENT_AND_SIZES(alignment, size_f1, size_f2) \
	((uint32_t)alignment | ((uint32_t)size_f1 << 8) | ((uint32_t)size_f2 << 16))

// Dot products (NxN).

#ifdef DST_MULTI_THREADING

static const void dstNonInlineCalculateDotProductsNxNV4(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNxNV4(n, f1, f2, dot);
}

#endif

void SIMD_FUNC(dstCalculateDotProductsNxNV4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	// Cost 64, alignment has to be four because of dot product stores.
	DOT_PRODUCT_FUNC_MULTI_THREAD_CHECK(dstNonInlineCalculateDotProductsNxNV4, 64,
		ALIGNMENT_AND_SIZES(4, 4, 4));
	dstInlineCalculateDotProductsNxNV4(n, f1, f2, dot);
}

#ifdef DST_MULTI_THREADING

static const void dstNonInlineCalculateDotProductsNxNV3P(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNxNV4(n, f1, f2, dot);
}

#endif

void SIMD_FUNC(dstCalculateDotProductsNxNV3P)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	DOT_PRODUCT_FUNC_MULTI_THREAD_CHECK(dstNonInlineCalculateDotProductsNxNV3P, 64,
		ALIGNMENT_AND_SIZES(4, 4, 4));
	dstInlineCalculateDotProductsNxNV3P(n, f1, f2, dot);
}

#ifdef DST_MULTI_THREADING

static const void dstNonInlineCalculateDotProductsNxNV3(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNxNV3(n, f1, f2, dot);
}

#endif

void SIMD_FUNC(dstCalculateDotProductsNxNV3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	DOT_PRODUCT_FUNC_MULTI_THREAD_CHECK(dstNonInlineCalculateDotProductsNxNV3, 64,
		ALIGNMENT_AND_SIZES(4, 3, 3));
	dstInlineCalculateDotProductsNxNV3(n, f1, f2, dot);
}

// Dot products (Nx1).

#ifdef DST_MULTI_THREADING

static const void dstNonInlineCalculateDotProductsNx1V4(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNx1V4(n, f1, f2, dot);
}

#endif

void SIMD_FUNC(dstCalculateDotProductsNx1V4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	DOT_PRODUCT_FUNC_MULTI_THREAD_CHECK(dstNonInlineCalculateDotProductsNx1V4, 48,
		ALIGNMENT_AND_SIZES(4, 4, 0));
	dstInlineCalculateDotProductsNx1V4(n, f1, f2, dot);
}

#ifdef DST_MULTI_THREADING

static const void dstNonInlineCalculateDotProductsNx1V3(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNx1V3(n, f1, f2, dot);
}

#endif

void SIMD_FUNC(dstCalculateDotProductsNx1V3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	DOT_PRODUCT_FUNC_MULTI_THREAD_CHECK(dstNonInlineCalculateDotProductsNx1V3, 48,
		ALIGNMENT_AND_SIZES(4, 3, 0));
	dstInlineCalculateDotProductsNx1V3(n, f1, f2, dot);
}

#ifdef DST_MULTI_THREADING

static const void dstNonInlineCalculateDotProductsNx1V3PV3(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNx1V3PV3(n, f1, f2, dot);
}

#endif

void SIMD_FUNC(dstCalculateDotProductsNx1V3PV3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	DOT_PRODUCT_FUNC_MULTI_THREAD_CHECK(dstNonInlineCalculateDotProductsNx1V3PV3, 48,
		ALIGNMENT_AND_SIZES(4, 4, 0));
	dstInlineCalculateDotProductsNx1V3PV3(n, f1, f2, dot);
}

#ifdef DST_MULTI_THREADING

static const void dstNonInlineCalculateDotProductsNx1P3PV4(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNx1P3PV4(n, f1, f2, dot);
}

#endif

void SIMD_FUNC(dstCalculateDotProductsNx1P3PV4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	DOT_PRODUCT_FUNC_MULTI_THREAD_CHECK(dstNonInlineCalculateDotProductsNx1P3PV4, 52,
		ALIGNMENT_AND_SIZES(4, 4, 0));
	dstInlineCalculateDotProductsNx1P3PV4(n, f1, f2, dot);
}

#ifdef DST_MULTI_THREADING

static const void dstNonInlineCalculateDotProductsNx1P3V4(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNx1P3V4(n, f1, f2, dot);
}

#endif

void SIMD_FUNC(dstCalculateDotProductsNx1P3V4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	DOT_PRODUCT_FUNC_MULTI_THREAD_CHECK(dstNonInlineCalculateDotProductsNx1P3V4, 52,
		ALIGNMENT_AND_SIZES(4, 3, 0));
	dstInlineCalculateDotProductsNx1P3V4(n, f1, f2, dot);
}

// Dot products and count negative dot products (Nx1).

void SIMD_FUNC(dstCalculateDotProductsAndCountNegativeNx1P3PV4)(
int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot,
int& DST_RESTRICT negative_count) {
	dstInlineCalculateDotProductsAndCountNegativeNx1P3PV4(n, f1, f2, dot, negative_count);
}

void SIMD_FUNC(dstCalculateDotProductsAndCountNegativeNx1P3V4)(
int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot,
int& DST_RESTRICT negative_count) {
	dstInlineCalculateDotProductsAndCountNegativeNx1P3V4(n, f1, f2, dot, negative_count);
}

// Determine the minimum and maximum dot product of an array of vertices with a
// given constant vector.

void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx1V3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float& DST_RESTRICT min_dot_product, float& DST_RESTRICT max_dot_product) {
	dstInlineCalculateMinAndMaxDotProductNx1V3(n, f1, f2,
		min_dot_product, max_dot_product);
}

void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx1V4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float& DST_RESTRICT min_dot_product, float& DST_RESTRICT max_dot_product) {
	dstInlineCalculateMinAndMaxDotProductNx1V4(n, f1, f2,
		min_dot_product, max_dot_product);
}

// Determine each minimum and maximum dot product of an array of vertices for each
// of three vectors.

void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product) {
	dstInlineCalculateMinAndMaxDotProductNx3V3(n, f1, f2,
		min_dot_product, max_dot_product);
}

void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V3PV3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product) {
	dstInlineCalculateMinAndMaxDotProductNx3V3PV3(n, f1, f2,
		min_dot_product, max_dot_product);
}

void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product) {
	dstInlineCalculateMinAndMaxDotProductNx3V4(n, f1, f2,
		min_dot_product, max_dot_product);
}

// Determine the indices within an array of vertices that have the minimum and
// maximum dot product with a given constant vector.

void SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax) {
	dstInlineGetIndicesWithMinAndMaxDotProductNx1V3(n, f1, f2, i_Pmin, i_Pmax);
}

void SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V3P)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax) {
	dstInlineGetIndicesWithMinAndMaxDotProductNx1V3P(n, f1, f2, i_Pmin, i_Pmax);
}

void SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax) {
	dstInlineGetIndicesWithMinAndMaxDotProductNx1V4(n, f1, f2, i_Pmin, i_Pmax);
}

#endif // !defined(DST_NO_SIMD)

