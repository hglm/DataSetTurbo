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

#ifndef __DST_MATRIX_MATH_H__
#define __DST_MATRIX_MATH_H__

#include "dstVectorMath.h"

// This code is largely based on the source code provided by Eric Lengyel
// in assocation with a book and on the net, as described below.
//
// Several changes and additions have been made to the code.

//============================================================================
//
// 3D/4D Matrix Classes
//
// Mathematics for 3D Game Programming and Computer Graphics, 3rd ed.
// By Eric Lengyel
//
// The code in this file may be freely used in any software. It is provided
// as-is, with no warranty of any kind.
//
//============================================================================

class DST_API Matrix3D
{
	public:

		float	n[3][3];

	public:

		Matrix3D() {}

		DST_API Matrix3D(const Vector3D& c1, const Vector3D& c2, const Vector3D& c3) {
			Set(c1, c2, c3);
		}

		DST_API Matrix3D(float n00, float n01, float n02, float n10, float n11, float n12,
		float n20, float n21, float n22) {
			Set(n00, n01, n02, n10, n11, n12, n20, n21, n22);
		}

		DST_API Matrix3D& Set(const Vector3D& c1, const Vector3D& c2, const Vector3D& c3) {
			return Set(c1.x, c2.x, c3.x, c1.y, c2.y, c3.y, c1.z, c2.z, c3.z);
		}

		// Initialize starting with the first row, then the next row, then subsequent columns.
		DST_API Matrix3D& Set(float n00, float n01, float n02, float n10,
		float n11, float n12, float n20, float n21, float n22) {
			n[0][0] = n00;
			n[0][1] = n10;
			n[0][2] = n20;
			n[1][0] = n01;
			n[1][1] = n11;
			n[1][2] = n21;
			n[2][0] = n02;
			n[2][1] = n12;
			n[2][2] = n22;
			return (*this);
		}

		float& operator ()(int i, int j)
		{
			return (n[j][i]);
		}

		const float& operator ()(int i, int j) const
		{
			return (n[j][i]);
		}

		float& Get(int column, int row) {
			return (n[column][row]);
		}

		const float& Get(int column, int row) const {
			return (n[column][row]);
		}

		Vector3D& operator [](int j)
		{
			return (*reinterpret_cast<Vector3D *>(n[j]));
		}

		const Vector3D& operator [](int j) const
		{
			return (*reinterpret_cast<const Vector3D *>(n[j]));
		}

		Vector3D GetRow(int i) const
		{
			return (Vector3D(n[0][i], n[1][i], n[2][i]));
		}

		Matrix3D& SetRow(int i, const Vector3D& row)
		{
			n[0][i] = row.x;
			n[1][i] = row.y;
			n[2][i] = row.z;
			return (*this);
		}

		DST_API Matrix3D& operator *=(const Matrix3D& m);
		DST_API Matrix3D& operator *=(float t);
		DST_API Matrix3D& operator /=(float t);

		DST_API Matrix3D& SetIdentity(void);
                DST_API Matrix3D& AssignRotationAlongAxis(const Vector3D& axis, float angle);
                DST_API Matrix3D& AssignRotationAlongXAxis(float angle);
                DST_API Matrix3D& AssignRotationAlongYAxis(float angle);
                DST_API Matrix3D& AssignRotationAlongZAxis(float angle);
                DST_API Matrix3D& AssignTranslation(const Vector2D& translation);
                DST_API Matrix3D& AssignScaling(float scaling);
		DST_API bool RotationMatrixPreservesAABB();
		// Return text respresentation. To be freed with delete [].
		DST_API char *GetString() const;

		friend DST_API Matrix3D operator *(const Matrix3D& m1, const Matrix3D& m2);
		friend DST_API Matrix3D operator *(const Matrix3D& m, float t);
		friend DST_API Matrix3D operator /(const Matrix3D& m, float t);
		friend DST_API Vector3D operator *(const Matrix3D& m, const Vector3D& v);
		friend DST_API Vector3D operator *(const Matrix3D& m, const Point3D& p);
		friend DST_API Vector3D operator *(const Vector3D& v, const Matrix3D& m);
		friend DST_API Vector3D operator *(const Point3D& p, const Matrix3D& m);
		friend DST_API bool operator ==(const Matrix3D& m1, const Matrix3D& m2);
		friend DST_API bool operator !=(const Matrix3D& m1, const Matrix3D& m2);
};


