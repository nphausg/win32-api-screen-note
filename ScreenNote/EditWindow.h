#pragma once

extern HWND			hWndEditControl;
extern HWND			hWndEditWindow;

extern WCHAR*		szEditClass;
extern WCHAR*		szEditTitleName;
extern HINSTANCE	hInst;

extern int nHeight, nWidth;
extern int cxScreen, cyScreen;
extern int midCxScreen, midCyScreen;
extern int deltaCXWindow, deltaCYWindow;

LRESULT CALLBACK EditWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	RECT		rect;
	PAINTSTRUCT ps;

	HRGN		hRegion;
	switch (message)
	{
	case WM_CREATE:

		// Create Edit Control
		hWndEditControl = CreateWindow(L"EDIT", NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_NOHIDESEL |
			ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
			0, 0, 0, 0, hWnd, NULL, hInst, NULL);

		if (hWndEditControl != NULL)
		{
			HFONT hFont = CreateFont(
				20, 8, 0, 0,
				FW_NORMAL,
				FALSE,	// Italice
				FALSE, // Underline
				FALSE, // StrikeOut
				VIETNAMESE_CHARSET,
				OUT_OUTLINE_PRECIS,
				CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Times New Roman"));
			SendMessage(hWndEditControl, WM_SETFONT, (WPARAM)hFont, NULL);
		}
		hRegion = CreateRoundRectRgn(0, 0, nWidth, nHeight, 20, 20);
		SetWindowRgn(hWnd, hRegion, false);
		break;
	case WM_SETFOCUS:
		SetFocus(hWndEditControl);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//
		GetClientRect(hWnd, &rect);
		rect.left = 10;
		rect.top = 20;
		DrawTextW(hdc, szEditTitleName, _tcslen(szEditTitleName), &rect, DT_LEFT | DT_INTERNAL | DT_MODIFYSTRING);
		//
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		// resize edit window to fit the client area of the main window
		MoveWindow(hWndEditControl, 10, 70, LOWORD(lParam), HIWORD(lParam), TRUE);
		break;
	case WM_CLOSE:
		SendMessage(hWnd, WM_DESTROY, 0, 0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM EditWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = EditWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SCREENNOTE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szEditClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));;

	return RegisterClassEx(&wcex);
}

BOOL InitEditWindowInstance(HINSTANCE hInstance, int nCmdShow)
{
	nWidth = midCxScreen - 2 * deltaCXWindow;
	nHeight = 2 * (midCyScreen - deltaCYWindow);

	hWndEditWindow = CreateWindowEx(
		WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
		szEditClass, NULL,
		WS_POPUP | WS_VISIBLE,
		deltaCXWindow, deltaCYWindow, nWidth, nHeight,
		NULL, NULL, hInst, NULL);

	if (hWndEditWindow)
	{
		ShowWindow(hWndEditWindow, SW_SHOW);
		UpdateWindow(hWndEditWindow);
		return TRUE;
	}
	return FALSE;
}
