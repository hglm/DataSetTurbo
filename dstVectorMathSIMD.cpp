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

#include "dstVectorMathSIMD.h"

// Vector processing functions that use SIMD. When SIMD is not enabled (DST_USE_SIMD is
// not defined), a C++ implementation is provided.

// Calculate an array of dot products.
// v1, v2, and dot are generally expected to be aligned to a 16-byte boundary.

void CalculateDotProducts(int n, const Vector3D *v1, const Vector3D *v2,
float *dot) {
    int i = 0;
#ifdef DST_USE_SIMD
    if ((((uintptr_t)v1 | (uintptr_t)&v2 | (uintptr_t)dot) & 0xF) == 0 && sizeof(Vector3D) == 16)
        for (; i + 3 < n; i += 4) {
            SIMDCalculateFourDotProducts(&v1[i], &v2[i], &dot[i]);
	}
    else
#endif
    for (; i + 3 < n; i += 4) {
        dot[i] = Dot(v1[i], v2[i]);
        dot[i + 1] = Dot(v1[i + 1], v2[i + 1]);
        dot[i + 2] = Dot(v1[i + 2], v2[i + 2]);
        dot[i + 3] = Dot(v1[i + 3], v2[i + 3]);
    }
    for (; i < n; i++)
        dot[i] = Dot(v1[i], v2[i]);
}

void CalculateDotProducts(int n, const Vector4D *v1, const Vector4D *v2,
float *dot) {
    int i = 0;
#ifdef DST_USE_SIMD
    if ((((uintptr_t)v1 | (uintptr_t)&v2 | (uintptr_t)dot) & 0xF) == 0)
        for (; i + 3 < n; i += 4) {
            SIMDCalculateFourDotProducts(&v1[i], &v2[i], &dot[i]);
	}
    else
#endif
    for (; i + 3 < n; i += 4) {
        dot[i] = Dot(v1[i], v2[i]);
        dot[i + 1] = Dot(v1[i + 1], v2[i + 1]);
        dot[i + 2] = Dot(v1[i + 2], v2[i + 2]);
        dot[i + 3] = Dot(v1[i + 3], v2[i + 3]);
    }
    for (; i < n; i++)
        dot[i] = Dot(v1[i], v2[i]);
}

// Calculate array of dot products of vector array v1 with constant vector v2.

void CalculateDotProductsWithConstantVector(int n, const Vector3D *v1, const Vector3D& v2,
float *dot) {
    int i = 0;
#ifdef DST_USE_SIMD
    if ((((uintptr_t)v1 | (uintptr_t)&v2 | (uintptr_t)dot) & 0xF) == 0 && sizeof(Vector3D) == 16) {
        SIMDCalculateDotProductsWithConstantVector(n, &v1[0], v2, &dot[0]);
        return;
    }
#endif
    for (; i + 3 < n; i += 4) {
        dot[i] = Dot(v1[i], v2);
        dot[i + 1] = Dot(v1[i + 1], v2);
        dot[i + 2] = Dot(v1[i + 2], v2);
        dot[i + 3] = Dot(v1[i + 3], v2);
    }
    for (; i < n; i++)
        dot[i] = Dot(v1[i], v2);
}

void CalculateDotProductsWithConstantVector(int n, const Vector4D *v1, const Vector4D& v2,
float *dot) {
#ifdef DST_USE_SIMD
    if ((((uintptr_t)v1 | (uintptr_t)&v2 | (uintptr_t)dot) & 0xF) == 0) {
        SIMDCalculateDotProductsWithConstantVector(n, &v1[0], v2, &dot[0]);
        return;
    }
#endif
    int i = 0;
    for (; i + 3 < n; i += 4) {
        dot[i] = Dot(v1[i], v2);
        dot[i + 1] = Dot(v1[i + 1], v2);
        dot[i + 2] = Dot(v1[i + 2], v2);
        dot[i + 3] = Dot(v1[i + 3], v2);
    }
    for (; i < n; i++)
        dot[i] = Dot(v1[i], v2);
}

void CalculateDotProductsWithConstantVector(int n, const Point3D *p1, const Vector4D& v2,
float *dot) {
    int i = 0;
#ifdef DST_USE_SIMD
    if ((((uintptr_t)p1 | (uintptr_t)&v2 | (uintptr_t)dot) & 0xF) == 0) {
        SIMDCalculateDotProductsWithConstantVector(n, &p1[i], v2, &dot[i]);
        return;
    }
#endif
    for (; i + 3 < n; i += 4) {
        dot[i] = Dot(p1[i], v2);
        dot[i + 1] = Dot(p1[i + 1], v2);
        dot[i + 2] = Dot(p1[i + 2], v2);
        dot[i + 3] = Dot(p1[i + 3], v2);
    }
    for (; i < n; i++)
        dot[i] = Dot(p1[i], v2);
}

