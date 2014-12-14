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

#ifndef __DST_SSE2_H__
#define __DST_SSE2_H__

// Provide a unifying set of SIMD primitives. This file implements primitives for
// PC class processor with SSE2 enabled, which optionally use additional features
// such as SSE3 (if DST_USE_SSE3 is defined).

typedef __m128 __simd128_float;
typedef __m128i __simd128_int;
typedef __m128d __simd128_double;

#define SHUFFLE(w0, w1, w2, w3) \
    (uint8_t)(w0 | (w1 << 2) | (w2 << 4) | (w3 << 6))

static inline_only __simd128_int simd128_cast_float_int(__simd128_float s) {
    return _mm_castps_si128(s);
}

static inline_only __simd128_float simd128_cast_int_float(__simd128_int s) {
    return _mm_castsi128_ps(s);
}

// Selection/merge operators. Generally usable based on the element data size in bits,
// independent of the element data type, using casting functions (which don't generate
// any instructions) to change the data type for arguments/results.

static inline_only __simd128_int simd128_select_int32(__simd128_int s1,
unsigned int word0, unsigned int word1, unsigned int word2, unsigned int word3) {
    return _mm_shuffle_epi32(s1, SHUFFLE(word0, word1, word2, word3));
}

static inline_only __simd128_float simd128_select_float(__simd128_float s1,
unsigned int word0, unsigned int word1, unsigned int word2, unsigned int word3) {
    return simd128_cast_int_float(simd128_select_int32(simd128_cast_float_int(s1),
        word0, word1, word2, word3));
}

// Return 128-bit SSE register with 32-bit values shuffled, starting from bit 0,
// with two words from m1 (counting from LSB) and two words from m2.

static inline_only __simd128_float simd128_merge_float(__simd128_float s1, __simd128_float s2,
unsigned int s1_word0, unsigned int s1_word1, unsigned int s2_word0,
unsigned int s2_word1) {
    return _mm_shuffle_ps(s1, s2, SHUFFLE(s1_word0, s1_word1, s2_word0, s2_word1));
}

static inline_only __simd128_int simd128_merge_int32(__simd128_int s1, __simd128_int s2,
unsigned int s1_word0, unsigned int s1_word1, unsigned int s2_word0,
unsigned int s2_word1) {
    return simd128_cast_float_int(simd128_merge_float(
        simd128_cast_int_float(s1), simd128_cast_int_float(s2),
        s1_word0, s1_word1, s2_word0, s2_word1));
}

// Merge and interleave word 0 of s1 and s2 in the lower, and word 1 in the upper half
// of the return value. The upper half of s1 and s2 is ignored.

static inline_only __simd128_float simd128_interleave_low_float(__simd128_float s1,
__simd128_float s2) {
    return _mm_unpacklo_ps(s1, s2);
}

// Merge and interleave word 2 of s1 and s2 in the lower, and word 3 in the upper half
// of the return value. The lower half of s1 and s2 is ignored.

static inline_only __simd128_float simd128_interleave_high_float(__simd128_float s1,
__simd128_float s2) {
    return _mm_unpackhi_ps(s1, s2);
}

// Return 128-bit SSE register with the lowest order 32-bit float from s1 and the
// remaining 32-bit floats from s2.

static inline_only __simd128_float simd128_merge1_float(__simd128_float s1,
__simd128_float s2) {
    return _mm_move_ss(s2, s1);
}

// float operators.

// Set all components to the same value.

static inline_only __simd128_float simd128_set_same_float(float f) {
    return _mm_set1_ps(f);
}

// Set four float components (from least significant, lowest order segment to highest).

static inline_only __simd128_float simd128_set_float(float f0, float f1, float f2, float f3) {
    return _mm_set_ps(f3, f2, f1, f0);
}

// Set only the first component, zeroing (bits) the other components.

static inline_only __simd128_float simd128_set_first_and_clear_float(float f) {
    return _mm_set_ss(f);
}

static inline_only __simd128_float simd128_set_zero_float() {
    return _mm_setzero_ps();
}

static inline_only float simd128_get_float(__simd128_float s) {
    return _mm_cvtss_f32(s);
}

// Load 16-byte-aligned float data.

static inline_only __simd128_float simd128_load_float(const float *fp) {
    return _mm_load_ps(fp);
}

