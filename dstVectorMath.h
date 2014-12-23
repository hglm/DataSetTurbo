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
		// Marker to recognize that the type is not Vector4D.
		T w_marker;
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

template <class T>
class DST_API Vector3DBasePadded : public Vector3DBase <T> {
public :
	Vector3DBasePadded <T> & operator =(const Vector3DBase <T> & v) {
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		return (*this);
	}
} DST_ALIGNED(sizeof(T) * 4);

typedef Vector3DBase <float> Vector3D;
typedef Vector3DBasePadded <float> Vector3DPadded;
typedef Vector3DBase <double> VectorDouble3D;
typedef Vector3DBasePadded <double> VectorDouble3DPadded;

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

template <class T>
class DST_API Point3DBasePadded : public Point3DBase <T> {
public :
	Point3DBasePadded <T> & operator =(const Vector3DBase <T> & v) {
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		return (*this);
	}

} DST_ALIGNED(sizeof(T) *4);

typedef Point3DBase <float> Point3D;
typedef Point3DBasePadded <float> Point3DPadded;
typedef Point3DBase <double> Point3DDouble;
typedef Point3DBasePadded <double> Point3DDoublePadded;

template <class T>
class DST_API Vector4DBase {
public:
	T x;
	T y;
	T z;
	union {
		T w;
		T w_marker;
	};

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

typedef Vector4DBase <float> Vector4D;
typedef Vector4DBase <double> Vector4DDouble;

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
// All arrays of vectors as well as resulting dot products must be aligned
// on a 16-byte boundary.

#if defined(DST_FIXED_SIMD) || defined(DST_NO_SIMD)
#define SIMD_FUNC(f) DST_FUNC_LOOKUP(f)
#include "dstSIMDFuncs.h"
#endif

// Calculate an array of dot products. v1, v2 and dot must be aligned on a 16-byte
// boundary.

template <class T>
DST_INLINE_ONLY void dstCalculateDotProductsNxN(int n, const T * DST_RESTRICT v1,
const T * DST_RESTRICT v2, float * DST_RESTRICT dot) {
	if (__builtin_offsetof(T, w_marker) == 12)
		return DST_FUNC_LOOKUP(dstCalculateDotProductsNxNV4)(n,
			(const float *)v1,(const float *)v2, dot);
	else if (sizeof(T) == 16)
		return DST_FUNC_LOOKUP(dstCalculateDotProductsNxNV3P)(n,
			(const float *)v1, (const float *)v2, dot);
	else
		return DST_FUNC_LOOKUP(dstCalculateDotProductsNxNV3)(n,
			(const float *)v1, (const float *)v2, dot);
}

// Calculate array of dot products of vector array v1 with vector v2. v1 and dot
// must be aligned on a 16-byte boundary.

template <class T, class U>
DST_INLINE_ONLY void dstCalculateDotProductsNx1(int n,
const T * DST_RESTRICT v1, const U& DST_RESTRICT v2, float * DST_RESTRICT dot) {
	if (__builtin_offsetof(T, w_marker) == 12)
		// T is Vector4D.
		return DST_FUNC_LOOKUP(dstCalculateDotProductsNx1V4)(n,
			(const float *)v1, (const float *)&v2, dot);
	else if (sizeof(T) == 16)
		// T is Vector3DPadded or Point3DPadded.
		if (__builtin_offsetof(U, w_marker) == 12)
			// U is Vector4D. In this case, we assume T is Point3DPadded (with
			// implicit w of 1.0f) because Vector3DPadded doesn't make sense.
			return DST_FUNC_LOOKUP(dstCalculateDotProductsNx1P3PV4)(n,
				(const float *)v1, (const float *)&v2, dot);
		else
			// U (v2) is assumed to be Vector3D. It doesn't matter if T is
			// Point3DPadded.
			return DST_FUNC_LOOKUP(dstCalculateDotProductsNx1V3PV3)(n,
				(const float *)v1, (const float *)&v2, dot);
	else
		// T is Vector3D or Point3D.
		if (__builtin_offsetof(U, w_marker) == 12)
			// U is Vector4D. In this case, we assume T is Point3D (with
			// implicit w of 1.0f) because Vector3D doesn't make sense.
			return DST_FUNC_LOOKUP(dstCalculateDotProductsNx1P3V4)(n,
				(const float *)v1, (const float *)&v2, dot);
		else
			// U (v2) is assumed to be Vector3D.
			return DST_FUNC_LOOKUP(dstCalculateDotProductsNx1V3)(n,
				(const float *)v1, (const float *)&v2, dot);
}

// Calculate array of dot products of point vector array p1 with constant vector v2,
// and count the number of dot products < 0. p1 must be aligned on a 16-byte
// boundary.

DST_INLINE_ONLY void dstCalculateDotProductsAndCountNegativeNx1(
int n, const Point3D * DST_RESTRICT p1, const Vector4D& DST_RESTRICT v2,
float * DST_RESTRICT dot, int& DST_RESTRICT negative_count) {
	DST_FUNC_LOOKUP(dstCalculateDotProductsAndCountNegativeNx1P3V4)(n, (const float *)p1,
		(const float *)&v2, dot, negative_count);
}

DST_INLINE_ONLY void dstCalculateDotProductsAndCountNegativeNx1(
int n, const Point3DPadded * DST_RESTRICT p1, const Vector4D& DST_RESTRICT v2,
float * DST_RESTRICT dot, int& DST_RESTRICT negative_count) {
	DST_FUNC_LOOKUP(dstCalculateDotProductsAndCountNegativeNx1P3PV4)(n, (const float *)p1,
		(const float *)&v2, dot, negative_count);
}

// Determine the minimum and maximum dot product of an array of vertices with a
// given constant vector. v1 must be aligned on a 16-byte boundary.

DST_INLINE_ONLY void dstCalculateMinAndMaxDotProductNx1(int n,
const Vector3D * DST_RESTRICT v1, const Vector3D& DST_RESTRICT v2,
float& DST_RESTRICT min_dot_product, float& DST_RESTRICT max_dot_product) {
	DST_FUNC_LOOKUP(dstCalculateMinAndMaxDotProductNx1V3)(n, (const float *)v1,
		(const float *)&v2, min_dot_product, max_dot_product);
}

DST_INLINE_ONLY void dstCalculateMinAndMaxDotProductNx1(int n,
const Vector4D * DST_RESTRICT v1, const Vector4D& DST_RESTRICT v2,
float& DST_RESTRICT min_dot_product, float& DST_RESTRICT max_dot_product) {
	DST_FUNC_LOOKUP(dstCalculateMinAndMaxDotProductNx1V4)(n, (const float *)v1,
		(const float *)&v2, min_dot_product, max_dot_product);
}

// Determine the minimum and maximum dot products of an array of vertices with three
// constant vectors. v1 must be aligned on a 16-byte boundary.

DST_INLINE_ONLY void dstCalculateMinAndMaxDotProductNx3(int n,
const Vector3D * DST_RESTRICT v1, const Vector3D * DST_RESTRICT v2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product) {
	DST_FUNC_LOOKUP(dstCalculateMinAndMaxDotProductNx3V3)(n, (const float *)v1,
		(const float *)v2, min_dot_product, max_dot_product);
}

DST_INLINE_ONLY void dstCalculateMinAndMaxDotProductNx3(int n,
const Vector4D * DST_RESTRICT v1, const Vector4D * DST_RESTRICT v2,
float * DST_RESTRICT min_dot_product, float * DST_RESTRICT max_dot_product) {
	DST_FUNC_LOOKUP(dstCalculateMinAndMaxDotProductNx3V4)(n, (const float *)v1,
		(const float *)v2, min_dot_product, max_dot_product);
}

// Determine the indices within an array of vertices that have the minimum and
// maximum dot product with a given constant vector. v1 must be aligned on a 16-byte
// boundary.

DST_INLINE_ONLY void dstGetIndicesWithMinAndMaxDotProductNx1(int n,
const Vector3D * DST_RESTRICT v1, const Vector3D& DST_RESTRICT v2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax) {
	DST_FUNC_LOOKUP(dstGetIndicesWithMinAndMaxDotProductNx1V3)(n, (const float *)v1,
		(const float *)&v2, i_Pmin, i_Pmax);
}

DST_INLINE_ONLY void dstGetIndicesWithMinAndMaxDotProductNx1(int n,
const Vector4D * DST_RESTRICT v1, const Vector4D& DST_RESTRICT v2,
int& DST_RESTRICT i_Pmin, int& DST_RESTRICT i_Pmax) {
	DST_FUNC_LOOKUP(dstGetIndicesWithMinAndMaxDotProductNx1V4)(n, (const float *)v1,
		(const float *)&v2, i_Pmin, i_Pmax);
}

#endif // defined(__DST_VECTOR_MATH__)

