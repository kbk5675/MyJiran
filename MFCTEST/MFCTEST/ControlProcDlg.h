#pragma once
#include "afxdialogex.h"
#include <vector>
#include <cstring>

class CMFCTESTDlg;

class CControlProcDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlProcDlg)

public:
	CControlProcDlg(CWnd* pParent = nullptr); 
	virtual ~CControlProcDlg();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONTROL_PROC };
#endif

public:
	afx_msg void	OnBnClickedBtnAddProc();
	afx_msg void	OnBnClickedBtnDelProc();
	afx_msg void	OnBnClickedOk();
	
protected:
	virtual void	DoDataExchange(CDataExchange* pDX);
	virtual BOOL	OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CMFCTESTDlg*	mainDlg;
	CListBox		m_control_proc_list;
	CEdit			m_edit_proc;
	
	std::vector<CString> CtrlProces_temp;
public:
	
	afx_msg void OnBnClickedCancel();
};
