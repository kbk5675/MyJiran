#pragma once
#include "pch.h"
#include "BlockingWebSiteManager.h"
#include <regex>
#include <Windows.h>
#include <winuser.h>
#include <UIAutomation.h>


BlockingWebSiteManager* g_BWSMgr = nullptr;

BlockingWebSiteManager::BlockingWebSiteManager(): m_IsBlockedSite(false)
{
	g_BWSMgr = this;
}

#pragma region Search Browser Window Functions
BOOL CALLBACK BlockingWebSiteManager::EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
	g_BWSMgr->SearchWebWindows(hwnd, lParam);
	return true;
}

void BlockingWebSiteManager::SearchWebWindows(_In_ HWND hwnd, _In_ LPARAM lParam)
{
	char wndTitle[255] = { 0, };

	GetClassNameA(hwnd, wndTitle, sizeof(wndTitle));

	if (strcmp(wndTitle, "Chrome_WidgetWin_1") == 0 || strcmp(wndTitle, "MozillaWindowClass") == 0)
	{
		if (IsWindowVisible(hwnd) && GetWindowTextLength(hwnd) > 0)
		{
			m_current_h_wnds.push_back(hwnd);
		}
	}

	memset(wndTitle, 0, sizeof(wndTitle));
	return;
}
#pragma endregion

#pragma region UIAutomation Functions
void BlockingWebSiteManager::StartBlockingWebSite()
{
	if (m_ctrl_web_list_cstring.empty())
	{
		return;
	}

	EnumWindows(EnumWindowsProc, NULL);

	if (m_current_h_wnds.empty())
	{
		return;
	}

	for (auto HwndIterator : m_current_h_wnds)
	{
		CoInitialize(NULL);
		CComPtr<IUIAutomation> uia;
		if SUCCEEDED(uia.CoCreateInstance(CLSID_CUIAutomation))
		{
			CComPtr<IUIAutomationElement> root;

			if SUCCEEDED(uia->ElementFromHandle(HwndIterator, &root))
			{
				if (CheckCtrlWebsite(uia, root) == true)
				{
					m_IsBlockedSite = true;
					break;
				}
				else
				{
					m_IsBlockedSite = false;
				}
			}
			uia.Release();
		}
		CoUninitialize();
	}
	m_current_h_wnds.clear();
}

void BlockingWebSiteManager::SetControlWebSiteNames(std::vector<std::string> ctrl_web_list)
{
	if (ctrl_web_list.empty())
	{
		return;
	}

	if (m_ctrl_web_list_cstring.empty())
	{
		m_ctrl_web_list_cstring.clear();
	}

	m_ctrl_web_list = ctrl_web_list;

	for (auto it : m_ctrl_web_list)
	{
		m_ctrl_web_list_cstring.push_back(it.c_str());
	}
}

CString BlockingWebSiteManager::GetBlockedSiteName()
{
	if (m_blocked_site_name.IsEmpty())
	{
		return _T("");
	}
	
	return m_blocked_site_name; 
}

void BlockingWebSiteManager::DestroyThis()
{
	delete(this);
}

bool BlockingWebSiteManager::CheckCtrlWebsite(IUIAutomation* uia, IUIAutomationElement* root)
{
	CComPtr<IUIAutomationCondition>		condition_find_editControl_typeID;
	CComPtr<IUIAutomationElementArray>	elements_editCtrlTypeId;

	uia->CreatePropertyCondition(UIA_ControlTypePropertyId, CComVariant(UIA_EditControlTypeId), &condition_find_editControl_typeID);
	if FAILED(root->FindAll(TreeScope_Descendants, condition_find_editControl_typeID, &elements_editCtrlTypeId))
	{
		return false;
	}

	CComPtr<IUIAutomationElement> search_bar = FindElementSerchBar(elements_editCtrlTypeId);
	if (search_bar == NULL)
	{
		return false;
	}
	
	// 데이터 유무 예외처리
	CComVariant variant_url;
	if FAILED(search_bar->GetCurrentPropertyValue(UIA_LegacyIAccessibleValuePropertyId, &variant_url))
	{
		return false;
	}

	//  Strng 유무 예외처리
	if (!variant_url.bstrVal)
	{
		return false;
	}

	CString str_url;
	str_url = (LPCTSTR)(variant_url.bstrVal);

	if (CheckIsControlWebsite(str_url, search_bar))
	{
		str_url.Empty();
		return true;
	}

	str_url.Empty();
	return false;
}

CComPtr<IUIAutomationElement> BlockingWebSiteManager::FindElementSerchBar(CComPtr<IUIAutomationElementArray> elements)
{
	CComPtr<IUIAutomationElement> element_editControlTypeId;
	CComPtr<IUIAutomationElement> element_searchBar;
	int elementsSize = NULL;
	elements->get_Length(&elementsSize);

	for (int i = 0; i < elementsSize; i++)
	{
		CComBSTR name;
		if SUCCEEDED(elements->GetElement(i, &element_editControlTypeId))
		{
			if SUCCEEDED(element_editControlTypeId->get_CurrentName(&name))
			{
				if (wcscmp(name, L"주소창 및 검색창") == 0 || wcscmp(name, L"Google 검색 또는 주소 입력") == 0)
				{
					element_searchBar = element_editControlTypeId;
					break;
				}
			}
		}
		element_editControlTypeId.Release();
	}

	return element_searchBar;
}

bool BlockingWebSiteManager::CheckIsControlWebsite(CString url, CComPtr<IUIAutomationElement> element_searchBar)
{
	CString cstr_url;
	std::string str_match = "";
	std::string str_url = std::string(CT2CA(url));
	
	std::regex regex_url("(https://)?([0-9a-zA-Z]*[.])?[0-9a-zA-Z]*[.][a-zA-Z]{2,3}");

	auto wordsBegin = std::sregex_iterator(str_url.begin(), str_url.end(), regex_url);
	auto wordsEnd = std::sregex_iterator();

	if (std::distance(wordsBegin, wordsEnd))
	{
		for (std::sregex_iterator i = wordsBegin; i != wordsEnd; ++i)
		{
			std::smatch match = *i;
			str_match = match.str();
			cstr_url = str_match.c_str();
			break;
		}
	}

	// 새로운 주소값 삽입
	for (CString it_str : m_ctrl_web_list_cstring)
	{
		if (cstr_url.Find(it_str) != -1)
		{
			InputNewUrl(element_searchBar);

			int cnt = 2;
			while (cnt--)
			{
				element_searchBar->SetFocus();
				AutoEnter();
			}

			m_blocked_site_name = url;

			cstr_url.Empty();
			return true;
		}
	}

	cstr_url.Empty();
	return false;
}

bool BlockingWebSiteManager::InputNewUrl(CComPtr<IUIAutomationElement> element_searchBar)
{
	IValueProvider* pattern = nullptr;
	if (FAILED(element_searchBar->GetCurrentPattern(UIA_ValuePatternId, (IUnknown**)&pattern)))
	{
		return false;
	}

	pattern->SetValue(L"https://gw.jiran.com/login");
	pattern->Release();
}

void BlockingWebSiteManager::AutoEnter()
{
	INPUT input[2] = { INPUT_KEYBOARD };
	input[0].ki.wVk = VK_RETURN;
	input[1] = input[0];
	input[1].ki.dwFlags |= KEYEVENTF_KEYUP;

	SendInput(2, input, sizeof(INPUT));
}


#pragma endregion

BlockingWebSiteManager* CreateBlockingWebSiteManager()
{
	return new BlockingWebSiteManager();
}

