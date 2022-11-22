#ifndef GUI_H
#define GUI_H

#include <Windows.h>
#include <iostream>
#include <string>

#include "../Security/Security.h"

using namespace std;

#define BUTTON_1 0x01
#define BUTTON_2 0x02

class GUI {
public:
	GUI(int nCmdLn = NULL, HINSTANCE hInst = NULL);

	int Run();
	static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND windowHandle;
	int count;

	HINSTANCE loader_handle { NULL };
	int cmd_show { 0 };
	int ScreenX { 0 };
	int ScreenY { 0 };
	int MenuX{ 500 };
	int MenuY{ 400 };
private:

};

#endif GUI_H