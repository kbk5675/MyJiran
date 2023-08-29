#pragma once
#include <string>
#include <vector>
#include <regex>
#include "ESensitiveInfoList.h"
#include "EThreadState.h"
#include "ThreadManager.h"

// <파일스캐너 클래스>

class FileScanner
{
public:
	FileScanner();
	virtual ~FileScanner();

protected:
	bool		SearchFile(const std::wstring& directory);
	void		Reset();

private:
	void		ReadFile(std::string filePath);
	void		ReadFile(CString filePath);
	void		SearchSensitiveInfo(CString text);
	void		ScanEmail(std::string text);
	void		ScanRRN(std::string text);
	void		ScanPhone(std::string text);
	void		ManufacturingText(CString& text);

protected:
	int			m_email_count;
	int			m_RRN_count;
	int			m_phoneNum_count;
	int			m_file_count;
	int			m_resultFile_count;
	bool		m_bIsSeninfoFile;

	std::vector<std::string> m_v_result_RRN;
	std::vector<std::string> m_v_result_phone;
	std::vector<std::string> m_v_result_email;
};