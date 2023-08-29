#include "pch.h"
#include "BlockingWebManager.h"
#include "BlockingWebSite.h"
#include "OfficeManager.h"
#include "MessageDlg.h"
#include "resource.h"

#define GetKeeperMgr OfficeManager::GetOfficeMgr()
#define GetThreadMgr OfficeManager::GetOfficeMgr()->GetThreadMgr()

typedef BlockingWebSite* (*CreateBlockingWebManager)();

BlockingWebManager* g_this= nullptr;

BlockingWebManager::BlockingWebManager()
{
	g_this = this;
	m_blockingWebSite = nullptr;
	m_hDll_BlockingWebSite = nullptr;

	m_ctrlWebsites.push_back("naver");
	m_ctrlWebsites.push_back("youtube");
	m_ctrlWebsites.push_back("instagram");
}

BlockingWebManager::~BlockingWebManager()
{
	if (m_blockingWebSite != nullptr)
	{
		m_blockingWebSite = nullptr;
	}

	m_ctrlWebsites.clear();
}

void BlockingWebManager::StartBlockingWebSiteDll()
{
	if (LoadBlockingWebDll())
	{
		m_blockingWebSite->SetControlWebSiteNames(m_ctrlWebsites);
		m_blockingWebSite->StartBlockingWebSite();

		if (m_blockingWebSite->GetIsBlockedSite() == true)
		{
			m_blocking_site_name = m_blockingWebSite->GetBlockedSiteName();
			GetThreadMgr->StartThread("thread_web_message", 500, ThreadMessage);
		}

		delete(m_blockingWebSite);
		StopBlockingWebStieDll(); // FreeLibrary
	}
}

void BlockingWebManager::StopBlockingWebStieDll()
{
	if (m_hDll_BlockingWebSite != NULL)
	{
		FreeLibrary(m_hDll_BlockingWebSite);
		OutputDebugString(L"Free Library : m_hDll_BlockingWebSite");
	}
}

UINT BlockingWebManager::ThreadMessage(LPVOID _mothod)
{
	int thread_speed = GetThreadMgr->GetThreadSpeed("thread_web_message");

	while (GetThreadMgr->GetThreadState("thread_web_message") == EThreadState::RUNNING)
	{
		AfxMessageBox( g_this->m_blocking_site_name + L"\n해당 사이트는 차단된 사이트입니다.");
		GetThreadMgr->StopThread("thread_web_message");
		Sleep(thread_speed);
	}

	return 0;
}

bool BlockingWebManager::LoadBlockingWebDll()
{
	m_hDll_BlockingWebSite = LoadLibrary(L"Dll_BlockingWebSite.dll");
	if (m_hDll_BlockingWebSite)
	{
		CreateBlockingWebManager lpCreateBlockingWebMgr;
		lpCreateBlockingWebMgr = (CreateBlockingWebManager)GetProcAddress(m_hDll_BlockingWebSite, "CreateBlockingWebSiteManager");
		if (lpCreateBlockingWebMgr)
		{
			m_blockingWebSite = lpCreateBlockingWebMgr();
			if (m_blockingWebSite)
			{
				return true;
			}
		}
	}
	return false;
}
