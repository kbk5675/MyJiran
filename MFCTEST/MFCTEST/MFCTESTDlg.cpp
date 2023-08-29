#include "pch.h"
#include "framework.h"
#include "MFCTEST.h"
#include "MFCTESTDlg.h"
#include "afxdialogex.h"
#include "OfficeManager.h"
#include "BlockingCapture.h"
#include "SearchTopWindow.h"

#include "FileSearchDlg.h"
#include "ControlProcDlg.h"
#include "ControlWebSiteDlg.h"
#include "SelectionScanMethodDlg.h"

#include <Windows.h>
#include <WinUser.h>
#include <AclAPI.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GetKeeperMgr OfficeManager::GetOfficeMgr()
#define GetThreadMgr OfficeManager::GetOfficeMgr()->GetThreadMgr()

typedef std::vector<CString>	(*GetProcesses)(std::vector<CString> ProcList);

CMFCTESTDlg* g_mainDlg = nullptr;

CMFCTESTDlg::CMFCTESTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MAIN, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDD_DIALOG_MAIN);

	m_blockingCaputre = nullptr;
	m_searchTopWindow = nullptr;
	m_hDll_ShowRunningProc	= NULL;
	g_mainDlg = this;
}
CMFCTESTDlg::~CMFCTESTDlg()
{
	m_searchTopWindow = nullptr;
	m_blockingCaputre = nullptr;

	m_hDll_ShowRunningProc = nullptr;
	GetThreadMgr->DestroyThreadAll();
	GetKeeperMgr->DestroyOfficeMgr();
}

void CMFCTESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_PROCESS, m_process_list);
	DDX_Control(pDX, IDC_CHECK_BLOCK_CAPTURE, m_check_block_capture);
	DDX_Control(pDX, IDC_STATIC_PROCESS, m_foreground_process_name);
	DDX_Control(pDX, IDC_CHECK_BLOCKING_WEBSITE, m_check_block_webSite);
	DDX_Control(pDX, IDC_CHECK_MONITORING_TOPWND, m_check_monitoring_topWnd);
}

BOOL CMFCTESTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	this->SetWindowTextW(L"Office Keeper Bogeun");
	ShowRunningProcess();
	return TRUE;
}

void CMFCTESTDlg::OnPaint()
{
	CPaintDC dc(this);
	CDC MemDC;
	CBitmap* pOldBmp;

	MemDC.CreateCompatibleDC(&dc);
	pOldBmp = MemDC.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, 500, 200, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBmp);
	MemDC.DeleteDC();
}

BEGIN_MESSAGE_MAP(CMFCTESTDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_BLOCK_CAPTURE, &CMFCTESTDlg::OnBnClickedCheckBlockCapture)
	ON_LBN_DBLCLK(IDC_LIST_PROCESS, &CMFCTESTDlg::OnLbnDblclkListProcess)
	ON_BN_CLICKED(IDC_BTN_SHOW_CONTROL_PROC_LIST, &CMFCTESTDlg::OnBnClickedBtnShowcontrolproclist)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BTN_SHOW_CONTROL_WEBSITE, &CMFCTESTDlg::OnBnClickedBtnShowControlWebsite)
	ON_BN_CLICKED(IDC_CHECK_BLOCKING_WEBSITE, &CMFCTESTDlg::OnBnClickedCheckBlockingWebsite)
	ON_BN_CLICKED(IDC_BTN_REFRESH, &CMFCTESTDlg::OnBnClickedBtnRefresh)
	ON_BN_CLICKED(IDC_CHECK_MONITORING_TOPWND, &CMFCTESTDlg::OnBnClickedCheckMonitoringTopwnd)
	ON_BN_CLICKED(IDC_BTN_SENSITIVEINFO_SCAN, &CMFCTESTDlg::OnBnClickedBtnSensitiveinfoScan)
END_MESSAGE_MAP()


/*--------------------------------------------------------------- Function ---------------------------------------------------------------*/

void CMFCTESTDlg::ShowRunningProcess()
{
	std::vector<CString> ProcList;    

	g_mainDlg->m_hDll_ShowRunningProc = LoadLibrary(L"DLL_ShowRunningProc.dll");
	if (g_mainDlg->m_hDll_ShowRunningProc)
	{
		GetProcesses IpGetprocesses;
		IpGetprocesses = (GetProcesses)GetProcAddress(g_mainDlg->m_hDll_ShowRunningProc, "GetProcesses");
		if (!IpGetprocesses)
		{
			AfxMessageBox(L"Dll_ShowRunningProc 함수 오류");
			return;
		}

		ProcList = IpGetprocesses(ProcList);

		g_mainDlg->m_process_list.ResetContent();
		for (auto it : ProcList)
		{
			g_mainDlg->m_process_list.InsertString(-1, it);
		}

		FreeLibrary(g_mainDlg->m_hDll_ShowRunningProc);
	}
}

