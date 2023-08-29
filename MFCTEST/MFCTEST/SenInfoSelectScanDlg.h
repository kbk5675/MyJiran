#pragma once
#include "afxdialogex.h"


// SenInfoSelectScanDlg 대화 상자

class SenInfoSelectScanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SenInfoSelectScanDlg)

public:
	SenInfoSelectScanDlg(CWnd* pParent = nullptr);   
	virtual ~SenInfoSelectScanDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELECT_SCAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
};
