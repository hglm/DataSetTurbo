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

#ifndef __DST_SIMD_DOT_H__
#define __DST_SIMD_DOT_H__

#include "dstMath.h"

// Calculate dot products of four four-component float vectors stored at f1 and f2.
//
// void dstCalculateFourDotProductsV4(
//     const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, __simd128_float& result);
// inline void dstInlineCalculateFourDotProductsV4(
//     const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, __simd128_float& result);
//
// Calculate dot products of four three-component float vectors stored at f1 and f2. The
// vectors are assumed to be stored in 128-bit fields (the last four bytes are unused).
//
// void dstCalculateFourDotProductsV3P(const float * DST_RESTRICT f1,
//     const float * DST_RESTRICT f2, __simd128_float& result);
// inline void dstInlineCalculateFourDotProductsV3P(
//     const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, __simd128_float& result);
//
// Calculate dot products of four three-component float vectors stored at f1 and f2. The
// vectors are assumed to be stored consecutively in packed 12-byte format.
//
// void dstCalculateFourDotProductsV3(const float * DST_RESTRICT f1,
//     const float * DST_RESTRICT f2, __simd128_float& result);
// inline void dstInlineCalculateFourDotProductsV3(const float * DST_RESTRICT f1,
//     const float * DST_RESTRICT f2, __simd128_float& result);
//
// Calculate n dot products from one array of four vectors and one single vector,
// and store the result in an array of floats.
//
// void dstCalculateDotProductsNx1V4(int n,
//     const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);
// inline void dstInlineCalculateDotProductsNx1V4(int n,
//     const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);
//
// Calculate dot products from one array of vectors with three floats with one four-float
// vector.
// When vectors of three floats are stored in 16-byte aligned format,
// simd128_load_float can be used but the fourth element of the SIMD register
// needs to be initialized with 0.0f (v2 only).
//
// void dstCalculateDotProductsNx1V3PV4(int n,
//     const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);
// inline void dstInlineCalculateDotProductsNx1V3PV4(int n,
//     const float * DST_RESTRICT f1, const __simd128_float m_v2, float * DST_RESTRICT dot);
// inline void dstInlineCalculateDotProductsNx1V3PV4(int n,
//     const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);
//
// The array of vectors is an array of points with implicit w coordinate of 1.0f,
// and is stored aligned on 16-byte boundaries. The constant vector has four
// components.
//
// void dstCalculateDotProductsNx1P3PV4(int n,
//     const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);
// inline void dstInlineCalculateDotProductsNx1P3PV4(int n,
//     const float * DST_RESTRICT f1, const __simd128_float m_v2, float * DST_RESTRICT dot);
// inline void dstInlineCalculateDotProductsNx1P3PV4(int n,
//     const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot)
//
// The array of vectors is an array of points with implicit w coordinate of 1.0f,
// and is stored unaligned on 12-byte boundaries. The constant vector has four
// components.
//
// void dstCalculateDotProductsNx1P3V4(int n,
//     const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);
// inline void dstInlineCalculateDotProductsNx1P3V4(int n,
//     const float * DST_RESTRICT f1, const __simd128_float m_v2, float * DST_RESTRICT dot);
// DST_INLINE_ONLY void dstInlineCalculateDotProductsNx1P3V4(int n,
//     const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);
//
// Calculate n dot products from one array of n point vectors stored on 16-byte
// boundaries and one single four-element vector, store the result in an array of
// floats, and count the number of dot products that is smaller than zero.
//
// void dstCalculateDotProductsNx1P3PV4_and_count_negative_float(
//     int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot,
//     int& negative_count);
//
// inline void dstInlineCalculateDotProductsNx1P3PV4_and_count_negative_float(
//     int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot,
//     int& negative_count);
//
// Similar to the previous function, but the point vectors are packed at 12-byte
// boundaries.
//
// void dstCalculateDotProductsNx1P3V4_and_count_negative_float(
//     int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot,
//     int& negative_count);
// inline void dstInlineCalculateDotProductsNx1P3V4_and_count_negative_float(
//     int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot,
//     int& negative_count);

// Calculate dot products of four four-component float vectors stored at f1 and f2. Both f1 and
// f2 must be 16-byte aligned.

DST_API void SIMD_FUNC(dstCalculateDotProducts4x4V4)(
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, __simd128_float& result);

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x4V4(
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, __simd128_float& m_result) {
    // Loading and using transpose seems to be more efficient than set_float calls, which
    // expand to numerous instructions.
    __simd128_float m_v1_0 = simd128_load_float(&f1[0]);
    __simd128_float m_v1_1 = simd128_load_float(&f1[4]);
    __simd128_float m_v1_2 = simd128_load_float(&f1[8]);
    __simd128_float m_v1_3 = simd128_load_float(&f1[12]);
    __simd128_float m_v2_0 = simd128_load_float(&f2[0]);
    __simd128_float m_v1_x, m_v1_y, m_v1_z, m_v1_w;
    simd128_transpose4to4_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z, m_v1_w);
    __simd128_float m_v2_1 = simd128_load_float(&f2[4]);
    __simd128_float m_v2_2 = simd128_load_float(&f2[8]);
    __simd128_float m_v2_3 = simd128_load_float(&f2[12]);
    __simd128_float m_v2_x, m_v2_y, m_v2_z, m_v2_w;
    simd128_transpose4to4_float(m_v2_0, m_v2_1, m_v2_2, m_v2_3, m_v2_x, m_v2_y, m_v2_z, m_v2_w);
    m_result = simd128_four_dot_products_vector4_vertical_float(m_v1_x, m_v1_y, m_v1_z, m_v1_w,
        m_v2_x, m_v2_y, m_v2_z, m_v2_w);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x4V4(
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    __simd128_float m_result;
    dstInlineCalculateDotProducts4x4V4(f1, f2, m_result);
    simd128_store_float(dot, m_result);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts16x16V4(
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    dstInlineCalculateDotProducts4x4V4(&f1[0 * 16], &f2[0 * 16], &dot[0]);
    dstInlineCalculateDotProducts4x4V4(&f1[1 * 16], &f2[1 * 16], &dot[4]);
    dstInlineCalculateDotProducts4x4V4(&f1[2 * 16], &f2[2 * 16], &dot[8]);
    dstInlineCalculateDotProducts4x4V4(&f1[3 * 16], &f2[3 * 16], &dot[12]);
}

// Calculate n dot products from two arrays of n vectors with four components,
// and store the result in an array of floats. f1, f2 and dot must all be 16-bytes aligned.

DST_API void SIMD_FUNC(dstCalculateDotProductsNxNV4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);

static DST_INLINE_ONLY void dstInlineCalculateDotProductsNxNV4(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    int i = 0;
    for (; i + 15 < n; i += 16)
        dstInlineCalculateDotProducts16x16V4(&f1[i * 4], &f2[i * 4], &dot[i]);
    for (; i + 3 < n; i += 4)
        dstInlineCalculateDotProducts4x4V4(&f1[i * 4], &f2[i * 4], &dot[i]);
    for (; i < n; i++) {
        __simd128_float m_v1 = simd128_load_float(&f1[i * 4]);
        __simd128_float m_v2 = simd128_load_float(&f2[i * 4]);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
#ifdef __SSE3__
        __simd128_float m_result = simd128_horizontal_add4_float(m_dot);
#else
        __simd128_float m_shifted_float1 = simd128_shift_right_float(m_dot, 1);
        __simd128_float m_shifted_float2 = simd128_shift_right_float(m_dot, 2);
        __simd128_float m_shifted_float3 = simd128_shift_right_float(m_dot, 3);
	__simd128_float m_result = simd128_add1_float(
            simd128_add1_float(m_shifted_float1, m_shifted_float2),
	    simd128_add1_float(m_shifted_float3, m_dot));
#endif
        simd128_store_first_float(&dot[i], m_result);
    }
}
// Calculate dot products of four three-component float vectors stored at f1 and f2. The
// vectors are assumed to be stored padded in 128-bit fields (the last four bytes are unused).
// Both f1 and f2 must be 16-byte aligned.

DST_API void SIMD_FUNC(dstCalculateDotProductsNxNV3P)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);

