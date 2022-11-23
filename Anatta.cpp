#include "Auth/Auth.h"
#include "GUI/GUI.h"
#include <thread>

void menu1(int ncmdshow, HINSTANCE hInst) {
	GUI s(ncmdshow, hInst);
	s.Run();
}

void menu2() {

    

}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	std::thread first(menu1, ncmdshow, hInst);

//	std::thread second(menu2);


	while (true) {
		Sleep(1000);
	}

	return 0;
}