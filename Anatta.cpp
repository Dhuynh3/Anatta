#include "Auth/Auth.h"
#include "GUI/GUI.h"
#include "Security/Security.h"






/**
* This thread sets up important handlers for a websocket connection with our server.
*/
PVOID WebSocketThread(void*) {

	// Set the message handler, this is called when a message is received.
	Connect.wsPtr->setMessageHandler([](const std::string& message, const WebSocketClientPtr&, const WebSocketMessageType& type) {

		// Find the correct message type.
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

	// Connect to the server.
	Connect.wsPtr->connectToServer(Connect.req, [](ReqResult r, const HttpResponsePtr&, const WebSocketClientPtr& wsPtr) {

		// Check if the connection was successful.
		if (r != ReqResult::Ok) {
			LOG_ERROR << "Failed to establish WebSocket connection!";
			wsPtr->stop();
			return;
		}

	LOG_INFO << "WebSocket connected!";

	printf("WsPtr2 %p\n", wsPtr.get());

	wsPtr->getConnection()->setPingMessage("", 2s);
	wsPtr->getConnection()->send("hello!");

	});


	// Run the websocket client after all handlers have been setup.
	Connect.Run();

	return 0;
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	Security Begin;
	Begin.AllocateConsole();

	

	Begin.RunThread(WebSocketThread, "WebSocket", 0);

	
	while (true) {
		printf("Heartbeat\n");
		
		printf("Connection %p\n", Connect.wsPtr.get()->getConnection().get());

		if (Connect.wsPtr.get()->getConnection().get() != nullptr) {
			Connect.wsPtr.get()->getConnection().get()->send("yeeeeee!");
		}
	
		
		Sleep(1000);
	}

	return 0;
}