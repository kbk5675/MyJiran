#include "pch.h"
#include "MFCTEST.h"
#include "afxdialogex.h"
#include "SenInfoSelectScanDlg.h"

IMPLEMENT_DYNAMIC(SenInfoSelectScanDlg, CDialogEx)

SenInfoSelectScanDlg::SenInfoSelectScanDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SELECT_SCAN, pParent)
{

}

SenInfoSelectScanDlg::~SenInfoSelectScanDlg()
{
}

void SenInfoSelectScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SenInfoSelectScanDlg, CDialogEx)
END_MESSAGE_MAP()