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

int dstTaskScheduler::FindEmptySlot() {
	LockMutex();
	for (int i = 0; task_info_array.Size(); i++) {
		dstTaskInfo *task_info = task_info_array.Get(i);
		if (task_info->flags & DST_TASK_FLAG_SLOT_AVAILABLE)
			return i;
	}
	UnlockMutex();
	return - 1;
}

dstTaskScheduler::dstTaskScheduler() {
	pthread_mutex_init(&task_info_mutex, NULL);
}

int dstTaskScheduler::AddTask(int flags, dstTaskFunc func, dstTaskDurationEstimate e) {
	dstTaskInfo *task_info;
	int empty_slot_index = FindEmptySlot();
        int index;
	if (empty_slot_index >= 0) {
		task_info = task_info_array.Get(empty_slot_index);
		index = empty_slot_index;
	}
	else {
		task_info = new dstTaskInfo;
		task_info_array.Add(task_info);
		index = task_info_array.Size();
	}
        task_info->flags = flags;
	task_info->task_func = func;
	task_info->duration_estimate = e;
	task_info->creation_time = dstGetCurrentTimeUSec();
        task_info->mutex = &task_info_mutex;
	pthread_create(&task_info->thread, NULL, dstInternalThreadFunc, task_info);
	return index;
}

void dstTaskScheduler::WaitUntilFinished(int task_index) {
	dstTaskInfo *task_info = task_info_array.Get(task_index);
	pthread_join(task_info->thread, NULL);
}

void dstTaskScheduler::WaitUntilFinished() {
	for (int i = 0; task_info_array.Size(); i++) {
		dstTaskInfo *task_info = task_info_array.Get(i);
		LockMutex();
		if (!(task_info->flags & DST_TASK_FLAG_COMPLETED)) {
			UnlockMutex();
			WaitUntilFinished(i);
		}
		else
			UnlockMutex();
	}
}

const dstIntArray *dstTaskScheduler::GetCompletionNotifications() {
        completion_notification_array.Truncate(0);
	LockMutex();
	for (int i = 0; i < task_info_array.Size(); i++) {
		dstTaskInfo *task_info = task_info_array.Get(i);
		if ((task_info->flags & (DST_TASK_FLAG_NOTIFY_COMPLETION |
                DST_TASK_FLAG_COMPLETED | DST_TASK_FLAG_SLOT_AVAILABLE))
                == (DST_TASK_FLAG_NOTIFY_COMPLETION | DST_TASK_FLAG_COMPLETED)) {
			completion_notification_array.Add(i);
			task_info->flags |= DST_TASK_FLAG_SLOT_AVAILABLE;
                }
	}
	UnlockMutex();
        return &completion_notification_array;
}

// Thread function.

static void *dstInternalThreadFunc(void *data) {
	dstTaskInfo *task_info = (dstTaskInfo *)data;
	task_info->task_func(task_info);
        pthread_mutex_lock(task_info->mutex);
	int flags = task_info->flags;
        flags |= DST_TASK_FLAG_COMPLETED;
        if (!(flags & DST_TASK_FLAG_NOTIFY_COMPLETION))
            flags |= DST_TASK_FLAG_SLOT_AVAILABLE;
        task_info->flags = flags;
        pthread_mutex_unlock(task_info->mutex);
	return NULL;
}

