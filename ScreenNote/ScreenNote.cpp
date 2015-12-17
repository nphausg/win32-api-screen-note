// ScreenNote.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ScreenNote.h"


#define MAX_LOADSTRING 100
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	// LoadString(hInstance, IDS_APP_TITLE, szTitle, 0);
	// LoadString(hInstance, IDC_SCREENNOTE, szWindowClass, 0);

	MyRegisterClass(hInstance);
	EditWindowClass(hInstance);
	OptionWindowClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SCREENNOTE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SCREENNOTE));
	wcex.hCursor = LoadCursor(NULL, IDC_HAND);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szGraphicsClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	nWidth = midCxScreen - 2 * deltaCXWindow;
	nHeight = 2 * (midCyScreen - deltaCYWindow);

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
		szGraphicsClass, NULL,
		WS_POPUP | WS_VISIBLE,
		midCxScreen + deltaCXWindow, deltaCYWindow,
		nWidth, nHeight, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, SW_SHOW);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;
	
	switch (message)
	{
	case WM_CREATE:
		return OnCreate(hWnd);
	case WM_SIZE:
		return OnSize(hWnd, wParam);
	case WM_RBUTTONDOWN:
		return OnLButtonDown(hWnd);
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONUP:
		return OnLButtonUp(hWnd, pStart, pCurrent, hPen, figure);
	case WM_LBUTTONDOWN:
		return OnLButtonDown(lParam, pStart, pCurrent);
	case WM_MOUSEMOVE:
		return OnMouseMove(hWnd, wParam, lParam, pStart, pCurrent, figure);
	case WM_NOTIFYICON:
		return OnNotify(hWnd, wParam, lParam);
	case WM_SCREEN_HOOK_MSG:
		if (IsWindowVisible(hWnd))
		{
			SendMessage(hWnd, WM_SIZE, SIZE_MINIMIZED, 0);
			SendMessage(hWndEditWindow, WM_SIZE, SIZE_MINIMIZED, 0);
			SendMessage(hWndOptionWindow, WM_SIZE, SIZE_MINIMIZED, 0);
			return 0;
		}
		else
		{
			ShowWindow(hWnd, SW_SHOW);
			ShowWindow(hWndEditWindow, SW_SHOW);
			ShowWindow(hWndOptionWindow, SW_SHOW);
			return 0;
		}
		break;
	
	case WM_CLOSE:
		return OnClose(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

