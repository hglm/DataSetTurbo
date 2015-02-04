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

#ifndef __DST_MISC_H__
#define __DST_MISC_H__

#include "dstConfig.h"

enum {
	DST_FLAG_SIMD_ENABLED = 0x1,
	DST_FLAG_SIMD_256 = 0x2,
	DST_FLAG_THREADING = 0x4,
	DST_FLAG_FIXED_NU_THREADS = 0x8,
};

enum {
	DST_SIMD_NONE = 0,
	DST_SIMD_FIRST = 1,
	DST_SIMD_SSE2 = 1,
	DST_SIMD_SSE3 = 2,
	DST_SIMD_SSSE3 = 3,
	DST_SIMD_SSE4A = 4,
	DST_SIMD_SSE41 = 5,
	DST_SIMD_SSE42 = 6,
	DST_SIMD_AVX = 7,
	DST_SIMD_NEON = 8,
	DST_SIMD_AVX_SSE4A_FMA4 = 9,
	DST_SIMD_AVX_FMA = 10,
	DST_SIMD_LAST = 10
};

enum {
	// Flags reflecting CPU SIMD support levels, roughly corresponding to
	// the table above.
	DST_SIMD_FLAG_NONE = 0x1,
	DST_SIMD_FLAG_SSE2 = 0x2,
	DST_SIMD_FLAG_SSE3 = 0x4,
	DST_SIMD_FLAG_SSSE3 = 0x8,
	DST_SIMD_FLAG_SSE4A = 0x10,
	DST_SIMD_FLAG_SSE41 = 0x20,
	DST_SIMD_FLAG_SSE42 = 0x40,
	DST_SIMD_FLAG_AVX = 0x80,
	DST_SIMD_FLAG_NEON = 0x100,
	DST_SIMD_FLAG_FMA4 = 0x200,
	DST_SIMD_FLAG_FMA = 0x400
};

class dstSIMDFuncs {
public :
	// Function pointers for SIMD-accelerated functions.

	// Dot product functions.
	// Calculate dot products of NxN vectors.
	void (*dstCalculateDotProductsNxNV4)(int n, const float *v1, const float *v2,
		float *dot);
	void (*dstCalculateDotProductsNxNV3P)(int n, const float *v1,
		const float *v2, float *dot);
	void (*dstCalculateDotProductsNxNV3)(int n, const float *v1,
		const float *v2, float *dot);
	// Calculate dot products of Nx1 vectors.
	void (*dstCalculateDotProductsNx1V4)(int n, const float *v1,
		const float *v2, float *dot);
	void (*dstCalculateDotProductsNx1V3PV3)(int n, const float *v1,
		const float *v2, float *dot);
	void (*dstCalculateDotProductsNx1V3)(int n, const float *v1,
		const float *v2, float *dot);
	void (*dstCalculateDotProductsNx1P3PV4)(int n, const float *v1,
		const float *v2, float *dot);
	void (*dstCalculateDotProductsNx1P3V4)(int n, const float *v1,
		const float *v2, float *dot);
	// Calculate dot products of Nx1 vectors and count the number of negative
	// results.
	void (*dstCalculateDotProductsAndCountNegativeNx1P3V4)(int n, const float *p1,
		const float *v2, float *dot, int& negative_count);
	void (*dstCalculateDotProductsAndCountNegativeNx1P3PV4)(int n, const float *p1,
		const float *v2, float *dot, int& negative_count);
	// Determine the minimum and maximum dot product of an array of vertices with a
	// given constant vector.
	void (*dstCalculateMinAndMaxDotProductNx1V3)(int nu_vertices, const float *v1,
		const float *v2, float& min_dot_product, float& max_dot_product);
	void (*dstCalculateMinAndMaxDotProductNx1V3P)(int nu_vertices, const float *v1,
		const float *v2, float& min_dot_product, float& max_dot_product);
	void (*dstCalculateMinAndMaxDotProductNx1V4)(int nu_vertices, const float *v1,
		const float *v2, float& min_dot_product, float& max_dot_product);
	// Determine the minimum and maximum dot products of an array of vertices with three
	// constant vectors.
	void (*dstCalculateMinAndMaxDotProductNx3V3)(int nu_vertices, const float *v1,
		const float *v2, float *min_dot_product, float *max_dot_product);
	void (*dstCalculateMinAndMaxDotProductNx3V3PV3)(int nu_vertices, const float *v1,
		const float *v2, float *min_dot_product, float *max_dot_product);
	void (*dstCalculateMinAndMaxDotProductNx3V4)(int nu_vertices, const float *v1,
		const float *v2, float *min_dot_product, float *max_dot_product);
	// Determine the indices within an array of vertices that have the minimum and
	// maximum dot product with a given constant vector.
	void (*dstGetIndicesWithMinAndMaxDotProductNx1V3)(int nu_vertices, const float *v1,
		const float *v2, int& i_Pmin, int& i_Pmax);
	void (*dstGetIndicesWithMinAndMaxDotProductNx1V3P)(int nu_vertices, const float *v1,
		const float *v2, int& i_Pmin, int& i_Pmax);
	void (*dstGetIndicesWithMinAndMaxDotProductNx1V4)(int nu_vertices, const float *v1,
		const float *v2, int& i_Pmin, int& i_Pmax);

