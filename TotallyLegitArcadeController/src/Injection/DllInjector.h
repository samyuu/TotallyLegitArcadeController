#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <tlhelp32.h>
#pragma comment(lib, "Shlwapi.lib")

namespace Injection
{
	class DllInjector
	{
	public:
		int GetProcessID(const std::string& processName);
		bool InjectDll(const int &processId, const std::string &dllPath);
		bool InjectDll(const std::string& processName, const std::string &dllPath);
	};
}
