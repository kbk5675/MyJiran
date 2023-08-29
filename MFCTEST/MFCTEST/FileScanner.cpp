#include "pch.h"
#include "FileScanner.h"
#include "MFCTESTDlg.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <codecvt>
#include <filesystem>


FileScanner::FileScanner() :
	m_email_count(0), m_RRN_count(0), m_phoneNum_count(0), 
	m_file_count(0), m_resultFile_count(0), m_bIsSeninfoFile(false)
{
}

FileScanner::~FileScanner()
{
}

// 파일 걸러내는 작업 함수
bool FileScanner::SearchFile(const std::wstring& directory)
{
    std::wstring searchPath = directory + L"\\*";
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return false;
	}

    do
    {
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
			continue;
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			continue;
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DEVICE)
			continue;
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
			continue;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (_tcscmp(findData.cFileName, L"Windows") == 0)
                continue;
            if (_tcscmp(findData.cFileName, L"Program Files") == 0)
                continue;
            if (_tcscmp(findData.cFileName, L"Program Files (x86)") == 0)
                continue;
			if (_tcscmp(findData.cFileName, L"OneDrive") == 0)
				continue;
		
            if (_tcscmp(findData.cFileName, L".") != 0 && _tcscmp(findData.cFileName, L"..") != 0)
            {
                std::wstring subDirectory = directory + L"\\" + findData.cFileName;
                SearchFile(subDirectory);
            }
        }
        else
        {
            std::wstring filePath = directory + L"\\" + findData.cFileName;
            std::string filePath_str = CT2CA(filePath.c_str());
      
            if (filePath_str.find(".txt") == std::string::npos)
            {
                continue;
            }

			ReadFile(filePath.c_str());
        }
    } while (FindNextFile(hFind, &findData));

	if (!FindNextFile(hFind, &findData))
	{
		FindClose(hFind);
		return true;
	}

	return false;
}

void FileScanner::ReadFile(std::string filePath)
{
	++m_file_count;

	CString text_cstr;

	std::string line;
	std::ifstream file(filePath);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (line.size() > 8)
			{
				//SearchSensitiveInfo(line);
			}
		}
		file.close();
	}
	else
	{
		return;
	}

	if (m_bIsSeninfoFile == true)
	{
		m_resultFile_count++;
		m_bIsSeninfoFile = false;
	}
}

void FileScanner::ReadFile(CString filePath)
{
	// 1. 파일 읽을 핸들 생성
	CStdioFile file;
	if (!file.Open(filePath, CFile::modeRead(NULL)))
	{
		return;
	}
	
	// 2. 검사 파일 갯수 증가
	++m_file_count;

	// 3. 파일 읽어 CString에 보관
	CString contents, line;
	while (file.ReadString(line)) 
	{
		if (line.GetLength() < 8)
		{
			continue;
		}
		
		contents += line;
	}

	// 4. 줄바꿈 모두 없애고 띄어쓰기를 줄바꿈으로 변경
	ManufacturingText(contents);

	// 5. 민감정보 검사
	SearchSensitiveInfo(contents);
	 
	// 6. 민감정보가 검출된 파일이면 갯수 증가
	if (m_bIsSeninfoFile == true)
	{
		m_resultFile_count++;
		m_bIsSeninfoFile = false;
	}
}

void FileScanner::ManufacturingText(CString& text)
{
	text.Remove('\n');
	text.Replace(' ', '\n');
}

void FileScanner::SearchSensitiveInfo(CString contents)
{
	// 5. 한줄씩 민감정보 검사
	CString line_cstr;
	for (long i = 0; i < contents.GetLength(); i++)
	{
		TCHAR character = contents.GetAt(i);

		line_cstr += character;

		if (character == _T('\n'))
		{
			if (!line_cstr.IsEmpty() && line_cstr.GetLength() < 8)
			{
				line_cstr.Empty();
				continue;
			}

			std::string line_str;
			line_str = CT2A(line_cstr);

			ScanRRN(line_str);
			ScanEmail(line_str);
			ScanPhone(line_str);

			line_cstr.Empty();
			continue;
		}
	}
}

void FileScanner::ScanEmail(std::string text)
{
	std::regex	regex_email_front("[0-9a-zA-Z]([-_.]?[0-9a-zA-Z])*@");
	std::regex	regex_email_back("([-_.]?[0-9a-zA-Z])*[.][a-zA-Z]{2,3}");

	int startIndex = 0;
	startIndex = text.find('@');

	if (0 >= (startIndex - 1) || text[startIndex - 1] == ' ')
	{
		return;
	}

	std::string str_front;
	for (int i = startIndex; i > 0; i--)
	{
		str_front = text[i] + str_front;
	}

	std::string str_back;
	for (int i = startIndex + 1; i < text.size(); i++)
	{
		str_back = str_back + text[i];
	}

	auto WordsBegin = std::sregex_iterator(str_front.begin(), str_front.end(), regex_email_front);
	auto WordsEnd = std::sregex_iterator();

	if (std::distance(WordsBegin, WordsEnd))
	{
		auto WordsBegin = std::sregex_iterator(str_back.begin(), str_back.end(), regex_email_back);
		auto WordsEnd = std::sregex_iterator();

		if (std::distance(WordsBegin, WordsEnd))
		{
			for (std::sregex_iterator i = WordsBegin; i != WordsEnd; ++i)
			{
				std::smatch match = *i;
				std::string match_str = match.str();
				m_v_result_email.push_back(match_str);
				++m_email_count;
				m_bIsSeninfoFile = true;
				return;
			}
		}
	}
	return;
}

void FileScanner::ScanRRN(std::string text)
{
	if (text.find('-') == std::string::npos)
	{
		return;
	}

	std::regex regex_RRN("[0-9]{2}(?:0[1-9]|1[0-2])(0[1-9]|[1,2][0-9]|3[0,1])-[1-4][0-9]{6}");

	auto wordsBegin = std::sregex_iterator(text.begin(), text.end(), regex_RRN);
	auto wordsEnd = std::sregex_iterator();

	if (std::distance(wordsBegin, wordsEnd))
	{
		for (std::sregex_iterator i = wordsBegin; i != wordsEnd; ++i)
		{
			std::smatch match = *i;
			std::string match_str = match.str();
			m_v_result_RRN.push_back(match_str);
			++m_RRN_count;
			m_bIsSeninfoFile = true;
			return;
		}
	}
	return;
}

void FileScanner::ScanPhone(std::string text)
{
	if (text.find('-') == std::string::npos)
	{
		return;
	}

	std::regex regex_phone("01([0|1|6|7|8|9]?)-?([0-9]{3,4})-?([0-9]{4})");

	auto wordsBegin = std::sregex_iterator(text.begin(), text.end(), regex_phone);
	auto wordsEnd = std::sregex_iterator();

	if (std::distance(wordsBegin, wordsEnd))
	{
		for (std::sregex_iterator i = wordsBegin; i != wordsEnd; ++i)
		{
			std::smatch match = *i;
			std::string match_str = match.str();
			m_v_result_phone.push_back(match_str);
			++m_phoneNum_count;
			m_bIsSeninfoFile = true;
			return;
		}
	}
	return;
}

void FileScanner::Reset()
{
	m_v_result_RRN.clear();
	m_v_result_phone.clear();
	m_v_result_email.clear();

	m_email_count		= 0;
	m_RRN_count			= 0;
	m_phoneNum_count	= 0;
	m_file_count		= 0;
	m_resultFile_count  = 0;
	m_bIsSeninfoFile	= 0;
}