
#include <Windows.h>
#include <string.h>
#include <wchar.h>
#include <tchar.h>

int sendKeystrokes(const TCHAR *const text) {

	INPUT *keystroke;
	UINT i, character_count, keystrokes_to_send, keystrokes_sent;
	HWND notepad;

	//assert(text != NULL);

	////Bring the Notepad window to the front.
	//if (!SetForegroundWindow(notepad))
	//	return 0;

	//Fill in the array of keystrokes to send.
	character_count = _tcslen(text);
	keystrokes_to_send = character_count * 2;
	keystroke = new INPUT[keystrokes_to_send];
	for (i = 0; i < character_count; ++i)
	{
		keystroke[i * 2].type = INPUT_KEYBOARD;
		keystroke[i * 2].ki.wVk = 0;
		keystroke[i * 2].ki.wScan = text[i];
		keystroke[i * 2].ki.dwFlags = KEYEVENTF_UNICODE;
		keystroke[i * 2].ki.time = 0;
		keystroke[i * 2].ki.dwExtraInfo = GetMessageExtraInfo();

		keystroke[i * 2 + 1].type = INPUT_KEYBOARD;
		keystroke[i * 2 + 1].ki.wVk = 0;
		keystroke[i * 2 + 1].ki.wScan = text[i];
		keystroke[i * 2 + 1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
		keystroke[i * 2 + 1].ki.time = 0;
		keystroke[i * 2 + 1].ki.dwExtraInfo = GetMessageExtraInfo();
	}

	//Send the keystrokes.
	keystrokes_sent = SendInput((UINT)keystrokes_to_send, keystroke, sizeof(*keystroke));
	delete[] keystroke;

	return keystrokes_sent == keystrokes_to_send;
}