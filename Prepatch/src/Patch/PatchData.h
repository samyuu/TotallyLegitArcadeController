#pragma once
#include <Windows.h>

namespace Patch
{
	struct PatchData
	{
		DWORD64 Address;
		DWORD Length;
		BYTE* OriginalBytes;
		BYTE* PatchedBytes;

		PatchData();
		~PatchData();
	};
}
