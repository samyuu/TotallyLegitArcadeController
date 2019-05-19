#pragma once
#include <iostream>
#include <Windows.h>
#include "Patch.h"

namespace Patch
{
	class Patcher
	{
	public:
		Patcher();
		~Patcher();
		bool LoadPatchFile(std::string patchFile);
		bool PatchProgram(std::string executablePath);

	private:
		Patch* patch;
	};
}
