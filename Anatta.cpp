#include "Auth/Auth.h"
#include "GUI/GUI.h"
#include "Security/Security.h"


#include <winternl.h>








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

	

	S.Run();

}





void Menu(PVOID args) {

	// Cast args to params struct and deference the address. 
	GuiParams te = *(GuiParams*)args;

	GUI GuiObject(te.cmdshow, te.hInst);

	GuiObject.Run();
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	Security Begin;
	Begin.AllocateConsole();
	
	GuiParams params;
	params.cmdshow = ncmdshow; 
	params.hInst = hInst;
	
	Begin.RunThread(Menu, "Menu", &params);

	
	while (true) {
		printf("Heartbeat\n");
		Sleep(1000);
	}

	return 0;
}