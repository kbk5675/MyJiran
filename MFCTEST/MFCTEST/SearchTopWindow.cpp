#include "pch.h"
#include "SearchTopWindow.h"
#include <Psapi.h>

SearchTopWindow::SearchTopWindow()
{
	m_using_wnd		= nullptr;
	m_dw_proc_id	= NULL;

	SearchUsingProcName();
}

SearchTopWindow::~SearchTopWindow()
{
	
}

CString SearchTopWindow::SearchUsingProcName()
{
	if (GetUsingWindow() == true)
	{
		if (GetProcID() == true)
		{
			if (GetProcName() == true)
			{
				return m_proc_name;
			}
		}
	}

	return _T("Can't Found");
}

bool SearchTopWindow::GetUsingWindow()
{
	m_using_wnd = CWnd::GetForegroundWindow();
	if (m_using_wnd != nullptr)
	{
		return true;
	}

	return false;
}

bool SearchTopWindow::GetProcID()
{
	GetWindowThreadProcessId(m_using_wnd->GetSafeHwnd(), &m_dw_proc_id);
	if (m_dw_proc_id != NULL)
	{
		return true;
	}

	return false;
}

bool SearchTopWindow::GetProcName()
{
	HANDLE h_proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, m_dw_proc_id);
	if (h_proc != NULL)
	{
		char szProcessName[MAX_PATH];
		if (GetModuleBaseNameA(h_proc, NULL, szProcessName, MAX_PATH) > 0)
		{
			CString proc_name(szProcessName);
			m_proc_name = proc_name;
			return true;
		}
		CloseHandle(h_proc);
	}
	return false;
}
