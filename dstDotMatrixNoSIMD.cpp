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

// Non-SIMD implemenations of low-level SIMD-accelerated functions. These functions cast
// pointers back towards VectorMath types.

#include "dstConfig.h"
#include "dstVectorMath.h"

#define SIMD_FUNC(f) f ## NoSIMD
#define SIMD_VAR(f) SIMD_FUNC(f)

#include "dstSIMDFuncs.h"
#include "dstSIMDFuncTable.cpp"

// NxN.

template <class T>
static DST_INLINE_ONLY void dstCalculateDotProductsNxNNoSIMDTemplate(int n,
const T * DST_RESTRICT v1, const T * DST_RESTRICT v2, float * DST_RESTRICT dot) {
	int i = 0;
	for (; i + 3 < n; i += 4) {
		dot[i] = Dot(v1[i], v2[i]);
		dot[i + 1] = Dot(v1[i + 1], v2[i + 1]);
		dot[i + 2] = Dot(v1[i + 2], v2[i + 2]);
		dot[i + 3] = Dot(v1[i + 3], v2[i + 3]);
	}
	for (; i < n; i++)
		dot[i] = Dot(v1[i], v2[i]);
}

void dstCalculateDotProductsNxNV4NoSIMD(int n, const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstCalculateDotProductsNxNNoSIMDTemplate(n,
		(const Vector4D *)f1, (const Vector4D *)f2, dot);
}

void dstCalculateDotProductsNxNV3PNoSIMD(int n, const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstCalculateDotProductsNxNNoSIMDTemplate(n,
		(const Vector3DPadded *)f1, (const Vector3DPadded *)f2, dot);
}

void dstCalculateDotProductsNxNV3NoSIMD(int n, const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, float * DST_RESTRICT dot) {
	dstCalculateDotProductsNxNNoSIMDTemplate(n,
		(const Vector3D *)f1, (const Vector3D *)f2, dot);
}

// Nx1.

template <class T, class U>
static DST_INLINE_ONLY void dstCalculateDotProductsNx1NoSIMDTemplate(int n,
const T * DST_RESTRICT v1, const U& DST_RESTRICT v2, float * DST_RESTRICT dot) {
	int i = 0;
	// When T is Point3D or Point3DPadded and U is Vector4D, Dot() will do the right
	// thing.
	for (; i + 3 < n; i += 4) {
		dot[i] = Dot(v1[i], v2);
		dot[i + 1] = Dot(v1[i + 1], v2);
		dot[i + 2] = Dot(v1[i + 2], v2);
		dot[i + 3] = Dot(v1[i + 3], v2);
	}
	for (; i < n; i++)
		dot[i] = Dot(v1[i], v2);
}

void dstCalculateDotProductsNx1V4NoSIMD(int n, const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, float *dot) {
	dstCalculateDotProductsNx1NoSIMDTemplate(n, (const Vector4D *)f1, *(const Vector4D *)f2,
		dot);
}

void dstCalculateDotProductsNx1V3NoSIMD(int n, const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, float *dot) {
	dstCalculateDotProductsNx1NoSIMDTemplate(n, (const Vector3D *)f1, *(const Vector3D *)f2,
		dot);
}

void dstCalculateDotProductsNx1V3PV3NoSIMD(int n, const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, float *dot) {
	dstCalculateDotProductsNx1NoSIMDTemplate(n, (const Vector3DPadded *)f1,
		*(const Vector3D *)f2, dot);
}

void dstCalculateDotProductsNx1P3PV4NoSIMD(int n, const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, float *dot) {
	dstCalculateDotProductsNx1NoSIMDTemplate(n, (const Point3DPadded *)f1,
		*(const Vector4D *)f2, dot);
}

void dstCalculateDotProductsNx1P3V4NoSIMD(int n, const float * DST_RESTRICT f1,
const float * DST_RESTRICT f2, float *dot) {
	dstCalculateDotProductsNx1NoSIMDTemplate(n, (const Point3D *)f1,
		*(const Vector4D *)f2, dot);
}

// Nx1 and count negative.

template <class T, class U>
static DST_INLINE_ONLY void dstCalculateDotProductsAndCountNegativeNx1Template(int n,
const T * DST_RESTRICT p1, const U& DST_RESTRICT v2, float * DST_RESTRICT dot,
int& DST_RESTRICT count) {
    int i = 0;
    count = 0;
    for (; i + 3 < n; i += 4) {
        dot[i] = Dot(p1[i], v2);
        dot[i + 1] = Dot(p1[i + 1], v2);
        dot[i + 2] = Dot(p1[i + 2], v2);
        dot[i + 3] = Dot(p1[i + 3], v2);
        count += (dot[i] < 0.0f) + (dot[i + 1] < 0.0f) + (dot[i + 2] < 0.0f) +
            (dot[i + 3] < 0.0f);
    }
    for (; i < n; i++) {
        dot[i] = Dot(p1[i], v2);
        if (dot[i] < 0.0f)
            count++;
    }
}

