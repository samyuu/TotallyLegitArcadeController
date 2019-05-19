#pragma once
#include <algorithm> 
#include <cctype>
#include <locale>
#include <vector>

namespace StringOperations
{
	std::vector<std::string> Split(const std::string& str, const std::string& delim);

	void LeftTrim(std::string &s);
	void RightTrim(std::string &s);
	void Trim(std::string &s);
}
