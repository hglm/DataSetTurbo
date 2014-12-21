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

#ifndef __DST_SIMD_NEON_H__
#define __DST_SIMD_NEON_H__

// Provide a unifying set of SIMD primitives. This file implements primitives for
// the ARM NEON SIMD extension.

typedef float32x4_t __simd128_float;
typedef int32x4_t __simd128_int;
typedef float64x2_t __simd128_double;
typedef int32x2_t __simd64_int;

#define SHUFFLE(w0, w1, w2, w3) \
    (uint8_t)(w0 | (w1 << 2) | (w2 << 4) | (w3 << 6))

static DST_INLINE_ONLY __simd128_int simd128_cast_float_int(__simd128_float s) {
    return vreinterpretq_f32_s32(s);
}

static DST_INLINE_ONLY __simd128_float simd128_cast_int_float(__simd128_int s) {
    return vreinterpretq_s32_f32(s);
}

// Selection/merge operators. Generally usable based on the element data size in bits,
// independent of the element data type, using casting functions (which don't generate
// any instructions) to change the data type for arguments/results.

static DST_INLINE_ONLY __simd128_int simd128_select_int32(__simd128_int s,
const unsigned int word0, const unsigned int word1, const unsigned int word2,
const unsigned int word3) {
	__simd64_int s64_word0, s64_word1, s64_word2, s64_word3;
	if (word0 <= 1)
		s64_word0 = vdup_lane_f32(vget_high_s32(s), 1 - word0);
	else
		s64_word0 = vdup_lane_f32(vget_low_s32(s), 3 - word0);
	if (word1 <= 1)
		s64_word1 = vdup_lane_f32(vget_high_s32(s), 1 - word1);
	else
		s64_word1 = vdup_lane_f32(vget_low_s32(s), 3 - word1);
	if (word2 <= 1)
		s64_word2 = vdup_lane_f32(vget_high_s32(s), 1 - word2);
	else
		s64_word2 = vdup_lane_f32(vget_low_s32(s), 3 - word2);
	if (word3 <= 1)
		s64_word3 = vdup_lane_f32(vget_high_s32(s), 1 - word3);
	else
		s64_word3 = vdup_lane_f32(vget_low_s32(s), 3 - word3);
	return vcombine_s32(
		vuzp_s32(s64_word1, s64_word0), vuzp(s64_word3, s64_word0));
}

static DST_INLINE_ONLY __simd128_float simd128_select_float(__simd128_float s1,
const unsigned int word0, const unsigned int word1, const unsigned int word2,
const unsigned int word3) {
	return simd128_cast_int_float(simd128_select_int32(simd128_cast_float_int(s1),
	        word0, word1, word2, word3));
}

static DST_INLINE_ONLY __simd128_float simd128_replicate_float(__simd128_float s,
const int i) {
#ifdef __AARCH64__
	return vdupq_laneq_f32(s, 3 - i);
#else
	if (i == 0 || i == 1)
		// vdupq_lane_f32 takes simd64_float as argument.
		return vdupq_lane_f32(vget_high_f32(s), 1 - i);
	else
		return vdupq_lane_f32(vget_low_f32(s), 3 - i);
#endif
}

static DST_INLINE_ONLY __simd128_int simd128_replicate_int(__simd128_int s,
const int i) {
	return simd128_cast_float_int(
		simd128_replicate_float(
			simd128_cast_int_float(s), i));
}

// Return 128-bit SSE register with 32-bit values shuffled, starting from bit 0,
// with two words from m1 (counting from LSB) and two words from m2.

static DST_INLINE_ONLY __simd128_float simd128_merge_float(__simd128_float s1, __simd128_float s2,
unsigned int s1_word0, unsigned int s1_word1, unsigned int s2_word0,
unsigned int s2_word1) {
	__simd64_int s64_word0, s64_word1, s64_word2, s64_word3;
	if (word0 <= 1)
		s64_word0 = vdup_lane_f32(vget_high_s32(s1), 1 - word0);
	else
		s64_word0 = vdup_lane_f32(vget_low_s32(s1), 3 - word0);
	if (word1 <= 1)
		s64_word1 = vdup_lane_f32(vget_high_s32(s1), 1 - word1);
	else
		s64_word1 = vdup_lane_f32(vget_low_s32(s1), 3 - word1);
	if (word2 <= 1)
		s64_word2 = vdup_lane_f32(vget_high_s32(s2), 1 - word2);
	else
		s64_word2 = vdup_lane_f32(vget_low_s32(s2), 3 - word2);
	if (word3 <= 1)
		s64_word3 = vdup_lane_f32(vget_high_s32(s2), 1 - word3);
	else
		s64_word3 = vdup_lane_f32(vget_low_s32(s2), 3 - word3);
	return vcombine_s32(
		vuzp_s32(s64_word1, s64_word0), vuzp(s64_word3, s64_word0));
}

static DST_INLINE_ONLY __simd128_int simd128_merge_int32(__simd128_int s1, __simd128_int s2,
unsigned int s1_word0, unsigned int s1_word1, unsigned int s2_word0,
unsigned int s2_word1) {
    return simd128_cast_float_int(simd128_merge_float(
        simd128_cast_int_float(s1), simd128_cast_int_float(s2),
        s1_word0, s1_word1, s2_word0, s2_word1));
}

