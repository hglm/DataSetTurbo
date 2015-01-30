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

const dstSIMDFuncs SIMD_VAR(dst_simd_funcs_) = {
	SIMD_FUNC(dstCalculateDotProductsNxNV4),
	SIMD_FUNC(dstCalculateDotProductsNxNV3P),
	SIMD_FUNC(dstCalculateDotProductsNxNV3),

	SIMD_FUNC(dstCalculateDotProductsNx1V4),
	SIMD_FUNC(dstCalculateDotProductsNx1V3PV3),
	SIMD_FUNC(dstCalculateDotProductsNx1V3),
	SIMD_FUNC(dstCalculateDotProductsNx1P3PV4),
	SIMD_FUNC(dstCalculateDotProductsNx1P3V4),

	SIMD_FUNC(dstCalculateDotProductsAndCountNegativeNx1P3V4),
	SIMD_FUNC(dstCalculateDotProductsAndCountNegativeNx1P3PV4),

	SIMD_FUNC(dstCalculateMinAndMaxDotProductNx1V3),
	SIMD_FUNC(dstCalculateMinAndMaxDotProductNx1V4),

	SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V3),
	SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V3PV3),
	SIMD_FUNC(dstCalculateMinAndMaxDotProductNx3V4),

	SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V3),
	SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V3P),
	SIMD_FUNC(dstGetIndicesWithMinAndMaxDotProductNx1V4),

	SIMD_FUNC(dstMatrixMultiply4x4CM),
	SIMD_FUNC(dstMatrixMultiply4x3RM),
	SIMD_FUNC(dstMatrixMultiply4x4CM4x3RM),

	SIMD_FUNC(dstMatrixMultiplyVectors1x4M4x4CMV4),
	SIMD_FUNC(dstMatrixMultiplyVectors1xNM4x4CMV4),
	SIMD_FUNC(dstMatrixMultiplyVectors1x4M4x4CMP3P),
	SIMD_FUNC(dstMatrixMultiplyVectors1xNM4x4CMP3P),
	dstMatrixMultiplyVectors1xNM4x4CMV3NoSIMD,
};

