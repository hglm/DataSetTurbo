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

// Calculate dot products of four four-component float vectors stored at f1 and f2.
//
// void simd_four_dot_products_vector4_float(
//     const float * __restrict f1, const float * __restrict f2, __simd128_float& result);
// inline void simd_inline_four_dot_products_vector4_float(
//     const float * __restrict f1, const float * __restrict f2, __simd128_float& result);
//
// Calculate dot products of four three-component float vectors stored at f1 and f2. The
// vectors are assumed to be stored in 128-bit fields (the last four bytes are unused).
//
// void simd_four_dot_products_vector3_storage4_float(const float * __restrict f1,
//     const float * __restrict f2, __simd128_float& result);
// inline void simd_inline_four_dot_products_vector3_storage4_float(
//     const float * __restrict f1, const float * __restrict f2, __simd128_float& result);
//
// Calculate dot products of four three-component float vectors stored at f1 and f2. The
// vectors are assumed to be stored consecutively in packed 12-byte format.
//
// void simd_four_dot_products_vector3_storage3_float(const float * __restrict f1,
//     const float * __restrict f2, __simd128_float& result);
// inline void simd_inline_four_dot_products_vector3_storage3_float(const float * __restrict f1,
//     const float * __restrict f2, __simd128_float& result);
//
// Calculate n dot products from one array of four vectors and one single vector,
// and store the result in an array of floats.
//
// void simd_dot_product_nx1_vector4_float(int n,
//     const float * __restrict f1, const float * __restrict f2, float * __restrict dot);
// inline void simd_inline_dot_product_nx1_vector4_float(int n,
//     const float * __restrict f1, const float * __restrict f2, float * __restrict dot);
//
// Calculate dot products from one array of vectors with three floats with one four-float
// vector.
// When vectors of three floats are stored in 16-byte aligned format,
// simd128_load_float can be used but the fourth element of the SIMD register
// needs to be initialized with 0.0f (v2 only).
//
// void simd_dot_product_nx1_vector3_storage4_vector4_float(int n,
//     const float * __restrict f1, const float * __restrict f2, float * __restrict dot);
// inline void simd_inline_dot_product_nx1_vector3_storage4_vector4_float(int n,
//     const float * __restrict f1, const __simd128_float m_v2, float * __restrict dot);
// inline void simd_inline_dot_product_nx1_vector3_storage4_vector4_float(int n,
//     const float * __restrict f1, const float * __restrict f2, float * __restrict dot);
//
// The array of vectors is an array of points with implicit w coordinate of 1.0f,
// and is stored aligned on 16-byte boundaries. The constant vector has four
// components.
//
// void simd_dot_product_nx1_point3_storage4_vector4_float(int n,
//     const float * __restrict f1, const float * __restrict f2, float * __restrict dot);
// inline void simd_inline_dot_product_nx1_point3_storage4_vector4_float(int n,
//     const float * __restrict f1, const __simd128_float m_v2, float * __restrict dot);
// inline void simd_inline_dot_product_nx1_point3_storage4_vector4_float(int n,
//     const float * __restrict f1, const float * __restrict f2, float * __restrict dot)
//
// The array of vectors is an array of points with implicit w coordinate of 1.0f,
// and is stored unaligned on 12-byte boundaries. The constant vector has four
// components.
//
// void simd_dot_product_nx1_point3_storage3_vector4_float(int n,
//     const float * __restrict f1, const float * __restrict f2, float * __restrict dot);
// inline void simd_inline_dot_product_nx1_point3_storage3_vector4_float(int n,
//     const float * __restrict f1, const __simd128_float m_v2, float * __restrict dot);
// inline_only void simd_inline_dot_product_nx1_point3_storage3_vector4_float(int n,
//     const float * __restrict f1, const float * __restrict f2, float * __restrict dot);
//
// Calculate n dot products from one array of n point vectors stored on 16-byte
// boundaries and one single four-element vector, store the result in an array of
// floats, and count the number of dot products that is smaller than zero.
//
// void simd_dot_product_nx1_point3_storage4_vector4_and_count_negative_float(
//     int n, const float * __restrict f1, const float * __restrict f2, float * __restrict dot,
//     int& negative_count);
//
// inline void simd_inline_dot_product_nx1_point3_storage4_vector4_and_count_negative_float(
//     int n, const float * __restrict f1, const float * __restrict f2, float * __restrict dot,
//     int& negative_count);
//
// Similar to the previous function, but the point vectors are packed at 12-byte
// boundaries.
//
// void simd_dot_product_nx1_point3_storage3_vector4_and_count_negative_float(
//     int n, const float * __restrict f1, const float * __restrict f2, float * __restrict dot,
//     int& negative_count);
// inline void simd_inline_dot_product_nx1_point3_storage3_vector4_and_count_negative_float(
//     int n, const float * __restrict f1, const float * __restrict f2, float * __restrict dot,
//     int& negative_count);

