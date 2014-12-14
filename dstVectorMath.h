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

#ifndef __DST_VECTOR_MATH_H__
#define __DST_VECTOR_MATH_H__

#include "dstConfig.h"
#include "dstMath.h"

// This code is largely based on the source code provided by Eric Lengyel
// in assocation with a book and on the net, as described below.
//
// Several changes and additions have been made to the code (one of which is
// changing the meaning of the * operator between vectors from dot product to
// pairwise multiplication).

//============================================================================
//
// 2D/3D/4D Vector Classes
//
// Mathematics for 3D Game Programming and Computer Graphics, 3rd ed.
// By Eric Lengyel
//
// The code in this file may be freely used in any software. It is provided
// as-is, with no warranty of any kind.
//
//============================================================================

template <class T>
class DST_API Vector2DBase {
public:
	T x;
	T y;

	Vector2DBase() {}

	Vector2DBase(T r, T s) {
		x = r;
		y = s;
	}

	Vector2DBase& Set(T r, T s) {
		x = r;
		y = s;
		return (*this);
	}

	T& operator [](int k) {
		return ((&x)[k]);
	}

	const T& operator [](int k) const {
		return ((&x)[k]);
	}

	Vector2DBase& operator +=(const Vector2DBase& v) {
		x += v.x;
		y += v.y;
		return (*this);
	}

	Vector2DBase& operator -=(const Vector2DBase& v) {
		x -= v.x;
		y -= v.y;
		return (*this);
	}

	Vector2DBase& operator *=(T t) {
		x *= t;
		y *= t;
		return (*this);
	}

	Vector2DBase& operator /=(T t) {
		T f = 1.0F / t;
		x *= f;
		y *= f;
		return (*this);
	}

	Vector2DBase& operator &=(const Vector2DBase& v) {
		x *= v.x;
		y *= v.y;
		return (*this);
	}

	Vector2DBase operator -() const {
		return (Vector2DBase(- x, - y));
	}

	Vector2DBase operator -(const Vector2DBase& v) const {
		return (Vector2DBase(x - v.x, y - v.y));
	}

	Vector2DBase operator +(const Vector2DBase& v) const {
		return (Vector2DBase(x + v.x, y + v.y));
	}

	Vector2DBase operator *(T t) const {
		return (Vector2DBase(x * t, y * t));
	}

	Vector2DBase operator /(T t) const {
		float f = 1.0f / t;
		return (Vector2DBase(x * f, y * f));
	}

	Vector2DBase& Normalize(void) {
		return (*this *= dstInvSqrt(x * x + y * y));
	}

	T operator *(const Vector2DBase& v) const {
		return (x * v.x + y * v.y);
	}

	// * operator is pair-wise multiplication.
	Vector2DBase operator *(const Vector2DBase& v) {
		return (Vector2DBase(x * v.x, y * v.y));
	}

	bool operator ==(const Vector2DBase& v) {
		return ((x == v.x) && (y == v.y));
	}

	bool operator !=(const Vector2DBase& v) {
		return ((x != v.x) || (y != v.y));
	}

	friend Vector2DBase operator *(T t, const Vector2DBase& v) {
		return (Vector2DBase(t * v.x, t * v.y));
	}

	friend T Dot(const Vector2DBase& v1, const Vector2DBase& v2) {
		return (v1.x * v2.x + v1.y * v2.y);
	}

	friend Vector2DBase ProjectOnto(const Vector2DBase& v1, const Vector2DBase& v2) {
		return (v2 * (v1 * v2));
	}

	friend T Magnitude(const Vector2DBase& v) {
		return (dstSqrt(v.x * v.x + v.y * v.y));
	}

	friend T InverseMag(const Vector2DBase& v) {
		return (dstInvSqrt(v.x * v.x + v.y * v.y));
	}

	friend T SquaredMag(const Vector2DBase& v) {
		return (v.x * v.x + v.y * v.y);
	}
};

typedef Vector2DBase <float> Vector2D;
typedef Vector2DBase <double> VectorDouble2D;


template <class T>
class DST_API Point2DBase : public Vector2DBase <T> {
public:
	Point2DBase() {}

	Point2DBase(float r, float s) : Vector2DBase <T>(r, s) { }

	Vector2DBase <T> & GetVector2D(void) {
		return (*this);
	}
		