inline Matrix3D operator *(float t, const Matrix3D& m)
{
	return (m * t);
}

DST_API float Determinant(const Matrix3D& m);
DST_API Matrix3D Inverse(const Matrix3D& m);
DST_API Matrix3D Adjugate(const Matrix3D& m);
DST_API Matrix3D Transpose(const Matrix3D& m);

class Matrix4D;

// Custom Matrix4x3RM class. These matrices are zero at n30, n31 and n32 and 1.0 at n33.

class DST_API Matrix4x3RM
{
	public:
		// The interal storage format has been switched from column-major to
		// row-major for Matrix4x3RM only.
		float n[3][4];

	public:
		Matrix4x3RM() {}

		Matrix4x3RM(float n00, float n01, float n02, float n03, float n10,
		float n11, float n12, float n13, float n20, float n21, float n22, float n23) {
			Set(n00, n01, n02, n03, n10, n11, n12, n13, n20, n21, n22, n23);
		}

		Matrix4x3RM(const Vector3D& c1, const Vector3D& c2, const Vector3D& c3,
		const Vector3D& c4) {
			Set(c1, c2, c3, c4);
		}

		Matrix4x3RM& Set(float n00, float n01, float n02, float n03,
		float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23) {
			n[0][0] = n00;
			n[0][1] = n01;
			n[0][2] = n02;
			n[0][3] = n03;
			n[1][0] = n10;
			n[1][1] = n11;
			n[1][2] = n12;
			n[1][3] = n13;
			n[2][0] = n20;
			n[2][1] = n21;
			n[2][2] = n22;
			n[2][3] = n23;
			return (*this);
		}

		Matrix4x3RM& Set(const Vector3D& c1, const Vector3D& c2, const Vector3D& c3,
		const Vector3D& c4) {
			return Set(c1.x, c2.x, c3.x, c4.x, c1.y, c2.y, c3.y, c4.y, c1.z, c2.z, c3.z, c4.z);
		}

		// Return element at row i, column j.
		const float& operator ()(int i, int j) const
		{
			return (n[i][j]);
		}

		const float& Get(int column, int row) const {
			return (n[row][column]);
		}

 		// Get column i.
		Vector3D GetColumn(int i) const
		{
			return Vector3D(n[0][i], n[1][i], n[2][i]);
		}

		const Vector4D& GetRow(int j) const
		{
			return (*reinterpret_cast<const Vector4D *>(n[j]));
		}

		Matrix4x3RM& SetRow(int j, const Vector3D& row)
		{
			n[j][0] = row.x;
			n[j][1] = row.y;
			n[j][2] = row.z;
			n[j][3] = 0.0f;
			return (*this);
		}

		Matrix4x3RM& SetRow(int j, const Vector4D& row)
		{
			n[j][0] = row.x;
			n[j][1] = row.y;
			n[j][2] = row.z;
			n[j][3] = row.w;
			return (*this);
		}

		DST_API Matrix4x3RM& operator *=(const Matrix4x3RM& m);

		DST_API Matrix4x3RM& SetIdentity(void);
                DST_API Matrix4x3RM& AssignRotationAlongXAxis(float angle);
                DST_API Matrix4x3RM& AssignRotationAlongYAxis(float angle);
                DST_API Matrix4x3RM& AssignRotationAlongZAxis(float angle);
                DST_API Matrix4x3RM& AssignTranslation(const Vector3D& translation);
                DST_API Matrix4x3RM& AssignScaling(float scaling);
		// Return text respresentation. To be freed with delete [].
		char *GetString() const;

