// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SCREENNOTEHOOKDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SCREENNOTEHOOKDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SCREENNOTEHOOKDLL_EXPORTS
#define SCREENNOTEHOOKDLL_API __declspec(dllexport)
#else
#define SCREENNOTEHOOKDLL_API __declspec(dllimport)
#endif

HWND		hWnd = NULL;
HHOOK		hHook = NULL;
HINSTANCE	hInstLib = NULL;

SCREENNOTEHOOKDLL_API BOOL fnInstallHook(HWND = NULL);
SCREENNOTEHOOKDLL_API BOOL fnUnInstallHook(HWND = NULL);

#define START_POINT 10001
#define WM_SCREEN_HOOK_MSG (WM_USER + START_POINT)
