#pragma once
#include "afxdialogex.h"

class FileSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FileSearchDlg)

public:
	FileSearchDlg(CWnd* pParent = nullptr);  
	virtual ~FileSearchDlg();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILESEARCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()

private:
	bool m_bStop;
	CString m_strToken;
	CImageList m_img;
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnEnChangeEditFileLocation();
	afx_msg void OnEnChangeEditFilename();
	afx_msg void OnStnClickedStaticStatus();
	afx_msg void OnLbnSelchangeListResult();
	virtual BOOL OnInitDialog();

	void InitializeListBox();
	afx_msg void OnTvnSelchangedTreeDir(NMHDR* pNMHDR, LRESULT* pResult);

private:
	CTreeCtrl m_TreeDir;
	CEdit m_edit_fileLocation;
};
