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
#include <dstMemory.h>

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

// Dynamic array template class allows any type of data.
// T is the type of the stored data.
// S is the integer type for the size of the array (normally int, allowing for
// 2G (2^31) entries, but for small arrays unsigned short/uint16_t (max 32768 entries)
// or especially unsigned char (max 128 entries) can save a little space, especially when
// lots of very small arrays are expected.

template <class T, class S>
class DST_API dstDynamicArrayBaseClass {
private :
	T *data;
	S nu_elements;
	S max_elements;

public :
	// By how much to expand array capacity. Return value is new capacity.
	virtual S GetExpansionHint(S capacity) const = 0;
	virtual S InternalMaxCapacity() const = 0;

protected :
	// The absolute maximum capacity allowed by the class variant.
        inline void ResizeCapacity(S new_capacity) {
                max_elements = new_capacity;
		data = (T *)realloc(data, sizeof(T) * max_elements);
	}
	inline void ExpandCapacity(S reference_capacity) {
		S new_capacity = GetExpansionHint(reference_capacity);
		if (new_capacity < reference_capacity) {
			// When new_capacity is less than expected, usually zero, overflow
			// has occurred and the maximum capacity has been reached, so the
			// element cannot be added. Exit with an error message.
			fprintf(stderr, "dstDynamicArray: maximum array capacity overflow\n");
			raise(SIGABRT);
		}
		ResizeCapacity(new_capacity);
        }
	// Expand capacity when the allocated space is full. The capacity expansion is by one
	// of more elements (usually a power of two).
	inline void ExpandCapacity() {
		ExpandCapacity(max_elements);
	}
	// Set capacity to a minimum of minimum_capacity.
	inline void SetMinimumCapacity(S minimum_capacity) {
		ExpandCapacity(minimum_capacity - 1);
	}

public :
	inline S MaxCapacity() const {
		return InternalMaxCapacity();
	}
	dstDynamicArrayBaseClass() {
		nu_elements = 0;
		max_elements = 0;
		data = NULL;
	}
	explicit dstDynamicArrayBaseClass(S starting_capacity) {
		nu_elements = 0;
		data = NULL;
		ResizeCapacity(starting_capacity);
	}
        // There is no deconstuctor. Run FreeData() or MakeEmpty() to free storage space.
	inline void FreeData() const {
		free(data);
        }
	inline void MakeEmpty() {
		FreeData();
		nu_elements = 0;
		max_elements = 0;
		data = NULL;	
	}
	inline S Size() const {
		return nu_elements;
	}
	inline S Capacity() const {
		return max_elements;
        }
	// Get value of element i.
	inline T& Get(S i) const {
		return data[i];
	}
        // Set value of element i.
        inline void Set(S i, const T& v) const {
		data[i] = v;
        }
	// Get a pointer to the start of the array.
	inline T *DataPointer() const {
                return data;
        }
	// Get a pointer to a specific element.
        inline T *DataPointer(S i) const {
		return &data[i];
        }
        // Trim capacity to the number of elements. This may result in a unusual
	// (e.g. non-power of two) capacity.
	inline void TrimCapacity() {
		ResizeCapacity(nu_elements);
	}
	// Set the maximum capacity to a specific size.
        inline void SetCapacity(S capacity) {
		ResizeCapacity(capacity);
	}
        // Truncate array to n elements. Capacity remains the same. n must be less or equal
	// to the current number of elements as reported by Size().
	inline void Truncate(S n) {
		nu_elements = n;
        }
	// Add element with value v without a capacity check. Capacity must have room for
	// at least one more element.
	inline void AddQuick(const T& v) {
		data[nu_elements] = v;
		nu_elements++;
	}
	// Add element with value v.
	inline void Add(const T& v) {
		if (nu_elements == max_elements)
			ExpandCapacity();
		AddQuick(v);
	}
private :
	// Add n elements from a buffer. No capacity check.
	inline void AddQuick(const T *v_pointer, S n,  bool memcpy_only, bool no_memcpy) {
		if (memcpy_only || (!no_memcpy && n >= DST_MEMCPY_THRESHOLD))
			dstMemcpyAlignedLarge(&data[nu_elements], &v_pointer[0],
				n * sizeof(data[0]));
		else if (!memcpy_only)
			dstMemcpyAlignedSmall(&data[nu_elements], &v_pointer[0],
				n * sizeof(data[0]));
		nu_elements += n;
	}
public :
	// Add n elements from a buffer. No capacity check. Flexible size.
	inline void AddQuick(const T *v_pointer, S n) {
		AddQuick(v_pointer, n, false, false);
	}
	// Add n elements from a buffer. No capacity check. Small size (no memcpy).
	inline void AddQuickSmall(const T *v_pointer, S n) {
		AddQuick(v_pointer, n, false, true);
	}
	// Add n elements from a buffer. No capacity check. Large size (use memcpy).
	inline void AddQuickLarge(const T *v_pointer, S n) {
		AddQuick(v_pointer, n, true, false);
	}
	// Add n elements from a buffer. Flexible n.
	inline void Add(const T *v_pointer, S n) {
		if (nu_elements + n > max_elements)
			SetMinimumCapacity(nu_elements + n);
		AddQuick(v_pointer, n);
	}
	// Add n elements from a buffer. Small n.
	inline void AddSmall(const T *v_pointer, S n) {
		if (nu_elements + n > max_elements)
			SetMinimumCapacity(nu_elements + n);
		AddQuickSmall(v_pointer, n);
	}
	// Add n elements from a buffer. Large n.
	inline void AddLarge(const T *v_pointer, S n) {
		if (nu_elements + n > max_elements)
			SetMinimumCapacity(nu_elements + n);
		AddQuickLarge(v_pointer, n);
	}
	// Add all elements from another array. No capacity check.
	inline void AddQuick(const dstDynamicArrayBaseClass <T, S> & array) {
		AddQuick(array.DataPointer(), array.Size());
	}
	// Add all elements from another array.
	inline void Add(const dstDynamicArrayBaseClass <T, S> & array) {
		Add(array.DataPointer(), array.Size());
	}
	// Pop element (return value of last element and decrease size by one) when
	// treating array as a stack. The array must contain at least one element.
	inline T Pop() {
		T v = data[nu_elements - 1];
		nu_elements--;
		return v;
	}
	// Pushing an element is equivalent to Add().
	inline void PushQuick(const T& v) {
		AddQuick(v);
	}
	inline void Push(const T& v) {
		Add(v);
	}
private :
	// Remove up to n elements from the head (start) of the array. This involves an
	// overlapping memory copy operation. When n is greater or equal to the number of
	// elements, the array will be empty.
	inline void RemoveHead(S n, bool memcpy_only, bool no_memcpy) {
		int n_leftover = nu_elements - n;
		if (n_leftover < 0) {
			nu_elements = 0;
			return;
		}
		if (memcpy_only || (!no_memcpy && n_leftover >= DST_MEMCPY_THRESHOLD))
			dstMemcpyAlignedLarge(&data[0], &data[n], n_leftover * sizeof(data[0]));
		else if (!memcpy_only)
			dstMemcpyAlignedSmall(&data[0], &data[n], n_leftover * sizeof(data[0]));
		nu_elements = n_leftover;
	}
public:
	// Flexible version for both small and large number of elements to be removed.
	inline void RemoveHead(S n) {
		RemoveHead(n, false, false);
	}
	// Version for small number of elements to be removed (do not use memcpy).
	inline void RemoveSmallHead(S n) {
		RemoveHead(n, false, true);
	}
	// Version for large number of elements to be removed (use memcpy).
	inline void RemoveLargeHead(S n) {
		RemoveHead(n, true, false);
	}
};

