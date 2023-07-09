#ifndef HWID_H
#define HWID_H

#include <Windows.h>
#include <iostream>
#include <intrin.h>
#include <initguid.h>
#include <wmistr.h>
#include "../Anatta.h"


class HWID {

public:

	bool GetCpuName(std::string& strCpu);
	bool GetDesktopName();
	bool GetTotalRam(PULONGLONG ram);
	bool GetPhysicalDriveSerialNumber(std::string& strSerialNumber);
	std::string GetFullHWID();

	const wchar_t* deviceName = (L"\\??\\PhysicalDrive0");
	const char* ErrorName = ("Error");

	int regs[4] = { 0,0,0,0 };
	char desktop_name[16]; // 16 bytes for the desktop name.

	std::string cpu_brand_name;
	std::string disk_serial;
	std::string monitor_name;
	std::string final_hwid;

	ULONGLONG total_physicalmemory;
};

#endif HWID_H