// Calculate dot products of four four-component float vectors stored at f1 and f2. Both f1 and
// f2 must be 16-byte aligned.

void simd_four_dot_products_vector4_float(
const float * __restrict f1, const float * __restrict f2, __simd128_float& result);

static inline_only void simd_inline_four_dot_products_vector4_float(
const float * __restrict f1, const float * __restrict f2, __simd128_float& result) {
#if 1
    // Loading and using tranpose seems to be more efficient than set_float calls, which
    // expand to numerous instructions.
    __simd128_float m_v1_0 = simd128_load_float(&f1[0]);
    __simd128_float m_v1_1 = simd128_load_float(&f1[4]);
    __simd128_float m_v1_2 = simd128_load_float(&f1[8]);
    __simd128_float m_v1_3 = simd128_load_float(&f1[12]);
    __simd128_float m_v2_0 = simd128_load_float(&f2[0]);
    __simd128_float m_v2_1 = simd128_load_float(&f2[4]);
    __simd128_float m_v2_2 = simd128_load_float(&f2[8]);
    __simd128_float m_v2_3 = simd128_load_float(&f2[12]);
    __simd128_float m_v1_x, m_v1_y, m_v1_z, m_v1_w;
    simd128_transpose4to4_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z, m_v1_w);
    __simd128_float m_v2_x, m_v2_y, m_v2_z, m_v2_w;
    simd128_transpose4to4_float(m_v2_0, m_v2_1, m_v2_2, m_v2_3, m_v2_x, m_v2_y, m_v2_z, m_v2_w);
#else
    __simd128_float m_v1_x = simd128_set_float(f1[0], f1[4], f1[8], f1[12]);
    __simd128_float m_v1_y = simd128_set_float(f1[1], f1[5], f1[9], f1[13]);
    __simd128_float m_v1_z = simd128_set_float(f1[2], f1[6], f1[10], f1[14]);
    __simd128_float m_v1_w = simd128_set_float(f1[3], f1[7], f1[11], f1[15);
    __simd128_float m_v2_x = simd128_set_float(f2[0], f2[4], f2[8], f2[12]);
    __simd128_float m_v2_y = simd128_set_float(f2[1], f2[5], f2[9], f2[13]);
    __simd128_float m_v2_z = simd128_set_float(f2[2], f2[6], f2[10], f2[14]);
    __simd128_float m_v2_w = simd128_set_float(f2[3], f2[7], f2[11], f2[15]);
#endif
    __simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
    __simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
    __simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
    __simd128_float m_dot_w = simd128_mul_float(m_v1_w, m_v2_w);
    result = simd128_add_float(
        simd128_add_float(m_dot_x, m_dot_y),
        simd128_add_float(m_dot_z, m_dot_w)
        );
}

// Calculate dot products of four three-component float vectors stored at f1 and f2. The
// vectors are assumed to be stored in 128-bit fields (the last four bytes are unused).
// Both f1 and f2 must be 16-byte aligned.

