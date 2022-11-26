#include "GUI.h"




GUI::GUI(int nCmdSh, HINSTANCE hInst) {
	
	Security S;
	
	cmd_show = nCmdSh; // 1 = Show window 0 = hide window
	loader_handle = hInst; 
	ScreenX = GetSystemMetrics(SM_CXSCREEN);
	ScreenY = GetSystemMetrics(SM_CYSCREEN);

	WNDCLASSEX wc { 0 };
	SecureZeroMemory(&wc, sizeof(wc));
	
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = HBRUSH(COLOR_GRAYTEXT + 1);
	wc.lpfnWndProc = MessageRouter;
	wc.lpszClassName = S.RandomWString(10).c_str();
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Failed to register window class", L"Error", MB_OK | MB_ICONERROR);
	}

	// We put a pointer to the current class instance as our last creation parameter. We'll recover it in the Message Routing function
	windowHandle = CreateWindowEx(NULL, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, ((ScreenX / 2) - (MenuX/2)), ((ScreenY / 2) - (MenuY / 2)), MenuX, MenuY, NULL, NULL, loader_handle, this);
	SetWindowLong(windowHandle, GWL_STYLE, 0);

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
	case WM_CREATE:
		CreateWindowA(("button"), ("Count"), WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE, 10, 10, 85, 18, hWnd, (HMENU)BUTTON_1, GetModuleHandle(NULL), NULL);
		CreateWindowA(("button"), ("Show count"), WS_CHILD | BS_DEFPUSHBUTTON | WS_VISIBLE, 100, 10, 85, 18, hWnd, (HMENU)BUTTON_2, GetModuleHandle(NULL), NULL);
		this->edit_handle = CreateWindowA(("EDIT"), (""), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_UPPERCASE | ES_WANTRETURN, 100, 50, 85, 18, hWnd, (HMENU)IDC_EDIT, this->loader_handle, NULL); 

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
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	
	return 0;
}