                // dstMultiply is a non-SIMD multiplication function.
		friend DST_API Matrix4x3RM dstMultiply(const Matrix4x3RM& m1, const Matrix4x3RM& m2);
		friend DST_API Matrix4D dstMultiply(const Matrix4D& m1, const Matrix4x3RM& m2);
		friend DST_INLINE_ONLY Matrix4x3RM dstInlineMultiply(const Matrix4x3RM& m1,
			const Matrix4x3RM& m2);
		friend DST_INLINE_ONLY Matrix4D dstInlineMultiply(const Matrix4D& m1,
			const Matrix4x3RM& m2);
                // The * operator is potentially SIMD-accelerated.
		friend DST_API Matrix4x3RM operator *(const Matrix4x3RM& m1, const Matrix4x3RM& m2);
		friend DST_API Matrix4D operator *(const Matrix4D& m1, const Matrix4x3RM& m2);
		friend DST_API Vector4D operator *(const Matrix4x3RM& m, const Vector4D& v);
		friend DST_API Vector4D operator *(const Vector4D& v, const Matrix4x3RM& m);
		friend DST_API Vector4D operator *(const Matrix4x3RM& m, const Vector3D& v);
		friend DST_API Vector4D operator *(const Vector3D& v, const Matrix4x3RM& m);
		friend DST_API Vector4D operator *(const Matrix4x3RM& m, const Point3D& p);
		friend DST_API Vector4D operator *(const Point3D& p, const Matrix4x3RM& m);
		friend DST_API bool operator ==(const Matrix4x3RM& m1, const Matrix4x3RM& m2);
		friend DST_API bool operator !=(const Matrix4x3RM& m1, const Matrix4x3RM& m2);
} DST_ALIGNED(16);

class DST_API Matrix4D
{
	public:
		// The internal storage is in column-major order.
		float n[4][4];

	public:

		Matrix4D() {}

		Matrix4D(const Vector4D& c1, const Vector4D& c2, const Vector4D& c3, const Vector4D& c4) {
			Set(c1, c2, c3, c4);
		}

		Matrix4D(float n00, float n01, float n02, float n03, float n10, float n11, float n12,
		float n13, float n20, float n21, float n22, float n23, float n30, float n31,
		float n32, float n33) {
			Set(n00, n01, n02, n03, n10, n11, n12, n13, n20, n21, n22, n23,
				n30, n31, n32, n33);
               }

		Matrix4D(const Matrix4x3RM& m) {
			Set(m.GetColumn(0), m.GetColumn(1), m.GetColumn(2), m.GetColumn(3));
		}

		Matrix4D& Set(const Vector4D& c1, const Vector4D& c2, const Vector4D& c3,
		const Vector4D& c4) {
			return Set(c1.x, c2.x, c3.x, c4.x, c1.y, c2.y, c3.y, c4.y, c1.z, c2.z, c3.z, c4.z,
		            c1.w, c2.w, c3.w, c4.w);
		}

		// Set row-by-row, column-by-column (second element is first row, second column.
		Matrix4D& Set(float n00, float n01, float n02, float n03, float n10,
		float n11, float n12, float n13, float n20, float n21, float n22, float n23,
		float n30, float n31, float n32, float n33) {
			n[0][0] = n00;
			n[1][0] = n01;
			n[2][0] = n02;
			n[3][0] = n03;
			n[0][1] = n10;
			n[1][1] = n11;
			n[2][1] = n12;
			n[3][1] = n13;
			n[0][2] = n20;
			n[1][2] = n21;
			n[2][2] = n22;
			n[3][2] = n23;
			n[0][3] = n30;
			n[1][3] = n31;
			n[2][3] = n32;
			n[3][3] = n33;
			return (*this);
		}

		// Return element at row i, column j.
		const float& operator ()(int i, int j) const
		{
			return (n[j][i]);
		}

		const float& Get(int column, int row) const {
			return (n[column][row]);
		}

 		// Get row j.
		Vector4D GetRow(int j) const
		{
			return (Vector4D(n[0][j], n[1][j], n[2][j], n[3][j]));
		}

		// Get column i.
		const Vector4D& GetColumn(int i) const
		{
			return (*reinterpret_cast<const Vector4D *>(n[i]));
		}

		Matrix4D& SetRow(int j, const Vector3D& row)
		{
			n[0][j] = row.x;
			n[1][j] = row.y;
			n[2][j] = row.z;
			n[3][j] = 0.0f;
			return (*this);
		}

