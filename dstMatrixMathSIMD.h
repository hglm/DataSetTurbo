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

#ifndef __DST_MATRIX_MATH_SIMD_H__
#define __DST_MATRIX_MATH_SIMD_H__

#include "dstSIMD.h"
#include "dstMatrixMath.h"
#include "dstVectorMathSIMD.h"
#include "dstSIMDMatrix.h"

#ifndef DST_NO_SIMD

// SSE matrix multiplication for matrices in column-major order.
// Requires 16-byte alignment of the matrices.

static DST_INLINE_ONLY void SIMDMatrixMultiply(const Matrix4D& __restrict m1,
const Matrix4D& __restrict__ m2, Matrix4D& __restrict m3) {
    simd_matrix_multiply_4x4CM_float(&m1.n[0][0], &m2.n[0][0], &m3.n[0][0]);
}

// Multiply 4x3 (4 rows, 3 rows) matrices in row-major order (different from
// other matrices which are in column-major order).
// The fourth row is implicitly defined as (0.0f, 0.0f, 0.0f, 1.0f).

static DST_INLINE_ONLY void SIMDMatrixMultiply(const MatrixTransform& __restrict m1,
const MatrixTransform& __restrict__ m2, MatrixTransform& __restrict m3) {
    simd_matrix_multiply_4x3RM_float(&m1.n[0][0], &m2.n[0][0], &m3.n[0][0]);
}

// Multiply 4x4 and 4x3 (4 rows, 3 columns) matrices. The 4x3 matrix is in row-major
// order (different from the 4x4 matrix which is in column-major order).
// The fourth row of the 4x3 matrix is implicitly defined as (0.0f, 0.0f, 0.0f, 1.0f).

static DST_INLINE_ONLY void SIMDMatrixMultiply(const Matrix4D& __restrict m1,
const MatrixTransform& __restrict__ m2, Matrix4D& __restrict m3) {
    simd_matrix_multiply_4x4CM_4x3RM_float(&m1.n[0][0], &m2.n[0][0], &m3.n[0][0]);
}

// Data type for using SIMD to multiply a specific matrix with one or more vertices.

class Matrix4DSIMD : public SIMDMatrix4x4 {
public :
    void Set(const Matrix4D& m) {
        // Load columns, then transpose to rows.
        m_row0 = simd128_load_float(&m.n[0][0]);
        m_row1 = simd128_load_float(&m.n[1][0]);
        m_row2 = simd128_load_float(&m.n[2][0]);
        m_row3 = simd128_load_float(&m.n[3][0]);
        simd128_transpose4_float(m_row0, m_row1, m_row2, m_row3);
    }
    DST_INLINE_ONLY Vector4D Multiply(const __simd128_float m_v) const {
        __simd128_float m_result;
        MultiplyVector4(m_v, m_result);
        return Vector4D(
            simd128_get_float(m_result),
            simd128_get_float(simd128_shift_right_float(m_result, 1)),
            simd128_get_float(simd128_shift_right_float(m_result, 2)),
            simd128_get_float(simd128_shift_right_float(m_result, 3))
            );
    }
    friend DST_INLINE_ONLY Vector4D operator *(const Matrix4DSIMD& m, const Vector4D& V) {
        return m.Multiply(simd128_set_float(V.x, V.y, V.z, V.w));
    }
    friend DST_INLINE_ONLY Vector4D operator *(const Matrix4DSIMD& m, const Vector4D *Vp) {
        return m.Multiply(simd128_load_float(&Vp[0][0]));
    }
};

// Multiple a 4x4 Matrix by a vector, and store the result in a vector.
// Matrix4DSIMD m must be initialized.

static DST_INLINE_ONLY void SIMDMatrixMultiplyVector(const Matrix4DSIMD& m, const Vector4D *v1p,
Vector4D *v2p) {
    __simd128_float m_v1 = simd128_load(v1p);
    __simd128_float m_result;
    m.MultiplyVector4(m_v1, m_result);
    simd128_store(v2p, m_result);
}

