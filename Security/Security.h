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

#include "base64.h"
#include "../Anatta.h"
#include "../Hwid/Hwid.h"



class Security : public HWID {
public:
	Security(); 
	bool AllocateConsole();
	bool RunThread(PVOID StartRoutine, std::string uniqueName, PVOID args); 
	bool CloseThread(std::string uniqueName);
	bool DebuggerCheck(PVOID args);
	void FakeExtendImage(PBYTE modbaseaddr);

	void Death();

	std::string GetTextHash();
	std::string CalcHash256(const std::filesystem::path& p);
	std::string Myexepath();
	
	std::wstring RandomWString(size_t length);


	std::string RSA_Encrypt(std::string input, RSA::PublicKey publickey);
	std::string RSA_Decrypt(std::string input, RSA::PrivateKey privatekey);
	
	


	std::string AES_Decrypt(const std::string str_in, const std::string key, const std::string iv, size_t* length);
	std::string AES_Encrypt(const std::string str_in, const std::string key, const std::string iv);

	
	std::map<std::string, HANDLE> tHandleList;
	pfnNtCreateThreadEx NtCreateThreadEx{ nullptr };
	pfnNtTerminateThread NtTerminateThread{ nullptr };
	int test = 0;

	AutoSeededRandomPool rng;
	RSA::PrivateKey LoaderPrivateKey;
	RSA::PublicKey LoaderPublicKey;

	RSA::PublicKey ServerPublicKey;

	std::string AES_KEY{ ("") };
	std::string AES_IV{ ("") };
};

extern Security Secure;

#endif SECURITY_H