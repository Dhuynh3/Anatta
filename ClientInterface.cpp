#include "../ClientInterface/ClientInterface.h"


ClientInterface ClientConnection("wss://gateway.dathuynh.io", "/heartbeat", 0);

/*
* ClientInterface Constructor will initialize the connection to the server and provide the interface needed for the client to communicate with the server.
* 
*/
ClientInterface::ClientInterface(std::string server_string, std::string uri, uint16_t port) {
	
	// Set data members to input values.
	this->server = server_string;
	this->port = port;
	this->uri = uri;

	// Create the server string.
	if (this->port == 0) {
		this->full_server_string = this->server; // ws://127.0.0.1/chat
	}
	else {
		this->full_server_string = this->server + ":" + std::to_string(this->port); // ws://127.0.0.1/chat:8848
	}

	this->drogon_ws = WebSocketClient::newWebSocketClient(this->full_server_string);
	this->drogon_req = HttpRequest::newHttpRequest();
	this->drogon_req->setPath(this->uri);
}


/*
* The ClientWebSocketThread sets up important handlers for a websocket connection with our server.
*/
PVOID ClientWebSocketThread(void*) {

	// Set the message handler, this is called when a message is received.
	ClientConnection.drogon_ws->setMessageHandler([](const std::string& message, const WebSocketClientPtr&, const WebSocketMessageType& type) {

		// Find the correct message type.
		std::string messageType = "Unknown";

		switch (type) {
			case WebSocketMessageType::Text: {
				messageType = "text";
				break;
			}
			case WebSocketMessageType::Pong: {
				messageType = "Pong";
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
			case WebSocketMessageType::Unknown: {
				break;
			}
		}

		LOG_INFO << "new message (" << messageType << "): " << message;

	});

	// Set the connection closed handler.
	ClientConnection.drogon_ws->setConnectionClosedHandler([](const WebSocketClientPtr&) {

		// Once the connection is closed, log it. TODO - Uninstall hooks, etc.
		// Connect.Death();

		LOG_INFO << "WebSocket connection closed!";
	});

	// Connect to the server. On connection we will send our public RSA key to the server, and the server will send us it's public key.
	ClientConnection.drogon_ws->connectToServer(ClientConnection.drogon_req, [](ReqResult r, const HttpResponsePtr&, const WebSocketClientPtr& wsPtr) {

		// Check if the connection was successful.
		if (r != ReqResult::Ok) {
			LOG_ERROR << "Failed to establish WebSocket connection!";
			wsPtr->stop();
			return;
		}
	
		if (r == ReqResult::Ok) {
			ClientConnection.drogon_ws.get()->getConnection().get()->send("Hello mans", WebSocketMessageType::Text);
		}

		LOG_INFO << "WebSocket connected!";
	});


	// Run the websocket client after all handlers have been setup.
	// For debugging outputs.
	app().setLogLevel(trantor::Logger::kDebug);

	// Run the websocket client and begin connection. This should be called in a seperate thread.
	app().run();

	return 0;
}