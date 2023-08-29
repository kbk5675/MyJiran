#pragma once
#include "pch.h"
#include "KeyboardHookManager.h"

KeyboardHookManager* g_key_hookMgr = nullptr;

KeyboardHookManager::KeyboardHookManager()
{
	g_key_hookMgr	= this;
	hKeyboardHook	= NULL;
	b_is_capture_ignored		= false;
}

KeyboardHookManager::~KeyboardHookManager()
{

}

void KeyboardHookManager::HookingKeyboard(int hookType)
{
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	if (hInstance == nullptr)
	{
		return;
	}

	switch (hookType)
	{
	case 0:
		/*hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardHookProc_default, hInstance, 0);
		if (hKeyboardHook == nullptr)
		{
			return;
		}*/
		break;
	case 1:
		hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardHookProc_capture, hInstance, 0);
		if (hKeyboardHook == nullptr)
		{
			return;
		}
		break;
	}
}

void KeyboardHookManager::UnhookingKeyboard()
{
	UnhookWindowsHookEx(hKeyboardHook);
}

//LRESULT CALLBACK KeyboardHookManager::KeyBoardHookProc_default(int nCode, WPARAM wParam, LPARAM lParam)
//{
//	if (nCode >= 0)
//	{
//		if (wParam == WM_KEYDOWN)
//		{
//			KBDLLHOOKSTRUCT* pKeyboardStruct = (KBDLLHOOKSTRUCT*)lParam;
//
//			if (pKeyboardStruct->vkCode)
//			{
//				OutputDebugStringA("Default Hooking\n");
//			}
//		}
//	}
//
//	return CallNextHookEx(NULL, nCode, wParam, lParam);
//}

LRESULT CALLBACK KeyboardHookManager::KeyBoardHookProc_capture(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		if (wParam == WM_KEYDOWN)
		{
			KBDLLHOOKSTRUCT* pKeyboardStruct = (KBDLLHOOKSTRUCT*)lParam;

			if (pKeyboardStruct->vkCode == 'S' &&
				(GetAsyncKeyState(VK_LWIN) & 0x8000) &&
				(GetAsyncKeyState(VK_SHIFT) & 0x8000))
			{
				g_key_hookMgr->b_is_capture_ignored = true;
				UnhookWindowsHookEx(g_key_hookMgr->hKeyboardHook);
				OutputDebugStringA("\nScreenshot ignored!!!\n");
				return 1;
			}
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

KeyboardHookManager* CreateKeyboardHookMgr()
{
	return new KeyboardHookManager();
}