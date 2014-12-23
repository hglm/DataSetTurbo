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

#ifndef __DST_SIMD_H__
#define __DST_SIMD_H__

#include "dstConfig.h"

// This header file defines inline SIMD accelerator vector processing
// primitives using compiler intrinsics. The functions are only included
// if SIMD capability is detected and DST_NO_SIMD is not defined.
//
// A common set of SIMD primitives for 128-bit SIMD vector registers, shared between
// platforms, is defined with certain features depending on the platform. Most
// primitives map to a single SIMD instruction, while others are emulated using
// multiple instructions, depending on on the platform.

// Proving these functions inline has significant benefits; they can be
// integrated into any pure or mixed SIMD processing function, which will
// enable the compiler to automatically mix/shuffle the code for advanced
// scheduling and register allocation optimizations.
//
// Data types:
//
// __simd128_float:  128-bit vector register with four 32-bit floats.
// __simd128_double: 128-bit vector register with two 64-bit double precision floats.
// __simd128_int:    128-bit vector register holding integer data.
//
// For most purposes and hardware implementations, register type are interchangable
// using provided cast functions which do not generate any instructions.
//
// The function names have the following form:
//
// simd128_<operator description>_<element_data_type>(operands)
//
// where <element_data_type> determines the size of the elements operated on and
// is one of the following:
//
// float:    32-bit float elements (four per register).
// double:   64-bit double elements (two per register).
// int32:    32-bit signed integer elements (four per register).
// uint32:   32-bit unsigned integer elements (four per register). When signedness
//           does not matter, only a primitive ending with _int32 is provided.
// int:      Generic integer data type, provided for integer primitives for which
//           the element data type does not matter.
//
// Examples:
//
// Declare three SIMD float register variables:
//     __simd128_float fv1, fv2, fv3;
// Add every corresponding component of fv1 and fv2, and store the result in fv3:
//     fv3 = simd128_add_float(fv1, fv2);
// Multiply components:
//     fv3 = simd128_mul_float(fv1, fv2);
// Load a float factor from 16-byte aligned memory:
//     fv1 = simd128_load_float(mem_pointer);
// Shift right by 1 float (so that fourth element becomes undefined, and every other
// element is moved one slot lower):
//     fv2 = simd128_shift_right_float(fv1);
// An identical shift could also be achieved using integer primitives and casts:
//     fv2 = simd128_cast_int_float(
//         simd128_shift_right_bytes_int(simd128_cast_float_int(fv1), 4)
//         );
// Load a constant into the first element, invalidating the other elements (bits cleared).
//     fv2 = simd128_set_first_and_clear_float(1.0f);
// Load the same constant into all four elements:
//     fv2 = simd128_set_same_float(1.0f);
// Set all four elements:
//     fv2 = simd128_set_float(1.0f, 2.0f, 3.0f, 4.0f);
// Note that the function definitions always assume the first argument is the lowest
// order element (this is opposite of the argument order used in some actual SSE
// intrinsic functions).
// Reverse the element order:
//     fv2 = simd128_select_float(fv1, 3, 2, 1, 0);
//     __simd128_int iv1, iv2, iv3;
// Add 32-bit integer components:
//     iv3 = simd128_add_int32(iv1, iv2);
//     __simd128_double dv1;
// Convert two floats in the first two elements of fv1 to two doubles:
//     dv1 = simd128_convert_float_double(fv1);
//

// Set macro to generate unique function identifiers for each SIMD type this file is compiled with.

