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

#ifndef __DST_MATH_H__
#define __DST_MATH_H__

// The header file defines several convenient small inline math functions.

static inline float dstSqrt(float x) {
	return sqrtf(x);
}

static inline double dstSqrt(double x) {
	return sqrt(x);
}

static inline float dstInvSqrt(float x) {
	return (1.0f / sqrtf(x));
}

static inline double dstInvSqrt(double x) {
	return (1.0d / sqrt(x));
}

// Define approximate equality inline functions, useful for geometrical
// calculations.

#define EPSILON_DEFAULT 0.0001

template <class T>
static inline bool AlmostEqual(T x, T y) {
    return (x >= y - EPSILON_DEFAULT) && (x <= y + EPSILON_DEFAULT);
}

template <class T>
static inline bool AlmostEqual(T x, T y, T epsilon) {
    return (x >= y - epsilon) && (x <= y + epsilon);
}

// Also define inline functions for general use such as square and the minimum and
// maximum of two or three values.

static inline float sqrf(float x) {
    return x * x;
}

static inline float minf(float x, float y) {
    if (x < y)
        return x;
    return y;
}

static inline float min3f(float x, float y, float z) {
    float m = x;
    if (y < m)
        m = y;
    if (z < m)
        m = z;
    return m;
}

static inline float maxf(float x, float y) {
    if (y > x)
        return y;
    return x;
}

static inline float max3f(float x, float y, float z) {
    float m = x;
    if (y > m)
        m = y;
    if (z > m)
        m = z;
    return m;
}

static inline float clampf(float x, float min_value, float max_value) {
    if (x < min_value)
        return min_value;
    if (x > max_value)
        return max_value;
    return x;
}   

static inline int mini(int x, int y) {
    if (x < y)
        return x;
    return y;
}

static inline int maxi(int x, int y) {
    if (y > x)
        return y;
    return x;
}

static inline double mind(double x, double y) {
    if (x < y)
        return x;
    return y;
}

static inline double maxd(double x, double y) {
    if (y > x)
        return y;
    return x;
}

// The following size global (non-class member) functions efficiently calculate
// log2 of an integer value, rounded down to an integer, as well as providing the
// number of bits needed to store a certain range of integers.

#ifdef DST_BITS_NEEDED_VALIDATION_CHECK
#define DST_VALIDATE_BITS_NEEDED(n, n_bits) dstValidateBitsNeeded(n, n_bits)

static void dstValidateBitsNeeded(unsigned int n, unsigned int n_bits) {
    unsigned int mask = ((unsigned int)1 << n_bits) - 1;
    if ((n & mask) != (n & (~((unsigned int)1 << n_bits))) ||
    (n != ((unsigned int)1 << n_bits) && (n & (~mask)) != 0))
        printf("Warning: bits needed = %d is not optimal for n = %d.\n",
            n_bits, n);
}

#else
#define VALIDATE_BITS_NEEDED(n, n_bits)
#endif

// Calculate floor(log2(n))). For a power of two, this is equivalent to the
// number of bits needed to represent the range 0 to n - 1. For a non-power-of-two,
// the return value is one less than the number of bits needed to represent
// the range 0 to n - 1.
DST_API static inline unsigned int dstCalculateLog2(unsigned int n) {
        // Set shift to 16 if bits 15-31 are non-zero, zero otherwise.
        unsigned int shift = (((n >> 16) + 0xFFFF) & 0x10000) >> 12;
        unsigned int bits = n >> shift;
        // Add 8 to shift if bits 8-15 of the highest non-zero half-word found previously
        // are non-zero.
        unsigned char byte_shift = (((bits >> 8) + 0xFF) & 0x100) >> 5;
        shift += byte_shift;
        bits >>= byte_shift;
        // Add 4 to shift if bits 4-7 of the highest non-zero byte found previously
        // are non-zero.
        unsigned char nibble_shift = (((bits >> 4) + 0xF) & 0x10) >> 2;
        shift += nibble_shift;
        bits >>= nibble_shift;
        // Add 2 to shift if bits 2-3 of the highest non-zero nibble found previously
        // are non-zero.
        unsigned char pair_shift = (((bits >> 2) + 0x3) & 0x4) >> 1;
        shift += pair_shift;
        bits >>= pair_shift;
        // Add 1 to shift if bit 1 of the highest non-zero pair found previously
        // is non-zero.
        shift += bits >> 1;
        return shift;
}