	const Vector2DBase <T> & GetVector2D(void) const {
		return (*this);
	}

	Point2DBase <T> & operator =(const Vector2DBase <T> & v) {
		*((Vector2DBase <T> *)this) = v;
		return (*this);
	}
};

typedef Point2DBase <float> Point2D;
typedef Point2DBase <double> PointDouble2D;


template <class T>
class DST_API Vector3DBase {
public:
	union {
		T x;
		T r;
	};
	union {
		T y;
		T g;
	};
	union {
		T z;
		T b;
	};

	Vector3DBase() {}

	Vector3DBase(T s, T t, T u) {
		x = s;
		y = t;
		z = u;
	}

	Vector3DBase(const Vector2D& v) {
		x = v.x;
		y = v.y;
		z = 0.0;
	}

	Vector3DBase(const Vector2D& v, float u) {
		x = v.x;
		y = v.y;
		z = u;
	}

	Vector3DBase& Set(T s, T t, T u) {
		x = s;
		y = t;
		z = u;
		return (*this);
	}

	T& operator [](int k) {
		return ((&x)[k]);
	}

	const T& operator [](int k) const {
		return ((&x)[k]);
	}

	Vector2DBase <T> & GetVector2DBase() {
		return (*reinterpret_cast<Vector2DBase <T> *>(this));
	}
		
	const Vector2DBase <T> & GetVector2D() const {
		return (*reinterpret_cast<const Vector2DBase <T> *>(this));
	}
		
	Point2DBase <T> & GetPoint2D() {
		return (*reinterpret_cast<Point2DBase <T> *>(this));
	}
		
	const Point2DBase <T>& GetPoint2D() const {
		return (*reinterpret_cast<const Point2DBase <T> *>(this));
	}

	Vector3DBase& operator +=(const Vector3DBase& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return (*this);
	}

	Vector3DBase& operator -=(const Vector3DBase& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return (*this);
	}

	Vector3DBase& operator *=(T t) {
		x *= t;
		y *= t;
		z *= t;
		return (*this);
	}

	Vector3DBase& operator /=(T t) {
		T f = 1.0F / t;
		x *= f;
		y *= f;
		z *= f;
		return (*this);
	}

	// * operator is pair-wise multiplication.
	Vector3DBase& operator *=(const Vector3DBase& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return (*this);
	}

	Vector3DBase operator -() const {
		return (Vector3DBase(- x, - y, - z));
	}

	Vector3DBase operator -(const Vector3DBase& v) const {
		return (Vector3DBase(x - v.x, y - v.y, z - v.z));
	}

	Vector3DBase operator -(const Vector2DBase <T> & v) {
		return (Vector3DBase(x - v.x, y - v.y, z));
	}

	Vector3DBase operator +(const Vector3DBase& v) const {
		return (Vector3DBase(x + v.x, y + v.y, z + v.z));
	}

	Vector3DBase operator +(const Vector2DBase <T>& v) const {
		return (Vector3DBase <T>(x + v.x, y + v.y, z));
	}

	Vector3DBase operator *(T t) const {
		return (Vector3DBase(x * t, y * t, z * t));
	}

	Vector3DBase operator /(T t) const {
		float f = 1.0f / t;
		return (Vector3DBase(x * f, y * f, z * f));
	}

	Vector3DBase& Normalize(void) {
		return (*this *= dstInvSqrt(x * x + y * y + z * z));
	}

	// * operator is pair-wise multiplication.
	Vector3DBase operator *(const Vector3DBase& v) const{
		return (Vector3DBase(x * v.x, y * v.y, z * v.z));
	}

	bool operator ==(const Vector3DBase& v) const {
		return ((x == v.x) && (y == v.y) && (z == v.z));
	}

	bool operator !=(const Vector3DBase& v) const {
		return ((x != v.x) || (y != v.y) || (z != v.z));
	}

	friend Vector3DBase operator *(T t, const Vector3DBase& v) {
		return (Vector3DBase(t * v.x, t * v.y, t * v.z));
	}

	friend T Dot(const Vector3DBase& v1, const Vector3DBase& v2) {
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}

