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
// Several changes and additions have been made to the code.

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


class DST_API Vector2D
{
	public:
		
		float	x;
		float	y;
		
		Vector2D() {}
		
		Vector2D(float r, float s)
		{
			x = r;
			y = s;
		}
		
		Vector2D& Set(float r, float s)
		{
			x = r;
			y = s;
			return (*this);
		}

		float& operator [](int k)
		{
			return ((&x)[k]);
		}

		const float& operator [](int k) const
		{
			return ((&x)[k]);
		}
		
		Vector2D& operator +=(const Vector2D& v)
		{
			x += v.x;
			y += v.y;
			return (*this);
		}
		
		Vector2D& operator -=(const Vector2D& v)
		{
			x -= v.x;
			y -= v.y;
			return (*this);
		}
		
		Vector2D& operator *=(float t)
		{
			x *= t;
			y *= t;
			return (*this);
		}
		
		Vector2D& operator /=(float t)
		{
			float f = 1.0F / t;
			x *= f;
			y *= f;
			return (*this);
		}
		
		Vector2D& operator &=(const Vector2D& v)
		{
			x *= v.x;
			y *= v.y;
			return (*this);
		}
		
		Vector2D& Normalize(void)
		{
			return (*this *= invsqrtf(x * x + y * y));
		}
};


inline Vector2D operator -(const Vector2D& v)
{
	return (Vector2D(-v.x, -v.y));
}

inline Vector2D operator +(const Vector2D& v1, const Vector2D& v2)
{
	return (Vector2D(v1.x + v2.x, v1.y + v2.y));
}

inline Vector2D operator -(const Vector2D& v1, const Vector2D& v2)
{
	return (Vector2D(v1.x - v2.x, v1.y - v2.y));
}

inline Vector2D operator *(const Vector2D& v, float t)
{
	return (Vector2D(v.x * t, v.y * t));
}

inline Vector2D operator *(float t, const Vector2D& v)
{
	return (Vector2D(t * v.x, t * v.y));
}

inline Vector2D operator /(const Vector2D& v, float t)
{
	float f = 1.0F / t;
	return (Vector2D(v.x * f, v.y * f));
}

inline float operator *(const Vector2D& v1, const Vector2D& v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}

inline Vector2D operator &(const Vector2D& v1, const Vector2D& v2)
{
	return (Vector2D(v1.x * v2.x, v1.y * v2.y));
}

inline bool operator ==(const Vector2D& v1, const Vector2D& v2)
{
	return ((v1.x == v2.x) && (v1.y == v2.y));
}

inline bool operator !=(const Vector2D& v1, const Vector2D& v2)
{
	return ((v1.x != v2.x) || (v1.y != v2.y));
}

class DST_API Point2D : public Vector2D
{
	public:
		
		Point2D() {}
		
		Point2D(float r, float s) : Vector2D(r, s) {}
		
		Vector2D& GetVector2D(void)
		{
			return (*this);
		}
		
		const Vector2D& GetVector2D(void) const
		{
			return (*this);
		}
		
		Point2D& operator =(const Vector2D& v)
		{
			x = v.x;
			y = v.y;
			return (*this);
		}
		
		Point2D& operator *=(float t)
		{
			x *= t;
			y *= t;
			return (*this);
		}
		
		Point2D& operator /=(float t)
		{
			float f = 1.0F / t;
			x *= f;
			y *= f;
			return (*this);
		}
};


inline Point2D operator -(const Point2D& p)
{
	return (Point2D(-p.x, -p.y));
}

inline Point2D operator +(const Point2D& p1, const Point2D& p2)
{
	return (Point2D(p1.x + p2.x, p1.y + p2.y));
}

inline Point2D operator +(const Point2D& p, const Vector2D& v)
{
	return (Point2D(p.x + v.x, p.y + v.y));
}

inline Point2D operator -(const Point2D& p, const Vector2D& v)
{
	return (Point2D(p.x - v.x, p.y - v.y));
}

inline Vector2D operator -(const Point2D& p1, const Point2D& p2)
{
	return (Vector2D(p1.x - p2.x, p1.y - p2.y));
}

inline Point2D operator *(const Point2D& p, float t)
{
	return (Point2D(p.x * t, p.y * t));
}