void simd_four_dot_products_vector3_storage4_float(const float * __restrict f1,
const float * __restrict f2, __simd128_float& result);

static inline_only void simd_inline_four_dot_products_vector3_storage4_float(
const float * __restrict f1, const float * __restrict f2, __simd128_float& result) {
#if 1
    __simd128_float m_v1_0 = simd128_load_float(&f1[0]);
    __simd128_float m_v1_1 = simd128_load_float(&f1[4]);
    __simd128_float m_v1_2 = simd128_load_float(&f1[8]);
    __simd128_float m_v1_3 = simd128_load_float(&f1[12]);
    __simd128_float m_v2_0 = simd128_load_float(&f2[0]);
    __simd128_float m_v2_1 = simd128_load_float(&f2[4]);
    __simd128_float m_v2_2 = simd128_load_float(&f2[8]);
    __simd128_float m_v2_3 = simd128_load_float(&f2[12]);
    __simd128_float m_v1_x, m_v1_y, m_v1_z;
    simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
    __simd128_float m_v2_x, m_v2_y, m_v2_z;
    simd128_transpose4to3_float(m_v2_0, m_v2_1, m_v2_2, m_v2_3, m_v2_x, m_v2_y, m_v2_z);
#else
    __simd128_float m_v1_x = simd128_set_float(f1[0], f1[4], f1[8], f1[12]);
    __simd128_float m_v1_y = simd128_set_float(f1[1], f1[5], f1[9], f1[13]);
    __simd128_float m_v1_z = simd128_set_float(f1[2], f1[6], f1[10], f1[14]);
    __simd128_float m_v2_x = simd128_set_float(f2[0], f2[4], f2[8], f2[12]);
    __simd128_float m_v2_y = simd128_set_float(f2[1], f2[5], f2[9], f2[13]);
    __simd128_float m_v2_z = simd128_set_float(f2[2], f2[6], f2[10], f2[14]);
#endif
    __simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
    __simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
    __simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
    result = simd128_add_float(
        simd128_add_float(m_dot_x, m_dot_y),
        m_dot_z
        );
}

// Calculate dot products of four three-component float vectors stored at f1 and f2. The
// vectors are assumed to be stored consecutively in packed 12-byte format.

void simd_four_dot_products_vector3_storage3_float(const float * __restrict f1,
const float * __restrict f2, __simd128_float& result);

static inline_only void simd_inline_four_dot_products_vector3_storage3_float(const float * __restrict f1,
const float * __restrict f2, __simd128_float& result) {
#if 1
    __simd128_float m_v1_0 = simd128_load3_float(&f1[0]);
    __simd128_float m_v1_1 = simd128_load3_float(&f1[4]);
    __simd128_float m_v1_2 = simd128_load3_float(&f1[8]);
    __simd128_float m_v1_3 = simd128_load3_float(&f1[12]);
    __simd128_float m_v2_0 = simd128_load3_float(&f2[0]);
    __simd128_float m_v2_1 = simd128_load3_float(&f2[4]);
    __simd128_float m_v2_2 = simd128_load3_float(&f2[8]);
    __simd128_float m_v2_3 = simd128_load3_float(&f2[12]);
    __simd128_float m_v1_x, m_v1_y, m_v1_z;
    simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
    __simd128_float m_v2_x, m_v2_y, m_v2_z;
    simd128_transpose4to3_float(m_v2_0, m_v2_1, m_v2_2, m_v2_3, m_v2_x, m_v2_y, m_v2_z);
#else
    __simd128_float m_v1_x = simd128_set_float(f1[0], f1[4], f1[8], f1[12]);
    __simd128_float m_v1_y = simd128_set_float(f1[1], f1[5], f1[9], f1[13]);
    __simd128_float m_v1_z = simd128_set_float(f1[2], f1[6], f1[10], f1[14]);
    __simd128_float m_v2_x = simd128_set_float(f2[0], f2[4], f2[8], f2[12]);
    __simd128_float m_v2_y = simd128_set_float(f2[1], f2[5], f2[9], f2[13]);
    __simd128_float m_v2_z = simd128_set_float(f2[2], f2[6], f2[10], f2[14]);
#endif
    __simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
    __simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
    __simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
    result = simd128_add_float(
        simd128_add_float(m_dot_x, m_dot_y),
        m_dot_z
        );
}

