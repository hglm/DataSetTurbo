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

#ifndef __DST_CONFIG_H__
#define __DST_CONFIG_H__

// Include standard headers used in various library header files.
#include <stdlib.h>  // For NULL.
#include <stdint.h>  // For uint64_t etc.
#include <math.h>
#include <limits.h>  // For __WORDSIZE.
#include <float.h>   // For DBL_MAX.

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define DST_HELPER_SHARED_IMPORT __declspec(dllimport)
  #define DST_HELPER_SHARED_EXPORT __declspec(dllexport)
  #define DST_HELPER_SHARED_LOCAL
#else
  #if __GNUC__ >= 4
    #define DST_HELPER_SHARED_IMPORT __attribute__ ((visibility ("default")))
    #define DST_HELPER_SHARED_EXPORT __attribute__ ((visibility ("default")))
    #define DST_HELPER_SHARED_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DST_HELPER_SHARED_IMPORT
    #define DST_HELPER_SHARED_EXPORT
    #define DST_HELPER_SHARED_LOCAL
  #endif
#endif

// Now we use the generic helper definitions above to define DST_API and DST_LOCAL.
// DST_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing
// for static build). DST_LOCAL is used for non-api symbols.

#ifdef DST_SHARED
  // Defined if DST is compiled as a shared library/DLL.
  #ifdef DST_SHARED_EXPORTS
  // Defined if we are building the DST shared library (instead of using it).
    #define DST_API DST_HELPER_SHARED_EXPORT
  #else
    #define DST_API DST_HELPER_SHARED_IMPORT
  #endif // DST_SHARED_EXPORTS
  #define DST_LOCAL DST_HELPER_SHARED_LOCAL
#else // DST_SHARED is not defined: this means DST is a static lib.
  #define DST_API
  #define DST_LOCAL
#endif // DST_SHARED

#include "dstConfigParams.h"

// Alignment macros (on some platforms, aligning or packing data structures may improve
// performance.

#ifndef __GNUC__

#define DST_ALIGNED(n)
#define DST_PACKED
#define DST_LINE_SIZE 128
#define DST_INLINE_ONLY inline
#define DST_RESTRICT

#else

#define DST_ALIGNED(n) __attribute__ ((aligned(n)))
#define DST_PACKED __attribute__ ((packed))
// Very rough guess at a cache line size usable for alignment,
#ifdef OPENGL_ES2
#define DST_LINE_SIZE 64
#else
#define DST_LINE_SIZE 128
#endif
// Define a macro for always-inlining attributes that work with recent versions
// of gcc.
#define DST_INLINE_ONLY __attribute__((always_inline)) inline
#define DST_RESTRICT __restrict

#endif

// The number of bytes starting for discriminating between the dstMemcpySmall and
// dstMemcpyLarge variants.
#define DST_MEMCPY_THRESHOLD 256

// SIMD detection.

#ifndef DST_NO_SIMD

#ifndef DST_FIXED_SIMD
#define DST_FUNC_LOOKUP(f) dst_config.simd_funcs.f
#endif

// Set flags for SIMD implementations supported by the library, hardwired or optionally.
// Depends on the compiler, but modern versions of gcc support all of these.

#if defined(_MSC_VEC) || (defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__)))

// x86 platform.

#ifndef DST_FIXED_SIMD
#define DST_SSE2_SUPPORT
#define DST_SSE3_SUPPORT
#define DST_SSSE3_SUPPORT
#define DST_SSE4A_SUPPORT
//#define DST_SSE41_SUPPORT
//#define DST_SSE42_SUPPORT
//#define DST_AVX_SUPPORT
//#define DST_AVX_SSE4A_FMA4_SUPPORT
//#define DST_X86_AVX_FMA_SUPPORT
#endif

#ifdef DST_FIXED_SIMD
#ifdef DST_FIXED_SIMD_SSE2
#define DST_FUNC_LOOKUP(f) f ## SSE2
#define DST_SSE2_SUPPORT
#elif defined(DST_FIXED_SIMD_SSE3)
#define DST_FUNC_LOOKUP(f) f ## SSE3
#define DST_SSE3_SUPPORT
#elif defined(DST_FIXED_SIMD_SSSE3)
#define DST_FUNC_LOOKUP(f) f ## SSSE3
#define DST_SSSE3_SUPPORT
#elif defined(DST_FIXED_SIMD_SSE4A)
#define DST_FUNC_LOOKUP(f) f ## SSE4A
#define DST_SSE4A_SUPPORT
#elif defined(DST_FIXED_SIMD_SSE41)
#define DST_FUNC_LOOKUP dst_simd_funcs_SSE41
#define DST_SSE41_SUPPORT
#elif defined(DST_FIXED_SIMD_SSE42)
#define DST_FUNC_LOOKUP dst_simd_funcs_SSE42
#define DST_SSE42_SUPPORT
#elif defined(DST_FIXED_SIMD_AVX)
#define DST_FUNC_LOOKUP dst_simd_funcs_AVX
#define DST_AVX_SUPPORT
#else
#error Unspecified SIMD type for DST_FIXED_SIMD for x86.
#endif
#endif // defined(DST_FIXED_SIMD)

