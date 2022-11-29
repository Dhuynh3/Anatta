#include "GUI.h"




GUI::GUI(int nCmdSh, HINSTANCE hInst) {
	
	Security S;
	
	cmd_show = nCmdSh; // 1 = Show window 0 = hide window
	loader_handle = hInst; 
	ScreenX = GetSystemMetrics(SM_CXSCREEN);
	ScreenY = GetSystemMetrics(SM_CYSCREEN);

	WNDCLASSEXW wc { 0 };
	SecureZeroMemory(&wc, sizeof(wc));
	
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = CreateSolidBrush(RGB(34, 34, 34));
	wc.lpfnWndProc = MessageRouter;
	wc.lpszClassName = L"Hello";
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc)) {
		printf("Failed to register class Error %i\n", GetLastError());
	}

	// We put a pointer to the current class instance as our last creation parameter. We'll recover it in the Message Routing function
	windowHandle = CreateWindowEx(NULL, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, ((ScreenX / 2) - (MenuX/2)), ((ScreenY / 2) - (MenuY / 2)), MenuX, MenuY, NULL, NULL, loader_handle, this);
	SetWindowLong(windowHandle, GWL_STYLE, 0);
	SetWindowLong(windowHandle, GWL_STYLE, WS_BORDER);
	SetWindowLong(windowHandle, GWL_EXSTYLE, WS_EX_TOPMOST);

	if (windowHandle != INVALID_HANDLE_VALUE)
	{
		ShowWindow(windowHandle, cmd_show);
		UpdateWindow(windowHandle);
	}

}

int GUI::Run()
{
	MSG msg{ 0 };
	
	if (windowHandle != INVALID_HANDLE_VALUE)
	{
		while (GetMessage(&msg, 0, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
	
}

LRESULT CALLBACK GUI::MessageRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GUI* app;
	if (msg == WM_CREATE)
	{
		// When the window is created, lParam contains a pointer to the current class instance. We cast it and asign it to a pointer to the class
		// That way we can access its members from a static context (as MessageRouter's)
		app = (GUI*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		// We store the reteived pointer to the window extra data space for it to be available later
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
	}
	else
	{
		//If the window has been created we recover the class pointer from the window extra data space
		app = (GUI*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	//We call the actual WndProc. This one is a member of the class and has access to its members
	return app->WndProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK GUI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE: {

	
		this->logoWindow = CreateWindowA(("Static"), "", WS_CHILD | SS_BITMAP | WS_VISIBLE, 0, 0, NULL, NULL, hWnd, NULL, GetModuleHandle(NULL), NULL);

		SendMessage(this->logoWindow, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(this->loader_handle, MAKEINTRESOURCE(IDB_BITMAP1)));



		this->button_handle = CreateWindowA(("button"), (""), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | ES_CENTER, 250, 5, 45, 25, hWnd, (HMENU)BUTTON_1, this->loader_handle, NULL);
		

		CreateWindowA(("button"), ("Show count"), WS_CHILD | WS_VISIBLE, 100, 10, 85, 18, hWnd, (HMENU)BUTTON_2, GetModuleHandle(NULL), NULL);

		this->edit_handle = CreateWindowA(("EDIT"), (""), WS_CHILD | WS_VISIBLE | ES_CENTER | ES_UPPERCASE, 25, 250, 250, 50, hWnd, (HMENU)IDC_EDIT, this->loader_handle, NULL);

		LOGFONT logfont;
		ZeroMemory(&logfont, sizeof(LOGFONT));
		logfont.lfCharSet = ANSI_CHARSET;
		logfont.lfHeight = -18;
		logfont.lfWeight = FW_DEMIBOLD;
		logfont.lfQuality = CLEARTYPE_QUALITY;
		logfont.lfPitchAndFamily = FF_DECORATIVE;

		HFONT hFont = CreateFontIndirect(&logfont);

		SendMessage(this->edit_handle, EM_SETLIMITTEXT, 10, NULL);
		SendMessage(this->edit_handle, WM_SETFONT, (WPARAM)hFont, TRUE);

		WCHAR placeholderText[] = L"Enter here";
		SendMessage(this->edit_handle, EM_SETCUEBANNER, FALSE, (LPARAM)placeholderText);

		break;
	}
	case WM_COMMAND:
		switch (wParam)
		{
		case BUTTON_1:
			this->count++;
			MessageBeep(MB_ICONINFORMATION);
			char text[100];
			GetWindowTextA(this->edit_handle, text, sizeof(text));


			printf("Text :%s\n", text);
			break;
		case BUTTON_2:
			MessageBox(hWnd, (wstring(L"You've pressed the button ") + to_wstring(this->count) + wstring(L" times")).c_str(), TEXT("Information"), 0);
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		CloseWindow(hWnd);
		PostQuitMessage(0);
		break;
	case WM_DRAWITEM: {

		LPDRAWITEMSTRUCT lpds = (DRAWITEMSTRUCT*)lParam;

		switch (lpds->CtlType) {

			case ODT_BUTTON: {
				
				if (lpds->hwndItem == this->button_handle) {
					printf("show count\n");
				}

				printf("nigger button\n");
				SIZE size;
				char text[100];
				GetTextExtentPoint32(lpds->hDC, L"X", strlen("X"), &size);
				SetTextColor(lpds->hDC, RGB(255, 255, 255));
				SetBkColor(lpds->hDC, RGB(34, 34, 34));
				ExtTextOutA(lpds->hDC, lpds->rcItem.left + 18, lpds->rcItem.top + 4, ETO_OPAQUE | ETO_CLIPPED, &lpds->rcItem, "X", strlen("X"), NULL);
				DrawEdge(lpds->hDC, &lpds->rcItem, (lpds->itemState & ODS_SELECTED) ? EDGE_SUNKEN : EDGE_RAISED, BF_MONO); //BF_MONO //BF_RECT

				break;
			}

		}
		
		break;
	}
	case WM_CTLCOLOREDIT: {
		HDC hdc = (HDC)wParam;
		HWND hwnd = (HWND)lParam;
		
		if (this->edit_handle == hwnd) { 
			printf("nigga handle %i\n", GetDlgCtrlID(hwnd));
			//SetTextAlign(hdc, TA_LEFT);
			SetBkColor(hdc, RGB(255, 34, 34));
			SetTextColor(hdc, RGB(200, 200, 200));
			SetDCBrushColor(hdc, RGB(34, 34, 34));
			return (LRESULT)GetStockObject(DC_BRUSH); // return a DC brush.
		}
		break;
	}
	case WM_KEYDOWN: { // Monitor keys

		break;
	}
	case WM_CHAR: {
		break;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	
	return 0;
}

