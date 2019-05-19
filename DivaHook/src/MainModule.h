#pragma once
#include <Windows.h>
#include <string>

namespace DivaHook
{
	class MainModule
	{
	private:
		static std::string *moduleDirectory;

	public:
		static const wchar_t* DivaWindowName;
		static const wchar_t* GlutDefaultName;

		static HWND DivaWindowHandle;
		static HMODULE Module;

		static std::string GetModuleDirectory();
		static RECT GetWindowBounds();
	};
}
