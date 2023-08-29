#include "pch.h"
#include "MFCTEST.h"
#include "afxdialogex.h"
#include "FileSearchDlg.h"


// FileSearchDlg 대화 상자

IMPLEMENT_DYNAMIC(FileSearchDlg, CDialogEx)

FileSearchDlg::FileSearchDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FILESEARCH, pParent)
{

}

FileSearchDlg::~FileSearchDlg()
{
}

void FileSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DIR, m_TreeDir);
	DDX_Control(pDX, IDC_EDIT_FILE_LOCATION, m_edit_fileLocation);
}


BEGIN_MESSAGE_MAP(FileSearchDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_START, &FileSearchDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &FileSearchDlg::OnBnClickedBtnStop)
	ON_EN_CHANGE(IDC_EDIT_FILE_LOCATION, &FileSearchDlg::OnEnChangeEditFileLocation)
	ON_EN_CHANGE(IDC_EDIT_FILENAME, &FileSearchDlg::OnEnChangeEditFilename)
	ON_STN_CLICKED(IDC_STATIC_STATUS, &FileSearchDlg::OnStnClickedStaticStatus)
	ON_LBN_SELCHANGE(IDC_LIST_RESULT, &FileSearchDlg::OnLbnSelchangeListResult)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DIR, &FileSearchDlg::OnTvnSelchangedTreeDir)
END_MESSAGE_MAP()


// FileSearchDlg 메시지 처리기


BOOL FileSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitializeListBox();




	return TRUE;
}

void FileSearchDlg::InitializeListBox()
{
	m_TreeDir.ModifyStyle(TVS_CHECKBOXES, 0);
	m_TreeDir.ModifyStyle(0, TVS_CHECKBOXES);

	HTREEITEM h_item;
	h_item = m_TreeDir.InsertItem(_T("C:", h_item = TVHT_ONITEMBUTTON));

	CFileFind finder;
	BOOL bWorking = finder.FindFile(_T("C:\\*.*"));


	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory())
		{
			m_TreeDir.InsertItem(finder.GetFileName(), h_item);

		}
	}
	

	m_TreeDir.EnsureVisible(h_item);
}



void FileSearchDlg::OnBnClickedBtnStart()
{
	CString filePath;
	m_edit_fileLocation.GetWindowTextW(filePath);


}


void FileSearchDlg::OnBnClickedBtnStop()
{
	
}


void FileSearchDlg::OnEnChangeEditFileLocation()
{
	
}


void FileSearchDlg::OnEnChangeEditFilename()
{
	
}


void FileSearchDlg::OnStnClickedStaticStatus()
{
	
}


void FileSearchDlg::OnLbnSelchangeListResult()
{
	
}



void FileSearchDlg::OnTvnSelchangedTreeDir(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	

	*pResult = 0;
}