#ifdef DST_NO_SIMD
#define SIMD_FUNC(f) f ## NoSIMD
#else
#if defined(DST_SIMD_MODE_SSE2)
#define SIMD_FUNC(f) f ## SSE2
#elif defined(DST_SIMD_MODE_SSE3)
#define SIMD_FUNC(f) f ## SSE3
#elif defined(DST_SIMD_MODE_SSSE3)
#define SIMD_FUNC(f) f ## SSSE3
#elif defined(DST_SIMD_MODE_SSE4A)
#define SIMD_FUNC(f) f ## SSE4A
#elif defined(DST_SIMD_MODE_SSE41)
#define SIMD_FUNC(f) f ## SSE41
#elif defined(DST_SIMD_MODE_SSE42)
#define SIMD_FUNC(f) f ## SSE42
#elif defined(DST_SIMD_MODE_AVX)
#define SIMD_FUNC(f) f ## AVX
#elif defined(DST_SIMD_MODE_NEON)
#define SIMD_FUNC(f) f ## NEON
#else
#endif
#endif
#define SIMD_VAR(f) SIMD_FUNC(f)


#if defined(_MSC_VEC)
/* Microsoft C/C++-compatible compiler */
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
/* GCC-compatible compiler, targeting x86/x86-64 */
#include <x86intrin.h>
#elif defined(__GNUC__) && defined(__ARM_NEON__)
/* GCC-compatible compiler, targeting ARM with NEON */
#include <arm_neon.h>
#elif defined(__GNUC__) && defined(__IWMMXT__)
/* GCC-compatible compiler, targeting ARM with WMMX */
#include <mmintrin.h>
#endif

// Set special SIMD functinons that are implemented. They may not actually be available
// depending the capabilities of the machine the library is used on.

#ifdef __SSE2__
// PC architecture (based on SSE2)
#define SIMD_HAVE_TRANSPOSE_4TO3
#define SIMD_HAVE_TRANSPOSE_3TO4
// Accurate square root calculation
#define SIMD_HAVE_ACCURATE_SQRT
#ifdef __SSE3__
// Horizontal pair-wise addition instruction.
#define SIMD_HAVE_HORIZONTAL_ADD2
#endif
#if defined(DST_SIMD_MODE_SSSE3) || defined (DST_FIXED_SSSE3)
// For CPUs where SSSE3 is likely to be the SSE level of choice, such as the Intel Core 2
// architecture, unaligned loads are very expensive, so we load aligned and unpack.
#define SIMD_UNALIGNED_LOADS_EXPENSIVE
#endif
#if defined(__FMA4__) || defined(__FMA__) || defined(__AVX__)
// 256-bit SIMD registers.
#define SIMD_HAVE_256
#endif
#if defined(__FMA4__) || defined(__FMA__)
// Fused multiply-add.
#define SIMD_HAVE_FMA
#endif

#else // Not SSE2-based
#ifdef __ARM_NEON_FP
// When implemented, define feature flags for NEON.
#else
// No SIMD.
#endif
#endif

// SIMD utility features that depend on lower-level features.

#if defined(SIMD_HAVE_TRANSPOSE_4TO3) && defined(SIMD_HAVE_TRANSPOSE_3TO4)
#define SIMD_HAVE_MATRIX4X3_VECTOR_MULTIPLICATION
#endif

// Define a unifying set of SIMD intrinsic functions.

#ifdef __SSE2__

// PC class processor with SSE2 enabled. Optionally use additional features
// such as SSE3 and later extensions.

#include "dstSIMDSSE2.h"

#else
#error __SSE2__ not defined.
#endif

#ifdef __ARM_NEON_FP

// Not yet implemented.

#endif

// Define shared set of generic SIMD functions shared between different platforms
// that are implemented using lower-level SIMD functions.
//
// These include:
//
// __simd128_float simd128_horizonal_min_float(__simd128_float s)
// __simd128_float simd128_horizonal_max_float(__simd128_float s)
// __simd128_float simd128_horizontal_add2_float(__simd128_float s1, _simd128_float s2) *
// __simd128_float simd128_horizontal_add4_float(__simd128_float s)
//
// * May already be available as a low-level instruction for a hardware platform.
//
//
// Utility function to count bits (not actually uses SIMD instructions).

static const char sre_simd_internal_bit_count4[16] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4
};

static DST_INLINE_ONLY int simd_count_bits_int4(int i) {
    return sre_simd_internal_bit_count4[i];
}

// Calculate the minimum of the four components of a vector, and store result in the
// first component.