	friend Vector3DBase Cross(const Vector3DBase& v1, const Vector3DBase& v2) {
		return Vector3DBase(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}

	friend Vector3DBase ProjectOnto(const Vector3DBase& v1, const Vector3DBase& v2) {
		return v2 * Dot(v1, v2);
	}

	// Project v1 onto or in the direction of v2, with the angle limited by the specified
	// value.
	friend Vector3DBase ProjectOntoWithLimit(const Vector3DBase& v1, const Vector3DBase& v2,
	T min_cos_angle) {
		float dot = Dot(v1, v2);
	        if (dot < min_cos_angle)
        	    dot = min_cos_angle;
		return v2 * dot;
	}

	friend T SquaredMag(const Vector3DBase& v) {
		return (v.x * v.x + v.y * v.y + v.z * v.z);
	}

	friend T Magnitude(const Vector3DBase& v) {
		return dstSqrt(SquaredMag(v));
	}

	friend T InverseMag(const Vector3DBase& v) {
		return dstInvSqrt(SquaredMag(v));
	}

	// Return text respresentation. To be freed with delete [].
	char *GetString() const;
};

typedef Vector3DBase <float> Vector3D;
typedef Vector3DBase <double> VectorDouble3D;


template <class T>
class DST_API Point3DBase : public Vector3DBase <T>
{
	public:
		
	Point3DBase() {}
	Point3DBase(T r, T s, T t) : Vector3DBase <T>(r, s, t) {}
	Point3DBase(const Vector2DBase <T> & v) : Vector3DBase <T>(v) {}
	Point3DBase(const Vector2DBase <T> & v, float u) : Vector3DBase <T>(v, u) {}
	Point3DBase(const Vector3DBase <T> & v) {
		*((Vector3DBase <T> *)this) = v;
	}

	Vector3DBase <T> & GetVector3D(void) {
		return (*this);
	}

	const Vector3DBase <T>& GetVector3D(void) const {
		return (*this);
	}

	Point2DBase <T> & GetPoint2D(void) {
		return (*reinterpret_cast<Point2DBase <T> *>(this));
	}

	const Point2DBase <T> & GetPoint2D(void) const {
		return (*reinterpret_cast<const Point2DBase <T> *>(this));
	}

	Point3DBase <T> & operator =(const Vector3DBase <T> & v) {
		*((Vector3DBase <T> *)this) = v;
		return (*this);
	}

	// Adding a vector to a point yields a point, so we cannot inherit the parent class
	// version.
	Point3DBase operator -(const Vector3DBase <T>& v) const {
		return (*(Vector3DBase <T> *)this) - v;
	}

	// Subtracting a vector from a point yields a point, so we cannot inherit the parent class
	// version.
	Point3DBase operator +(const Vector3DBase <T>& v) const {
		return (*(Vector3DBase <T> *)this) + v;
	}

	friend Vector3DBase <T> CalculateNormal(const Point3DBase <T> &v1, const Point3DBase <T> &v2,
	const Point3DBase <T> &v3) {
	    Vector3DBase <T> v = Cross(v2 - v1, v3 - v1);
	    v.Normalize();
	    return v;
	}
};

typedef Point3DBase <float> Point3D;
typedef Point3DBase <double> PointDouble3D;

template <class T>

class DST_API Vector4DBase {
public:
	T x;
	T y;
	T z;
	T w;

	Vector4DBase() {}

	Vector4DBase(T r, T s, T t, T u) {
		x = r;
		y = s;
		z = t;
		w = u;
	}

	Vector4DBase(const Vector3DBase <T> & v, T u) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = u;
	}

	Vector4DBase(const Vector3DBase <T> & v, const Point3D& q) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = - Dot(v, q);
	}
	
	Vector4DBase(const Vector3DBase <T> & v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = 0.0;
	}
		
	Vector4DBase(const Point3DBase <T> & p) {
		x = p.x;
		y = p.y;
		z = p.z;
		w = 1.0;
	}

	Vector4DBase(const Vector2DBase <T> & v) {
		x = v.x;
		y = v.y;
		z = 0.0;
		w = 0.0;
	}

	Vector4DBase(const Point2DBase <T> & p) {
		x = p.x;
		y = p.y;
		z = 0.0;
		w = 1.0;
	}
		
	Vector4DBase& Set(T r, T s, T t, T u) {
		x = r;
		y = s;
		z = t;
		w = u;
		return (*this);
	}

	Vector4DBase& Set(const Vector3DBase <T> &v, T u) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = u;
		return (*this);
	}
		
	Vector4DBase& Set(const Vector3DBase <T> &v, const Point3DBase <T> & q) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = - Dot(v, q);
		return (*this);
	}
		
	T& operator [](int k) {
		return ((&x)[k]);
	}

	const T& operator [](int k) const {
		return ((&x)[k]);
	}

	const Vector3DBase <T> & GetVector3D(void) const {
		return (*reinterpret_cast<const Vector3DBase <T> *>(this));
	}

	const Point3DBase <T> & GetPoint3D(void) const {
		return (*reinterpret_cast<const Point3DBase <T> *>(this));
	}