void CalculateDotProductsWithConstantVectorAndCountNegative(int n, const Point3D *p1,
const Vector4D& v2, float *dot, int& count) {
    int i = 0;
    count = 0;
#ifdef DST_USE_SIMD
    if ((((uintptr_t)p1 | (uintptr_t)&v2 | (uintptr_t)dot) & 0xF) == 0 && sizeof(Point3D) == 16) {
        SIMDCalculateDotProductsWithConstantVectorAndCountNegative(n, &p1[i], v2,
            &dot[i], count);
        return;
    }
#endif
    for (; i + 3 < n; i += 4) {
        dot[i] = Dot(p1[i], v2);
        dot[i + 1] = Dot(p1[i + 1], v2);
        dot[i + 2] = Dot(p1[i + 2], v2);
        dot[i + 3] = Dot(p1[i + 3], v2);
        count += (dot[i] < 0.0f) + (dot[i + 1] < 0.0f) + (dot[i + 2] < 0.0f) +
            (dot[i + 3] < 0.0f);
    }
    for (; i < n; i++) {
        dot[i] = Dot(p1[i], v2);
        if (dot[i] < 0.0f)
            count++;
    }
}

// Determine the minimum and maximum dot product of an array of vertices with a
// given constant vector.
// vertex must be aligned on a 16-bytes boundary.

