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

#ifndef __DST_DYNAMIC_ARRAY_H__
#define __DST_DYNAMIC_ARRAY_H__

#include <dstConfig.h>
#include <dstRandom.h>

// Dynamic array template class allows any type of data.
// T is the type of the stored data.
// S is the integer type for the size of the array (normally int, allowing for
// 2G (2^31) entries, but for small arrays unsigned short/uint16_t (max 32768 entries)
// or especially unsigned char (max 128 entries) can save a little space, especially when
// lots of very small arrays are expected.

template <class T, class S>
class DST_API dstDynamicArrayBaseClass {
private :
	S nu_elements;
	S max_elements;
	S expansion_hint;
	T *data;

	// By how much to expand the array the next time it is full.
	virtual S GetExpansionHint(S size) = 0;
        inline void ResizeCapacity(S new_capacity) {
                max_elements = new_capacity;
		data = (T *)realloc(data, sizeof(T) * max_elements);
		expansion_hint = GetExpansionHint(max_elements);
	}
	inline void ExpandCapacity() {
		ResizeCapacity(max_elements + expansion_hint);
        }

public :
	dstDynamicArrayBaseClass(S starting_capacity = 4) {
		nu_elements = 0;
		max_elements = 0;
		expansion_hint = starting_capacity;
		data = NULL;
	}
	// There is no deconstuctor. Run FreeData or MakeEmpty() to free storage space.
	inline void FreeData() const {
		free(data);
        }
	inline void MakeEmpty() {
		FreeData();
		nu_elements = 0;
		max_elements = 0;
		expansion_hint = 4;
		data = NULL;	
	}
	inline S Size() const {
		return nu_elements;
	}
	inline S AllocatedSize() const {
		return max_elements;
        }
	// Get value of element i.
	inline T Get(uint32_t i) const {
		return data[i];
	}
	// Get a pointer to the start of the array.
	inline T *DataPointer() const {
                return data;
        }
	// Get a pointer to a specific element.
        inline T *DataPointer(uint32_t i) const {
		return &data[i];
        }
        // Trim capacity to the number of elements. This may result in a unusual
	// (e.g. non-power of two) capacity.
	inline void TrimCapacity() {
		ResizeCapacity(nu_elements);
	}
        // Truncate array to n elements. Capacity remains the same. n must be less or equal
	// to the current number of elements as reported by Size().
	inline void Truncate(S n) {
		nu_elements = n;
        }
	// Add element with value V.
	inline void Add(T v) {
		if (nu_elements == max_elements)
			ExpandCapacity();
		data[nu_elements] = v;
		nu_elements++;
	}
};

// Standard implementation of the class, which doubles the capacity each time
// the array is full.

template <class T, class S>
class DST_API dstDynamicArray : public dstDynamicArrayBaseClass <T, S> {
public :
	dstDynamicArray(S starting_capacity = 4) { }

private :
	// By how much to expand the array the next time it is full.
	inline S GetExpansionHint(S size) {
		// Expand the size to the next power of two.
                uint32_t log2_size = dstCalculateLog2(size);
		return (S)((uint32_t)1 << (log2_size + 1));
	}
};

// Template class to cast DynamicArray class type to another, same-sized type.
// U is the new type, T is the same-sized type for which an existing class exists,
// S is the integer type for the size storage (usually uint32_t), and C2 is the name
// of the existing class.

template <class U, class T, class S, class C2>
class DST_API dstCastDynamicArray : public C2 {
public :
	dstCastDynamicArray(S starting_capacity = 4) { }
	inline U Get(S i) const {
		return (U)((C2 *)this->Get(i));
	}
	inline void Add(U s) {
		((C2 *)this)->Add((T)s);
	}
};

// Dynamic array types for int, int64_t, void * char *, with a size storage size
// of 32 bits (max 2G entries).
typedef dstDynamicArray <int, uint32_t> dstIntArray;
typedef dstDynamicArray <int64_t, uint32_t> dstInt64Array;
#if UINTPTR_MAX == 0xFFFFFFFF
// 32-bit pointers.
typedef dstCastDynamicArray <void *, int, uin32_t, dstIntArray> dstPointerArray;
#else
// 64-bit pointers
typedef dstCastDynamicArray <void *, int64_t, uint32_t, dstInt64Array> dstPointerArray;
#endif
typedef dstCastDynamicArray <char *, void *, uint32_t, dstPointerArray> dstCharPointerArray;

// Dynamic array types for int, int64_t, void * char *, with a size storage size
// of 16 bits (max 32768 entries).
typedef dstDynamicArray <int, uint16_t> dstSmallIntArray;
typedef dstDynamicArray <int64_t, uint16_t> dstSmallInt64Array;
#if UINTPTR_MAX == 0xFFFFFFFF
// 32-bit pointers.
typedef dstCastDynamicArray <void *, int, uint16_t, dstSmallIntArray> dstSmallPointerArray;
#else
// 64-bit pointers
typedef dstCastDynamicArray <void *, int64_t, uint16_t, dstSmallInt64Array> dstSmallPointerArray;
#endif
typedef dstCastDynamicArray <char *, void *, uint16_t, dstSmallPointerArray> dstSmallCharPointerArray;

