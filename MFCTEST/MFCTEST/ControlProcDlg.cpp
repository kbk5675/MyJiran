// ControlProcDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCTEST.h"
#include "afxdialogex.h"
#include "ControlProcDlg.h"
#include "MFCTESTDlg.h"
#include "BlockingCapture.h"

// ControlProcDlg 대화 상자

IMPLEMENT_DYNAMIC(CControlProcDlg, CDialogEx)



CControlProcDlg::CControlProcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CONTROL_PROC, pParent)
{
	mainDlg = (CMFCTESTDlg*)AfxGetApp()->m_pMainWnd;
}


CControlProcDlg::~CControlProcDlg()
{
}


void CControlProcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONTROL_PROC, m_control_proc_list);
	DDX_Control(pDX, IDC_EDIT_ADD_PROC, m_edit_proc);
}


BOOL CControlProcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	this->SetWindowTextW(L"Control Process List");
	
	CtrlProces_temp = mainDlg->m_blockingCaputre->Get_control_process_list();

	for (auto it : mainDlg->m_blockingCaputre->Get_control_process_list())
	{
		CString cstr = (CString)it;
		m_control_proc_list.AddString(cstr);
	}

	m_edit_proc.SetWindowTextW(_T("EX) chrome"));
	return TRUE;
}

BEGIN_MESSAGE_MAP(CControlProcDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD_PROC, &CControlProcDlg::OnBnClickedBtnAddProc)
	ON_BN_CLICKED(IDC_BTN_DEL_PROC, &CControlProcDlg::OnBnClickedBtnDelProc)
	ON_BN_CLICKED(IDOK, &CControlProcDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CControlProcDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

void CControlProcDlg::OnBnClickedBtnAddProc()
{
	// 입력창 초기화
	CString cstr = _T("");
	m_edit_proc.GetWindowTextW(cstr);

	if (!cstr.IsEmpty())
	{
		mainDlg->m_blockingCaputre->Get_control_process_list().push_back(cstr + L".exe");
		m_control_proc_list.AddString(cstr + L".exe");
		m_edit_proc.SetWindowTextW(_T(""));
	}
}

void CControlProcDlg::OnBnClickedBtnDelProc()
{
	int nCurSel = m_control_proc_list.GetCurSel();
	CString CurDataName = _T("");

	if (nCurSel >= 0)
	{
		m_control_proc_list.GetText(nCurSel, CurDataName);
		USES_CONVERSION;
		

		// 제어 프로세스 리스트에서 삭제
		mainDlg->m_blockingCaputre->Get_control_process_list().erase(
			remove(mainDlg->m_blockingCaputre->Get_control_process_list().begin(),
			mainDlg->m_blockingCaputre->Get_control_process_list().end(), CurDataName),
			mainDlg->m_blockingCaputre->Get_control_process_list().end());

		m_control_proc_list.DeleteString(nCurSel);
	}
}



void CControlProcDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CControlProcDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	mainDlg->m_blockingCaputre->Get_control_process_list() = CtrlProces_temp;
}
