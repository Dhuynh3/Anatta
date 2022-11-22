#include "Security.h"

Security::Security() {

	
}


std::wstring Security::random_wstring(size_t length)
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