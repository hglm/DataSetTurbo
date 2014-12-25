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

#include <dstThread.h>

static void *dstInternalThreadFunc(void *);

dstTaskScheduler::dstTaskScheduler() {
	pthread_mutex_init(&task_info_array_mutex, NULL);
	pthread_mutex_init(&empty_slot_array_mutex, NULL);
	start_time = dstGetCurrentTimeUSec();
}

int dstTaskScheduler::AddTask(int flags, dstTaskFunc func, const void *_user_data,
dstTaskDurationEstimate e, const dstTaskDivisionData& division) {
	uint32_t start_index, nu_elements;
	if (division.size != 0) {
		// Be fool-proof for small sizes (empty subdivisions), division by an odd number
		// of subdivisions, and element index alignment requirements.
		start_index = division.size * division.index /
			division.nu_subdivisions;
		uint32_t next_start_index = division.size * (division.index + 1) /
			division.nu_subdivisions;
		uint32_t alignment = division.alignment;
		if (alignment != 1) {
			start_index = ((start_index + alignment - 1) / alignment) * alignment;
			next_start_index = ((next_start_index + alignment - 1) / alignment) * alignment;
			next_start_index = mini(next_start_index, division.size);
		}
		if (start_index >= division.size || start_index == next_start_index)
			return - 1;
		nu_elements = next_start_index - start_index;
//		printf("start_index = %d, nu_elements = %d.\n", start_index, nu_elements);
	}
	dstTaskInfo *task_info;
	int array_index;
	LockMutexEmptySlotArray();
	if (empty_slot_array.Size() > 0) {
		int empty_slot_index = empty_slot_array.Size();
		empty_slot_array.Truncate(empty_slot_array.Size() - 1);
		UnlockMutexEmptySlotArray();
		task_info = task_info_array.Get(empty_slot_index);
		array_index = empty_slot_index;
//		printf("Empty slot = %d\n", index);
	}
	else {
		UnlockMutexEmptySlotArray();
		task_info = new dstTaskInfo;
		LockMutexTaskInfoArray();
		array_index = task_info_array.Size();
		task_info_array.Add(task_info);
		UnlockMutexTaskInfoArray();
	}
        task_info->flags = flags;
	task_info->task_func = func;
	task_info->duration_estimate = e;
	task_info->creation_time = dstGetCurrentTimeUSec() - start_time;
	task_info->user_data = _user_data;
	if (division.size != 0) {
		task_info->subdivision.start_index = start_index;
		task_info->subdivision.nu_elements = nu_elements;
	}
	pthread_create(&task_info->thread, NULL, dstInternalThreadFunc, task_info);
	return array_index;
}

void dstTaskScheduler::WaitUntilFinished(int task_index) {
	dstTaskInfo *task_info = task_info_array.Get(task_index);
	LockMutexTaskInfoArray();
	if (task_info->flags & DST_TASK_FLAG_COMPLETED) {
		// If the completed flag is set, the process has finished and was already
		// evaluated by WaitUntilFinished() and the slot was added to the empty
		// slots list.
		UnlockMutexTaskInfoArray();
		return;
	}
	UnlockMutexTaskInfoArray();
	pthread_join(task_info->thread, NULL);
	LockMutexTaskInfoArray();
	task_info->flags |= DST_TASK_FLAG_COMPLETED;
	UnlockMutexTaskInfoArray();
	LockMutexEmptySlotArray();
	empty_slot_array.Add(task_index);
#if 1
	// When there are no remaining processing, clear the arrays.
	LockMutexTaskInfoArray();
	if (empty_slot_array.Size() == task_info_array.Size()) {
		ClearTaskInfoArray();
		empty_slot_array.Truncate(0);
	}
	UnlockMutexTaskInfoArray();
#endif
	UnlockMutexEmptySlotArray();
}

void dstTaskScheduler::ClearTaskInfoArray() {
	uint32_t n = task_info_array.Size();
	for (uint32_t i = 0; i < n; i++)
		delete task_info_array.Get(i);
	task_info_array.Truncate(0);
}

void dstTaskScheduler::WaitUntilFinished() {
	for (uint32_t i = 0; i < task_info_array.Size(); i++)
		WaitUntilFinished(i);
	ClearTaskInfoArray();
	empty_slot_array.Truncate(0);
}

const dstIntArray *dstTaskScheduler::GetCompletionNotifications() {
        completion_notification_array.Truncate(0);
	LockMutexTaskInfoArray();
	for (uint32_t i = 0; i < task_info_array.Size(); i++) {
		dstTaskInfo *task_info = task_info_array.Get(i);
		if ((task_info->flags & (DST_TASK_FLAG_NOTIFY_COMPLETION |
                DST_TASK_FLAG_COMPLETED | DST_TASK_FLAG_COMPLETION_NOTIFIED))
                == (DST_TASK_FLAG_NOTIFY_COMPLETION | DST_TASK_FLAG_COMPLETED)) {
			completion_notification_array.Add(i);
			task_info->flags |= DST_TASK_FLAG_COMPLETION_NOTIFIED;
                }
	}
	UnlockMutexTaskInfoArray();
        return &completion_notification_array;
}

// Thread function.

static void *dstInternalThreadFunc(void *data) {
	dstTaskInfo *task_info = (dstTaskInfo *)data;
	task_info->task_func(task_info);
	return NULL;
}

