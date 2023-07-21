// Header Guards
#ifndef CLIENT_INTERFACE_H
#define CLIENT_INTERFACE_H

// Includes
#include <drogon/WebSocketClient.h>
#include <drogon/HttpAppFramework.h>
#include <iostream>

// Namespaces
using namespace drogon;
using namespace std::chrono_literals;

/*
* ClientInterface class represents the interface between the client and the server. 
* It uses Drogon framework for HTTP/HTTPS request and for websocket connection.
*/
class ClientInterface {
public: 
	ClientInterface(std::string server_string, std::string path, uint16_t port);

	drogon::WebSocketClientPtr drogon_ws{ NULL };
	drogon::HttpRequestPtr drogon_req{ NULL };
	std::string server{ ("") };
	std::string uri{ ("") };
	std::string full_server_string{ ("") }; // server + port + uri
	uint16_t port{ 0 }; // Unsigned 16 bit integer, Range is 0 to +infinity only 2 bytes (16 bits). 8848 = 10001010010000 (14 bits)
};

extern ClientInterface ClientConnection;

extern PVOID ClientWebSocketThread(void*);

// Header Guard End
#endif CLIENT_INTERFACE_H