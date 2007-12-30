#ifndef _HOOKS_H_
#define _HOOKS_H_

#include <windows.h>

extern "C" {

	DWORD __declspec(dllexport) __stdcall init(DWORD threadId);
	void __declspec(dllexport) __stdcall readConfig();

	LRESULT __declspec(dllexport) __stdcall mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	LRESULT __declspec(dllexport) __stdcall keyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

}

#endif