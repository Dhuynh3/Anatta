#include "../Auth/Auth.h"




Connection Connect("ws://127.0.0.1", "/chat", 8848);


Connection::Connection(std::string input_server_string, std::string input_path, uint16_t input_port) {
	
	// Set data members to input values.
	this->server = input_server_string;
	this->port = input_port;
	this->path = input_path;
	
	// Create the server string.
	if (this->port == 0) {
		this->serverString = this->server;
	}
	else {
		this->serverString = this->server + ":" + std::to_string(this->port);
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


