#ifndef SECURITY_H
#define SECURITY_H

#include <Windows.h>
#include <algorithm>
#include <string>
#include <time.h>
#include <ntstatus.h>
#include <vector>
#include <map>

#pragma comment(lib, "ntdll.lib")

#include "../Anatta.h"



class Security {
public:
	Security(); 
	bool AllocateConsole();
	bool RunThread(PVOID StartRoutine, std::string uniqueName, PVOID args); 
	bool CloseThread(std::string uniqueName);
	



	
	std::wstring RandomWString(size_t length);

	
	std::map<std::string, HANDLE> tHandleList;
	pfnNtCreateThreadEx NtCreateThreadEx{ nullptr };
	pfnNtTerminateThread NtTerminateThread{ nullptr };
};

#endif SECURITY_H