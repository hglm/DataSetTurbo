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
#include <dstTimer.h>

int main(int argc, char *argv[]) {
	dstIntArray a;
        int size = a.Size();
	for (int i = 0; i < 1000000; i++) {
		int previous_size = size;
		a.Add(i);
		size = a.AllocatedSize();
		if (size != previous_size)
			printf("dstTightIntArray size changed from %d to %d.\n",
				previous_size, size);
	}

	dstTightIntArray ta;
        size = ta.AllocatedSize();
	for (int i = 0; i < 1000000; i++) {
		int previous_size = size;
		ta.Add(i);
		size = ta.AllocatedSize();
		if (size != previous_size)
			printf("dstTightIntArray size changed from %d to %d.\n",
				previous_size, size);
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

	exit(0);
}

