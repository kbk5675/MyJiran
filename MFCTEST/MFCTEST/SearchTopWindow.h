#pragma once
#include <memory>

class SearchTopWindow
{
public:
	SearchTopWindow();
	virtual ~SearchTopWindow();
	CString SearchUsingProcName();

private:
	bool GetUsingWindow();
	bool GetProcID();
	bool GetProcName();

private:
	CWnd*	m_using_wnd;
	DWORD	m_dw_proc_id;
	CString	m_proc_name;
};