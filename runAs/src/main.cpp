#pragma warning(disable:4996)


#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <mutex>
#include <filesystem>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

namespace fs = std::experimental::filesystem;

#if defined(_WIN32)
	#include <Windows.h>
	#include <Lmcons.h>
	#include <shlobj.h>

#endif



void execute(std::string fileToRun, char *dir = NULL) {


	std::vector<char> v3(fileToRun.length() + 1);
	std::strcpy(&v3[0], fileToRun.c_str());
	char *lpApplicationName = &v3[0];


	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	/*
	std::vector<char> v1(desiredCurrentDirectory.length() + 1);
	std::strcpy(&v1[0], desiredCurrentDirectory.c_str());
	char* desiredCurrentDirChar = &v1[0];
	*/

	// start the program up
	CreateProcess(lpApplicationName,   // the path
		NULL,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		dir,           // Use parent's starting directory by default(if null)
		&si,            // Pointer to STARTUPINFO structure
		&pi);           // Pointer to PROCESS_INFORMATION structure


						/*
						// Close process and thread handles.
						CloseHandle(pi.hProcess);
						CloseHandle(pi.hThread);
						*/
}

int main() {

	execute("cmd.exe");

	return 0;
}