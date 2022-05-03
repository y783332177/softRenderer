#pragma once
#ifndef __COMMON_H__
#define __COMMON_H__

#include "VectorNT.h"
#include "Matrix.h"

#include <string>
#include <sstream>
#include <vector>

extern Mat4x4f worldMatrix;
extern Mat4x4f viewMatrix;
extern Mat4x4f projectionMatrix;
extern Mat4x4f mvp;

class common
{
};

void Stringsplit(std::string& str, const char& split, std::vector<std::string>& rst);

void StoiVUvN(std::vector<std::string>& rst, int& vIndex, int& uvIndex, int& nIndex);

inline float Clamp(const float& value, const float& min = 0, const float& max = 1)
{
	return std::max(min, std::min(value, max));
}

inline float Interpolate(const float& min, const float& max, const float& gradient)
{
	return min + (max - min) * Clamp(gradient);
}

inline Vector3f Interpolate(const Vector3f& barycentricCoord, const Vector3f& v0, const Vector3f& v1, const Vector3f& v2)
{
	return (v0 * barycentricCoord[0] + v1 * barycentricCoord[1] + v2 * barycentricCoord[2]);
}

inline Vector2f Interpolate(const Vector3f& barycentricCoord, const Vector2f& v0, const Vector2f& v1, const Vector2f& v2)
{
	return (v0 * barycentricCoord[0] + v1 * barycentricCoord[1] + v2 * barycentricCoord[2]);
}

inline int RoundF2I(const float& value)
{
	return (int)(value + 0.5f);
}

inline Vector3f PVec4f2Vec3f(const Vector4f& v)
{
	return Vector3f(v[0] / v[3], v[1] / v[3], v[2] / v[3]);
}

static Vector3f computeBarycentric2D(float x, float y, const Vector3f* v)
{
	float c1 = (x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * y + v[1].x * v[2].y - v[2].x * v[1].y) / (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x * v[2].y - v[2].x * v[1].y);
	float c2 = (x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * y + v[2].x * v[0].y - v[0].x * v[2].y) / (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x * v[0].y - v[0].x * v[2].y);
	float c3 = (x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * y + v[0].x * v[1].y - v[1].x * v[0].y) / (v[2].x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * v[2].y + v[0].x * v[1].y - v[1].x * v[0].y);
	return { c1,c2,c3 };
}

#endif