inline Point2D operator *(float t, const Point2D& p)
{
	return (Point2D(t * p.x, t * p.y));
}

inline Point2D operator /(const Point2D& p, float t)
{
	float f = 1.0F / t;
	return (Point2D(p.x * f, p.y * f));
}


inline float Dot(const Vector2D& v1, const Vector2D& v2)
{
	return (v1 * v2);
}

inline Vector2D ProjectOnto(const Vector2D& v1, const Vector2D& v2)
{
	return (v2 * (v1 * v2));
}

inline float Magnitude(const Vector2D& v)
{
	return (sqrtf(v.x * v.x + v.y * v.y));
}

inline float InverseMag(const Vector2D& v)
{
	return (invsqrtf(v.x * v.x + v.y * v.y));
}

inline float SquaredMag(const Vector2D& v)
{
	return (v.x * v.x + v.y * v.y);
}


class DST_API Vector3D
{
	public:

		union {
			float	x;
			float	r;
		};
		union {
			float	y;
			float   g;
		};
		union {
			float	z;
			float	b;
		};
		
		Vector3D() {}
		
		Vector3D(float r, float s, float t)
		{
			x = r;
			y = s;
			z = t;
		}
		
		Vector3D(const Vector2D& v)
		{
			x = v.x;
			y = v.y;
			z = 0.0F;
		}
		
		Vector3D(const Vector2D& v, float u)
		{
			x = v.x;
			y = v.y;
			z = u;
		}
		
		Vector3D& Set(float r, float s, float t)
		{
			x = r;
			y = s;
			z = t;
			return (*this);
		}
		
		Vector3D& Set(const Vector2D& v, float u)
		{
			x = v.x;
			y = v.y;
			z = u;
			return (*this);
		}

                // Keep the non-const operator.
		float& operator [](int k)
		{
			return ((&x)[k]);
		}

		const float& operator [](int k) const
		{
			return ((&x)[k]);
		}

		Vector2D& GetVector2D(void)
		{
			return (*reinterpret_cast<Vector2D *>(this));
		}
		
		const Vector2D& GetVector2D(void) const
		{
			return (*reinterpret_cast<const Vector2D *>(this));
		}
		
		Point2D& GetPoint2D(void)
		{
			return (*reinterpret_cast<Point2D *>(this));
		}
		
		const Point2D& GetPoint2D(void) const
		{
			return (*reinterpret_cast<const Point2D *>(this));
		}
		
		Vector3D& operator =(const Vector2D& v)
		{
			x = v.x;
			y = v.y;
			z = 0.0F;
			return (*this);
		}
		
		Vector3D& operator +=(const Vector3D& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return (*this);
		}
		
		Vector3D& operator +=(const Vector2D& v)
		{
			x += v.x;
			y += v.y;
			return (*this);
		}
		
		Vector3D& operator -=(const Vector3D& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return (*this);
		}
		
		Vector3D& operator -=(const Vector2D& v)
		{
			x -= v.x;
			y -= v.y;
			return (*this);
		}
		
		Vector3D& operator *=(float t)
		{
			x *= t;
			y *= t;
			z *= t;
			return (*this);
		}
		
		Vector3D& operator /=(float t)
		{
			float f = 1.0F / t;
			x *= f;
			y *= f;
			z *= f;
			return (*this);
		}
		
		Vector3D& operator %=(const Vector3D& v)
		{
			float		r, s;
			
			r = y * v.z - z * v.y;
			s = z * v.x - x * v.z;
			z = x * v.y - y * v.x;
			x = r;
			y = s;
			
			return (*this);
		}
		
		Vector3D& operator &=(const Vector3D& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return (*this);
		}
		
		Vector3D& Normalize(void)
		{
			return (*this *= invsqrtf(x * x + y * y + z * z));
		}

		// Return text respresentation. To be freed with delete [].
		char *GetString() const;
};


inline Vector3D operator -(const Vector3D& v)
{
	return (Vector3D(-v.x, -v.y, -v.z));
}

inline Vector3D operator +(const Vector3D& v1, const Vector3D& v2)
{
	return (Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z));
}

inline Vector3D operator +(const Vector3D& v1, const Vector2D& v2)
{
	return (Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z));
}

