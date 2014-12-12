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

#ifndef __DST_COLOR_H__
#define __DST_COLOR_H__

#include "dstVectorMath.h"

// Color class derived from Vector3D.

class DST_API Color : public Vector3D
{
	public:
		Color() {}
		
		Color(float r, float g, float b) : Vector3D(r, g, b) {}

		Color(const Vector3D& v)
		{
			r = v.x;
			g = v.y;
			b = v.z;
		}

		Vector3D& GetVector3D(void)
		{
			return (*this);
		}
		
		const Vector3D& GetVector3D(void) const
		{
			return (*this);
		}

                DST_API Color& SetRGB888(int r8, int g8, int b8);
		DST_API Color& SetRandom(void);

		DST_API Color GetLinearFromSRGB() const;
		DST_API Color GetSRGBFromLinear() const;
		DST_API float LinearIntensity() const;
                DST_API float SRGBIntensity() const;
                DST_API unsigned int GetRGBX8() const;
                DST_API float GetCompressed() const;
};

// We want overload the * operator to do pairwise multiplication.

inline Color operator *(const Color& c1, const Color& c2)
{
	return (Color(c1.r * c2.r, c1.g * c2.g, c1.b * c2.b));
}

inline Color operator *(const Color& c, const Vector3D& v)
{
	return (Color(c.r * v.x, c.g * v.y, c.b * v.z));
}

inline Color operator *(const Vector3D& v, const Color& c)
{
	return (Color(v.x * c.r, v.y * c.g, v.z * c.b));
}

float InverseSRGBGamma(float c);

float SRGBGamma(float c);

#endif // defined(__DST_COLOR_H__)

