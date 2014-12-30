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

#include <stdio.h>
#include <signal.h>
#include <sched.h>

#include "dstConfig.h"
#include "dstRandom.h"
#include "dstTimer.h"

#include "dstCpuInfo.h"
#include "dstVectorMath.h"


// Configuration variables.

dstConfig dst_config;

// Intialization.

static const char *simd_name_table[] = {
	"None", "SSE2", "SSE3", "SSSE3", "SSE4A", "SSE41", "SSE42",
	"AVX", "AVX_SSE4A_FMA4", "AVX_FMA3", "NEON"
};

static const uint32_t simd_type_flags[] = {
	// SIMD type NONE
	0,
	// SIMD type SSE2
	DST_SIMD_FLAG_SSE2,
	// SIMD type SSE3
	DST_SIMD_FLAG_SSE2 | DST_SIMD_FLAG_SSE3,
	// SIMD type SSES3
	DST_SIMD_FLAG_SSE2 | DST_SIMD_FLAG_SSE3 | DST_SIMD_FLAG_SSSE3,
	// SIMD type SSE4A
	DST_SIMD_FLAG_SSE2 | DST_SIMD_FLAG_SSE3 | DST_SIMD_FLAG_SSE4A,
	// SIMD type SSE41
	DST_SIMD_FLAG_SSE2 | DST_SIMD_FLAG_SSE3 | DST_SIMD_FLAG_SSSE3 | DST_SIMD_FLAG_SSE41,
	// SIMD type SSE42
	DST_SIMD_FLAG_SSE2 | DST_SIMD_FLAG_SSE3 | DST_SIMD_FLAG_SSSE3 | DST_SIMD_FLAG_SSE41 |
	DST_SIMD_FLAG_SSE42,
	// SIMD type AVX
	DST_SIMD_FLAG_SSE2 | DST_SIMD_FLAG_SSE3 | DST_SIMD_FLAG_SSSE3 | DST_SIMD_FLAG_SSE41 |
	DST_SIMD_FLAG_SSE42 | DST_SIMD_FLAG_AVX,
	// SIMD type NEON
	DST_SIMD_FLAG_NEON,
	// SIMD type AVX with SSE4A and FMA4 (AMD)
	DST_SIMD_FLAG_SSE2 | DST_SIMD_FLAG_SSE3 | DST_SIMD_FLAG_SSSE3 | DST_SIMD_FLAG_SSE4A |
	DST_SIMD_FLAG_SSE41 | DST_SIMD_FLAG_SSE42 | DST_SIMD_FLAG_AVX | DST_SIMD_FLAG_FMA4,
	// SIMD type AVX with FMA3
	DST_SIMD_FLAG_SSE2 | DST_SIMD_FLAG_SSE3 | DST_SIMD_FLAG_SSSE3 | DST_SIMD_FLAG_SSE41 |
	DST_SIMD_FLAG_SSE42 | DST_SIMD_FLAG_AVX | DST_SIMD_FLAG_FMA,
};

static void PrintSIMDFlagsString(uint32_t flags) {
        if (flags == 0) {
		printf(" None");
		return;
	}
	for (int i = DST_SIMD_FIRST; i <= DST_SIMD_LAST; i++)
		if (flags & (1 << i))
			printf(" %s", simd_name_table[i]);
}

const char *dstGetSIMDTypeString(int simd_type) {
	if (simd_type >= DST_SIMD_NONE && simd_type <= DST_SIMD_LAST)
		return simd_name_table[simd_type];
	else
		return "Unknown";
}