static inline void SIMDMatrixMultiplyFourVectorsNoStore(const Matrix4DSIMD& m,
const Vector4D *v_source_p,
__simd128_float& m_result_0, __simd128_float& m_result_1, __simd128_float& m_result_2,
__simd128_float& m_result_3) {
    // Load four vectors and transpose so that all similar coordinates are
    // stored in a single vector.
    __simd128_float m_v_x = simd128_load(&v_source_p[0]);
    __simd128_float m_v_y = simd128_load(&v_source_p[1]);
    __simd128_float m_v_z = simd128_load(&v_source_p[2]);
    __simd128_float m_v_w = simd128_load(&v_source_p[3]);
    simd128_transpose4_float(m_v_x, m_v_y, m_v_z, m_v_w);
    __simd128_float m_mul0, m_mul1, m_mul2, m_mul3;
    // Process the x coordinates.
    m_mul0 = simd128_mul_float(simd128_replicate_float(m.m_row0, 0), m_v_x);
    m_mul1 = simd128_mul_float(simd128_replicate_float(m.m_row0, 1), m_v_y);
    m_mul2 = simd128_mul_float(simd128_replicate_float(m.m_row0, 2), m_v_z);
    m_mul3 = simd128_mul_float(simd128_replicate_float(m.m_row0, 3), m_v_w);
    __simd128_float m_result_x =
        simd128_add_float(
            simd128_add_float(m_mul0, m_mul1),
            simd128_add_float(m_mul2, m_mul3));
    // Process the y coordinates.
    m_mul0 = simd128_mul_float(simd128_replicate_float(m.m_row1, 0), m_v_x);
    m_mul1 = simd128_mul_float(simd128_replicate_float(m.m_row1, 1), m_v_y);
    m_mul2 = simd128_mul_float(simd128_replicate_float(m.m_row1, 2), m_v_z);
    m_mul3 = simd128_mul_float(simd128_replicate_float(m.m_row1, 3), m_v_w);
    __simd128_float m_result_y =
        simd128_add_float(
            simd128_add_float(m_mul0, m_mul1),
            simd128_add_float(m_mul2, m_mul3));
    // Process the z coordinates.
    m_mul0 = simd128_mul_float(simd128_replicate_float(m.m_row2, 0), m_v_x);
    m_mul1 = simd128_mul_float(simd128_replicate_float(m.m_row2, 1), m_v_y);
    m_mul2 = simd128_mul_float(simd128_replicate_float(m.m_row2, 2), m_v_z);
    m_mul3 = simd128_mul_float(simd128_replicate_float(m.m_row2, 3), m_v_w);
    __simd128_float m_result_z =
        simd128_add_float(
            simd128_add_float(m_mul0, m_mul1),
            simd128_add_float(m_mul2, m_mul3));
    // Process the w coordinates.
    m_mul0 = simd128_mul_float(simd128_replicate_float(m.m_row3, 0), m_v_x);
    m_mul1 = simd128_mul_float(simd128_replicate_float(m.m_row3, 1), m_v_y);
    m_mul2 = simd128_mul_float(simd128_replicate_float(m.m_row3, 2), m_v_z);
    m_mul3 = simd128_mul_float(simd128_replicate_float(m.m_row3, 3), m_v_w);
    __simd128_float m_result_w =
        simd128_add_float(
            simd128_add_float(m_mul0, m_mul1),
            simd128_add_float(m_mul2, m_mul3));
    // Transpose results so that each vector holds multiplication product.
    simd128_transpose4to4_float(m_result_x, m_result_y, m_result_z, m_result_w,
        m_result_0, m_result_1, m_result_2, m_result_3); 
}

static inline void SIMDMatrixMultiplyFourVectors(const Matrix4DSIMD& m,
const Vector4D *v_source_p, Vector4D *v_result_p) {
    __simd128_float m_result_0, m_result_1, m_result_2, m_result_3;
    SIMDMatrixMultiplyFourVectorsNoStore(m, v_source_p,
        m_result_0, m_result_1, m_result_2, m_result_3);
    // Store the results as Vector4D.
    simd128_store(&v_result_p[0], m_result_0);
    simd128_store(&v_result_p[1], m_result_1);
    simd128_store(&v_result_p[2], m_result_2);
    simd128_store(&v_result_p[3], m_result_3);
}

static DST_INLINE_ONLY void SIMDMatrixMultiplyVector(const Matrix4DSIMD& m, const Point3D *p1p,
Point3D *p2p) {
    __simd128_float m_p1 = simd128_set_float(p1p->x, p1p->y, p1p->z, 1.0f);
    __simd128_float m_result;
    m.MultiplyVector4(m_p1, m_result);
    simd128_store(p2p, m_result);
}