		Matrix4D& SetRow(int j, const Vector4D& row)
		{
			n[0][j] = row.x;
			n[1][j] = row.y;
			n[2][j] = row.z;
			n[3][j] = row.w;
			return (*this);
		}

		DST_API Matrix4D& operator =(const Matrix3D& m) {
                    Set(m.n[0][0], m.n[1][0], m.n[2][0], 0.0f,
			m.n[0][1], m.n[1][1], m.n[2][1], 0.0f,
                        m.n[0][2], m.n[1][2], m.n[2][2], 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f);
		    return (*this);
		}

                DST_API Matrix4D& operator =(const Matrix4x3RM& m) {
		    Set(m.n[0][0], m.n[0][1], m.n[0][2], m.n[0][3],
			m.n[1][0], m.n[1][1], m.n[1][2], m.n[1][3],
                        m.n[2][0], m.n[2][1], m.n[2][2], m.n[2][3],
                        0.0f, 0.0f, 0.0f, 1.0f);
		    return (*this);
                }

		DST_API Matrix4D& operator *=(const Matrix4D& m);
		DST_API Matrix4D& operator *=(const Matrix3D& m);

		DST_API Matrix4D& SetIdentity(void);
                DST_API Matrix4D& AssignRotationAlongXAxis(float angle);
                DST_API Matrix4D& AssignRotationAlongYAxis(float angle);
                DST_API Matrix4D& AssignRotationAlongZAxis(float angle);
                DST_API Matrix4D& AssignTranslation(const Vector3D& translation);
                DST_API Matrix4D& AssignScaling(float scaling);
		// Return text respresentation. To be freed with delete [].
		char *GetString() const;

                // dstMultiply is a non-SIMD multiplication function.
		friend DST_API Matrix4D dstMultiply(const Matrix4D& m1, const Matrix4D& m2);
		friend DST_INLINE_ONLY Matrix4D dstInlineMultiply(const Matrix4D& m1, const Matrix4D& m2);
                // The * operator is potentially SIMD-accelerated.
		friend DST_API Matrix4D operator *(const Matrix4D& m1, const Matrix4D& m2);
		friend DST_API Matrix4D operator *(const Matrix4D& m1, const Matrix3D& m2);
		friend DST_API Vector4D operator *(const Matrix4D& m, const Vector4D& v);
		friend DST_API Vector4D operator *(const Vector4D& v, const Matrix4D& m);
		friend DST_API Vector4D operator *(const Matrix4D& m, const Vector3D& v);
		friend DST_API Vector4D operator *(const Vector3D& v, const Matrix4D& m);
		friend DST_API Vector4D operator *(const Matrix4D& m, const Point3D& p);
		friend DST_API Vector4D operator *(const Point3D& p, const Matrix4D& m);
		friend DST_API Vector4D operator *(const Matrix4D& m, const Vector2D& v);
		friend DST_API Vector4D operator *(const Vector2D& v, const Matrix4D& m);
		friend DST_API Vector4D operator *(const Matrix4D& m, const Point2D& p);
		friend DST_API Vector4D operator *(const Point2D& p, const Matrix4D& m);
		friend DST_API bool operator ==(const Matrix4D& m1, const Matrix4D& m2);
		friend DST_API bool operator !=(const Matrix4D& m1, const Matrix4D& m2);
} DST_ALIGNED(16);

DST_API Matrix4D Inverse(const Matrix4x3RM& m);
DST_API Matrix4D Transpose(const Matrix4x3RM& m);

DST_API float Determinant(const Matrix4D& m);
DST_API Matrix4D Inverse(const Matrix4D& m);
DST_API Matrix4D Adjugate(const Matrix4D& m);
DST_API Matrix4D Transpose(const Matrix4D& m);

// Inline multiplication functions.

