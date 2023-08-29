// SearchFileDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCTEST.h"
#include "afxdialogex.h"
#include "SearchFileDlg.h"


// SearchFileDlg 대화 상자

IMPLEMENT_DYNAMIC(SearchFileDlg, CDialogEx)

SearchFileDlg::SearchFileDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

SearchFileDlg::~SearchFileDlg()
{
}

void SearchFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SearchFileDlg, CDialogEx)
END_MESSAGE_MAP()


// SearchFileDlg 메시지 처리기
