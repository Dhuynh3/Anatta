#include "Security.h"

Security::Security() {
	
	NtCreateThreadEx = (pfnNtCreateThreadEx)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx");
	NtTerminateThread = (pfnNtTerminateThread)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtTerminateThread");

}

bool Security::AllocateConsole() {
	if (AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
		return true;
	}
	return false;
}




bool Security::RunThread(PVOID StartRoutine, std::string uniqueName, PVOID args) {
	
	HANDLE ThreadHandle;
	NTSTATUS Status = this->NtCreateThreadEx(&ThreadHandle, MAXIMUM_ALLOWED, nullptr, (HANDLE)-1, StartRoutine, args, 0x40, 0, 0, 0, nullptr);
	if (!NT_SUCCESS(Status)) {
		return false;
	}

	tHandleList.insert(std::pair<std::string, HANDLE>(uniqueName, ThreadHandle));
	return true;
}

bool Security::CloseThread(std::string uniqueName) {

	std::map<std::string, HANDLE>::iterator it;
	it = tHandleList.find(uniqueName); 

	if (it != tHandleList.end()) { // If the key exists.
		NTSTATUS Status = this->NtTerminateThread(it->second, 0);
		if (!NT_SUCCESS(Status)) {
			return false;
		}
		NtClose(it->second);
		tHandleList.erase(it);
		return true;
	}

	return false; // Thread not found.
}

std::wstring Security::RandomWString(size_t length)
{
	srand(time(NULL));
	
	auto randchar = []() -> wchar_t
	{
		const wchar_t charset[] =
			L"0123456789"
			L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			L"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};

	std::wstring str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}