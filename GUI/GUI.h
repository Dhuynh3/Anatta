#ifndef GUI_H
#define GUI_H

#include "../Auth/Auth.h"

#include <Windows.h>
#include <iostream>
#include <string>
#include <CommCtrl.h>
#include <WinUser.h>

#include "../resource.h"
#include "../Security/Security.h"


using namespace std;

#define IDC_CLOSE 0x01
#define BUTTON_2 0x02
#define IDC_EDIT 0x03
#define IDC_LOGIN 0x05
#define IDC_TEXT 0x06
#define IDC_INJECT 0x07

#define BACKGROUND_COLOR RGB(34, 34, 34)

class GUI {
public:
	GUI(int nCmdLn = NULL, HINSTANCE hInst = NULL);

	int Run();
	static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	HWND ZwCreateWindow(LPCSTR name, int PosX, int PosY, int width, int height, DWORD flags, HMENU child_id, LPCSTR text, std::string UniqueName);
	bool ExitWindow(std::string UniqueName);
	HWND GetWindow(std::string UniqueName);


	bool DisplayLoginResults(Json::Value results);
	

	HANDLE logoImage{ NULL };
	HWND logoWindow{ NULL };
	HWND windowHandle{ NULL };
	//HWND edit_handle{ NULL };
	HINSTANCE loader_handle { NULL };
	

	std::map<std::string, std::pair<HWND, HMENU>> WindowHandleList; // O(Log N), we will use this Key-Value pair container to store all the HWNDs and associated HMENU for the buttons with a unique name.
	

	int cmd_show { NULL };
	int ScreenX { NULL };
	int ScreenY { NULL };
	int MenuX{ 300 };
	int MenuY{ 350 };
private:

};


extern PVOID MainForm(PVOID args);

extern PVOID ResultsForm(PVOID args);

#endif GUI_H