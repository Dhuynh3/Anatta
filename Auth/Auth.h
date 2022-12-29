#ifndef AUTH_H
#define AUTH_H

#include <drogon/WebSocketClient.h>
#include <drogon/HttpAppFramework.h>
#include <iostream>

using namespace drogon;
using namespace std::chrono_literals;







/**
* This class will allow the loader to connect to our server and authenticate the loader.
*/
class Connection {
public:
	Connection(std::string server_string, std::string path, uint16_t port);
	void Run();
	void PrintConnectionDetails();

	
	
	
	
	drogon::WebSocketClientPtr wsPtr{ NULL };
	drogon::HttpRequestPtr req{ NULL };
	std::string server{ ("") };
	std::string path{ ("") };
	std::string serverString { ("") };
	uint16_t port { 0 }; // Unsigned 16 bit integer, Range is 0 to +infinity only 2 bytes. 8848 = 10001010010000 (14 bits)

};

extern Connection Connect;

#endif AUTH_H