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

// Alignment macros (on some platforms, aligning or packing data structures may improve
// performance.

#ifndef __GNUC__

#define DST_ALIGNED(n)
#define DST_PACKED
#define DST_LINE_SIZE 128

#else

#define DST_ALIGNED(n) __attribute__ ((aligned(n)))
#define DST_PACKED __attribute__ ((packed))
// Very rough guess at a cache line size usable for alignment,
#ifdef OPENGL_ES2
#define DST_LINE_SIZE 64
#else
#define DST_LINE_SIZE 128
#endif

#endif

#endif // !defined(__DST_CONFIG_H__)

