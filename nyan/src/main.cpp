#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)
	#include <Windows.h>
#endif

std::string file = "nyan.mp4";

int main() {

	// hide console window
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	ShellExecute(0, 0, file.c_str(), 0, 0, SW_SHOW);
	
}