DST_API void SIMD_FUNC(dstCalculateDotProducts4x4V3P)(const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, __simd128_float& result);

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x4V3P(
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, __simd128_float& m_result) {
    __simd128_float m_v1_0 = simd128_load_float(&f1[0]);
    __simd128_float m_v1_1 = simd128_load_float(&f1[4]);
    __simd128_float m_v1_2 = simd128_load_float(&f1[8]);
    __simd128_float m_v1_3 = simd128_load_float(&f1[12]);
    __simd128_float m_v2_0 = simd128_load_float(&f2[0]);
    __simd128_float m_v1_x, m_v1_y, m_v1_z;
    simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
    __simd128_float m_v2_1 = simd128_load_float(&f2[4]);
    __simd128_float m_v2_2 = simd128_load_float(&f2[8]);
    __simd128_float m_v2_3 = simd128_load_float(&f2[12]);
    __simd128_float m_v2_x, m_v2_y, m_v2_z;
    simd128_transpose4to3_float(m_v2_0, m_v2_1, m_v2_2, m_v2_3, m_v2_x, m_v2_y, m_v2_z);
    m_result = simd128_four_dot_products_vector3_vertical_float(m_v1_x, m_v1_y, m_v1_z, m_v2_x, m_v2_y,
        m_v2_z);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x4V3P(
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    __simd128_float m_result;
    dstInlineCalculateDotProducts4x4V3P(f1, f2, m_result);
    simd128_store_float(dot, m_result);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts16x16V3P(
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    dstInlineCalculateDotProducts4x4V3P(&f1[0 * 16], &f2[0 * 16], &dot[0]);
    dstInlineCalculateDotProducts4x4V3P(&f1[1 * 16], &f2[1 * 16], &dot[4]);
    dstInlineCalculateDotProducts4x4V3P(&f1[2 * 16], &f2[2 * 16], &dot[8]);
    dstInlineCalculateDotProducts4x4V3P(&f1[3 * 16], &f2[3 * 16], &dot[12]);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProductsNxNV3P(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    int i = 0;
    for (; i + 15 < n; i += 16)
        dstInlineCalculateDotProducts16x16V3P(&f1[i * 4], &f2[i * 4], &dot[i]);
    for (; i + 3 < n; i += 4)
        dstInlineCalculateDotProducts4x4V3P(&f1[i * 4], &f2[i * 4], &dot[i]);
    for (; i < n; i++) {
        __simd128_float m_v1 = simd128_load_float(&f1[i * 4]);
        __simd128_float m_v2 = simd128_load_float(&f2[i * 4]);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_shifted_float1 = simd128_shift_right_float(m_dot, 1);
        __simd128_float m_shifted_float2 = simd128_shift_right_float(m_dot, 2);
	__simd128_float m_result = simd128_add1_float(m_dot,
            simd128_add1_float(m_shifted_float1, m_shifted_float2));
        simd128_store_first_float(&dot[i], m_result);
    }
}

// Calculate dot products of four three-component float vectors stored at f1 and f2. The
// vectors are assumed to be stored consecutively in packed 12-byte format.

DST_API void SIMD_FUNC(dstCalculateDotProducts4x4V3)(const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, __simd128_float& result);

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x4V3(const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, __simd128_float& m_result) {
#ifdef SIMD_UNALIGNED_LOADS_EXPENSIVE
	// For CPUs where SSSE3 is likely to be the SSE level of choice, such as the Intel Core 2
	// architecture, unaligned loads are very expensive, so we load aligned and unpack.
	__simd128_float m_v1_0 = simd128_load_float(&f1[0]);
	__simd128_float m_v1_1 = simd128_load_float(&f1[4]);
	__simd128_float m_v1_2 = simd128_load_float(&f1[8]);
	__simd128_float m_v1_x, m_v1_y, m_v1_z;
	simd128_unpack3to4_and_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_x, m_v1_y, m_v1_z);
	__simd128_float m_v2_0 = simd128_load_float(&f2[0]);
	__simd128_float m_v2_1 = simd128_load_float(&f2[4]);
	__simd128_float m_v2_2 = simd128_load_float(&f2[8]);
	__simd128_float m_v2_x, m_v2_y, m_v2_z;
	simd128_unpack3to4_and_transpose4to3_float(m_v2_0, m_v2_1, m_v2_2, m_v2_x, m_v2_y, m_v2_z);
#else
    __simd128_float m_v1_0 = simd128_load_float(&f1[0]);
    __simd128_float m_v1_1 = simd128_load_unaligned_float(&f1[3]);
    __simd128_float m_v1_2 = simd128_load_unaligned_float(&f1[6]);
    __simd128_float m_v1_3 = simd128_load_unaligned_float(&f1[9]);
    __simd128_float m_v2_0 = simd128_load_float(&f2[0]);
    __simd128_float m_v1_x, m_v1_y, m_v1_z;
    simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
    __simd128_float m_v2_1 = simd128_load_unaligned_float(&f2[3]);
    __simd128_float m_v2_2 = simd128_load_unaligned_float(&f2[6]);
    __simd128_float m_v2_3 = simd128_load_unaligned_float(&f2[9]);
    __simd128_float m_v2_x, m_v2_y, m_v2_z;
    simd128_transpose4to3_float(m_v2_0, m_v2_1, m_v2_2, m_v2_3, m_v2_x, m_v2_y, m_v2_z);
#endif
    m_result = simd128_four_dot_products_vector3_vertical_float(m_v1_x, m_v1_y, m_v1_z, m_v2_x, m_v2_y,
        m_v2_z);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x4V3(
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    __simd128_float m_result;
    dstInlineCalculateDotProducts4x4V3(f1, f2, m_result);
    simd128_store_float(dot, m_result);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts16x16V3(
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    dstInlineCalculateDotProducts4x4V3(&f1[0 * 12], &f2[0 * 12], &dot[0]);
    dstInlineCalculateDotProducts4x4V3(&f1[1 * 12], &f2[1 * 12], &dot[4]);
    dstInlineCalculateDotProducts4x4V3(&f1[2 * 12], &f2[2 * 12], &dot[8]);
    dstInlineCalculateDotProducts4x4V3(&f1[3 * 12], &f2[3 * 12], &dot[12]);
}

DST_API void SIMD_FUNC(dstCalculateDotProductsNxNV3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);

static DST_INLINE_ONLY void dstInlineCalculateDotProductsNxNV3(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    int i = 0;
    for (; i + 15 < n; i += 16) {
        dstInlineCalculateDotProducts16x16V3(&f1[i * 3], &f2[i * 3], &dot[i]);
    }
    for (; i + 3 < n; i += 4)
        dstInlineCalculateDotProducts4x4V3(&f1[i * 3], &f2[i * 3], &dot[i]);
    for (; i < n; i++) {
        /// Note: simd128_load3_float assigns 0.0f to the w component.
        __simd128_float m_v1 = simd128_load3_float(&f1[i * 3]);
        __simd128_float m_v2 = simd128_load3_float(&f2[i * 3]);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_shifted_float1 = simd128_shift_right_float(m_dot, 1);
        __simd128_float m_shifted_float2 = simd128_shift_right_float(m_dot, 2);
	__simd128_float m_result = simd128_add1_float(m_dot,
            simd128_add1_float(m_shifted_float1, m_shifted_float2));
        simd128_store_first_float(&dot[i], m_result);
    }
}

// Calculate n dot products from one array of n vectors and one single vector,
// and store the result in an array of floats. Multiple variants based on the vector
// storage types. f1, f2 and dot must all be 16-bytes aligned.

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x1V4(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x,
const __simd128_float& m_v2_y, const __simd128_float& m_v2_z, const __simd128_float& m_v2_w,
__simd128_float& m_result) {
        __simd128_float m_v1_0 = simd128_load_float(&f1[0]);
        __simd128_float m_v1_1 = simd128_load_float(&f1[4]);
        __simd128_float m_v1_2 = simd128_load_float(&f1[8]);
        __simd128_float m_v1_3 = simd128_load_float(&f1[12]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z, m_v1_w;
        simd128_transpose4to4_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3,
            m_v1_x, m_v1_y, m_v1_z, m_v1_w);
	m_result = simd128_four_dot_products_vector4_vertical_float(m_v1_x, m_v1_y, m_v1_z, m_v1_w,
		m_v2_x, m_v2_y, m_v2_z, m_v2_w);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x1V4(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x,
const __simd128_float& m_v2_y, const __simd128_float& m_v2_z, const __simd128_float& m_v2_w,
float * DST_RESTRICT dot) {
    __simd128_float m_result;
    dstInlineCalculateDotProducts4x1V4(f1, m_v2_x, m_v2_y, m_v2_z,
        m_v2_w, m_result);
    simd128_store_float(dot, m_result);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts16x1V4(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x, const __simd128_float& m_v2_y,
const __simd128_float& m_v2_z, const __simd128_float& m_v2_w, float * DST_RESTRICT dot) {
        dstInlineCalculateDotProducts4x1V4(&f1[0 * 16], m_v2_x, m_v2_y, m_v2_z,
            m_v2_w, &dot[0]);
        dstInlineCalculateDotProducts4x1V4(&f1[1 * 16], m_v2_x, m_v2_y, m_v2_z,
            m_v2_w, &dot[4]);
        dstInlineCalculateDotProducts4x1V4(&f1[2 * 16], m_v2_x, m_v2_y, m_v2_z,
            m_v2_w, &dot[8]);
        dstInlineCalculateDotProducts4x1V4(&f1[3 * 16], m_v2_x, m_v2_y, m_v2_z,
            m_v2_w, &dot[12]);
}

DST_API void SIMD_FUNC(CalculateDotProductsNx1V4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);

static DST_INLINE_ONLY void dstInlineCalculateDotProductsNx1V4(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    __simd128_float m_v2 = simd128_load_unaligned_float(f2);
    __simd128_float m_v2_x, m_v2_y, m_v2_z, m_v2_w;
    m_v2_x = simd128_replicate_float(m_v2, 0);
    m_v2_y = simd128_replicate_float(m_v2, 1);
    m_v2_z = simd128_replicate_float(m_v2, 2);
    m_v2_w = simd128_replicate_float(m_v2, 3);
    int i = 0;
    for (; i + 15 < n; i += 16)
        dstInlineCalculateDotProducts16x1V4(&f1[i * 4], m_v2_x, m_v2_y, m_v2_z, m_v2_w,
            &dot[i]);
    for (; i + 3 < n; i += 4)
        dstInlineCalculateDotProducts4x1V4(&f1[i * 4], m_v2_x, m_v2_y, m_v2_z, m_v2_w,
            &dot[i]);
    for (; i < n; i++) {
        __simd128_float m_v1 = simd128_load_float(&f1[i * 4]);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_result = simd128_horizontal_add4_float(m_dot);
        simd128_store_first_float(&dot[i], m_result);
    }
}

// Calculate dot products of Nx1 vectors, all vectored stored in packed format.

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x1V3(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x,
const __simd128_float& m_v2_y, const __simd128_float& m_v2_z,
__simd128_float& m_result) {
#ifdef SIMD_UNALIGNED_LOADS_EXPENSIVE
	// For CPUs where SSSE3 is likely to be the SSE level of choice, such as the Intel Core 2
	// architecture, unaligned loads are very expensive, so we load aligned and unpack before
	// transposing.
	__simd128_float m_v1_0 = simd128_load_float(&f1[0]);
	__simd128_float m_v1_1 = simd128_load_float(&f1[4]);
	__simd128_float m_v1_2 = simd128_load_float(&f1[8]);
	__simd128_float m_v1_x, m_v1_y, m_v1_z;
	simd128_unpack3to4_and_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_x, m_v1_y, m_v1_z);
#else
    __simd128_float m_v1_0 = simd128_load_float(&f1[0]);
    __simd128_float m_v1_1 = simd128_load_unaligned_float(&f1[3]);
    __simd128_float m_v1_2 = simd128_load_unaligned_float(&f1[6]);
    __simd128_float m_v1_3 = simd128_load_unaligned_float(&f1[9]);
    __simd128_float m_v1_x, m_v1_y, m_v1_z;
    simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
#endif
    m_result = simd128_four_dot_products_vector3_vertical_float(m_v1_x, m_v1_y, m_v1_z, m_v2_x, m_v2_y,
        m_v2_z);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x1V3(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x,
const __simd128_float& m_v2_y, const __simd128_float& m_v2_z, float * DST_RESTRICT dot) {
    __simd128_float m_result;
    dstInlineCalculateDotProducts4x1V3(f1, m_v2_x, m_v2_y, m_v2_z,
        m_result);
    simd128_store_float(dot, m_result);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts16x1V3(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x, const __simd128_float& m_v2_y,
const __simd128_float& m_v2_z, float * DST_RESTRICT dot) {
        dstInlineCalculateDotProducts4x1V3(&f1[0 * 12],
		m_v2_x, m_v2_y, m_v2_z, &dot[0]);
        dstInlineCalculateDotProducts4x1V3(&f1[1 * 12],
		m_v2_x, m_v2_y, m_v2_z, &dot[4]);
        dstInlineCalculateDotProducts4x1V3(&f1[2 * 12],
		m_v2_x, m_v2_y, m_v2_z, &dot[8]);
        dstInlineCalculateDotProducts4x1V3(&f1[3 * 12],
		m_v2_x, m_v2_y, m_v2_z, &dot[12]);
}

DST_API void SIMD_FUNC(dstCalculateDotProductsNx1V3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);

static DST_INLINE_ONLY void dstInlineCalculateDotProductsNx1V3(int n,
const float * DST_RESTRICT f1, const __simd128_float m_v2, float * DST_RESTRICT dot) {
    __simd128_float m_v2_x, m_v2_y, m_v2_z;
    m_v2_x = simd128_replicate_float(m_v2, 0);
    m_v2_y = simd128_replicate_float(m_v2, 1);
    m_v2_z = simd128_replicate_float(m_v2, 2);
    int i = 0;
    for (; i + 15 < n; i += 16)
        dstInlineCalculateDotProducts16x1V3(&f1[i * 3],
            m_v2_x, m_v2_y, m_v2_z, &dot[i]);
    for (; i + 3 < n; i += 4)
        dstInlineCalculateDotProducts4x1V3(&f1[i * 3],
            m_v2_x, m_v2_y, m_v2_z, &dot[i]);
    for (; i < n; i++) {
        __simd128_float m_v1 = simd128_load3_float(&f1[i * 3]);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_shifted_float1 = simd128_shift_right_float(m_dot, 1);
        __simd128_float m_shifted_float2 = simd128_shift_right_float(m_dot, 2);
	__simd128_float m_result = simd128_add1_float(m_dot,
            simd128_add1_float(m_shifted_float1, m_shifted_float2));
        simd128_store_first_float(&dot[i], m_result);
    }
}

static DST_INLINE_ONLY void dstInlineCalculateDotProductsNx1V3(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    __simd128_float m_v2 = simd128_load3_float(f2);
    dstInlineCalculateDotProductsNx1V3(n, f1, m_v2, dot);
}

// Calculate dot product of Nx1 vectors, where the N vectors are stored in padded format
// (16-byte aligned). Because vectors of three floats are stored in 16-byte aligned format,
// loading becomes more straightforward since simd128_load_float can be used. For the fourth
// element of v2 the SIMD register needs to be initialized with 0.0f.
// f1 and dot must be 16-bytes aligned.

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x1V3PV3(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x,
const __simd128_float& m_v2_y, const __simd128_float& m_v2_z, __simd128_float& m_result) {
        __simd128_float m_v1_0 = simd128_load_float(&f1[0]);
        __simd128_float m_v1_1 = simd128_load_float(&f1[4]);
        __simd128_float m_v1_2 = simd128_load_float(&f1[8]);
        __simd128_float m_v1_3 = simd128_load_float(&f1[12]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
	m_result = simd128_four_dot_products_vector3_vertical_float(m_v1_x, m_v1_y, m_v1_z,
		m_v2_x, m_v2_y, m_v2_z);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x1V3PV3(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x,
const __simd128_float& m_v2_y, const __simd128_float& m_v2_z, float * DST_RESTRICT dot) {
    __simd128_float m_result;
    dstInlineCalculateDotProducts4x1V3PV3(f1, m_v2_x, m_v2_y, m_v2_z, m_result);
    simd128_store_float(dot, m_result);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts16x1V3PV3(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x, const __simd128_float& m_v2_y,
const __simd128_float& m_v2_z, float * DST_RESTRICT dot) {
        dstInlineCalculateDotProducts4x1V3PV3(&f1[0 * 16], m_v2_x, m_v2_y, m_v2_z, &dot[0]);
        dstInlineCalculateDotProducts4x1V3PV3(&f1[1 * 16], m_v2_x, m_v2_y, m_v2_z, &dot[4]);
        dstInlineCalculateDotProducts4x1V3PV3(&f1[2 * 16], m_v2_x, m_v2_y, m_v2_z, &dot[8]);
        dstInlineCalculateDotProducts4x1V3PV3(&f1[3 * 16], m_v2_x, m_v2_y, m_v2_z, &dot[12]);
}

DST_API void SIMD_FUNC(dstCalculateDotProductsNx1V3PV3)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);

static DST_INLINE_ONLY void dstInlineCalculateDotProductsNx1V3PV3(int n,
const float * DST_RESTRICT f1, const __simd128_float m_v2, float * DST_RESTRICT dot) {
    __simd128_float m_v2_x, m_v2_y, m_v2_z;
    m_v2_x = simd128_replicate_float(m_v2, 0);
    m_v2_y = simd128_replicate_float(m_v2, 1);
    m_v2_z = simd128_replicate_float(m_v2, 2);
    int i = 0;
    for (; i + 15 < n; i += 16)
        dstInlineCalculateDotProducts16x1V3PV3(&f1[i * 4],
            m_v2_x, m_v2_y, m_v2_z, &dot[i]);
    for (; i + 3 < n; i += 4)
        dstInlineCalculateDotProducts4x1V3PV3(&f1[i * 4],
            m_v2_x, m_v2_y, m_v2_z, &dot[i]);
    for (; i < n; i++) {
        __simd128_float m_v1 = simd128_set_last_zero_float(simd128_load_float(&f1[i * 4]));
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_result = simd128_horizontal_add4_float(m_dot);
        simd128_store_first_float(&dot[i], m_result);
    }
}

static DST_INLINE_ONLY void dstInlineCalculateDotProductsNx1V3PV3(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    __simd128_float m_v2 = simd128_load3_float(f2);
    // The w component as loaded has been set to 0.0f.
    dstInlineCalculateDotProductsNx1V3PV3(n, f1, m_v2, dot);
}

// The array of vectors is an array of points with implicit w coordinate of 1.0f,
// and is stored aligned on 16-byte boundaries. The constant vector has four
// components.
// f1 and dot must both be 16-bytes aligned.

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x1P3PV4(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x, const __simd128_float& m_v2_y,
const __simd128_float m_v2_z, const __simd128_float& m_v2_w, __simd128_float& m_result) {
        __simd128_float m_v1_0 = simd128_load_float(&f1[0]);
        __simd128_float m_v1_1 = simd128_load_float(&f1[4]);
        __simd128_float m_v1_2 = simd128_load_float(&f1[8]);
        __simd128_float m_v1_3 = simd128_load_float(&f1[12]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
	m_result = simd128_four_dot_products_point3_vector4_vertical_float(
		m_v1_x, m_v1_y, m_v1_z,	m_v2_x, m_v2_y, m_v2_z, m_v2_w);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x1P3PV4(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x, const __simd128_float& m_v2_y,
const __simd128_float& m_v2_z, const __simd128_float& m_v2_w, float * DST_RESTRICT dot) {
    __simd128_float m_result;
    dstInlineCalculateDotProducts4x1P3PV4(f1, m_v2_x, m_v2_y, m_v2_z, m_v2_w, m_result);
    simd128_store_float(dot, m_result);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts16x1P3PV4(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x, const __simd128_float& m_v2_y,
const __simd128_float& m_v2_z, const __simd128_float& m_v2_w, float * DST_RESTRICT dot) {
        dstInlineCalculateDotProducts4x1P3PV4(&f1[0 * 16], m_v2_x, m_v2_y, m_v2_z, m_v2_w, &dot[0]);
        dstInlineCalculateDotProducts4x1P3PV4(&f1[1 * 16], m_v2_x, m_v2_y, m_v2_z, m_v2_w, &dot[4]);
        dstInlineCalculateDotProducts4x1P3PV4(&f1[2 * 16], m_v2_x, m_v2_y, m_v2_z, m_v2_w, &dot[8]);
        dstInlineCalculateDotProducts4x1P3PV4(&f1[3 * 16], m_v2_x, m_v2_y, m_v2_z, m_v2_w, &dot[12]);
}

DST_API void SIMD_FUNC(dstCalculateDotProductsNx1P3PV4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);

static DST_INLINE_ONLY void dstInlineCalculateDotProductsNx1P3PV4(int n,
const float * DST_RESTRICT f1, const __simd128_float m_v2, float * DST_RESTRICT dot) {
    __simd128_float m_v2_x, m_v2_y, m_v2_z, m_v2_w;
    m_v2_x = simd128_replicate_float(m_v2, 0);
    m_v2_y = simd128_replicate_float(m_v2, 1);
    m_v2_z = simd128_replicate_float(m_v2, 2);
    m_v2_w = simd128_replicate_float(m_v2, 3);
    int i = 0;
#if 1
    for (; i + 15 < n; i += 16)
        dstInlineCalculateDotProducts16x1P3PV4(&f1[i * 4],
            m_v2_x, m_v2_y, m_v2_z, m_v2_w, &dot[i]);
    for (; i + 3 < n; i += 4)
        dstInlineCalculateDotProducts4x1P3PV4(&f1[i * 4],
            m_v2_x, m_v2_y, m_v2_z, m_v2_w, &dot[i]);
#else
    for (; i + 3 < n; i += 4) {
        __simd128_float m_v1_0 = simd128_load_float(&f1[i * 4 + 0]);
        __simd128_float m_v1_1 = simd128_load_float(&f1[i * 4 + 4]);
        __simd128_float m_v1_2 = simd128_load_float(&f1[i * 4 + 8]);
        __simd128_float m_v1_3 = simd128_load_float(&f1[i * 4 + 12]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
	__simd128_float m_result = simd128_four_dot_products_point3_vector4_vertical_float(
		m_v1_x, m_v1_y, m_v1_z,	m_v2_x, m_v2_y, m_v2_z, m_v2_w);
        simd128_store_float(&dot[i], m_result);
    }
#endif
    for (; i < n; i++) {
         // Load the point vector and make sure the w component is 1.0f.
        __simd128_float m_v1 = simd128_set_last_float(
            simd128_load_float(&f1[i * 4]), 1.0f);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_result = simd128_horizontal_add4_float(m_dot);
        simd128_store_first_float(&dot[i], m_result);
    }
}

static DST_INLINE_ONLY void dstInlineCalculateDotProductsNx1P3PV4(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    __simd128_float m_v2 = simd128_load_unaligned_float(f2);
    dstInlineCalculateDotProductsNx1P3PV4(n, f1, m_v2, dot);
}

// The array of vectors is an array of points with implicit w coordinate of 1.0f,
// and is stored unaligned on 12-byte boundaries (but starting on a 16-byte boundary).
// The constant vector has four components. f1 and dot must be 16-bytes aligned.

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x1P3V4(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x, const __simd128_float& m_v2_y,
const __simd128_float m_v2_z, const __simd128_float& m_v2_w, __simd128_float& m_result) {
#ifdef SIMD_UNALIGNED_LOADS_EXPENSIVE
	// For CPUs where SSSE3 is likely to be the SSE level of choice, such as the Intel Core 2
	// architecture, unaligned loads are very expensive, so we load aligned and unpack.
	__simd128_float m_v1_0 = simd128_load_float(&f1[0]);
	__simd128_float m_v1_1 = simd128_load_float(&f1[4]);
	__simd128_float m_v1_2 = simd128_load_float(&f1[8]);
	__simd128_float m_v1_x, m_v1_y, m_v1_z;
	simd128_unpack3to4_and_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_x, m_v1_y, m_v1_z);
#else
        __simd128_float m_v1_0 = simd128_load_float(&f1[0]);
        __simd128_float m_v1_1 = simd128_load_unaligned_float(&f1[3]);
        __simd128_float m_v1_2 = simd128_load_unaligned_float(&f1[6]);
        __simd128_float m_v1_3 = simd128_load_unaligned_float(&f1[9]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
#endif
	m_result = simd128_four_dot_products_point3_vector4_vertical_float(
		m_v1_x, m_v1_y, m_v1_z,	m_v2_x, m_v2_y, m_v2_z, m_v2_w);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts4x1P3V4(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x, const __simd128_float& m_v2_y,
const __simd128_float& m_v2_z, const __simd128_float& m_v2_w, float * DST_RESTRICT dot) {
    __simd128_float m_result;
    dstInlineCalculateDotProducts4x1P3V4(f1, m_v2_x, m_v2_y, m_v2_z, m_v2_w, m_result);
    simd128_store_float(dot, m_result);
}

static DST_INLINE_ONLY void dstInlineCalculateDotProducts16x1P3V4(
const float * DST_RESTRICT f1, const __simd128_float& m_v2_x, const __simd128_float& m_v2_y,
const __simd128_float& m_v2_z, const __simd128_float& m_v2_w, float * DST_RESTRICT dot) {
        dstInlineCalculateDotProducts4x1P3V4(&f1[0 * 12], m_v2_x, m_v2_y, m_v2_z, m_v2_w, &dot[0]);
        dstInlineCalculateDotProducts4x1P3V4(&f1[1 * 12], m_v2_x, m_v2_y, m_v2_z, m_v2_w, &dot[4]);
        dstInlineCalculateDotProducts4x1P3V4(&f1[2 * 12], m_v2_x, m_v2_y, m_v2_z, m_v2_w, &dot[8]);
        dstInlineCalculateDotProducts4x1P3V4(&f1[3 * 12], m_v2_x, m_v2_y, m_v2_z, m_v2_w, &dot[12]);
}

DST_API void SIMD_FUNC(dstCalculateDotProductsNx1P3V4)(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot);

static DST_INLINE_ONLY void dstInlineCalculateDotProductsNx1P3V4(int n,
const float * DST_RESTRICT f1, const __simd128_float m_v2, float * DST_RESTRICT dot) {
    __simd128_float m_v2_x, m_v2_y, m_v2_z, m_v2_w;
    m_v2_x = simd128_replicate_float(m_v2, 0);
    m_v2_y = simd128_replicate_float(m_v2, 1);
    m_v2_z = simd128_replicate_float(m_v2, 2);
    m_v2_w = simd128_replicate_float(m_v2, 3);
    int i = 0;
#if 1
    for (; i + 15 < n; i += 16)
        dstInlineCalculateDotProducts16x1P3V4(&f1[i * 3],
            m_v2_x, m_v2_y, m_v2_z, m_v2_w, &dot[i]);
    for (; i + 3 < n; i += 4)
        dstInlineCalculateDotProducts4x1P3V4(&f1[i * 3],
            m_v2_x, m_v2_y, m_v2_z, m_v2_w, &dot[i]);
#else
    for (; i + 3 < n; i += 4) {
#ifdef SIMD_UNALIGNED_LOADS_EXPENSIVE
	// For CPUs where SSSE3 is likely to be the SSE level of choice, such as the Intel Core 2
	// architecture, unaligned loads are very expensive, so we load aligned and unpack.
	__simd128_float m_v1_0 = simd128_load_float(&f1[0]);
	__simd128_float m_v1_1 = simd128_load_float(&f1[4]);
	__simd128_float m_v1_2 = simd128_load_float(&f1[8]);
	__simd128_float m_v1_x, m_v1_y, m_v1_z;
	simd128_unpack3to4_and_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_x, m_v1_y, m_v1_z);
#else
        __simd128_float m_v1_0 = simd128_load_float(&f1[0]);
        __simd128_float m_v1_1 = simd128_load_unaligned_float(&f1[3]);
        __simd128_float m_v1_2 = simd128_load_unaligned_float(&f1[6]);
        __simd128_float m_v1_3 = simd128_load_unaligned_float(&f1[9]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
#endif
	__simd128_float m_result = simd128_four_dot_products_point3_vector4_vertical_float(
		m_v1_x, m_v1_y, m_v1_z,	m_v2_x, m_v2_y, m_v2_z, m_v2_w);
        simd128_store_float(&dot[i], m_result);
    }
#endif
    for (; i < n; i++) {
         // Load the point vector and make sure the w component is 1.0f.
        __simd128_float m_v1 = simd128_set_last_float(
            simd128_load3_float(&f1[i * 3]), 1.0f);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_result = simd128_horizontal_add4_float(m_dot);
        simd128_store_first_float(&dot[i], m_result);
    }
}

static DST_INLINE_ONLY void dstInlineCalculateDotProductsNx1P3V4(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
    __simd128_float m_v2 = simd128_load_unaligned_float(f2);
    dstInlineCalculateDotProductsNx1P3V4(n, f1, m_v2, dot);
}

// Calculate n dot products from one array of n point vectors stored on 16-byte
// boundaries and one single four-element vector, store the result in an array of
// floats, and count the number of dot products that is smaller than zero.
// f1 and dot must both be 16-bytes aligned.

DST_API void SIMD_FUNC(dstCalculateDotProductsAndCountNegativeNx1P3PV4)(
int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot,
int& negative_count);

static DST_INLINE_ONLY void dstInlineCalculateDotProductsAndCountNegativeNx1P3PV4(
int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot,
int& negative_count) {
    __simd128_float m_v2 = simd128_load_unaligned_float(f2);
    __simd128_float m_v2_x, m_v2_y, m_v2_z, m_v2_w;
    m_v2_x = simd128_replicate_float(m_v2, 0);
    m_v2_y = simd128_replicate_float(m_v2, 1);
    m_v2_z = simd128_replicate_float(m_v2, 2);
    m_v2_w = simd128_replicate_float(m_v2, 3);
    __simd128_float m_zeros = simd128_set_zero_float();
    int count = 0;
    int i = 0;
    for (; i + 3 < n; i += 4) {
        __simd128_float m_v1_0 = simd128_load_float(&f1[i * 4 + 0]);
        __simd128_float m_v1_1 = simd128_load_float(&f1[i * 4 + 4]);
        __simd128_float m_v1_2 = simd128_load_float(&f1[i * 4 + 8]);
        __simd128_float m_v1_3 = simd128_load_float(&f1[i * 4 + 12]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
	__simd128_float m_result = simd128_four_dot_products_point3_vector4_vertical_float(
		m_v1_x, m_v1_y, m_v1_z, m_v2_x, m_v2_y, m_v2_z, m_v2_w);
        simd128_store_float(&dot[i], m_result);
        // Produce 32-bit bit masks indicating whether each result is smaller than zero.
        __simd128_int m_comp = simd128_cmplt_float(m_result, m_zeros);
        // Convert to 32-bit mask (bit 31) to packed bits and count the ones.
        count += simd_count_bits_int4(simd128_convert_masks_int32_int1(m_comp));
    }
    for (; i < n; i++) {
         // Load the point vector and make sure the w component is 1.0f.
        __simd128_float m_v1 = simd128_set_last_float(
            simd128_load_float(&f1[i * 4]), 1.0f);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_result = simd128_horizontal_add4_float(m_dot);
        simd128_store_first_float(&dot[i], m_result);
        count += (simd128_get_float(m_result) < 0.0f);
    }
    negative_count = count;
}

// Similar to the previous function, but the point vectors are packed at 12-byte
// boundaries.
// f1 and dot must be 16-bytes aligned.

DST_API void SIMD_FUNC(dstCalculateDotProductsAndCountNegativeNx1P3V4)(
int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot,
int& negative_count);

static DST_INLINE_ONLY void dstInlineCalculateDotProductsAndCountNegativeNx1P3V4(
int n, const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float * DST_RESTRICT dot,
int& negative_count) {
    __simd128_float m_v2 = simd128_load_unaligned_float(f2);
    __simd128_float m_v2_x, m_v2_y, m_v2_z, m_v2_w;
    m_v2_x = simd128_replicate_float(m_v2, 0);
    m_v2_y = simd128_replicate_float(m_v2, 1);
    m_v2_z = simd128_replicate_float(m_v2, 2);
    m_v2_w = simd128_replicate_float(m_v2, 3);
    __simd128_float m_zeros = simd128_set_zero_float();
    int count = 0;
    int i = 0;
    for (; i + 3 < n; i += 4) {
#ifdef SIMD_UNALIGNED_LOADS_EXPENSIVE
	// For CPUs where SSSE3 is likely to be the SSE level of choice, such as the Intel Core 2
	// architecture, unaligned loads are very expensive, so we load aligned and unpack.
	__simd128_float m_v1_0 = simd128_load_float(&f1[0]);
	__simd128_float m_v1_1 = simd128_load_float(&f1[4]);
	__simd128_float m_v1_2 = simd128_load_float(&f1[8]);
	__simd128_float m_v1_x, m_v1_y, m_v1_z;
	simd128_unpack3to4_and_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_x, m_v1_y, m_v1_z);
#else
        __simd128_float m_v1_0 = simd128_load_float(&f1[i * 3]);
        __simd128_float m_v1_1 = simd128_load_unaligned_float(&f1[i * 3 + 3]);
        __simd128_float m_v1_2 = simd128_load_unaligned_float(&f1[i * 3 + 6]);
        __simd128_float m_v1_3 = simd128_load_unaligned_float(&f1[i * 3 + 9]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
#endif
	__simd128_float m_result = simd128_four_dot_products_point3_vector4_vertical_float(
		m_v1_x, m_v1_y, m_v1_z,	m_v2_x, m_v2_y, m_v2_z, m_v2_w);
        simd128_store_float(&dot[i], m_result);
        // Produce 32-bit bit masks indicating whether each result is smaller than zero.
        __simd128_int m_comp = simd128_cmplt_float(m_result, m_zeros);
        // Convert to 32-bit mask (bit 31) to packed bits and count the ones.
        count += simd_count_bits_int4(simd128_convert_masks_int32_int1(m_comp));
    }
    for (; i < n; i++) {
         // Load the point vector and make sure the w component is 1.0f.
        __simd128_float m_v1 = simd128_set_last_float(
            simd128_load3_float(&f1[i * 3]), 1.0f);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_result = simd128_horizontal_add4_float(m_dot);
        simd128_store_first_float(&dot[i], m_result);
        count += (simd128_get_float(m_result) < 0.0f);
    }
    negative_count = count;
}

// Non-SIMD helper functions for dot product calculation.

static DST_INLINE_ONLY float dstDotVector3(const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2) {
	return f1[0] * f2[0] + f1[1] * f2[1] + f1[2] * f2[2];
}

static DST_INLINE_ONLY float dstDotVector4(const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2) {
	return f1[0] * f2[0] + f1[1] * f2[1] + f1[2] * f2[2] + f1[3] * f2[3];
}

// Determine the minimum and maximum dot product of an array of vertices with a
// given constant vector.
// The array of vertices must be aligned on a 16-bytes boundary.

DST_API void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx1V3)(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
float& DST_RESTRICT min_dot_product, float& DST_RESTRICT max_dot_product);

static DST_INLINE_ONLY void dstInlineCalculateMinAndMaxDotProductNx1V3(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float& DST_RESTRICT min_dot_product, float& DST_RESTRICT max_dot_product) {
    int i = 0;
    if (((uintptr_t)f1 & 0xF) == 0) {
        __simd128_float m_v2_x = simd128_set_same_float(f2[0]);
        __simd128_float m_v2_y = simd128_set_same_float(f2[1]);
        __simd128_float m_v2_z = simd128_set_same_float(f2[2]);
        __simd128_float m_min_dot = simd128_set_same_float(FLT_MAX);
        __simd128_float m_max_dot = simd128_set_same_float(- FLT_MAX);
        for (; i + 3 < n; i += 4) {
            __simd128_float m_vertex0 = simd128_load_unaligned_float(&f1[i * 3]);
            __simd128_float m_vertex1 = simd128_load_unaligned_float(&f1[i * 3 + 3]);
            __simd128_float m_vertex2 = simd128_load_unaligned_float(&f1[i * 3 + 6]);
            __simd128_float m_vertex3 = simd128_load_unaligned_float(&f1[i * 3 + 9]);
            __simd128_float m_v1_x, m_v1_y, m_v1_z;
            simd128_transpose4to3_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
                m_v1_x, m_v1_y, m_v1_z);
            __simd128_float m_dot = simd128_four_dot_products_vector3_vertical_float(
                m_v1_x, m_v1_y, m_v1_z,	m_v2_x, m_v2_y, m_v2_z);
            m_min_dot = simd128_min_float(m_min_dot, m_dot);
            m_max_dot = simd128_max_float(m_max_dot, m_dot);
        }
        __simd128_float shifted_float1, shifted_float2, shifted_float3;
        __simd128_float m_min_dot_23, m_max_dot_23;
        shifted_float1 = simd128_shift_right_float(m_min_dot, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot, 3);
        m_min_dot = simd128_min1_float(m_min_dot, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot = simd128_min1_float(m_min_dot, m_min_dot_23);
        min_dot_product = simd128_get_float(m_min_dot);
        shifted_float1 = simd128_shift_right_float(m_max_dot, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot, 3);
        m_min_dot = simd128_max1_float(m_max_dot, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot = simd128_max1_float(m_max_dot, m_max_dot_23);
        max_dot_product = simd128_get_float(m_max_dot);
        // Process the remaining vertices using regular code.
    }
    else
    {
        min_dot_product = FLT_MAX;
        max_dot_product = - FLT_MAX;
    }
    for (; i < n; i++) {
        min_dot_product = minf(min_dot_product, dstDotVector3(&f1[i * 3], f2));
        max_dot_product = maxf(max_dot_product, dstDotVector3(&f1[i * 3], f2));
    }
}

DST_API void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx1V4)(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
float& DST_RESTRICT min_dot_product, float& DST_RESTRICT max_dot_product);

static DST_INLINE_ONLY void dstInlineCalculateMinAndMaxDotProductNx1V4(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float& DST_RESTRICT min_dot_product, float& DST_RESTRICT max_dot_product) {
    int i = 0;
    if (((uintptr_t)f1 & 0xF) == 0) {
	__simd128_float m_v2 = simd128_load_unaligned_float(f2);
        __simd128_float m_v2_x = simd128_replicate_float(m_v2, 0);
        __simd128_float m_v2_y = simd128_replicate_float(m_v2, 1);
        __simd128_float m_v2_z = simd128_replicate_float(m_v2, 2);
        __simd128_float m_v2_w = simd128_replicate_float(m_v2, 3);
        __simd128_float m_min_dot = simd128_set_same_float(FLT_MAX);
        __simd128_float m_max_dot = simd128_set_same_float(- FLT_MAX);
        for (; i + 3 < n; i += 4) {
            __simd128_float m_vertex0 = simd128_load_float(&f1[i * 4]);
            __simd128_float m_vertex1 = simd128_load_float(&f1[i * 4 + 4]);
            __simd128_float m_vertex2 = simd128_load_float(&f1[i * 4 + 8]);
            __simd128_float m_vertex3 = simd128_load_float(&f1[i * 4 + 12]);
            __simd128_float m_v1_x, m_v1_y, m_v1_z, m_v1_w;
            simd128_transpose4to4_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
                m_v1_x, m_v1_y, m_v1_z, m_v1_w);
	    __simd128_float m_dot = simd128_four_dot_products_vector4_vertical_float(
		m_v1_x, m_v1_y, m_v1_z, m_v1_w,
		m_v2_x, m_v2_y, m_v2_z, m_v2_w);
            m_min_dot = simd128_min_float(m_min_dot, m_dot);
            m_max_dot = simd128_max_float(m_max_dot, m_dot);
        }
        __simd128_float shifted_float1, shifted_float2, shifted_float3;
        __simd128_float m_min_dot_23, m_max_dot_23;
        shifted_float1 = simd128_shift_right_float(m_min_dot, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot, 3);
        m_min_dot = simd128_min1_float(m_min_dot, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot = simd128_min1_float(m_min_dot, m_min_dot_23);
        min_dot_product = simd128_get_float(m_min_dot);
        shifted_float1 = simd128_shift_right_float(m_max_dot, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot, 3);
        m_min_dot = simd128_max1_float(m_max_dot, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot = simd128_max1_float(m_max_dot, m_max_dot_23);
        max_dot_product = simd128_get_float(m_max_dot);
        // Process the remaining vertices using regular code.
    }
    else
    {
        min_dot_product = FLT_MAX;
        max_dot_product = - FLT_MAX;
    }
    for (; i < n; i++) {
        min_dot_product = minf(min_dot_product, dstDotVector4(&f1[i * 4], f2));
        max_dot_product = maxf(max_dot_product, dstDotVector4(&f1[i * 4], f2));
    }
}

// Determine the minimum and maximum dot products of an array of vertices with three
// constant vectors.

DST_API void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V3)(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product);

static DST_INLINE_ONLY void dstInlineCalculateMinAndMaxDotProductNx3V3(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product) {
    int i = 0;
    if (((uintptr_t)f1 & 0xF) == 0) {
        __simd128_float m_v2_0_x = simd128_set_same_float(f2[0]);
        __simd128_float m_v2_0_y = simd128_set_same_float(f2[1]);
        __simd128_float m_v2_0_z = simd128_set_same_float(f2[2]);
        __simd128_float m_v2_1_x = simd128_set_same_float(f2[3]);
        __simd128_float m_v2_1_y = simd128_set_same_float(f2[4]);
        __simd128_float m_v2_1_z = simd128_set_same_float(f2[5]);
        __simd128_float m_v2_2_x = simd128_set_same_float(f2[6]);
        __simd128_float m_v2_2_y = simd128_set_same_float(f2[7]);
        __simd128_float m_v2_2_z = simd128_set_same_float(f2[8]);
        __simd128_float m_min_dot_C0 = simd128_set_same_float(FLT_MAX);
        __simd128_float m_min_dot_C1 = m_min_dot_C0;
        __simd128_float m_min_dot_C2 = m_min_dot_C0;
        __simd128_float m_max_dot_C0 = simd128_set_same_float(- FLT_MAX);
        __simd128_float m_max_dot_C1 = m_max_dot_C0;
        __simd128_float m_max_dot_C2 = m_max_dot_C0;
        for (; i + 3 < n; i += 4) {
            __simd128_float m_vertex0 = simd128_load_unaligned_float(&f1[i * 3]);
            __simd128_float m_vertex1 = simd128_load_unaligned_float(&f1[i * 3 + 3]);
            __simd128_float m_vertex2 = simd128_load_unaligned_float(&f1[i * 3 + 6]);
            __simd128_float m_vertex3 = simd128_load_unaligned_float(&f1[i * 3 + 9]);
            __simd128_float m_v1_x, m_v1_y, m_v1_z;
            simd128_transpose4to3_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
                m_v1_x, m_v1_y, m_v1_z);
            __simd128_float m_dot = simd128_four_dot_products_vector3_vertical_float(
	        m_v1_x, m_v1_y, m_v1_z, m_v2_0_x, m_v2_0_y, m_v2_0_z);
            m_min_dot_C0 = simd128_min_float(m_min_dot_C0, m_dot);
            m_max_dot_C0 = simd128_max_float(m_max_dot_C0, m_dot);
            m_dot = simd128_four_dot_products_vector3_vertical_float(
	        m_v1_x, m_v1_y, m_v1_z, m_v2_1_x, m_v2_1_y, m_v2_1_z);
            m_min_dot_C1 = simd128_min_float(m_min_dot_C1, m_dot);
            m_max_dot_C1 = simd128_max_float(m_max_dot_C1, m_dot);
            m_dot = simd128_four_dot_products_vector3_vertical_float(
	        m_v1_x, m_v1_y, m_v1_z, m_v2_2_x, m_v2_2_y, m_v2_2_z);
            m_min_dot_C2 = simd128_min_float(m_min_dot_C2, m_dot);
            m_max_dot_C2 = simd128_max_float(m_max_dot_C2, m_dot);
        }
        __simd128_float shifted_float1, shifted_float2, shifted_float3;
        __simd128_float m_min_dot_23, m_max_dot_23;
        shifted_float1 = simd128_shift_right_float(m_min_dot_C0, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot_C0, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot_C0, 3);
        m_min_dot_C0 = simd128_min1_float(m_min_dot_C0, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot_C0 = simd128_min1_float(m_min_dot_C0, m_min_dot_23);
        min_dot_product[0] = simd128_get_float(m_min_dot_C0);
        shifted_float1 = simd128_shift_right_float(m_max_dot_C0, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot_C0, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot_C0, 3);
        m_min_dot_C0 = simd128_max1_float(m_max_dot_C0, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot_C0 = simd128_max1_float(m_max_dot_C0, m_max_dot_23);
        max_dot_product[0] = simd128_get_float(m_max_dot_C0);

        shifted_float1 = simd128_shift_right_float(m_min_dot_C1, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot_C1, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot_C1, 3);
        m_min_dot_C1 = simd128_min1_float(m_min_dot_C1, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot_C1 = simd128_min1_float(m_min_dot_C1, m_min_dot_23);
        min_dot_product[1] = simd128_get_float(m_min_dot_C1);
        shifted_float1 = simd128_shift_right_float(m_max_dot_C1, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot_C1, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot_C1, 3);
        m_max_dot_C1 = simd128_max1_float(m_max_dot_C1, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot_C1 = simd128_max1_float(m_max_dot_C1, m_max_dot_23);
        max_dot_product[1] = simd128_get_float(m_max_dot_C1);

        shifted_float1 = simd128_shift_right_float(m_min_dot_C2, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot_C2, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot_C2, 3);
        m_min_dot_C2 = simd128_min1_float(m_min_dot_C2, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot_C2 = simd128_min1_float(m_min_dot_C2, m_min_dot_23);
        min_dot_product[2] = simd128_get_float(m_min_dot_C2);
        shifted_float1 = simd128_shift_right_float(m_max_dot_C2, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot_C2, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot_C2, 3);
        m_max_dot_C2 = simd128_max1_float(m_max_dot_C2, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot_C2 = simd128_max1_float(m_max_dot_C2, m_max_dot_23);
        max_dot_product[2] = simd128_get_float(m_max_dot_C2);
    }
    else
    for (int j = 0; j < 3; j++) {
        min_dot_product[j] = FLT_MAX;
        max_dot_product[j] = - FLT_MAX;
    }
    // Process the remaining vertices.
    for (; i < n; i++)
        for (int j = 0; j < 3; j++) {
            min_dot_product[j] = minf(min_dot_product[j], dstDotVector3(&f1[i * 3], &f2[j * 3]));
            max_dot_product[j] = maxf(max_dot_product[j], dstDotVector3(&f1[i * 3], &f2[j * 3]));
        }
}

DST_API void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V3PV3)(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product);

static DST_INLINE_ONLY void dstInlineCalculateMinAndMaxDotProductNx3V3PV3(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product) {
    int i = 0;
    if (((uintptr_t)f1 & 0xF) == 0) {
        __simd128_float m_v2_0_x = simd128_set_same_float(f2[0]);
        __simd128_float m_v2_0_y = simd128_set_same_float(f2[1]);
        __simd128_float m_v2_0_z = simd128_set_same_float(f2[2]);
        __simd128_float m_v2_1_x = simd128_set_same_float(f2[3]);
        __simd128_float m_v2_1_y = simd128_set_same_float(f2[4]);
        __simd128_float m_v2_1_z = simd128_set_same_float(f2[5]);
        __simd128_float m_v2_2_x = simd128_set_same_float(f2[6]);
        __simd128_float m_v2_2_y = simd128_set_same_float(f2[7]);
        __simd128_float m_v2_2_z = simd128_set_same_float(f2[8]);
        __simd128_float m_min_dot_C0 = simd128_set_same_float(FLT_MAX);
        __simd128_float m_min_dot_C1 = m_min_dot_C0;
        __simd128_float m_min_dot_C2 = m_min_dot_C0;
        __simd128_float m_max_dot_C0 = simd128_set_same_float(- FLT_MAX);
        __simd128_float m_max_dot_C1 = m_max_dot_C0;
        __simd128_float m_max_dot_C2 = m_max_dot_C0;
        for (; i + 3 < n; i += 4) {
            __simd128_float m_vertex0 = simd128_load_float(&f1[i * 4]);
            __simd128_float m_vertex1 = simd128_load_float(&f1[i * 4 + 4]);
            __simd128_float m_vertex2 = simd128_load_float(&f1[i * 4 + 8]);
            __simd128_float m_vertex3 = simd128_load_float(&f1[i * 4 + 12]);
            __simd128_float m_v1_x, m_v1_y, m_v1_z;
            simd128_transpose4to3_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
                m_v1_x, m_v1_y, m_v1_z);
            __simd128_float m_dot = simd128_four_dot_products_vector3_vertical_float(
	        m_v1_x, m_v1_y, m_v1_z, m_v2_0_x, m_v2_0_y, m_v2_0_z);
            m_min_dot_C0 = simd128_min_float(m_min_dot_C0, m_dot);
            m_max_dot_C0 = simd128_max_float(m_max_dot_C0, m_dot);
            m_dot = simd128_four_dot_products_vector3_vertical_float(
	        m_v1_x, m_v1_y, m_v1_z, m_v2_1_x, m_v2_1_y, m_v2_1_z);
            m_min_dot_C1 = simd128_min_float(m_min_dot_C1, m_dot);
            m_max_dot_C1 = simd128_max_float(m_max_dot_C1, m_dot);
            m_dot = simd128_four_dot_products_vector3_vertical_float(
	        m_v1_x, m_v1_y, m_v1_z, m_v2_2_x, m_v2_2_y, m_v2_2_z);
            m_min_dot_C2 = simd128_min_float(m_min_dot_C2, m_dot);
            m_max_dot_C2 = simd128_max_float(m_max_dot_C2, m_dot);
        }
        __simd128_float shifted_float1, shifted_float2, shifted_float3;
        __simd128_float m_min_dot_23, m_max_dot_23;
        shifted_float1 = simd128_shift_right_float(m_min_dot_C0, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot_C0, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot_C0, 3);
        m_min_dot_C0 = simd128_min1_float(m_min_dot_C0, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot_C0 = simd128_min1_float(m_min_dot_C0, m_min_dot_23);
        min_dot_product[0] = simd128_get_float(m_min_dot_C0);
        shifted_float1 = simd128_shift_right_float(m_max_dot_C0, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot_C0, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot_C0, 3);
        m_min_dot_C0 = simd128_max1_float(m_max_dot_C0, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot_C0 = simd128_max1_float(m_max_dot_C0, m_max_dot_23);
        max_dot_product[0] = simd128_get_float(m_max_dot_C0);

        shifted_float1 = simd128_shift_right_float(m_min_dot_C1, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot_C1, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot_C1, 3);
        m_min_dot_C1 = simd128_min1_float(m_min_dot_C1, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot_C1 = simd128_min1_float(m_min_dot_C1, m_min_dot_23);
        min_dot_product[1] = simd128_get_float(m_min_dot_C1);
        shifted_float1 = simd128_shift_right_float(m_max_dot_C1, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot_C1, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot_C1, 3);
        m_max_dot_C1 = simd128_max1_float(m_max_dot_C1, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot_C1 = simd128_max1_float(m_max_dot_C1, m_max_dot_23);
        max_dot_product[1] = simd128_get_float(m_max_dot_C1);

        shifted_float1 = simd128_shift_right_float(m_min_dot_C2, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot_C2, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot_C2, 3);
        m_min_dot_C2 = simd128_min1_float(m_min_dot_C2, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot_C2 = simd128_min1_float(m_min_dot_C2, m_min_dot_23);
        min_dot_product[2] = simd128_get_float(m_min_dot_C2);
        shifted_float1 = simd128_shift_right_float(m_max_dot_C2, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot_C2, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot_C2, 3);
        m_max_dot_C2 = simd128_max1_float(m_max_dot_C2, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot_C2 = simd128_max1_float(m_max_dot_C2, m_max_dot_23);
        max_dot_product[2] = simd128_get_float(m_max_dot_C2);
    }
    else
        for (int j = 0; j < 3; j++) {
            min_dot_product[j] = FLT_MAX;
            max_dot_product[j] = - FLT_MAX;
        }
    // Process the remaining vertices.
    for (; i < n; i++)
        for (int j = 0; j < 3; j++) {
            min_dot_product[j] = minf(min_dot_product[j], dstDotVector3(&f1[i * 4], &f2[j * 3]));
            max_dot_product[j] = maxf(max_dot_product[j], dstDotVector3(&f1[i * 4], &f2[j * 3]));
        }
}

DST_API void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V4)(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product);

static DST_INLINE_ONLY void dstInlineCalculateMinAndMaxDotProductNx3V4(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product) {
    int i = 0;
    if (((uintptr_t)f1 & 0xF) == 0) {
        __simd128_float m_v2_0_x = simd128_set_same_float(f2[0]);
        __simd128_float m_v2_0_y = simd128_set_same_float(f2[1]);
        __simd128_float m_v2_0_z = simd128_set_same_float(f2[2]);
        __simd128_float m_v2_0_w = simd128_set_same_float(f2[3]);
        __simd128_float m_v2_1_x = simd128_set_same_float(f2[4]);
        __simd128_float m_v2_1_y = simd128_set_same_float(f2[5]);
        __simd128_float m_v2_1_z = simd128_set_same_float(f2[7]);
        __simd128_float m_v2_1_w = simd128_set_same_float(f2[7]);
        __simd128_float m_v2_2_x = simd128_set_same_float(f2[8]);
        __simd128_float m_v2_2_y = simd128_set_same_float(f2[9]);
        __simd128_float m_v2_2_z = simd128_set_same_float(f2[10]);
        __simd128_float m_v2_2_w = simd128_set_same_float(f2[11]);

    __simd128_float m_min_dot_C0 = simd128_set_same_float(FLT_MAX);
    __simd128_float m_min_dot_C1 = m_min_dot_C0;
    __simd128_float m_min_dot_C2 = m_min_dot_C0;
    __simd128_float m_max_dot_C0 = simd128_set_same_float(- FLT_MAX);
    __simd128_float m_max_dot_C1 = m_max_dot_C0;
    __simd128_float m_max_dot_C2 = m_max_dot_C0;
    for (; i + 3 < n; i += 4) {
        __simd128_float m_vertex0 = simd128_load_float(&f1[i * 4]);
        __simd128_float m_vertex1 = simd128_load_float(&f1[i * 4 + 4]);
        __simd128_float m_vertex2 = simd128_load_float(&f1[i * 4 + 8]);
        __simd128_float m_vertex3 = simd128_load_float(&f1[i * 4 + 12]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z, m_v1_w;
        simd128_transpose4to4_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
            m_v1_x, m_v1_y, m_v1_z, m_v1_w);
	__simd128_float m_dot = simd128_four_dot_products_vector4_vertical_float(
		m_v1_x, m_v1_y, m_v1_z, m_v1_w,
		m_v2_0_x, m_v2_0_y, m_v2_0_z, m_v2_0_w);
        m_min_dot_C0 = simd128_min_float(m_min_dot_C0, m_dot);
        m_max_dot_C0 = simd128_max_float(m_max_dot_C0, m_dot);
        m_dot = simd128_four_dot_products_vector4_vertical_float(
	    m_v1_x, m_v1_y, m_v1_z, m_v1_w,
	    m_v2_1_x, m_v2_1_y, m_v2_1_z, m_v2_1_w);
        m_min_dot_C1 = simd128_min_float(m_min_dot_C1, m_dot);
        m_max_dot_C1 = simd128_max_float(m_max_dot_C1, m_dot);
        m_dot = simd128_four_dot_products_vector4_vertical_float(
	    m_v1_x, m_v1_y, m_v1_z, m_v1_w,
	    m_v2_2_x, m_v2_2_y, m_v2_2_z, m_v2_2_w);
        m_min_dot_C2 = simd128_min_float(m_min_dot_C2, m_dot);
        m_max_dot_C2 = simd128_max_float(m_max_dot_C2, m_dot);
    }
    __simd128_float shifted_float1, shifted_float2, shifted_float3;
    __simd128_float m_min_dot_23, m_max_dot_23;
    shifted_float1 = simd128_shift_right_float(m_min_dot_C0, 1);
    shifted_float2 = simd128_shift_right_float(m_min_dot_C0, 2);
    shifted_float3 = simd128_shift_right_float(m_min_dot_C0, 3);
    m_min_dot_C0 = simd128_min1_float(m_min_dot_C0, shifted_float1);
    m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
    m_min_dot_C0 = simd128_min1_float(m_min_dot_C0, m_min_dot_23);
    min_dot_product[0] = simd128_get_float(m_min_dot_C0);
    shifted_float1 = simd128_shift_right_float(m_max_dot_C0, 1);
    shifted_float2 = simd128_shift_right_float(m_max_dot_C0, 2);
    shifted_float3 = simd128_shift_right_float(m_max_dot_C0, 3);
    m_min_dot_C0 = simd128_max1_float(m_max_dot_C0, shifted_float1);
    m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
    m_max_dot_C0 = simd128_max1_float(m_max_dot_C0, m_max_dot_23);
    max_dot_product[0] = simd128_get_float(m_max_dot_C0);

    shifted_float1 = simd128_shift_right_float(m_min_dot_C1, 1);
    shifted_float2 = simd128_shift_right_float(m_min_dot_C1, 2);
    shifted_float3 = simd128_shift_right_float(m_min_dot_C1, 3);
    m_min_dot_C1 = simd128_min1_float(m_min_dot_C1, shifted_float1);
    m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
    m_min_dot_C1 = simd128_min1_float(m_min_dot_C1, m_min_dot_23);
    min_dot_product[1] = simd128_get_float(m_min_dot_C1);
    shifted_float1 = simd128_shift_right_float(m_max_dot_C1, 1);
    shifted_float2 = simd128_shift_right_float(m_max_dot_C1, 2);
    shifted_float3 = simd128_shift_right_float(m_max_dot_C1, 3);
    m_max_dot_C1 = simd128_max1_float(m_max_dot_C1, shifted_float1);
    m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
    m_max_dot_C1 = simd128_max1_float(m_max_dot_C1, m_max_dot_23);
    max_dot_product[1] = simd128_get_float(m_max_dot_C1);

    shifted_float1 = simd128_shift_right_float(m_min_dot_C2, 1);
    shifted_float2 = simd128_shift_right_float(m_min_dot_C2, 2);
    shifted_float3 = simd128_shift_right_float(m_min_dot_C2, 3);
    m_min_dot_C2 = simd128_min1_float(m_min_dot_C2, shifted_float1);
    m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
    m_min_dot_C2 = simd128_min1_float(m_min_dot_C2, m_min_dot_23);
    min_dot_product[2] = simd128_get_float(m_min_dot_C2);
    shifted_float1 = simd128_shift_right_float(m_max_dot_C2, 1);
    shifted_float2 = simd128_shift_right_float(m_max_dot_C2, 2);
    shifted_float3 = simd128_shift_right_float(m_max_dot_C2, 3);
    m_max_dot_C2 = simd128_max1_float(m_max_dot_C2, shifted_float1);
    m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
    m_max_dot_C2 = simd128_max1_float(m_max_dot_C2, m_max_dot_23);
    max_dot_product[2] = simd128_get_float(m_max_dot_C2);
    }
    else
    for (int j = 0; j < 3; j++) {
        min_dot_product[j] = FLT_MAX;
        max_dot_product[j] = - FLT_MAX;
    }
    // Process the remaining vertices.
    for (; i < n; i++)
        for (int j = 0; j < 3; j++) {
            min_dot_product[j] = minf(min_dot_product[j], dstDotVector4(&f1[i * 4], &f2[j * 4]));
            max_dot_product[j] = maxf(max_dot_product[j], dstDotVector4(&f1[i * 4], &f2[j * 4]));
        }
}

// Determine the indices within an array of vertices that have the minimum and
// maximum dot product with a given constant vector.

DST_API void SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V3)(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax);

static DST_INLINE_ONLY void dstInlineGetIndicesWithMinAndMaxDotProductNx1V3(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax) {
    int i = 0;
    float min_dot_product;
    float max_dot_product;
    if (((uintptr_t)f1 & 0xF) == 0) {
    __simd128_float m_v2_x = simd128_set_same_float(f2[0]);
    __simd128_float m_v2_y = simd128_set_same_float(f2[1]);
    __simd128_float m_v2_z = simd128_set_same_float(f2[2]);
    __simd128_float m_min_dot = simd128_set_same_float(FLT_MAX);
    __simd128_float m_max_dot = simd128_set_same_float(- FLT_MAX);
    __simd128_int m_min_dot_index = simd128_set_zero_int();
    __simd128_int m_max_dot_index = simd128_set_zero_int();
    // Keep track of four minimum and four maximum dot products (each representing the
    // min/max for a quarter of the vertices).
    for (; i + 3 < n; i += 4) {
        __simd128_float m_vertex0 = simd128_load_unaligned_float(&f1[i * 3]);
        __simd128_float m_vertex1 = simd128_load_unaligned_float(&f1[i * 3 + 3]);
        __simd128_float m_vertex2 = simd128_load_unaligned_float(&f1[i * 3 + 6]);
        __simd128_float m_vertex3 = simd128_load_unaligned_float(&f1[i * 3 + 9]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
            m_v1_x, m_v1_y, m_v1_z);
        __simd128_float m_dot = simd128_four_dot_products_vector3_vertical_float(
            m_v1_x, m_v1_y, m_v1_z, m_v2_x, m_v2_y, m_v2_z);
        m_min_dot = simd128_min_float(m_min_dot, m_dot);
        m_max_dot = simd128_max_float(m_max_dot, m_dot);
        // Update the index for each component for which the min or max was updated.
        __simd128_int comp_min, comp_max;
        __simd128_int indices = simd128_set_int32(i, i + 1, i + 2, i + 3);
        // Set the bitmask for components for which the minimum/maximum was updated.
        comp_min = simd128_cmpeq_float(m_min_dot, m_dot);
        comp_max = simd128_cmpeq_float(m_max_dot, m_dot);
        m_min_dot_index = simd128_or_int(
            simd128_andnot_int(comp_min, m_min_dot_index),
            simd128_and_int(comp_min, indices)
            );
        m_max_dot_index = simd128_or_int(
            simd128_andnot_int(comp_max, m_max_dot_index),
            simd128_and_int(comp_max, indices)
            );
    }
    // Combine the minimum/maximum calculated for each quarter of the vertices.
    min_dot_product = simd128_get_float(m_min_dot);
    i_Pmin = simd128_get_int32(m_min_dot_index);
    max_dot_product = simd128_get_float(m_max_dot);
    i_Pmax = simd128_get_int32(m_max_dot_index);
    for (int j = 1; j < 4; j++) {
        m_min_dot = simd128_shift_right_float(m_min_dot, 1);
        m_max_dot = simd128_shift_right_float(m_max_dot, 1);
        m_min_dot_index = simd128_shift_right_bytes_int(m_min_dot_index, 4);
        m_max_dot_index = simd128_shift_right_bytes_int(m_max_dot_index, 4);
        float v_min = simd128_get_float(m_min_dot);
        float v_max = simd128_get_float(m_max_dot);
        if (v_min < min_dot_product) {
            min_dot_product = v_min;
            i_Pmin = simd128_get_int32(m_min_dot_index);
        }
        if (v_max > max_dot_product) {
            max_dot_product = v_max;
            i_Pmax = simd128_get_int32(m_max_dot_index);
        }
    }
    }
    else
    {
        min_dot_product = FLT_MAX;
        max_dot_product = - FLT_MAX;
    }
    for (; i < n; i++) {
        float dot = dstDotVector3(&f1[i * 3], f2);
        if (dot < min_dot_product) {
            min_dot_product = dot;
            i_Pmin = i;
        }
        if (dot > max_dot_product) {
            max_dot_product = dot;
            i_Pmax = i;
        }
    }
}

DST_API void SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V3P)(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax);

static DST_INLINE_ONLY void dstInlineGetIndicesWithMinAndMaxDotProductNx1V3P(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax) {
    int i = 0;
    float min_dot_product;
    float max_dot_product;
    if (((uintptr_t)f1 & 0xF) == 0) {
    __simd128_float m_v2_x = simd128_set_same_float(f2[0]);
    __simd128_float m_v2_y = simd128_set_same_float(f2[1]);
    __simd128_float m_v2_z = simd128_set_same_float(f2[2]);
    __simd128_float m_min_dot = simd128_set_same_float(FLT_MAX);
    __simd128_float m_max_dot = simd128_set_same_float(- FLT_MAX);
    __simd128_int m_min_dot_index = simd128_set_zero_int();
    __simd128_int m_max_dot_index = simd128_set_zero_int();
    // Keep track of four minimum and four maximum dot products (each representing the
    // min/max for a quarter of the vertices).
    for (; i + 3 < n; i += 4) {
        __simd128_float m_vertex0 = simd128_load_unaligned_float(&f1[i * 4]);
        __simd128_float m_vertex1 = simd128_load_unaligned_float(&f1[i * 4 + 4]);
        __simd128_float m_vertex2 = simd128_load_unaligned_float(&f1[i * 4 + 8]);
        __simd128_float m_vertex3 = simd128_load_unaligned_float(&f1[i * 4 + 12]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
            m_v1_x, m_v1_y, m_v1_z);
        __simd128_float m_dot = simd128_four_dot_products_vector3_vertical_float(
            m_v1_x, m_v1_y, m_v1_z, m_v2_x, m_v2_y, m_v2_z);
        m_min_dot = simd128_min_float(m_min_dot, m_dot);
        m_max_dot = simd128_max_float(m_max_dot, m_dot);
        // Update the index for each component for which the min or max was updated.
        __simd128_int comp_min, comp_max;
        __simd128_int indices = simd128_set_int32(i, i + 1, i + 2, i + 3);
        // Set the bitmask for components for which the minimum/maximum was updated.
        comp_min = simd128_cmpeq_float(m_min_dot, m_dot);
        comp_max = simd128_cmpeq_float(m_max_dot, m_dot);
        m_min_dot_index = simd128_or_int(
            simd128_andnot_int(comp_min, m_min_dot_index),
            simd128_and_int(comp_min, indices)
            );
        m_max_dot_index = simd128_or_int(
            simd128_andnot_int(comp_max, m_max_dot_index),
            simd128_and_int(comp_max, indices)
            );
    }
    // Combine the minimum/maximum calculated for each quarter of the vertices.
    min_dot_product = simd128_get_float(m_min_dot);
    i_Pmin = simd128_get_int32(m_min_dot_index);
    max_dot_product = simd128_get_float(m_max_dot);
    i_Pmax = simd128_get_int32(m_max_dot_index);
    for (int j = 1; j < 4; j++) {
        m_min_dot = simd128_shift_right_float(m_min_dot, 1);
        m_max_dot = simd128_shift_right_float(m_max_dot, 1);
        m_min_dot_index = simd128_shift_right_bytes_int(m_min_dot_index, 4);
        m_max_dot_index = simd128_shift_right_bytes_int(m_max_dot_index, 4);
        float v_min = simd128_get_float(m_min_dot);
        float v_max = simd128_get_float(m_max_dot);
        if (v_min < min_dot_product) {
            min_dot_product = v_min;
            i_Pmin = simd128_get_int32(m_min_dot_index);
        }
        if (v_max > max_dot_product) {
            max_dot_product = v_max;
            i_Pmax = simd128_get_int32(m_max_dot_index);
        }
    }
    }
    else
    {
        min_dot_product = FLT_MAX;
        max_dot_product = - FLT_MAX;
    }
    for (; i < n; i++) {
        float dot = dstDotVector3(&f1[i * 4], f2);
        if (dot < min_dot_product) {
            min_dot_product = dot;
            i_Pmin = i;
        }
        if (dot > max_dot_product) {
            max_dot_product = dot;
            i_Pmax = i;
        }
    }
}

DST_API void SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V4)(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax);

static DST_INLINE_ONLY void dstInlineGetIndicesWithMinAndMaxDotProductNx1V4(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax) {
    int i = 0;
    float min_dot_product;
    float max_dot_product;
    if (((uintptr_t)f1 & 0xF) == 0) {
    __simd128_float m_v2 = simd128_load_unaligned_float(f2);
    __simd128_float m_v2_x = simd128_replicate_float(m_v2, 0);
    __simd128_float m_v2_y = simd128_replicate_float(m_v2, 1);
    __simd128_float m_v2_z = simd128_replicate_float(m_v2, 2);
    __simd128_float m_v2_w = simd128_replicate_float(m_v2, 3);
    __simd128_float m_min_dot = simd128_set_same_float(FLT_MAX);
    __simd128_float m_max_dot = simd128_set_same_float(- FLT_MAX);
    __simd128_int m_min_dot_index = simd128_set_zero_int();
    __simd128_int m_max_dot_index = simd128_set_zero_int();
    // Keep track of four minimum and four maximum dot products (each representing the
    // min/max for a quarter of the vertices).
    for (; i + 3 < n; i += 4) {
        __simd128_float m_vertex0 = simd128_load_float(&f1[i * 4]);
        __simd128_float m_vertex1 = simd128_load_float(&f1[i * 4 + 4]);
        __simd128_float m_vertex2 = simd128_load_float(&f1[i * 4 + 8]);
        __simd128_float m_vertex3 = simd128_load_float(&f1[i * 4 + 12]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z, m_v1_w;
        simd128_transpose4to4_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
            m_v1_x, m_v1_y, m_v1_z, m_v1_w);
	__simd128_float m_dot = simd128_four_dot_products_vector4_vertical_float(
		m_v1_x, m_v1_y, m_v1_z, m_v1_w,
		m_v2_x, m_v2_y, m_v2_z, m_v2_w);
        m_min_dot = simd128_min_float(m_min_dot, m_dot);
        m_max_dot = simd128_max_float(m_max_dot, m_dot);
        // Update the index for each component for which the min or max was updated.
        __simd128_int comp_min, comp_max;
        __simd128_int indices = simd128_set_int32(i, i + 1, i + 2, i + 3);
        // Set the bitmask for components for which the minimum/maximum was updated.
        comp_min = simd128_cmpeq_float(m_min_dot, m_dot);
        comp_max = simd128_cmpeq_float(m_max_dot, m_dot);
        m_min_dot_index = simd128_or_int(
            simd128_andnot_int(comp_min, m_min_dot_index),
            simd128_and_int(comp_min, indices)
            );
        m_max_dot_index = simd128_or_int(
            simd128_andnot_int(comp_max, m_max_dot_index),
            simd128_and_int(comp_max, indices)
            );
    }
    // Combine the minimum/maximum calculated for each quarter of the vertices.
    min_dot_product = simd128_get_float(m_min_dot);
    i_Pmin = simd128_get_int32(m_min_dot_index);
    max_dot_product = simd128_get_float(m_max_dot);
    i_Pmax = simd128_get_int32(m_max_dot_index);
    for (int j = 1; j < 4; j++) {
        m_min_dot = simd128_shift_right_float(m_min_dot, 1);
        m_max_dot = simd128_shift_right_float(m_max_dot, 1);
        m_min_dot_index = simd128_shift_right_bytes_int(m_min_dot_index, 4);
        m_max_dot_index = simd128_shift_right_bytes_int(m_max_dot_index, 4);
        float v_min = simd128_get_float(m_min_dot);
        float v_max = simd128_get_float(m_max_dot);
        if (v_min < min_dot_product) {
            min_dot_product = v_min;
            i_Pmin = simd128_get_int32(m_min_dot_index);
        }
        if (v_max > max_dot_product) {
            max_dot_product = v_max;
            i_Pmax = simd128_get_int32(m_max_dot_index);
        }
    }
    }
    else
    {
        min_dot_product = FLT_MAX;
        max_dot_product = - FLT_MAX;
    }
    for (; i < n; i++) {
        float dot = dstDotVector4(&f1[i * 4], f2);;
        if (dot < min_dot_product) {
            min_dot_product = dot;
            i_Pmin = i;
        }
        if (dot > max_dot_product) {
            max_dot_product = dot;
            i_Pmax = i;
        }
    }
}

#endif // defined(__DST_SIMD_DOT_H__)

