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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include <dstDynamicArray.h>
#include <dstQueue.h>
#include <dstTimer.h>

int main(int argc, char *argv[]) {
	dstIntQueue queue;
	printf("dstIntQueue queue capacity = %d\n", queue.Capacity());
	queue.Enqueue(0);
	dstIntQueue queue8(8);
	printf("dstIntQueue queue(8) capacity = %d\n", queue.Capacity());
	queue.Enqueue(0);

	dstIntArray a;
	printf("dstIntArray: MaxCapacity() = %u\n", a.MaxCapacity());
        int capacity = a.Capacity();
	for (int i = 0; i < 1000000; i++) {
		int previous_capacity = capacity;
		a.Add(i);
		capacity = a.Capacity();
		if (capacity != previous_capacity)
			printf("dstIntArray capacity changed from %d to %d.\n",
				previous_capacity, capacity);
	}

	dstTightIntArray ta;
	printf("dstTightIntArray: MaxCapacity() = %u\n", ta.MaxCapacity());
        capacity = ta.Capacity();
	for (int i = 0; i < 1000000; i++) {
		int previous_capacity = capacity;
		ta.Add(i);
		capacity = ta.Capacity();
		if (capacity != previous_capacity)
			printf("dstTightIntArray capacity changed from %d to %d.\n",
				previous_capacity, capacity);
	}

	dstTightVerySmallIntArray tvsa1;
	int tvsa_max_capacity = tvsa1.MaxCapacity();
	printf("dstTightVerySmallIntArray: MaxCapacity() = %u\n", tvsa_max_capacity);
        capacity = tvsa1.Capacity();
	for (int i = 0; i < tvsa_max_capacity; i++) {
		int previous_capacity = capacity;
		tvsa1.Add(i);
		capacity = tvsa1.Capacity();
		if (capacity != previous_capacity)
			printf("dstTightVerySmallIntArray capacity changed from %d to %d.\n",
				previous_capacity, capacity);
	}

	dstTimer timer;
	dstIntArray a2;
        for (int j = 0; j < 10001; j++) {
		// Allow one round of warm-up before starting timer.
		if (j == 1)
			timer.Start();
		a2.MakeEmpty();
		for (int i = 0; i < 100000; i++) {
			a2.Add(i);
		}
	}
	double elapsed = timer.Elapsed();
        printf("dstIntArray: Time taken: %.3lfs, %.3lf million adds per second\n", elapsed,
		10000.0d * 100000.0d / 1000000.0d / elapsed);

	dstTightIntArray ta2;
        for (int j = 0; j < 10001; j++) {
		// Allow one round of warm-up before starting timer.
		if (j == 1)
			timer.Start();
		ta2.MakeEmpty();
		for (int i = 0; i < 100000; i++) {
			ta2.Add(i);
		}
	}
	elapsed = timer.Elapsed();
        printf("dstTightIntArray: Time taken: %.3lfs, %.3lf million adds per second\n", elapsed,
		10000.0d * 100000.0d / 1000000.0d / elapsed);

	dstIntArray a3(100000);
        for (int j = 0; j < 10001; j++) {
		// Allow one round of warm-up before starting timer.
		if (j == 1)
			timer.Start();
		a3.MakeEmpty();
		for (int i = 0; i < 100000; i++) {
			a3.Add(i);
		}
	}
	elapsed = timer.Elapsed();
        printf("Preallocated dstIntArray: Time taken: %.3lfs, %.3lf million adds per second\n", elapsed,
		10000.0d * 100000.0d / 1000000.0d / elapsed);

#define NU_ARRAYS 16384
	dstIntArray ma[NU_ARRAYS];
        for (int j = 0; j < 1001; j++) {
		// Allow one round of warm-up before starting timer.
		if (j == 1)
			timer.Start();
		for (int i = 0 ; i < NU_ARRAYS; i++)
			ma[i].Truncate(0);
		for (int i = 0; i < 100000; i++) {
			ma[i & (NU_ARRAYS - 1)].Add(i);
		}
	}
	elapsed = timer.Elapsed();
        printf("dstIntArray (many small arrays): Time taken: %.3lfs, %.3lf million adds per second\n",
		elapsed, 1000.0d * 100000.0d / 1000000.0d / elapsed);

	dstHugeIntArray ha;
	uint64_t ha_max_capacity = ha.MaxCapacity();
	printf("dstHugeIntArray: MaxCapacity() = %lu\n", ha_max_capacity);

	dstTightHugeIntArray tha[NU_ARRAYS];
	uint64_t tha_max_capacity = tha[0].MaxCapacity();
	printf("dstTightHugeIntArray: MaxCapacity() = %lu\n", tha_max_capacity);
        for (int j = 0; j < 1001; j++) {
		// Allow one round of warm-up before starting timer.
		if (j == 1)
			timer.Start();
		for (int i = 0 ; i < NU_ARRAYS; i++)
			tha[i].Truncate(0);
		for (int i = 0; i < 100000; i++) {
			tha[i & (NU_ARRAYS - 1)].Add(i);
		}
	}
	elapsed = timer.Elapsed();
        printf("dstTightHugeIntArray (many small arrays): Time taken: %.3lfs, %.3lf million adds per second\n",
		elapsed, 1000.0d * 100000.0d / 1000000.0d / elapsed);

	dstTightIntArray mta[NU_ARRAYS];
        for (int j = 0; j < 1001; j++) {
		// Allow one round of warm-up before starting timer.
		if (j == 1)
			timer.Start();
		for (int i = 0 ; i < NU_ARRAYS; i++)
			mta[i].Truncate(0);
		for (int i = 0; i < 100000; i++) {
			mta[i & (NU_ARRAYS - 1)].Add(i);
		}
	}
	elapsed = timer.Elapsed();
        printf("dstTightIntArray (many small arrays): Time taken: %.3lfs, %.3lf million adds per second\n",
		elapsed, 1000.0d * 100000.0d / 1000000.0d / elapsed);

	dstVerySmallIntArray vsa[NU_ARRAYS];
        for (int j = 0; j < 1001; j++) {
		// Allow one round of warm-up before starting timer.
		if (j == 1)
			timer.Start();
		for (int i = 0 ; i < NU_ARRAYS; i++)
			vsa[i].Truncate(0);
		for (int i = 0; i < 100000; i++) {
			vsa[i & (NU_ARRAYS - 1)].Add(i);
		}
	}
	elapsed = timer.Elapsed();
        printf("dstVerySmallIntArray (many small arrays): Time taken: %.3lfs, %.3lf million adds per second\n",
		elapsed, 1000.0d * 100000.0d / 1000000.0d / elapsed);

	dstTightVerySmallIntArray tvsa[NU_ARRAYS];
        for (int j = 0; j < 1001; j++) {
		// Allow one round of warm-up before starting timer.
		if (j == 1)
			timer.Start();
		for (int i = 0 ; i < NU_ARRAYS; i++)
			tvsa[i].Truncate(0);
		for (int i = 0; i < 100000; i++) {
			tvsa[i & (NU_ARRAYS - 1)].Add(i);
		}
	}
	elapsed = timer.Elapsed();
        printf("dstTightVerySmallIntArray (many small arrays): Time taken: %.3lfs, %.3lf million adds per second\n",
		elapsed, 1000.0d * 100000.0d / 1000000.0d / elapsed);


	dstIntQueue q;
	printf("dstIntQueue: MaxCapacity() = %u\n", q.MaxCapacity());
        capacity = q.Capacity();
	for (int i = 0; i < 1000; i++) {
		int previous_capacity = capacity;
		q.Enqueue(i);
		capacity = q.Capacity();
		if (capacity != previous_capacity)
			printf("dstIntQueue minimum capacity changed from %d to %d.\n",
				previous_capacity, capacity);
	}

        for (int j = 0; j < 1001; j++) {
		// Allow one round of warm-up before starting timer.
		if (j == 1)
			timer.Start();
		__volatile__ int v = 0;
		for (int i = 0; i < 100000; i++) {
			v += q.Dequeue();
			q.Enqueue(i);
		}
	}
	elapsed = timer.Elapsed();
        printf("dstIntQueue: Time taken: %.3lfs, %.3lf million dequeue/enqueue combinations per second\n",
		elapsed, 1000.0d * 100000.0d / 1000000.0d / elapsed);

	dstSmallIntQueue sq;
	printf("dstSmallIntQueue: MaxCapacity() = %u\n", sq.MaxCapacity());
	for (int i = 0; i < 64; i++) {
		sq.Enqueue(i);
	}

        for (int j = 0; j < 1001; j++) {
		// Allow one round of warm-up before starting timer.
		if (j == 1)
			timer.Start();
		__volatile__ int v = 0;
		for (int i = 0; i < 100000; i++) {
			v += sq.Dequeue();
			sq.Enqueue(i);
		}
	}
	elapsed = timer.Elapsed();
        printf("dstSmallIntQueue: Time taken: %.3lfs, %.3lf million dequeue/enqueue combinations per second\n",
		elapsed, 1000.0d * 100000.0d / 1000000.0d / elapsed);

	exit(0);
}

