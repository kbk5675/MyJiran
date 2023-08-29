#pragma once
#include "FileScannerManager.h"
#include "ThreadManager.h"
#include "BlockingWebManager.h"
#include <memory>

class FileScanManager;
class ThreadManager;
class BlockingWebManager;

class OfficeManager
{
public:
	static std::unique_ptr<OfficeManager>& GetOfficeMgr();
	static void DestroyOfficeMgr();

	std::shared_ptr<ThreadManager>& GetThreadMgr()				{ return m_thread_manager; }
	std::shared_ptr<FileScanManager>& GetFileScanManager()		{ return m_fileScanner_manager; }
	std::shared_ptr<BlockingWebManager>& GetBlockingWebMgr()	{ return m_blocking_web_manager; }

public:
	OfficeManager();

private:
	static std::unique_ptr<OfficeManager>	m_office_manager;
	std::shared_ptr<BlockingWebManager>		m_blocking_web_manager;
	std::shared_ptr<FileScanManager>		m_fileScanner_manager;
	std::shared_ptr<ThreadManager>			m_thread_manager;
};