	// Matrix multiplication.	
	void (*dstMatrixMultiply4x4CM)(const float *f1, const float *f2, float *f3);
	void (*dstMatrixMultiply4x3RM)(const float *f1, const float *f2, float *f3);
	void (*dstMatrixMultiply4x4CM4x3RM)(const float *f1, const float *f2, float *f3);

	void (*dstMatrixMultiplyVectors1x4M4x4CMV4)(const float *m, const float *v, float *v_result);
	void (*dstMatrixMultiplyVectors1xNM4x4CMV4)(int n, const float *m, const float *v,
		float *v_result);
	void (*dstMatrixMultiplyVectors1x4M4x4CMP3P)(const float *m, const float *v,
		float *v_result);
	void (*dstMatrixMultiplyVectors1xNM4x4CMP3P)(int n, const float *m, const float *v,
		float *v_result);
	void (*dstMatrixMultiplyVectors1xNM4x4CMV3)(int n, const float *m, const float *v,
		float *v_result);
};

extern const dstSIMDFuncs dst_simd_funcs_NoSIMD;
#ifdef DST_SSE2_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_SSE2;
extern const dstSIMDFuncs dst_simd_funcs_SSE2Stream;
#endif
#ifdef DST_SSE3_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_SSE3;
extern const dstSIMDFuncs dst_simd_funcs_SSE3Stream;
#endif
#ifdef DST_SSSE3_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_SSSE3;
extern const dstSIMDFuncs dst_simd_funcs_SSSE3Stream;
#endif
#ifdef DST_SSE4A_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_SSE4A;
extern const dstSIMDFuncs dst_simd_funcs_SSE4AStream;
#endif
#ifdef DST_SSE41_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_SSE41;
#endif
#ifdef DST_SSE42_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_SSE42;
#endif
#ifdef DST_AVX_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_AVX;
#endif
#ifdef DST_NEON_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_NEON;
#endif
#ifdef DST_AVX_SSE4A_FMA4_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_AVX_SSE4_FMA4;
#endif
#ifdef DST_AVX_FMA_SUPPORT
extern const dstSIMDFuncs dst_simd_funcs_AVX_FMA;
#endif

class DST_API dstConfig {
public :
	// Global configuration flags.
	uint32_t flags;
	// SIMD type that is currently actually used by the library.
	uint32_t simd_type;
	// All SIMD features that are supported by the current CPU.
	uint32_t simd_cpu_flags;
	// Number of CPU cores.
	uint32_t nu_cpus;
	// The logical CPU on which the main library thread is running.
	int main_thread_cpu;
	// The maximum number of threads to use in a CPU-intensive function.
	int max_threads_per_function;
	// Optional fixed number of threads setting.
	int fixed_nu_threads;
	// Function table.
	dstSIMDFuncs simd_funcs;
	dstSIMDFuncs simd_funcs_stream;
	dstTaskScheduler task_scheduler;
	int nu_tasks;
	int max_tasks;
};

extern DST_API dstConfig dst_config;

DST_API void dstInit();

static DST_INLINE_ONLY bool dstCheckFlag(uint32_t flag) {
#ifdef DST_FIXED_SIMD
	// When SIMD is guaranteed, just return true.
	if (flag == DST_FLAG_SIMD_ENABLED)
		return true;
#endif
#ifdef DST_NO_SIMD
	if (flag == DST_FLAG_SIMD_ENABLED)
		return false;
#endif
#if defined(DST_FIXED_SIMD_AVX) || defined(DST_AVX_SSE4A_FMA4) || defined(DST_AVX_FMA)
	// When 256-bit SIMD is guaranteed, just return true.
	if (flag == DST_FLAG_SIMD_256)
		return true;
#endif
	return (dst_config.flags & flag) != 0;
}