// Calculate number of bits needed for an integer range of n (log2(n - 1) + 1).
DST_API static inline unsigned int dstCalculateBitsNeeded(unsigned int n) {
        unsigned int shift = dstCalculateLog2(n);
        // If n is not a power of two, one more bit is needed.
        // Rely on the fact that bit 31 will be set when subtracting n from 2 ^ shift
        // and n is not power of two.
        shift += ((1 << shift) - n) >> 31;
//        VALIDATE_BITS_NEEDED(n, shift);
	return shift;
}

// Calculate floor(log2(n)) when n is guaranteed to be <= 256, so that the
// return value will be <= 8. Undefined for n > 256.
DST_API static inline unsigned int dstCalculateLog2Max256(unsigned int n) {
        // Set shift to 4 if bits 4-7 of the highest non-zero byte found previously
        // are non-zero.
        unsigned int shift = (((n >> 4) + 0xF) & 0x10) >> 2;
        unsigned int bits = n >> shift;
        // Add 2 to shift if bits 2-3 of the highest non-zero nibble found previously
        // are non-zero.
        unsigned char pair_shift = (((bits >> 2) + 0x3) & 0x4) >> 1;
        shift += pair_shift;
        bits >>= pair_shift;
        // Add 1 to shift if bit 1 of the highest non-zero pair found previously
        // is non-zero.
        shift += bits >> 1;
        // When n = 2^16, set shift to 16 (shift will still be zero).
        shift += (n & 0x10000) >> 12;
        return shift;
    }

// Calculate number of bits needed for an integer range of n (log2(n - 1) + 1),
// when n <= 256.
DST_API static inline unsigned int dstCalculateBitsNeededMax256(unsigned int n) {
	unsigned int shift = dstCalculateLog2Max256(n);
        // If n is not a power of two, one more bit is needed.
        // Rely on the fact that bit 31 will be set when subtracting n from 2 ^ shift
        // and n is not power of two.
        shift += ((1 << shift) - n) >> 31;
        VALIDATE_BITS_NEEDED(n, shift);
	return shift;
}

// Calculate floor(log2(n)) when n is guaranteed to be <= 2^16, so that the
// return value will be <= 16. Undefined for n > 2^16.
DST_API static inline unsigned int dstCalculateLog2Max65536(unsigned int n) {
        // Set shift to 8 if bits 7-15 are non-zero.
        unsigned int shift = (((n >> 8) + 0xFF) & 0x100) >> 5;
        unsigned bits = n >> shift;
        // Add 4 to shift if bits 4-7 of the highest non-zero byte found previously
        // are non-zero.
        unsigned char nibble_shift = (((bits >> 4) + 0xF) & 0x10) >> 2;
        shift += nibble_shift;
        bits >>= nibble_shift;
        // Add 2 to shift if bits 2-3 of the highest non-zero nibble found previously
        // are non-zero.
        unsigned char pair_shift = (((bits >> 2) + 0x3) & 0x4) >> 1;
        shift += pair_shift;
        bits >>= pair_shift;
        // Add 1 to shift if bit 1 of the highest non-zero pair found previously
        // is non-zero.
        shift += bits >> 1;
        // When n = 256, set shift to 8 (shift will still be zero).
        shift += (n & 0x100) >> 5;
        return shift;
    }

// Calculate number of bits needed for an integer range of n (log2(n - 1) + 1),
// when n <= 65536.
DST_API static inline unsigned int dstCalculateBitsNeededMax65536(unsigned int n) {
        unsigned int shift = dstCalculateLog2Max65536(n);
        // If n is not a power of two, one more bit is needed.
        // Rely on the fact that bit 31 will be set when subtracting n from 2 ^ shift
        // and n is not power of two.
        shift += ((1 << shift) - n) >> 31;
        VALIDATE_BITS_NEEDED(n, shift);
	return shift;
}

DST_API inline uint16_t dstCalculateLog2(uint16_t n) {
        return (uint16_t)dstCalculateLog2Max65536(n);
}
DST_API inline uint8_t dstCalculateLog2(uint8_t n) {
        return (uint8_t)dstCalculateLog2Max256(n);
}
DST_API inline uint32_t dstCalculateLog2(uint64_t n) {
        if ((n >> 32) == 0)
            return dstCalculateLog2((uint32_t)n);
        return 32 + dstCalculateLog2(n >> 32);
}
DST_API inline uint16_t dstCalculateBitsNeeded(uint16_t n) {
        return (uint16_t)dstCalculateBitsNeededMax65536(n);
}
DST_API inline uint8_t dstCalculateBitsNeeded(uint8_t n) {
        return (uint8_t)dstCalculateBitsNeededMax256(n);
}

#endif // defined(__DST_MATH_H__)