void CalculateMinAndMaxDotProduct(int nu_vertices, const Vector3D *vertex,
const Vector3D& v2, float& min_dot_product, float& max_dot_product) {
    int i = 0;
#ifdef DST_USE_SIMD
    if (((uintptr_t)vertex & 0xF) == 0 && sizeof(Vector3D) == 16) {
        __simd128_float m_v2_x = simd128_set_same_float(v2.x);
        __simd128_float m_v2_y = simd128_set_same_float(v2.y);
        __simd128_float m_v2_z = simd128_set_same_float(v2.z);
        __simd128_float m_min_dot = simd128_set_same_float(FLT_MAX);
        __simd128_float m_max_dot = simd128_set_same_float(- FLT_MAX);
        for (; i + 3 < nu_vertices; i += 4) {
#if 1
            __simd128_float m_vertex0 = simd128_load(&vertex[i]);
            __simd128_float m_vertex1 = simd128_load(&vertex[i + 1]);
            __simd128_float m_vertex2 = simd128_load(&vertex[i + 2]);
            __simd128_float m_vertex3 = simd128_load(&vertex[i + 3]);
            __simd128_float m_vertex_x, m_vertex_y, m_vertex_z;
            simd128_transpose4to3_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
                m_vertex_x, m_vertex_y, m_vertex_z);
#else
            __simd128_float m_vertex_x = simd128_set_float(
                vertex[i].x, vertex[i + 1].x, vertex[i + 2].x, vertex[i + 3].x);
            __simd128_float m_vertex_y = simd128_set_float(
                vertex[i].y, vertex[i + 1].y, vertex[i + 2].y, vertex[i + 3].y);
            __simd128_float m_vertex_z = simd128_set_float(
                vertex[i].z, vertex[i + 1].z, vertex[i + 2].z, vertex[i + 3].z);
#endif
            __simd128_float m_dot_x, m_dot_y, m_dot_z, m_dot;
            m_dot_x = simd128_mul_float(m_vertex_x, m_v2_x);
            m_dot_y = simd128_mul_float(m_vertex_y, m_v2_y);
            m_dot_z = simd128_mul_float(m_vertex_z, m_v2_z);
            m_dot = simd128_add_float(
                simd128_add_float(m_dot_x, m_dot_y), m_dot_z);
            m_min_dot = simd128_min_float(m_min_dot, m_dot);
            m_max_dot = simd128_max_float(m_max_dot, m_dot);
        }
        __simd128_float shifted_float1, shifted_float2, shifted_float3;
        __simd128_float m_min_dot_23, m_max_dot_23;
        shifted_float1 = simd128_shift_right_float(m_min_dot, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot, 3);
        m_min_dot = simd128_min1_float(m_min_dot, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot = simd128_min1_float(m_min_dot, m_min_dot_23);
        min_dot_product = simd128_get_float(m_min_dot);
        shifted_float1 = simd128_shift_right_float(m_max_dot, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot, 3);
        m_min_dot = simd128_max1_float(m_max_dot, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot = simd128_max1_float(m_max_dot, m_max_dot_23);
        max_dot_product = simd128_get_float(m_max_dot);
        // Process the remaining vertices using regular code.
    }
    else
#endif
    {
        min_dot_product = FLT_MAX;
        max_dot_product = - FLT_MAX;
    }
    for (; i < nu_vertices; i++) {
        min_dot_product = minf(min_dot_product, Dot(vertex[i], v2));
        max_dot_product = maxf(max_dot_product, Dot(vertex[i], v2));
    }
}

void CalculateMinAndMaxDotProduct(int nu_vertices, const Vector4D *vertex,
const Vector4D& v2, float& min_dot_product, float& max_dot_product) {
    int i = 0;
#ifdef DST_USE_SIMD
    if (((uintptr_t)vertex & 0xF) == 0) {
        __simd128_float m_v2_x = simd128_set_same_float(v2.x);
        __simd128_float m_v2_y = simd128_set_same_float(v2.y);
        __simd128_float m_v2_z = simd128_set_same_float(v2.z);
        __simd128_float m_v2_w = simd128_set_same_float(v2.w);
        __simd128_float m_min_dot = simd128_set_same_float(FLT_MAX);
        __simd128_float m_max_dot = simd128_set_same_float(- FLT_MAX);
        for (; i + 3 < nu_vertices; i += 4) {
#if 1
            __simd128_float m_vertex0 = simd128_load(&vertex[i]);
            __simd128_float m_vertex1 = simd128_load(&vertex[i + 1]);
            __simd128_float m_vertex2 = simd128_load(&vertex[i + 2]);
            __simd128_float m_vertex3 = simd128_load(&vertex[i + 3]);
            __simd128_float m_vertex_x, m_vertex_y, m_vertex_z, m_vertex_w;
            simd128_transpose4to4_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
                m_vertex_x, m_vertex_y, m_vertex_z, m_vertex_w);
#else
            __simd128_float m_vertex_x = simd128_set_float(
                vertex[i].x, vertex[i + 1].x, vertex[i + 2].x, vertex[i + 3].x);
            __simd128_float m_vertex_y = simd128_set_float(
                vertex[i].y, vertex[i + 1].y, vertex[i + 2].y, vertex[i + 3].y);
            __simd128_float m_vertex_z = simd128_set_float(
                vertex[i].z, vertex[i + 1].z, vertex[i + 2].z, vertex[i + 3].z);
            __simd128_float m_vertex_w = simd128_set_float(
                vertex[i].w, vertex[i + 1].w, vertex[i + 2].w, vertex[i + 3].w);
#endif
            __simd128_float m_dot_x, m_dot_y, m_dot_z, m_dot_w, m_dot;
            m_dot_x = simd128_mul_float(m_vertex_x, m_v2_x);
            m_dot_y = simd128_mul_float(m_vertex_y, m_v2_y);
            m_dot_z = simd128_mul_float(m_vertex_z, m_v2_z);
            m_dot_w = simd128_mul_float(m_vertex_w, m_v2_w);
            m_dot = simd128_add_float(
                simd128_add_float(m_dot_x, m_dot_y),
                simd128_add_float(m_dot_z, m_dot_w)
                );
            m_min_dot = simd128_min_float(m_min_dot, m_dot);
            m_max_dot = simd128_max_float(m_max_dot, m_dot);
        }
        __simd128_float shifted_float1, shifted_float2, shifted_float3;
        __simd128_float m_min_dot_23, m_max_dot_23;
        shifted_float1 = simd128_shift_right_float(m_min_dot, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot, 3);
        m_min_dot = simd128_min1_float(m_min_dot, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot = simd128_min1_float(m_min_dot, m_min_dot_23);
        min_dot_product = simd128_get_float(m_min_dot);
        shifted_float1 = simd128_shift_right_float(m_max_dot, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot, 3);
        m_min_dot = simd128_max1_float(m_max_dot, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot = simd128_max1_float(m_max_dot, m_max_dot_23);
        max_dot_product = simd128_get_float(m_max_dot);
        // Process the remaining vertices using regular code.
    }
    else
#endif
    {
        min_dot_product = FLT_MAX;
        max_dot_product = - FLT_MAX;
    }
    for (; i < nu_vertices; i++) {
        min_dot_product = minf(min_dot_product, Dot(vertex[i], v2));
        max_dot_product = maxf(max_dot_product, Dot(vertex[i], v2));
    }
}

// Determine the minimum and maximum dot products of an array of vertices with three
// constant vectors.

void CalculateMinAndMaxDotProductWithThreeConstantVectors(int nu_vertices,
const Vector3D *vertex, const Vector3D *C, float *min_dot_product, float *max_dot_product) {
    int i = 0;
#ifdef DST_USE_SIMD
    if (((uintptr_t)vertex & 0xF) == 0 && sizeof(Vector3D) == 16) {
        __simd128_float m_v2_0_x = simd128_set_same_float(C[0].x);
        __simd128_float m_v2_0_y = simd128_set_same_float(C[0].y);
        __simd128_float m_v2_0_z = simd128_set_same_float(C[0].z);
        __simd128_float m_v2_1_x = simd128_set_same_float(C[1].x);
        __simd128_float m_v2_1_y = simd128_set_same_float(C[1].y);
        __simd128_float m_v2_1_z = simd128_set_same_float(C[1].z);
        __simd128_float m_v2_2_x = simd128_set_same_float(C[2].x);
        __simd128_float m_v2_2_y = simd128_set_same_float(C[2].y);
        __simd128_float m_v2_2_z = simd128_set_same_float(C[2].z);
        __simd128_float m_min_dot_C0 = simd128_set_same_float(FLT_MAX);
        __simd128_float m_min_dot_C1 = simd128_set_same_float(FLT_MAX);
        __simd128_float m_min_dot_C2 = simd128_set_same_float(FLT_MAX);
        __simd128_float m_max_dot_C0 = simd128_set_same_float(- FLT_MAX);
        __simd128_float m_max_dot_C1 = simd128_set_same_float(- FLT_MAX);
        __simd128_float m_max_dot_C2 = simd128_set_same_float(- FLT_MAX);
        for (; i + 3 < nu_vertices; i += 4) {
#if 1
            __simd128_float m_vertex0 = simd128_load(&vertex[i]);
            __simd128_float m_vertex1 = simd128_load(&vertex[i + 1]);
            __simd128_float m_vertex2 = simd128_load(&vertex[i + 2]);
            __simd128_float m_vertex3 = simd128_load(&vertex[i + 3]);
            __simd128_float m_vertex_x, m_vertex_y, m_vertex_z;
            simd128_transpose4to3_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
                m_vertex_x, m_vertex_y, m_vertex_z);
#else
            __simd128_float m_vertex_x = simd128_set_float(
                vertex[i].x, vertex[i + 1].x, vertex[i + 2].x, vertex[i + 3].x);
            __simd128_float m_vertex_y = simd128_set_float(
                vertex[i].y, vertex[i + 1].y, vertex[i + 2].y, vertex[i + 3].y);
            __simd128_float m_vertex_z = simd128_set_float(
                vertex[i].z, vertex[i + 1].z, vertex[i + 2].z, vertex[i + 3].z);
#endif
            __simd128_float m_dot_x, m_dot_y, m_dot_z, m_dot;
            m_dot_x = simd128_mul_float(m_vertex_x, m_v2_0_x);
            m_dot_y = simd128_mul_float(m_vertex_y, m_v2_0_y);
            m_dot_z = simd128_mul_float(m_vertex_z, m_v2_0_z);
            m_dot = simd128_add_float(
               simd128_add_float(m_dot_x, m_dot_y), m_dot_z);
            m_min_dot_C0 = simd128_min_float(m_min_dot_C0, m_dot);
            m_max_dot_C0 = simd128_max_float(m_max_dot_C0, m_dot);
            m_dot_x = simd128_mul_float(m_vertex_x, m_v2_1_x);
            m_dot_y = simd128_mul_float(m_vertex_y, m_v2_1_y);
            m_dot_z = simd128_mul_float(m_vertex_z, m_v2_1_z);
            m_dot = simd128_add_float(
                simd128_add_float(m_dot_x, m_dot_y), m_dot_z);
            m_min_dot_C1 = simd128_min_float(m_min_dot_C1, m_dot);
            m_max_dot_C1 = simd128_max_float(m_max_dot_C1, m_dot);
            m_dot_x = simd128_mul_float(m_vertex_x, m_v2_2_x);
            m_dot_y = simd128_mul_float(m_vertex_y, m_v2_2_y);
            m_dot_z = simd128_mul_float(m_vertex_z, m_v2_2_z);
            m_dot = simd128_add_float(
                simd128_add_float(m_dot_x, m_dot_y), m_dot_z);
            m_min_dot_C2 = simd128_min_float(m_min_dot_C2, m_dot);
            m_max_dot_C2 = simd128_max_float(m_max_dot_C2, m_dot);
        }
        __simd128_float shifted_float1, shifted_float2, shifted_float3;
        __simd128_float m_min_dot_23, m_max_dot_23;
        shifted_float1 = simd128_shift_right_float(m_min_dot_C0, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot_C0, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot_C0, 3);
        m_min_dot_C0 = simd128_min1_float(m_min_dot_C0, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot_C0 = simd128_min1_float(m_min_dot_C0, m_min_dot_23);
        min_dot_product[0] = simd128_get_float(m_min_dot_C0);
        shifted_float1 = simd128_shift_right_float(m_max_dot_C0, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot_C0, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot_C0, 3);
        m_min_dot_C0 = simd128_max1_float(m_max_dot_C0, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot_C0 = simd128_max1_float(m_max_dot_C0, m_max_dot_23);
        max_dot_product[0] = simd128_get_float(m_max_dot_C0);

        shifted_float1 = simd128_shift_right_float(m_min_dot_C1, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot_C1, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot_C1, 3);
        m_min_dot_C1 = simd128_min1_float(m_min_dot_C1, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot_C1 = simd128_min1_float(m_min_dot_C1, m_min_dot_23);
        min_dot_product[1] = simd128_get_float(m_min_dot_C1);
        shifted_float1 = simd128_shift_right_float(m_max_dot_C1, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot_C1, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot_C1, 3);
        m_max_dot_C1 = simd128_max1_float(m_max_dot_C1, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot_C1 = simd128_max1_float(m_max_dot_C1, m_max_dot_23);
        max_dot_product[1] = simd128_get_float(m_max_dot_C1);

        shifted_float1 = simd128_shift_right_float(m_min_dot_C2, 1);
        shifted_float2 = simd128_shift_right_float(m_min_dot_C2, 2);
        shifted_float3 = simd128_shift_right_float(m_min_dot_C2, 3);
        m_min_dot_C2 = simd128_min1_float(m_min_dot_C2, shifted_float1);
        m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
        m_min_dot_C2 = simd128_min1_float(m_min_dot_C2, m_min_dot_23);
        min_dot_product[2] = simd128_get_float(m_min_dot_C2);
        shifted_float1 = simd128_shift_right_float(m_max_dot_C2, 1);
        shifted_float2 = simd128_shift_right_float(m_max_dot_C2, 2);
        shifted_float3 = simd128_shift_right_float(m_max_dot_C2, 3);
        m_max_dot_C2 = simd128_max1_float(m_max_dot_C2, shifted_float1);
        m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
        m_max_dot_C2 = simd128_max1_float(m_max_dot_C2, m_max_dot_23);
        max_dot_product[2] = simd128_get_float(m_max_dot_C2);
    }
    else
#endif
    for (int j = 0; j < 3; j++) {
        min_dot_product[j] = FLT_MAX;
        max_dot_product[j] = - FLT_MAX;
    }
    // Process the remaining vertices.
    for (; i < nu_vertices; i++)
        for (int j = 0; j < 3; j++) {
            min_dot_product[j] = minf(min_dot_product[j], Dot(vertex[i], C[j]));
            max_dot_product[j] = maxf(max_dot_product[j], Dot(vertex[i], C[j]));
        }
}

void CalculateMinAndMaxDotProductWithThreeConstantVectors(int nu_vertices,
const Vector4D *vertex, const Vector4D *C, float *min_dot_product, float *max_dot_product) {
    int i = 0;
#ifdef DST_USE_SIMD
    if (((uintptr_t)vertex & 0xF) == 0) {
    __simd128_float m_v2_0_x = simd128_set_same_float(C[0].x);
    __simd128_float m_v2_0_y = simd128_set_same_float(C[0].y);
    __simd128_float m_v2_0_z = simd128_set_same_float(C[0].z);
    __simd128_float m_v2_0_w = simd128_set_same_float(C[0].w);
    __simd128_float m_v2_1_x = simd128_set_same_float(C[1].x);
    __simd128_float m_v2_1_y = simd128_set_same_float(C[1].y);
    __simd128_float m_v2_1_z = simd128_set_same_float(C[1].z);
    __simd128_float m_v2_1_w = simd128_set_same_float(C[1].w);
    __simd128_float m_v2_2_x = simd128_set_same_float(C[2].x);
    __simd128_float m_v2_2_y = simd128_set_same_float(C[2].y);
    __simd128_float m_v2_2_z = simd128_set_same_float(C[2].z);
    __simd128_float m_v2_2_w = simd128_set_same_float(C[2].w);
    __simd128_float m_min_dot_C0 = simd128_set_same_float(FLT_MAX);
    __simd128_float m_min_dot_C1 = simd128_set_same_float(FLT_MAX);
    __simd128_float m_min_dot_C2 = simd128_set_same_float(FLT_MAX);
    __simd128_float m_max_dot_C0 = simd128_set_same_float(- FLT_MAX);
    __simd128_float m_max_dot_C1 = simd128_set_same_float(- FLT_MAX);
    __simd128_float m_max_dot_C2 = simd128_set_same_float(- FLT_MAX);
    for (; i + 3 < nu_vertices; i += 4) {
#if 1
        __simd128_float m_vertex0 = simd128_load(&vertex[i]);
        __simd128_float m_vertex1 = simd128_load(&vertex[i + 1]);
        __simd128_float m_vertex2 = simd128_load(&vertex[i + 2]);
        __simd128_float m_vertex3 = simd128_load(&vertex[i + 3]);
        __simd128_float m_vertex_x, m_vertex_y, m_vertex_z, m_vertex_w;
        simd128_transpose4to4_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
            m_vertex_x, m_vertex_y, m_vertex_z, m_vertex_w);
#else
        __simd128_float m_vertex_x = simd128_set_float(
            vertex[i].x, vertex[i + 1].x, vertex[i + 2].x, vertex[i + 3].x);
        __simd128_float m_vertex_y = simd128_set_float(
            vertex[i].y, vertex[i + 1].y, vertex[i + 2].y, vertex[i + 3].y);
        __simd128_float m_vertex_z = simd128_set_float(
            vertex[i].z, vertex[i + 1].z, vertex[i + 2].z, vertex[i + 3].z);
        __simd128_float m_vertex_w = simd128_set_float(
            vertex[i].w, vertex[i + 1].w, vertex[i + 2].w, vertex[i + 3].w);
#endif
        __simd128_float m_dot_x, m_dot_y, m_dot_z, m_dot_w, m_dot;
        m_dot_x = simd128_mul_float(m_vertex_x, m_v2_0_x);
        m_dot_y = simd128_mul_float(m_vertex_y, m_v2_0_y);
        m_dot_z = simd128_mul_float(m_vertex_z, m_v2_0_z);
        m_dot_w = simd128_mul_float(m_vertex_w, m_v2_0_w);
        m_dot = simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y),
            simd128_add_float(m_dot_z, m_dot_w)
            );
        m_min_dot_C0 = simd128_min_float(m_min_dot_C0, m_dot);
        m_max_dot_C0 = simd128_max_float(m_max_dot_C0, m_dot);
        m_dot_x = simd128_mul_float(m_vertex_x, m_v2_1_x);
        m_dot_y = simd128_mul_float(m_vertex_y, m_v2_1_y);
        m_dot_z = simd128_mul_float(m_vertex_z, m_v2_1_z);
        m_dot_w = simd128_mul_float(m_vertex_w, m_v2_1_w);
        m_dot = simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y),
            simd128_add_float(m_dot_z, m_dot_w)
            );
        m_min_dot_C1 = simd128_min_float(m_min_dot_C1, m_dot);
        m_max_dot_C1 = simd128_max_float(m_max_dot_C1, m_dot);
        m_dot_x = simd128_mul_float(m_vertex_x, m_v2_2_x);
        m_dot_y = simd128_mul_float(m_vertex_y, m_v2_2_y);
        m_dot_z = simd128_mul_float(m_vertex_z, m_v2_2_z);
        m_dot_w = simd128_mul_float(m_vertex_w, m_v2_2_w);
        m_dot = simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y),
            simd128_add_float(m_dot_z, m_dot_w)
            );
        m_min_dot_C2 = simd128_min_float(m_min_dot_C2, m_dot);
        m_max_dot_C2 = simd128_max_float(m_max_dot_C2, m_dot);
    }
    __simd128_float shifted_float1, shifted_float2, shifted_float3;
    __simd128_float m_min_dot_23, m_max_dot_23;
    shifted_float1 = simd128_shift_right_float(m_min_dot_C0, 1);
    shifted_float2 = simd128_shift_right_float(m_min_dot_C0, 2);
    shifted_float3 = simd128_shift_right_float(m_min_dot_C0, 3);
    m_min_dot_C0 = simd128_min1_float(m_min_dot_C0, shifted_float1);
    m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
    m_min_dot_C0 = simd128_min1_float(m_min_dot_C0, m_min_dot_23);
    min_dot_product[0] = simd128_get_float(m_min_dot_C0);
    shifted_float1 = simd128_shift_right_float(m_max_dot_C0, 1);
    shifted_float2 = simd128_shift_right_float(m_max_dot_C0, 2);
    shifted_float3 = simd128_shift_right_float(m_max_dot_C0, 3);
    m_min_dot_C0 = simd128_max1_float(m_max_dot_C0, shifted_float1);
    m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
    m_max_dot_C0 = simd128_max1_float(m_max_dot_C0, m_max_dot_23);
    max_dot_product[0] = simd128_get_float(m_max_dot_C0);

    shifted_float1 = simd128_shift_right_float(m_min_dot_C1, 1);
    shifted_float2 = simd128_shift_right_float(m_min_dot_C1, 2);
    shifted_float3 = simd128_shift_right_float(m_min_dot_C1, 3);
    m_min_dot_C1 = simd128_min1_float(m_min_dot_C1, shifted_float1);
    m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
    m_min_dot_C1 = simd128_min1_float(m_min_dot_C1, m_min_dot_23);
    min_dot_product[1] = simd128_get_float(m_min_dot_C1);
    shifted_float1 = simd128_shift_right_float(m_max_dot_C1, 1);
    shifted_float2 = simd128_shift_right_float(m_max_dot_C1, 2);
    shifted_float3 = simd128_shift_right_float(m_max_dot_C1, 3);
    m_max_dot_C1 = simd128_max1_float(m_max_dot_C1, shifted_float1);
    m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
    m_max_dot_C1 = simd128_max1_float(m_max_dot_C1, m_max_dot_23);
    max_dot_product[1] = simd128_get_float(m_max_dot_C1);

    shifted_float1 = simd128_shift_right_float(m_min_dot_C2, 1);
    shifted_float2 = simd128_shift_right_float(m_min_dot_C2, 2);
    shifted_float3 = simd128_shift_right_float(m_min_dot_C2, 3);
    m_min_dot_C2 = simd128_min1_float(m_min_dot_C2, shifted_float1);
    m_min_dot_23 = simd128_min1_float(shifted_float2, shifted_float3);
    m_min_dot_C2 = simd128_min1_float(m_min_dot_C2, m_min_dot_23);
    min_dot_product[2] = simd128_get_float(m_min_dot_C2);
    shifted_float1 = simd128_shift_right_float(m_max_dot_C2, 1);
    shifted_float2 = simd128_shift_right_float(m_max_dot_C2, 2);
    shifted_float3 = simd128_shift_right_float(m_max_dot_C2, 3);
    m_max_dot_C2 = simd128_max1_float(m_max_dot_C2, shifted_float1);
    m_max_dot_23 = simd128_max1_float(shifted_float2, shifted_float3);
    m_max_dot_C2 = simd128_max1_float(m_max_dot_C2, m_max_dot_23);
    max_dot_product[2] = simd128_get_float(m_max_dot_C2);
    }
    else