// Calculate n dot products from one array of four vectors and one single vector,
// and store the result in an array of floats.
// f1, f2 and dot must all be 16-bytes aligned.

void simd_dot_product_nx1_vector4_float(int n,
const float * __restrict f1, const float * __restrict f2, float * __restrict dot);

static inline_only void simd_inline_dot_product_nx1_vector4_float(int n,
const float * __restrict f1, const float * __restrict f2, float * __restrict dot) {
    __simd128_float m_v2 = simd128_load_float(f2);
    __simd128_float m_v2_x, m_v2_y, m_v2_z, m_v2_w;
    m_v2_x = simd128_select_float(m_v2, 0, 0, 0, 0);
    m_v2_y = simd128_select_float(m_v2, 1, 1, 1, 1);
    m_v2_z = simd128_select_float(m_v2, 2, 2, 2, 2);
    m_v2_w = simd128_select_float(m_v2, 3, 3, 3, 3);
    int i = 0;
    for (; i + 3 < n; i += 4) {
        // Loading and using transpose seems to be more efficient than set_float calls,
        // which expand to numerous instructions.
        __simd128_float m_v1_0 = simd128_load_float(&f1[i * 4 + 0]);
        __simd128_float m_v1_1 = simd128_load_float(&f1[i * 4 + 4]);
        __simd128_float m_v1_2 = simd128_load_float(&f1[i * 4 + 8]);
        __simd128_float m_v1_3 = simd128_load_float(&f1[i * 4 + 12]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z, m_v1_w;
        simd128_transpose4to4_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3,
            m_v1_x, m_v1_y, m_v1_z, m_v1_w);
        __simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
        __simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
        __simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
        __simd128_float m_dot_w = simd128_mul_float(m_v1_w, m_v2_w);
        __simd128_float m_result = simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y),
            simd128_add_float(m_dot_z, m_dot_w)
            );
        simd128_store_float(&dot[i], m_result);
    }
    for (; i < n; i++) {
        __simd128_float m_v1 = simd128_load_float(&f1[i * 4]);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_result = simd128_horizontal_add4_float(m_dot);
        simd128_store_first_float(&dot[i], m_result);
    }
}

// When vectors of three floats are stored in packed format, simd128_load3_float
// will make sure the fourth component of the SIMD register is set to 0.0f.

void simd_dot_product_nx1_vector3_storage3_float(int n,
const float * __restrict f1, const float * __restrict f2, float * __restrict dot);

static inline_only void simd_inline_dot_product_nx1_vector3_storage3_float(int n,
const float * __restrict f1, const __simd128_float m_v2, float * __restrict dot) {
    __simd128_float m_v2_x, m_v2_y, m_v2_z;
    m_v2_x = simd128_select_float(m_v2, 0, 0, 0, 0);
    m_v2_y = simd128_select_float(m_v2, 1, 1, 1, 1);
    m_v2_z = simd128_select_float(m_v2, 2, 2, 2, 2);
    int i = 0;
    for (; i + 3 < n; i += 4) {
        __simd128_float m_v1_0 = simd128_load3_float(&f1[i * 3 + 0]);
        __simd128_float m_v1_1 = simd128_load3_float(&f1[i * 3 + 3]);
        __simd128_float m_v1_2 = simd128_load3_float(&f1[i * 3 + 6]);
        __simd128_float m_v1_3 = simd128_load3_float(&f1[i * 3 + 9]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
        __simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
        __simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
        __simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
        __simd128_float m_result = simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y),
            m_dot_z
            );
        simd128_store_float(&dot[i], m_result);
    }
    for (; i < n; i++) {
        __simd128_float m_v1 = simd128_load3_float(&f1[i * 3]);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_result = simd128_horizontal_add4_float(m_dot);
        simd128_store_first_float(&dot[i], m_result);
    }
}

