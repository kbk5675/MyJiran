#pragma once
#include "EThreadState.h"
#include <memory>
#include <vector>
#include <atlstr.h>
#include <string>

struct MYTHREADINFO
{
	CWinThread*		TI_thread;
	std::string		TI_threadName;
	int				TI_threadKey;
	EThreadState	TI_threadState;
	int				TI_threadSpeed;
	UINT(*TI_pFunction)(LPVOID);
};

class ThreadManager
{
public:
	ThreadManager();
	virtual ~ThreadManager();

	void		 StartThread		(std::string threadName, int threadSpeed, UINT(*pFunction)(LPVOID));
	void		 DestroyThread		(std::string threadName);
	void		 DestroyThreadAll	();

	EThreadState GetThreadState		(std::string threadName);
	EThreadState GetThreadState		(int threadKey);

	CWinThread*& GetThread(std::string threadName);
	CWinThread*& GetThread(int threadID);

	int			 GetThreadKey		(std::string threadName);
	int			 GetThreadSpeed		(std::string threadName);

	template<typename T>
	void StopThread(T threadValue);

private:
	void		 CreateMyThread		(std::string threadName, int threadSpeed, UINT(*pFunction)(LPVOID));
	void		 SetThreadState		(std::string threadName, EThreadState threadState);
	void		 SetThreadState		(int threadID, EThreadState threadState);
	
	bool		 FindThreadInfo		(std::string threadName);
	bool		 FindThreadInfo		(int threadID);

	template<typename T>
	void RestartThread (T threadValue);

private:
	std::vector<MYTHREADINFO>	m_threadInfoes;
	int	m_threadCount;
};

template<typename T>
inline void ThreadManager::RestartThread(T threadValue)
{
	CWinThread* winThread = GetThread(threadValue);
	winThread->ResumeThread();
	SetThreadState(threadValue, EThreadState::RUNNING);
}

template<typename T>
inline void ThreadManager::StopThread(T threadValue)
{
	if (!FindThreadInfo(threadValue))
	{
		return;
	}

	if (GetThread(threadValue) != NULL && GetThread(threadValue)->m_hThread != NULL)
	{
		SetThreadState(threadValue, EThreadState::PAUSE);
		GetThread(threadValue)->SuspendThread();
	}
}