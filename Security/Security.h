#ifndef SECURITY_H
#define SECURITY_H

#include <Windows.h>
#include <algorithm>
#include <string>
#include <time.h>
#include <ntstatus.h>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <filesystem>

#pragma comment(lib, "ntdll.lib")

#include "../Anatta.h"



class Security {
public:
	Security(); 
	bool AllocateConsole();
	bool RunThread(PVOID StartRoutine, std::string uniqueName, PVOID args); 
	bool CloseThread(std::string uniqueName);
	bool DebuggerCheck(PVOID args);
	void FakeExtendImage(PBYTE modbaseaddr);
	std::string GetTextHash();
	std::string CalcHash256(const std::filesystem::path& p);
	std::string Myexepath();
	
	std::wstring RandomWString(size_t length);

	
	std::map<std::string, HANDLE> tHandleList;
	pfnNtCreateThreadEx NtCreateThreadEx{ nullptr };
	pfnNtTerminateThread NtTerminateThread{ nullptr };
	int test = 0;
};

#endif SECURITY_H