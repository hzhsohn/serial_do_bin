// NetLogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetLogDlg.h"
#include "afxdialogex.h"


// CNetLogDlg 对话框

IMPLEMENT_DYNAMIC(CNetLogDlg, CDialogEx)

CNetLogDlg::CNetLogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetLogDlg::IDD, pParent)
{
	_dwLogLine=0;
}

CNetLogDlg::~CNetLogDlg()
{
}

void CNetLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, _txtEventLog);

	_txtEventLog.setHwnd(this->GetSafeHwnd());
}


BEGIN_MESSAGE_MAP(CNetLogDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNetLogDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNetLogDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_MYMESSAGE,OnEditClearCallback)
	ON_WM_SIZING()
END_MESSAGE_MAP()


// CNetLogDlg 消息处理程序


void CNetLogDlg::OnBnClickedOk()
{
	
}


void CNetLogDlg::OnBnClickedCancel()
{
	this->ShowWindow(0);
}

void CNetLogDlg::setlog(CString buf)
{
	 #define RETURN_LINE _T("\r\n")

	 TCHAR buf2[52];

	 struct tm *ptm = NULL;
	 time_t tme;
	 tme = time(NULL);
	 ptm = localtime(&tme);


	 //_stprintf(buf2,_T("[%d-%02d-%02d %02d:%02d:%02d]"),(ptm->tm_year + 1900),ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min,ptm->tm_sec);
	  _stprintf(buf2,_T("[ %02d:%02d:%02d ]"), ptm->tm_hour, ptm->tm_min,ptm->tm_sec);

	 _sLog+=buf2;
	 _sLog+=buf; 
	 _sLog+=RETURN_LINE;


	 _dwLogLine++;
	 while(_dwLogLine>200)
	 {
	  _sLog.Delete(0,_sLog.Find(RETURN_LINE)+2);
	  _dwLogLine--;
	 }

	 _txtEventLog.SetWindowText(_sLog);
	 _txtEventLog.PostMessage(WM_VSCROLL, SB_BOTTOM, 0); 
}

LRESULT CNetLogDlg::OnEditClearCallback(WPARAM wParam, LPARAM lParam)
{
	_sLog=_T("");
	_dwLogLine=0;
	_txtEventLog.SetWindowText(_sLog);
	return S_OK;
}

//移动窗体函数
CRect CNetLogDlg::MoveFrm(int top,int left,CRect winRect)
{ 
    winRect.right=left+(winRect.right-winRect.left);
    winRect.bottom=top+(winRect.bottom-winRect.top);
    winRect.left=left;
    winRect.top=top;
    return winRect;
}

void CNetLogDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	//改变窗体大小
	CRect rx; 
    this->GetWindowRect(&rx); 
	rx.right-=18;
	rx=MoveFrm(0,0,rx); 
	_txtEventLog.MoveWindow(rx);
}
