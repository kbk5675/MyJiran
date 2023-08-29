// MessageDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCTEST.h"
#include "afxdialogex.h"
#include "MessageDlg.h"


// MessageDlg 대화 상자

IMPLEMENT_DYNAMIC(MessageDlg, CDialogEx)

MessageDlg::MessageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MESSAGE, pParent)
{

}

MessageDlg::~MessageDlg()
{
}

void MessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MessageDlg, CDialogEx)
END_MESSAGE_MAP()


// MessageDlg 메시지 처리기
