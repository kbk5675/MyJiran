#pragma once
#include "KeyboardHook.h"
#include "pch.h"


class KeyboardHookManager : public KeyboardHook
{
public:
	KeyboardHookManager();
	~KeyboardHookManager();

	void HookingKeyboard(int hookType);
	void UnhookingKeyboard();
	bool GetbIsCapture()			{ return b_is_capture_ignored; }
	void SetbIsCapture(bool value)	{ b_is_capture_ignored = value; }

	//static LRESULT	CALLBACK KeyBoardHookProc_default(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT	CALLBACK KeyBoardHookProc_capture(int nCode, WPARAM wParam, LPARAM lParam);

public:
	HHOOK hKeyboardHook;
	bool b_is_capture_ignored;
};

extern "C" __declspec(dllexport) KeyboardHookManager* CreateKeyboardHookMgr();