inline Vector3D operator -(const Vector3D& v1, const Vector3D& v2)
{
	return (Vector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z));
}

inline Vector3D operator -(const Vector3D& v1, const Vector2D& v2)
{
	return (Vector3D(v1.x - v2.x, v1.y - v2.y, v1.z));
}

inline Vector3D operator *(const Vector3D& v, float t)
{
	return (Vector3D(v.x * t, v.y * t, v.z * t));
}

inline Vector3D operator *(float t, const Vector3D& v)
{
	return (Vector3D(t * v.x, t * v.y, t * v.z));
}

inline Vector3D operator /(const Vector3D& v, float t)
{
	float f = 1.0F / t;
	return (Vector3D(v.x * f, v.y * f, v.z * f));
}

inline float operator *(const Vector3D& v1, const Vector3D& v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

inline float operator *(const Vector3D& v1, const Vector2D& v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}

inline Vector3D operator %(const Vector3D& v1, const Vector3D& v2)
{
	return (Vector3D(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x));
}

inline Vector3D operator &(const Vector3D& v1, const Vector3D& v2)
{
	return (Vector3D(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z));
}

inline bool operator ==(const Vector3D& v1, const Vector3D& v2)
{
	return ((v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z));
}

inline bool operator !=(const Vector3D& v1, const Vector3D& v2)
{
	return ((v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z));
}


class DST_API Point3D : public Vector3D
{
	public:
		
		Point3D() {}
		
		Point3D(float r, float s, float t) : Vector3D(r, s, t) {}
		Point3D(const Vector2D& v) : Vector3D(v) {}
		Point3D(const Vector2D& v, float u) : Vector3D(v, u) {}
		Point3D(const Vector3D& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
		}
		
		Vector3D& GetVector3D(void)
		{
			return (*this);
		}
		
		const Vector3D& GetVector3D(void) const
		{
			return (*this);
		}
		
		Point2D& GetPoint2D(void)
		{
			return (*reinterpret_cast<Point2D *>(this));
		}
		
		const Point2D& GetPoint2D(void) const
		{
			return (*reinterpret_cast<const Point2D *>(this));
		}
		
		Point3D& operator =(const Vector3D& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return (*this);
		}
		
		Point3D& operator =(const Vector2D& v)
		{
			x = v.x;
			y = v.y;
			z = 0.0F;
			return (*this);
		}
		
		Point3D& operator *=(float t)
		{
			x *= t;
			y *= t;
			z *= t;
			return (*this);
		}
		
		Point3D& operator /=(float t)
		{
			float f = 1.0F / t;
			x *= f;
			y *= f;
			z *= f;
			return (*this);
		}
		
		Point3D& operator &=(const Vector3D& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return (*this);
		}
};


inline Point3D operator -(const Point3D& p)
{
	return (Point3D(-p.x, -p.y, -p.z));
}

inline Point3D operator +(const Point3D& p1, const Point3D& p2)
{
	return (Point3D(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z));
}

inline Point3D operator +(const Point3D& p, const Vector3D& v)
{
	return (Point3D(p.x + v.x, p.y + v.y, p.z + v.z));
}

inline Point3D operator +(const Vector3D& v, const Point3D& p)
{
	return (Point3D(v.x + p.x, v.y + p.y, v.z + p.z));
}

inline Vector3D operator -(const Point3D& p1, const Point3D& p2)
{
	return (Vector3D(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z));
}

inline Point3D operator -(const Point3D& p, const Vector3D& v)
{
	return (Point3D(p.x - v.x, p.y - v.y, p.z - v.z));
}

inline Point3D operator -(const Vector3D& v, const Point3D& p)
{
	return (Point3D(v.x - p.x, v.y - p.y, v.z - p.z));
}

inline Point3D operator *(const Point3D& p, float t)
{
	return (Point3D(p.x * t, p.y * t, p.z * t));
}

inline Point3D operator *(float t, const Point3D& p)
{
	return (Point3D(t * p.x, t * p.y, t * p.z));
}

inline Point3D operator /(const Point3D& p, float t)
{
	float f = 1.0F / t;
	return (Point3D(p.x * f, p.y * f, p.z * f));
}

inline float operator *(const Point3D& p1, const Point3D& p2)
{
	return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
}

inline float operator *(const Point3D& p, const Vector3D& v)
{
	return (p.x * v.x + p.y * v.y + p.z * v.z);
}

inline float operator *(const Vector3D& v, const Point3D& p)
{
	return (v.x * p.x + v.y * p.y + v.z * p.z);
}

inline float operator *(const Point3D& p, const Vector2D& v)
{
	return (p.x * v.x + p.y * v.y);
}

inline float operator *(const Vector2D& v, const Point3D& p)
{
	return (v.x * p.x + v.y * p.y);
}

inline Vector3D operator %(const Point3D& p1, const Point3D& p2)
{
	return (Vector3D(p1.y * p2.z - p1.z * p2.y, p1.z * p2.x - p1.x * p2.z, p1.x * p2.y - p1.y * p2.x));
}

inline Vector3D operator %(const Point3D& p, const Vector3D& v)
{
	return (Vector3D(p.y * v.z - p.z * v.y, p.z * v.x - p.x * v.z, p.x * v.y - p.y * v.x));
}

inline Vector3D operator %(const Vector3D& v, const Point3D& p)
{
	return (Vector3D(v.y * p.z - v.z * p.y, v.z * p.x - v.x * p.z, v.x * p.y - v.y * p.x));
}

inline Point3D operator &(const Point3D& p1, const Point3D& p2)
{
	return (Point3D(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z));
}

inline Point3D operator &(const Point3D& p, const Vector3D& v)
{
	return (Point3D(p.x * v.x, p.y * v.y, p.z * v.z));
}

inline Point3D operator &(const Vector3D& v, const Point3D& p)
{
	return (Point3D(v.x * p.x, v.y * p.y, v.z * p.z));
}

inline float Dot(const Vector3D& v1, const Vector3D& v2)
{
	return (v1 * v2);
}

inline float Dot(const Point3D& p, const Vector3D& v)
{
	return (p * v);
}

inline Vector3D Cross(const Vector3D& v1, const Vector3D& v2)
{
	return (v1 % v2);
}

inline Vector3D Cross(const Point3D& p, const Vector3D& v)
{
	return (p % v);
}

inline Vector3D ProjectOnto(const Vector3D& v1, const Vector3D& v2)
{
	return (v2 * (v1 * v2));
}

// Project v1 onto or in the direction of v2, with the angle limited by the specified value.

inline Vector3D ProjectOntoWithLimit(const Vector3D& v1, const Vector3D& v2, float min_cos_angle)
{
	float dot = Dot(v1, v2);
        if (dot < min_cos_angle)
            dot = min_cos_angle;
	return v2 * dot;
}

inline float Magnitude(const Vector3D& v)
{
	return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline float InverseMag(const Vector3D& v)
{
	return (invsqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline float SquaredMag(const Vector3D& v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

inline Vector3D CalculateNormal(const Point3D &v1, const Point3D &v2, const Point3D &v3) {
    Vector3D v = Cross(v2 - v1, v3 - v1);
    v.Normalize();
    return v;
}

class DST_API Vector4D
{
	public:
		
		float	x;
		float	y;
		float	z;
		float	w;
		
		Vector4D() {}
		
		Vector4D(float r, float s, float t, float u)
		{
			x = r;
			y = s;
			z = t;
			w = u;
		}
		
		Vector4D(const Vector3D& v, float u)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = u;
		}
		
		Vector4D(const Vector3D& v, const Point3D& q)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = -(v * q);
		}
		
		Vector4D(const Vector3D& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = 0.0F;
		}
		
		Vector4D(const Point3D& p)
		{
			x = p.x;
			y = p.y;
			z = p.z;
			w = 1.0F;
		}
		
		Vector4D(const Vector2D& v)
		{
			x = v.x;
			y = v.y;
			z = 0.0F;
			w = 0.0F;
		}
		
		Vector4D(const Point2D& p)
		{
			x = p.x;
			y = p.y;
			z = 0.0F;
			w = 1.0F;
		}
		
		Vector4D& Set(float r, float s, float t, float u)
		{
			x = r;
			y = s;
			z = t;
			w = u;
			return (*this);
		}
		
		Vector4D& Set(const Vector3D &v, float u)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = u;
			return (*this);
		}
		
		Vector4D& Set(const Vector3D &v, const Point3D& q)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = -(v * q);
			return (*this);
		}
		
		float& operator [](int k)
		{
			return ((&x)[k]);
		}

		const float& operator [](int k) const
		{
			return ((&x)[k]);
		}
		const Vector3D& GetVector3D(void) const
		{
			return (*reinterpret_cast<const Vector3D *>(this));
		}

		const Point3D& GetPoint3D(void) const
		{
			return (*reinterpret_cast<const Point3D *>(this));
		}

#if 0
		Vector3D& GetVector3D(void)
		{
			return (*reinterpret_cast<Vector3D *>(this));
		}

		Point3D& GetPoint3D(void)
		{
			return (*reinterpret_cast<Point3D *>(this));
		}
#else
		// Work around issues encountered with gcc 4.8.2 and -O2 and higher
		// on armhf.
		Vector3D GetVector3D(void)
		{
			return Vector3D(x, y, z);
		}

		Point3D GetPoint3D(void)
		{
			return Point3D(x, y, z);
		}
#endif
		
		Vector4D& operator =(const Vector3D& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			w = 0.0F;
			return (*this);
		}
		
		Vector4D& operator =(const Point3D& p)
		{
			x = p.x;
			y = p.y;
			z = p.z;
			w = 1.0F;
			return (*this);
		}
		
		Vector4D& operator =(const Vector2D& v)
		{
			x = v.x;
			y = v.y;
			z = 0.0F;
			w = 0.0F;
			return (*this);
		}
		
		Vector4D& operator =(const Point2D& p)
		{
			x = p.x;
			y = p.y;
			z = 0.0F;
			w = 1.0F;
			return (*this);
		}
		
		Vector4D& operator +=(const Vector4D& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return (*this);
		}
		
		Vector4D& operator +=(const Vector3D& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return (*this);
		}
		
		Vector4D& operator +=(const Vector2D& v)
		{
			x += v.x;
			y += v.y;
			return (*this);
		}
		
		Vector4D& operator -=(const Vector4D& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return (*this);
		}
		
		Vector4D& operator -=(const Vector3D& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return (*this);
		}
		
		Vector4D& operator -=(const Vector2D& v)
		{
			x -= v.x;
			y -= v.y;
			return (*this);
		}
		
		Vector4D& operator *=(float t)
		{
			x *= t;
			y *= t;
			z *= t;
			w *= t;
			return (*this);
		}
		
		Vector4D& operator /=(float t)
		{
			float f = 1.0F / t;
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return (*this);
		}
		
		Vector4D& operator &=(const Vector4D& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;
			return (*this);
		}
		
		Vector4D& Normalize(void)
		{
			return (*this *= invsqrtf(x * x + y * y + z * z + w * w));
		}

                Vector4D OrientPlaneTowardsPoint(const Point3D &P);

		// Return text respresentation. To be freed with delete [].
		char *GetString() const;
} DST_ALIGNED(16);


inline Vector4D operator -(const Vector4D& v)
{
	return (Vector4D(-v.x, -v.y, -v.z, -v.w));
}

inline Vector4D operator +(const Vector4D& v1, const Vector4D& v2)
{
	return (Vector4D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w));
}

