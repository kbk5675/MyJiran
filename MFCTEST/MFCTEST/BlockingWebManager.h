#pragma once
#include <vector>
#include <string>

class BlockingWebSite;

class BlockingWebManager
{
public:
	BlockingWebManager();
	virtual ~BlockingWebManager();

	void StartBlockingWebSiteDll();
	void StopBlockingWebStieDll();
	
	std::vector<std::string>& GetWebSiteList() { return m_ctrlWebsites; }

private:
	static UINT		ThreadMessage(LPVOID _mothod);
	bool			LoadBlockingWebDll();

private:
	BlockingWebSite*		 m_blockingWebSite;
	HMODULE					 m_hDll_BlockingWebSite;
	std::vector<std::string> m_ctrlWebsites;
	CString					 m_blocking_site_name;
};