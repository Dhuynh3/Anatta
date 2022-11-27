#include "Auth/Auth.h"
#include "GUI/GUI.h"
#include "Security/Security.h"


#include <winternl.h>


void menu1(PVOID args) {

	struct Params {
		int cmdshow;
		HINSTANCE hInst;
	};


	// Cast args to params struct and deference the address. 
	Params te = *(Params*)args;
	printf("Test addr :%p", args);

	GUI s(te.cmdshow, te.hInst);

	printf("Hnst %p\n", s.loader_handle);

	s.Run();
}






void DebuggerThread(PVOID args) {
	
	Security* s = (Security*)args;
	while (true) {
		if (s->DebuggerCheck(args)) {
			printf("Debugger detected!\n");
	
		}
		Sleep(1000);
	}

}


Connection S("ws://127.0.0.1", "/chat", 8848);

void WebSocket() {



	S.SetupWebSocket();
	printf("Server :%s\n", S.serverString.c_str());

	S.wsPtr->setMessageHandler([](const std::string& message,
		const WebSocketClientPtr&,
		const WebSocketMessageType& type) {
			std::string messageType = "Unknown";
	if (type == WebSocketMessageType::Text)
		messageType = "text";
	else if (type == WebSocketMessageType::Pong)
		messageType = "pong";
	else if (type == WebSocketMessageType::Ping)
		messageType = "ping";
	else if (type == WebSocketMessageType::Binary)
		messageType = "binary";
	else if (type == WebSocketMessageType::Close)
		messageType = "Close";

	LOG_INFO << "new message (" << messageType << "): " << message;
		});

	S.wsPtr->setConnectionClosedHandler([](const WebSocketClientPtr&) {
		LOG_INFO << "WebSocket connection closed!";
		});

	LOG_INFO << "Connecting to WebSocket at " << S.server;

	S.wsPtr->connectToServer(S.req, [](ReqResult r, const HttpResponsePtr&, const WebSocketClientPtr& wsPtr) {

		if (r != ReqResult::Ok) {
			LOG_ERROR << "Failed to establish WebSocket connection!";
			wsPtr->stop();
			return;
		}

	LOG_INFO << "WebSocket connected!";
	wsPtr->getConnection()->setPingMessage("", 2s);
	wsPtr->getConnection()->send("hello!");
		});

	S.Run();

}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	Security Begin;
	Begin.AllocateConsole();
	Begin.RunThread(WebSocket, "WebSocket", nullptr);
	
	while (true) {
		printf("Heartbeat\n");
		Sleep(1000);
	}

	return 0;
}