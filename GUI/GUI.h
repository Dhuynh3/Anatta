#ifndef GUI_H
#define GUI_H

#include <Windows.h>
#include <iostream>
#include <string>
#include <CommCtrl.h>
#include <WinUser.h>

#include "../resource.h"
#include "../Security/Security.h"

using namespace std;

#define BUTTON_1 0x01
#define BUTTON_2 0x02
#define IDC_EDIT 0x03

class GUI {
public:
	GUI(int nCmdLn = NULL, HINSTANCE hInst = NULL);

	int Run();
	static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	
	int count;

	HANDLE logoImage{ NULL };
	HWND logoWindow{ NULL };
	HWND windowHandle{ NULL };
	HWND edit_handle{ NULL };
	HWND button_handle{ NULL };
	HINSTANCE loader_handle { NULL };
	int cmd_show { NULL };
	int ScreenX { NULL };
	int ScreenY { NULL };
	int MenuX{ 300 };
	int MenuY{ 350 };
private:

};

#endif GUI_H