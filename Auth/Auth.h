#ifndef AUTH_H
#define AUTH_H

#include <drogon/WebSocketClient.h>
#include <drogon/HttpAppFramework.h>
#include <iostream>

using namespace drogon;
using namespace std::chrono_literals;

class Connection {
public:
	Connection();
	void Run();
	

	
	
	drogon::WebSocketClientPtr wsPtr{ NULL };
	std::string server{ ("")};
	std::string path{ ("")};
	std::string serverString = server + ":" + std::to_string(port.value()) + path;
	optional<uint16_t> port{ NULL }; // Unsigned 16 bit integer, 0 - +infinity only 2 bytes 8848 = 10001010010000 (14 bits)
};

#endif AUTH_H