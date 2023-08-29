#pragma once
#include <vector>
#include <atlstr.h>
#include <string>

class BlockingWebSite
{
public:
	BlockingWebSite() {}
	virtual ~BlockingWebSite() {}
	virtual void DestroyThis() = 0;
	virtual void StartBlockingWebSite() = 0;
	virtual void SetControlWebSiteNames(std::vector<std::string> value) = 0;
	virtual bool GetIsBlockedSite() = 0;
	virtual CString GetBlockedSiteName() = 0;
};

