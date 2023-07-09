#ifndef AUTH_H
#define AUTH_H

#include <drogon/WebSocketClient.h>
#include <drogon/HttpAppFramework.h>
#include <iostream>

#include "../Security/Security.h" 
#include "../GUI/GUI.h"
#include "../Injection/Exploit.h"

using namespace drogon;
using namespace std::chrono_literals;

/**
* This class will allow the loader to connect to our server and authenticate the loader.
*/
class Connection: public Security {
public:
	Connection(std::string server_string, std::string path, uint16_t port);
	void Run();
	void PrintConnectionDetails();
	void SendMsg(std::string msg, drogon::WebSocketMessageType type);
	void CloseConnection();
	
	bool Login(std::string LicenseKey);

	void HandleLoginResults(std::string result);

	class GUI* frame;
	drogon::WebSocketClientPtr wsPtr{ NULL };
	drogon::HttpRequestPtr req{ NULL };
	std::string server{ ("") };
	std::string path{ ("") };
	std::string serverString { ("") };
	uint16_t port { 0 }; // Unsigned 16 bit integer, Range is 0 to +infinity only 2 bytes. 8848 = 10001010010000 (14 bits)
	
	bool LoggedIn{ false };
};

extern Connection Connect;

extern PVOID WebSocketThread(void*);

#endif AUTH_H