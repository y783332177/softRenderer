#include "common.h"

void Stringsplit(std::string &str, const char& split, std::vector<std::string>& rst)
{
	std::istringstream iss(str);
	std::string token;
	while (getline(iss, token, split))
	{
		rst.push_back(token);
	}
}

void StoiVUvN(std::vector<std::string>& rst, int& vIndex, int& uvIndex, int& nIndex)
{
	vIndex = stoi(rst[0]);
	uvIndex = stoi(rst[1]);
	nIndex = stoi(rst[2]);
}
