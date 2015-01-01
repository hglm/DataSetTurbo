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

#ifndef __DST_QUEUE_H__
#define __DST_QUEUE_H__

#include <dstDynamicArray.h>

template <class T, class S>
class DST_API dstQueueBaseClass : public dstDynamicArray <T, S> {
protected :
	S head_index;
public :
	inline dstQueueBaseClass() : dstDynamicArray <T, S>() {
		head_index = 0;
	}
	inline dstQueueBaseClass(S starting_capacity) : dstDynamicArray <T, S>(starting_capacity) {
		head_index = 0;
	}
	inline S MaxCapacity() const {
		return ((dstDynamicArray <T, S> *)this)->MaxCapacity() / 2;
	}
	// Return lower limit on queue capacity.
	inline S Capacity() const {
		return ((dstDynamicArray <T, S> *)this)->Capacity() / 2;
	}
	inline S Size() const {
		return ((dstDynamicArray <T, S> *)this)->Size() - head_index;
	}
	inline bool IsEmpty() const {
		return Size() == 0;
	}
	inline T Peek() const {
		return this->Get(head_index);
	}
	inline void SetFirst(const T& v) {
		this->Set(head_index, v);
	}
	inline T Dequeue() {
		T v = this->Get(head_index);
		head_index++;
		return v;
	}
	inline T RemoveFirst() {
		head_index++;
	}
	inline void EnqueueQuick(const T& v) {
		this->Add(v);
	}
	virtual inline void Enqueue(const T& v) = 0;
};

// Standard dstQueue has capacity up to 2^31.

template <class T>
class DST_API dstQueue : public dstQueueBaseClass <T, uint32_t> {
public :
	inline dstQueue() : dstQueueBaseClass <T, uint32_t>() { }
	inline dstQueue(uint32_t starting_capacity) :
		dstQueueBaseClass <T, uint32_t>(starting_capacity) { }
	inline void Enqueue(const T& v) {
		this->Add(v);
		uint32_t queue_size = this->Size();
		// When the array space before the head of the queue is as large as
		// the queue itself, move the queue to the front of the array.
		if (this->head_index >= 8 && this->head_index >= queue_size) {
			this->RemoveHead(this->head_index);
			this->head_index = 0;
		}
	}
	inline void Enqueue(const T *v_pointer, uint8_t n) {
		this->Add(v_pointer, n);
	}
};

typedef dstQueue <int> dstIntQueue;
typedef dstQueue <int64_t> dstInt64Queue;
typedef dstQueue <float> dstFloatQueue;
typedef dstQueue <double> dstDoubleQueue;
typedef dstQueue <uint32_t> dstUnsignedIntQueue;
typedef dstQueue <void *> dstPointerQueue;

// Small and fast dstSmallQueue has capacity up to 64 elements (the small array has a capacity
// of 128 elements, and allowing for the array to be just less than half empty.

template <class T>
class DST_API dstSmallQueue : public dstQueueBaseClass <T, uint8_t> {
public :
	inline dstSmallQueue() : dstQueueBaseClass <T, uint8_t>() { }
	inline dstSmallQueue(uint8_t starting_capacity) :
		dstQueueBaseClass <T, uint8_t>(starting_capacity) { }
	inline void Enqueue(const T& v) {
		this->Add(v);
		uint8_t queue_size = this->Size();
		// When the array space before the head of the queue is as large as
		// the queue itself, move the queue to the front of the array.
		if (this->head_index >= 8 && this->head_index >= queue_size) {
			this->RemoveSmallHead(this->head_index);
			this->head_index = 0;
		}
	}
	inline void Enqueue(const T *v_pointer, uint8_t n) {
		this->AddSmall(v_pointer, n);
	}
};

typedef dstSmallQueue <int> dstSmallIntQueue;
typedef dstSmallQueue <int64_t> dstSmallInt64Queue;
typedef dstSmallQueue <float> dstSmallFloatQueue;
typedef dstSmallQueue <double> dstSmallDoubleQueue;
typedef dstSmallQueue <uint32_t> dstSmallUnsignedIntQueue;
typedef dstSmallQueue <void *> dstSmallPointerQueue;

#endif