#elif defined(__arm__)

// ARM platform.

#define DST_NEON_SUPPORT

#ifdef DST_FIXED_SIMD
#ifdef DST_FIXED_SIMD_NEON
#define DST_FUNC_LOOKUP f ## NEON
#define DST_NEON_SUPPORT
#else
#error Unspecified SIMD type for DST_FIXED_SIMD for ARM.
#endif
#endif // defined(DST_FIXED_SIMD)

#endif /// defined(__arm__)

#else // defined(DST_NO_SIMD)

#define DST_FUNC_LOOKUP(f) f ## NoSIMD

#endif


enum {
	DST_FLAG_SIMD_ENABLED = 0x1,
	DST_FLAG_SIMD_256 = 0x2,
};

enum {
	DST_SIMD_NONE = 0,
	DST_SIMD_FIRST = 1,
	DST_SIMD_SSE2 = 1,
	DST_SIMD_SSE3 = 2,
	DST_SIMD_SSSE3 = 3,
	DST_SIMD_SSE4A = 4,
	DST_SIMD_SSE41 = 5,
	DST_SIMD_SSE42 = 6,
	DST_SIMD_AVX = 7,
	DST_SIMD_NEON = 8,
	DST_SIMD_AVX_SSE4A_FMA4 = 9,
	DST_SIMD_AVX_FMA = 10,
	DST_SIMD_LAST = 10
};

enum {
	// Flags reflecting CPU SIMD support levels, roughly corresponding to
	// the table above.
	DST_SIMD_FLAG_NONE = 0x1,
	DST_SIMD_FLAG_SSE2 = 0x2,
	DST_SIMD_FLAG_SSE3 = 0x4,
	DST_SIMD_FLAG_SSSE3 = 0x8,
	DST_SIMD_FLAG_SSE4A = 0x10,
	DST_SIMD_FLAG_SSE41 = 0x20,
	DST_SIMD_FLAG_SSE42 = 0x40,
	DST_SIMD_FLAG_AVX = 0x80,
	DST_SIMD_FLAG_NEON = 0x100,
	DST_SIMD_FLAG_FMA4 = 0x200,
	DST_SIMD_FLAG_FMA = 0x400
};

class dstSIMDFuncs {
public :
	// Function pointers for SIMD-accelerated functions.

	// Dot product functions.
	// Calculate dot products of NxN vectors.
	void (*dstCalculateDotProductsNxNV4)(int n, const float *v1, const float *v2,
		float *dot);
	void (*dstCalculateDotProductsNxNV3P)(int n, const float *v1,
		const float *v2, float *dot);
	void (*dstCalculateDotProductsNxNV3)(int n, const float *v1,
		const float *v2, float *dot);
	// Calculate dot products of Nx1 vectors.
	void (*dstCalculateDotProductsNx1V4)(int n, const float *v1,
		const float *v2, float *dot);
	void (*dstCalculateDotProductsNx1V3PV3)(int n, const float *v1,
		const float *v2, float *dot);
	void (*dstCalculateDotProductsNx1V3)(int n, const float *v1,
		const float *v2, float *dot);
	void (*dstCalculateDotProductsNx1P3PV4)(int n, const float *v1,
		const float *v2, float *dot);
	void (*dstCalculateDotProductsNx1P3V4)(int n, const float *v1,
		const float *v2, float *dot);
	// Calculate dot products of Nx1 vectors and count the number of negative
	// results.
	void (*dstCalculateDotProductsAndCountNegativeNx1P3V4)(int n, const float *p1,
		const float *v2, float *dot, int& negative_count);
	void (*dstCalculateDotProductsAndCountNegativeNx1P3PV4)(int n, const float *p1,
		const float *v2, float *dot, int& negative_count);
	// Determine the minimum and maximum dot product of an array of vertices with a
	// given constant vector.
	void (*dstCalculateMinAndMaxDotProductNx1V3)(int nu_vertices, const float *v1,
		const float *v2, float& min_dot_product, float& max_dot_product);
	void (*dstCalculateMinAndMaxDotProductNx1V4)(int nu_vertices, const float *v1,
		const float *v2, float& min_dot_product, float& max_dot_product);
	// Determine the minimum and maximum dot products of an array of vertices with three
	// constant vectors.
	void (*dstCalculateMinAndMaxDotProductNx3V3)(int nu_vertices, const float *v1,
		const float *v2, float *min_dot_product, float *max_dot_product);
	void (*dstCalculateMinAndMaxDotProductNx3V4)(int nu_vertices, const float *v1,
		const float *v2, float *min_dot_product, float *max_dot_product);
	// Determine the indices within an array of vertices that have the minimum and
	// maximum dot product with a given constant vector.
	void (*dstGetIndicesWithMinAndMaxDotProductNx1V3)(int nu_vertices, const float *v1,
		const float *v2, int& i_Pmin, int& i_Pmax);
	void (*dstGetIndicesWithMinAndMaxDotProductNx1V4)(int nu_vertices, const float *v1,
		const float *v2, int& i_Pmin, int& i_Pmax);
};

