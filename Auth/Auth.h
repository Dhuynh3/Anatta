#ifndef AUTH_H
#define AUTH_H

#include <drogon/WebSocketClient.h>
#include <drogon/HttpAppFramework.h>
#include <iostream>

using namespace drogon;
using namespace std::chrono_literals;

class Connection {
public:
	Connection(std::string input_server_string, std::string input_path, uint16_t input_port);
	bool SetupWebSocket();
	void Run();
	

	
	
	drogon::WebSocketClientPtr wsPtr{ NULL };
	std::string server{ ("") };
	std::string path{ ("") };
	std::string serverString { ("") };
	uint16_t port { 0 }; // Unsigned 16 bit integer, 0 - +infinity only 2 bytes 8848 = 10001010010000 (14 bits)
};

#endif AUTH_H