static DST_INLINE_ONLY bool dstCheckSIMDCPUFlag(uint32_t flag) {
#ifdef DST_FIXED_SIMD_SSE2
	if (flag == DST_SIMD_FLAG_SSE2)
		// When SSE2 is guaranteed, just return true.
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_SSE3
	if (flag == DST_SIMD_SSE2 || flag == DST_SIMD_FLAG_SSE3)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_SSSE3
	if (flag >= DST_SIMD_FLAG_SSE2 && flag <= DST_SIMD_FLAG_SSSE3)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_SSE4A
	if (flag == DST_SIMD_FLAG_SSE2 || flag == DST_SIMD_FLAG_SSE3 ||
	flag == DST_SIMD_FLAG_SSE4A)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_SSE41
	if ((flag >= DST_SIMD_FLAG_SSE2 && flag <= DST_SIMD_FLAG_SSE41) &&
	flag != DST_SIMD_FLAG_SSE4A)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_SSE42
	if ((flag >= DST_SIMD_FLAG_SSE2 && flag <= DST_SIMD_FLAG_SSE42) &&
	flag != DST_SIMD_FLAG_SSE4A)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_AVX
	if ((flag >= DST_SIMD_FLAG_SSE2 && flag <= DST_SIMD_FLAG_AVX) &&
	flag != DST_SIMD_FLAG_SSE4A)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_NEON
	if (flag == DST_SIMD_FLAG_NEON)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_AVX_SSE4A_FMA4
	if (flag >= DST_SIMD_FLAG_SSE2 && flag <= DST_SIMD_FLAG_FMA4)
		return true;
	else
		return false;
#endif
#ifdef DST_FIXED_SIMD_AVX_FMA
	if ((flag >= DST_SIMD_FLAG_SSE2 && flag <= DST_SIMD_FLAG_AVX) ||
	flag == DST_SIMD_FLAG_FMA)
		return true;
	else
		return false;
#endif
	return (dst_config.simd_cpu_flags & flag) != 0;
}

static DST_INLINE_ONLY void dstSetFlags(uint32_t flags) {
	dst_config.flags = flags;
}

static DST_INLINE_ONLY uint32_t dstGetFlags() {
	return dst_config.flags;
}

static DST_INLINE_ONLY void dstSetFlag(uint32_t flag) {
	dst_config.flags |= flag;
}

static DST_INLINE_ONLY void dstClearFlag(uint32_t flag) {
	dst_config.flags &= (~flag);
}

DST_API void dstSetSIMDType(int simd_type);

DST_API void dstSetStreamingSIMDType(int simd_type);

DST_API void dstSetNonStreamingSIMDType(int simd_type);

static DST_INLINE_ONLY int dstGetSIMDType() {
	return dst_config.simd_type;
}

DST_API const char *dstGetSIMDTypeString(int simd_type);

// Set two-thread cost threshold. For example, when it is equal to 1048576 (1 << 20), two threads
// will be used when the total cost is 1048576 (1 << 20) or higher, four threads will be used when
// the cost is (1 << 21) or higher, eight threads when it is (1 << 22) or higher, etc, up to the
// maximum allowed number of threads.
#define DST_TWO_THREADS_COST_THRESHOLD (1 << 21)

// Get hint about number of threads to use for operation on N elements, each with a given relative
// cost. A cost of one per element roughly corresponds to a simple operation that adds up all elements
// of an array of floats. A four vector dot product calculation with two arrays has a cost of roughly 64
// per element. This always returns a power of two.

static DST_INLINE_ONLY int dstGetNumberOfThreadsHint(uint32_t n, uint32_t cost) {
	if (dst_config.flags & DST_FLAG_FIXED_NU_THREADS)
		return dst_config.fixed_nu_threads;
	int nu_threads = mini(128, 1 + (uint64_t)n * cost / DST_TWO_THREADS_COST_THRESHOLD);
	return mini(nu_threads, dst_config.max_threads_per_function);
}

// Set fixed number of threads. The DST_FLAG_FIXED_NU_THREADS flag is not set.

static DST_INLINE_ONLY void dstSetFixedNumberOfThreads(int nu_threads) {
	dst_config.fixed_nu_threads = nu_threads;
}

// Set the maximum number of tasks that will be queued at any one time. When equal to one, no
// functions will keep threads running. Otherwise, up to max_tasks - 1 may be queued with active
// threads.

static DST_INLINE_ONLY void dstSetMaxNumberOfTasks(int max_tasks) {
	dst_config.max_tasks = max_tasks;
}

static DST_INLINE_ONLY void dstSyncTasks() {
	if (dst_config.max_tasks > 1)
		dst_config.task_scheduler.WaitUntilFinished();
}

#endif

