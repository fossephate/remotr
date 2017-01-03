#include <stdio.h>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <string>

#include <chrono>
#include <thread>

#if defined(_WIN32)
	#include <Windows.h>
#endif



int main() {

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	
	for (int i = 0; i < 4; ++i) {

		int numOfMilliSeconds = rand() % 10000 + 1;

		std::this_thread::sleep_for(std::chrono::milliseconds(numOfMilliSeconds));

		MessageBox(nullptr, TEXT("ERROR!"), TEXT("ERROR!"), MB_ICONINFORMATION | MB_OKCANCEL);
	}

	return 0;
}