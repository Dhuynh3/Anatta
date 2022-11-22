#include "GUI/GUI.h"
#include <thread>

void menu1(int ncmdshow, HINSTANCE hInst) {
	GUI s(ncmdshow, hInst);
	s.Run();
}



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	std::thread first(menu1, ncmdshow, hInst);



	while (true) {
		Sleep(1000);
	}

	return 0;
}