#pragma once

#include "resource.h"
#include "EditWindow.h"
#include "OptionWindow.h"
#include "GlobalFunction.h"
#include <shellapi.h>
#include "../ScreenNoteHookDLL/ScreenNoteHookDLL.h"


BOOL				InitInstance(HINSTANCE, int);
ATOM				MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void				InstallHook(HWND);
void				UnInstallHook(HWND);

#define  TRAY_ICON_ID		   1	
#define WM_NOTIFYICON (WM_USER + 101)


LRESULT OnSystemTray(HWND hWnd, UINT uID, HICON hIcon, LPCWSTR lpszTip)
{
	BOOL result = FALSE;
	NOTIFYICONDATA tnid;

	tnid.cbSize = sizeof(NOTIFYICONDATA);
	tnid.hWnd = hWnd;
	tnid.uID = uID;
	tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnid.uCallbackMessage = WM_NOTIFYICON;
	tnid.hIcon = hIcon;
	if (lpszTip)
		lstrcpyn(tnid.szTip, lpszTip, sizeof(tnid.szTip));
	else
		tnid.szTip[0] = '\0';

	result = Shell_NotifyIcon(NIM_ADD, &tnid);

	if (hIcon)
		DestroyIcon(hIcon);

	return result;
}

LRESULT OnSize(HWND hWnd, WPARAM wParam)
{
	if (wParam == SIZE_MINIMIZED)
	{
		ShowWindow(hWnd, SW_HIDE);
		ShowWindow(hWndEditWindow, SW_HIDE);
		ShowWindow(hWndOptionWindow, SW_HIDE);
		HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
		OnSystemTray(hWnd, TRAY_ICON_ID, hIcon,
			(LPCWSTR)L"Click or ctrl + right click to restore!");
		return TRUE;
	}
	return FALSE;
}

LRESULT StopSystemTray(HWND hWnd, UINT uID)
{
	BOOL result = FALSE;
	NOTIFYICONDATA tnid;

	tnid.cbSize = sizeof(NOTIFYICONDATA);
	tnid.hWnd = hWnd;
	tnid.uID = uID;

	result = Shell_NotifyIcon(NIM_DELETE, &tnid);
	return result;
}
LRESULT OnNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (lParam == WM_LBUTTONDOWN)
	{
		ShowWindow(hWnd, SW_SHOW);
		ShowWindow(hWnd, SW_RESTORE);

		ShowWindow(hWndEditWindow, SW_SHOW);
		ShowWindow(hWndEditWindow, SW_RESTORE);

		ShowWindow(hWndOptionWindow, SW_SHOW);
		ShowWindow(hWndOptionWindow, SW_RESTORE);

		return StopSystemTray(hWnd, TRAY_ICON_ID);
	}
	return FALSE;
}

void OnPaint(HDC hdc)
{
	if (shapes.empty())
		return;

	SelectObject(hdc, hPen);
	for (int i = 0; i < shapes.size(); i++)
		shapes[i]->Paint(hdc);

}
LRESULT OnCreate(HWND hWnd)
{
	// Create windows
	InitEditWindowInstance(hInst, SW_SHOW);
	OptionWindowInstance(hInst, SW_SHOW);

	// Pen
	hPen = CreatePen(PS_SOLID, 2, COLORREF_LIST[COLORREF_MODE::BLACK]);

	// Install Hook
	InstallHook(hWnd);

	HRGN hRegion = CreateRoundRectRgn(0, 0, nWidth, nHeight, 20, 20);
	SetWindowRgn(hWnd, hRegion, false);

	return TRUE;
}
LRESULT OnClose(HWND hWnd)
{
	// UnInstall Hook
	UnInstallHook(hWnd);
	SendMessage(hWnd, WM_DESTROY, 0, 0);
	return TRUE;
}

LRESULT OnLButtonDown(HWND hWnd)
{
	// Xoá tất cả các hình đã vẽ
	shapes.clear();
	SendMessage(hWnd, WM_ERASEBKGND, (WPARAM)GetDC(hWnd), 0);
	return TRUE;
}
LRESULT OnLButtonDown(LPARAM lParam, POINT& pStart, POINT& pCurr)
{
	// Set the start point
	pStart.x = pCurr.x = GET_X_LPARAM(lParam); // LOWORD(lParam)
	pStart.y = pCurr.y = GET_Y_LPARAM(lParam); // HIWORD(lParam)
	return FALSE;
}
LRESULT OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam, POINT& pStart, POINT& pCurr, FIGURE figure)
{
	HDC hdc = GetDC(hWnd);

	if (!(wParam & MK_LBUTTON)) return FALSE;

	switch (figure)
	{
	case FIGURE::LINE:

		SetROP2(hdc, R2_NOTXORPEN);
		MoveToEx(hdc, pStart.x, pStart.y, NULL);
		LineTo(hdc, pCurr.x, pCurr.y);

		pCurr.x = GET_X_LPARAM(lParam);
		pCurr.y = GET_Y_LPARAM(lParam);

		MoveToEx(hdc, pStart.x, pStart.y, NULL);
		LineTo(hdc, pCurr.x, pCurr.y);
		ReleaseDC(hWnd, hdc);

		return TRUE;
	case FIGURE::FREE:

		pCurr.x = GET_X_LPARAM(lParam);
		pCurr.y = GET_Y_LPARAM(lParam);

		MoveToEx(hdc, pStart.x, pStart.y, NULL);
		LineTo(hdc, pCurr.x, pCurr.y);
		pStart.x = pCurr.x;
		pStart.y = pCurr.y;

		ReleaseDC(hWnd, hdc);
		return TRUE;
	case FIGURE::ELLIPSE:

		SetROP2(hdc, R2_NOTXORPEN);
		MoveToEx(hdc, pStart.x, pStart.y, NULL);
		Ellipse(hdc, pStart.x, pStart.y, pCurr.x, pCurr.y);

		pCurr.x = GET_X_LPARAM(lParam);
		pCurr.y = GET_Y_LPARAM(lParam);

		MoveToEx(hdc, pStart.x, pStart.y, NULL);
		Ellipse(hdc, pStart.x, pStart.y, pCurr.x, pCurr.y);
		ReleaseDC(hWnd, hdc);

		return TRUE;
	case FIGURE::RECTANGLE:

		SetROP2(hdc, R2_NOTXORPEN);
		MoveToEx(hdc, pStart.x, pStart.y, NULL);
		Rectangle(hdc, pStart.x, pStart.y, pCurr.x, pCurr.y);

		pCurr.x = GET_X_LPARAM(lParam);
		pCurr.y = GET_Y_LPARAM(lParam);

		MoveToEx(hdc, pStart.x, pStart.y, NULL);
		Rectangle(hdc, pStart.x, pStart.y, pCurr.x, pCurr.y);
		ReleaseDC(hWnd, hdc);

		return TRUE;
	}
	return FALSE;
}
LRESULT OnLButtonUp(HWND hWnd, POINT pStart, POINT pCurr, HPEN hPen, FIGURE figure)
{
	switch (figure)
	{
	case FIGURE::LINE:
		return DrawLine(hWnd, pStart, pCurr);
	case FIGURE::ELLIPSE:
		return DrawEllipse(hWnd, pStart, pCurr);
	case FIGURE::RECTANGLE:
		return DrawRectangle(hWnd, pStart, pCurr);
	}
	return FALSE;
}