static inline_only void simd_inline_dot_product_nx1_vector3_storage3_float(int n,
const float * __restrict f1, const float * __restrict f2, float * __restrict dot) {
    __simd128_float m_v2 = simd128_load3_float(f2);
    simd_inline_dot_product_nx1_vector3_storage3_float(n, f1, m_v2, dot);
}

// When vectors of three floats are stored in 16-byte aligned format,
// simd128_load_float can be used but the fourth element of the SIMD register
// needs to be initialized with 0.0f (v2 only).
// f1, f2 and dot must all be 16-bytes aligned.

void simd_dot_product_nx1_vector3_storage4_vector4_float(int n,
const float * __restrict f1, const float * __restrict f2, float * __restrict dot);

static inline_only void simd_inline_dot_product_nx1_vector3_storage4_vector4_float(int n,
const float * __restrict f1, const __simd128_float m_v2, float * __restrict dot) {
    __simd128_float m_v2_x, m_v2_y, m_v2_z;
    m_v2_x = simd128_select_float(m_v2, 0, 0, 0, 0);
    m_v2_y = simd128_select_float(m_v2, 1, 1, 1, 1);
    m_v2_z = simd128_select_float(m_v2, 2, 2, 2, 2);
    int i = 0;
    for (; i + 3 < n; i += 4) {
        __simd128_float m_v1_0 = simd128_load_float(&f1[i * 4 + 0]);
        __simd128_float m_v1_1 = simd128_load_float(&f1[i * 4 + 4]);
        __simd128_float m_v1_2 = simd128_load_float(&f1[i * 4 + 8]);
        __simd128_float m_v1_3 = simd128_load_float(&f1[i * 4 + 12]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
        __simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
        __simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
        __simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
        __simd128_float m_result = simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y),
            m_dot_z
            );
        simd128_store_float(&dot[i], m_result);
    }
    for (; i < n; i++) {
        __simd128_float m_v1 = simd128_set_last_zero_float(simd128_load_float(&f1[i * 4]));
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_result = simd128_horizontal_add4_float(m_dot);
        simd128_store_first_float(&dot[i], m_result);
    }
}

static inline_only void simd_inline_dot_product_nx1_vector3_storage4_vector4_float(int n,
const float * __restrict f1, const float * __restrict f2, float * __restrict dot) {
    __simd128_float m_v2 = simd128_load_float(f2);
    // The w component as loaded is undefined.
    // Set w to 0.0f.
    __simd128_float m_zeros = simd128_set_zero_float();
    __simd128_int m_mask_012x = simd128_shift_right_bytes_int(
        simd128_set_same_int32(0xFFFFFFFF), 4);
    m_v2 = simd128_cast_int_float(simd128_or_int(
        simd128_and_int(m_mask_012x, simd128_cast_float_int(m_v2)),
        simd128_andnot_int(m_mask_012x, simd128_cast_float_int(m_zeros))
        ));
    simd_inline_dot_product_nx1_vector3_storage4_vector4_float(n, f1, m_v2, dot);
}

// The array of vectors is an array of points with implicit w coordinate of 1.0f,
// and is stored aligned on 16-byte boundaries. The constant vector has four
// components.
// f1, f2 and dot must all be 16-bytes aligned.

void simd_dot_product_nx1_point3_storage4_vector4_float(int n,
const float * __restrict f1, const float * __restrict f2, float * __restrict dot);

