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

#include <sched.h>

#include <dstThread.h>

static void *dstInternalThreadFunc(void *);
static void *dstInternalGroupThreadFunc(void *);

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

static int CalculateSubdivision(const dstTaskDivisionData& division, uint32_t &start_index,
uint32_t &nu_elements) {
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
	if (start_index >= division.size || start_index == next_start_index) {
		nu_elements = 0;
		return - 1;
	}
	nu_elements = next_start_index - start_index;
	return 0;
}

int dstTaskScheduler::AddTask(int flags, dstTaskFunc func, const void *_user_data,
dstTaskDurationEstimate e, const dstTaskDivisionData& division) {
	uint32_t start_index, nu_elements;
	if (division.size != 0) {
		int r = CalculateSubdivision(division, start_index, nu_elements);
		if (r < 0)
			return r;
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
	if (new_thread) {
		// Create mutex and conditions for this thread.
		pthread_mutex_init(&task_info->continue_mutex, NULL);
		pthread_cond_init(&task_info->continue_condition, NULL);
		task_info->continue_signal = DST_TASK_THREAD_WAIT;
		pthread_mutex_init(&task_info->complete_mutex, NULL);
		pthread_cond_init(&task_info->complete_condition, NULL);
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


int dstTaskScheduler::AddSubdividedTaskGroup(int flags, dstTaskFunc func, const void *user_data,
dstTaskDivisionData& division) {
	// Find an inactive task group of the right size.
	int current_group_index = - 1;
	for (uint32_t i = 0; i < task_group_array.Size(); i++) {
		dstTaskGroup *group = task_group_array.Get(i);
		if (group->member_array.Size() == division.nu_subdivisions) {
			// The size matches.
			pthread_mutex_lock(&group->complete_mutex);
			bool inactive = (group->nu_active_members == 0);
			pthread_mutex_unlock(&group->complete_mutex);
			if (inactive) {
				// All threads are inactive, so use it.
				current_group_index = i;
				break;
			}
		}
	}
	bool new_task_group = (current_group_index < 0);

	dstTaskGroup *group;
	if (new_task_group) {
		group = new dstTaskGroup;
		pthread_mutex_init(&group->continue_mutex, NULL);
		pthread_cond_init(&group->continue_condition, NULL);
		pthread_mutex_init(&group->complete_mutex, NULL);
		pthread_cond_init(&group->complete_condition, NULL);
		group->nu_active_members = 0;
		current_group_index = task_group_array.Size();
		task_group_array.Add(group);
		group->member_array.SetCapacity(division.nu_subdivisions);
	}
	else
		group = task_group_array.Get(current_group_index);

	for (uint32_t i = 0; i < division.nu_subdivisions; i++) {
		division.index = i;
		uint32_t start_index, nu_elements;
		int r = CalculateSubdivision(division, start_index, nu_elements);
		if (r < 0)
			// Thread operates on zero elements, and does not need to do anything.
			;

		dstTaskGroupMember member;
		member.flags = flags;
		member.task_func = func;
		member.user_data = user_data;
		member.subdivision.start_index = start_index;
		member.subdivision.nu_elements = nu_elements;
		member.group = group;
		member.continue_signal = DST_TASK_THREAD_WAIT;

		int current_member_index;
		if (new_task_group) {
			current_member_index = i;
			group->member_array.Add((const dstTaskGroupMember &)member);
		}
		else {
			current_member_index = i;
			group->member_array.Set(current_member_index, (const dstTaskGroupMember &)member);
		}

		dstTaskGroupMember *member_p = &group->member_array.Get(current_member_index);

		if (new_task_group) {
			// Create the thread. For group threads, the task will not start until the
			// continue signal is given. Set CPU affinity so that each thread is on a
			// a different CPU.
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			cpu_set_t cpuset;
			CPU_ZERO(&cpuset);
			CPU_SET(current_member_index % dst_config.nu_cpus, &cpuset);
			pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
			pthread_create(&member_p->thread, &attr, dstInternalGroupThreadFunc, member_p);
			pthread_attr_destroy(&attr);
		}
	}

	// Set the number of active threads. It will take a little while for them to be really active,
	// but we are only concerned about the value reaching zero.
	group->nu_active_members = division.nu_subdivisions;

	// Broadcast the continue signal.
	for (uint32_t i = 0; i < division.nu_subdivisions; i++) {
		dstTaskGroupMember *member = &group->member_array.Get(i);
		member->continue_signal = DST_TASK_THREAD_CONTINUE;
	}
	pthread_mutex_lock(&group->continue_mutex);
	pthread_cond_broadcast(&group->continue_condition);
	pthread_mutex_unlock(&group->continue_mutex);

	return current_group_index;
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

void dstTaskScheduler::WaitUntilGroupFinished(int group_index) {
	dstTaskGroup *group = task_group_array.Get(group_index);
	pthread_mutex_lock(&group->complete_mutex);
	int r = 0;
	while (group->nu_active_members > 0 && r == 0) {
		r = pthread_cond_wait(&group->complete_condition,
			&group->complete_mutex);
	}
	if (r != 0) {
		printf("pthread_cond_wait returned error.\n");
		exit(1);
	}
	pthread_mutex_unlock(&group->complete_mutex);
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

void dstTaskScheduler::ClearGroups() {
	// Clear task groups, stopping each thread, assuming all tasks have finished
	// already.
	uint32_t n = task_group_array.Size();
	for (uint32_t i = 0; i < n; i++) {
		dstTaskGroup *group = task_group_array.Get(i);
		if (group->member_array.Size() == 0)
			continue;
		for (uint32_t j = 0; j < group->member_array.Size(); j++) {
			dstTaskGroupMember *member = &group->member_array.Get(i);
			member->continue_signal = DST_TASK_THREAD_EXIT;
		}
		pthread_cond_signal(&group->continue_condition);
		for (uint32_t j = 0; j < group->member_array.Size(); j++)
			pthread_join(group->member_array.Get(j).thread, NULL);
		pthread_cond_destroy(&group->continue_condition);
		pthread_mutex_destroy(&group->continue_mutex);
		pthread_cond_destroy(&group->complete_condition);
		pthread_mutex_destroy(&group->complete_mutex);
		group->member_array.FreeData();
		delete group;
	}
	task_group_array.Truncate(0);
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

// Start a group of n tasks. The next n tasks added with AddGroupTask will be grouped together,
// with increased threading efficiency. AddGroupTask has not yet been implemented.

int dstTaskScheduler::StartTaskGroup(int n) {
	dstTaskGroup *task_group = new dstTaskGroup;
	// The group member array of task_group will be automatically initialized
	// as an empty array by the constructor.
	task_group->member_array.SetCapacity(n);
	int current_group = task_group_array.Size();
	// Add the task group.
	task_group_array.Add(task_group);
	return current_group;
}

// Thread functions.

static void *dstInternalThreadFunc(void *data) {
	dstTaskInfo *task_info = (dstTaskInfo *)data;
	bool exit_signalled = false;
	while (!exit_signalled) {
		// Run the task.
		task_info->task_func(task_info);

		task_info->complete_signal = DST_TASK_THREAD_CONTINUE;
		pthread_mutex_lock(&task_info->complete_mutex);
		pthread_cond_signal(&task_info->complete_condition);
		pthread_mutex_unlock(&task_info->complete_mutex);

		task_info->LockMutexContinue();
		int r = 0;
		while (task_info->continue_signal == DST_TASK_THREAD_WAIT && r == 0)
			r = pthread_cond_wait(&task_info->continue_condition,
				&task_info->continue_mutex);
		if (r != 0) {
			printf("pthread_cond_wait returned error.\n");
			exit(1);
		}
		if (task_info->continue_signal == DST_TASK_THREAD_CONTINUE) {
			// Will continue with new task.
			// Reset signal code.
			task_info->continue_signal = DST_TASK_THREAD_WAIT;
		}
		else if (task_info->continue_signal == DST_TASK_THREAD_EXIT)
			exit_signalled = true;
		task_info->UnlockMutexContinue();
	}
	return NULL;
}

static void *dstInternalGroupThreadFunc(void *data) {
	dstTaskGroupMember *member = (dstTaskGroupMember *)data;
	dstTaskGroup *group = member->group;
	bool exit_signalled = false;
	for (;;) {
		// Wait for the continue signal.
		pthread_mutex_lock(&group->continue_mutex);
		int r = 0;
		while (member->continue_signal == DST_TASK_THREAD_WAIT && r == 0)
			r = pthread_cond_wait(&group->continue_condition,
				&group->continue_mutex);
		if (r != 0) {
			printf("pthread_cond_wait returned error.\n");
			exit(1);
		}
		if (member->continue_signal == DST_TASK_THREAD_CONTINUE) {
			// Will continue with new task.
			// Reset signal code.
			member->continue_signal = DST_TASK_THREAD_WAIT;
		}
		else if (member->continue_signal == DST_TASK_THREAD_EXIT)
			exit_signalled = true;
		pthread_mutex_unlock(&group->continue_mutex);
		if (exit_signalled)
			break;

		// Run the task, but only if the number of elements it operates on is
		// greater than zero.
//		printf("dstInternalGroupThreadFunc: task_func: start_index = %d, nu_elements = %d.\n",
//			member->subdivision.start_index, member->subdivision.nu_elements);
//		fflush(stdout);
		if (member->subdivision.nu_elements > 0)
			member->task_func(member);

		pthread_mutex_lock(&group->complete_mutex);
		group->nu_active_members--;
		if (group->nu_active_members == 0)
			pthread_cond_signal(&group->complete_condition);
		pthread_mutex_unlock(&group->complete_mutex);

	}
	return NULL;
}