inline Vector4D operator +(const Vector4D& v1, const Vector3D& v2)
{
	return (Vector4D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w));
}

inline Vector4D operator +(const Vector3D& v1, const Vector4D& v2)
{
	return (Vector4D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v2.w));
}

inline Vector4D operator +(const Vector4D& v1, const Vector2D& v2)
{
	return (Vector4D(v1.x + v2.x, v1.y + v2.y, v1.z, v1.w));
}

inline Vector4D operator +(const Vector2D& v1, const Vector4D& v2)
{
	return (Vector4D(v1.x + v2.x, v1.y + v2.y, v2.z, v2.w));
}

inline Vector4D operator -(const Vector4D& v1, const Vector4D& v2)
{
	return (Vector4D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w));
}

inline Vector4D operator -(const Vector4D& v1, const Vector3D& v2)
{
	return (Vector4D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w));
}

inline Vector4D operator -(const Vector3D& v1, const Vector4D& v2)
{
	return (Vector4D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, -v2.w));
}

inline Vector4D operator -(const Vector4D& v1, const Vector2D& v2)
{
	return (Vector4D(v1.x - v2.x, v1.y - v2.y, v1.z, v1.w));
}

inline Vector4D operator -(const Vector2D& v1, const Vector4D& v2)
{
	return (Vector4D(v1.x - v2.x, v1.y - v2.y, -v2.z, -v2.w));
}