#if 1
	Vector3DBase <T> & GetVector3D(void) {
		return (*reinterpret_cast<Vector3DBase <T> *>(this));
	}

	Point3DBase <T>& GetPoint3D(void) {
		return (*reinterpret_cast<Point3DBase <T> *>(this));
	}
#else
	// Work around issues encountered with gcc 4.8.2 and -O2 and higher
	// on armhf.
	Vector3D GetVector3D(void) {
		return Vector3D(x, y, z);
	}

	Point3D GetPoint3D(void) {
		return Point3D(x, y, z);
	}
#endif

	Vector4DBase& operator =(const Vector3DBase <T> & v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = 0.0;
		return (*this);
	}
		
	Vector4DBase& operator =(const Point3D& p) {
		x = p.x;
		y = p.y;
		z = p.z;
		w = 1.0;
		return (*this);
	}

	Vector4DBase& operator =(const Vector2D& v) {
		x = v.x;
		y = v.y;
		z = 0.0;
		w = 0.0;
		return (*this);
	}

	Vector4DBase& operator =(const Point2D& p) {
		x = p.x;
		y = p.y;
		z = 0.0;
		w = 1.0;
		return (*this);
	}

	Vector4DBase& operator +=(const Vector4DBase& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return (*this);
	}

	Vector4DBase& operator +=(const Vector3D& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return (*this);
	}

	Vector4DBase& operator +=(const Vector2D& v) {
		x += v.x;
		y += v.y;
		return (*this);
	}

	Vector4DBase& operator -=(const Vector4DBase& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return (*this);
	}

	Vector4DBase& operator -=(const Vector3D& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return (*this);
	}

	Vector4DBase& operator -=(const Vector2D& v) {
		x -= v.x;
		y -= v.y;
		return (*this);
	}

	Vector4DBase& operator *=(T t) {
		x *= t;
		y *= t;
		z *= t;
		w *= t;
		return (*this);
	}

	Vector4DBase& operator /=(T t) {
		T f = 1.0 / t;
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return (*this);
	}

#if 0
	// * operator is pair-wise multiplication.
	Vector4DBase& operator *=(const Vector4DBase& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return (*this);
	}
