#pragma once
#include "FileScanner.h"
#include "EThreadState.h"

class CMFCTESTDlg;

class FileScanManager : public FileScanner
{
public:
	FileScanManager();
	~FileScanManager();

	bool StartFullScan(std::wstring filePath);
	bool StartScan(std::string filePath, ESENSITIVEINFO senInfo);
	void StopSearchInfo();
	void StopScan();
	void Reset();
	
	int	 GetEmailCount()	{ return m_email_count; }
	int	 GetRRNCount()		{ return m_RRN_count; }
	int	 GetPhoneNumCount()	{ return m_phoneNum_count; }
	int	 GetFileCount()		{ return m_file_count; }
	int  GetResultCount()	{ return m_resultFile_count; }

private:
	static UINT	ThreadFullScan(LPVOID _mothod);
	
private:
	bool m_is_scan_over;
	std::wstring m_start_file_path;
};