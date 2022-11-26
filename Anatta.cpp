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

void menu2() {

	Connection S("ws://127.0.0.1", "/chat", 8848);
	S.SetupWebSocket();
	printf("Server :%s\n", S.serverString.c_str());
    
	
}

void threadtest() {
	while (true) {
		Sleep(1000);
		printf("igged\n");
	}
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	Security Begin;
	Begin.AllocateConsole();
	
	while (true) {
		printf("Heartbeat\n");
		Sleep(1000);
	}

	return 0;
}


















/*
if (AllocConsole()) {
		freopen("CONOUT$", "w", stdout);

		Security S;

		struct Params {
			int cmdshow;
			HINSTANCE hInst;
		};
		Params test;
		test.cmdshow = ncmdshow;
		test.hInst = hInst;
		printf("Test addr :%p", &test);

		S.RunThread(menu1, "menu1", &test);
		S.RunThread(threadtest, "nig", nullptr);
		Sleep(5000);

		S.CloseThread("nig");

		


	}



	
*/