static inline_only void simd_inline_dot_product_nx1_point3_storage4_vector4_float(int n,
const float * __restrict f1, const __simd128_float m_v2, float * __restrict dot) {
    __simd128_float m_v2_x, m_v2_y, m_v2_z, m_v2_w;
    m_v2_x = simd128_select_float(m_v2, 0, 0, 0, 0);
    m_v2_y = simd128_select_float(m_v2, 1, 1, 1, 1);
    m_v2_z = simd128_select_float(m_v2, 2, 2, 2, 2);
    m_v2_w = simd128_select_float(m_v2, 3, 3, 3, 3);
    int i = 0;
    for (; i + 3 < n; i += 4) {
        __simd128_float m_v1_0 = simd128_load_float(&f1[i * 4 + 0]);
        __simd128_float m_v1_1 = simd128_load_float(&f1[i * 4 + 4]);
        __simd128_float m_v1_2 = simd128_load_float(&f1[i * 4 + 8]);
        __simd128_float m_v1_3 = simd128_load_float(&f1[i * 4 + 12]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
        __simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
        __simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
        __simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
        // m_v1_w would contain only 1.0f so m_dot_w is equal to m_v2_w.
        __simd128_float m_result = simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y),
            simd128_add_float(m_dot_z, m_v2_w)
            );
        simd128_store_float(&dot[i], m_result);
    }
    for (; i < n; i++) {
         // Load the point vector and make sure the w component is 1.0f.
        __simd128_float m_v1 = simd128_set_last_float(
            simd128_load_float(&f1[i * 4]), 1.0f);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_result = simd128_horizontal_add4_float(m_dot);
        simd128_store_first_float(&dot[i], m_result);
    }
}

static inline_only void simd_inline_dot_product_nx1_point3_storage4_vector4_float(int n,
const float * __restrict f1, const float * __restrict f2, float * __restrict dot) {
    __simd128_float m_v2 = simd128_load_float(f2);
    simd_inline_dot_product_nx1_point3_storage4_vector4_float(n, f1, m_v2, dot);
}

// The array of vectors is an array of points with implicit w coordinate of 1.0f,
// and is stored unaligned on 12-byte boundaries. The constant vector has four
// components.
// f2 and dot must be 16-bytes aligned.


void simd_dot_product_nx1_point3_storage3_vector4_float(int n,
const float * __restrict f1, const float * __restrict f2, float * __restrict dot);

static inline_only void simd_inline_dot_product_nx1_point3_storage3_vector4_float(int n,
const float * __restrict f1, const __simd128_float m_v2, float * __restrict dot) {
    __simd128_float m_v2_x, m_v2_y, m_v2_z, m_v2_w;
    m_v2_x = simd128_select_float(m_v2, 0, 0, 0, 0);
    m_v2_y = simd128_select_float(m_v2, 1, 1, 1, 1);
    m_v2_z = simd128_select_float(m_v2, 2, 2, 2, 2);
    m_v2_w = simd128_select_float(m_v2, 3, 3, 3, 3);
    int i = 0;
    for (; i + 3 < n; i += 4) {
        __simd128_float m_v1_0 = simd128_load3_float(&f1[i * 3 + 0]);
        __simd128_float m_v1_1 = simd128_load3_float(&f1[i * 3 + 3]);
        __simd128_float m_v1_2 = simd128_load3_float(&f1[i * 3 + 6]);
        __simd128_float m_v1_3 = simd128_load3_float(&f1[i * 3 + 9]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
        __simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
        __simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
        __simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
        // m_v1_w would contain only 1.0f so m_dot_w is equal to m_v2_w.
        __simd128_float m_result = simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y),
            simd128_add_float(m_dot_z, m_v2_w)
            );
        simd128_store_float(&dot[i], m_result);
    }
    for (; i < n; i++) {
         // Load the point vector and make sure the w component is 1.0f.
        __simd128_float m_v1 = simd128_set_last_float(
            simd128_load3_float(&f1[i * 3]), 1.0f);
        __simd128_float m_dot = simd128_mul_float(m_v1, m_v2);
        __simd128_float m_result = simd128_horizontal_add4_float(m_dot);
        simd128_store_first_float(&dot[i], m_result);
    }
}