static inline void SIMDMatrixMultiplyFourVectorsNoStore(const Matrix4DSIMD& m,
const Point3D *p_source_p,
__simd128_float& m_result_0, __simd128_float& m_result_1, __simd128_float& m_result_2,
__simd128_float& m_result_3) {
    // Load four vectors and transpose so that all similar coordinates are
    // stored in a single vector.
    __simd128_float m_v_x = simd128_load(&p_source_p[0]);
    __simd128_float m_v_y = simd128_load(&p_source_p[1]);
    __simd128_float m_v_z = simd128_load(&p_source_p[2]);
    __simd128_float m_v_w = simd128_load(&p_source_p[3]);
    simd128_transpose4_float(m_v_x, m_v_y, m_v_z, m_v_w);
    // Make sure the last component of each vector is set to 1.0f (i.e. m_v_w must
    // be all 1.0f), because the source vectors are points.
    m_v_w = simd128_set_same_float(1.0f);
    __simd128_float m_mul0, m_mul1, m_mul2, m_mul3;
    // Process the x coordinates.
    m_mul0 = simd128_mul_float(simd128_replicate_float(m.m_row0, 0), m_v_x);
    m_mul1 = simd128_mul_float(simd128_replicate_float(m.m_row0, 1), m_v_y);
    m_mul2 = simd128_mul_float(simd128_replicate_float(m.m_row0, 2), m_v_z);
    m_mul3 = simd128_mul_float(simd128_replicate_float(m.m_row0, 3), m_v_w);
    __simd128_float m_result_x =
        simd128_add_float(
            simd128_add_float(m_mul0, m_mul1),
            simd128_add_float(m_mul2, m_mul3));
    // Process the y coordinates.
    m_mul0 = simd128_mul_float(simd128_replicate_float(m.m_row1, 0), m_v_x);
    m_mul1 = simd128_mul_float(simd128_replicate_float(m.m_row1, 1), m_v_y);
    m_mul2 = simd128_mul_float(simd128_replicate_float(m.m_row1, 2), m_v_z);
    m_mul3 = simd128_mul_float(simd128_replicate_float(m.m_row1, 3), m_v_w);
    __simd128_float m_result_y =
        simd128_add_float(
            simd128_add_float(m_mul0, m_mul1),
            simd128_add_float(m_mul2, m_mul3));
    // Process the z coordinates.
    m_mul0 = simd128_mul_float(simd128_replicate_float(m.m_row2, 0), m_v_x);
    m_mul1 = simd128_mul_float(simd128_replicate_float(m.m_row2, 1), m_v_y);
    m_mul2 = simd128_mul_float(simd128_replicate_float(m.m_row2, 2), m_v_z);
    m_mul3 = simd128_mul_float(simd128_replicate_float(m.m_row2, 3), m_v_w);
    __simd128_float m_result_z =
        simd128_add_float(
            simd128_add_float(m_mul0, m_mul1),
            simd128_add_float(m_mul2, m_mul3));
    // Process the w coordinates.
    m_mul0 = simd128_mul_float(simd128_replicate_float(m.m_row3, 0), m_v_x);
    m_mul1 = simd128_mul_float(simd128_replicate_float(m.m_row3, 1), m_v_y);
    m_mul2 = simd128_mul_float(simd128_replicate_float(m.m_row3, 2), m_v_z);
    m_mul3 = simd128_mul_float(simd128_replicate_float(m.m_row3, 3), m_v_w);
    __simd128_float m_result_w =
        simd128_add_float(
            simd128_add_float(m_mul0, m_mul1),
            simd128_add_float(m_mul2, m_mul3));
    // Transpose results so that each result vector holds multiplication product.
    simd128_transpose4to4_float(m_result_x, m_result_y, m_result_z, m_result_w,
        m_result_0, m_result_1, m_result_2, m_result_3); 
}

static inline void SIMDMatrixMultiplyFourVectors(const Matrix4DSIMD& m,
const Point3D *p_source_p, Point3D *p_result_p) {
    __simd128_float m_result_0, m_result_1, m_result_2, m_result_3;
    SIMDMatrixMultiplyFourVectorsNoStore(m, p_source_p,
        m_result_0, m_result_1, m_result_2, m_result_3);
    // Store the results as Vector4D.
    simd128_store(&p_result_p[0], m_result_0);
    simd128_store(&p_result_p[1], m_result_1);
    simd128_store(&p_result_p[2], m_result_2);
    simd128_store(&p_result_p[3], m_result_3);
}

#ifdef SIMD_HAVE_MATRIX4X3_VECTOR_MULTIPLICATION

// Requires simd128_transpose4to3_float and simd_transpose3to4_float.

