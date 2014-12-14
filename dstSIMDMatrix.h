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

#ifndef __DST_SIMD_MATRIX_H__
#define __DST_SIMD_MATRIX_H__

// The following SIMD functions for matrix multiplication have been implemented.
//
// SIMD 4x4 float matrix multiplication for matrices in column-major order.
// Requires 16-byte alignment of the matrices.

// void simd_matrix_multiply_4x4CM_float(const float * __restrict m1,
//     const float * __restrict__ m2, float * __restrict m3);
// inline void simd_inline_matrix_multiply_4x4CM_float(const float * __restrict m1,
//     const float * __restrict__ m2, float * __restrict m3);
//
// Multiply 4x3 (4 rows, 3 rows) matrices in row-major order. The fourth
// row is implicitly defined as (0.0f, 0.0f, 0.0f, 1.0f).
//
// void simd_matrix_multiply_4x3RM_float(const float * __restrict m1,
//     const float * __restrict__ m2, float * __restrict m3);
// inline void simd_inline_matrix_multiply_4x3RM_float(const float * __restrict m1,
//     const float * __restrict__ m2, float * __restrict m3);
//
// Multiply 4x4 (column-major) and 4x3 (4 rows, 3 rows, row-major order) matrices.
// The fourth row of the 4x3 matrix is implicitly defined as (0.0f, 0.0f, 0.0f, 1.0f).
//
// void simd_matrix_multiply_4x4CM_4x3RM_float(const float * __restrict m1,
//     const float * __restrict__ m2, float * __restrict m3);
// inline void simd_inline_matrix_multiply_4x4CM_4x3RM_float(const float * __restrict m1,
//     const float * __restrict__ m2, float * __restrict m3);
//
// Classes have been defined for using SIMD to multiply a specific matrix with one or more
// vertices.
//
// The following class is for a 4x4 matrix and can be initialized with matrices
// (float arrays) in both row-major or column-major format. It can subsequently be used to
// multiply the matrix with one or more vertices.
//
// class SIMDMatrix4x4;
//
// The following class is for a 4x3 matrix and can be initialized with matrices
// (float arrays) in both row-major or column-major format. It requires
// simd128_transpose4to3_float and simd_transpose3to4_float.
//
// class SIMDMatrix4x3;
//

// The following more elaborate functions, for matrix multiplication and
// dot products, are provided both as an inline functions and as regular functions
// that are implemented with the inline function.
//
// In some cases, using the inline version may allow the compiler to simplify
// code (e.g. eliminate loads or stores) at the expense of greater overall code size
// of the application; in many other cases, there is not much benefit from using
// using the inline version while code size and CPU instruction cache utilization
// may significantly benefit from using a regular function call.

// SIMD 4x4 float matrix multiplication for matrices in column-major order.
// Requires 16-byte alignment of the matrices.

void simd_matrix_multiply_4x4CM_float(const float * __restrict m1,
const float * __restrict__ m2, float * __restrict m3);

static inline_only void simd_inline_matrix_multiply_4x4CM_float(const float * __restrict m1,
const float * __restrict__ m2, float * __restrict m3) {
    // m.n[column][row]
    // First value (row = 0, column = 0):
    // m1.n[0][0] * m2.n[0][0] + m1.n[1][0] * m2.n[0][1] +
    // m1.n[2][0] * m2.n[0][2] + m1.n[3][0] * m2.n[0][3]
    // Second value (row = 0, column = 1):
    // m1.n[0][0] * m2.n[1][0] + m1.n[1][0] * m2.n[1][1] +
    // m1.n[2][0] * m2.n[1][2] + m1.n[3][0] * m2.n[1][3]
    // Second row:
    // m1.n[0][1] * m2.n[0][0] + m1.n[1][1] * m2.n[0][1] +
    // m1.n[2][1] * m2.n[0][2] + m1.n[3][1] * m2.n[0][3],
    // Third row:
    // m1.n[0][2] * m2.n[0][0] + m1.n[1][2] * m2.n[0][1] +
    // m1.n[2][2] * m2.n[0][2] + m1.n[3][2] * m2.n[0][3],
    __simd128_float col0_m1 = simd128_load_float(&m1[0]);
    __simd128_float col1_m1 = simd128_load_float(&m1[4]);
    __simd128_float col2_m1 = simd128_load_float(&m1[8]);
    __simd128_float col3_m1 = simd128_load_float(&m1[12]);
    // For each result column.
    for (int i = 0; i < 4; i++) {
        __simd128_float col_m2 = simd128_load_float(&m2[i * 4]);
	__simd128_float v0 = simd128_select_float(col_m2, 0, 0, 0, 0);
        __simd128_float v1 = simd128_select_float(col_m2, 1, 1, 1, 1);
        __simd128_float v2 = simd128_select_float(col_m2, 2, 2, 2, 2);
        __simd128_float v3 = simd128_select_float(col_m2, 3, 3, 3, 3);
        // v0 = m1.n[0][i], v1 = m1.n[1][i], v2 = m1.n[2][i], ...
        __simd128_float result_col = simd128_add_float(
            simd128_add_float(
                simd128_mul_float(v0, col0_m1),
                simd128_mul_float(v1, col1_m1)),
            simd128_add_float(
                simd128_mul_float(v2, col2_m1),
                simd128_mul_float(v3, col3_m1))
            );
        simd128_store_float(&m3[i * 4], result_col);
    }
}

