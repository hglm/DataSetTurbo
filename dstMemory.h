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
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


template <class T>
static inline T *dstNewAligned(size_t n, size_t alignment) {
   T *buffer;
   posix_memalign((void **)&buffer, alignment, n * sizeof(T));
   return buffer;
}

template <class T>
static inline T *dstNewAlignedHugeTLB(size_t n) {
	T *buffer;
//	int fd = open("/dev/mem", O_RDWR);
	buffer = (T *)mmap(NULL, n * sizeof(T), PROT_READ | PROT_WRITE,
		MAP_ANONYMOUS | MAP_PRIVATE /* | MAP_HUGETLB */, 0, 0);
	if (buffer == MAP_FAILED) {
		printf("mmap failed.\n");
		exit(1);
	}
//	close(fd);
	return buffer;
}

#ifdef DST_PREFER_LIBC_MEMCPY

template <class T>
static inline void dstMemcpyAlignedSmall(T *dest, const T *src, uint32_t n) {
	memcpy(dest, src, n);
}

template <class T>
static inline void dstMemcpyUnalignedSmall(T *dest, const T *src, uint32_t n) {
	memcpy(dest, src, n);
}

template <class T>
static inline void dstMemcpyAlignedLarge(T *dest, const T *src, size_t n) {
	memcpy(dest, src, n);
}

template <class T>
static inline void dstMemcpyUnalignedLarge(T *dest, const T *src, size_t n) {
	memcpy(dest, src, n);
}

template <class T>
static inline void dstMemcpyTypeAlignedSmall(T *dest, const T *src, uint32_t n) {
	memcpy(dest, src, n);
}

template <class T>
static inline void dstMemcpyTypeAlignedLarge(T *dest, const T *src, size_t n) {
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
// Other values		  Destination and source are aligned on a 32-bit boundary, while n is
//                        any number of bytes (but a multiple of T).

template <class T>
static inline void dstMemcpyAlignedSmall(T *dest, const T *src, uint32_t n) {
	if (sizeof(T) == 8 || (sizeof(T) > 8 && (sizeof(T) & 7) == 0)) {
		n >>= 2;
		uint32_t n_even = n & (~0x1);
		uint32_t *srcp = (uint32_t *)src;
		uint32_t *destp = (uint32_t *)dest;
		uint32_t i;
		for (i = 0; i < n_even; i += 2)
			*(uint64_t *)(destp + i) =
				*(uint64_t *)(srcp + i);
		if (n & 0x1)
			destp[i] = srcp[i];
	}
	else if (sizeof(T) == 4 || (sizeof(T) > 8 && (sizeof(T) & 3) == 0)) {
		n >>= 2;
		uint32_t *srcp = (uint32_t *)&src[0];
		uint32_t *destp = (uint32_t *)&dest[0];	
		for (uint32_t i = 0; i < n; i++)
			destp[i] = srcp[i];
	}
	else {
		// sizeof(T) <= 2 or any other value (T is a struct/class that is not a multiple
		// of four or eight bytes in size).
		uint8_t *srcp = (uint8_t *)src;
		uint8_t *destp = (uint8_t *)dest;
		uint32_t i = 0;
		// src and dest are guaranteed to be aligned on a 32-bit boundary.
		for (; i + 3 < n; i += 4)
			*(uint32_t *)(destp + i) = *(uint32_t *)(srcp + i);
		for (; i < n; i++)
			destp[i] = srcp[i];
	}
}

template <class T>
static inline void dstMemcpyUnalignedSmall(T *dest, const T *src, uint32_t n) {
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
			*(uint32_t *)((uint8_t *)dest + i) =
				*(uint32_t *)((uint8_t *)src + i);
	for (; i < n; i++)
		*((uint8_t *)dest + i) = *((uint8_t *)src + i);
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
static inline void dstMemcpyAlignedLarge(T *dest, const T *src, size_t n) {
	if (sizeof(T) == 8 || (sizeof(T) > 8 && (sizeof(T) & 7) == 0)) {
		uint32_t i = 0;
		uint32_t remainder = n & 0x4;
		n >>= 3;
		uint64_t *srcp = (uint64_t *)&src[0];
		uint64_t *destp = (uint64_t *)&dest[0];
		for (; i + 7 < n; i+= 8) {
			destp[i] = srcp[i];
			destp[i + 1] = srcp[i + 1];
			destp[i + 2] = srcp[i + 2];
			destp[i + 3] = srcp[i + 3];
			destp[i + 4] = srcp[i + 4];
			destp[i + 5] = srcp[i + 5];
			destp[i + 6] = srcp[i + 6];
			destp[i + 7] = srcp[i + 7];
		}
		for (; i < n; i++)
			destp[i] = srcp[i];
		if (remainder != 0)
			*(uint32_t *)&destp[i] = *(uint32_t *)&srcp[i];
	}
	else if (sizeof(T) == 4 || (sizeof(T) > 8 && (sizeof(T) & 3) == 0)) {
		size_t i = 0;
		n >>= 2;
		uint32_t *srcp = (uint32_t *)&src[0];
		uint32_t *destp = (uint32_t *)&dest[0];
		for (; i + 7 < n; i += 8) {
			destp[i] = srcp[i];
			destp[i + 1] = srcp[i + 1];
			destp[i + 2] = srcp[i + 2];
			destp[i + 3] = srcp[i + 3];
			destp[i + 4] = srcp[i + 4];
			destp[i + 5] = srcp[i + 5];
			destp[i + 6] = srcp[i + 6];
			destp[i + 7] = srcp[i + 7];
		}
		for (; i < n; i++)
			destp[i] = srcp[i];
	}
	else
		memcpy(dest, src, n);
}

template <class T>
static inline void dstMemcpyUnalignedLarge(T *dest, const T *src, uint32_t n) {
	// Trust memcpy for this one.
	memcpy(dest, src, n);
}

// The source and destination pointers are aligned to the type (T) boundary.

template <class T>
static inline void dstMemcpyTypeAlignedSmall(T *dest, const T *src, uint32_t n) {
	if (sizeof(T) <= 2)
		dstMemcpyUnalignedSmall(dest, src, n);
	else
		dstMemcpyAlignedSmall(dest, src, n);
}

template <class T>
static inline void dstMemcpyTypeAlignedLarge(T *dest, const T *src, uint32_t n) {
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