void dstInit() {
	dstCpuInfo *cpuinfo = dstCpuInfoInit();
	dst_config.flags = 0;
	dst_config.simd_cpu_flags = 0;
	if (cpuinfo->has_arm_neon)
		dst_config.simd_cpu_flags |= DST_SIMD_FLAG_NEON;
	if (cpuinfo->has_sse2)
		dst_config.simd_cpu_flags |= DST_SIMD_FLAG_SSE2;
	if (cpuinfo->has_sse3)
		dst_config.simd_cpu_flags |= DST_SIMD_FLAG_SSE3;
	if (cpuinfo->has_ssse3)
		dst_config.simd_cpu_flags |= DST_SIMD_FLAG_SSSE3;
	if (cpuinfo->has_sse4a)
		dst_config.simd_cpu_flags |= DST_SIMD_FLAG_SSE4A;
	if (cpuinfo->has_sse41)
		dst_config.simd_cpu_flags |= DST_SIMD_FLAG_SSE41;
	if (cpuinfo->has_sse42)
		dst_config.simd_cpu_flags |= DST_SIMD_FLAG_SSE42;
	if (cpuinfo->has_avx)
		dst_config.simd_cpu_flags |= DST_SIMD_FLAG_AVX;
	if (cpuinfo->has_fma4)
		dst_config.simd_cpu_flags |= DST_SIMD_FLAG_FMA4;
	if (cpuinfo->has_fma3)
		dst_config.simd_cpu_flags |= DST_SIMD_FLAG_FMA;

#ifdef DST_NO_SIMD
	dst_config.simd_type = DST_SIMD_NONE;
#elif defined(DST_FIXED_SIMD)
#ifdef DST_FIXED_SIMD_SSE2
	dst_config.simd_type = DST_SIMD_SSE2;
#endif
#ifdef DST_FIXED_SIMD_SSE3
	dst_config.simd_type = DST_SIMD_SSE3;
#endif
#ifdef DST_FIXED_SIMD_SSSE3
	dst_config.simd_type = DST_SIMD_SSSE3;
#endif
#ifdef DST_FIXED_SIMD_SSE4A
	dst_config.simd_type = DST_SIMD_SSE4A;
#endif
#ifdef DST_FIXED_SIMD_SSE41
	dst_config.simd_type = DST_SIMD_SSE41;
#endif
#ifdef DST_FIXED_SIMD_SSE42
	dst_config.simd_type = DST_SIMD_SSE42;
#endif
#ifdef DST_FIXED_SIMD_AVX
	dst_config.simd_type = DST_SIMD_AVX;
#endif
#ifdef DST_FIXED_SIMD_NEON
	dst_config.simd_type = DST_SIMD_NEON;
#endif
#ifdef DST_FIXED_SIMD_AVX_SSE4A_FMA4
	dst_config.simd_type = DST_SIMD_AVX_SSE4A_FMA4;
#endif
#ifdef DST_FIXED_SIMD_AVX_FMA
	dst_config.simd_type = DST_SIMD_AVX_FMA;
#endif
	// Verify that the fixed SIMD type is available.
	if ((dst_config.simd_cpu_flags & simd_type_flags[dst_config.simd_type]) !=
	simd_type_flags[dst_config.simd_type]) {
		printf("Error -- no support required SIMD extensions, aborting.\n");
		raise(SIGABRT);
	}
#endif // defined(DST_FIXED_SIMD)

#if !defined(DST_NO_SIMD) && !defined(DST_FIXED_SIMD)
	// Auto-detect SIMD type.
	dst_config.simd_type = DST_SIMD_NONE;
	// Check availability of SIMD types from most advanced (recent) to oldest.
	for (int i = DST_SIMD_LAST; i >= DST_SIMD_FIRST; i--)
		if ((dst_config.simd_cpu_flags & simd_type_flags[i]) == simd_type_flags[i]) {
			dst_config.simd_type = i;
			break;
		}
#endif

	if (dst_config.simd_type != DST_SIMD_NONE)
		dst_config.flags |= DST_FLAG_SIMD_ENABLED;
	if (dst_config.simd_type == DST_SIMD_AVX || 
	(dst_config.simd_type >= DST_SIMD_AVX_SSE4A_FMA4 && dst_config.simd_type <= DST_SIMD_AVX_FMA))
		dst_config.flags |= DST_FLAG_SIMD_256;
#if !defined(DST_NO_SIMD) && !defined(DST_FIXED_SIMD)
	// The SIMD type has been autodetected and can be changed.
	// Initialize the function pointers.
	dstSetSIMDType(dst_config.simd_type);
#endif

	dst_config.nu_cpus = sysconf(_SC_NPROCESSORS_CONF);
	// Make the main library thread execute on CPU 0.
	cpu_set_t cpu_set;
	CPU_ZERO(&cpu_set);
	CPU_SET(0, &cpu_set);
	sched_setaffinity(0, sizeof(cpu_set_t), &cpu_set);
	dst_config.main_thread_cpu = 0;

	dst_config.max_threads_per_function = dst_config.nu_cpus;
	if (dst_config.max_threads_per_function > 1)
		dst_config.flags |= DST_FLAG_THREADING;

	printf("dstInit: Processor name: %s\n", cpuinfo->processor_name);
	printf("dstInit: Processor SIMD features: ");
	PrintSIMDFlagsString(dst_config.simd_cpu_flags);
	printf("\n");
	printf("dstInit: SIMD feature level used: %s (%s)\n", dstGetSIMDTypeString(dst_config.simd_type),
#if defined(DST_FIXED_SIMD) || defined(DST_NO_SIMD)
	"fixed"
#else
	"detected"
#endif
		);
	printf("dstInit: Max level of multi-threading per function: %d\n",
		dst_config.max_threads_per_function);
}


#ifndef DST_NO_SIMD