// Store 16-byte-aligned float data.

static inline_only void simd128_store_float(float *fp, __simd128_float s) {
    _mm_store_ps(fp, s);
}

// Load one float into the lowest-order element. The other elements are filled with
// zero bits (not 0.0f float values).

static inline_only __simd128_float simd128_load_first_float(const float *fp) {
    return _mm_load_ps(fp);
}

static inline_only void simd128_store_first_float(float *fp, __simd128_float s) {
    _mm_store_ss(fp, s);
}

static inline_only __simd128_float simd128_mul_float(__simd128_float s1, __simd128_float s2) {
    return _mm_mul_ps(s1, s2);
}

static inline_only __simd128_float simd128_div_float(__simd128_float s1, __simd128_float s2) {
    return _mm_div_ps(s1, s2);
}

static inline_only __simd128_float simd128_add_float(__simd128_float s1, __simd128_float s2) {
    return _mm_add_ps(s1, s2);
}

static inline_only __simd128_float simd128_add1_float(__simd128_float s1, __simd128_float s2) {
    return _mm_add_ss(s1, s2);
}

static inline_only __simd128_float simd128_sub_float(__simd128_float s1, __simd128_float s2) {
    return _mm_sub_ps(s1, s2);
}

static inline_only __simd128_float simd128_sub1_float(__simd128_float s1, __simd128_float s2) {
    return _mm_sub_ss(s1, s2);
}

// Approximate reciprocal (1.0f / element[i]) with maximum relative error
// less than 1.5*2^-12.

static inline_only __simd128_float simd128_approximate_reciprocal_float(__simd128_float s) {
    return _mm_rcp_ps(s);
}

// Approximate reciprocal square root (1.0f / sqrtf(element[i])) with maximum relative
// error less than 1.5*2^-12.

static inline_only __simd128_float simd128_approximate_reciprocal_sqrt_float(__simd128_float s) {
    return _mm_rsqrt_ps(s);
}

// Calculate square root (accurately) of four floats.

static inline_only __simd128_float simd128_sqrt_float(__simd128_float s) {
    return _mm_sqrt_ps(s);
}

// int32 operators.

static inline_only __simd128_int simd128_set_int32(int i0, int i1, int i2, int i3) {
    return _mm_set_epi32(i3, i2, i1, i0);
}

static inline_only __simd128_int simd128_set_same_int32(int i) {
    return _mm_set1_epi32(i);
}

static inline_only int simd128_get_int32(__simd128_int s) {
    return _mm_cvtsi128_si32(s);
}

// int32 math operators.

static inline_only __simd128_int simd128_add_int32(__simd128_int s1, __simd128_int s2) {
    return _mm_add_epi32(s1, s2);
}

static inline_only __simd128_int simd128_sub_int32(__simd128_int s1, __simd128_int s2) {
    return _mm_sub_epi32(s1, s2);
}

// Multiply unpacked unsigned integers of s1 and s2 (in words 0 and 2, words 1 and 3
// are ignored) and store the result as two 64-bit unsigned integers.

static inline_only __simd128_int simd128_mul_unpacked_uint32_uint64(
__simd128_int s1, __simd128_int s2) {
    return _mm_mul_epu32(s1, s2);
}

// Multiply unsigned integers in lower half (words 0 and 1) of s1 and s2 and store the
// result as two 64-bit unsigned integers.

static inline_only __simd128_int simd128_mul_uint32_uint64(
__simd128_int s1, __simd128_int s2) {
    return _mm_mul_epu32(
        simd128_select_int32(s1, 0, 0, 1, 1),
        simd128_select_int32(s2, 0, 0, 1, 1));
}

// int64 operators.

static inline_only __simd128_int simd128_set_int64(int64_t i0, int64_t i1) {
    return _mm_set_epi64x(i1, i0);
}

static inline_only __simd128_int simd128_set_same_int64(int64_t i) {
    return _mm_set1_epi64x(i);
}

static inline_only int64_t simd128_get_int64(__simd128_int s) {
    return _mm_cvtsi128_si64(s);
}

// General integer operators.

static inline_only __simd128_int simd128_set_zero_int() {
    return _mm_setzero_si128();
}