DST_INLINE_ONLY Matrix4x3RM dstInlineMultiply(const Matrix4x3RM& m1, const Matrix4x3RM& m2) {
    return
        Matrix4x3RM(
            m1.Get(0, 0) * m2.Get(0, 0) + m1.Get(1, 0) * m2.Get(0, 1) + m1.Get(2, 0) * m2.Get(0, 2),
            m1.Get(0, 0) * m2.Get(1, 0) + m1.Get(1, 0) * m2.Get(1, 1) + m1.Get(2, 0) * m2.Get(1, 2),
            m1.Get(0, 0) * m2.Get(2, 0) + m1.Get(1, 0) * m2.Get(2, 1) + m1.Get(2, 0) * m2.Get(2, 2),
            m1.Get(0, 0) * m2.Get(3, 0) + m1.Get(1, 0) * m2.Get(3, 1) + m1.Get(2, 0) * m2.Get(3, 2) +
		m1.Get(3, 0),
            m1.Get(0, 1) * m2.Get(0, 0) + m1.Get(1, 1) * m2.Get(0, 1) + m1.Get(2, 1) * m2.Get(0, 2),
            m1.Get(0, 1) * m2.Get(1, 0) + m1.Get(1, 1) * m2.Get(1, 1) + m1.Get(2, 1) * m2.Get(1, 2),
            m1.Get(0, 1) * m2.Get(2, 0) + m1.Get(1, 1) * m2.Get(2, 1) + m1.Get(2, 1) * m2.Get(2, 2),
            m1.Get(0, 1) * m2.Get(3, 0) + m1.Get(1, 1) * m2.Get(3, 1) + m1.Get(2, 1) * m2.Get(3, 2) +
		m1.Get(3, 1),
            m1.Get(0, 2) * m2.Get(0, 0) + m1.Get(1, 2) * m2.Get(0, 1) + m1.Get(2, 2) * m2.Get(0, 2),
            m1.Get(0, 2) * m2.Get(1, 0) + m1.Get(1, 2) * m2.Get(1, 1) + m1.Get(2, 2) * m2.Get(1, 2),
            m1.Get(0, 2) * m2.Get(2, 0) + m1.Get(1, 2) * m2.Get(2, 1) + m1.Get(2, 2) * m2.Get(2, 2),
            m1.Get(0, 2) * m2.Get(3, 0) + m1.Get(1, 2) * m2.Get(3, 1) + m1.Get(2, 2) * m2.Get(3, 2) +
		m1.Get(3, 2));
}

DST_INLINE_ONLY Matrix4D dstInlineMultiply(const Matrix4D& m1, const Matrix4x3RM& m2) {
    return
        Matrix4D(m1.Get(0, 0) * m2.Get(0, 0) + m1.Get(1, 0) * m2.Get(0, 1) + m1.Get(2, 0) * m2.Get(0, 2),
            m1.Get(0, 0) * m2.Get(1, 0) + m1.Get(1, 0) * m2.Get(1, 1) + m1.Get(2, 0) * m2.Get(1, 2),
            m1.Get(0, 0) * m2.Get(2, 0) + m1.Get(1, 0) * m2.Get(2, 1) + m1.Get(2, 0) * m2.Get(2, 2),
            m1.Get(0, 0) * m2.Get(3, 0) + m1.Get(1, 0) * m2.Get(3, 1) + m1.Get(2, 0) * m2.Get(3, 2) +
		m1.Get(3, 0),
            m1.Get(0, 1) * m2.Get(0, 0) + m1.Get(1, 1) * m2.Get(0, 1) + m1.Get(2, 1) * m2.Get(0, 2),
            m1.Get(0, 1) * m2.Get(1, 0) + m1.Get(1, 1) * m2.Get(1, 1) + m1.Get(2, 1) * m2.Get(1, 2),
            m1.Get(0, 1) * m2.Get(2, 0) + m1.Get(1, 1) * m2.Get(2, 1) + m1.Get(2, 1) * m2.Get(2, 2),
            m1.Get(0, 1) * m2.Get(3, 0) + m1.Get(1, 1) * m2.Get(3, 1) + m1.Get(2, 1) * m2.Get(3, 2) +
		m1.Get(3, 1),
            m1.Get(0, 2) * m2.Get(0, 0) + m1.Get(1, 2) * m2.Get(0, 1) + m1.Get(2, 2) * m2.Get(0, 2),
            m1.Get(0, 2) * m2.Get(1, 0) + m1.Get(1, 2) * m2.Get(1, 1) + m1.Get(2, 2) * m2.Get(1, 2),
            m1.Get(0, 2) * m2.Get(2, 0) + m1.Get(1, 2) * m2.Get(2, 1) + m1.Get(2, 2) * m2.Get(2, 2),
            m1.Get(0, 2) * m2.Get(3, 0) + m1.Get(1, 2) * m2.Get(3, 1) + m1.Get(2, 2) * m2.Get(3, 2) +
		m1.Get(3, 2),
            m1.Get(0, 3) * m2.Get(0, 0) + m1.Get(1, 3) * m2.Get(0, 1) + m1.Get(2, 3) * m2.Get(0, 2),
            m1.Get(0, 3) * m2.Get(1, 0) + m1.Get(1, 3) * m2.Get(1, 1) + m1.Get(2, 3) * m2.Get(1, 2),
            m1.Get(0, 3) * m2.Get(2, 0) + m1.Get(1, 3) * m2.Get(2, 1) + m1.Get(2, 3) * m2.Get(2, 2),
            m1.Get(0, 3) * m2.Get(3, 0) + m1.Get(1, 3) * m2.Get(3, 1) + m1.Get(2, 3) * m2.Get(3, 2) + m1.Get(3, 3));
}