// Standard implementation of the class, which doubles the capacity each time
// the array is full. T is the type of the stored data, S the index storage type.

template <class T, class S>
class DST_API dstDynamicArray : public dstDynamicArrayBaseClass <T, S> {
public :
	dstDynamicArray() : dstDynamicArrayBaseClass <T, S>() { }
	explicit dstDynamicArray(S starting_capacity) :
		dstDynamicArrayBaseClass <T, S>(starting_capacity) { }

public :
	// The absolute maximum capacity allowed by the class variant.
	virtual S InternalMaxCapacity() const {
		return (S)1 << (sizeof(S) * 8 - 1);
	}
	// By how much to expand array capacity. Return value is new capacity.
	virtual S GetExpansionHint(S size) const {
		// Expand the size to the next power of two.
                uint32_t log2_size = dstCalculateLog2(size);
		return (S)((S)1 << (log2_size + 1));
	}
};

// Template class to cast DynamicArray class type to another, same-sized type.
// U is the new type, T is the same-sized type for which an existing class exists,
// S is the integer type for the size storage (usually uint32_t), and C2 is the name
// of the existing class.

template <class U, class T, class S, class C2>
class DST_API dstCastDynamicArray : public C2 {
public :
	dstCastDynamicArray() : C2() { }
	explicit dstCastDynamicArray(S starting_capacity) : C2(starting_capacity) { }
	// Get value of element i.
	inline U& Get(S i) const {
		return (U&)C2::Get(i);
	}
        // Set value of element i.
        inline void Set(S i, U v) const {
		C2::Set(i, (T)v);
        }
	// Get a pointer to the start of the array.
	inline U *DataPointer() const {
                return (U *)C2::DataPointer();
        }
	// Get a pointer to a specific element.
        inline U *DataPointer(S i) const {
		return &(U *)C2::DataPointer(i);
        }
	inline void Add(U v) {
		C2::Add((T)v);
	}
	inline void AddQuick(U v) {
		C2::AddQuick((T)v);
	}
	inline void Add(const U *v_pointer, S n) {
		C2::Add((T *)v_pointer, n);
	}
	inline void AddQuick(const U *v_pointer, S n) {
		C2::AddQuick((T *)v_pointer, n);
	}
	inline void Add(const dstDynamicArrayBaseClass <U, S> & array) {
		C2::Add((const dstDynamicArrayBaseClass <T, S> &)array);
	}
	inline void AddQuick(const dstDynamicArrayBaseClass <U, S> & array) {
		C2::AddQuick((const dstDynamicArrayBaseClass <T, S> &)array);
	}
};