#endif
    for (int j = 0; j < 3; j++) {
        min_dot_product[j] = FLT_MAX;
        max_dot_product[j] = - FLT_MAX;
    }
    // Process the remaining vertices.
    for (; i < nu_vertices; i++)
        for (int j = 0; j < 3; j++) {
            min_dot_product[j] = minf(min_dot_product[j], Dot(vertex[i], C[j]));
            max_dot_product[j] = maxf(max_dot_product[j], Dot(vertex[i], C[j]));
        }
}

// Determine the indices within an array of vertices that have the minimum and
// maximum dot product with the given constant vector.

void GetIndicesWithMinAndMaxDotProduct(int nu_vertices, const Vector3D *vertex,
const Vector3D& v2, int& i_Pmin, int& i_Pmax) {
    int i = 0;
    float min_dot_product;
    float max_dot_product;
#ifdef DST_USE_SIMD
    if (((uintptr_t)vertex & 0xF) == 0 && sizeof(Vector3D) == 16) {
    __simd128_float m_v2_x = simd128_set_same_float(v2.x);
    __simd128_float m_v2_y = simd128_set_same_float(v2.y);
    __simd128_float m_v2_z = simd128_set_same_float(v2.z);
    __simd128_float m_min_dot = simd128_set_same_float(FLT_MAX);
    __simd128_float m_max_dot = simd128_set_same_float(- FLT_MAX);
    __simd128_int m_min_dot_index = simd128_set_zero_int();
    __simd128_int m_max_dot_index = simd128_set_zero_int();
    // Keep track of four minimum and four maximum dot products (each representing the
    // min/max for a quarter of the vertices).
    for (; i + 3 < nu_vertices; i += 4) {
#if 1
        __simd128_float m_vertex0 = simd128_load(&vertex[i]);
        __simd128_float m_vertex1 = simd128_load(&vertex[i + 1]);
        __simd128_float m_vertex2 = simd128_load(&vertex[i + 2]);
        __simd128_float m_vertex3 = simd128_load(&vertex[i + 3]);
        __simd128_float m_vertex_x, m_vertex_y, m_vertex_z;
        simd128_transpose4to3_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
            m_vertex_x, m_vertex_y, m_vertex_z);
#else
        __simd128_float m_vertex_x = simd128_set_float(
            vertex[i].x, vertex[i + 1].x, vertex[i + 2].x, vertex[i + 3].x);
        __simd128_float m_vertex_y = simd128_set_float(
            vertex[i].y, vertex[i + 1].y, vertex[i + 2].y, vertex[i + 3].y);
        __simd128_float m_vertex_z = simd128_set_float(
            vertex[i].z, vertex[i + 1].z, vertex[i + 2].z, vertex[i + 3].z);
#endif
        __simd128_float m_dot_x, m_dot_y, m_dot_z, m_dot;
        m_dot_x = simd128_mul_float(m_vertex_x, m_v2_x);
        m_dot_y = simd128_mul_float(m_vertex_y, m_v2_y);
        m_dot_z = simd128_mul_float(m_vertex_z, m_v2_z);
        m_dot = simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y), m_dot_z);
        m_min_dot = simd128_min_float(m_min_dot, m_dot);
        m_max_dot = simd128_max_float(m_max_dot, m_dot);
        // Update the index for each component for which the min or max was updated.
        __simd128_int comp_min, comp_max;
        __simd128_int indices = simd128_set_int32(i, i + 1, i + 2, i + 3);
        // Set the bitmask for components for which the minimum/maximum was updated.
        comp_min = simd128_cmpeq_float(m_min_dot, m_dot);
        comp_max = simd128_cmpeq_float(m_max_dot, m_dot);
        m_min_dot_index = simd128_or_int(
            simd128_andnot_int(comp_min, m_min_dot_index),
            simd128_and_int(comp_min, indices)
            );
        m_max_dot_index = simd128_or_int(
            simd128_andnot_int(comp_max, m_max_dot_index),
            simd128_and_int(comp_max, indices)
            );
    }
    // Combine the minimum/maximum calculated for each quarter of the vertices.
    min_dot_product = simd128_get_float(m_min_dot);
    i_Pmin = simd128_get_int32(m_min_dot_index);
    max_dot_product = simd128_get_float(m_max_dot);
    i_Pmax = simd128_get_int32(m_max_dot_index);
    for (int j = 1; j < 4; j++) {
        m_min_dot = simd128_shift_right_float(m_min_dot, 1);
        m_max_dot = simd128_shift_right_float(m_max_dot, 1);
        m_min_dot_index = simd128_shift_right_bytes_int(m_min_dot_index, 4);
        m_max_dot_index = simd128_shift_right_bytes_int(m_max_dot_index, 4);
        float v_min = simd128_get_float(m_min_dot);
        float v_max = simd128_get_float(m_max_dot);
        if (v_min < min_dot_product) {
            min_dot_product = v_min;
            i_Pmin = simd128_get_int32(m_min_dot_index);
        }
        if (v_max > max_dot_product) {
            max_dot_product = v_max;
            i_Pmax = simd128_get_int32(m_max_dot_index);
        }
    }
    }
    else
