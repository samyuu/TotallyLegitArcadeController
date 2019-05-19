#pragma once

#include "TextFile.h"
#include <unordered_map>

namespace DivaHook::FileSystem
{
	class ConfigFile : public TextFile
	{
	public:
		ConfigFile(const std::string &path);
		ConfigFile(const std::string &directory, const std::string &file);

		std::unordered_map<std::string, std::string> ConfigMap;

		bool TryGetValue(const std::string &key, std::string *&value);

	protected:
		virtual void Parse(std::ifstream &fileStream) override;

	private:
		bool IsComment(const std::string &line);
	};
}