DST_INLINE_ONLY Matrix4D dstInlineMultiply(const Matrix4D& m1, const Matrix4D& m2) {
	return (Matrix4D(
		m1.n[0][0] * m2.n[0][0] + m1.n[1][0] * m2.n[0][1] + m1.n[2][0] * m2.n[0][2] + m1.n[3][0] * m2.n[0][3],
		m1.n[0][0] * m2.n[1][0] + m1.n[1][0] * m2.n[1][1] + m1.n[2][0] * m2.n[1][2] + m1.n[3][0] * m2.n[1][3],
		m1.n[0][0] * m2.n[2][0] + m1.n[1][0] * m2.n[2][1] + m1.n[2][0] * m2.n[2][2] + m1.n[3][0] * m2.n[2][3],
		m1.n[0][0] * m2.n[3][0] + m1.n[1][0] * m2.n[3][1] + m1.n[2][0] * m2.n[3][2] + m1.n[3][0] * m2.n[3][3],
		m1.n[0][1] * m2.n[0][0] + m1.n[1][1] * m2.n[0][1] + m1.n[2][1] * m2.n[0][2] + m1.n[3][1] * m2.n[0][3],
		m1.n[0][1] * m2.n[1][0] + m1.n[1][1] * m2.n[1][1] + m1.n[2][1] * m2.n[1][2] + m1.n[3][1] * m2.n[1][3],
		m1.n[0][1] * m2.n[2][0] + m1.n[1][1] * m2.n[2][1] + m1.n[2][1] * m2.n[2][2] + m1.n[3][1] * m2.n[2][3],
		m1.n[0][1] * m2.n[3][0] + m1.n[1][1] * m2.n[3][1] + m1.n[2][1] * m2.n[3][2] + m1.n[3][1] * m2.n[3][3],
		m1.n[0][2] * m2.n[0][0] + m1.n[1][2] * m2.n[0][1] + m1.n[2][2] * m2.n[0][2] + m1.n[3][2] * m2.n[0][3],
		m1.n[0][2] * m2.n[1][0] + m1.n[1][2] * m2.n[1][1] + m1.n[2][2] * m2.n[1][2] + m1.n[3][2] * m2.n[1][3],
		m1.n[0][2] * m2.n[2][0] + m1.n[1][2] * m2.n[2][1] + m1.n[2][2] * m2.n[2][2] + m1.n[3][2] * m2.n[2][3],
		m1.n[0][2] * m2.n[3][0] + m1.n[1][2] * m2.n[3][1] + m1.n[2][2] * m2.n[3][2] + m1.n[3][2] * m2.n[3][3],
		m1.n[0][3] * m2.n[0][0] + m1.n[1][3] * m2.n[0][1] + m1.n[2][3] * m2.n[0][2] + m1.n[3][3] * m2.n[0][3],
		m1.n[0][3] * m2.n[1][0] + m1.n[1][3] * m2.n[1][1] + m1.n[2][3] * m2.n[1][2] + m1.n[3][3] * m2.n[1][3],
		m1.n[0][3] * m2.n[2][0] + m1.n[1][3] * m2.n[2][1] + m1.n[2][3] * m2.n[2][2] + m1.n[3][3] * m2.n[2][3],
		m1.n[0][3] * m2.n[3][0] + m1.n[1][3] * m2.n[3][1] + m1.n[2][3] * m2.n[3][2] + m1.n[3][3] * m2.n[3][3]));
}