void dstCalculateDotProductsAndCountNegativeNx1P3PV4NoSIMD(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float *dot, int& negative_count) {
	dstCalculateDotProductsAndCountNegativeNx1Template(n, (const Point3DPadded *)f1,
		*(const Vector4D *)f2, dot, negative_count);
}

void dstCalculateDotProductsAndCountNegativeNx1P3V4NoSIMD(int n,
const float * DST_RESTRICT f1, const float * DST_RESTRICT f2, float *dot, int& negative_count) {
	dstCalculateDotProductsAndCountNegativeNx1Template(n, (const Point3D *)f1,
		*(const Vector4D *)f2, dot, negative_count);
}

// Determine the minimum and maximum dot product of an array of vertices with a
// given constant vector (Nx1).

template <class T>
static void dstCalculateMinAndMaxDotProductNx1Template(int n, const T * DST_RESTRICT v1,
const T& DST_RESTRICT v2, float& DST_RESTRICT min_dot_product,
float& DST_RESTRICT max_dot_product) {
	min_dot_product = FLT_MAX;
	max_dot_product = - FLT_MAX;
	for (int i = 0; i < n; i++) {
		min_dot_product = minf(min_dot_product, Dot(v1[i], v2));
		max_dot_product = maxf(max_dot_product, Dot(v1[i], v2));
	}
}

void dstCalculateMinAndMaxDotProductNx1V3NoSIMD(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
float& DST_RESTRICT min_dot_product, float& DST_RESTRICT max_dot_product) {
	dstCalculateMinAndMaxDotProductNx1Template(n, (const Vector3D *)v1,
		*(const Vector3D *)v2, min_dot_product, max_dot_product);
}

void dstCalculateMinAndMaxDotProductNx1V4NoSIMD(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
float& DST_RESTRICT min_dot_product, float& DST_RESTRICT max_dot_product) {
	dstCalculateMinAndMaxDotProductNx1Template(n, (const Vector4D *)v1,
		*(const Vector4D *)v2, min_dot_product, max_dot_product);
}

// Determine the minimum and maximum dot products of an array of vertices with three
// constant vectors.

template <class T>
void DST_INLINE_ONLY dstCalculateMinAndMaxDotProductNx3Template(int n,
const T * DST_RESTRICT v1, const T * DST_RESTRICT v2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product) {
    for (int j = 0; j < 3; j++) {
        min_dot_product[j] = FLT_MAX;
        max_dot_product[j] = - FLT_MAX;
    }
    // Process the remaining vertices.
    for (int i = 0; i < n; i++)
        for (int j = 0; j < 3; j++) {
            min_dot_product[j] = minf(min_dot_product[j], Dot(v1[i], v2[j]));
            max_dot_product[j] = maxf(max_dot_product[j], Dot(v1[i], v2[j]));
        }
}

void dstCalculateMinAndMaxDotProductNx3V3NoSIMD(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product) {
	dstCalculateMinAndMaxDotProductNx3Template(n, (const Vector3D *)v1,
		(const Vector3D *)v2, min_dot_product, max_dot_product);
}

void dstCalculateMinAndMaxDotProductNx3V4NoSIMD(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product) {
	dstCalculateMinAndMaxDotProductNx3Template(n, (const Vector4D *)v1,
		(const Vector4D *)v2, min_dot_product, max_dot_product);
}

// Determine the indices within an array of vertices that have the minimum and
// maximum dot product with a given constant vector.

template <class T>
static void DST_INLINE_ONLY dstGetIndicesWithMinAndMaxDotProductNx1Template(int n,
const T * DST_RESTRICT v1, const T & DST_RESTRICT v2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax) {
	float min_dot_product = FLT_MAX;
	float max_dot_product = - FLT_MAX;
	for (int i = 0; i < n; i++) {
		float dot = Dot(v1[i], v2);
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

void dstGetIndicesWithMinAndMaxDotProductNx1V3NoSIMD(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax) {
	dstGetIndicesWithMinAndMaxDotProductNx1Template(n,
		(const Vector3D *)v1, *(const Vector3D *)v2, i_Pmin, i_Pmax);
}

void dstGetIndicesWithMinAndMaxDotProductNx1V4NoSIMD(int n,
const float * DST_RESTRICT v1, const float * DST_RESTRICT v2, int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax) {
	dstGetIndicesWithMinAndMaxDotProductNx1Template(n,
		(const Vector4D *)v1, *(const Vector4D *)v2, i_Pmin, i_Pmax);
}

