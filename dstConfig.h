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

// The amount of stores in bytes in a SIMD function that will trigger the use of
// the streaming (non-temporal storage) version of the function, sparing L1 cache
// pollution, which faster for larger data sets. Should be set to a value somewhat
// lower than the L1 data cache size. For modern AMD processors it should be something
// like 56000, for Intel processors 28000.
#define DST_STREAM_THRESHOLD 28000

// SIMD detection.

#ifndef DST_NO_SIMD

#ifndef DST_FIXED_SIMD
#define DST_FUNC_LOOKUP(f) dst_config.simd_funcs.f
#define DST_FUNC_STREAM_LOOKUP(f) dst_config.simd_funcs_stream.f
#endif
#define DST_FUNC_LOOKUP_NO_SIMD(f) dst_simd_funcs_NoSIMD.f

// Set flags for SIMD implementations supported by the library, hardwired or optionally.
// Depends on the compiler, but modern versions of gcc support all of these.

#if defined(_MSC_VEC) || (defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__)))

// x86 platform.

#if !defined(DST_FIXED_SIMD) && !defined(DST_NO_SIMD)
#define DST_SSE2_SUPPORT
#define DST_SSE3_SUPPORT
#define DST_SSSE3_SUPPORT
#define DST_SSE4A_SUPPORT
//#define DST_SSE41_SUPPORT
//#define DST_SSE42_SUPPORT
//#define DST_AVX_SUPPORT
//#define DST_AVX_SSE4A_FMA4_SUPPORT
//#define DST_AVX_FMA_SUPPORT
#endif

#ifdef DST_FIXED_SIMD
#ifdef DST_FIXED_SIMD_SSE2
#define DST_FUNC_LOOKUP(f) f ## SSE2
#define DST_FUNC_STREAM_LOOKUP(f) f ## SSE2Stream
#define DST_SSE2_SUPPORT
#elif defined(DST_FIXED_SIMD_SSE3)
#define DST_FUNC_LOOKUP(f) f ## SSE3
#define DST_FUNC_STREAM_LOOKUP(f) f ## SSE3Stream
#define DST_SSE3_SUPPORT
#elif defined(DST_FIXED_SIMD_SSSE3)
#define DST_FUNC_LOOKUP(f) f ## SSSE3
#define DST_FUNC_STREAM_LOOKUP(f) f ## SSSE3Stream
#define DST_SSSE3_SUPPORT
#elif defined(DST_FIXED_SIMD_SSE4A)
#define DST_FUNC_LOOKUP(f) f ## SSE4A
#define DST_FUNC_STREAM_LOOKUP(f) f ## SSE4AStream
#define DST_SSE4A_SUPPORT
#elif defined(DST_FIXED_SIMD_SSE41)
#define DST_FUNC_LOOKUP(f) f ## SSE41
#define DST_FUNC_STREAM_LOOKUP(f) f ## SSE41Stream
#define DST_SSE41_SUPPORT
#elif defined(DST_FIXED_SIMD_SSE42)
#define DST_FUNC_LOOKUP(f) f ## SSE42
#define DST_FUNC_STREAM_LOOKUP(f) f ## SSE42Stream
#define DST_SSE42_SUPPORT
#elif defined(DST_FIXED_SIMD_AVX)
#define DST_FUNC_LOOKUP(f) f ## AVX
#define DST_FUNC_STREAM_LOOKUP(f) f ## AVXStream
#define DST_AVX_SUPPORT
#elif defined(DST_FIXED_SIMD_AVX_SSE4A_FMA4)
#define DST_FUNC_LOOKUP(f) f ## AVX_SSE4A_FMA4
#define DST_FUNC_STREAM_LOOKUP(f) f ## AVX_SSE4A_FMA4Stream
#define DST_AVX_SSE4A_FMA4_SUPPORT
#elif defined(DST_FIXED_SIMD_AVX_FMA)
#define DST_FUNC_LOOKUP(f) f ## AVX_FMA
#define DST_FUNC_STREAM_LOOKUP(f) f ## AVX_FMAStream
#define DST_AVX_FMA_SUPPORT
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
#define DST_FUNC_STREAM_LOOKUP(f) f ## NoSIMD
#define DST_FUNC_LOOKUP_NO_SIMD(f) f ## NoSIMD

#endif

#include "dstMath.h"
#include "dstThread.h"

#endif // !defined(__DST_CONFIG_H__)

