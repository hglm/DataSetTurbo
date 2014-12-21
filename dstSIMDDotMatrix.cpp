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

#ifndef DST_NO_SIMD

// Matrix multiplication.

void SIMD_FUNC(dstMatrixMultiply4x4CM)(const float * DST_RESTRICT m1,
const float * DST_RESTRICT m2, float * DST_RESTRICT m3) {
    dstInlineMatrixMultiply4x4CM(m1, m2, m3);
}

void SIMD_FUNC(dstMatrixMultiply4x4RM)(const float * DST_RESTRICT m1,
const float * DST_RESTRICT m2, float * DST_RESTRICT m3) {
    dstInlineMatrixMultiply4x4RM(m1, m2, m3);
}

void SIMD_FUNC(dstMatrixMultiply4x4CM_4x3RM)(const float * DST_RESTRICT m1,
const float * DST_RESTRICT m2, float * DST_RESTRICT m3) {
    dstInlineMatrixMultiply4x4CM_4x3RM(m1, m2, m3);
}

// Dot products (4x4).

void SIMD_FUNC(dstCalculateFourDotProductsV4)(
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, __simd128_float& m_result) {
	dstInlineCalculateFourDotProductsV4(f1, f2, m_result);
}

void SIMD_FUNC(dstCalculateFourDotProductsV3P)(const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, __simd128_float& m_result) {
	dstInlineCalculateFourDotProductsV3P(f1, f2, m_result);
}

void SIMD_FUNC(dstCalculateFourDotProductsV3)(const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, __simd128_float& m_result) {
	dstInlineCalculateFourDotProductsV3(f1, f2, m_result);
}

// Dot products (NxN).

void SIMD_FUNC(dstCalculateDotProductsNxNV4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNxNV4(n, f1, f2, dot);
}

void SIMD_FUNC(dstCalculateDotProductsNxNV3P)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNxNV3P(n, f1, f2, dot);
}

void SIMD_FUNC(dstCalculateDotProductsNxNV3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNxNV3(n, f1, f2, dot);
}

// Dot products (Nx1).

void SIMD_FUNC(dstCalculateDotProductsNx1V4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNx1V4(n, f1, f2, dot);
}

void SIMD_FUNC(dstCalculateDotProductsNx1V3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNx1V3(n, f1, f2, dot);
}

void SIMD_FUNC(dstCalculateDotProductsNx1V3PV3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNx1V3PV3(n, f1, f2, dot);
}

void SIMD_FUNC(dstCalculateDotProductsNx1P3PV4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstInlineCalculateDotProductsNx1P3PV4(n, f1, f2, dot);
}

void SIMD_FUNC(dstCalculateDotProductsNx1P3V4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
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

void SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax) {
	dstInlineGetIndicesWithMinAndMaxDotProductNx1V4(n, f1, f2, i_Pmin, i_Pmax);
}

#endif // !defined(DST_NO_SIMD)

