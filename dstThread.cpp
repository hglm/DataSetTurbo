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

dstTaskScheduler::~dstTaskScheduler() {
	ClearTasks();
	pthread_mutex_destroy(&task_info_array_mutex);
	pthread_mutex_destroy(&empty_slot_array_mutex);
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
	}
	dstTaskInfo *task_info;
	int array_index;
	LockMutexEmptySlotArray();
	bool new_thread;
	if (empty_slot_array.Size() > 0) {
		// Use an existing slot for which a thread already exists.
		int empty_slot_index = empty_slot_array.Pop();
		UnlockMutexEmptySlotArray();
		LockMutexTaskInfoArray();
		task_info = task_info_array.Get(empty_slot_index);
		array_index = empty_slot_index;
		new_thread = false;
//		printf("Empty slot = %d\n", index);
	}
	else {
		UnlockMutexEmptySlotArray();
		task_info = new dstTaskInfo;
		LockMutexTaskInfoArray();
		array_index = task_info_array.Size();
		task_info_array.Add(task_info);
		// Create mutex and condition for this thread.
		pthread_mutex_init(&task_info->continue_mutex, NULL);
		pthread_cond_init(&task_info->continue_condition, NULL);
		task_info->continue_signal = DST_TASK_THREAD_WAIT;
		pthread_mutex_init(&task_info->complete_mutex, NULL);
		pthread_cond_init(&task_info->complete_condition, NULL);
		new_thread = true;
	}
        task_info->flags = flags;
	task_info->task_func = func;
	task_info->duration_estimate = e;
	task_info->creation_time = dstGetCurrentTimeUSec() - start_time;
	task_info->user_data = _user_data;
	task_info->complete_signal = DST_TASK_THREAD_WAIT;
	if (division.size != 0) {
		task_info->subdivision.start_index = start_index;
		task_info->subdivision.nu_elements = nu_elements;
//		printf("AddTask: slot = %d, start_index = %d, nu_elements = %d.\n",
//			array_index, start_index, nu_elements);
//		fflush(stdout);
	}
	UnlockMutexTaskInfoArray();
	if (new_thread) {
		// Create the thread.
		pthread_create(&task_info->thread, NULL, dstInternalThreadFunc, task_info);
	}
	else {
		// Signal the thread to continue and start working on the new
		// task.
		task_info->continue_signal = DST_TASK_THREAD_CONTINUE;
		task_info->LockMutexContinue();
		pthread_cond_signal(&task_info->continue_condition);
		task_info->UnlockMutexContinue();
	}
	return array_index;
}

void dstTaskScheduler::WaitUntilFinished(int task_index) {
	LockMutexTaskInfoArray();
	dstTaskInfo *task_info = task_info_array.Get(task_index);
	if (task_info->flags & DST_TASK_FLAG_COMPLETED) {
		// If the completed flag is set, the task has finished and was already
		// evaluated by WaitUntilFinished() and the slot was added to the empty
		// slots list.
		UnlockMutexTaskInfoArray();
		return;
	}
	UnlockMutexTaskInfoArray();

	// Wait for completion.
	pthread_mutex_lock(&task_info->complete_mutex);
	int r = 0;
	while (task_info->complete_signal == DST_TASK_THREAD_WAIT && r == 0) {
		r = pthread_cond_wait(&task_info->complete_condition,
			&task_info->complete_mutex);
	}
	if (r != 0) {
		printf("pthread_cond_wait returned error.\n");
		exit(1);
	}
	pthread_mutex_unlock(&task_info->complete_mutex);

	LockMutexTaskInfoArray();
	task_info->flags |= DST_TASK_FLAG_COMPLETED;
	UnlockMutexTaskInfoArray();
	LockMutexEmptySlotArray();
	empty_slot_array.Add(task_index);
	UnlockMutexEmptySlotArray();
}

void dstTaskScheduler::WaitUntilFinished() {
	for (uint32_t i = 0; i < task_info_array.Size(); i++)
		WaitUntilFinished(i);
//	ClearTasks();
}

void dstTaskScheduler::ClearTasks() {
	// Clear task info array, stopping each thread, assuming all tasks have finished
	// already.
	uint32_t n = task_info_array.Size();
	for (uint32_t i = 0; i < n; i++) {
		dstTaskInfo *task_info = task_info_array.Get(i);
		task_info->continue_signal = DST_TASK_THREAD_EXIT;
		pthread_cond_signal(&task_info->continue_condition);
		pthread_join(task_info->thread, NULL);
		pthread_cond_destroy(&task_info->continue_condition);
		pthread_mutex_destroy(&task_info->continue_mutex);
		pthread_cond_destroy(&task_info->complete_condition);
		pthread_mutex_destroy(&task_info->complete_mutex);
		delete task_info;
	}
	task_info_array.Truncate(0);
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
	bool exit_signalled = false;
	while (!exit_signalled) {
		// Run the task.
		task_info->task_func(task_info);
		// To make sure that WaitUntilFinished is waiting for the complete signal when it is
		// signalled (the complete signal is not issued too early), wait for the waiting signal.
		task_info->complete_signal = DST_TASK_THREAD_CONTINUE;
		pthread_mutex_lock(&task_info->complete_mutex);
		pthread_cond_signal(&task_info->complete_condition);
		pthread_mutex_unlock(&task_info->complete_mutex);

		// Wait for the continue signal.
		task_info->LockMutexContinue();
		int r = 0;
		while (task_info->continue_signal == DST_TASK_THREAD_WAIT && r == 0)
			r = pthread_cond_wait(&task_info->continue_condition,
				&task_info->continue_mutex);
		if (r != 0) {
			printf("pthread_cond_wait returned error.\n");
			exit(1);
		}
		if (task_info->continue_signal == DST_TASK_THREAD_CONTINUE)
			// Will continue with new task.
			// Reset signal code.
			task_info->continue_signal = DST_TASK_THREAD_WAIT;
		else if (task_info->continue_signal == DST_TASK_THREAD_EXIT)
			exit_signalled = true;
		task_info->UnlockMutexContinue();
	}
	return NULL;
}

