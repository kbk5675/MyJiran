#pragma once
#include "afxdialogex.h"
#include "ESensitiveInfoList.h"
#include <vector>
#include <string>
#include <cstring>

class FileScanManager;
class ThreadManager;
class CMFCTESTDlg;

class SenInfoFullScanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SenInfoFullScanDlg)

public:
	SenInfoFullScanDlg(CWnd* pParent = nullptr);   
	virtual ~SenInfoFullScanDlg();

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FULL_SCAN };
#endif

protected:
	DECLARE_MESSAGE_MAP()

	virtual void	DoDataExchange(CDataExchange* pDX);
	
	
private:
	static  UINT	ThreadFullScan(LPVOID _mothod);
	static  UINT	ThreadScanTimer(LPVOID _mothod);

	virtual BOOL	OnInitDialog();
	void			FullScan();
	void			GetAllDrivers();
	void			InitDisplay();
	void			SetData(int count, CStatic& static_value);

private:
	CMFCTESTDlg* m_mainDlg;

	HANDLE m_hEvent;

	std::vector<std::string> m_array_of_drivers;
	
public:
	CStatic m_static_timer;
	CStatic m_static_fullScan_file_count;
	CStatic m_static_fullScan_result_count;
	CStatic m_static_fullScan_email_count;
	CStatic m_static_fullScan_RRN_count;
	CStatic m_static_fullScan_phone_count;
	CButton m_btn_ok;
	float	m_time;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};