static DST_INLINE_ONLY __simd128_float simd128_horizontal_min_float(__simd128_float s) {
    __simd128_float m_shifted_float1, m_shifted_float2, m_shifted_float3;
    __simd128_float m_min_01, m_min_23;
    m_shifted_float1 = simd128_shift_right_float(s, 1);
    m_shifted_float2 = simd128_shift_right_float(s, 2);
    m_shifted_float3 = simd128_shift_right_float(s, 3);
    m_min_01 = simd128_min1_float(s, m_shifted_float1);
    m_min_23 = simd128_min1_float(m_shifted_float2, m_shifted_float3);
    return simd128_min1_float(m_min_01, m_min_23);
}

// Calculate the maximum of the four components of a vector, and store result in the
// first component.

static DST_INLINE_ONLY __simd128_float simd128_horizontal_max_float(__simd128_float s) {
    __simd128_float m_shifted_float1, m_shifted_float2, m_shifted_float3;
    __simd128_float m_max_01, m_max_23;
    m_shifted_float1 = simd128_shift_right_float(s, 1);
    m_shifted_float2 = simd128_shift_right_float(s, 2);
    m_shifted_float3 = simd128_shift_right_float(s, 3);
    m_max_01 = simd128_max1_float(s, m_shifted_float1);
    m_max_23 = simd128_max1_float(m_shifted_float2, m_shifted_float3);
    return simd128_max1_float(m_max_01, m_max_23);
}

#ifndef SIMD_HAVE_HORIZONTAL_ADD2

// Implement higher-level pair-wise addition when not available as a low-level
// primitive.

static DST_INLINE_ONLY __simd128_float simd128_horizontal_add2_float(__simd128_float s1,
__simd128_float s2) {
    __simd128_float e0 = simd128_merge_float(s1, s2, 0, 2, 0, 2);
    __simd128_float e1 = simd128_merge_float(s1, s2, 1, 3, 1, 3);
    return simd128_add_float(e0, e1);
}

#endif

#ifndef SIMD_HAVE_HORIZONTAL_ADD4

// When not already defined as a primitive, implement function to horizontally add
// four elements and store in the lowest order element. Generally, this operation is
// not efficient even when achievable with two hadd instructions (SSE3), and should
// be avoided when possible.

static DST_INLINE_ONLY __simd128_float simd128_horizontal_add4_float(__simd128_float s) {
#ifdef SIMD_HAVE_HORIZONTAL_ADD2
    __simd128_float m_zeros = simd128_set_zero_float();
    return simd128_horizontal_add2_float(
        simd128_horizontal_add2_float(s, m_zeros),
        m_zeros);
#else
     __simd128_float m_shifted_float1, m_shifted_float2, m_shifted_float3;
    __simd128_float m_sum_01, m_sum_23;
    m_shifted_float1 = simd128_shift_right_float(s, 1);
    m_shifted_float2 = simd128_shift_right_float(s, 2);
    m_shifted_float3 = simd128_shift_right_float(s, 3);
    m_sum_01 = simd128_add1_float(s, m_shifted_float1);
    m_sum_23 = simd128_add1_float(m_shifted_float2, m_shifted_float3);
    return simd128_add1_float(m_sum_01, m_sum_23);
#endif
}

#endif // !defined(SIMD_HAVE_HORIZONTAL_ADD4)

// Calculate four dot products vertically (with the matching coordinates of each vector already
// packed into the same SIMD registers).

