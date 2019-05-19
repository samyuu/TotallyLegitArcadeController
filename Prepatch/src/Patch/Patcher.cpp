#include "Patcher.h"

namespace Patch
{
	static bool EndsWith(const std::string& str, const std::string& suffix)
	{
		return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
	}

	Patcher::Patcher()
	{
	}

	Patcher::~Patcher()
	{
		if (patch != nullptr)
			delete patch;
	}

	bool Patcher::LoadPatchFile(std::string patchFile)
	{
		printf("Patcher::LoadPatchFile(): Opening patch file...\n");

		patch = new Patch();
		bool result = patch->ParseFile(patchFile);

		if (result)
		{
			printf("Patcher::LoadPatchFile(): Patch file loaded\n");
		}
		else
		{
			printf("Patcher::LoadPatchFile(): Failed to load patch file\n");
		}

		return result;
	}

	bool Patcher::PatchProgram(std::string executablePath)
	{
		if (patch == nullptr)
		{
			printf("Patcher::PatchProgram(): No patch has been loaded.\n");
			return FALSE;
		}

		if (!EndsWith(executablePath, ".exe"))
		{
			printf("Patcher::PatchProgram(): The input file is not an executable\n");
			return FALSE;
		}

		FILE* file = nullptr; 
		auto error = fopen_s(&file, executablePath.c_str(), "rb");

		if (file == nullptr)
		{
			printf("Patcher::PatchProgram(): Unable to open input file %s. Error: 0x%X\n", executablePath.c_str(), error);
			return FALSE;
		}

		fseek(file, 0, SEEK_END);
		int fileSize = ftell(file);
		rewind(file);

		BYTE* buffer = new BYTE[fileSize];

		fread(buffer, 1, fileSize, file);
		fclose(file);

		fopen_s(&file, executablePath.c_str(), "wb");
		fwrite(buffer, 1, fileSize, file);

		for (auto& patchData : patch->PatchDataVector)
		{
			printf("Patcher::PatchProgram(): Patching - Address: 0x%016I64X Data: ", patchData->Address);
			for (int i = 0; i < patchData->Length; i++)
				printf("%02X", patchData->PatchedBytes[i]);
			printf("\n");

			auto fileAddress = patchData->Address - patch->ImageBase;

			fseek(file, fileAddress, SEEK_SET);
			fwrite(patchData->PatchedBytes, 1, patchData->Length, file);
		}
		
		printf("Patcher::PatchProgram(): Patches applied\n");
		printf("Patcher::PatchProgram(): Closing file...\n");

		fclose(file);
		delete[] buffer;

		return TRUE;
	}
}
