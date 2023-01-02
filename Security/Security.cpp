#include "Security.h"



Security Secure;

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

bool Security::DebuggerCheck(PVOID args) {
	
	PPEB pPeb = (PPEB)(__readgsqword)(0x60);
	bool IsDebug = IsDebuggerPresent();
	BOOL IsDebugRemote = false; (CheckRemoteDebuggerPresent)((GetCurrentProcess)(), &IsDebugRemote);

	if (pPeb->BeingDebugged == 1 || 
		IsDebug == true ||
		IsDebugRemote == true) {

		
		return true;
	}

	
	return false;
}

void Security::FakeExtendImage(PBYTE modbaseaddr) {
	
	PPEBS pPeb = (PPEBS)(__readgsqword)(0x60);
	LIST_ENTRY* head = pPeb->Ldr->InMemoryOrderModuleList.Flink;
	LIST_ENTRY* node = head;
	
	// Do while will execute the condition block once before checking 
	do {
		PPLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD(node, LLDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		if (entry->DllBase == modbaseaddr) {
			entry->SizeOfImage = 0;
			break;
		}

		node = node->Flink;

	} while (head != node);
	

	head = pPeb->Ldr->InLoadOrderModuleList.Flink;
	node = head;
	
	do {

		PPLDR_DATA_TABLE_ENTRY Current = CONTAINING_RECORD(node, LLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

		if (Current->DllBase == modbaseaddr) {
			Current->SizeOfImage = 0;
			break;
		}

		node = node->Flink;

	} while (head != node);

	
	head = pPeb->Ldr->InInitializationOrderModuleList.Flink;
	node = head;

	do {

		PPLDR_DATA_TABLE_ENTRY En = CONTAINING_RECORD(node, LLDR_DATA_TABLE_ENTRY, InInitializationOrderLinks);

		if (En->DllBase == modbaseaddr) {
			En->SizeOfImage = 0;
			break;
		}

		node = node->Flink;

	} while (head != node);

}



bool Security::RunThread(PVOID StartRoutine, std::string uniqueName, PVOID args) {
	
	HANDLE ThreadHandle;
	
	NTSTATUS Status = this->NtCreateThreadEx(
		&ThreadHandle,
		MAXIMUM_ALLOWED, 
		nullptr, 
		(HANDLE)-1,
		StartRoutine,
		args,
		0x40, // This flag here makes it so our thread cannot be suspended
		0,
		0,
		0, 
		nullptr);
	
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

std::string Security::GetTextHash() {

	BYTE cbCalculatedImageHash[CryptoPP::SHA224::DIGESTSIZE];

	// .text is always 0x1000 + base addr
	LPVOID ImageBase = GetModuleHandle(NULL);

	PIMAGE_DOS_HEADER pDosHd = (PIMAGE_DOS_HEADER)ImageBase;
	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)((DWORD_PTR)ImageBase + pDosHd->e_lfanew);
	PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)(pNtHdr + 1);

	for (int i = 0; i < pNtHdr->FileHeader.NumberOfSections; ++i) {

		auto pCurrentSectionHeader = pSectionHeader + i;
		auto Characteristics = pCurrentSectionHeader->Characteristics;

		if (pCurrentSectionHeader->VirtualAddress == 0x1000) {

			LPVOID lpVirtualAddress = (LPVOID)((DWORD64)ImageBase + pSectionHeader->VirtualAddress);
			SIZE_T dwSizeOfRawData = pSectionHeader->Misc.VirtualSize;

			CryptoPP::SHA224 hash;
			hash.Update((PBYTE)lpVirtualAddress, dwSizeOfRawData);
			hash.Final(cbCalculatedImageHash);

			break;

		}

	}

	std::stringstream ss; ss << "0x";
	for (size_t i = 0; i < 10; ++i) {
		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2 * sizeof(BYTE)) << static_cast<unsigned>(cbCalculatedImageHash[i]);
	}

	return ss.str();
}

std::string Security::CalcHash256(const std::filesystem::path& p)
{
	
	CryptoPP::SHA256 hash;
	BYTE NewImageHash[CryptoPP::SHA256::DIGESTSIZE];

	FileSource(p.string().c_str(), true, new HashFilter(hash, new HexEncoder(new ArraySink(NewImageHash, sizeof(NewImageHash)))));

	return std::string(reinterpret_cast<const char*>(NewImageHash), sizeof(NewImageHash));
}

std::string Security::Myexepath()
{
	char shitter[_MAX_PATH]; // defining the path
	GetModuleFileNameA(NULL, shitter, _MAX_PATH); // getting the path
	return std::string(shitter); //returning the path
}





std::string AuthDecrypt(const std::string str_in, const std::string key, const std::string iv, size_t* length)
{
	
	std::string str_out;
	CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());
	
	CryptoPP::StringSource decryptor(str_in, true,
		new CryptoPP::Base64Decoder(
			new CryptoPP::StreamTransformationFilter(decryption,
				new CryptoPP::StringSink(str_out)
			)
		)
	);
	

	if (length != nullptr)	*length = str_out.length();
	
	return str_out;
}

std::string AuthEncrypt(const std::string str_in, const std::string key, const std::string iv)
{
	
	std::string str_out;
	CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());
	CryptoPP::StringSource encryptor(str_in, true,
		new CryptoPP::StreamTransformationFilter(encryption,
			new CryptoPP::Base64Encoder(
				new CryptoPP::StringSink(str_out),
				false
			)
		)
	);
	
	return str_out;
}