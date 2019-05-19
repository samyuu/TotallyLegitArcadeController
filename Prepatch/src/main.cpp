#include "Patch/Patcher.h"

const std::string PATCH_FILE_FILE_NAME = "patch.txt";
const std::string DIVA_PROCESS_NAME = "diva.exe";

int GetDirectorySeperatorPosition(std::string path)
{
	for (int i = path.size() - 1; i >= 0; i--)
	{
		auto currentChar = path[i];
		if (currentChar == '\\' || currentChar == '/')
			return i;
	}

	return -1;
}

std::string GetModuleDirectory()
{
	HMODULE module = GetModuleHandleW(NULL);
	CHAR modulePathBuffer[MAX_PATH];
	GetModuleFileName(module, modulePathBuffer, MAX_PATH);

	auto modulePath = std::string(modulePathBuffer);
	int seperatorPos = GetDirectorySeperatorPosition(modulePath);

	if (seperatorPos != -1)
		return std::string(modulePathBuffer).substr(0, seperatorPos);

	return NULL;
}

bool DoesFileExist(std::string filePath)
{
	auto fileAttrib = GetFileAttributes(filePath.c_str());
	return fileAttrib != INVALID_FILE_ATTRIBUTES;
}

int main(int argc, char** argv)
{
	printf("main(): Checking %s location...\n", PATCH_FILE_FILE_NAME.c_str());

	auto moduleDirectory = GetModuleDirectory();
	auto patchPath = moduleDirectory + "/" + PATCH_FILE_FILE_NAME;

	if (!DoesFileExist(patchPath))
	{
		printf("main(): Unable to locate %s\n", PATCH_FILE_FILE_NAME.c_str());
		printf("main(): Press enter to exit...");
		std::cin.get();
		return EXIT_FAILURE;
	}
	else
	{
		printf("main(): %s successfully located\n", PATCH_FILE_FILE_NAME.c_str());
	}

	printf("main(): Checking executable location...\n");

	if (argc < 2)
	{
		printf("main(): Insufficient number of arguments supplied\n");
		printf("main(): Please drag the target %s on top of this executable to apply the patches\n", DIVA_PROCESS_NAME.c_str());
		printf("main(): Press enter to exit...");
		std::cin.get();
		return EXIT_FAILURE;
	}

	std::string executablePath = std::string(argv[1]);

	if (!DoesFileExist(executablePath))
	{
		printf("main(): Unable to locate %s\n", executablePath.c_str());
		printf("main(): Press enter to exit...");
		std::cin.get();
		return EXIT_FAILURE;
	}
	else
	{
		printf("main(): %s successfully located\n", executablePath.c_str());
	}

	Patch::Patcher patcher;
	patcher.LoadPatchFile(patchPath);
	patcher.PatchProgram(executablePath);

	printf("main(): Press enter to exit...");
	std::cin.get();
	return EXIT_SUCCESS;
}