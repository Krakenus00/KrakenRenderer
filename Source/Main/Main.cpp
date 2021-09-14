#ifndef UNICODE
#define UNICODE
#endif

#if defined(_WIN32)

#include "WinPrefs.h"

int WINAPI wWinMain(
	_In_		HINSTANCE hInstance,
	_In_opt_	HINSTANCE hPrevInstance,
	_In_		LPWSTR pCmdLine,
	_In_		int nCmdShow)
{

}

#elif defined(__APPLE__)
#error "MacOS is currently unsupported. Sorry :)"
#else
#error "Maybe next time."
#endif