// Load 16-byte-aligned integer data.

static inline_only __simd128_int simd128_load_int(const int *ip) {
    return _mm_load_si128((const __simd128_int *)ip);
}

// Store 16-byte-aligned integer data.

static inline_only void simd128_store_int(int *ip, __simd128_int s) {
    _mm_store_si128((__simd128_int *)ip, s);
}

// Integer logical operators.

static inline_only __simd128_int simd128_and_int(__simd128_int s1, __simd128_int s2) {
    return _mm_and_si128(s1, s2);
}

static inline_only __simd128_int simd128_andnot_int(__simd128_int s1, __simd128_int s2) {
    return _mm_andnot_si128(s1, s2);
}

static inline_only __simd128_int simd128_or_int(__simd128_int s1, __simd128_int s2) {
    return _mm_or_si128(s1, s2);
}

static inline_only __simd128_int simd128_not_int(__simd128_int s) {
    __simd128_int m_full_mask = simd128_set_same_int32(0xFFFFFFFF);
    return _mm_xor_si128(s, m_full_mask);
}

// Comparison functions. Returns integer vector with full bitmask set for each
// element for which the condition is true.

// float

static inline_only __simd128_int simd128_cmpge_float(__simd128_float s1, __simd128_float s2) {
    return simd128_cast_float_int(_mm_cmpge_ps(s1, s2));
}

static inline_only __simd128_int simd128_cmpgt_float(__simd128_float s1, __simd128_float s2) {
    return simd128_cast_float_int(_mm_cmpgt_ps(s1, s2));
}

static inline_only __simd128_int simd128_cmple_float(__simd128_float s1, __simd128_float s2) {
    return simd128_cast_float_int(_mm_cmple_ps(s1, s2));
}

static inline_only __simd128_int simd128_cmplt_float(__simd128_float s1, __simd128_float s2) {
    return simd128_cast_float_int(_mm_cmplt_ps(s1, s2));
}

static inline_only __simd128_int simd128_cmpeq_float(__simd128_float s1, __simd128_float s2) {
    return simd128_cast_float_int(_mm_cmpeq_ps(s1, s2));
}

static inline_only __simd128_int simd128_cmpne_float(__simd128_float s1, __simd128_float s2) {
    return simd128_cast_float_int(_mm_cmpneq_ps(s1, s2));
}

// int32

static inline_only __simd128_int simd128_cmpge_int32(__simd128_int s1, __simd128_int s2) {
    return simd128_not_int(_mm_cmplt_epi32(s1, s2));
}

static inline_only __simd128_int simd128_cmpgt_int32(__simd128_int s1, __simd128_int s2) {
    return _mm_cmpgt_epi32(s1, s2);
}

static inline_only __simd128_int simd128_cmple_int32(__simd128_int s1, __simd128_int s2) {
    return simd128_not_int(_mm_cmpgt_epi32(s1, s2));
}

static inline_only __simd128_int simd128_cmplt_int32(__simd128_int s1, __simd128_int s2) {
    return _mm_cmplt_epi32(s1, s2);
}

static inline_only __simd128_int simd128_cmpeq_int32(__simd128_int s1, __simd128_int s2) {
    return _mm_cmpeq_epi32(s1, s2);
}

static inline_only __simd128_int simd128_cmpneq_int32(__simd128_int s1, __simd128_int s2) {
    return simd128_not_int(_mm_cmpeq_epi32(s1, s2));
}

// Convert the lower two float elements to doubles occupying the lower and upper half
// of the return value.

static inline_only __simd128_double simd128_convert_float_double(__simd128_float s) {
    return _mm_cvtps_pd(s);
}

// Convert the two doubles to two floats occupying the lower half of the return value.

static inline_only __simd128_float simd128_convert_double_float(__simd128_double s) {
    return _mm_cvtpd_ps(s);
}

// Convert 32-bit signed integers to floats.

static inline_only __simd128_float simd128_convert_int32_float(__simd128_int s) {
    return _mm_cvtepi32_ps(s);
}

// Convert 32-bit signed integers to doubles.

static inline_only __simd128_double simd128_convert_int32_double(__simd128_int s) {
    return _mm_cvtepi32_pd(s);
}

// Convert 32-bit floats to 32-bit signed integers.

