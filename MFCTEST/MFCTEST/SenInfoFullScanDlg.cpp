#include "pch.h"
#include "afxdialogex.h"
#include "SenInfoFullScanDlg.h"
#include "MFCTESTDlg.h"
#include "FileScannerManager.h"
#include "OfficeManager.h"

#include <fileapi.h>
#include "EThreadState.h"

#define GetKeeperMgr	OfficeManager::GetOfficeMgr()
#define GetFileScanMgr	OfficeManager::GetOfficeMgr()->GetFileScanManager()
#define GetThreadMgr	OfficeManager::GetOfficeMgr()->GetThreadMgr()

SenInfoFullScanDlg* g_thisDlg = nullptr;

IMPLEMENT_DYNAMIC(SenInfoFullScanDlg, CDialogEx)

SenInfoFullScanDlg::SenInfoFullScanDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FULL_SCAN, pParent)
{
	m_mainDlg	= (CMFCTESTDlg*)AfxGetApp()->m_pMainWnd;
	m_time		= 0.f;
	g_thisDlg	= this;
}

SenInfoFullScanDlg::~SenInfoFullScanDlg()
{


	m_mainDlg	= nullptr;
	m_time		= 0;
}

void SenInfoFullScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FULLSCAN_TIMER, m_static_timer);
	DDX_Control(pDX, IDC_STATIC_FULLSCAN_RESULT_COUNT, m_static_fullScan_result_count);
	DDX_Control(pDX, IDC_STATIC_FULLSCAN_FILE_COUNT, m_static_fullScan_file_count);
	DDX_Control(pDX, IDC_STATIC_FULLSCAN_EMAIL_COUNT, m_static_fullScan_email_count);
	DDX_Control(pDX, IDC_STATIC_FULLSCAN_RRN_COUNT, m_static_fullScan_RRN_count);
	DDX_Control(pDX, IDC_STATIC_FULLSCAN_PHONE_COUNT, m_static_fullScan_phone_count);
	DDX_Control(pDX, IDOK, m_btn_ok);
}

BEGIN_MESSAGE_MAP(SenInfoFullScanDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

BOOL SenInfoFullScanDlg::OnInitDialog()
{
	if (!CDialogEx::OnInitDialog())
		return FALSE;

	m_hEvent = CreateEvent(NULL, FALSE, FALSE, L"terminate_sensitive_scan");

	this->SetWindowTextW(L"검사중");
	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	return TRUE;
}

void SenInfoFullScanDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow == TRUE && nStatus == 0)
	{
		if (GetThreadMgr->GetThreadState("thread_FullScan") != EThreadState::RUNNING)
		{
			GetThreadMgr->StartThread("thread_FullScan", 10, ThreadFullScan);
		}
	}
}

UINT SenInfoFullScanDlg::ThreadFullScan(LPVOID _mothod)
{
	while (GetThreadMgr->GetThreadState("thread_FullScan") == EThreadState::RUNNING)
	{
		g_thisDlg->FullScan();
		Sleep(10);
	}

	return 0;
}

UINT SenInfoFullScanDlg::ThreadScanTimer(LPVOID _mothod)
{
	int speed = GetThreadMgr->GetThreadSpeed("thread_scanTimer");
	auto startTime = std::chrono::high_resolution_clock::now();

	while (GetThreadMgr->GetThreadState("thread_scanTimer") == EThreadState::RUNNING)
	{
		auto endTime = std::chrono::high_resolution_clock::now();
		
		int currentTime = g_thisDlg->CalcScanTime(startTime, endTime);

		CString currentTime_cstr;
		currentTime_cstr.Format(_T("%d.%d"), currentTime / 1000, (currentTime % 1000) / 10);
		g_thisDlg->m_static_timer.SetWindowTextW(currentTime_cstr);
		Sleep(speed);
	}

	return 0;
}

void SenInfoFullScanDlg::FullScan()
{
	GetAllDrivers();

	if (GetThreadMgr->GetThreadState("thread_scanTimer") != EThreadState::RUNNING)
	{
		GetThreadMgr->StartThread("thread_scanTimer", 1, ThreadScanTimer);
	}

	for (auto driver : m_array_of_drivers)
	{
		std::wstring wstr;
		wstr.assign(driver.begin(), driver.end());

		// 파일 스캔 시작
		while (GetFileScanMgr->StartFullScan(wstr.c_str()) != true)
		{
			InitDisplay(); 
		
		}
		
		this->SetWindowTextW(L"검사완료");

		GetFileScanMgr->Reset();
		GetFileScanMgr->StopSearchInfo();
		GetThreadMgr->DestroyThread("thread_scanTimer");
	}

	SetEvent(m_hEvent);
	GetThreadMgr->StopThread("thread_FullScan");
}

void SenInfoFullScanDlg::GetAllDrivers()
{
	int n = 0;
	
	char* szDrives = new char[MAX_PATH]();
	if (GetLogicalDriveStringsA(MAX_PATH, szDrives))
	{
		while (szDrives[n] != NULL)
		{
			for (int i = 0; i < sizeof(*(szDrives + n)); i++)
			{
				std::string temp = szDrives + n;
				m_array_of_drivers.push_back(temp);
				n += 4;
			}
		}
	}
}

void SenInfoFullScanDlg::InitDisplay()
{
	SetData(GetFileScanMgr->GetFileCount(), g_thisDlg->m_static_fullScan_file_count);
	SetData(GetFileScanMgr->GetResultCount(), g_thisDlg->m_static_fullScan_result_count);
	SetData(GetFileScanMgr->GetEmailCount(), g_thisDlg->m_static_fullScan_email_count);
	SetData(GetFileScanMgr->GetRRNCount(), g_thisDlg->m_static_fullScan_RRN_count);
	SetData(GetFileScanMgr->GetPhoneNumCount(), g_thisDlg->m_static_fullScan_phone_count);
}

void SenInfoFullScanDlg::SetData(int count, CStatic& static_value)
{
	if (static_value)
	{
		CString cstr;
		cstr.Format(_T("%d"), count);
		static_value.SetWindowText(cstr);
		cstr.Empty();

	}	
}

int SenInfoFullScanDlg::CalcScanTime(std::chrono::time_point<std::chrono::steady_clock> startTime,
	std::chrono::time_point<std::chrono::steady_clock> endTime)
{
	std::chrono::milliseconds currentSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
	int currentTime = static_cast<int>(currentSeconds.count());

	return currentTime;
}

void SenInfoFullScanDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (GetThreadMgr->GetThreadState("thread_search_infomation") == EThreadState::RUNNING)
	{
		if (nID != SC_CLOSE)
		{
			CDialogEx::OnSysCommand(nID, lParam);
		}
	}
	else
	{
		if (nID == SC_CLOSE)
		{
			CDialogEx::OnSysCommand(nID, lParam);
		}
	}
}