extern const dstSIMDFuncs dst_simd_funcs_NoSIMD;
#ifdef DST_SSE2_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_SSE2;
#endif
#ifdef DST_SSE3_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_SSE3;
#endif
#ifdef DST_SSSE3_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_SSSE3;
#endif
#ifdef DST_SSE4A_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_SSE4A;
#endif
#ifdef DST_SSE41_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_SSE41;
#endif
#ifdef DST_SSE42_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_SSE42;
#endif
#ifdef DST_AVX_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_AVX;
#endif
#ifdef DST_NEON_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_NEON;
#endif
#ifdef DST_AVX_SSE4A_FMA4_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_AVX_SSE4_FMA4;
#endif
#ifdef DST_AVX_FMA_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_AVX_FMA;
#endif

class DST_API dstConfig {
public :
	// Global configuration flags.
	uint32_t flags;
	// SIMD type that is currently actually used by the library.
	uint32_t simd_type;
	// All SIMD features that are supported by the current CPU.
	uint32_t simd_cpu_flags;
	// Function table.
	dstSIMDFuncs simd_funcs;
};


extern DST_API dstConfig dst_config;

DST_API void dstInit();

static DST_INLINE_ONLY bool dstCheckFlag(uint32_t flag) {
#ifdef DST_FIXED_SIMD
	// When SIMD is guaranteed, just return true.
	if (flag == DST_FLAG_SIMD_ENABLED)
		return true;
#endif
#if defined(DST_FIXED_SIMD_AVX) || defined(DST_AVX_SSE4A_FMA4) || defined(DST_AVX_FMA)
	// When 256-bit SIMD is guaranteed, just return true.
	if (flag == DST_FLAG_SIMD_256)
		return true;
#endif
	return (dst_config.flags & flag) != 0;
}

static DST_INLINE_ONLY bool dstCheckSIMDCPUFlag(uint32_t flag) {
#ifdef DST_FIXED_SIMD_SSE2
	if (flag == DST_SIMD_FLAG_SSE2)
		// When SSE2 is guaranteed, just return true.
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_SSE3
	if (flags == DST_SIMD_SSE2 || flag == DST_SIMD_FLAG_SSE3)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_SSSE3
	if (flag >= DST_SIMD_FLAG_SSE2 && flag <= DST_SIMD_FLAG_SSSE3)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_SSE4A
	if (flag == DST_SIMD_FLAG_SSE2 || flag == DST_SIMD_FLAG_SSE3 ||
	flag == DST_SIMD_FLAG_SSE4A)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_SSE41
	if ((flag >= DST_SIMD_FLAG_SSE2 && flag <= DST_SIMD_FLAG_SSE41) &&
	flag != DST_SIMD_FLAG_SSE4A)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_SSE42
	if ((flag >= DST_SIMD_FLAG_SSE2 && flag <= DST_SIMD_FLAG_SSE42) &&
	flag != DST_SIMD_FLAG_SSE4A)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_AVX
	if ((flag >= DST_SIMD_FLAG_SSE2 && flag <= DST_SIMD_FLAG_AVX) &&
	flag != DST_SIMD_FLAG_SSE4A)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_NEON
	if (flag == DST_SIMD_FLAG_NEON)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_AVX_SSE4A_FMA4
	if (flag >= DST_SIMD_FLAG_SSE2 && flag <= DST_SIMD_FLAG_FMA4)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_AVX_FMA
	if ((flag >= DST_SIMD_FLAG_SSE2 && flag <= DST_SIMD_FLAG_AVX) ||
	flag == DST_SIMD_FLAG_FMA)
		return true;
	else
		return false;
#endif
	return (dst_config.simd_cpu_flags & flag) != 0;
}

DST_API void dstSetSIMDType(int simd_type);

DST_INLINE_ONLY int dstGetSIMDType() {
	return dst_config.simd_type;
}

DST_API const char *dstGetSIMDTypeString(int simd_type);

#endif // !defined(__DST_CONFIG_H__)

