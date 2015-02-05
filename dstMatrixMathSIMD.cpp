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

#ifndef DST_NO_SIMD

#include "dstMatrixMath.h"

// Non-inline member funtions for Matrix3D, Matrix4D and Matrix4x3RM classes
// that use SIMD. SIMD functions are taken advantage of for the following multiplications:
// - Matrix4 * Matrix4
// - Matix4x3RM * Matrix4x3RM
// - Matrix4D * Matrix4x3RM
//
// When DST_NO_SIMD is defined, these member function will be defined as regular C++
// functions in sreMatrixMath.cpp.
//
// Because of the nested function call from a table lookup, this is not very efficient unless
// DST_FIXED_SIMD is defined.

Matrix4D operator *(const Matrix4D& DST_RESTRICT m1, const Matrix4D& DST_RESTRICT m2) {
	Matrix4D m3;
	DST_FUNC_LOOKUP(dstMatrixMultiply4x4CM)((const float *)&m1, (const float *)&m2, (float *)&m3);
	return m3;
}

Matrix4x3RM operator *(const Matrix4x3RM& DST_RESTRICT m1, const Matrix4x3RM& DST_RESTRICT m2) {
	Matrix4x3RM m3;
	DST_FUNC_LOOKUP(dstMatrixMultiply4x3RM)((const float *)&m1, (const float *)&m2, (float *)&m3);
	return m3;
}

Matrix4D operator *(const Matrix4D& DST_RESTRICT m1, const Matrix4x3RM& DST_RESTRICT m2) {
	Matrix4D m3;
	DST_FUNC_LOOKUP(dstMatrixMultiply4x4CM4x3RM)((const float *)&m1, (const float *)&m2, (float *)&m3);
	return m3;
}

#endif