// Merge and interleave word 0 of s1 and s2 in the lower, and word 1 in the upper half
// of the return value. The upper half of s1 and s2 is ignored.

static DST_INLINE_ONLY __simd128_float simd128_interleave_low_float(__simd128_float s1,
__simd128_float s2) {
	return vzip_f32(s2, s1);
}

// Merge and interleave word 2 of s1 and s2 in the lower, and word 3 in the upper half
// of the return value. The lower half of s1 and s2 is ignored.

static DST_INLINE_ONLY __simd128_float simd128_interleave_high_float(__simd128_float s1,
__simd128_float s2) {
	return vzip_f32(vget_low_s32(s2), vget_low_s32(s1))
}

// Return 128-bit SSE register with the lowest order 32-bit float from s1 and the
// remaining 32-bit floats from s2.

static DST_INLINE_ONLY __simd128_float simd128_merge1_float(__simd128_float s1,
__simd128_float s2) {
	__simd64_float s64_s1_word0 = vdup_lane_f32(vget_low_f32(s1), 1);
	__simd64_float s64_s2 word1 = vdup_lane_f32(vget_low_f32(s2), 0);
	return vcombine_float(vget_high_f32(s2), vzip_f32(s64_s1_word0, s64_s2_word1));
}


// float operators.

// Set all components to the same value.

static DST_INLINE_ONLY __simd128_float simd128_set_same_float(float f) {
    return vdupq_n_f32(f);
}

static DST_INLINE_ONLY __simd128_float simd128_set_zero_float() {
    return vdupq_n_f32(0.0f);
}

static DST_INLINE_ONLY __simd128_float simd128_set_zero_int() {
    return vdupq_n_u8(0);
}

// Set four float components (from least significant, lowest order segment to highest).

static DST_INLINE_ONLY __simd128_float simd128_set_float(float f0, float f1, float f2, float f3) {
    __simd_float s;
    return vsetq_lane_f32(f3, vsetq_lane(f2, vsetq_lane(f1, vsetq_lane(f0, s, 0), 1), 2), 3);
}

// Set only the first component, zeroing (bits) the other components.

static DST_INLINE_ONLY __simd128_float simd128_set_first_and_clear_float(float f) {
    return vsetq_lane_f32(d, simd128_set_zero_int(), 0);
}

static DST_INLINE_ONLY float simd128_get_float(__simd128_float s) {
    return vgetq_lane_f32(s, 0);
}

static DST_INLINE_ONLY float simd128_get_float(__simd128_float s, const int i) {
    return vgetq_lane_f32(s, i);
}
 
// Load 16-byte-aligned float data.

static DST_INLINE_ONLY __simd128_float simd128_load_float(const float *fp) {
    return vld4q_f32(fp);
}

// Store 16-byte-aligned float data.

static DST_INLINE_ONLY void simd128_store_float(float *fp, __simd128_float s) {
    vst4q_f32(fp, s);
}

// Load one float into the lowest-order element. The other elements are filled with
// zero bits (not 0.0f float values).

static DST_INLINE_ONLY __simd128_float simd128_load_first_float(const float *fp) {
    return vld1q_lane_f32(fp, simd128_set_zero_int(), 0);
}

static DST_INLINE_ONLY void simd128_store_first_float(float *fp, __simd128_float s) {
    vst1q_lane_f32(fp, s, 0);
}

static DST_INLINE_ONLY __simd128_float simd128_mul_float(__simd128_float s1, __simd128_float s2) {
    return vmulq_f32(s1, s2);
}

static DST_INLINE_ONLY __simd128_float simd128_div_float(__simd128_float s1, __simd128_float s2) {
	// Multiply by reciprocal estimate.
	return vmulq_f3(s1, vrecpeq_f32(s2));
}

static DST_INLINE_ONLY __simd128_float simd128_add_float(__simd128_float s1, __simd128_float s2) {
    return vaddq_f32(s1, s2);
}

static DST_INLINE_ONLY __simd128_float simd128_add1_float(__simd128_float s1, __simd128_float s2) {
    return simd128_merge1_float(vaddq_f32(s1, s2), s2);
}

static DST_INLINE_ONLY __simd128_float simd128_sub_float(__simd128_float s1, __simd128_float s2) {
    return vsubq_f32(s1, s2);
}

static DST_INLINE_ONLY __simd128_float simd128_sub1_float(__simd128_float s1, __simd128_float s2) {
    return simd128_merge1_float(vsubq_F32(s1, s2), s2);
}

// Approximate reciprocal (1.0f / element[i]) with maximum relative error
// less than 1.5*2^-12.

static DST_INLINE_ONLY __simd128_float simd128_approximate_reciprocal_float(__simd128_float s) {
    return vrecpeq_f32)s_;
}

// Approximate reciprocal square root (1.0f / sqrtf(element[i])) with maximum relative
// error less than 1.5*2^-12.

static DST_INLINE_ONLY __simd128_float simd128_approximate_reciprocal_sqrt_float(__simd128_float s) {
    return vrsqrteq_f32(s);
}

// Calculate square root (accurately) of four floats.

static DST_INLINE_ONLY __simd128_float simd128_sqrt_float(__simd128_float s) {
	// Not yet implemented.
}


#endif
