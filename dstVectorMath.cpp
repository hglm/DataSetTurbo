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

#include <stdio.h>

#include "dstVectorMath.h"
#include "dstColor.h"
#include "dstRandom.h"

// Additional member functions for debugging.

template <>
char *Vector3D::GetString() const {
	char *s = new char[80];
	sprintf(s, "Vector3D(%.10G, %.10G, %.10G)", x, y, z);
	return s;
}

template <>
char *Vector3DPadded::GetString() const {
	char *s = new char[80];
	sprintf(s, "Vector3DPadded(%.10G, %.10G, %.10G)", x, y, z);
	return s;
}

template <>
char *Point3D::GetString() const {
	char *s = new char[80];
	sprintf(s, "Point3D(%.10G, %.10G, %.10G)", x, y, z);
	return s;
}

template <>
char *Point3DPadded::GetString() const {
	char *s = new char[80];
	sprintf(s, "Point3DPadded(%.10G, %.10G, %.10G)", x, y, z);
	return s;
}

template <>
char *VectorDouble3D::GetString() const {
	char *s = new char[80];
	sprintf(s, "VectorDouble3D(%.10G, %.10G, %10G)", x, y, z);
	return s;
}

template <>
char *PointDouble3D::GetString() const {
	char *s = new char[80];
	sprintf(s, "PointDouble3D(%.10G, %.10G, %10G)", x, y, z);
	return s;
}

template <>
char *Vector4D::GetString() const {
    char *s = new char[80];
    sprintf(s, "Vector4D(%.10G, %.10G, %.10G, %.10G)", x, y, z, w);
    return s;
}

template <>
char *VectorDouble4D::GetString() const {
    char *s = new char[128];
    sprintf(s, "VectorDouble4D(%.10G, %.10G, %.10G, %.10G)", x, y, z, w);
    return s;
}

// Additional color member functions.

Color& Color::SetRGB888(int r8, int g8, int b8) {
    const float f = 1.0f / 255.0f;
    r = (float)r8 * f;
    g = (float)g8 * f;
    b = (float)b8 * f;
    return (*this);
}

Color& Color::SetRandom() {
    dstRNG *rng = dstGetDefaultRNG();
    r = rng->RandomFloat(1.0f);
    g = rng->RandomFloat(1.0f);
    b = rng->RandomFloat(1.0f);
    return (*this);
}

static const Vector3D Crgb = Vector3D(
    0.212655f, // Red factor
    0.715158f, // Green factor
    0.072187f  // Blue factor
    );

// Inverse of sRGB "gamma" function. (approx 2.2).
float InverseSRGBGamma(float c) {
    if (c <= 0.04045f)
        return c / 12.92f;
    else 
        return powf(((c + 0.055f) / (1.055f)), 2.4);
}

// sRGB "gamma" function (approx 2.2).
float SRGBGamma(float v) {
    if (v <= 0.0031308f)
        v *= 12.92f;
    else
        v = 1.055f * powf(v, 1.0f / 2.4f) - 0.055f;
    return v;
}

Color Color::GetLinearFromSRGB() const {
    return Color(
        InverseSRGBGamma(r),
        InverseSRGBGamma(g),
        InverseSRGBGamma(b)
        );
}

Color Color::GetSRGBFromLinear() const {
    return Color(
        SRGBGamma(r),
        SRGBGamma(g),
        SRGBGamma(b)
        );
}

float Color::LinearIntensity() const {
    return Dot((*this), Crgb);
}

float Color::SRGBIntensity() const {
    return SRGBGamma(GetLinearFromSRGB().LinearIntensity());
}

unsigned int Color::GetRGBX8() const {
    unsigned int pixel;
    pixel = (unsigned int)floorf(r * 255.0f + 0.5f) +
        ((unsigned int)floorf(g * 255.0f + 0.5f) << 8) +
        ((unsigned int)floorf(b * 255.0f + 0.5f) << 16) + 0xFF000000;
    return pixel;
}

float Color::GetCompressed() const {
    // Return compressed representation of color stored in a single float.
    const float f1 = 1.0f / 256.0f; 
    const float f2 = 1.0f / 65536.0f;
    const float f3 = 1.0f / 16777216.0f;
    float r8 = floorf(r * 255.0f + 0.5f);
    float g8 = floorf(g * 255.0f + 0.5f);
    float b8 = floorf(b * 255.0f + 0.5f);
    return r8 * f1 + g8 * f2 + b8 * f3;
    // The value can be decompressed as follows:
/*
    float r8 = floorf(cc * 256.0f);
    cc -= r8 * (1.0f / 16.0f);
    float g8 = floorf(cc * 65536.0f);
    cc -= g8 * (1.0f / 65536.0f);
    float b8 = floorf(cc * 16777216.0f);
    Color c;
    c.SetRGB888(r8, g8, b8);
*/
}


