#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include "PatchData.h"

namespace Patch
{
	class Patch
	{
	public:
		std::string FileName;
		std::string Author;
		std::string Version;
		std::string Date;
		DWORD64 ImageBase;
		std::vector<PatchData*> PatchDataVector;

		Patch();
		~Patch();
		bool ParseFile(const std::string& patchPath);
	};
}

