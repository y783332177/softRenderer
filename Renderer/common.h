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

#endif