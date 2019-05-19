#pragma once
#include "Operations.h"

namespace StringOperations
{
	std::vector<std::string> Split(const std::string& str, const std::string& delim)
	{
		std::vector<std::string> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);
			if (pos == std::string::npos)
				pos = str.length();

			std::string token = str.substr(prev, pos - prev);

			if (!token.empty())
				tokens.push_back(token);

			prev = pos + delim.length();
		} while (pos < str.length() && prev < str.length());

		return tokens;
	}

	void LeftTrim(std::string &s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
		{
			return !std::isspace(ch);
		}));
	}

	void RightTrim(std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch)
		{
			return !std::isspace(ch);
		}).base(), s.end());
	}

	void Trim(std::string &s)
	{
		LeftTrim(s);
		RightTrim(s);
	}
}