static inline_only __simd128_int simd128_convert_float_int32(__simd128_float s) {
    return _mm_cvtps_epi32(s);
}

// Convert 32-bit floats to 32-bit signed integers with truncation.

static inline_only __simd128_int simd128_convert_float_int32_truncate(__simd128_float s) {
    return _mm_cvttps_epi32(s);
}

// Convert 64-bit doubles to 32-bit signed integers.

static inline_only __simd128_int simd128_convert_double_int32_truncate(__simd128_double s) {
    return _mm_cvtpd_epi32(s);
}

// Convert four 32-bit signed integers to four 8-bit signed integers using signed
// saturation. zerosi must be initialized as an integer vector with zeros.

static inline_only __simd128_int simd128_convert_int32_int8_saturate(__simd128_int s,
__simd128_int zerosi) {
    return _mm_packs_epi16(_mm_packs_epi32(s, zerosi), zerosi);
}

// Convert four 32-bit integer masks (each either 0xFFFFFFFF or 0x00000000) to 1-bit
// masks using the highest-order bit of each 32-bit value.

static inline_only int simd128_convert_masks_int32_int1(__simd128_int s) {
     return _mm_movemask_ps(simd128_cast_int_float(s));
}

// Shift float register right by number of 32-bit floats, shifting in zero bits
// (i.e. invalid floating point values) at the high end. n must be constant.

static inline_only __simd128_float simd128_shift_right_float(__simd128_float s, const int n) {
    return _mm_castsi128_ps(
        _mm_srli_si128(_mm_castps_si128(s), n * 4));
}

// Shift 128-bit integer right by n * 8 bits (n bytes).

static inline_only __simd128_int simd128_shift_right_bytes_int(__simd128_int s, const int n) {
    return _mm_srli_si128(s, n);
}

// Shift 32-bit unsigned integers right by n bits.

static inline_only __simd128_int simd128_shift_right_uint32(__simd128_int s, const int n) {
    return _mm_srli_epi32(s, n);
}

// Shift 16-bit unsigned integers right by n bits.

static inline_only __simd128_int simd128_shift_right_uint16(__simd128_int s, const int n) {
    return _mm_srli_epi16(s, n);
}

// Shift 32-bit signed integers right left by n bits.

static inline_only __simd128_int simd128_shift_right_int32(__simd128_int s, const int n) {
    return _mm_srai_epi32(s, n);
}

// Shift float register left by number of 32-bit floats, shifting in zero bits
// (i.e. invalid floating point values). n must be constant.

static inline_only __simd128_float simd128_shift_left_float(__simd128_float s, const int n) {
    return _mm_castsi128_ps(
        _mm_slli_si128(_mm_castps_si128(s), n * 4));
}

// Shift 128-bit integer left by n * 8 bits (n bytes).

static inline_only __simd128_int simd128_shift_left_bytes_int(__simd128_int s, const int n) {
    return _mm_slli_si128(s, n);
}

// Shift 32-bit signed/unsigned integers left by n bits.

static inline_only __simd128_int simd128_shift_left_int32(__simd128_int s, const int n) {
    return _mm_slli_epi32(s, n);
}

// Shift 16-bit signed/unsigned integers left by n bits.

static inline_only __simd128_int simd128_shift_left_int16(__simd128_int s, const int n) {
    return _mm_slli_epi16(s, n);
}

// Calculate the mimimum of each component of two vectors.

static inline_only __simd128_float simd128_min_float(__simd128_float s1, __simd128_float s2) {
     return _mm_min_ps(s1, s2);
}

// Calculate the maximum of each component of two vectors.

static inline_only __simd128_float simd128_max_float(__simd128_float s1, __simd128_float s2) {
     return _mm_max_ps(s1, s2);
}

// Calculate the mimimum of the first component of two vectors.

static inline_only __simd128_float simd128_min1_float(__simd128_float s1, __simd128_float s2) {
     return _mm_min_ss(s1, s2);
}

// Calculate the maximum of the first component of two vectors.

static inline_only __simd128_float simd128_max1_float(__simd128_float s1, __simd128_float s2) {
     return _mm_max_ss(s1, s2);
}

#ifdef DST_USE_SSE3

// Horizontally add pairs of elements over two four-float vectors and return
// in a single vector. Generally inefficient even when available as a single instruction
// (SSE3).