// Matrix functions with optional SIMD support.
// All matrices must be aligned on a 16-byte boundary.

DST_INLINE_ONLY void dstMatrixMultiply(const Matrix4D & DST_RESTRICT m1, const Matrix4D & DST_RESTRICT m2,
Matrix4D & DST_RESTRICT result) {
	DST_FUNC_LOOKUP(dstMatrixMultiply4x4CM)((const float *)&m1, (const float *)&m2, (float *)&result);
}

DST_INLINE_ONLY void dstMatrixMultiply(const Matrix4x3RM & DST_RESTRICT m1,
const Matrix4x3RM & DST_RESTRICT m2, Matrix4x3RM & DST_RESTRICT result) {
	DST_FUNC_LOOKUP(dstMatrixMultiply4x3RM)((const float *)&m1, (const float *)&m2, (float *)&result);
}

DST_INLINE_ONLY void dstMatrixMultiply(const Matrix4D & DST_RESTRICT m1,
const Matrix4x3RM & DST_RESTRICT m2, Matrix4D & DST_RESTRICT result) {
	DST_FUNC_LOOKUP(dstMatrixMultiply4x4CM4x3RM)((const float *)&m1, (const float *)&m2,
		(float *)&result);
}

// Multiply a constant matrix with an array of vectors.

DST_INLINE_ONLY void dstMatrixMultiplyVectors1x4(const Matrix4D & DST_RESTRICT m,
const Vector4D * DST_RESTRICT v1, Vector4D * DST_RESTRICT v2) {
	DST_FUNC_LOOKUP(dstMatrixMultiplyVectors1x4M4x4CMV4)((const float *)&m,
		(const float *)v1, (float *)v2);
}

DST_INLINE_ONLY void dstMatrixMultiplyVectors1xN(int n, const Matrix4D & DST_RESTRICT m,
const Vector4D * DST_RESTRICT v1, Vector4D * DST_RESTRICT v2) {
	DST_FUNC_LOOKUP(dstMatrixMultiplyVectors1xNM4x4CMV4)(n, (const float *)&m,
		(const float *)v1, (float *)v2);
}

DST_INLINE_ONLY void dstMatrixMultiplyVectors1x4(const Matrix4D & DST_RESTRICT m,
const Point3DPadded * DST_RESTRICT v1, Point3DPadded * DST_RESTRICT v2) {
	// Not yet SIMD-accelerated.
	DST_FUNC_LOOKUP_NO_SIMD(dstMatrixMultiplyVectors1x4M4x4CMP3P)((const float *)&m,
		(const float *)v1, (float *)v2);
}

DST_INLINE_ONLY void dstMatrixMultiplyVectors1xN(int n, const Matrix4D & DST_RESTRICT m,
const Point3DPadded * DST_RESTRICT v1, Point3DPadded * DST_RESTRICT v2) {
	// Not yet SIMD-accelerated.
	DST_FUNC_LOOKUP_NO_SIMD(dstMatrixMultiplyVectors1xNM4x4CMP3P)(n, (const float *)&m,
		(const float *)v1, (float *)v2);
}

DST_INLINE_ONLY void dstMatrixMultiplyVectors1xN(int n, const Matrix4D & DST_RESTRICT m,
const Vector3D * DST_RESTRICT v1, Vector3D * DST_RESTRICT v2) {
	// Not yet SIMD-accelerated.
	DST_FUNC_LOOKUP_NO_SIMD(dstMatrixMultiplyVectors1xNM4x4CMV3)(n, (const float *)&m,
		(const float *)v1, (float *)v2);
}

#endif // __defined(__DST_MATRIX_MATH_H__)