void CMFCTESTDlg::MakeMemBitmap()
{
	CClientDC dc(this); 
	CBitmap* pOldBmp; 
	CDC MemDC;

	m_bitmap.DeleteObject(); 
	m_bitmap.CreateCompatibleBitmap(&dc, 500, 200); 
	MemDC.CreateCompatibleDC(&dc); 
	pOldBmp = MemDC.SelectObject(&m_bitmap); 

	MemDC.MoveTo(0, 0);
	MemDC.LineTo(500, 200); 
	MemDC.SelectObject(pOldBmp); 
	MemDC.DeleteDC();
}

void CMFCTESTDlg::MonitoringTopWindow()
{
	if (m_searchTopWindow != nullptr)
	{
		CString proc_name;
		proc_name = m_searchTopWindow->SearchUsingProcName();
		if (proc_name.IsEmpty() != true)
		{
			g_mainDlg->m_foreground_process_name.SetWindowText(proc_name);
		}
	}
}

HCURSOR CMFCTESTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*--------------------------------------------------------------- Thread Function ---------------------------------------------------------------*/


UINT CMFCTESTDlg::ThreadMonitoringTopWindow(LPVOID _mothod)
{
	int thread_speed = GetThreadMgr->GetThreadSpeed("thread_monitoring");

	while (GetThreadMgr->GetThreadState("thread_monitoring") == EThreadState::RUNNING)
	{
		g_mainDlg->MonitoringTopWindow();
		Sleep(thread_speed);
	}

	return 0;
}

UINT CMFCTESTDlg::ThreadBlockingWebSite(LPVOID _mothod)
{
	int thread_speed = GetThreadMgr->GetThreadSpeed("thread_blockingWebsite");

	while (GetThreadMgr->GetThreadState("thread_blockingWebsite") == EThreadState::RUNNING)
	{
		GetKeeperMgr->GetBlockingWebMgr()->StartBlockingWebSiteDll();
		Sleep(thread_speed);
	}

	return 0;
}


/*--------------------------------------------------------------- Event Function ---------------------------------------------------------------*/


void CMFCTESTDlg::OnBnClickedCheckMonitoringTopwnd()
{
	if (m_check_monitoring_topWnd.GetCheck() == BST_CHECKED)
	{
		if (m_searchTopWindow == nullptr)
		{
			m_searchTopWindow = std::make_shared<SearchTopWindow>();
		}

		GetThreadMgr->StartThread("thread_monitoring", 100, this->ThreadMonitoringTopWindow);
	}
	else if (m_check_monitoring_topWnd.GetCheck() == BST_UNCHECKED)
	{
		
		GetThreadMgr->StopThread("thread_monitoring");
		m_foreground_process_name.SetWindowTextW(L"감지되지 않음");
	}
}

void CMFCTESTDlg::OnBnClickedCheckBlockCapture()
{
	if (m_blockingCaputre == nullptr)
	{
		m_blockingCaputre = std::make_shared<BlockingCapture>();
	}

	if (m_check_block_capture.GetCheck() == BST_CHECKED)
	{
		if (m_blockingCaputre)
		{
			m_blockingCaputre->Start_blocking_capture();
		}
	}
	else if (m_check_block_capture.GetCheck() == BST_UNCHECKED)
	{
		if (m_blockingCaputre)
		{
			m_blockingCaputre->Stop_blocking_capture();
		}
	}
}

void CMFCTESTDlg::OnBnClickedCheckBlockingWebsite()
{
	if (m_check_block_webSite.GetCheck() == BST_CHECKED)
	{
		GetThreadMgr->StartThread("thread_blockingWebsite", 100, ThreadBlockingWebSite);
	}
	else if (m_check_block_webSite.GetCheck() == BST_UNCHECKED)
	{
		GetThreadMgr->StopThread("thread_blockingWebsite");
		if (GetThreadMgr->GetThreadState("thread_web_message") == EThreadState::RUNNING)
		{
			GetThreadMgr->DestroyThread("thread_web_message");
		}
	}
}

void CMFCTESTDlg::OnLbnDblclkListProcess()
{
	int nCurSel;

	nCurSel = m_process_list.GetCurSel();
	CString CurDataName = _T("");
	m_process_list.GetText(nCurSel, CurDataName);

	AfxMessageBox(CurDataName);
}

void CMFCTESTDlg::OnBnClickedBtnShowcontrolproclist()
{
	if (m_blockingCaputre != nullptr)
	{
		CControlProcDlg dlg;
		dlg.DoModal();
	}
}

void CMFCTESTDlg::OnBnClickedBtnShowControlWebsite()
{
	ControlWebSiteDlg dlg;
	dlg.DoModal();
}

void CMFCTESTDlg::OnBnClickedBtnRefresh()
{
	ShowRunningProcess();
}

void CMFCTESTDlg::OnBnClickedBtnSensitiveinfoScan()
{
	SelectionScanMethodDlg dlg;
	dlg.DoModal();
}


	