inline Vector4D operator *(const Vector4D& v, float t)
{
	return (Vector4D(v.x * t, v.y * t, v.z * t, v.w * t));
}

inline Vector4D operator *(float t, const Vector4D& v)
{
	return (Vector4D(t * v.x, t * v.y, t * v.z, t * v.w));
}

inline Vector4D operator /(const Vector4D& v, float t)
{
	float f = 1.0F / t;
	return (Vector4D(v.x * f, v.y * f, v.z * f, v.w * f));
}

inline float operator *(const Vector4D& v1, const Vector4D& v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
}

inline float operator *(const Vector4D& v1, const Vector3D& v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

inline float operator *(const Vector3D& v1, const Vector4D& v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

inline float operator *(const Vector4D& v, const Point3D& p)
{
	return (v.x * p.x + v.y * p.y + v.z * p.z + v.w);
}

inline float operator *(const Point3D& v1, const Vector4D& v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v2.w);
}

inline float operator *(const Vector4D& v1, const Vector2D& v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}

inline float operator *(const Vector2D& v1, const Vector4D& v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}

inline float operator *(const Vector4D& v, const Point2D& p)
{
	return (v.x * p.x + v.y * p.y + v.w);
}

inline float operator *(const Point2D& v1, const Vector4D& v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v2.w);
}