#endif

	Vector4DBase operator -(void) const {
		return (Vector4DBase(- x, - y, - z, - w));
	}

	Vector4DBase operator +(const Vector4DBase& v) const {
		return (Vector4DBase(x + v.x, y + v.y, z + z, w + v.w));
	}

	Vector4DBase operator +(const Vector3DBase <T> & v) const {
		return (Vector4DBase(x + v.x, y + v.y, z + v.z, w));
	}

	Vector4DBase operator +(const Vector2DBase <T> & v) const {
		return (Vector4DBase(x + v.x, y + v.y, z, w));
	}

	Vector4DBase operator -(const Vector4DBase& v) const {
		return (Vector4DBase(x - v.x, y - v.y, z - v.z, w - v.w));
	}

	Vector4DBase operator -(const Vector3DBase <T> & v) const {
		return (Vector4DBase(x - v.x, y - v.y, z - v.z, w));
	}

	Vector4DBase operator -(const Vector2DBase <T> & v) const {
		return (Vector4DBase(x - v.x, y - v.y, z, w));
	}

	Vector4DBase operator *(float t) const {
		return (Vector4DBase(x * t, y * t, z * t, w * t));
	}

	Vector4DBase operator /(float t) const {
		float f = 1.0 / t;
		return (Vector4DBase(x * f, y * f, z * f, w * f));
	}

	// * is pair-wise multiplication
	Vector4DBase operator *(const Vector4DBase& v) const {
		return (Vector4DBase(x * v.x, y * v.y, z * v.z, w * v.w));
	}

	Vector4DBase& Normalize(void) {
		return (*this *= dstInvSqrt(x * x + y * y + z * z + w * w));
	}

	Vector4DBase& OrientPlaneTowardsPoint(const Point3DBase <T> &P) {
		if (Dot((*this), P) < 0)
			return (*this) = - (*this);
	        else
			return (*this);
	}

	// Return text respresentation. To be freed with delete [].
	char *GetString() const;

	bool operator ==(const Vector4DBase& v) const {
		return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
	}

	bool operator !=(const Vector4DBase& v) const {
		return ((x != v.x) || (y != v.y) || (z != v.z) || (w != v.w));
	}

	friend Vector4DBase operator *(float t, const Vector4DBase& v) {
		return (Vector4DBase(t * v.x, t * v.y, t * v.z, t * v.w));
	}

	friend T Dot(const Vector4DBase& v1, const Vector4DBase& v2) {
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
	}

	friend T Dot(const Vector4DBase& v1, const Vector3D& v2) {
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}

	friend T Dot(const Vector3D& v1, const Vector4DBase& v2) {
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}

	friend T Dot(const Vector4DBase& v, const Point3D& p) {
		return (v.x * p.x + v.y * p.y + v.z * p.z + v.w);
	}

	friend T Dot(const Point3D& v1, const Vector4DBase& v2) {
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v2.w);
	}

	friend T Dot(const Vector4DBase& v1, const Vector2D& v2) {
		return (v1.x * v2.x + v1.y * v2.y);
	}

	friend T Dot(const Vector2D& v1, const Vector4DBase& v2) {
		return (v1.x * v2.x + v1.y * v2.y);
	}

	friend T Dot(const Vector4DBase& v, const Point2D& p) {
		return (v.x * p.x + v.y * p.y + v.w);
	}

	friend T Dot(const Point2D& v1, const Vector4DBase& v2) {
		return (v1.x * v2.x + v1.y * v2.y + v2.w);
	}

	friend Vector3DBase <T> Cross(const Vector4DBase <T> & v1, const Vector3DBase <T> & v2) {
		return (Vector3D(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x));
	}

	friend Vector4DBase ProjectOnto(const Vector4DBase& v1, const Vector4DBase& v2) {
		return (v2 * Dot(v1, v2));
	}
	friend T SquaredMag(const Vector4DBase& v) {
		return (v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	}

	friend T Magnitude(const Vector4DBase& v) {
		return dstSqrt(SquaredMag(v));
	}

	friend T InverseMag(const Vector4DBase& v) {
		return dstInvSqrt(SquaredMag(v));
	}
};

typedef Vector4DBase <float> Vector4D DST_ALIGNED(16);
typedef Vector4DBase <double> VectorDouble4D DST_ALIGNED(32);


template <class T>
inline Vector4DBase <T> dstPlaneFromPoints(const Point3DBase <T>& v1, const Point3DBase <T>& v2,
const Point3DBase <T> & v3) {
	Vector3DBase <T> aux1 = v2 - v1;
	Vector3DBase <T> aux2 = v3 - v1;
	Vector3DBase <T> normal = Cross(aux2, aux1);
	normal.Normalize();
	T distance = - Dot(normal, v2);
	return Vector4DBase <T>(normal, distance);
}


// Define some convenient inline functions specifically using the Vector classes.

template <class T>
static inline bool AlmostEqual(const Vector2DBase <T> & v1, const Vector2DBase <T> & v2) {
	return AlmostEqual(v1.x, v2.x) && AlmostEqual(v1.y, v2.y);
}

template <class T>
static inline bool AlmostEqual(const Vector3DBase <T> & v1, const Vector3DBase <T> & v2) {
	return AlmostEqual(v1.x, v2.x) && AlmostEqual(v1.y, v2.y) && AlmostEqual(v1.z, v2.z);
}

template <class T>
static inline bool AlmostEqual(const Vector2DBase <T> & v1, const Vector2DBase <T> & v2,
T epsilon) {
	return AlmostEqual(v1.x, v2.x, epsilon) && AlmostEqual(v1.y, v2.y, epsilon);
}