// Multiply 4x3 (4 rows, 3 rows) matrices in row-major order. The fourth
// row is implicitly defined as (0.0f, 0.0f, 0.0f, 1.0f).

void simd_matrix_multiply_4x3RM_float(const float * __restrict m1,
const float * __restrict__ m2, float * __restrict m3);

static inline_only void simd_inline_matrix_multiply_4x3RM_float(const float * __restrict m1,
const float * __restrict__ m2, float * __restrict m3) {
    __simd128_float row0 = simd128_load_float(&m2[0]);
    __simd128_float row1 = simd128_load_float(&m2[4]);
    __simd128_float row2 = simd128_load_float(&m2[8]);
    __simd128_float zeros = _mm_setzero_ps();
    // For each row in m1.
    for (int i = 0; i < 3; i++) {
        // Get each column from row i in m1.
        __simd128_float row = simd128_load_float(&m1[i * 4]);
	__simd128_float v0 = simd128_select_float(row, 0, 0, 0, 0);
        __simd128_float v1 = simd128_select_float(row, 1, 1, 1, 1);
        __simd128_float v2 = simd128_select_float(row, 2, 2, 2, 2);
        __simd128_float v3_mult = simd128_select_float(
            simd128_merge1_float(zeros, row), 0, 0, 0, 3);
        __simd128_float result_row = simd128_add_float(
            simd128_add_float(
                simd128_mul_float(v0, row0),
                simd128_mul_float(v1, row1)),
            simd128_add_float(
                simd128_mul_float(v2, row2),
                v3_mult)
            );
        simd128_store_float(&m3[i * 4], result_row);
    }
}

// Multiply 4x4 (column-major) and 4x3 (4 rows, 3 rows, row-major order) matrices.
// The fourth row of the 4x3 matrix is implicitly defined as (0.0f, 0.0f, 0.0f, 1.0f).

void simd_matrix_multiply_4x4CM_4x3RM_float(const float * __restrict m1,
const float * __restrict__ m2, float * __restrict m3);

static inline_only void simd_inline_matrix_multiply_4x4CM_4x3RM_float(const float * __restrict m1,
const float * __restrict__ m2, float * __restrict m3) {
    __simd128_float col0 = simd128_load_float(&m1[0]);
    __simd128_float col1 = simd128_load_float(&m1[4]);
    __simd128_float col2 = simd128_load_float(&m1[8]);
    __simd128_float col3 = simd128_load_float(&m1[12]);
    __simd128_float m2_row0 = simd128_load_float(&m2[0]);
    __simd128_float m2_row1 = simd128_load_float(&m2[4]);
    __simd128_float m2_row2 = simd128_load_float(&m2[8]);
    for (int i = 0; i < 3; i++) {
        // Get column i from each row of m2.
	__simd128_float m2_coli_row0 = simd128_select_float(m2_row0, 0, 0, 0, 0);
 	__simd128_float m2_coli_row1 = simd128_select_float(m2_row1, 0, 0, 0, 0);
	__simd128_float m2_coli_row2 = simd128_select_float(m2_row2, 0, 0, 0, 0);
        m2_row0 = simd128_shift_right_float(m2_row0, 1);
        m2_row1 = simd128_shift_right_float(m2_row1, 1);
        m2_row2 = simd128_shift_right_float(m2_row2, 1);
        __simd128_float result_col = simd128_add_float(
            simd128_add_float(
                simd128_mul_float(m2_coli_row0, col0),
                simd128_mul_float(m2_coli_row1, col1)),
            simd128_mul_float(m2_coli_row2, col2)
            );
        simd128_store_float(&m3[i * 4], result_col);
    }
    // Element 0 of m2_coli_row0/1/2 now contains column 3 of each row.
    __simd128_float m2_col3_row0 = simd128_select_float(m2_row0, 0, 0, 0, 0);
    __simd128_float m2_col3_row1 = simd128_select_float(m2_row1, 0, 0, 0, 0);
    __simd128_float m2_col3_row2 = simd128_select_float(m2_row2, 0, 0, 0, 0);
    __simd128_float result_col3 = simd128_add_float(
        simd128_add_float(
            simd128_mul_float(m2_col3_row0, col0),
            simd128_mul_float(m2_col3_row1, col1)),
        simd128_add_float(
            simd128_mul_float(m2_col3_row2, col2),
            col3)
        );
    simd128_store_float(&m3[3 * 4], result_col3);
}