inline Vector3D operator %(const Vector4D& v1, const Vector3D& v2)
{
	return (Vector3D(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x));
}

inline Vector4D operator &(const Vector4D& v1, const Vector4D& v2)
{
	return (Vector4D(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w));
}

inline bool operator ==(const Vector4D& v1, const Vector4D& v2)
{
	return ((v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z) && (v1.w == v2.w));
}

inline bool operator !=(const Vector4D& v1, const Vector4D& v2)
{
	return ((v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z) || (v1.w != v2.w));
}


inline float Dot(const Vector4D& v1, const Vector4D& v2)
{
	return (v1 * v2);
}

inline Vector4D ProjectOnto(const Vector4D& v1, const Vector4D& v2)
{
	return (v2 * (v1 * v2));
}

inline float Magnitude(const Vector4D& v)
{
	return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w));
}

inline float InverseMag(const Vector4D& v)
{
	return (invsqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w));
}

inline float SquaredMag(const Vector4D& v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

inline Vector4D PlaneFromPoints(const Point3D& v1, const Point3D& v2, const Point3D& v3) {
    Vector3D aux1 = v2 - v1;
    Vector3D aux2 = v3 - v1;
    Vector3D normal = Cross(aux2, aux1);
    normal.Normalize();
    float distance = - Dot(normal, v2);
    return Vector4D(normal, distance);
}

inline Vector4D Vector4D::OrientPlaneTowardsPoint(const Point3D &P) {
	if ((*this) * P < 0)
		return (*this) = - (*this);
        else
		return (*this);
}

// Define some convenient inline functions specifically using the Vector classes.

static inline bool AlmostEqual(const Vector2D& v1, const Vector2D& v2) {
    return AlmostEqual(v1.x, v2.x) && AlmostEqual(v1.y, v2.y);
}

static inline bool AlmostEqual(const Vector3D& v1, const Vector3D& v2) {
    return AlmostEqual(v1.x, v2.x) && AlmostEqual(v1.y, v2.y) && AlmostEqual(v1.z, v2.z);
}

static inline bool AlmostEqual(const Vector2D& v1, const Vector2D& v2, float epsilon) {
    return AlmostEqual(v1.x, v2.x, epsilon) && AlmostEqual(v1.y, v2.y, epsilon);
}

static inline bool AlmostEqual(const Vector3D& v1, const Vector3D& v2, float epsilon) {
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

