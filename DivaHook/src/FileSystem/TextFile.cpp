#include "TextFile.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace DivaHook::FileSystem
{
	TextFile::TextFile(const std::string &path)
	{
		FileName = path;
	}

	TextFile::TextFile(const std::string &directory, const std::string &file)
	{
		auto fullPath = directory + "/" + file;
		FileName = fullPath;
	}

	TextFile::~TextFile()
	{
	}

	bool TextFile::OpenRead()
	{
		fs::path configPath(FileName);

		if (!fs::exists(configPath))
			return false;

		std::ifstream fileStream(configPath.string().c_str());
		
		if (!fileStream.is_open())
			return false;

		Parse(fileStream);

		fileStream.close();

		return true;
	}
}
