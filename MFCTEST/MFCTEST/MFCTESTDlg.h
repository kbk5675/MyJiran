
#pragma once
#include "EThreadState.h"
#include "MFCTEST.h"
#include <string>
#include <memory>

class BlockingCapture;
class SearchTopWindow;

class CMFCTESTDlg : public CDialogEx
{
public:
	CMFCTESTDlg(CWnd* pParent = nullptr);
	~CMFCTESTDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN};
#endif

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);
	virtual BOOL	OnInitDialog();
	afx_msg void	OnPaint();
	afx_msg HCURSOR	OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

public:
	static UINT		ThreadMonitoringTopWindow(LPVOID _mothod);
	static UINT		ThreadBlockingWebSite(LPVOID _mothod);

private:
	void			MonitoringTopWindow();
	void			ShowRunningProcess();
	void			MakeMemBitmap();

	afx_msg void	OnBnClickedCheckBlockingWebsite();
	afx_msg void	OnBnClickedCheckBlockCapture();
	afx_msg void	OnBnClickedCheckMonitoringTopwnd();
	afx_msg void	OnBnClickedBtnShowcontrolproclist();
	afx_msg void	OnBnClickedBtnShowControlWebsite();
	afx_msg void	OnBnClickedBtnRefresh();
	afx_msg void	OnBnClickedBtnSensitiveinfoScan();
	afx_msg void	OnLbnDblclkListProcess();

public:
	std::shared_ptr<BlockingCapture> m_blockingCaputre;

protected:
	HICON				m_hIcon;

private:
	std::shared_ptr<SearchTopWindow> m_searchTopWindow;

	CEdit				m_edit_address;
	CListBox			m_process_list;
	CButton				m_check_monitoring_topWnd;
	CButton				m_check_block_capture;
	CButton				m_check_block_webSite;
	CStatic				m_foreground_process_name;
	CBitmap				m_bitmap;
	HMODULE				m_hDll_ShowRunningProc;
};