
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


void sendEnter() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_RETURN;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_RETURN;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendBackspace() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_BACK;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_BACK;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendTab() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_TAB;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_TAB;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendUpArrow() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_UP;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_UP;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendDownArrow() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_DOWN;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_DOWN;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendLeftArrow() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LEFT;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_LEFT;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendRightArrow() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_RIGHT;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_RIGHT;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendShift() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_SHIFT;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_SHIFT;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendCaps() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_CAPITAL;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_CAPITAL;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendControl() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_CONTROL;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_CONTROL;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendAlt() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_MENU;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_MENU;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendDelete() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_DELETE;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_DELETE;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendInsert() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_INSERT;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_INSERT;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendWindows() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_LWIN;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_LWIN;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}

void sendEscape() {
	INPUT input[2];

	// key down
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = VK_ESCAPE;
	input[0].ki.dwFlags = 0;
	input[0].ki.dwExtraInfo = GetMessageExtraInfo();

	// key up:
	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = VK_ESCAPE;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;
	input[1].ki.dwExtraInfo = GetMessageExtraInfo();

	SendInput(2, input, sizeof(INPUT));
}