static const dstSIMDFuncs *dst_simd_funcs_table[] = {
	&dst_simd_funcs_NoSIMD,
#ifdef DST_SSE2_SUPPORT
	&dst_simd_funcs_SSE2,
#else
	NULL,
#endif
#ifdef DST_SSE3_SUPPORT
	&dst_simd_funcs_SSE3,
#else
	NULL,
#endif
#ifdef DST_SSSE3_SUPPORT
	&dst_simd_funcs_SSSE3,
#else
	NULL,
#endif
#ifdef DST_SSE4A_SUPPORT
	&dst_simd_funcs_SSE4A,
#else
	NULL,
#endif
#ifdef DST_SSE41_SUPPORT
	&dst_simd_funcs_SSE41,
#else
	NULL,
#endif
#ifdef DST_SSE42_SUPPORT
	&dst_simd_funcs_SSE42,
#else
	NULL,
#endif
#ifdef DST_AVX_SUPPORT
	&dst_simd_funcs_AVX,
#else
	NULL,
#endif
#ifdef DST_NEON_SUPPORT
	&dst_simd_funcs_NEON,
#else
	NULL,
#endif
#ifdef DST_AVX_SSE4A_FMA4_SUPPORT
	&dst_simd_funcs_AVX_SSE4A_FMA4,
#else
	NULL,
#endif
#ifdef DST_AVX_FMA_SUPPORT
	&dst_simd_funcs_AVX_FMA,
#else
	NULL,
#endif
};

static const dstSIMDFuncs *dst_simd_funcs_stream_table[] = {
	&dst_simd_funcs_NoSIMD,
#ifdef DST_SSE2_SUPPORT
	&dst_simd_funcs_SSE2Stream,
#else
	NULL,
#endif
#ifdef DST_SSE3_SUPPORT
	&dst_simd_funcs_SSE3Stream,
#else
	NULL,
#endif
#ifdef DST_SSSE3_SUPPORT
	&dst_simd_funcs_SSSE3Stream,
#else
	NULL,
#endif
#ifdef DST_SSE4A_SUPPORT
	&dst_simd_funcs_SSE4AStream,
#else
	NULL,
#endif
#ifdef DST_SSE41_SUPPORT
	&dst_simd_funcs_SSE41Stream,
#else
	NULL,
#endif
#ifdef DST_SSE42_SUPPORT
	&dst_simd_funcs_SSE42Stream,
#else
	NULL,
#endif
#ifdef DST_AVX_SUPPORT
	&dst_simd_funcs_AVXStream,
#else
	NULL,
#endif
#ifdef DST_NEON_SUPPORT
	&dst_simd_funcs_NEONStream,
#else
	NULL,
#endif
#ifdef DST_AVX_SSE4A_FMA4_SUPPORT
	&dst_simd_funcs_AVX_SSE4A_FMA4Stream,
#else
	NULL,
#endif
#ifdef DST_AVX_FMA_SUPPORT
	&dst_simd_funcs_AVX_FMAStream,
#else
	NULL,
#endif
};
#endif

void dstSetSIMDType(int simd_type) {
#if !defined(DST_NO_SIMD) && !defined(DST_FIXED_SIMD)
	dst_config.simd_type = simd_type;
	dst_config.simd_funcs = *dst_simd_funcs_table[simd_type];
	dst_config.simd_funcs_stream = *dst_simd_funcs_stream_table[simd_type];
	if (simd_type == DST_SIMD_NONE)
		dst_config.flags &= (~DST_FLAG_SIMD_ENABLED);
	else
		dst_config.flags |= DST_FLAG_SIMD_ENABLED;

#endif
}

void dstSetStreamingSIMDType(int simd_type) {
#if !defined(DST_NO_SIMD) && !defined(DST_FIXED_SIMD)
	dstSetSIMDType(simd_type);
	dst_config.simd_funcs = *dst_simd_funcs_stream_table[simd_type];
	dst_config.simd_funcs_stream = *dst_simd_funcs_stream_table[simd_type];
#endif
}

void dstSetNonStreamingSIMDType(int simd_type) {
#if !defined(DST_NO_SIMD) && !defined(DST_FIXED_SIMD)
	dstSetSIMDType(simd_type);
	dst_config.simd_funcs = *dst_simd_funcs_table[simd_type];
	dst_config.simd_funcs_stream = *dst_simd_funcs_table[simd_type];
#endif
}


// Default random number generator.

// This global variable definition will trigger the constructor at
// program initialization time.
static dstCMWCRNG dst_internal_rng;
static dstRNG *dst_default_rng = NULL;

dstRNG *dstGetDefaultRNG() {
	if (dst_default_rng == NULL)
		dst_default_rng = &dst_internal_rng;
	return dst_default_rng;
}

void dstSetDefaultRNG(dstRNG* rng) {
	if (rng == NULL)
		dst_default_rng = &dst_internal_rng;
	else
		dst_default_rng = rng;
}

#ifdef _WIN32

// Windows version of gettimeofday().

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

void gettimeofday(struct timeval *tv, struct timezone *tz) {
    FILETIME ft;
    uint64_t tmpres = 0;
    if (NULL != tv) {
        GetSystemTimeAsFileTime(&ft);
        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;
        tmpres /= 10;  /*convert into microseconds*/
                       /*converting file time to unix epoch*/
        tmpres -= DELTA_EPOCH_IN_MICROSECS; 
        tv->tv_sec = (long)(tmpres / 1000000UL);
        tv->tv_usec = (long)(tmpres % 1000000UL);
    }
}

#endif

