#include "pch.h"
#include "MFCTEST.h"
#include "afxdialogex.h"
#include "SelectionScanMethodDlg.h"
#include "SenInfoFullScanDlg.h"
#include "SenInfoSelectScanDlg.h"
#include "FileSearchDlg.h"


IMPLEMENT_DYNAMIC(SelectionScanMethodDlg, CDialogEx)

SelectionScanMethodDlg::SelectionScanMethodDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SENSITIVEINFO_SCAN, pParent)
{

}

SelectionScanMethodDlg::~SelectionScanMethodDlg()
{
	
}

void SelectionScanMethodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(SelectionScanMethodDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_FULL_SCAN, &SelectionScanMethodDlg::OnBnClickedBtnFullScan)
	ON_BN_CLICKED(IDC_BTN_SELECT_SCAN, &SelectionScanMethodDlg::OnBnClickedBtnSelectScan)
	ON_BN_CLICKED(IDOK, &SelectionScanMethodDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void SelectionScanMethodDlg::OnBnClickedBtnFullScan()
{
	this->SendMessage(WM_CLOSE);
	SenInfoFullScanDlg dlg;
	dlg.DoModal();
}

void SelectionScanMethodDlg::OnBnClickedBtnSelectScan()
{
	this->SendMessage(WM_CLOSE);
	FileSearchDlg dlg;
	dlg.DoModal();

	//SenInfoSelectScanDlg dldg;
	//dldg.DoModal();
}

void SelectionScanMethodDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}