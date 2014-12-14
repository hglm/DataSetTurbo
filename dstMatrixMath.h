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
		
		DST_API Matrix3D(const Vector3D& c1, const Vector3D& c2, const Vector3D& c3);
		DST_API Matrix3D(float n00, float n01, float n02, float n10, float n11, float n12, float n20, float n21, float n22);
		
		DST_API Matrix3D& Set(const Vector3D& c1, const Vector3D& c2, const Vector3D& c3);
		DST_API Matrix3D& Set(float n00, float n01, float n02, float n10, float n11, float n12, float n20, float n21, float n22);
		
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

class MatrixTransform;

class DST_API Matrix4D
{
	public:
		float	n[4][4];
	
	public:
		
		Matrix4D() {}
		
		Matrix4D(const Vector4D& c1, const Vector4D& c2, const Vector4D& c3, const Vector4D& c4);
		Matrix4D(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23, float n30, float n31, float n32, float n33);
		Matrix4D(const MatrixTransform& m);
		
		Matrix4D& Set(const Vector4D& c1, const Vector4D& c2, const Vector4D& c3, const Vector4D& c4);
		Matrix4D& Set(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23, float n30, float n31, float n32, float n33);
		
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
		
		DST_API Matrix4D& operator =(const Matrix3D& m);
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


DST_API float Determinant(const Matrix4D& m);
DST_API Matrix4D Inverse(const Matrix4D& m);
DST_API Matrix4D Adjugate(const Matrix4D& m);
DST_API Matrix4D Transpose(const Matrix4D& m);

// Custom MatrixTransform class. Transform matrices are zero at n30, n31 and n32 and 1.0 at n33.

class DST_API MatrixTransform
{
	public:
		// The interal storage format has been switched from column-major to
		// row-major for MatrixTransform only.
		float n[3][4];

	public:
		MatrixTransform() {}
		
		MatrixTransform(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23);
		MatrixTransform(const Vector3D& c1, const Vector3D& c2, const Vector3D& c3, const Vector3D& c4);
		MatrixTransform& Set(float n00, float n01, float n02, float n03, float n10, float n11, float n12, float n13, float n20, float n21, float n22, float n23);
		MatrixTransform& Set(const Vector3D& c1, const Vector3D& c2, const Vector3D& c3, const Vector3D& c4);

		// Return element at row i, column j.
		const float& operator ()(int i, int j) const
		{
			return (n[i][j]);
		}

		const float& Get(int column, int row) const {
			return (n[row][column]);
		}

 		// Get column i.
		Vector4D GetColumn(int i) const
		{
			return Vector4D(n[0][i], n[1][i], n[2][i],
                            Vector4D(0.0f, 0.0f, 0.0f, 1.0f)[i]);
		}

		const Vector4D& GetRow(int j) const
		{
			return (*reinterpret_cast<const Vector4D *>(n[j]));
		}

		MatrixTransform& SetRow(int j, const Vector3D& row)
		{
			n[j][0] = row.x;
			n[j][1] = row.y;
			n[j][2] = row.z;
			n[j][3] = 0.0f;
			return (*this);
		}
		
		MatrixTransform& SetRow(int j, const Vector4D& row)
		{
			n[j][0] = row.x;
			n[j][1] = row.y;
			n[j][2] = row.z;
			n[j][3] = row.w;
			return (*this);
		}

		DST_API MatrixTransform& operator *=(const MatrixTransform& m);

		DST_API MatrixTransform& SetIdentity(void);
                DST_API MatrixTransform& AssignRotationAlongXAxis(float angle);
                DST_API MatrixTransform& AssignRotationAlongYAxis(float angle);
                DST_API MatrixTransform& AssignRotationAlongZAxis(float angle);
                DST_API MatrixTransform& AssignTranslation(const Vector3D& translation);
                DST_API MatrixTransform& AssignScaling(float scaling);
		// Return text respresentation. To be freed with delete [].
		char *GetString() const;

		friend DST_API MatrixTransform operator *(const MatrixTransform& m1, const MatrixTransform& m2);
		friend DST_API Matrix4D operator *(const Matrix4D& m1, const MatrixTransform& m2);
		friend DST_API Vector4D operator *(const MatrixTransform& m, const Vector4D& v);
		friend DST_API Vector4D operator *(const Vector4D& v, const MatrixTransform& m);
		friend DST_API Vector4D operator *(const MatrixTransform& m, const Vector3D& v);
		friend DST_API Vector4D operator *(const Vector3D& v, const MatrixTransform& m);
		friend DST_API Vector4D operator *(const MatrixTransform& m, const Point3D& p);
		friend DST_API Vector4D operator *(const Point3D& p, const MatrixTransform& m);
		friend DST_API bool operator ==(const MatrixTransform& m1, const MatrixTransform& m2);
		friend DST_API bool operator !=(const MatrixTransform& m1, const MatrixTransform& m2);
} DST_ALIGNED(16);

DST_API Matrix4D Inverse(const MatrixTransform& m);
DST_API Matrix4D Transpose(const MatrixTransform& m);


// Multiply a constant matrix with an array of vectors.

DST_API void MatrixMultiplyVectors(int n, const Matrix4D& m, const Vector4D *v1, Vector4D *v2);

DST_API void MatrixMultiplyVectors(int n, const Matrix4D& m, const Point3D *p1, Point3D *p2);

DST_API void MatrixMultiplyVectors(int n, const MatrixTransform& m, const Vector3D *v1,
Vector3D *v2);

#endif // __defined(__DST_MATRIX_MATH_H__)