class MatrixTransformSIMD : public SIMDMatrix4x3 {
public :
    void Set(const MatrixTransform& m) {
        // MatrixTransform is already in row-major format.
        m_row0 = simd128_load_float(&m.n[0][0]);
        m_row1 = simd128_load_float(&m.n[1][0]);
        m_row2 = simd128_load_float(&m.n[2][0]);
    }
    DST_INLINE_ONLY Vector3D Multiply(const __simd128_float m_v) const {
        __simd128_float m_result;
        MultiplyVector3(m_v, m_result);
        return Vector3D(
            simd128_get_float(m_result),
            simd128_get_float(simd128_shift_right_float(m_result, 1)),
            simd128_get_float(simd128_shift_right_float(m_result, 2))
            );
    }
    friend DST_INLINE_ONLY Vector3D operator *(const MatrixTransformSIMD& m, const Vector3D& V) {
        return m.Multiply(simd128_set_float(V.x, V.y, V.z, 0.0f));
    }
    friend DST_INLINE_ONLY Vector3D operator *(const MatrixTransformSIMD& m, const Vector3D *Vp) {
        return m.Multiply(simd128_load_float(&Vp[0][0]));
    }
};

// Multiply a 4x3 matrix by a vector, and store the result in a vector.
// MatrixTransformSIMD m must be initialized.

static DST_INLINE_ONLY void SIMDMatrixMultiplyVector(const MatrixTransformSIMD& m,
const Vector3D *v1p, Vector3D *v2p) {
    // Using simd128_load(const Vector3D *v).
    __simd128_float m_v1 = simd128_load(&v1p[0]);
    __simd128_float m_result;
    m.MultiplyVector3(m_v1, m_result);
    // Using simd128_store(Vector3D *v).
    simd128_store(&v2p[0], m_result);
}

// Multiply a 4x3 matrix by an array of four vectors, and store the result in
// an array of four vectors.

static inline void SIMDMatrixMultiplyFourVectors(const MatrixTransformSIMD& m,
const Vector3D *v_source_p, Vector3D *v_result_p) {
    // Load four vectors and transpose so that all similar coordinates are
    // stored in a single vector.
    __simd128_float m_v0 = simd128_load(&v_source_p[0]);
    __simd128_float m_v1 = simd128_load(&v_source_p[1]);
    __simd128_float m_v2 = simd128_load(&v_source_p[2]);
    __simd128_float m_v3 = simd128_load(&v_source_p[3]);
    // Transpose four three-float vectors, and store result in m_v_x, m_v_y and m_v_z.
    __simd128_float m_v_x, m_v_y, m_v_z;
    simd128_transpose4to3_float(m_v0, m_v1, m_v2, m_v3, m_v_x, m_v_y, m_v_z);
    __simd128_float m_mul0, m_mul1, m_mul2;
    // Process the x coordinates.
    m_mul0 = simd128_mul_float(simd128_replicate_float(m.m_row0, 0), m_v_x);
    m_mul1 = simd128_mul_float(simd128_replicate_float(m.m_row0, 1), m_v_y);
    m_mul2 = simd128_mul_float(simd128_replicate_float(m.m_row0, 2), m_v_z);
    __simd128_float m_result_x =
        simd128_add_float(
            simd128_add_float(m_mul0, m_mul1),
            m_mul2);
    // Process the y coordinates.
    m_mul0 = simd128_mul_float(simd128_replicate_float(m.m_row1, 0), m_v_x);
    m_mul1 = simd128_mul_float(simd128_replicate_float(m.m_row1, 1), m_v_y);
    m_mul2 = simd128_mul_float(simd128_replicate_float(m.m_row1, 2), m_v_z);
    __simd128_float m_result_y =
        simd128_add_float(
            simd128_add_float(m_mul0, m_mul1),
            m_mul2);
    // Process the z coordinates.
    m_mul0 = simd128_mul_float(simd128_replicate_float(m.m_row2, 0), m_v_x);
    m_mul1 = simd128_mul_float(simd128_replicate_float(m.m_row2, 1), m_v_y);
    m_mul2 = simd128_mul_float(simd128_replicate_float(m.m_row2, 2), m_v_z);
    __simd128_float m_result_z =
        simd128_add_float(
            simd128_add_float(m_mul0, m_mul1),
            m_mul2);
    // Transpose results so that each vector holds multiplication product.
    __simd128_float m_result_v0, m_result_v1, m_result_v2, m_result_v3;
    simd128_transpose3to4_float(m_result_x, m_result_y, m_result_z,
        m_result_v0, m_result_v1, m_result_v2, m_result_v3); 
    simd128_store(&v_result_p[0], m_result_v0);
    simd128_store(&v_result_p[1], m_result_v1);
    simd128_store(&v_result_p[2], m_result_v2);
    simd128_store(&v_result_p[3], m_result_v3);
}

#endif // defined(SIMD_HAVE_MATRIX4X3_VECTOR_MULTIPLICATION)

#endif // !defined(DST_NO_SIMD)

#endif // defined(__DST_MATRIX_MATH_SIMD_H__)

