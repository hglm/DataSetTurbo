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

#endif // defined(__DST_MATH_H__)