#endif
    {
        min_dot_product = FLT_MAX;
        max_dot_product = - FLT_MAX;
    }
    for (; i < nu_vertices; i++) {
        float dot = Dot(vertex[i], v2);
        if (dot < min_dot_product) {
            min_dot_product = dot;
            i_Pmin = i;
        }
        if (dot > max_dot_product) {
            max_dot_product = dot;
            i_Pmax = i;
        }
    }
}

void GetIndicesWithMinAndMaxDotProduct(int nu_vertices, const Vector4D *vertex,
const Vector4D& v2, int& i_Pmin, int& i_Pmax) {
    int i = 0;
    float min_dot_product;
    float max_dot_product;
#ifdef DST_USE_SIMD
    if (((uintptr_t)vertex & 0xF) == 0) {
    __simd128_float m_v2_x = simd128_set_same_float(v2.x);
    __simd128_float m_v2_y = simd128_set_same_float(v2.y);
    __simd128_float m_v2_z = simd128_set_same_float(v2.z);
    __simd128_float m_v2_w = simd128_set_same_float(v2.w);
    __simd128_float m_min_dot = simd128_set_same_float(FLT_MAX);
    __simd128_float m_max_dot = simd128_set_same_float(- FLT_MAX);
    __simd128_int m_min_dot_index = simd128_set_zero_int();
    __simd128_int m_max_dot_index = simd128_set_zero_int();
    // Keep track of four minimum and four maximum dot products (each representing the
    // min/max for a quarter of the vertices).
    for (; i + 3 < nu_vertices; i += 4) {
#if 1
        __simd128_float m_vertex0 = simd128_load(&vertex[i]);
        __simd128_float m_vertex1 = simd128_load(&vertex[i + 1]);
        __simd128_float m_vertex2 = simd128_load(&vertex[i + 2]);
        __simd128_float m_vertex3 = simd128_load(&vertex[i + 3]);
        __simd128_float m_vertex_x, m_vertex_y, m_vertex_z, m_vertex_w;
        simd128_transpose4to4_float(m_vertex0, m_vertex1, m_vertex2, m_vertex3,
            m_vertex_x, m_vertex_y, m_vertex_z, m_vertex_w);
#else
        __simd128_float m_vertex_x = simd128_set_float(
            vertex[i].x, vertex[i + 1].x, vertex[i + 2].x, vertex[i + 3].x);
        __simd128_float m_vertex_y = simd128_set_float(
            vertex[i].y, vertex[i + 1].y, vertex[i + 2].y, vertex[i + 3].y);
        __simd128_float m_vertex_z = simd128_set_float(
            vertex[i].z, vertex[i + 1].z, vertex[i + 2].z, vertex[i + 3].z);
        __simd128_float m_vertex_w = simd128_set_float(
            vertex[i].w, vertex[i + 1].w, vertex[i + 2].w, vertex[i + 3].w);
#endif
        __simd128_float m_dot_x, m_dot_y, m_dot_z, m_dot_w, m_dot;
        m_dot_x = simd128_mul_float(m_vertex_x, m_v2_x);
        m_dot_y = simd128_mul_float(m_vertex_y, m_v2_y);
        m_dot_z = simd128_mul_float(m_vertex_z, m_v2_z);
        m_dot_w = simd128_mul_float(m_vertex_w, m_v2_w);
        m_dot = simd128_add_float(
            simd128_add_float(m_dot_x, m_dot_y),
            simd128_add_float(m_dot_z, m_dot_w)
            );
        m_min_dot = simd128_min_float(m_min_dot, m_dot);
        m_max_dot = simd128_max_float(m_max_dot, m_dot);
        // Update the index for each component for which the min or max was updated.
        __simd128_int comp_min, comp_max;
        __simd128_int indices = simd128_set_int32(i, i + 1, i + 2, i + 3);
        // Set the bitmask for components for which the minimum/maximum was updated.
        comp_min = simd128_cmpeq_float(m_min_dot, m_dot);
        comp_max = simd128_cmpeq_float(m_max_dot, m_dot);
        m_min_dot_index = simd128_or_int(
            simd128_andnot_int(comp_min, m_min_dot_index),
            simd128_and_int(comp_min, indices)
            );
        m_max_dot_index = simd128_or_int(
            simd128_andnot_int(comp_max, m_max_dot_index),
            simd128_and_int(comp_max, indices)
            );
    }
    // Combine the minimum/maximum calculated for each quarter of the vertices.
    min_dot_product = simd128_get_float(m_min_dot);
    i_Pmin = simd128_get_int32(m_min_dot_index);
    max_dot_product = simd128_get_float(m_max_dot);
    i_Pmax = simd128_get_int32(m_max_dot_index);
    for (int j = 1; j < 4; j++) {
        m_min_dot = simd128_shift_right_float(m_min_dot, 1);
        m_max_dot = simd128_shift_right_float(m_max_dot, 1);
        m_min_dot_index = simd128_shift_right_bytes_int(m_min_dot_index, 4);
        m_max_dot_index = simd128_shift_right_bytes_int(m_max_dot_index, 4);
        float v_min = simd128_get_float(m_min_dot);
        float v_max = simd128_get_float(m_max_dot);
        if (v_min < min_dot_product) {
            min_dot_product = v_min;
            i_Pmin = simd128_get_int32(m_min_dot_index);
        }
        if (v_max > max_dot_product) {
            max_dot_product = v_max;
            i_Pmax = simd128_get_int32(m_max_dot_index);
        }
    }
    }
    else
#endif
    {
        min_dot_product = FLT_MAX;
        max_dot_product = - FLT_MAX;
    }
    for (; i < nu_vertices; i++) {
        float dot = Dot(vertex[i], v2);
        if (dot < min_dot_product) {
            min_dot_product = dot;
            i_Pmin = i;
        }
        if (dot > max_dot_product) {
            max_dot_product = dot;
            i_Pmax = i;
        }
    }
}

