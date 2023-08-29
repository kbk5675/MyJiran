// ControlWebSiteDlg.cpp: 구현 파일
#include "pch.h"
#include "afxdialogex.h"
#include "ControlWebSiteDlg.h"
#include "MFCTESTDlg.h"
#include "OfficeManager.h"
#include <algorithm>
#include <string>

using namespace std;
#define GetBlockingWebSiteMgr OfficeManager::GetOfficeMgr()->GetBlockingWebMgr()
#define GetCtrlWebSiteList OfficeManager::GetOfficeMgr()->GetBlockingWebMgr()->GetWebSiteList()

// ControlWebSiteDlg 대화 상자

IMPLEMENT_DYNAMIC(ControlWebSiteDlg, CDialogEx)

ControlWebSiteDlg::ControlWebSiteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CONTROL_WEBSITE, pParent)
{
}

ControlWebSiteDlg::~ControlWebSiteDlg() 
{
}

void ControlWebSiteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_LIST_CONTROL_WEBSITE, m_list_ctrl_website);
	DDX_Control(pDX, IDC_EDIT_SITE, m_edit_site);
}

BOOL ControlWebSiteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	this->SetWindowTextW(L"Control WebSite List");

	ctrl_webSite_list_temp = GetBlockingWebSiteMgr->GetWebSiteList();

	for (string it : GetCtrlWebSiteList)
	{
		CString cstr = (CString)it.c_str();
		m_list_ctrl_website.AddString(cstr);
	}

	m_edit_site.SetWindowTextW(_T("ex)instagram"));
	return 0;
}

void ControlWebSiteDlg::InitWebSiteListBox()
{
	m_list_ctrl_website.ResetContent();
	for (string it : GetBlockingWebSiteMgr->GetWebSiteList())
	{
		CString cstr = (CString)it.c_str();
		m_list_ctrl_website.AddString(cstr);
	}
}

BEGIN_MESSAGE_MAP(ControlWebSiteDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ADD_SITE, &ControlWebSiteDlg::OnBnClickedBtnAddSite)
	ON_BN_CLICKED(IDC_BTN_DEL_SITE, &ControlWebSiteDlg::OnBnClickedBtnDelSite)
	ON_BN_CLICKED(IDOK, &ControlWebSiteDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ControlWebSiteDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


void ControlWebSiteDlg::OnBnClickedBtnAddSite()
{
	CString strData = _T("");
	m_edit_site.GetWindowTextW(strData);

	if (!strData.IsEmpty())
	{
		m_edit_site.SetWindowTextW(_T(""));

		CT2CA convertedString(strData);
		std::string str = std::string(convertedString);
		GetCtrlWebSiteList.push_back(str);
	}

	GetCtrlWebSiteList.erase(
		unique(GetCtrlWebSiteList.begin(),
			GetCtrlWebSiteList.end()),
		GetCtrlWebSiteList.end());

	InitWebSiteListBox();
}

void ControlWebSiteDlg::OnBnClickedBtnDelSite()
{
	int nCurSel = m_list_ctrl_website.GetCurSel();
	CString CurDataName = _T("");

	if (nCurSel >= 0)
	{
		m_list_ctrl_website.GetText(nCurSel, CurDataName);

		USES_CONVERSION;
		std::string str = std::string(CT2CA(CurDataName));

		GetCtrlWebSiteList.erase(
			remove(GetCtrlWebSiteList.begin(),
				GetCtrlWebSiteList.end(), str),
			GetCtrlWebSiteList.end());

		InitWebSiteListBox();
	}
}

void ControlWebSiteDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void ControlWebSiteDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	GetCtrlWebSiteList = ctrl_webSite_list_temp;
}
