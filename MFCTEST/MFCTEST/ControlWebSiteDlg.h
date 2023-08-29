#pragma once
#include "afxdialogex.h"
#include <string>
#include <vector>


// ControlWebSiteDlg 대화 상자

class CMFCTESTDlg;

class ControlWebSiteDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ControlWebSiteDlg)

public:
	ControlWebSiteDlg(CWnd* pParent = nullptr);  
	virtual ~ControlWebSiteDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONTROL_WEBSITE
	};
#endif

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);  
	virtual BOOL	OnInitDialog();
	void			InitWebSiteListBox();
	
private:
	CListBox	 m_list_ctrl_website;
	CEdit		 m_edit_site;

	std::vector<std::string>	ctrl_webSite_list_temp;
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnBnClickedBtnAddSite();
	afx_msg void OnBnClickedBtnDelSite();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