// Classes for using SIMD to multiply a specific matrix with one or more vertices.

// The following class is for a 4x4 matrix and can be initialized with matrices
// (float arrays) in both row-major or column-major format.

class SIMDMatrix4x4 {
public :
    __simd128_float m_row0;
    __simd128_float m_row1;
    __simd128_float m_row2;
    __simd128_float m_row3;

    // Set row-major matrix data.
    inline_only void SetRM(const float *f) {
        // Load rows.
        m_row0 = simd128_load_float(&f[0]);
        m_row1 = simd128_load_float(&f[4]);
        m_row2 = simd128_load_float(&f[8]);
        m_row3 = simd128_load_float(&f[12]);
    }
    // Set column-major matrix data.
    inline_only void SetCM(const float *f) {
        // Load columns, then transpose to rows.
        SetRM(f);
        simd128_transpose4_float(m_row0, m_row1, m_row2, m_row3);
    }
    // Multiply with a four-float SIMD vector.
    // Note: This uses the inefficient horizontal addition primitive.
    inline_only void MultiplyVector4(const __simd128_float m_v, __simd128_float& m_result) const {
        __simd128_float m_mul0  = simd128_mul_float(m_row0, m_v);
        __simd128_float m_mul1  = simd128_mul_float(m_row1, m_v);
        __simd128_float m_mul2  = simd128_mul_float(m_row2, m_v);
        __simd128_float m_mul3  = simd128_mul_float(m_row3, m_v);
        m_result =
            simd128_horizontal_add2_float(
                simd128_horizontal_add2_float(m_mul0, m_mul1),
                simd128_horizontal_add2_float(m_mul2, m_mul3));
    }
    inline_only void MultiplyVector4(const __simd128_float m_v, float &result_x,
    float &result_y, float &result_z, float& result_w) const {
        __simd128_float m_result;
        MultiplyVector4(m_v, m_result);
        result_x = simd128_get_float(m_result);
        result_y = simd128_get_float(simd128_shift_right_float(m_result, 1));
        result_z = simd128_get_float(simd128_shift_right_float(m_result, 2));
        result_w = simd128_get_float(simd128_shift_right_float(m_result, 3));
    }
    inline_only void MultiplyVector4(const __simd128_float m_v, float *result) const {
        __simd128_float m_result;
        MultiplyVector4(m_v, m_result);
        simd128_store_float(result, m_result);
    }
    inline_only void MultiplyVector4(const float *v, __simd128_float m_result) const {
        __simd128_float m_v = simd128_load_float(&v[0]);
        MultiplyVector4(m_v, m_result);
    }
    inline_only void MultiplyVector4(const float *v, float &result_x,
    float &result_y, float &result_z, float& result_w) const {
        __simd128_float m_v = simd128_load_float(&v[0]);
        MultiplyVector4(m_v, result_x, result_y, result_z, result_w);
    }
    inline_only void MultiplyVector4(const float *v, float *result) const {
        __simd128_float m_v = simd128_load_float(&v[0]);
        MultiplyVector4(m_v, result);
    }
};

#ifdef SIMD_HAVE_MATRIX4X3_VECTOR_MULTIPLICATION

// The following class is for a 4x3 matrix and can be initialized with matrices
// (float arrays) in both row-major or column-major format.
//
// Requires simd128_transpose4to3_float and simd_transpose3to4_float.