static inline_only __simd128_float simd128_horizontal_add2_float(__simd128_float s1,
__simd128_float s2) {
    return _mm_hadd_ps(s1, s2);
}

#endif

// Set only the last component, preserving the other components.

static inline_only __simd128_float simd128_set_last_float(__simd128_float s, float f) {
    __simd128_float m_f = simd128_cast_int_float(
        simd128_shift_left_bytes_int(
            simd128_cast_float_int(simd128_set_first_and_clear_float(f)),
            12)
        );
    __simd128_int m_mask_012x = simd128_shift_right_bytes_int(
        simd128_set_same_int32(0xFFFFFFFF), 4);
    return simd128_cast_int_float(simd128_or_int(
        simd128_and_int(m_mask_012x, simd128_cast_float_int(s)),
        simd128_andnot_int(m_mask_012x, simd128_cast_float_int(m_f))
        ));
}

static inline_only __simd128_float simd128_set_last_zero_float(__simd128_float s) {
    __simd128_float m_f = simd128_set_zero_float();
    __simd128_int m_mask_012x = simd128_shift_right_bytes_int(
        simd128_set_same_int32(0xFFFFFFFF), 4);
    return simd128_cast_int_float(simd128_or_int(
        simd128_and_int(m_mask_012x, simd128_cast_float_int(s)),
        simd128_andnot_int(m_mask_012x, simd128_cast_float_int(m_f))
        ));
}

// Load three floats without accessing memory beyond them.

static inline_only __simd128_float simd128_load3_float(const float *f) {
    // Take care not to read beyond the allocated data structure.
    __simd128_float m_zeros = simd128_set_zero_float();
    // temp1: Load v.x and v.y into the lower two words (with 0.0f for the rest),
    // temp2: Load v.z into the low word (with 0.0f for the rest).
    // Merge temp1 and temp2 so that (v.x, v.y, v.z, 0.0f) is returned.
    return simd128_merge_float(
        _mm_loadl_pi(m_zeros, (const __m64 *)f),
        simd128_load_first_float(&f[2]),
        0, 1, 0, 1
        );

}

// Store three floats without accessing memory beyond the stored elements.

static inline_only void simd128_store3_float(float *f, __simd128_float m_v) {
    // Take care not to write beyond the allocated data structure.
    _mm_storel_pi((__m64 *)f, m_v);
    simd128_store_first_float(&f[2], simd128_shift_right_float(m_v, 2));
}

// Symmetrically transpose a 4x4 matrix.

// In place.

static inline_only void simd128_transpose4_float(__simd128_float& row0,
__simd128_float& row1, __simd128_float& row2, __simd128_float& row3) {
   _MM_TRANSPOSE4_PS(row0, row1, row2, row3);
}

// Put result in different registers.

static inline_only void simd128_transpose4to4_float(const __simd128_float row0,
const __simd128_float& row1, const __simd128_float& row2, const __simd128_float row3,
__simd128_float& m_result_v0, __simd128_float& m_result_v1, __simd128_float& m_result_v2,
__simd128_float& m_result_v3) {
   m_result_v0 = row0;
   m_result_v1 = row1;
   m_result_v2 = row2;
   m_result_v3 = row3;
   _MM_TRANSPOSE4_PS(m_result_v0, m_result_v1, m_result_v2, m_result_v3);
}

// Transpose four three-float vectors, and store result in three four-float vectors.

static inline_only void simd128_transpose4to3_float(const __simd128_float m_v0,
const __simd128_float m_v1, const __simd128_float m_v2, const __simd128_float m_v3,
__simd128_float& m_result_x, __simd128_float& m_result_y, __simd128_float& m_result_z) {
    __simd128_float m_tmp0, m_tmp1, m_tmp2, m_tmp3;
    // tmp0 = (v0.x, v1.x, v0.y, v1.y)
    m_tmp0 = _mm_unpacklo_ps(m_v0, m_v1);
    // tmp1 = (v2.x, v2.x, v3.y, v3.y)
    m_tmp1 = _mm_unpacklo_ps(m_v2, m_v3);
    // tmp2 = (v0.z, v1.z, v0.w, v1.w)
    m_tmp2 = _mm_unpackhi_ps(m_v0, m_v1);
    // tmp3 = (v2.z, v2.z, v3.w, v3.w)
    m_tmp3 = _mm_unpackhi_ps(m_v2, m_v3);
    m_result_x = _mm_movelh_ps(m_tmp0, m_tmp1);
    m_result_y = _mm_movehl_ps(m_tmp1, m_tmp0);
    m_result_z = _mm_movelh_ps(m_tmp2, m_tmp3);
}

