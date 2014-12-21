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
DST_API void dstCalculateMinAndMaxDotProductNx1V4NoSIMD(int nu_vertices, const float *v1,
	const float *v2, float& min_dot_product, float& max_dot_product);

DST_API void dstCalculateMinAndMaxDotProductNx3V3NoSIMD(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);
DST_API void dstCalculateMinAndMaxDotProductNx3V4NoSIMD(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);

DST_API void dstGetIndicesWithMinAndMaxDotProductNx1V3NoSIMD(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);
DST_API void dstGetIndicesWithMinAndMaxDotProductNx1V4NoSIMD(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);

// SIMD variant.

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
DST_API void SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V4)(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);

DST_API void SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V3)(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);
DST_API void SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V4)(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);

#if 0
// SSE2.

DST_API void dstCalculateDotProductsNxNV4SSE3(int n, const float *f1, const float *f2, float *dot);
DST_API void dstCalculateDotProductsNxNV3PSSE3(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNxNV3SSE3(int n, const float *f1, const float *f2, float *dot);

DST_API void dstCalculateDotProductsNx1V4SSE3(int n, const float *f1, const float *f2, float *dot);
DST_API void dstCalculateDotProductsNx1V3SSE3(int n, const float *f1, const float *f2, float *dot);
DST_API void dstCalculateDotProductsNx1V3PV3SSE3(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNx1P3PV4SSE3(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNx1P3V4SSE3(int n, const float *f1, const float *f2,
	float *dot);

DST_API void dstCalculateDotProductsAndCountNegativeNx1P3PV4SSE3(
	int n, const float *f1, const float *f2, float *dot, int& negative_count);
DST_API void dstCalculateDotProductsAndCountNegativeNx1P3V4SSE3(
	int n, const float *f1, const float *f2, float *dot, int& negative_count);

DST_API void dstCalculateMinAndMaxDotProductNx1V3SSE3(int nu_vertices, const float *v1,
	const float *v2, float& min_dot_product, float& max_dot_product);
DST_API void dstCalculateMinAndMaxDotProductNx1V4SSE3(int nu_vertices, const float *v1,
	const float *v2, float& min_dot_product, float& max_dot_product);

DST_API void dstCalculateMinAndMaxDotProductNx3V3SSE3(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);
DST_API void dstCalculateMinAndMaxDotProductNx3V4SSE3(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);

DST_API void dstGetIndicesWithMinAndMaxDotProductNx1V3SSE3(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);
DST_API void dstGetIndicesWithMinAndMaxDotProductNx1V4SSE3(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);

// SSSE3.

DST_API void dstCalculateDotProductsNxNV4SSSE3(int n, const float *f1, const float *f2, float *dot);
DST_API void dstCalculateDotProductsNxNV3PSSSE3(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNxNV3SSSE3(int n, const float *f1, const float *f2, float *dot);

DST_API void dstCalculateDotProductsNx1V4SSSE3(int n, const float *f1, const float *f2, float *dot);
DST_API void dstCalculateDotProductsNx1V3SSSE3(int n, const float *f1, const float *f2, float *dot);
DST_API void dstCalculateDotProductsNx1V3PV3SSSE3(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNx1P3PV4SSSE3(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNx1P3V4SSSE3(int n, const float *f1, const float *f2,
	float *dot);

DST_API void dstCalculateDotProductsAndCountNegativeNx1P3PV4SSSE3(
	int n, const float *f1, const float *f2, float *dot, int& negative_count);
DST_API void dstCalculateDotProductsAndCountNegativeNx1P3V4SSSE3(
	int n, const float *f1, const float *f2, float *dot, int& negative_count);

DST_API void dstCalculateMinAndMaxDotProductNx1V3SSSE3(int nu_vertices, const float *v1,
	const float *v2, float& min_dot_product, float& max_dot_product);
DST_API void dstCalculateMinAndMaxDotProductNx1V4SSSE3(int nu_vertices, const float *v1,
	const float *v2, float& min_dot_product, float& max_dot_product);

DST_API void dstCalculateMinAndMaxDotProductNx3V3SSSE3(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);
DST_API void dstCalculateMinAndMaxDotProductNx3V4SSSE3(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);

DST_API void dstGetIndicesWithMinAndMaxDotProductNx1V3SSSE3(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);
DST_API void dstGetIndicesWithMinAndMaxDotProductNx1V4SSSE3(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);

// SSE4A.

DST_API void dstCalculateDotProductsNxNV4SSE4A(int n, const float *f1, const float *f2, float *dot);
DST_API void dstCalculateDotProductsNxNV3PSSE4A(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNxNV3SSE4A(int n, const float *f1, const float *f2, float *dot);

DST_API void dstCalculateDotProductsNx1V4SSE4A(int n, const float *f1, const float *f2, float *dot);
DST_API void dstCalculateDotProductsNx1V3SSE4A(int n, const float *f1, const float *f2, float *dot);
DST_API void dstCalculateDotProductsNx1V3PV3SSE4A(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNx1P3PV4SSE4A(int n, const float *f1, const float *f2,
	float *dot);
DST_API void dstCalculateDotProductsNx1P3V4SSE4A(int n, const float *f1, const float *f2,
	float *dot);

DST_API void dstCalculateDotProductsAndCountNegativeNx1P3PV4SSE4A(
	int n, const float *f1, const float *f2, float *dot, int& negative_count);
DST_API void dstCalculateDotProductsAndCountNegativeNx1P3V4SSE4A(
	int n, const float *f1, const float *f2, float *dot, int& negative_count);

DST_API void dstCalculateMinAndMaxDotProductNx1V3SSE4A(int nu_vertices, const float *v1,
	const float *v2, float& min_dot_product, float& max_dot_product);
DST_API void dstCalculateMinAndMaxDotProductNx1V4SSE4A(int nu_vertices, const float *v1,
	const float *v2, float& min_dot_product, float& max_dot_product);

DST_API void dstCalculateMinAndMaxDotProductNx3V3SSE4A(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);
DST_API void dstCalculateMinAndMaxDotProductNx3V4SSE4A(int nu_vertices, const float *v1,
	const float *v2, float *min_dot_product, float *max_dot_product);

DST_API void dstGetIndicesWithMinAndMaxDotProductNx1V3SSE4A(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);
DST_API void dstGetIndicesWithMinAndMaxDotProductNx1V4SSE4A(int nu_vertices, const float *v1,
	const float *v2, int& i_Pmin, int& i_Pmax);

#endif
