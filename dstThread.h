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

#ifndef __DST_THREAD_H__
#define __DST_THREAD_H__

// Thread scheduler class. After a dstTaskScheduler is created, threads
// are created as tasks are added. When a task is finished, the thread
// is kept active, waiting for the signal to start with the next task.
// The number of created threads is always equal to the maximum number
// of tasks that has been concurrently active since the creation of
// the dstTaskScheduler. When a dstTaskScheduler is destroyed, the threads
// will exit.


#include <pthread.h>

#include <dstDynamicArray.h>
#include <dstTimer.h>

enum {
    DST_TASK_DURATION_UNIT_NONE = 0,
    DST_TASK_DURATION_UNIT_USEC
};

enum {
    // Specified flags.
    DST_TASK_FLAG_PRIORITY_HIGH = 0x1,
    DST_TASK_FLAG_DURATION_ESTIMATE = 0x2,
    DST_TASK_FLAG_NOTIFY_COMPLETION = 0x4,
    // Flags that are set after completion.
    DST_TASK_FLAG_COMPLETED = 0x10000,
    DST_TASK_FLAG_COMPLETION_NOTIFIED = 0x20000
};

class DST_API dstTaskDurationEstimate {
public :
	int unit_type;
	uint32_t duration;

	dstTaskDurationEstimate() { }
	dstTaskDurationEstimate(int u, uint32_t d) {
		unit_type = u;
		duration = d;
	}
};

class dstTaskDivisionData {
public:
	uint32_t size;
	uint32_t nu_subdivisions;
	uint32_t alignment;
	uint32_t index;
};

class dstTaskSubdivisionData {
public :
	uint32_t start_index;
	uint32_t nu_elements;
};

class DST_API dstTaskInfo;

typedef void (*dstTaskFunc)(dstTaskInfo *task_info);

enum dstTaskThreadSignal {
	DST_TASK_THREAD_WAIT,
	DST_TASK_THREAD_CONTINUE,
	DST_TASK_THREAD_EXIT
};

class DST_API dstTaskInfo {
public :
	pthread_t thread;
	int flags;
	dstTaskFunc task_func;
	const void *user_data;
	uint64_t creation_time;
	dstTaskDurationEstimate duration_estimate;
	dstTaskSubdivisionData subdivision;
	pthread_mutex_t continue_mutex;
	pthread_cond_t continue_condition;
	dstTaskThreadSignal continue_signal;
	pthread_mutex_t complete_mutex;
	pthread_cond_t complete_condition;
	dstTaskThreadSignal complete_signal;

	inline void LockMutexContinue() {
		pthread_mutex_lock(&continue_mutex);
	}
	inline void UnlockMutexContinue() {
		pthread_mutex_unlock(&continue_mutex);
	}
};

typedef dstCastDynamicArray <dstTaskInfo *, void *, uint32_t, dstPointerArray>
    dstTaskInfoPointerArray;

class DST_API dstTaskScheduler {
private :
	dstTaskInfoPointerArray task_info_array;
	dstIntArray empty_slot_array;
        dstIntArray completion_notification_array;
	uint64_t start_time;
        pthread_mutex_t task_info_array_mutex;
	pthread_mutex_t empty_slot_array_mutex;

private :
	inline void LockMutexTaskInfoArray() {
		pthread_mutex_lock(&task_info_array_mutex);
	}
	inline void UnlockMutexTaskInfoArray() {
		pthread_mutex_unlock(&task_info_array_mutex);
	}
	inline void LockMutexEmptySlotArray() {
		pthread_mutex_lock(&empty_slot_array_mutex);
	}
	inline void UnlockMutexEmptySlotArray() {
		pthread_mutex_unlock(&empty_slot_array_mutex);
	}
	void ClearTasks();

public :
	dstTaskScheduler();
	~dstTaskScheduler();
	int AddTask(int flags, dstTaskFunc func, const void *user_data, dstTaskDurationEstimate e,
		const dstTaskDivisionData& division);
	inline int AddTask(int flags, dstTaskFunc func, const void *user_data) {
		dstTaskDivisionData division;
		division.size = 0;
		return AddTask(flags, func, user_data,
			dstTaskDurationEstimate(DST_TASK_DURATION_UNIT_NONE, 0), division);
	}
	inline int AddTask(int flags, dstTaskFunc func, const void *user_data, dstTaskDurationEstimate e) {
		dstTaskDivisionData division;
		division.size = 0;
		return AddTask(flags, func, user_data, e, division);
	}
	inline int AddTask(int flags, dstTaskFunc func, const void *user_data,
	const dstTaskDivisionData& division) {
		return AddTask(flags, func, user_data,
			dstTaskDurationEstimate(DST_TASK_DURATION_UNIT_NONE, 0), division);
	}
	inline void AddSubdividedTasks(int flags, dstTaskFunc func, const void *user_data,
	dstTaskDivisionData& division) {
		for (uint32_t i = 0; i < division.nu_subdivisions; i++) {
			division.index = i;
			AddTask(flags, func, user_data, division);
		}
	}
	void WaitUntilFinished(int task_index);
	void WaitUntilFinished();
	const dstIntArray *GetCompletionNotifications();
};

#endif

