#include "Auth/Auth.h"
#include "GUI/GUI.h"
#include "Security/Security.h"








int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	
	Secure.AllocateConsole();


	InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(Secure.rng, 3072);
	
	Secure.LoaderPrivateKey = RSA::PrivateKey(params);
	Secure.LoaderPublicKey = RSA::PublicKey(Secure.LoaderPrivateKey);
	

	Secure.RunThread(WebSocketThread, "WebSocket", 0);

	
	while (true) {
		printf("Heartbeat\n");
		
		printf("Connection %p\n", Connect.wsPtr.get()->getConnection().get());

		if (Connect.wsPtr.get()->getConnection().get() != nullptr) {
			//Connect.wsPtr.get()->getConnection().get()->send("yeeeeee!");
		}
	
		
		Sleep(1000);
	}

	return 0;
}