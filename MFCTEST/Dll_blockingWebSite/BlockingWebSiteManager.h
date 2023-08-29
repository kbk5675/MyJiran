#pragma once
#include "BlockingWebSite.h"
#include <string>
#include <thread>

class IUIAutomation;
class IUIAutomationElement;
class IUIAutomationElementArray;

class BlockingWebSiteManager : public BlockingWebSite
{
public:
	BlockingWebSiteManager();
	~BlockingWebSiteManager() 
	{
		m_ctrl_web_list.clear();
		m_ctrl_web_list_cstring.clear();
		m_current_h_wnds.clear();
	}

	void DestroyThis();
	void StartBlockingWebSite();
	void SetControlWebSiteNames(std::vector<std::string> value);

	bool GetIsBlockedSite() { return m_IsBlockedSite; }
	CString GetBlockedSiteName();
	
private:
	static BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam);
	void	SearchWebWindows(_In_ HWND hwnd, _In_ LPARAM lParam);
	CComPtr<IUIAutomationElement> FindElementSerchBar(CComPtr<IUIAutomationElementArray> elements);
	bool	CheckCtrlWebsite(IUIAutomation* uia, IUIAutomationElement* root);
	bool	CheckIsControlWebsite(CString url, CComPtr<IUIAutomationElement> element_searchBar);
	bool	InputNewUrl(CComPtr<IUIAutomationElement> element_searchBar);
	void	AutoEnter();
	
public:
	
private:
	std::vector<std::string>	m_ctrl_web_list;
	std::vector<CString>		m_ctrl_web_list_cstring;
	std::vector<HWND>			m_current_h_wnds;
	bool						m_IsBlockedSite;
	CString						m_blocked_site_name;
};

extern "C" __declspec(dllexport) BlockingWebSiteManager * CreateBlockingWebSiteManager();