template <class T>
static inline bool AlmostEqual(const Vector3DBase <T> & v1, const Vector3DBase <T> & v2,
T epsilon) {
	return AlmostEqual(v1.x, v2.x, epsilon) && AlmostEqual(v1.y, v2.y, epsilon) &&
		AlmostEqual(v1.z, v2.z, epsilon);
}

template <class T>
static inline bool AlmostEqual(const Point2DBase <T> & v1, const Point2DBase <T> & v2) {
	return AlmostEqual(v1.x, v2.x) && AlmostEqual(v1.y, v2.y);
}

template <class T>
static inline bool AlmostEqual(const Point3DBase <T> & v1, const Point3DBase <T> & v2) {
	return AlmostEqual(v1.x, v2.x) && AlmostEqual(v1.y, v2.y) && AlmostEqual(v1.z, v2.z);
}

template <class T>
static inline bool AlmostEqual(const Point2DBase <T> & v1, const Point2DBase <T> & v2,
T epsilon) {
	return AlmostEqual(v1.x, v2.x, epsilon) && AlmostEqual(v1.y, v2.y, epsilon);
}

template <class T>
static inline bool AlmostEqual(const Point3DBase <T> & v1, const Point3DBase <T> & v2,
T epsilon) {
	return AlmostEqual(v1.x, v2.x, epsilon) && AlmostEqual(v1.y, v2.y, epsilon) &&
		AlmostEqual(v1.z, v2.z, epsilon);
}

static inline float max3f(const Vector3D& V) {
    float m = V.x;
    if (V.y > m)
        m = V.y;
    if (V.z > m)
        m = V.z;
    return m;
}

static inline Vector3D maxf(const Vector3D& V1, const Vector3D& V2) {
    return Vector3D(maxf(V1.x, V2.x), maxf(V1.y, V2.y), maxf(V1.z, V2.z));
}

// Vector functions with optional SIMD (SSE, NEON etc) support.

// Calculate an array of dot products.

DST_API void CalculateDotProducts(int n, const Vector3D *v1, const Vector3D *v2,
float *dot);

DST_API void CalculateDotProducts(int n, const Vector4D *v1, const Vector4D *v2,
float *dot);

// Calculate array of dot products of vector array v1 with constant vector v2.

DST_API void CalculateDotProductsWithConstantVector(int n, const Vector3D *v1, const Vector3D& v2,
float *dot);

DST_API void CalculateDotProductsWithConstantVector(int n, const Vector4D *v1, const Vector4D& v2,
float *dot);

// Calculate array of dot products of point vector array v1 with constant vector v2.

DST_API void CalculateDotProductsWithConstantVector(int n, const Point3D *p1, const Vector4D& v2,
float *dot);

// Calculate array of dot products of point vector array v1 with constant vector v2,
// and count the number of dot products < 0.

DST_API void CalculateDotProductsWithConstantVectorAndCountNegative(int n, const Point3D *p1,
const Vector4D& v2, float *dot, int& negative_count);

// Determine the minimum and maximum dot product of an array of vertices with a
// given constant vector.

DST_API void CalculateMinAndMaxDotProduct(int nu_vertices, Vector3D *vertex,
const Vector3D& v2, float& min_dot_product, float& max_dot_product);

DST_API void CalculateMinAndMaxDotProduct(int nu_vertices, Vector4D *vertex,
const Vector4D& v2, float& min_dot_product, float& max_dot_product);

// Determine the minimum and maximum dot products of an array of vertices with three
// constant vectors.

DST_API void CalculateMinAndMaxDotProductWithThreeConstantVectors(int nu_vertices,
Vector3D *vertex, const Vector3D* v2, float* min_dot_product, float* max_dot_product);

DST_API void CalculateMinAndMaxDotProductWithThreeConstantVectors(int nu_vertices,
Vector4D *vertex, const Vector4D *v2, float *min_dot_product, float *max_dot_product);

// Determine the indices within an array of vertices that have the minimum and
// maximum dot product with a given constant vector.

DST_API void GetIndicesWithMinAndMaxDotProduct(int nu_vertices, Vector3D *vertex,
const Vector3D& v2, int& i_Pmin, int& i_Pmax);

DST_API void GetIndicesWithMinAndMaxDotProduct(int nu_vertices, Vector4D *vertex,
const Vector4D& v2, int& i_Pmin, int& i_Pmax);

#endif // defined(__DST_VECTOR_MATH__)