// Transpose three four-float vectors, and store result in four three-float vectors.

static inline_only void simd128_transpose3to4_float(const __simd128_float m_v0,
const __simd128_float m_v1, const __simd128_float m_v2,
__simd128_float& m_result_v0, __simd128_float& m_result_v1, __simd128_float& m_result_v2,
__simd128_float& m_result_v3) {
    __simd128_float m_tmp0, m_tmp1, m_tmp2, m_tmp3;
    __simd128_float m_zeros = simd128_set_zero_float();
    // tmp0 = (v0[0], v1[0], v0[1], v1[1])
    m_tmp0 = _mm_unpacklo_ps(m_v0, m_v1);
    // tmp1 = (v2[0], 0.0f, v2[1], 0.0f)
    m_tmp1 = _mm_unpacklo_ps(m_v2, m_zeros);
    // tmp2 = (v0[2], v1[2], v0[3], v1[3])
    m_tmp2 = _mm_unpackhi_ps(m_v0, m_v1);
    // tmp3 = (v2[2], 0.0f, v2[3], 0.0f)
    m_tmp3 = _mm_unpackhi_ps(m_v2, m_zeros);
    // result_v0 = (v0[0], v1[0], v2[0], 0.0f)
    m_result_v0 = _mm_movelh_ps(m_tmp0, m_tmp1);
    // result_v1 = (v0[1], v1[1], v2[1], 0.0f);
    m_result_v1 = _mm_movehl_ps(m_tmp1, m_tmp0);
    // result_v2 = (v0[2], v1[2], v2[2], 0.0f);
    m_result_v2 = _mm_movelh_ps(m_tmp2, m_tmp3);
    // result_v3 = (v0[3], v1[3], v2[3], 0.0f);
    m_result_v3 = _mm_movehl_ps(m_tmp3, m_tmp2);
}

// Double precision functions.

static inline_only __simd128_int simd128_cast_double_int(__simd128_double s) {
    return _mm_castpd_si128(s);
}

static inline_only __simd128_double simd128_cast_int_double(__simd128_int s) {
    return _mm_castsi128_pd(s);
}

static inline_only __simd128_double simd128_set_double(double d0, double d1) {
    return _mm_set_pd(d1, d0);
}

static inline_only __simd128_double simd128_set_same_double(double d) {
    return _mm_set1_pd(d);
}

// Set lower order double, and clear (zero bits) upper half of return value.

static inline_only __simd128_double simd128_set_first_and_clear_double(double d) {
    return _mm_set_sd(d);
}

static inline_only __simd128_double simd128_set_zero_double() {
    return _mm_setzero_pd();
}

static inline_only double simd128_get_double(__simd128_double s) {
    return _mm_cvtsd_f64(s);
}

// Set lower and higher double to either existing lower or higher double.

static inline_only __simd128_double simd128_select_double(__simd128_double s1,
unsigned int double0, unsigned int double1) {
    return simd128_cast_int_double(simd128_select_int32(simd128_cast_double_int(s1),
        double0 * 2, double0 * 2 + 1, double1 * 2, double1 * 2 + 1));
}

static inline_only __simd128_double simd128_add_double(__simd128_double s1, __simd128_double s2) {
    return _mm_add_pd(s1, s2);
}

static inline_only __simd128_double simd128_mul_double(__simd128_double s1, __simd128_double s2) {
    return _mm_mul_pd(s1, s2);
}

static inline_only __simd128_double simd128_div_double(__simd128_double s1, __simd128_double s2) {
    return _mm_div_pd(s1, s2);
}

// Calculate square root (accurately) of two doubles.

static inline_only __simd128_double simd128_sqrt_double(__simd128_double s) {
    return _mm_sqrt_pd(s);
}

#endif // defined(__DST_SSE2_H__)

