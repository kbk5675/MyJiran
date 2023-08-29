#include "pch.h"
#include "BlockingCapture.h"
#include "OfficeManager.h"
#include "KeyboardHook.h"
#include <Windows.h>
#include <Psapi.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

#define GetKeeperMgr OfficeManager::GetOfficeMgr()
#define GetThreadMgr OfficeManager::GetOfficeMgr()->GetThreadMgr()


typedef KeyboardHook* (*CreateKeyboardHookMgr)();

BlockingCapture* g_this = nullptr;

BlockingCapture::BlockingCapture()
{
	g_this				= this;
	m_keyboardHookMgr	= nullptr;
	m_hDll_keyboardHook = NULL;

	m_ctrlProces.push_back(L"whale.exe");
}

BlockingCapture::~BlockingCapture()
{
	m_keyboardHookMgr = nullptr;
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
}

void BlockingCapture::Start_blocking_capture()
{
	GetThreadMgr->StartThread("thread_blockingCapture", 100, ThreadBlockingCapture);
}

UINT BlockingCapture::ThreadBlockingCapture(LPVOID _mothod)
{
	int thread_speed = GetThreadMgr->GetThreadSpeed("thread_blockingCapture");

	while (GetThreadMgr->GetThreadState("thread_blockingCapture") == EThreadState::RUNNING)
	{
		g_this->SearchControlWindow();
		Sleep(thread_speed);
	}

	return 0;
}



void BlockingCapture::Stop_blocking_capture()
{
	if (m_hDll_keyboardHook != NULL)
	{
		FreeLibrary(m_hDll_keyboardHook);
	}
		
	GetThreadMgr->StopThread("thread_blockingCapture");
}

void BlockingCapture::SearchControlWindow()
{
	EnumWindows(EnumProc, NULL);
}

BOOL BlockingCapture::EnumProc(HWND hwnd, LPARAM lParam)
{
	CString procName = g_this->GetProcName(hwnd);

	if (!procName.IsEmpty())
	{
		for (auto iterator : g_this->m_ctrlProces)
		{
			CString cstr = iterator;

			if (procName.Compare(cstr) == 0)
			{
				g_this->StartKeyboardHook();
				return FALSE;
			}
		}
	}
	return TRUE;
}

void BlockingCapture::StartKeyboardHook()
{
	m_hDll_keyboardHook = LoadLibrary(L"Dll_keyboardHook.dll");
	if (m_hDll_keyboardHook)
	{
		CreateKeyboardHookMgr lpCreateKeyboardHookMgr;
		lpCreateKeyboardHookMgr = (CreateKeyboardHookMgr)GetProcAddress(m_hDll_keyboardHook, "CreateKeyboardHookMgr");
		if (lpCreateKeyboardHookMgr)
		{
			m_keyboardHookMgr = lpCreateKeyboardHookMgr();

			if (m_keyboardHookMgr)
			{
				g_this->m_keyboardHookMgr->HookingKeyboard(1);

				_Notnull_ MSG msg;
				PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
				
				if (m_keyboardHookMgr->GetbIsCapture() == true)
				{
					AfxMessageBox(L"\n Cancel screenshot!");
					m_keyboardHookMgr->UnhookingKeyboard();
				}

				FreeLibrary(m_hDll_keyboardHook);
			}
		}
	}
}

CString BlockingCapture::GetProcName(_In_ HWND hwnd)
{
	DWORD processId = 0;

	::GetWindowThreadProcessId(hwnd, &processId);

	HANDLE processHandle = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
	if (processHandle != NULL)
	{
		TCHAR processName[MAX_PATH];
		if (::GetModuleBaseName(processHandle, NULL, processName, sizeof(processName) / sizeof(TCHAR)) > 0)
		{
			::CloseHandle(processHandle);
			return CString(processName);
		}

		::CloseHandle(processHandle);
	}
	return CString("");
}

void BlockingCapture::UnhookingKeyboard()
{
	m_keyboardHookMgr->UnhookingKeyboard();
}