static DST_INLINE_ONLY __simd128_float simd128_four_dot_products_vector4_vertical_float(
const __simd128_float& m_v1_x, const __simd128_float& m_v1_y, const __simd128_float& m_v1_z,
const __simd128_float& m_v1_w, const __simd128_float& m_v2_x, const __simd128_float& m_v2_y,
const __simd128_float& m_v2_z, const __simd128_float& m_v2_w) {
	// Note: This is a good place to take advantage of FMA (fused multiply-add)
	// instructions in recent SIMD implementations.
#ifdef SIMD_HAVE_FMA
	__simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
	__simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
	__simd128_float m_dot_xy = simd128_multiply_add_float(m_v1_y, m_v2_y, m_dot_x);
	__simd128_float m_dot_zw = simd128_multiply_add_float(m_v1_w, m_w_y, m_dot_z);
	return simd128_add_float(m_dot_xy, m_dot_zw);
#else
	__simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
	__simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
	__simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
	__simd128_float m_dot_w = simd128_mul_float(m_v1_w, m_v2_w);
	return simd128_add_float(
		simd128_add_float(m_dot_x, m_dot_y),
		simd128_add_float(m_dot_z, m_dot_w)
		);
#endif
}

// For vectors with three components.

static DST_INLINE_ONLY __simd128_float simd128_four_dot_products_vector3_vertical_float(
const __simd128_float& m_v1_x, const __simd128_float& m_v1_y, const __simd128_float& m_v1_z,
const __simd128_float& m_v2_x, const __simd128_float& m_v2_y, const __simd128_float& m_v2_z) {
	__simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
	__simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
	__simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
	return simd128_add_float(
	        simd128_add_float(m_dot_x, m_dot_y),
	        m_dot_z
	        );
}

// Dot products of a point with a four-component vector. The fourth component of the point is
// implicitly 1.0f.

static DST_INLINE_ONLY __simd128_float simd128_four_dot_products_point3_vector4_vertical_float(
const __simd128_float& m_v1_x, const __simd128_float& m_v1_y, const __simd128_float& m_v1_z,
const __simd128_float& m_v2_x, const __simd128_float& m_v2_y, const __simd128_float& m_v2_z,
const __simd128_float& m_v2_w) {
        __simd128_float m_dot_x = simd128_mul_float(m_v1_x, m_v2_x);
        __simd128_float m_dot_y = simd128_mul_float(m_v1_y, m_v2_y);
        __simd128_float m_dot_z = simd128_mul_float(m_v1_z, m_v2_z);
        // m_v1_w would contain only 1.0f so m_dot_w is equal to m_v2_w.
        return simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y),
            simd128_add_float(m_dot_z, m_v2_w)
            );
}

// Multiply a matrix with a four-component vector, resulting in a four-component vector.

static DST_INLINE_ONLY void simd128_multiply_matrix4x4_vector4(
__simd128_float m_row0, __simd128_float m_row1, __simd128_float m_row2, __simd128_float m_row3,
__simd128_float m_v, __simd128_float m_result) {
        __simd128_float m_mul0  = simd128_mul_float(m_row0, m_v);
        __simd128_float m_mul1  = simd128_mul_float(m_row1, m_v);
        __simd128_float m_mul2  = simd128_mul_float(m_row2, m_v);
        __simd128_float m_mul3  = simd128_mul_float(m_row3, m_v);
        m_result =
            simd128_horizontal_add2_float(
                simd128_horizontal_add2_float(m_mul0, m_mul1),
                simd128_horizontal_add2_float(m_mul2, m_mul3));
}

// Multiply a 4x3 matrix with a four-component vector, resulting in a four-component vector.
// The fourth row of the 4x3 matrix is implicitly (0.0, 0.0, 0.0, 1.0). The w component
// of the vector is usually 0.0 or 1.0.

static DST_INLINE_ONLY void simd128_multiply_matrix4x3_vector4(
__simd128_float m_row0, __simd128_float m_row1, __simd128_float m_row2,
__simd128_float m_v, __simd128_float m_result) {
        __simd128_float m_mul0  = simd128_mul_float(m_row0, m_v);
        __simd128_float m_mul1  = simd128_mul_float(m_row1, m_v);
        __simd128_float m_mul2  = simd128_mul_float(m_row2, m_v);
        __simd128_float m_zeros = simd128_set_zero_float();
        m_result =
            simd128_horizontal_add2_float(
                simd128_horizontal_add2_float(m_mul0, m_mul1),
                simd128_horizontal_add2_float(m_mul2, m_zeros));
}

#endif // defined(__DST_SIMD_H__)

