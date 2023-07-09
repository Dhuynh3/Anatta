#include "../Hwid/Hwid.h"

/**
* This function gets the user's CPU brand name.
* @params CpuName - The string to store the CPU brand name in.
*/
bool HWID::GetCpuName(std::string& CpuName) {
	for (int i = 0x80000002; i < 0x80000005; ++i) {
		__cpuid(regs, i);
		CpuName += std::string((const char*)&regs[0], 4);
		CpuName += std::string((const char*)&regs[1], 4);
		CpuName += std::string((const char*)&regs[2], 4);
		CpuName += std::string((const char*)&regs[3], 4);
	}
	return true;
}

/**
* This function gets the user's Desktop Name.
*/
bool HWID::GetDesktopName() {

	DWORD Size = sizeof(desktop_name);
	
	// If this function fails, it will return garbage values.
	if (!GetUserNameA(desktop_name, &Size)) {
		void* Trash = (char*)malloc(Size);
		memcpy(desktop_name, Trash, Size);
		free(Trash);
		return false;
	}
	
	return true;
}

bool HWID::GetTotalRam(PULONGLONG ram) {
	if (!GetPhysicallyInstalledSystemMemory(ram)) {
		ram = 0x0;
		return false;
	}
	return true;
}

bool HWID::GetPhysicalDriveSerialNumber(std::string& strSerialNumber)
{

	UNICODE_STRING HI;
	RtlInitUnicodeString(&HI, deviceName);
	IO_STATUS_BLOCK IoSb;
	OBJECT_ATTRIBUTES Oa = { 0 };
	HANDLE Handle;
	strSerialNumber = ErrorName;

	Oa.Length = sizeof Oa;
	Oa.ObjectName = &HI;
	Oa.Attributes = OBJ_CASE_INSENSITIVE;
	Oa.RootDirectory = 0;

	NTSTATUS A = NtOpenFile(&Handle, FILE_READ_DATA, &Oa, &IoSb, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_SEQUENTIAL_ONLY);

	if (!NT_SUCCESS(A)) {
		NtClose(Handle);
		return false;
	}

	// Set the input data structure
	STORAGE_PROPERTY_QUERY storagePropertyQuery;
	ZeroMemory(&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY));
	storagePropertyQuery.PropertyId = StorageDeviceProperty;
	storagePropertyQuery.QueryType = PropertyStandardQuery;

	// Get the necessary output buffer size
	STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader = { 0 };
	DWORD dwBytesReturned = 0;

	NTSTATUS B = NtDeviceIoControlFile(Handle, NULL, NULL, NULL, &IoSb, IOCTL_STORAGE_QUERY_PROPERTY,
		&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY), &storageDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER));

	if (!NT_SUCCESS(B)) {
		NtClose(Handle);
		return false;
	}

	// Alloc the output buffer
	const DWORD dwOutBufferSize = storageDescriptorHeader.Size;
	BYTE* pOutBuffer = new BYTE[dwOutBufferSize];
	ZeroMemory(pOutBuffer, dwOutBufferSize);

	NTSTATUS C = NtDeviceIoControlFile(Handle, NULL, NULL, NULL, &IoSb, IOCTL_STORAGE_QUERY_PROPERTY,
		&storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY), pOutBuffer, dwOutBufferSize);

	if (!NT_SUCCESS(C)) {
		NtClose(Handle);
		return false;
	}

	STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pOutBuffer;
	const DWORD dwSerialNumberOffset = pDeviceDescriptor->Version;
	if (dwSerialNumberOffset != 0)
	{
		strSerialNumber = std::string(reinterpret_cast<char*>(pOutBuffer + dwSerialNumberOffset));

	}

	delete[]pOutBuffer;
	NtClose(Handle);
	return true;
}




/**
* This function grabs the complete HWID of the user
* TODO - Optimize function and remove strcat.
*/
std::string HWID::GetFullHWID() {
	
	// Zero out the memory or else we will get garbage.
	char FinalHWID[MAX_PATH];
	SecureZeroMemory(FinalHWID, sizeof(FinalHWID));

	GetCpuName(this->cpu_brand_name);
	GetDesktopName();
	GetTotalRam(&this->total_physicalmemory);
	GetPhysicalDriveSerialNumber(this->disk_serial);
	
	strcat(FinalHWID, this->cpu_brand_name.c_str());
	strcat(FinalHWID, desktop_name);
	strcat(FinalHWID, std::to_string(this->total_physicalmemory).c_str());
	strcat(FinalHWID, this->disk_serial.c_str());

	return FinalHWID;
}