// Dynamic array types for int, int64_t, void * char *, with a size storage size
// of 8 bits (max 128 entries).
typedef dstDynamicArray <int, uint8_t> dstVerySmallIntArray;
typedef dstDynamicArray <int64_t, uint8_t> dstVerySmallInt64Array;
#if UINTPTR_MAX == 0xFFFFFFFF
// 32-bit pointers.
typedef dstCastlDynamicArray <void *, int, uint8_t, dstVerySmallIntArray> dstVerySmallPointerArray;
#else
// 64-bit pointers
typedef dstCastDynamicArray <void *, int64_t, uint8_t, dstVerySmallInt64Array> dstVerySmallPointerArray;
#endif
typedef dstCastDynamicArray <char *, void *, uint8_t, dstVerySmallPointerArray> dstVerySmallCharPointerArray;

template <class T, class S>
class DST_API dstTightDynamicArray : public dstDynamicArrayBaseClass <T, S> {
public :
	dstTightDynamicArray(S starting_capacity = 4) { }

private :
	// By how much to expand the array the next time it is full.
	inline S GetExpansionHint(S size) {
		// Conservatively expand the size of the array, keeping it tight.
		// A fast, integer log2 function is used.
		S log2_size = dstCalculateLog2(size);
		S expansion;
                if (log2_size >= 3) {
                        // Round down size to power of two and divide by 8.
			expansion = ((uint32_t)1 << log2_size) >> 3;
			if (log2_size >= 8)
				// Starting from 256, limit the expansion.
				expansion >>= ((log2_size - 4) / 4);
			// When size is irregular, adjust the expansion
			// so that it goes back to aligned power-of-two expansions.
			S offset = size & (expansion - 1);
			expansion -= offset;
		}
		else
			expansion = 1;
		// Formula:
		// expansion = size / 8
		// if (size >= 256)
		//	expansion >>= (log2(size) - 4) / 4;
		//
		// Size		Log2	Expand by
		// 1		0	1 (0)
		// 2		1	1 (0)
		// 4		2	1
		// 8		3	1
		// 16		4	2
		// 32   	5	4
		// 64		6	8
		// 128		7	16
		// 256		8	16
		// 512		9	32
		// 1024		10	64
		// 4096		12	128
		// 65536	16 	512
		// 1048576	20	2 ^ (17 - 4) = 8192
		// 256M		28	2 ^ (25 - 11) = 16384
		return (S)expansion;
	}
};

// dstTightDynamicArray types for int, in64_t, void * char *, with a size storage size
// of 32 bits (max 4G entries). The tight array type does not expand as quickly when the array
// storage is full (it does not double in size, limiting address space/memory use).
typedef dstTightDynamicArray <int, uint32_t> dstTightIntArray;
typedef dstTightDynamicArray <int64_t, uint32_t> dstTightInt64Array;
#if UINTPTR_MAX == 0xFFFFFFFF
// 32-bit pointers.
typedef dstCastDynamicArray <void *, int, uint32_t, dstTightIntArray> dstTightPointerArray;
#else
// 64-bit pointers
typedef dstCastDynamicArray <void *, int64_t, uint32_t, dstTightInt64Array> dstTightPointerArray;
#endif
typedef dstCastDynamicArray <char *, void *, uint32_t, dstTightPointerArray>
    dstTightCharPointerArray;

// dstTightSmallDynamicArray types for int, int64_t, void * char *, with a size storage size
// of 16 bits (max 65536 entries).
typedef dstTightDynamicArray <int, uint16_t> dstTightSmallIntArray;
typedef dstTightDynamicArray <int64_t, uint16_t> dstTightSmallInt64Array;
#if UINTPTR_MAX == 0xFFFFFFFF
// 32-bit pointers.
typedef dstCastDynamicArray <void *, int, uint16_t, dstTightSmallIntArray> dstTightSmallPointerArray;
#else
// 64-bit pointers
typedef dstCastDynamicArray <void *, int64_t, uint16_t, dstTightSmallInt64Array> dstTightSmallPointerArray;
#endif
typedef dstCastDynamicArray <char *, void *, uint16_t, dstTightSmallPointerArray>
    dstTightSmallCharPointerArray;

// Tight dynamic array types for int, int64_t, void * char *, with a size storage size
// of 8 bits (max 256 entries).
typedef dstTightDynamicArray <int, uint8_t> dstTightVerySmallIntArray;
typedef dstTightDynamicArray <int64_t, uint8_t> dstTightVerySmallInt64Array;
#if UINTPTR_MAX == 0xFFFFFFFF
// 32-bit pointers.
typedef dstCastDynamicArray <void *, int, uint8_t, dstVerySmallIntArray> dstTightVerySmallPointerArray;
#else
// 64-bit pointers
typedef dstCastDynamicArray <void *, int64_t, uint8_t, dstVerySmallInt64Array> dstTightVerySmallPointerArray;
#endif
typedef dstCastDynamicArray <char *, void *, uint8_t, dstVerySmallPointerArray>
    dstTightVerySmallCharPointerArray;

#endif

