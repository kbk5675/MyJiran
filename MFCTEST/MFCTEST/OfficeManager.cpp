#include "pch.h"
#include "OfficeManager.h"


std::unique_ptr<OfficeManager> OfficeManager::m_office_manager = nullptr;

OfficeManager::OfficeManager() : m_fileScanner_manager(nullptr), m_thread_manager(nullptr)
{
	m_blocking_web_manager = std::make_shared<BlockingWebManager>();
	m_fileScanner_manager	= std::make_shared<FileScanManager>();
	m_thread_manager		= std::make_shared<ThreadManager>();
}

std::unique_ptr<OfficeManager>& OfficeManager::GetOfficeMgr()
{
	if (m_office_manager == NULL)
	{
		m_office_manager = std::make_unique<OfficeManager>();
	}

	return m_office_manager;
}

void OfficeManager::DestroyOfficeMgr()
{
	m_office_manager->m_blocking_web_manager= nullptr;
	m_office_manager->m_fileScanner_manager = nullptr;
	m_office_manager->m_thread_manager		= nullptr;
	

	if (m_office_manager)
	{
		m_office_manager = nullptr;
	}
}