static inline_only void simd_inline_dot_product_nx1_point3_storage3_vector4_float(int n,
const float * __restrict f1, const float * __restrict f2, float * __restrict dot) {
    __simd128_float m_v2 = simd128_load_float(f2);
    simd_inline_dot_product_nx1_point3_storage3_vector4_float(n, f1, m_v2, dot);
}

// Calculate n dot products from one array of n point vectors stored on 16-byte
// boundaries and one single four-element vector, store the result in an array of
// floats, and count the number of dot products that is smaller than zero.
// f1, f2 and dot must all be 16-bytes aligned.

void simd_dot_product_nx1_point3_storage4_vector4_and_count_negative_float(
int n, const float * __restrict f1, const float * __restrict f2, float * __restrict dot,
int& negative_count);

static inline_only void simd_inline_dot_product_nx1_point3_storage4_vector4_and_count_negative_float(
int n, const float * __restrict f1, const float * __restrict f2, float * __restrict dot,
int& negative_count) {
    __simd128_float m_v2 = simd128_load_float(f2);
    __simd128_float m_v2_x, m_v2_y, m_v2_z, m_v2_w;
    m_v2_x = simd128_select_float(m_v2, 0, 0, 0, 0);
    m_v2_y = simd128_select_float(m_v2, 1, 1, 1, 1);
    m_v2_z = simd128_select_float(m_v2, 2, 2, 2, 2);
    m_v2_w = simd128_select_float(m_v2, 3, 3, 3, 3);
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
        __simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
        __simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
        __simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
        // m_v1_w would contain only 1.0f so m_dot_w is equal to m_v2_w.
        __simd128_float m_result = simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y),
            simd128_add_float(m_dot_z, m_v2_w)
            );
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
// f2 and dot must be 16-bytes aligned.

void simd_dot_product_nx1_point3_storage3_vector4_and_count_negative_float(
int n, const float * __restrict f1, const float * __restrict f2, float * __restrict dot,
int& negative_count);

static inline_only void simd_inline_dot_product_nx1_point3_storage3_vector4_and_count_negative_float(
int n, const float * __restrict f1, const float * __restrict f2, float * __restrict dot,
int& negative_count) {
    __simd128_float m_v2 = simd128_load_float(f2);
    __simd128_float m_v2_x, m_v2_y, m_v2_z, m_v2_w;
    m_v2_x = simd128_select_float(m_v2, 0, 0, 0, 0);
    m_v2_y = simd128_select_float(m_v2, 1, 1, 1, 1);
    m_v2_z = simd128_select_float(m_v2, 2, 2, 2, 2);
    m_v2_w = simd128_select_float(m_v2, 3, 3, 3, 3);
    __simd128_float m_zeros = simd128_set_zero_float();
    int count = 0;
    int i = 0;
    for (; i + 3 < n; i += 4) {
        __simd128_float m_v1_0 = simd128_load3_float(&f1[i * 3 + 0]);
        __simd128_float m_v1_1 = simd128_load3_float(&f1[i * 3 + 3]);
        __simd128_float m_v1_2 = simd128_load3_float(&f1[i * 3 + 6]);
        __simd128_float m_v1_3 = simd128_load3_float(&f1[i * 3 + 9]);
        __simd128_float m_v1_x, m_v1_y, m_v1_z;
        simd128_transpose4to3_float(m_v1_0, m_v1_1, m_v1_2, m_v1_3, m_v1_x, m_v1_y, m_v1_z);
        __simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
        __simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
        __simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
        // m_v1_w would contain only 1.0f so m_dot_w is equal to m_v2_w.
        __simd128_float m_result = simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y),
            simd128_add_float(m_dot_z, m_v2_w)
            );
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

#endif // defined(__DST_SIMD_DOT_H__)

