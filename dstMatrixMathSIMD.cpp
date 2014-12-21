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

#include "dstMatrixMathSIMD.h"

// Non-inline member funtions for Matrix3D, Matrix4D and MatrixTransform classes
// that use SIMD. SIMD functions are taken advantage of for the following multiplications:
// - Matrix4 * Matrix4
// - MatixTransform * MatrixTranform
// - Matrix4D * MatrixTransform
//
// When DST_USE_SIMD is not defined, these member function will be defined as regular C++
// functions in sreMatrixMath.cpp.
//
// Additional functions include multiplying a matrix by an array of vectors:
// - Matrix4D * Vector4D[]
// - Matrix4D * Point3D[]
// - MatrixTransform * Vector3D[]
//
// C++ code is provided for the latter functions when DST_USE_SIMD is not defined.

#ifndef DST_NO_SIMD

Matrix4D operator *(const Matrix4D& __restrict__ m1, const Matrix4D& __restrict__ m2) {
    Matrix4D m3;
    SIMDMatrixMultiply(m1, m2, m3);
    return m3;
}

MatrixTransform operator *(const MatrixTransform& __restrict__ m1,
const MatrixTransform& __restrict__ m2) {
    MatrixTransform m3;
    SIMDMatrixMultiply(m1, m2, m3);
#if 0
    char *s1 = m3.GetString();
    Matrix4D m4 = Matrix4D(m1) * Matrix4D(m2);
    char *s2 = m4.GetString();
    sreMessage(SRE_MESSAGE_INFO, "%s\n%s", s1, s2);
    delete [] s1;
    delete [] s2;
#endif
    return m3;
}

Matrix4D operator *(const Matrix4D& __restrict__ m1, const MatrixTransform& __restrict__ m2)
{
    Matrix4D m3;
    SIMDMatrixMultiply(m1, m2, m3);
#if 0
    char *s1 = m3.GetString();
    Matrix4D m4 = m1 * Matrix4D(m2);
    char *s2 = m4.GetString();
    sreMessage(SRE_MESSAGE_INFO, "SIMD %s\nNon-SIMD %s", s1, s2);
    delete [] s1;
    delete [] s2;
#endif
    return m3;
}

#endif

void MatrixMultiplyVectors(int n, const Matrix4D& m, const Vector4D *v1, Vector4D *v2) {
    int i = 0;
#ifndef DST_NO_SIMD
    Matrix4DSIMD m_simd;
    m_simd.Set(m);
    for (; i + 3 < n; i += 4) {
        // Processing one vertex at a time using SIMD is not very efficient.
        // Process four at at time.
        SIMDMatrixMultiplyFourVectors(m_simd, &v1[i], &v2[i]);
    }
    // Handle remaining vertices.
    for (; i < n; i++)
        SIMDMatrixMultiplyVector(m_simd, &v1[i], &v2[i]);
#endif
    for (; i < n; i++)
        v2[i] = m * v1[i];
}

void MatrixMultiplyVectors(int n, const Matrix4D& m, const Point3D *p1, Point3D *p2) {
    int i = 0;
#ifndef DST_NO_SIMD
    Matrix4DSIMD m_simd;
    m_simd.Set(m);
    for (; i + 3 < n; i += 4) {
        // Processing one vertex at a time using SIMD is not very efficient.
        // Process four at at time.
        SIMDMatrixMultiplyFourVectors(m_simd, &p1[i], &p2[i]);
    }
    // Handle remaining vertices.
    for (; i < n; i++)
        SIMDMatrixMultiplyVector(m_simd, &p1[i], &p2[i]);
#else
    for (; i < n; i++)
        p2[i] = (m * p1[i]).GetPoint3D();
#endif
}

void MatrixMultiplyVectors(int n, const MatrixTransform& m, const Vector3D *v1, Vector3D *v2) {
    int i = 0;
#ifdef SIMD_HAVE_MATRIX4X3_VECTOR_MULTIPLICATION
    MatrixTransformSIMD m_simd;
    m_simd.Set(m);
    for (; i + 3 < n; i += 4) {
        // Processing one vertex at a time using SIMD is not very efficient.
        // Process four at at time.
        SIMDMatrixMultiplyFourVectors(m_simd, &v1[i], &v2[i]);
    }
    // Handle remaining vertices.
    for (; i < n; i++)
        SIMDMatrixMultiplyVector(m_simd, &v1[i], &v2[i]);
#else
    for (; i < n; i++)
        v2[i] = (m * v1[i]).GetVector3D();
#endif
}

