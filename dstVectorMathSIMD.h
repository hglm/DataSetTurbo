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

#ifndef __DST_VECTOR_MATH_SIMD_H__
#define __DST_VECTOR_MATH_SIMD_H__

#include "dstVectorMath.h"
#include "dstSIMD.h"

// Loading/storing Vector3D/Vector3D/Point3D structures. The implementation
// of Vector3D/Point3D depends on whether the structures are aligned on
// 16-byte boundaries or packed every 12 bytes. Vector4D structures are assumed
// to have 16-byte alignment.

static inline_only __simd128_float simd128_load(const Vector4D *v) {
    return simd128_load_float(&v->x);
}

static inline_only __simd128_float simd128_load(const Vector3D *v) {
    // When Vector3D is stored as 16 bytes (the same amount of space as Vector4D),
    // a standard 16-byte aligned load can be used.
    // Rely on compiler optimization to eliminate the conditional and unused code
    // at compile time.
    if (sizeof(Vector3D) == 16)
        return simd128_load_float(&v->x);
    else
        // Otherwise, take care not to read beyond the allocated data structure.
        return simd128_load3_float(&v->x);
}

// Load a vector and set the fourth component w to 0.0f.

static inline_only __simd128_float simd128_load_and_set_w(const Vector3D *v) {
    // When Vector3D is stored as 16 bytes (the same amount of space as Vector4D),
    // a standard 16-byte aligned load can be used.
    // Rely on compiler optimization to eliminate the conditional and unused code
    // at compile time.
    if (sizeof(Vector3D) == 16) {
        __simd128_float m_v = simd128_load_float(&v->x);
        // The w component as loaded is undefined.
        // Set w to 0.0f.
        __simd128_float m_zeros = simd128_set_zero_float();
        __simd128_int m_mask_012x = simd128_shift_right_bytes_int(
            simd128_set_same_int32(0xFFFFFFFF), 4);
        return simd128_cast_int_float(simd128_or_int(
            simd128_and_int(m_mask_012x, simd128_cast_float_int(m_v)),
            simd128_andnot_int(m_mask_012x, simd128_cast_float_int(m_zeros))
            ));
    }
    else
        // Otherwise, take care not to read beyond the allocated data structure.
        // w is already set to 0.0f by simd128_load3_float.
        return simd128_load3_float(&v->x);
}

static inline_only __simd128_float simd128_load(const Point3D *p) {
    return simd128_load(&p->GetVector3D());
}

// Load a point vector and set the fourth component w to 1.0f.

static inline_only __simd128_float simd128_load_and_set_w(const Point3D *p) {
    __simd128_float m_v;
    if (sizeof(Vector3D) == 16)
         m_v = simd128_load_float(&p->x);
    else
         m_v = simd128_load3_float(&p->x);
    // The w component as loaded is undefined or 0.0f.
    // Set w to 1.0f.
    __simd128_float m_ones = simd128_set_same_float(1.0f);
    __simd128_int m_mask_012x = simd128_shift_right_bytes_int(
        simd128_set_same_int32(0xFFFFFFFF), 4);
    return simd128_cast_int_float(simd128_or_int(
        simd128_and_int(m_mask_012x, simd128_cast_float_int(m_v)),
        simd128_andnot_int(m_mask_012x, simd128_cast_float_int(m_ones))
        ));
}

static inline_only void simd128_store(Vector4D *v, __simd128_float m_v) {
    simd128_store_float(&v->x, m_v);
}

static inline_only void simd128_store(Vector3D *v, __simd128_float m_v) {
    // When Vector3D is stored as 16 bytes (the same amount of space as Vector4D),
    // a standard 16-byte aligned store can be used.
    // Rely on compiler optimization to eliminate the conditional and unused code
    // at compile time.
    if (sizeof(Vector3D) == 16) {
        simd128_store_float(&v->x, m_v);
        return;
    }
    else
        // Otherwise, take care not to write beyond the allocated data structure.
        simd128_store3_float(&v->x, m_v);
}

static inline_only void simd128_store(Point3D *p, __simd128_float m_v) {
    simd128_store(&p->GetVector3D(), m_v);
}

// Calculate four dot products from two vector arrays and return the results in a
// SIMD register.

