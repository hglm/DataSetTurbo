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

#ifndef __DST_TIMER_H__
#define __DST_TIMER_H__

#include <dstConfig.h>

#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#ifdef _WIN32
void gettimeofday(struct timeval *tv, struct timezone *tz);
#endif

// Return current system time/date in cumulative microseconds

DST_API inline uint64_t dstGetCurrentTimeUSec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

// Return current system time/date in seconds (double floating point format)

DST_API inline double dstGetCurrentTime() {
    return (double)dstGetCurrentTimeUSec() * 0.000001d;
}

// Simple time measurement between two moments.

class DST_API dstTimer {
private :
	uint64_t start_time;
public :
	void Start() {
		start_time = dstGetCurrentTimeUSec();
	}
	uint64_t ElapsedUSec() {
		uint64_t end_time = dstGetCurrentTimeUSec();
		uint64_t diff_time = end_time - start_time;
		start_time = end_time;
		return diff_time;
	}
	double Elapsed() {
		return (double)ElapsedUSec() * 0.000001d;
	}
};

// Threaded time-out.

class DST_API dstThreadedTimeout {
private :
	bool *stop_signalled;
	uint64_t timeout_period;
	pthread_t thread;
	pthread_mutex_t mutex;
	
	static void *Thread(void *p) {
		dstThreadedTimeout *tt = (dstThreadedTimeout *)p;
		int secs = tt->timeout_period / 1000000;
		if (secs > 0)
			sleep(secs);
		int usecs = tt->timeout_period % 1000000;
		if (usecs > 0)
			usleep(usecs);
		pthread_mutex_lock(&tt->mutex);
		*tt->stop_signalled = true;
		pthread_mutex_unlock(&tt->mutex);
		return NULL;
	}
public :
	dstThreadedTimeout() {
		stop_signalled = new bool[1];
		pthread_mutex_init(&mutex, NULL);
	}
	~dstThreadedTimeout() {
		Stop();
		delete [] stop_signalled;
	}
	void Start(uint64_t timeout_in_usec) {
		timeout_period = timeout_in_usec;
		*stop_signalled = false;
		void * (*thread_func)(void *);
		thread_func = &dstThreadedTimeout::Thread;
		pthread_create(&thread, NULL, thread_func, this);
	}
	void Stop() {
		pthread_mutex_lock(&mutex);
		if (!(*stop_signalled))
			pthread_cancel(thread);
		pthread_mutex_unlock(&mutex);
		pthread_join(thread, NULL);
	}
	bool StopSignalled() {
		pthread_mutex_lock(&mutex);
		bool b = *stop_signalled;
		pthread_mutex_unlock(&mutex);
		return b;
	}
};

#endif

