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



// No SIMD.

DST_API void dstCalculateDotProductsNxNV4NoSIMD(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNxNV3PNoSIMD(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNxNV3NoSIMD(int n, const float *f1, const float *f2,
	float *dot);

DST_API void dstCalculateDotProductsNx1V4NoSIMD(int n, const float *f1, const float *f2,
	float 	*dot);
DST_API void dstCalculateDotProductsNx1V3NoSIMD(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNx1V3PV3NoSIMD(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNx1P3PV4NoSIMD(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNx1P3V4NoSIMD(int n, const float *f1, const float *f2,
	float *dot);

DST_API void dstCalculateDotProductsAndCountNegativeNx1P3PV4NoSIMD(
	int n, const float *f1, const float *f2, float *dot, int& negative_count);
DST_API void dstCalculateDotProductsAndCountNegativeNx1P3V4NoSIMD(
	int n, const float *f1, const float *f2, float *dot, int& negative_count);

DST_API void dstCalculateMinAndMaxDotProductNx1V3NoSIMD(int nu_vertices, const float *v1,
	const float *v2, float& min_dot_product, float& max_dot_product);
DST_API void dstCalculateMinAndMaxDotProductNx1V3PNoSIMD(int nu_vertices, const float *v1,
	const float *v2, float& min_dot_product, float& max_dot_product);
DST_API void dstCalculateMinAndMaxDotProductNx1V4NoSIMD(int nu_vertices, const float *v1,
	const float *v2, float& min_dot_product, float& max_dot_product);

DST_API void dstCalculateMinAndMaxDotProductNx3V3NoSIMD(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);
DST_API void dstCalculateMinAndMaxDotProductNx3V3PV3NoSIMD(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);
DST_API void dstCalculateMinAndMaxDotProductNx3V4NoSIMD(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);

DST_API void dstGetIndicesWithMinAndMaxDotProductNx1V3NoSIMD(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);
DST_API void dstGetIndicesWithMinAndMaxDotProductNx1V3PNoSIMD(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);
DST_API void dstGetIndicesWithMinAndMaxDotProductNx1V4NoSIMD(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);

DST_API void dstMatrixMultiply4x4CMNoSIMD(const float *f1, const float *f2, float *f3);
DST_API void dstMatrixMultiply4x3RMNoSIMD(const float *f1, const float *f2, float *f3);
DST_API void dstMatrixMultiply4x4CM4x3RMNoSIMD(const float *f1, const float *f2, float *f3);

DST_API void dstMatrixMultiplyVectors1x4M4x4CMV4NoSIMD(const float *m, const float *v, float *v_result);
DST_API void dstMatrixMultiplyVectors1xNM4x4CMV4NoSIMD(int n, const float *m, const float *v,
	float *v_result);
DST_API void dstMatrixMultiplyVectors1x4M4x4CMP3PNoSIMD(const float *m, const float *v,
	float *v_result);
DST_API void dstMatrixMultiplyVectors1xNM4x4CMP3PNoSIMD(int n, const float *m, const float *v,
	float *v_result);
DST_API void dstMatrixMultiplyVectors1xNM4x4CMV3NoSIMD(int n, const float *m, const float *v,
	float *v_result);

// SIMD variant.

// Dot products.

DST_API void SIMD_FUNC(dstCalculateDotProductsNxNV4)(int n, const float *f1, const float *f2, float *dot);
DST_API void SIMD_FUNC(dstCalculateDotProductsNxNV3P)(int n, const float *f1, const float *f2,
	float *dot);
DST_API void SIMD_FUNC(dstCalculateDotProductsNxNV3)(int n, const float *f1, const float *f2, float *dot);

DST_API void SIMD_FUNC(dstCalculateDotProductsNx1V4)(int n, const float *f1, const float *f2, float *dot);
DST_API void SIMD_FUNC(dstCalculateDotProductsNx1V3)(int n, const float *f1, const float *f2, float *dot);
DST_API void SIMD_FUNC(dstCalculateDotProductsNx1V3PV3)(int n, const float *f1, const float *f2,
	float *dot);
DST_API void SIMD_FUNC(dstCalculateDotProductsNx1P3PV4)(int n, const float *f1, const float *f2,
	float *dot);
DST_API void SIMD_FUNC(dstCalculateDotProductsNx1P3V4)(int n, const float *f1, const float *f2,
	float *dot);

DST_API void SIMD_FUNC(dstCalculateDotProductsAndCountNegativeNx1P3PV4)(
	int n, const float *f1, const float *f2, float *dot, int& negative_count);
DST_API void SIMD_FUNC(dstCalculateDotProductsAndCountNegativeNx1P3V4)(
	int n, const float *f1, const float *f2, float *dot, int& negative_count);

DST_API void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx1V3)(int nu_vertices, const float *v1,
	const float *v2, float& min_dot_product, float& max_dot_product);
DST_API void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx1V4)(int nu_vertices, const float *v1,
	const float *v2, float& min_dot_product, float& max_dot_product);

DST_API void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V3)(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);
DST_API void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V3PV3)(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);
DST_API void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V4)(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);

DST_API void SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V3)(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);
DST_API void SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V3P)(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);
DST_API void SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V4)(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);

// Matrix multiplication

DST_API void SIMD_FUNC(dstMatrixMultiply4x4CM)(const float *f1, const float *f2, float *f3);
DST_API void SIMD_FUNC(dstMatrixMultiply4x3RM)(const float *f1, const float *f2, float *f3);
DST_API void SIMD_FUNC(dstMatrixMultiply4x4CM4x3RM)(const float *f1, const float *f2, float *f3);

DST_API void SIMD_FUNC(dstMatrixMultiplyVectors1x4M4x4CMV4)(const float *m, const float *v,
	float *v_result);
DST_API void SIMD_FUNC(dstMatrixMultiplyVectors1xNM4x4CMV4)(int n, const float *m, const float *v,
	float *v_result);
DST_API void SIMD_FUNC(dstMatrixMultiplyVectors1x4M4x4CMP3P)(const float *m, const float *v,
	float *v_result);
DST_API void SIMD_FUNC(dstMatrixMultiplyVectors1xNM4x4CMP3P)(int n, const float *m, const float *v,
	float *v_result);
// dstMatrixMultiplyVectors1xNM4x4CMV3 not yet implemented.


