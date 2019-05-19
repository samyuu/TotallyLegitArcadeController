#include <stdio.h>
#include "ConfigFile.h"
#include "../Utilities/Operations.h"

namespace DivaHook::FileSystem
{
	ConfigFile::ConfigFile(const std::string &path) : TextFile(path)
	{
		return;
	}

	ConfigFile::ConfigFile(const std::string &directory, const std::string &file) : TextFile(directory, file)
	{
		return;
	}

	bool ConfigFile::TryGetValue(const std::string &key, std::string *&value)
	{
		auto pair = ConfigMap.find(key);

		bool found = pair != ConfigMap.end();

		value = found ? new std::string(pair->second) : nullptr;

		return found;
	}

	void ConfigFile::Parse(std::ifstream &fileStream)
	{
		std::string line;

		while (std::getline(fileStream, line))
		{
			if (IsComment(line))
				continue;

			auto splitline = Utilities::Split(line, "=");

			for (auto &line : splitline)
				Utilities::Trim(line);

			ConfigMap.insert(std::make_pair(splitline[0], splitline[1]));
		}
	}

	bool ConfigFile::IsComment(const std::string &line)
	{
		return line.size() <= 0 || line[0] == '#' || line._Starts_with("//");
	}
}
