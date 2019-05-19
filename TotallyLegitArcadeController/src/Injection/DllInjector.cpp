#include "DllInjector.h"

namespace Injection
{
	int DllInjector::GetProcessID(const std::string& processName)
	{
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 structprocsnapshot = { 0 };

		structprocsnapshot.dwSize = sizeof(PROCESSENTRY32);

		if (snapshot == INVALID_HANDLE_VALUE || Process32First(snapshot, &structprocsnapshot) == FALSE)
			return NULL;

		while (Process32Next(snapshot, &structprocsnapshot))
		{
			if (!strcmp(structprocsnapshot.szExeFile, processName.c_str()))
			{
				CloseHandle(snapshot);

				printf("DllInjector::GetProcessID(): Process %s found. Process ID: %d\n", processName.c_str(), structprocsnapshot.th32ProcessID);
				return structprocsnapshot.th32ProcessID;
			}
		}
		CloseHandle(snapshot);

		printf("DllInjector::GetProcessID(): Process %s not found\n", processName.c_str());
		return NULL;
	}

	bool DllInjector::InjectDll(const int &processId, const std::string &dllPath)
	{
		long dllSize = dllPath.length() + 1;
		HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

		if (hProc == NULL)
		{
			printf("DllInjector::InjectDll(): Failed to OpenProcess() target. Error: 0x%X\n", GetLastError());
			return false;
		}

		printf("DllInjector::InjectDll(): Opening target process...\n");

		LPVOID myAlloc = VirtualAllocEx(hProc, NULL, dllSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (myAlloc == NULL)
		{
			printf("DllInjector::InjectDll(): Failed to allocate memory in target process. Error: 0x%X\n", GetLastError());
			return false;
		}

		printf("DllInjector::InjectDll(): Allocating memory in targer process...\n");

		int isWriteOK = WriteProcessMemory(hProc, myAlloc, dllPath.c_str(), dllSize, 0);
		if (isWriteOK == 0)
		{
			printf("DllInjector::InjectDll(): Failed to WriteProcessMemory() in target. Error: 0x%X\n", GetLastError());
			return false;
		}
		printf("DllInjector::InjectDll(): Creating remote thread in target process...\n");

		DWORD threadId;
		LPTHREAD_START_ROUTINE addrLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(LoadLibrary("kernel32"), "LoadLibraryA");
		HANDLE threadReturn = CreateRemoteThread(hProc, NULL, 0, addrLoadLibrary, myAlloc, 0, &threadId);

		if (threadReturn == NULL)
		{
			printf("DllInjector::InjectDll(): Failed to CreateRemoteThread() in target. Error: 0x%X\n", GetLastError());
			return false;
		}

		if ((hProc != NULL) && (myAlloc != NULL) && (isWriteOK != ERROR_INVALID_HANDLE) && (threadReturn != NULL))
		{
			printf("DllInjector::InjectDll(): The Dll has been successfully injected\n");
			return true;
		}

		return false;
	}

	bool DllInjector::InjectDll(const std::string& processName, const std::string &dllPath)
	{
		return InjectDll(GetProcessID(processName), dllPath);
	}
}