#include "Injection/DllInjector.h"

const char* TLAC_VERSION = "1.10.00";

const std::string DIVA_PROCESS_NAME = "diva.exe";
const std::string DIVA_HOOK_DLL_NAME = "divahook.dll";

char waitDisplayStrings[8][6] =
{
	"o....",
	".o...",
	"..o..",
	"...o.",
	"....o",
	"...o.",
	"..o..",
	".o...",
};

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

void PrintProgramInfo()
{
	printf("// -------------------------------------------------\n");

#ifdef _DEBUG
	printf("// -- DEBUG_BUILD: --\n//\n");
#endif

	printf("// Totally Legit Arcade Controller (TLAC)			\n");
	printf("//													\n");
	printf("// v.%s                            -by samyuu       \n", TLAC_VERSION);
	printf("// -------------------------------------------------\n");
}

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi);
	return cbsi.dwCursorPosition;
}

void WaitExit(int duration = 6000, int iterations = 32 + 1)
{
	DWORD interval = duration / iterations;

	auto consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);

	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
	{
		auto prerPos = GetConsoleCursorPosition(consoleHandle);

		for (int i = 0; i < iterations; i++)
		{
			SetConsoleCursorPosition(consoleHandle, prerPos);
			printf("%s", waitDisplayStrings[i % 8]);
			Sleep(interval);
		}
	}
	cursorInfo.bVisible = true;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

bool DoesFileExist(std::string filePath)
{
	auto fileAttrib = GetFileAttributes(filePath.c_str());
	return fileAttrib != INVALID_FILE_ATTRIBUTES;
}

int main(int argc, char** argv)
{
	PrintProgramInfo();

	auto moduleDirectory = GetModuleDirectory();
	auto dllPath = moduleDirectory + "/" + DIVA_HOOK_DLL_NAME;

	if (!DoesFileExist(dllPath))
	{
		printf("main(): Unable to locate %s\n", DIVA_HOOK_DLL_NAME.c_str());
		printf("main(): Press enter to exit...");
		std::cin.get();
		return EXIT_FAILURE;
	}
	else
	{
		printf("main(): %s successfully located\n", DIVA_HOOK_DLL_NAME.c_str());
	}

	Injection::DllInjector injector;
	bool result = injector.InjectDll(DIVA_PROCESS_NAME, dllPath);

	if (!result)
	{
		printf("main(): Injection failed. Press enter to exit...");
		std::cin.get();
		return EXIT_FAILURE;
	}

	printf("main(): Exiting ");

	WaitExit();
	return EXIT_SUCCESS;
}
