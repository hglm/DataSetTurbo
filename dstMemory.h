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

#ifndef __DST_MEMORY_H__
#define __DST_MEMORY_H__

#include <stdlib.h>
#include <string.h>

#include "dstSIMD.h"

template <class T>
static inline T *dstNewAligned(size_t n, size_t alignment) {
   T *buffer;
   posix_memalign((void **)&buffer, alignment, n * sizeof(T));
   return buffer;
}

#ifdef DST_PREFER_LIBC_MEMCPY

template <class T>
static inline void dstMemcpyAlignedSmall(T *dest, T *src, uint32_t n) {
	memcpy(dest, src, n);
}

template <class T>
static inline void dstMemcpyUnalignedSmall(T *dest, T *src, uint32_t n) {
	memcpy(dest, src, n);
}

template <class T>
static inline void dstMemcpyAlignedLarge(T *dest, T *src, size_t n) {
	memcpy(dest, src, n);
}

template <class T>
static inline void dstMemcpyUnalignedLarge(T *dest, T *src, size_t n) {
	memcpy(dest, src, n);
}

template <class T>
static inline void dstMemcpyTypeAlignedSmall(T *dest, T *src, uint32_t n) {
	memcpy(dest, src, n);
}

template <class T>
static inline void dstMemcpyTypeAlignedLarge(T *dest, T *src, size_t n) {
	memcpy(dest, src, n);
}

#else

// Aligned memcpy for small sizes, not overlapping in a problematic way (e.g. copy forward).
//
// Supported variants:
// sizeof(T)
// 4 (int32_t, uint32_t,  Destination and source are aligned on a 32-bit boundary, while n is
//   pointer (32-bit))    a multiple of four bytes.
// 8 (int64_t, uint64_t,  Destination and source are aligned on a 64-bit boundary, while n is
//   pointer (64-bit))    a multiple of four bytes.
// 1 (uint8_t, char)      Destination and source are aligned on a 32-bit boundary, while n is
//                        any number of bytes.
// 2 (uint16_t, short)    Destination and source are aligned on a 32-bit boundary, while n is
//                        any number of bytes.

template <class T>
static inline void dstMemcpyAlignedSmall(T *dest, T *src, uint32_t n) {
	if (sizeof(T) <= 2) {
		uint32_t i = 0;
		// src and dest are guaranteed to be aligned on a 32-bit boundary.
		for (; i + 3 < n; i += 4)
			*(uint32_t *)(&dest[0] + i) = *(uint32_t *)(&src[0] + i);
		for (; i < n; i++)
			*((uint8_t *)&dest[0] + i) = *((uint8_t *)src[0] + i);
	}
	else if (sizeof(T) == 4) {
		n >>= 2;	
		for (uint32_t i = 0; i < n; i++)
			dest[i] = src[i];
	}
	else if (sizeof(T) == 8) {
		n >>= 2;
		uint32_t n_even = n & (~0x1);
		for (uint32_t i = 0; i < n_even; i += 2)
			*(uint64_t *)((uint32_t *)&dest[0] + i) =
				*(uint64_t *)((uint32_t *)&src[0] + i);
		if (n & 0x1)
			*dest[n - 1] = src[n - 1];
	}

}

template <class T>
static inline void dstMemcpyUnalignedSmall(T *dest, T *src, uint32_t n) {
	uint32_t i = 0;
	// No distinction is made between sizeof(T) of 1, 2, 4 or 8; treat the copy as bytes
	// but use 32-bit word access when possible.
	// This may result in unaligned 32-bit memory access, but virtually all modern
	// CPUs can handle this, and is faster for small sizes. However, allow special
	// handling when access needs to be aligned.
#ifndef DST_UNALIGNED_MEMORY_ACCESS
	if ((((uintptr_t)src | (uintptr_t)dest) & 0x3) == 0)
#endif
		for (; i + 3 < n; i += 4)
			*(uint32_t *)((uint8_t *)&dest[0] + i) =
				*(uint32_t *)((uint8_t *)&src[0] + i);
	for (; i < n; i++)
		*((uint8_t *)&dest + i) = *((uint8_t *)&src + i);
}

// Aligned memcpy for larger sizes.
//
// Supported variants:
// sizeof(T)
// 4 (int32_t, uint32_t,  Destination and source are aligned on a 32-bit boundary, while n is
//   pointer (32-bit))    a multiple of four bytes.
// 8 (int64_t, uint64_t,  Destination and source are aligned on a 64-bit boundary, while n is
//   pointer (64-bit))    a multiple of four bytes.
// 1 (uint8_t, char)      Destination and source are aligned on a 32-bit boundary, while n is
//                        any number of bytes.
// 2 (uint16_t, short)    Destination and source are aligned on a 32-bit boundary, while n is
//                        any number of bytes.

template <class T>
static inline void dstMemcpyAlignedLarge(T *dest, T *src, size_t n) {
	if (sizeof(T) == 4) {
		size_t i = 0;
		n >>= 2;
		for (; i + 7 < n; i += 8) {
			dest[i] = src[i];
			dest[i + 1] = src[i + 1];
			dest[i + 2] = src[i + 2];
			dest[i + 3] = src[i + 3];
			dest[i + 4] = src[i + 4];
			dest[i + 5] = src[i + 5];
			dest[i + 6] = src[i + 6];
			dest[i + 7] = src[i + 7];
		}
		for (; i < n; i++)
			dest[i] = src[i];
	}
	else if (sizeof(T) == 8) {
		uint32_t i = 0;
		uint32_t remainder = n & 0x4;
		n >>= 3;
		for (; i + 7 < n; i+= 8) {
			dest[i] = src[i];
			dest[i + 1] = src[i + 1];
			dest[i + 2] = src[i + 2];
			dest[i + 3] = src[i + 3];
			dest[i + 4] = src[i + 4];
			dest[i + 5] = src[i + 5];
			dest[i + 6] = src[i + 6];
			dest[i + 7] = src[i + 7];
		}
		for (; i < n; i++)
			dest[i] = src[i];
		if (remainder != 0)
			*(uint32_t *)&dest[i] = *(uint32_t *)&src[i];
	}
	else
		memcpy(dest, src, n);
}

template <class T>
static inline void dstMemcpyUnalignedLarge(T *dest, T *src, uint32_t n) {
	// Trust memcpy for this one.
	memcpy(dest, src, n);
}

// The source and destination pointers are aligned to the type (T) boundary.

template <class T>
static inline void dstMemcpyTypeAlignedSmall(T *dest, T *src, uint32_t n) {
	if (sizeof(T) <= 2)
		dstMemcpyUnalignedSmall(dest, src, n);
	else
		dstMemcpyAlignedSmall(dest, src, n);
}

template <class T>
static inline void dstMemcpyTypeAlignedLarge(T *dest, T *src, uint32_t n) {
	if (sizeof(T) <= 2)
		memcpy(dest, src, n);
	else
		dstMemcpyAlignedLarge(dest, src, n);
}

#endif

#if __ARM_ARCH__ >= 6

static inline void dstMemset(void *p, int c, size_t size) {
#ifdef DST_USE_ARM_NEON
	dstMemsetARMv7NEON(p, c, size);
#else
	dstMemsetARMv6(p, c, s);
#endif
}

#else

static inline void dstMemset(void *p, int c, size_t size) {
	memset(p, c, size);
}

#endif

#endif // defined(DST_MEMORY_H)

