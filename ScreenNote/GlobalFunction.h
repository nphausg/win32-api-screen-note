#pragma once

HWND		hWndEditWindow = NULL;
HWND		hWndEditControl = NULL;
HWND		hWndOptionWindow = NULL;
HINSTANCE	hInst = NULL;

// Window class name
WCHAR*		szEditClass = L"EditClass";
WCHAR*		szOptionClass = L"OptionClass";
WCHAR*		szGraphicsClass = L"GraphicsClass";

// Window title name
WCHAR*		szEditTitleName = TEXT("Đây là cửa sổ note dạng text\nCho phép gõ các note văn bản");

HPEN		hPen;
POINT		pStart = { 0 };
POINT		pCurrent = { 0 };

enum COLORREF_MODE { BLACK, RED, GREEN, BLUE };
const COLORREF COLORREF_LIST[] = {
	RGB(0, 0, 0),		// BLACK
	RGB(255, 0, 0),		// RED
	RGB(0, 255, 0),		// GREEN
	RGB(0, 0, 255)		// BLUE
};

// The size of all windows
int		cxScreen = GetSystemMetrics(SM_CXSCREEN);
int		cyScreen = GetSystemMetrics(SM_CYSCREEN);
int		deltaCXWindow = 50;
int		deltaCYWindow = 100;
int		nHeight = 0, nWidth = 0;
int		midCxScreen = cxScreen / 2;
int		midCyScreen = cyScreen / 2;


//
// DRAWING FUNCTIONS
//

class MyFigure
{
protected:
	RECT rect;
public:
	MyFigure(RECT _rect)
	{
		rect.bottom = _rect.bottom;
		rect.top = _rect.top;
		rect.right = _rect.right;
		rect.left = _rect.left;
	}

	virtual void Paint(HDC){};
};

static vector<MyFigure*> shapes;

class MyLine : public MyFigure
{
public:
	MyLine(RECT _rect) : MyFigure(_rect){};

	void Paint(HDC hdc)
	{
		MoveToEx(hdc, rect.left, rect.top, NULL);
		LineTo(hdc, rect.right, rect.bottom);
	}
};
class MyEllipse : public MyFigure
{
public:
	MyEllipse(RECT _rect) : MyFigure(_rect){};
	void Paint(HDC hdc)
	{
		Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
	}
};
class MyRectangle : public MyFigure
{
public:
	MyRectangle(RECT _rect) : MyFigure(_rect){};
	void Paint(HDC hdc)
	{
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
	}
};


LRESULT DrawLine(HWND hWnd, POINT pStart, POINT pCurr)
{
	// Get a client DC.
	HDC hdc = GetDC(hWnd);

	SelectObject(hdc, hPen);
	SetROP2(hdc, R2_NOTXORPEN);
	MoveToEx(hdc, pStart.x, pStart.y, NULL);
	LineTo(hdc, pCurr.x, pCurr.y);

	SetROP2(hdc, R2_COPYPEN);
	MoveToEx(hdc, pStart.x, pStart.y, NULL);
	LineTo(hdc, pCurr.x, pCurr.y);
	ReleaseDC(hWnd, hdc);

	RECT rect;

	rect.bottom = pCurr.y;
	rect.left = pStart.x;
	rect.right = pCurr.x;
	rect.top = pStart.y;

	MyLine * myLine = new MyLine(rect);
	shapes.push_back(myLine);

	return TRUE;
}

LRESULT DrawRectangle(HWND hWnd, POINT pStart, POINT pCurr)
{
	// Get a client DC.
	HDC hdc = GetDC(hWnd);

	SelectObject(hdc, hPen);
	SetROP2(hdc, R2_NOTXORPEN);
	MoveToEx(hdc, pStart.x, pStart.y, NULL);
	Rectangle(hdc, pStart.x, pStart.y, pCurr.x, pCurr.y);

	SetROP2(hdc, R2_COPYPEN);
	MoveToEx(hdc, pStart.x, pStart.y, NULL);
	Rectangle(hdc, pStart.x, pStart.y, pCurr.x, pCurr.y);
	ReleaseDC(hWnd, hdc);

	RECT rect;

	rect.bottom = pCurr.y;
	rect.left = pStart.x;
	rect.right = pCurr.x;
	rect.top = pStart.y;

	MyRectangle *rectangle = new MyRectangle(rect);
	shapes.push_back(rectangle);

	return TRUE;
}
LRESULT DrawEllipse(HWND hWnd, POINT pStart, POINT pCurr)
{
	// Get a client DC.
	HDC hdc = GetDC(hWnd);

	SelectObject(hdc, hPen);
	SetROP2(hdc, R2_NOTXORPEN);
	MoveToEx(hdc, pStart.x, pStart.y, NULL);
	Ellipse(hdc, pStart.x, pStart.y, pCurr.x, pCurr.y);

	SetROP2(hdc, R2_COPYPEN);
	MoveToEx(hdc, pStart.x, pStart.y, NULL);
	Ellipse(hdc, pStart.x, pStart.y, pCurr.x, pCurr.y);
	ReleaseDC(hWnd, hdc);

	RECT rect;

	rect.bottom = pCurr.y;
	rect.left = pStart.x;
	rect.right = pCurr.x;
	rect.top = pStart.y;

	MyEllipse* ellipse = new MyEllipse(rect);
	shapes.push_back(ellipse);

	return TRUE;
}

//
// HOOK FUNCTIONS
//
void InstallHook(HWND hWnd)
{
	typedef VOID(*MYPROC)(HWND);

	HINSTANCE hInstLib;
	MYPROC ProcAddr;

	hInstLib = LoadLibrary(L"ScreenNoteHookDLL.dll");

	if (hInstLib != NULL)
	{
		ProcAddr = (MYPROC)GetProcAddress(hInstLib, "fnInstallHook");
		if (ProcAddr != NULL)
			ProcAddr(hWnd);
	}
}
void UnInstallHook(HWND hWnd)
{
	typedef VOID(*MYPROC)(HWND);

	HINSTANCE hInstLib;
	MYPROC ProcAddr;

	hInstLib = LoadLibrary(L"ScreenNoteHookDLL.dll");

	if (hInstLib != NULL)
	{
		ProcAddr = (MYPROC)GetProcAddress(hInstLib, "fnUnInstallHook");
		if (ProcAddr != NULL)
			ProcAddr(hWnd);
	}
}