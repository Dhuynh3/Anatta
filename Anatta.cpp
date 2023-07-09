#include "Auth/Auth.h"
#include "GUI/GUI.h"
#include "Security/Security.h"







/**
* This function is the application's entrypoint.
*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	// Open console for debugging.
	Connect.AllocateConsole();
	
	// Generate an RSA keypair.
	InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(Secure.rng, 3072);
	Connect.LoaderPrivateKey = RSA::PrivateKey(params);
	Connect.LoaderPublicKey = RSA::PublicKey(Connect.LoaderPrivateKey);
	
	// Test functions
	std::string exehash = Secure.CalcHash256(Secure.Myexepath());
	printf("My hash %s\n", exehash.c_str());

	printf("Final HWID %s\n", Secure.GetFullHWID().c_str());

	// Run the websocket thread.
	Secure.RunThread(WebSocketThread, "WebSocket", 0);

	// Run the GUI thread
	GuiParams guip;
	guip.cmdshow = ncmdshow;
	guip.hInst = hInst;
	Secure.RunThread(MainForm, "MainForm", &guip);
	
	// Loop is needed to keep the other threads running.
	while (true) {
		Sleep(1000);
	}
	
	return 0;
}