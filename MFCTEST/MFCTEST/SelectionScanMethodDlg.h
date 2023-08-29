#pragma once
#include "afxdialogex.h"


// SelectionScanMethodDlg 대화 상자

class SelectionScanMethodDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SelectionScanMethodDlg)

public:
	SelectionScanMethodDlg(CWnd* pParent = nullptr);   
	virtual ~SelectionScanMethodDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SENSITIVEINFO_SCAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnFullScan();
	afx_msg void OnBnClickedBtnSelectScan();
	afx_msg void OnBnClickedOk();
};
