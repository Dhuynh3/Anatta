#include "../Auth/Auth.h"



Connection::Connection(std::string input_server_string, std::string input_path, uint16_t input_port) {
	
	this->server = input_server_string;
	this->port = input_port;
	this->path = input_path;

}

bool Connection::SetupWebSocket() {

	if (this->port == 0) {
		this->serverString = this->server;
	}
	else {
		this->serverString = this->server + ":" + std::to_string(this->port);
	}
	
	this->wsPtr = WebSocketClient::newWebSocketClient(this->serverString);
	this->req = HttpRequest::newHttpRequest();
	this->req->setPath(this->path);
	
	return true;
}

void Connection::Run() {
    app().setLogLevel(trantor::Logger::kDebug);
    app().run();
}