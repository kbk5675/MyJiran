#include "pch.h"
#include "FileScannerManager.h"
#include "MFCTESTDlg.h"
#include "OfficeManager.h"
#include "ThreadManager.h"
#include <Windows.h>

#define GetKeeperMgr OfficeManager::GetOfficeMgr()
#define GetThreadMgr OfficeManager::GetOfficeMgr()->GetThreadMgr()

FileScanManager* g_this = nullptr;

FileScanManager::FileScanManager(): m_is_scan_over(false)
{
    g_this = this;
}

FileScanManager::~FileScanManager()
{
    FileScanner::~FileScanner();
}

bool FileScanManager::StartFullScan(std::wstring start_file_path)
{
    m_start_file_path = start_file_path;

    if (GetThreadMgr->GetThreadState("thread_search_infomation") != EThreadState::RUNNING)
    {
        GetThreadMgr->StartThread("thread_search_infomation", 10, ThreadFullScan);
    }

    return m_is_scan_over;
}

UINT FileScanManager::ThreadFullScan(LPVOID _mothod)
{
    while (GetThreadMgr->GetThreadState("thread_search_infomation") == EThreadState::RUNNING)
    {
        if (g_this->SearchFile(g_this->m_start_file_path) == true)
        {
            g_this->m_is_scan_over = true;
        }
        Sleep(10);
    }
    return 0;
}

bool FileScanManager::StartScan(std::string filePath, ESENSITIVEINFO senInfo)
{
    return false;
}

void FileScanManager::StopSearchInfo()
{
    if (GetThreadMgr->GetThreadState("thread_search_infomation") == EThreadState::RUNNING)
    {
        GetThreadMgr->StopThread("thread_search_infomation");
    }
}

void FileScanManager::StopScan()
{
}

void FileScanManager::Reset()
{
    FileScanner::Reset();
    m_is_scan_over = false;
}