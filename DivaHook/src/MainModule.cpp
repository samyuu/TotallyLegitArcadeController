#include "MainModule.h"
#include <filesystem>

namespace DivaHook
{
	typedef std::filesystem::path fspath;

	std::string *MainModule::moduleDirectory;

	const wchar_t* MainModule::DivaWindowName = L"Hatsune Miku Project DIVA Arcade Future Tone";
	const wchar_t* MainModule::GlutDefaultName = L"GLUT";

	HWND MainModule::DivaWindowHandle;
	HMODULE MainModule::Module;

	std::string MainModule::GetModuleDirectory()
	{
		if (moduleDirectory == nullptr)
		{
			CHAR modulePathBuffer[MAX_PATH];
			GetModuleFileNameA(MainModule::Module, modulePathBuffer, MAX_PATH);

			fspath configPath = fspath(modulePathBuffer).parent_path();
			moduleDirectory = new std::string(configPath.u8string());
		}

		return *moduleDirectory;
	}

	RECT MainModule::GetWindowBounds()
	{
		RECT windowRect;
		GetWindowRect(DivaWindowHandle, &windowRect);

		return windowRect;
	}
}