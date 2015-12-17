// ScreenNoteHookDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ScreenNoteHookDLL.h"

LRESULT CALLBACK ScreenNoteProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// Không xử lý
	if (nCode < 0 || nCode == HC_NOREMOVE)
		return CallNextHookEx(hHook, nCode, wParam, lParam);
	
	
	if (wParam == WM_RBUTTONDOWN && GetKeyState(VK_CONTROL) < 0)
		PostMessage(hWnd, WM_SCREEN_HOOK_MSG, wParam, lParam);

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

SCREENNOTEHOOKDLL_API BOOL fnInstallHook(HWND hWndCaller)
{
	
	// Đã cài hook
	if (hHook != NULL) return FALSE;

	// Định danh tiến trình gọi
	hWnd = hWndCaller;

	hHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)ScreenNoteProc, hInstLib, 0);
	return (hHook) ? TRUE : FALSE;
}
SCREENNOTEHOOKDLL_API BOOL fnUnInstallHook(HWND hWndCaller)
{
	if (!hHook) return FALSE;
	UnhookWindowsHookEx(hHook);
	hHook = NULL;
	return TRUE;
}