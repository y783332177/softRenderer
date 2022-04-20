#pragma once
#ifndef __COMMON_H__
#define __COMMON_H__
#include <string>
#include <sstream>
#include <vector>
class common
{
};

void Stringsplit(std::string str, const const char split, std::vector<std::string>& rst);

void StoiVUvN(std::vector<std::string>rst, int& vIndex, int& uvIndex, int& nIndex);

inline float Clamp(float value, float min = 0, float max = 1)
{
	return std::max(min, std::min(value, max));
}

inline float Interpolate(float min, float max, float gradient)
{
	return min + (max - min) * Clamp(gradient);
}

inline int RoundF2I(float value)
{
	return (int)(value + 0.5f);
}
#endif