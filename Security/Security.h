#ifndef SECURITY_H
#define SECURITY_H

#include <Windows.h>
#include <algorithm>
#include <string>
#include <time.h>

class Security {
public:
	Security();
	std::wstring random_wstring(size_t length);

	
};

#endif SECURITY_H