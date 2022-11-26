#include "../Auth/Auth.h"



Connection::Connection(std::string input_server_string, std::string input_path, uint16_t input_port) {
	
	this->server = input_server_string;
	this->port = input_port;
	this->path = input_path;

}

bool Connection::SetupWebSocket() {

	this->serverString = "niggers";

	//this->wsPtr = WebSocketClient::newWebSocketClient(this->server + ":" + std::to_string(port));
	return true;
}

void Connection::Run() {
    app().setLogLevel(trantor::Logger::kDebug);
    app().run();
}