// Dynamic array types for int, int64_t, void * char *, with a size storage size
// of 32 bits (max 2G entries).
typedef dstDynamicArray <int, uint32_t> dstIntArray;
typedef dstDynamicArray <int64_t, uint32_t> dstInt64Array;
#if UINTPTR_MAX == 0xFFFFFFFF
// 32-bit pointers.
typedef dstCastDynamicArray <void *, int, uint32_t, dstIntArray> dstPointerArray;
#else
// 64-bit pointers
typedef dstCastDynamicArray <void *, int64_t, uint32_t, dstInt64Array> dstPointerArray;
#endif
typedef dstCastDynamicArray <char *, void *, uint32_t, dstPointerArray> dstCharPointerArray;
typedef dstCastDynamicArray <float, int, uint32_t, dstIntArray> dstFloatArray;
typedef dstCastDynamicArray <double, int64_t, uint32_t, dstInt64Array> dstDoubleArray;
typedef dstCastDynamicArray <uint32_t, int, uint32_t, dstIntArray> dstUnsignedIntArray;

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
typedef dstCastDynamicArray <float, int, uint16_t, dstSmallIntArray> dstSmallFloatArray;
typedef dstCastDynamicArray <double, int64_t, uint16_t, dstSmallInt64Array> dstSmallDoubleArray;
typedef dstCastDynamicArray <uint32_t, int, uint16_t, dstSmallIntArray> dstSmallUnsignedIntArray;

// Dynamic array types for int, int64_t, void * char *, with a size storage size
// of 8 bits (max 128 entries).
typedef dstDynamicArray <int, uint8_t> dstVerySmallIntArray;
typedef dstDynamicArray <int64_t, uint8_t> dstVerySmallInt64Array;
#if UINTPTR_MAX == 0xFFFFFFFF
// 32-bit pointers.
typedef dstCastDynamicArray <void *, int, uint8_t, dstVerySmallIntArray> dstVerySmallPointerArray;
#else
// 64-bit pointers
typedef dstCastDynamicArray <void *, int64_t, uint8_t, dstVerySmallInt64Array> dstVerySmallPointerArray;
#endif
typedef dstCastDynamicArray <char *, void *, uint8_t, dstVerySmallPointerArray> dstVerySmallCharPointerArray;
typedef dstCastDynamicArray <float, int, uint8_t, dstVerySmallIntArray> dstVerySmallFloatArray;
typedef dstCastDynamicArray <double, int64_t, uint8_t, dstVerySmallInt64Array>
    dstVerySmallDoubleArray;
typedef dstCastDynamicArray <uint32_t, int, uint16_t, dstVerySmallIntArray> dstVerySmallUnsignedIntArray;