static inline_only void SIMDCalculateFourDotProductsNoStore(
const Vector4D * __restrict v1, const Vector4D * __restrict v2, __simd128_float& result) {
    simd_four_dot_products_vector4_float(&v1[0].x, &v2[0].x, result);
}

static inline_only void SIMDCalculateFourDotProductsNoStore(
const Vector3D * __restrict v1, const Vector3D * __restrict v2, __simd128_float& result) {
    if (sizeof(Vector3D) == 16)
        simd_four_dot_products_vector3_storage4_float(&v1[0].x, &v2[0].x, result);
    else
        simd_four_dot_products_vector3_storage3_float(&v1[0].x, &v2[0].x, result);
}

static inline_only void SIMDCalculateFourDotProducts(const Vector4D * __restrict v1,
const Vector4D * __restrict v2, float * __restrict dot) {
    __simd128_float m_result;
    SIMDCalculateFourDotProductsNoStore(v1, v2, m_result);
    simd128_store_float(dot, m_result);
}

static inline_only void SIMDCalculateFourDotProducts(const Vector3D * __restrict v1,
const Vector3D * __restrict v2, float * __restrict dot) {
    __simd128_float m_result;
    SIMDCalculateFourDotProductsNoStore(v1, v2, m_result);
    simd128_store_float(dot, m_result);
}

// Calculate n dot products from one vector array and one constant vector,
// and store the result in an array of floats. v1, v2 and dot must generally be
// 16 bytes-aligned.

static inline_only void SIMDCalculateDotProductsWithConstantVector(int n,
const Vector4D * __restrict v1, const Vector4D& __restrict v2, float * __restrict dot) {
    simd_dot_product_nx1_vector4_float(n, &v1[0].x, &v2.x, dot);
}

static inline_only void SIMDCalculateDotProductsWithConstantVector(int n,
const Vector3D * __restrict v1, const Vector4D& __restrict v2, float * __restrict dot) {
    if (sizeof(Vector3D) == 16)
        simd_dot_product_nx1_vector3_storage4_vector4_float(n, &v1[0].x, &v2.x, dot);
    else
        simd_dot_product_nx1_vector3_storage3_float(n, &v1[0].x, &v2.x, dot);
}

static inline_only void SIMDCalculateDotProductsWithConstantVector(int n,
const Point3D * __restrict p1, const Vector4D& __restrict v2, float * __restrict dot) {
    if (sizeof(Vector3D) == 16)
        simd_dot_product_nx1_point3_storage4_vector4_float(n, &p1[0].x, &v2.x, dot);
    else
        simd_dot_product_nx1_point3_storage3_vector4_float(n, &p1[0].x, &v2.x, dot);
}

static inline_only void SIMDCalculateFourDotProductsWithConstantVector(
const Vector4D * __restrict v1, const Vector4D& __restrict v2, float * __restrict dot) {
    SIMDCalculateDotProductsWithConstantVector(4, v1, v2, dot);
}

static inline_only void SIMDCalculateFourDotProductsWithConstantVector(
const Vector3D * __restrict v1, const Vector3D& __restrict v2, float * __restrict dot) {
    SIMDCalculateDotProductsWithConstantVector(4, v1, v2, dot);
}

static inline_only void SIMDCalculateFourDotProductsWithConstantVector(
const Point3D * __restrict p1, const Vector4D& __restrict v2, float * __restrict dot) {
    SIMDCalculateDotProductsWithConstantVector(4, p1, v2, dot);
}

static inline_only void SIMDCalculateDotProductsWithConstantVectorAndCountNegative(int n,
const Point3D * __restrict p1, const Vector4D& __restrict v2, float * __restrict dot,
int& negative_count) {
    if (sizeof(Vector3D) == 16)
        simd_dot_product_nx1_point3_storage4_vector4_and_count_negative_float(
            n, &p1[0].x, &v2.x, dot, negative_count);
    else
        simd_dot_product_nx1_point3_storage3_vector4_and_count_negative_float(
            n, &p1[0].x, &v2.x, dot, negative_count);
}

static inline_only void SIMDCalculateFourDotProductsWithConstantVectorAndCountNegative(
const Point3D * __restrict p1, const Vector4D& __restrict v2, float * __restrict dot,
int& negative_count) {
    SIMDCalculateDotProductsWithConstantVectorAndCountNegative(4, p1, v2, dot,
        negative_count);
}

#endif

