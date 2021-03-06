#include <tchar.h>

#include "cursorwindow.hpp"
#include "globals.hpp"
#include "debug.hpp"

CursorWindow::CursorWindow()
:
	d_class(createClass()),
	d_handle(createWindow())
{
	DEBUGLOG("Created cursor window 0x%08x", d_handle);
}

CursorWindow::~CursorWindow() {
	DestroyWindow(d_handle);
	UnregisterClass(MAKEINTATOM(d_class), GetModuleHandle(NULL));
	DEBUGLOG("Destroyed cursor window 0x%08x", d_handle);
}

void CursorWindow::setCursor(Cursor cursor) {
	SetClassLongPtr(d_handle, GCLP_HCURSOR, (LONG_PTR)globals->cursors().get(cursor));
	// SetWindowPos documentation: "If you have changed certain window data using SetWindowLong,
	// you must call SetWindowPos for the changes to take effect.
	// SetWindowPos(d_handle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

ATOM CursorWindow::createClass() {
	WNDCLASS wc;
	wc.style = 0;
	wc.lpfnWndProc = &winProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _T("CurWin");
	return RegisterClass(&wc);
}

HWND CursorWindow::createWindow() {
	int x = GetSystemMetrics(SM_XVIRTUALSCREEN), y = GetSystemMetrics(SM_YVIRTUALSCREEN);
	int w = GetSystemMetrics(SM_CXVIRTUALSCREEN), h = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	return CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, MAKEINTATOM(d_class), _T(""), WS_POPUP | WS_VISIBLE, x, y, w, h, NULL, NULL, GetModuleHandle(NULL), 0);
}

LRESULT CALLBACK CursorWindow::winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			// If this happens, something has gone badly wrong. Since we don't want to keep
			// this window, but cannot delete it from here, we hide it and hope for the best.
			//DEBUGLOG("Hiding cursor window 0x%08x", hwnd);
			//ShowWindow(hwnd, SW_HIDE);
			return 0;
		case WM_PAINT:
			// If we do not at least call BeginPaint()/EndPaint(),
			// the system will keep sending this message over and over again.
			// The CPU won't get any rest!
			paint(hwnd);
			return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CursorWindow::paint(HWND window) {
	PAINTSTRUCT p;
	BeginPaint(window, &p);
#ifdef _DEBUG
	RECT rect;
	GetClientRect(window, &rect);
	DrawEdge(p.hdc, &rect, EDGE_SUNKEN, BF_LEFT | BF_RIGHT | BF_BOTTOM | BF_TOP);
#endif
	EndPaint(window, &p);
}