// Dynamic array types for int, int64_t, void *, char *, float and double with a size
// storage size of 64 bits (allowing arrays with much more than 2G entries). Because the size
// of the array expands by doubling when it is full, this version is less appropriate than the
// "tight" versions (dstTightHugeIntArray etc) which are less wasteful with address space/memory.
typedef dstDynamicArray <int, uint64_t> dstHugeIntArray;
typedef dstDynamicArray <int64_t, uint64_t> dstHugeInt64Array;
#if UINTPTR_MAX == 0xFFFFFFFF
// 32-bit pointers. 64-bit index doesn't really make sense in this case.
typedef dstCastDynamicArray <void *, int, uint64_t, dstHugeIntArray> dstHugePointerArray;
#else
// 64-bit pointers
typedef dstCastDynamicArray <void *, int64_t, uint64_t, dstHugeInt64Array> dstHugePointerArray;
#endif
typedef dstCastDynamicArray <char *, void *, uint64_t, dstHugePointerArray> dstHugeCharPointerArray;
typedef dstCastDynamicArray <float, int, uint64_t, dstHugeIntArray> dstHugeFloatArray;
typedef dstCastDynamicArray <double, int64_t, uint64_t, dstHugeInt64Array> dstHugeDoubleArray;
typedef dstCastDynamicArray <uint32_t, int, uint64_t, dstHugeIntArray> dstHugeUnsignedIntArray;

// Tight array variant, which does not expand by doubling capacity but by smaller steps.

template <class T, class S>
class DST_API dstTightDynamicArray : public dstDynamicArrayBaseClass <T, S> {
public :
	dstTightDynamicArray() : dstDynamicArrayBaseClass <T, S>() { }
	explicit dstTightDynamicArray(S starting_capacity) :
		dstDynamicArrayBaseClass <T, S>(starting_capacity) { }

public :
	// The absolute maximum capacity allowed by the class variant.
	virtual S InternalMaxCapacity() const {
		// Calculate the expansion step size near the end of the size range.
		S largest_power_of_two_size = (S)1 << (sizeof(S) * 8 - 1);
		S next = GetExpansionHint(largest_power_of_two_size);
		S step = next - largest_power_of_two_size;
		// Since the size storage type S is guaranteed to be an unsigned integer type,
		// (- step) will be equal the largest possible size (just before 2^64, 2^32, 2^16,
		// and 2^8.
		return (S)(- step);
	}
	// By how much to expand array capacity. Return value is new capacity.
	virtual S GetExpansionHint(S size) const {
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
		return size + expansion;
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
typedef dstCastDynamicArray <float, int, uint32_t, dstTightIntArray> dstTightFloatArray;
typedef dstCastDynamicArray <double, int64_t, uint32_t, dstTightInt64Array> dstTightDoubleArray;
typedef dstCastDynamicArray <uint32_t, int, uint32_t, dstTightIntArray> dstTightUnsignedIntArray;

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
typedef dstCastDynamicArray <float, int, uint16_t, dstTightSmallIntArray> dstTightSmallFloatArray;
typedef dstCastDynamicArray <double, int64_t, uint16_t, dstTightSmallInt64Array>
	dstTightSmallDoubleArray;
typedef dstCastDynamicArray <uint32_t, int, uint16_t, dstTightSmallIntArray>
	dstTightSmallUnsignedIntArray;

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
typedef dstCastDynamicArray <float, int, uint8_t, dstTightVerySmallIntArray>
    dstTightVerySmallFloatArray;
typedef dstCastDynamicArray <double, int64_t, uint8_t, dstTightVerySmallInt64Array>
    dstTightVerySmallDoubleArray;
typedef dstCastDynamicArray <uint32_t, int, uint8_t, dstTightVerySmallIntArray>
	dstTightVerySmallUnsignedIntArray;

// Dynamic array types for int, int64_t, void *, char *, float and double with a size
// storage size of 64 bits (allowing arrays with much more than 2G entries).
typedef dstTightDynamicArray <int, uint64_t> dstTightHugeIntArray;
typedef dstTightDynamicArray <int64_t, uint64_t> dstTightHugeInt64Array;
#if UINTPTR_MAX == 0xFFFFFFFF
// 32-bit pointers. 64-bit index doesn't really make sense in this case.
typedef dstCastDynamicArray <void *, int, uint64_t, dstTightHugeIntArray> dstTightHugePointerArray;
#else
// 64-bit pointers
typedef dstCastDynamicArray <void *, int64_t, uint64_t, dstTightHugeInt64Array>
    dstTightHugePointerArray;
#endif
typedef dstCastDynamicArray <char *, void *, uint64_t, dstTightHugePointerArray>
    dstTightHugeCharPointerArray;
typedef dstCastDynamicArray <float, int, uint64_t, dstTightHugeIntArray> dstTightHugeFloatArray;
typedef dstCastDynamicArray <double, int64_t, uint64_t, dstTightHugeInt64Array>
    dstTightHugeDoubleArray;
typedef dstCastDynamicArray <uint32_t, int, uint64_t, dstTightHugeIntArray>
	dstTightHugeUnsignedIntArray;

#endif

