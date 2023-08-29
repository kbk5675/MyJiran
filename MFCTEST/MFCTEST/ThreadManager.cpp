#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{
	m_threadCount = 0;
}

ThreadManager::~ThreadManager()
{
	m_threadInfoes.resize(0);
}

void ThreadManager::CreateMyThread(std::string threadName, int threadSpeed, UINT(*pFunction)(LPVOID))
{
	MYTHREADINFO threadInfo;

	threadInfo.TI_thread	  = nullptr;
	threadInfo.TI_threadName  = threadName;
	threadInfo.TI_threadKey	  = m_threadCount;
	threadInfo.TI_threadSpeed = threadSpeed;
	threadInfo.TI_threadState = EThreadState::RUNNING;
	threadInfo.TI_pFunction	  = pFunction;
	m_threadInfoes.push_back(threadInfo);
}

void ThreadManager::StartThread(std::string threadName, int threadSpeed, UINT(*pFunction)(LPVOID))
{
	if (FindThreadInfo(threadName))
	{
		RestartThread(threadName);
		return;
	}

	CreateMyThread(threadName, threadSpeed, pFunction);
 	m_threadInfoes[m_threadCount].TI_thread = AfxBeginThread(pFunction, this);
	m_threadInfoes[m_threadCount].TI_thread->m_bAutoDelete = FALSE;
	m_threadCount++;
	return;
}

void ThreadManager::DestroyThread(std::string threadName)
{
	if (!FindThreadInfo(threadName))
	{
		return;
	}
	
	for (MYTHREADINFO it : m_threadInfoes)
	{
		for (int i = 0; i < m_threadInfoes.size(); i++)
		{
			if (m_threadInfoes[i].TI_threadName.compare(threadName) == 0)
			{
				m_threadInfoes[i].TI_thread->SuspendThread();
				m_threadInfoes[i].TI_thread->ExitInstance();
				delete(m_threadInfoes[i].TI_thread);
				m_threadInfoes[i].TI_thread = nullptr;
				m_threadInfoes.erase(m_threadInfoes.begin() + i);
				m_threadCount--;
				return;
			}
		}
	}
}

void ThreadManager::DestroyThreadAll()
{
	if (m_threadInfoes.empty() == true)
	{
		return;
	}

	for (auto it : m_threadInfoes)
	{
		delete(it.TI_thread);
		m_threadCount--;
	}

	m_threadInfoes.clear();
	return;
}

EThreadState ThreadManager::GetThreadState(std::string threadName) 
{
	if (!FindThreadInfo(threadName))
	{
		return EThreadState::NONE;
	}

	for (MYTHREADINFO it : m_threadInfoes)
	{
		if (it.TI_threadName.compare(threadName) == 0)
		{
			return it.TI_threadState;
		}
	}

	return EThreadState::NONE;
}
EThreadState ThreadManager::GetThreadState(int threadID)
{
	if (!FindThreadInfo(threadID))
	{
		return EThreadState::NONE;
	}

	for (MYTHREADINFO it : m_threadInfoes)
	{
		if (it.TI_threadKey == threadID)
		{
			return it.TI_threadState;
		}
	}

	return EThreadState::NONE;
}

int ThreadManager::GetThreadKey(std::string threadName)
{
	int threadKey = 0;

	if (!FindThreadInfo(threadName))
	{
		return threadKey;
	}

	for (auto it : m_threadInfoes)
	{
		if (it.TI_threadName.compare(threadName) == 0)
		{
			threadKey = it.TI_threadKey;
			return threadKey;
		}
	}

	return threadKey;
}

int ThreadManager::GetThreadSpeed(std::string threadName)
{
	if (!FindThreadInfo(threadName))
	{
		return 0;
	}

	for (auto it : m_threadInfoes)
	{
		if (it.TI_threadName.compare(threadName) == 0)
		{
			return it.TI_threadSpeed;
		}
	}

	return 0;
}

bool ThreadManager::FindThreadInfo(std::string threadName)
{
	if (m_threadInfoes.empty())
	{
		return false;
	}

	for (auto it : m_threadInfoes)
	{
		if (it.TI_threadName.compare(threadName) == 0)
		{
			return true;
		}
	}
	return false;
}

bool ThreadManager::FindThreadInfo(int threadID)
{
	if (m_threadInfoes.empty())
	{
		return false;
	}

	for (MYTHREADINFO it : m_threadInfoes)
	{
		if (it.TI_threadKey == threadID)
		{
			return true;
		}
	}
	return false;
}

CWinThread*& ThreadManager::GetThread(std::string threadName)
{
	CWinThread* nullThread = nullptr;

	for (MYTHREADINFO it : m_threadInfoes)
	{
		if (it.TI_threadName.compare(threadName) == 0)
		{
			return it.TI_thread;
		}
	}
	return nullThread;
}
CWinThread*& ThreadManager::GetThread(int threadID)
{
	CWinThread* nullThread = nullptr;

	for (MYTHREADINFO it : m_threadInfoes)
	{
		if (it.TI_threadKey == threadID)
		{
			return it.TI_thread;
		}
	}

	return nullThread;
}

void ThreadManager::SetThreadState(std::string threadName, EThreadState threadState)
{
	if (!FindThreadInfo(threadName))
	{
		return;
	}

	for (MYTHREADINFO& it : m_threadInfoes)
	{
		if (it.TI_threadName.compare(threadName) == 0)
		{
			it.TI_threadState = threadState;
			return;
		}
	}
}
void ThreadManager::SetThreadState(int threadID, EThreadState threadState)
{
	if (!FindThreadInfo(threadID))
	{
		return;
	}

	for (MYTHREADINFO& it : m_threadInfoes)
	{
		if (it.TI_threadKey == threadID)
		{
			it.TI_threadState = threadState;
			return;
		}
	}
}