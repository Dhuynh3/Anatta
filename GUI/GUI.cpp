#include "GUI.h"




/**
* GUI constructor, we want to initialize a main form.
*/
GUI::GUI(int nCmdSh, HINSTANCE hInst) {
	
	// Store some values
	this->cmd_show = nCmdSh; // 1 = Show window 0 = hide window
	this->loader_handle = hInst; // Handle to the loader
	
	// Grab screen size X, Y.
	ScreenX = GetSystemMetrics(SM_CXSCREEN);
	ScreenY = GetSystemMetrics(SM_CYSCREEN);

	WNDCLASSEXW wc { 0 };
	SecureZeroMemory(&wc, sizeof(wc));
	
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.hbrBackground = CreateSolidBrush(BACKGROUND_COLOR);
	wc.lpfnWndProc = MessageRouter;
	wc.lpszClassName = L"CiceroUIWndFrame"; // I don't remember why I named it this. Who did this.
	wc.style = CS_VREDRAW | CS_HREDRAW;

	// Failed to register class, exit.
	if (!RegisterClassEx(&wc)) {
		printf("Failed to register class Error %i\n", GetLastError());
	}

	// We put a pointer to the current class instance as our last creation parameter. We'll recover it in the Message Routing function
	windowHandle = CreateWindowEx(NULL, wc.lpszClassName, wc.lpszClassName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, ((ScreenX / 2) - (MenuX/2)), ((ScreenY / 2) - (MenuY / 2)), MenuX, MenuY, NULL, NULL, loader_handle, this);
	
	SetWindowLong(windowHandle, GWL_STYLE, 0); // Clear all window's flag
	SetWindowLong(windowHandle, GWL_STYLE, WS_BORDER); // The window has a thin-line border
	SetWindowLong(windowHandle, GWL_EXSTYLE, WS_EX_TOPMOST); // Make this window top most

	// If we successfully made a window, show it.
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



		/*
		this->edit_handle = CreateWindowA(("EDIT"), (""), WS_CHILD | WS_VISIBLE | ES_CENTER | ES_UPPERCASE, 25, 250, 240, 25, hWnd, (HMENU)IDC_EDIT, this->loader_handle, NULL);

		LOGFONT logfont;
		ZeroMemory(&logfont, sizeof(LOGFONT));
		logfont.lfCharSet = ANSI_CHARSET;
		logfont.lfHeight = -18;
		logfont.lfWeight = FW_DEMIBOLD;
		logfont.lfQuality = CLEARTYPE_QUALITY;
		logfont.lfPitchAndFamily = FF_DECORATIVE;

		HFONT hFont = CreateFontIndirect(&logfont);

		SendMessage(this->edit_handle, EM_SETLIMITTEXT, 20, NULL);
		SendMessage(this->edit_handle, WM_SETFONT, (WPARAM)hFont, TRUE);

		WCHAR placeholderText[] = L"Enter License";
		SendMessage(this->edit_handle, EM_SETCUEBANNER, FALSE, (LPARAM)placeholderText);
		*/
		
		break;
	}
	case WM_COMMAND:
		switch (wParam)
		{
			// User pressed the close button.
			case IDC_CLOSE: {
				
				Connect.CloseConnection();
				SendMessage(hWnd, WM_CLOSE, NULL, NULL);
				Connect.Death();

				break;
			}
					
			// User pressed the login button.
			case IDC_LOGIN: { 
				
				HWND LicenseKeyBox = this->GetWindow("LicenseKeyBox");

				// Get the text from the edit box.
				char License[20];
				GetWindowTextA(LicenseKeyBox, License, sizeof(License));
				Connect.Login(License);
				
				break;
			}

			// User pressed the inject button.
			case IDC_INJECT: {
				
				if (Connect.LoggedIn == true) {
					Connect.RunThread(ExploitThread, "ExploitThread", 0);
				}
				
				break;
			}
			
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		CloseWindow(hWnd);
		PostQuitMessage(0);
		break;
	case WM_DRAWITEM: { // Process any HWND with BS_OWNERDRAW style.

		LPDRAWITEMSTRUCT lpds = (DRAWITEMSTRUCT*)lParam;

		switch (lpds->CtlType) {

			case ODT_STATIC: {
			
				HWND TextOutput = this->GetWindow("Text");
				
				HWND LoginOutput = this->GetWindow("LoginOutput");

				char staticText[100]; SIZE size;
				GetWindowTextA(lpds->hwndItem, staticText, sizeof(staticText));

				printf("Static text %s\n", staticText);
				
				if (lpds->hwndItem == TextOutput) {
					
				
					HDC hdc = lpds->hDC;
					RECT rc = lpds->rcItem;
					//HBRUSH hbr = CreateSolidBrush(RGB(34, 34, 34));
					//FillRect(hdc, &rc, hbr);
					//DeleteObject(hbr);

					SetBkColor(lpds->hDC, RGB(34, 34, 34));
					SetTextColor(hdc, RGB(255, 255, 255));

					HFONT hFont = CreateFont(18, 0, 0, 0, FW_DEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
					SelectObject(hdc, hFont);

					DrawTextA(hdc, staticText, -1, &rc, DT_LEFT | DT_WORDBREAK);
		
					DeleteObject(hFont);
					
				}

				// Printing the results to the user
				if (lpds->hwndItem == LoginOutput) {

					HDC hdc = lpds->hDC;
					RECT rc = lpds->rcItem;
				
					SetBkColor(lpds->hDC, RGB(34, 34, 34));
					SetTextColor(hdc, RGB(255, 255, 255));

					HFONT hFont = CreateFont(13, 0, 0, 0, FW_DEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");
					SelectObject(hdc, hFont);

					DrawTextA(hdc, staticText, -1, &rc, DT_LEFT | DT_WORDBREAK);

					DeleteObject(hFont);
				}
				
				break;
			}

			case ODT_BUTTON: {
				
				HWND LoginBtn = this->GetWindow("Login");
				HWND CloseBtn = this->GetWindow("Close");
				HWND InjectBtn = this->GetWindow("Inject");
				
				// For every button we want to get the text.
				char staticText[100]; SIZE size;
				GetWindowTextA(lpds->hwndItem, staticText, sizeof(staticText));

				// Custom drawing the Inject button
				if (lpds->hwndItem == InjectBtn) {
					
					GetTextExtentPoint32A(lpds->hDC, staticText, strlen(staticText), &size);
					SetBkColor(lpds->hDC, RGB(34, 34, 34));
					SetTextColor(lpds->hDC, RGB(0, 255, 100));
					ExtTextOutA(lpds->hDC, lpds->rcItem.left + 20, lpds->rcItem.top, ETO_OPAQUE | ETO_CLIPPED, &lpds->rcItem, staticText, strlen(staticText), NULL);
					DrawEdge(lpds->hDC, &lpds->rcItem, (lpds->itemState& ODS_SELECTED) ? EDGE_SUNKEN : EDGE_RAISED, BF_MONO); //BF_MONO //BF_RECT

				}

				// Custom drawing the login button
				if (lpds->hwndItem == LoginBtn) {
					
					GetTextExtentPoint32A(lpds->hDC, staticText, strlen(staticText), &size);

					SetBkColor(lpds->hDC, RGB(34, 34, 34));
					SetTextColor(lpds->hDC, RGB(255, 255, 255));
					
					// + 20 on the left to center the text
					ExtTextOutA(lpds->hDC, lpds->rcItem.left + 20, lpds->rcItem.top, ETO_OPAQUE | ETO_CLIPPED, &lpds->rcItem, staticText, strlen(staticText), NULL);
					DrawEdge(lpds->hDC, &lpds->rcItem, (lpds->itemState & ODS_SELECTED) ? EDGE_SUNKEN : EDGE_RAISED, BF_MONO); //BF_MONO //BF_RECT
					
				}

				// Custom drawing the close button.
				if (lpds->hwndItem == CloseBtn) {

					
					SetTextColor(lpds->hDC, RGB(255, 255, 255));
					SetBkColor(lpds->hDC, RGB(34, 34, 34));
					ExtTextOutA(lpds->hDC, lpds->rcItem.left + 18, lpds->rcItem.top + 4, ETO_OPAQUE | ETO_CLIPPED, &lpds->rcItem, staticText, strlen(staticText), NULL);
					
					//BF_RECT shows the rectangle around the button, BF_MONO doesn't
					DrawEdge(lpds->hDC, &lpds->rcItem, (lpds->itemState& ODS_SELECTED) ? EDGE_SUNKEN : EDGE_RAISED, BF_MONO); //BF_MONO //BF_RECT
				
				}

				break;
			}

		}
		
		break;
	}
	case WM_CTLCOLOREDIT: {
		HDC hdc = (HDC)wParam;
		HWND hwnd = (HWND)lParam;
		
		HWND LicenseKeyBox = this->GetWindow("LicenseKeyBox");

		if (LicenseKeyBox == hwnd) {
			//SetTextAlign(hdc, TA_LEFT);
			SetBkColor(hdc, RGB(34, 34, 34));
			SetTextColor(hdc, RGB(200, 200, 200));
			SetDCBrushColor(hdc, RGB(34, 34, 34));
			return (LRESULT)GetStockObject(DC_BRUSH); // return a DC brush.
		}

		break;
	}

	case WM_CTLCOLORBTN: {
		
		HDC hdc = (HDC)wParam;
		HWND hwnd = (HWND)lParam;

	
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






/**
* This function creates a window and stores the HMENU and HWND in an ordered map.
*/
HWND GUI::ZwCreateWindow(LPCSTR name, int PosX, int PosY, int width, int height, DWORD flags, HMENU child_id, LPCSTR text, std::string UniqueName) {
	
	HWND hwnd = CreateWindowA(name,
								    text, 
								   flags,
									PosX,
									PosY,
								   width,
								  height,
					  this->windowHandle, 
						 (HMENU)child_id,
					 this->loader_handle, 
									NULL);

	
	if (hwnd != INVALID_HANDLE_VALUE) {
		WindowHandleList.insert(std::pair<std::string, std::pair<HWND, HMENU>>(UniqueName, std::pair<HWND, HMENU>(hwnd, child_id)));
	}
	
	return hwnd;
}


/**
* This function searches the WindowHandleList for the HWND associated with the unique name and closes the window.
*/
bool GUI::ExitWindow(std::string UniqueName) {

	std::map<std::string, std::pair<HWND, HMENU>>::iterator it;
	
	it = this->WindowHandleList.find(UniqueName);

	if (it != this->WindowHandleList.end()) { // If the key exists.

		std::pair<HWND, HMENU> value = it->second;
		
		HWND hwnd = value.first;
		HMENU hMenu = value.second;
	
		if (DestroyWindow(hwnd)) {
			this->WindowHandleList.erase(it);
			return true;
		}
		
	}

	return false;
}

/**
* This function searches the WindowHandleList for the HWND associated with the unique name and returns the handle to the window.
*/
HWND GUI::GetWindow(std::string UniqueName) {
	
	std::map<std::string, std::pair<HWND, HMENU>>::iterator it;

	it = this->WindowHandleList.find(UniqueName);

	if (it != this->WindowHandleList.end()) { // If the key exists.

		std::pair<HWND, HMENU> value = it->second;

		HWND hwnd = value.first;
		
		return hwnd;

	}

	return NULL;
}


/**
* This function will display the results to the user.
*/
bool GUI::DisplayLoginResults(Json::Value result) {
	

	// If result is successful, close the Login button and License box.
	if (result["R"].asString() == "Success") {
		std::string output = "Game Selected\n" + result["GN"].asString() + "\n\n" + "Expiration\n" + result["E"].asString() + "\n\n" + "Status\n" + result["S"].asString();
		
		SetWindowTextA(this->GetWindow("LoginOutput"), output.c_str());
		
		SendMessage(this->GetWindow("Login"), WM_CLOSE, NULL, NULL);
		SendMessage(this->GetWindow("LicenseKeyBox"), WM_CLOSE, NULL, NULL);


		SetWindowTextA(this->GetWindow("Inject"), "Inject");
		Connect.LoggedIn = true;

	}
	
	return true;
}


/**
* This thread is responsible for managing/showing the result to the user and prompting them to inject.
*/
PVOID ResultsForm(PVOID args) {
	
	ResultsParams res = *(ResultsParams*)args;
	
	//Connect.frame->DisplayLoginResults(res.result);
	
	
	
	
	

	
	
	
	while (true) {
		Sleep(1000);
		printf("Results\n");
	}

	return 0;
}


/**
* This thread initializes and runs our form. We must create all the windows in advance in this thread and from other threads use SendMessage for any changes.
*/
PVOID MainForm(PVOID args) {

	// Get the parameters from WinMain.
	GuiParams param = *(GuiParams*)args;

	// Construct our GUI object and store it's pointer into Connect class.
	GUI gframe(param.cmdshow, param.hInst);
	Connect.frame = &gframe;

	// We specify BS_OWNERDRAW so in the WM_DRAWITEM Wndproc we can have our own custom login button.
	HWND LoginButton = gframe.ZwCreateWindow("button",
		110,
		280,
		80,
		20, 
		ES_CENTER | WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		(HMENU)IDC_LOGIN, 
		"Login", 
		"Login");
	printf("Login Window HWND %p\n", LoginButton);
	
	// Creating our own close button
	HWND CloseButton = gframe.ZwCreateWindow("button",
		250,
		5,
		45,
		25,
		ES_CENTER | WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		(HMENU)IDC_CLOSE,
		"X",
		"Close");


	// Create the license key box
	HWND LicenseKeyBox = gframe.ZwCreateWindow("edit",
		30,
		250,
		240,
		25,
		ES_CENTER | WS_CHILD | WS_VISIBLE | ES_UPPERCASE,
		(HMENU)IDC_EDIT,
		"",
		"LicenseKeyBox");
	
	LOGFONT logfont;
	ZeroMemory(&logfont, sizeof(LOGFONT));
	logfont.lfCharSet = ANSI_CHARSET;
	logfont.lfHeight = -18;
	logfont.lfWeight = FW_DEMIBOLD;
	logfont.lfQuality = CLEARTYPE_QUALITY;
	logfont.lfPitchAndFamily = FF_DECORATIVE;

	HFONT hFont = CreateFontIndirect(&logfont);

	SendMessage(LicenseKeyBox, EM_SETLIMITTEXT, 20, NULL);
	SendMessage(LicenseKeyBox, WM_SETFONT, (WPARAM)hFont, TRUE);

	
	// Prompting the user to enter the license. 
	SendMessage(LicenseKeyBox, EM_SETCUEBANNER, FALSE, (LPARAM)L"Enter License");

	
	// Create the inject box.
	HWND InjectButton = gframe.ZwCreateWindow("button",
		110,
		310,
		85,
		20,
		ES_CENTER | WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		(HMENU)IDC_INJECT,
		"",
		"Inject");


	// Create the field for the results to be displayed
	HWND LoginOutput = gframe.ZwCreateWindow("static",
		40,
		150,
		300,
		140,
		ES_CENTER | WS_CHILD | WS_VISIBLE | SS_OWNERDRAW,
		(HMENU)IDC_TEXT,
		"",
		"LoginOutput");


	


	
	
	// Run the gui loop.
	gframe.Run();
	
	return 0;
}