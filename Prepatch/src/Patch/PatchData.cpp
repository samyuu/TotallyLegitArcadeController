#include "PatchData.h"

namespace Patch
{
	Patch::PatchData::PatchData()
	{
	}

	Patch::PatchData::~PatchData()
	{
		if (OriginalBytes != nullptr)
			delete[] OriginalBytes;

		if (PatchedBytes != nullptr)
			delete[] PatchedBytes;
	}
}