class SIMDMatrix4x3 {
public :
    __simd128_float m_row0;
    __simd128_float m_row1;
    __simd128_float m_row2;

    // Set row-major matrix data.
    inline_only void SetRM(const float *f) {
        // Load rows.
        m_row0 = simd128_load_float(&f[0]);
        m_row1 = simd128_load_float(&f[4]);
        m_row2 = simd128_load_float(&f[8]);
    }
    // Set column-major matrix data.
    inline_only void SetCM(const float *f) {
        // Load columns, then transpose to rows.
        __simd128_float m_col0 = simd128_load3_float(&f[0]);
        __simd128_float m_col1 = simd128_load3_float(&f[3]);
        __simd128_float m_col2 = simd128_load3_float(&f[6]);
        __simd128_float m_col3 = simd128_load3_float(&f[9]);
        simd128_transpose4to3_float(m_col0, m_col1, m_col2, m_col3,
            m_row0, m_row1, m_row2);
    }
    // Multiply with a three-float SIMD vector.
    // Note: This uses the inefficient horizontal addition primitive.
    // The fourth component of m_v (0.0f or 1.0f) makes a difference.
    inline_only void MultiplyVector3(const __simd128_float m_v, __simd128_float& m_result) const {
        __simd128_float m_mul0  = simd128_mul_float(m_row0, m_v);
        __simd128_float m_mul1  = simd128_mul_float(m_row1, m_v);
        __simd128_float m_mul2  = simd128_mul_float(m_row2, m_v);
        __simd128_float m_zeros = simd128_set_zero_float();
        m_result =
            simd128_horizontal_add2_float(
                simd128_horizontal_add2_float(m_mul0, m_mul1),
                simd128_horizontal_add2_float(m_mul2, m_zeros));
    }
    // The fourth component of m_v (0.0f or 1.0f) makes a difference.
    inline_only void MultiplyVector3(const __simd128_float m_v, float &result_x,
    float &result_y, float &result_z) const {
        __simd128_float m_result;
        MultiplyVector3(m_v, m_result);
        result_x = simd128_get_float(m_result);
        result_y = simd128_get_float(simd128_shift_right_float(m_result, 1));
        result_z = simd128_get_float(simd128_shift_right_float(m_result, 2));
    }
    // Unpacked three-float result vector (16-byte aligned, with four bytes padding).
    // The fourth component of source vector m_v (0.0f or 1.0f) makes a difference.
    inline_only void MultiplyVector3(const __simd128_float m_v, float *result) const {
        __simd128_float m_result;
        MultiplyVector3(m_v, m_result);
        simd128_store_float(result, m_result);
    }
    // Unpacked three-float source vector (16-byte aligned, with four bytes padding).
    inline_only void MultiplyVector3(const float *v, __simd128_float m_result) const {
        // Make sure the fourth element is zero after loading.
        __simd128_float m_v = simd128_set_last_zero_float(simd128_load_float(&v[0]));
        MultiplyVector3(m_v, m_result);
    }
    // Unpacked three-float source vector (16-byte aligned, with four bytes padding).
    inline_only void MultiplyVector3(const float *v, float &result_x,
    float &result_y, float &result_z) const {
        __simd128_float m_v = simd128_load_float(&v[0]);
        MultiplyVector3(m_v, result_x, result_y, result_z);
    }
    // Unpacked three-float source and result vectors (16-byte aligned, with four bytes padding).
    inline_only void MultiplyVector3(const float *v, float *result) const {
        __simd128_float m_v = simd128_set_last_zero_float(simd128_load_float(&v[0]));
        MultiplyVector3(m_v, result);
    }
    // Packed three-float result vector (unaligned).
    // The fourth component of source vector m_v (0.0f or 1.0f) makes a difference.
    inline_only void MultiplyVector3Packed(const __simd128_float m_v, float *result) const {
        __simd128_float m_result;
        MultiplyVector3(m_v, m_result);
        simd128_store3_float(result, m_result);
    }
    // Packed three-float source vector (unaligned).
    inline_only void MultiplyVector3Packed(const float *v, __simd128_float m_result) const {
        __simd128_float m_v = simd128_load3_float(&v[0]);
        MultiplyVector3(m_v, m_result);
    }
    // Packed three-float source vector (unaligned).
    inline_only void MultiplyVector3Packed(const float *v, float &result_x,
    float &result_y, float &result_z) const {
        __simd128_float m_v = simd128_load3_float(&v[0]);
        MultiplyVector3(m_v, result_x, result_y, result_z);
    }
    // Packed three-float source and result vectors (unaligned).
    inline_only void MultiplyVector3Packed(const float *v, float *result) const {
        __simd128_float m_v = simd128_load3_float(&v[0]);
        MultiplyVector3Packed(m_v, result);
    }
    // Four-float vectors including (fourth) w component.
    inline_only void MultiplyVector4(const __simd128_float m_v, __simd128_float& m_result) const {
        __simd128_float m_mul0  = simd128_mul_float(m_row0, m_v);
        __simd128_float m_mul1  = simd128_mul_float(m_row1, m_v);
        __simd128_float m_mul2  = simd128_mul_float(m_row2, m_v);
        // Set m_mul3 to (0.0f, 0.0f, 0.0f, m_v.w)
        __simd128_float m_mul3  = simd128_select_float(
            simd128_merge_float(simd128_set_zero_float(), m_v, 0, 0, 3, 3),
            0, 0, 0, 3);
        m_result =
            simd128_horizontal_add2_float(
                simd128_horizontal_add2_float(m_mul0, m_mul1),
                simd128_horizontal_add2_float(m_mul2, m_mul3));
    }
    // Result is stored in three seperate floats.
    inline_only void MultiplyVector4(const __simd128_float m_v, float &result_x,
    float &result_y, float &result_z) const {
        __simd128_float m_result;
        MultiplyVector4(m_v, m_result);
        result_x = simd128_get_float(m_result);
        result_y = simd128_get_float(simd128_shift_right_float(m_result, 1));
        result_z = simd128_get_float(simd128_shift_right_float(m_result, 2));
    }
    // Result is stored in four seperate floats.
    inline_only void MultiplyVector4(const __simd128_float m_v, float &result_x,
    float &result_y, float &result_z, float& result_w) const {
        __simd128_float m_result;
        MultiplyVector4(m_v, m_result);
        result_x = simd128_get_float(m_result);
        result_y = simd128_get_float(simd128_shift_right_float(m_result, 1));
        result_z = simd128_get_float(simd128_shift_right_float(m_result, 2));
        result_w = simd128_get_float(simd128_shift_right_float(m_result, 3));
    }
    // Four-float result vector.
    inline_only void MultiplyVector4(const __simd128_float m_v, float *result) const {
        __simd128_float m_result;
        MultiplyVector4(m_v, m_result);
        simd128_store_float(result, m_result);
    }
    inline_only void MultiplyVector4(const float *v, __simd128_float m_result) const {
        __simd128_float m_v = simd128_load_float(&v[0]);
        MultiplyVector4(m_v, m_result);
    }
    inline_only void MultiplyVector4(const float *v, float &result_x,
    float &result_y, float &result_z, float& result_w) const {
        __simd128_float m_v = simd128_load_float(&v[0]);
        MultiplyVector4(m_v, result_x, result_y, result_z, result_w);
    }
    // Four-float result vector.
    inline_only void MultiplyVector4(const float *v, float *result) const {
        __simd128_float m_v = simd128_load_float(&v[0]);
        MultiplyVector4(m_v, result);
    }
    // Unpacked three-float source point vector (16-byte aligned, with four bytes padding).
    // Point vector has implicit w component of 1.0f.
    inline_only void MultiplyPoint3(const float *v, __simd128_float m_result) const {
        // Make sure the fourth element is 1.0f after loading.
        __simd128_float m_v = simd128_set_last_float(simd128_load_float(&v[0]), 1.0f);
        MultiplyVector3(m_v, m_result);
    }
    // Unpacked three-float source vector (16-byte aligned, with four bytes padding).
    inline_only void MultiplyPoint3(const float *v, float &result_x,
    float &result_y, float &result_z) const {
        __simd128_float m_v = simd128_set_last_float(simd128_load_float(&v[0]), 1.0f);
        MultiplyVector3(m_v, result_x, result_y, result_z);
    }
    // Unpacked three-float source and result vectors (16-byte aligned, with four bytes padding).
    inline_only void MultiplyPoint3(const float *v, float *result) const {
        __simd128_float m_v = simd128_set_last_float(simd128_load_float(&v[0]), 1.0f);
        MultiplyVector3(m_v, result);
    }
};

#endif // defined(SIMD_HAVE_MATRIX4X3_VECTOR_MULTIPLICATION)

#endif // defined(__DST_SIMD_MATRIX_H__)
