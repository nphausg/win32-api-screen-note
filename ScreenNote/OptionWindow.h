#pragma once
#include "resource.h"
#include <windowsx.h>
#include <vector>

using namespace std;

extern WCHAR*		szOptionClass;
extern HINSTANCE	hInst;

extern HWND			hWndOptionWindow;

extern int deltaCXWindow;
extern int deltaCYWindow;
extern int nHeight, nWidth;
extern int cxScreen, cyScreen;
extern int midCxScreen, midCyScreen;

static enum FIGURE {
	RECTANGLE, ELLIPSE, LINE, FREE
};


static FIGURE figure = FIGURE::LINE;


#define BUTTON_WEIGHT 70
#define BUTTON_HEIGHT deltaCYWindow / 4
#define DELTA 20
#define BUTTON_ID_LINE 0x1000
#define BUTTON_ID_PIXEL 0x1001
#define BUTTON_ID_ELLIPSE 0x1002
#define BUTTON_ID_RECTANGLE 0x1003

BOOL CreateButton(HWND hWnd, WCHAR* buttonName, int itemId, int nCmdShow, int x, int y)
{
	HWND btnHwnd = CreateWindow(
		L"BUTTON", buttonName,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD |
		BS_CENTER | BS_NOTIFY | BS_PUSHBUTTON,
		x, y,
		BUTTON_WEIGHT, BUTTON_HEIGHT,
		hWnd, (HMENU)itemId, hInst, NULL);

	if (btnHwnd != NULL)
	{
		ShowWindow(btnHwnd, nCmdShow);
		return TRUE;
	}
	return FALSE;
}


LRESULT CALLBACK OptionWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT	rect;
	HRGN		hRegion;
	switch (message)
	{
	case WM_CREATE:

		// Get the size of client of window
		GetClientRect(hWnd, &rect);

		// Create Button Control
		CreateButton(hWnd, L"F", BUTTON_ID_PIXEL, SW_SHOW, rect.right - BUTTON_WEIGHT - DELTA, rect.bottom / 2);
		CreateButton(hWnd, L"L", BUTTON_ID_LINE, SW_SHOW, rect.right - 2 * BUTTON_WEIGHT - DELTA, rect.bottom / 2);
		CreateButton(hWnd, L"E", BUTTON_ID_ELLIPSE, SW_SHOW, rect.right - 3 * BUTTON_WEIGHT  - DELTA, rect.bottom / 2);
		CreateButton(hWnd, L"R", BUTTON_ID_RECTANGLE, SW_SHOW, rect.right - 4 * BUTTON_WEIGHT - DELTA, rect.bottom / 2);


		hRegion = CreateRoundRectRgn(nWidth - BUTTON_WEIGHT * 4 - DELTA, rect.bottom / 2, nWidth - DELTA, rect.bottom / 2 + BUTTON_HEIGHT, 0, 0);
		SetWindowRgn(hWnd, hRegion, false);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case BUTTON_ID_LINE: figure = FIGURE::LINE; break;
		case BUTTON_ID_PIXEL: figure = FIGURE::FREE; break;
		case BUTTON_ID_ELLIPSE: figure = FIGURE::ELLIPSE; break;
		case BUTTON_ID_RECTANGLE: figure = FIGURE::RECTANGLE; break;
		default: figure = FIGURE::FREE; break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM OptionWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;
	wcex.lpfnWndProc = OptionWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SCREENNOTE));
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szOptionClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));;

	return RegisterClassEx(&wcex);
}

BOOL OptionWindowInstance(HINSTANCE hInstance, int nCmdShow)
{
	nWidth = midCxScreen - 2 * deltaCXWindow;
	nHeight = 2 * (midCyScreen - deltaCYWindow);

	hWndOptionWindow = CreateWindowEx(
		WS_EX_TOOLWINDOW| WS_EX_TRANSPARENT | WS_EX_TOPMOST,
		szOptionClass, NULL,
		WS_POPUP | WS_VISIBLE,
		midCxScreen + deltaCXWindow,
		deltaCYWindow / 4, nWidth,
		deltaCYWindow - deltaCYWindow / 4,
		NULL, NULL, hInst, NULL);

	if (hWndOptionWindow)
	{
		ShowWindow(hWndOptionWindow, SW_SHOW);
		UpdateWindow(hWndOptionWindow);
		return TRUE;
	}

	return FALSE;

}
