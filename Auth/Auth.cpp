#include "../Auth/Auth.h"
#include "../Security/Security.h"


Connection Connect("ws://127.0.0.1", "/chat", 8848);


Connection::Connection(std::string input_server_string, std::string input_path, uint16_t input_port) {
	
	// Set data members to input values.
	this->server = input_server_string;
	this->port = input_port;
	this->path = input_path;
	
	// Create the server string.
	if (this->port == 0) {
		this->serverString = this->server; // ws://127.0.0.1/chat
	}
	else {
		this->serverString = this->server + ":" + std::to_string(this->port); // ws://127.0.0.1/chat:8848
	}
	
	this->wsPtr = WebSocketClient::newWebSocketClient(this->serverString);
	this->req = HttpRequest::newHttpRequest();
	this->req->setPath(this->path);
	
}

void Connection::Run() {
	
	// For debugging outputs.
    app().setLogLevel(trantor::Logger::kDebug);

	// Run the websocket client and begin connection. This should be called in a seperate thread.
    app().run();
    
}

void Connection::PrintConnectionDetails() {
	printf("Full Server String: %s\n", this->serverString.c_str());
}







/**
* This thread sets up important handlers for a websocket connection with our server.
* 
* MessageHandler will handle all messages sent from the server.
* 
* ConnectionClosedHandler will handle when the connection is closed.
* 
* 
*/
PVOID WebSocketThread(void*) {

	// Set the message handler, this is called when a message is received.
	Connect.wsPtr->setMessageHandler([](const std::string& message, const WebSocketClientPtr&, const WebSocketMessageType& type) {

		// Find the correct message type.

		// TODO, experimnt with the WebSocketMessageType add our own type to it.
		
		std::string messageType = "Unknown";
	
		switch (type) {
			case WebSocketMessageType::Text: {
				messageType = "text";
				break;
			}
			case WebSocketMessageType::Pong: {
				messageType = "pong";
				break;
			}
			case WebSocketMessageType::Ping: {
				messageType = "ping";
				break;
			}
			case WebSocketMessageType::Binary: {
				messageType = "binary";
				break;
			}
			case WebSocketMessageType::Close: {
				messageType = "Close";
				break;
			}
		}

		LOG_INFO << "new message (" << messageType << "): " << message;

	});

	// Set the connection closed handler.
	Connect.wsPtr->setConnectionClosedHandler([](const WebSocketClientPtr&) {

		// Once the connection is closed, log it. TODO - Uninstall hooks, etc.

		LOG_INFO << "WebSocket connection closed!";
	});

	

	std::string spki;
	StringSink ss(spki);
	Secure.LoaderPublicKey.Save(ss);

	Connect.req->setBody(spki);

	
	// Connect to the server. On connection we will send our public RSA key to the server, and the server will send us it's public key.
	Connect.wsPtr->connectToServer(Connect.req, [](ReqResult r, const HttpResponsePtr&, const WebSocketClientPtr& wsPtr) {

		// Check if the connection was successful.
		if (r != ReqResult::Ok) {
			LOG_ERROR << "Failed to establish WebSocket connection!";
			wsPtr->stop();
			return;
		}

		LOG_INFO << "WebSocket connected!";

	
		

		
	});

	
	// Run the websocket client after all handlers have been setup.
	Connect.Run();

	return 0;
}
