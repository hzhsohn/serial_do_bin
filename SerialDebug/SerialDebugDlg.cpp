
// SerialDebugDlg.cpp : 实现文件
/*
    By 韩智鸿-Han.zh
	串口调试助手,功能就是调试呗~
*/

//

#include "stdafx.h"
#include "SerialDebug.h"
#include "SerialDebugDlg.h"
#include "afxdialogex.h"
#include "CzhEncode.h"
#include "StringUtil.h"
#include <direct.h>
#include "platform.h"
#include   <afxdlgs.h>

#define TIMER1		1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//----------------------------
//逻辑变量
bool _isSendStatus;	
TYPE_CS g_sendCS;  //发送互斥锁
TYPE_CS g_recvPrintCS;  //打印互斥锁
	
//协议集的列表
CListBox _ProtocolList;

//
#define PIEXL_BN		2

// CSerialDebugDlg 对话框
#define TIMER_AUTO_SEND		2

//日志
DWORD _dwLogLine; //日志行数
CString _sLog;
CMainDlgMenuEdit _txtEventLog;

//
CNetLogDlg _netLogDlg;
//
//定义工具栏按钮命令ID
#define ID_BTN1  1001
#define ID_BTN2  1002
#define ID_BTN3  1003
#define ID_BTN4  1004
#define ID_BTN5  1005
#define ID_BTN6  1006
#define ID_BTN7  1007
#define ID_BTN8  1008
#define ID_BTN9  1009

struct TagListData{
	unsigned char buf[16];
	int len;
};
vector<TagListData> g_arylistData;

void setMainLog(CString buf)
{
	 #define RETURN_LINE _T("\r\n")

	 TCHAR buf2[52];

	 struct tm *ptm = NULL;
	 time_t tme;
	 tme = time(NULL);
	 ptm = localtime(&tme);


	 //_stprintf(buf2,_T("[%d-%02d-%02d %02d:%02d:%02d]%s"),(ptm->tm_year + 1900),ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min,ptm->tm_sec,RETURN_LINE);
	  _stprintf(buf2,_T("[%02d:%02d:%02d] "), ptm->tm_hour, ptm->tm_min,ptm->tm_sec);

	 _sLog+=buf2;
	 _sLog+=buf; 
	 _sLog+=RETURN_LINE;


	 _dwLogLine++;
	 while(_dwLogLine>100)
	 {
	  _sLog.Delete(0,_sLog.Find(RETURN_LINE)+2);
	  _dwLogLine--;
	 }

	 _txtEventLog.SetWindowText(_sLog);
	 _txtEventLog.PostMessage(WM_VSCROLL, SB_BOTTOM, 0); 
	 _txtEventLog.UpdateWindow();
}

void setNetLog(CString buf)
{
	_netLogDlg.setlog(buf);
	_netLogDlg.UpdateWindow();
}

CRect MoveFrm(int top,int left,CRect winRect)
{
	winRect.right=left+(winRect.right-winRect.left);
	winRect.bottom=top+(winRect.bottom-winRect.top);
	winRect.left=left;
	winRect.top=top;
	return winRect;
}

CSerialDebugDlg::CSerialDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerialDebugDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT7, _txtEventLog);
	DDX_Control(pDX, IDC_COMBO1, _cbPort);
	DDX_Control(pDX, IDC_COMBO2, _cbBute);

	_bIsOpen=false;	
	_isSerialConnect=false;
	_isShowOpen=false;	
	memset(&_sp,0,sizeof(SERIAL));
	_cbPort.SetCurSel(0);
	_cbBute.SetCurSel(0);

	SetTimer(TIMER1,1,NULL);
	DDX_Control(pDX, IDC_PROGRESS1, _progress);
	DDX_Control(pDX, IDC_LIST1, _fileHex16String);
}

BEGIN_MESSAGE_MAP(CSerialDebugDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CSerialDebugDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSerialDebugDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT2, &CSerialDebugDlg::OnEnChangeEdit2)
	ON_MESSAGE(WM_MYEDIT_MESSAGE,OnEditClearCallback)
	ON_BN_CLICKED(ID_BTN1, &CSerialDebugDlg::OnBnClickedButton1)
	ON_BN_CLICKED(ID_BTN2, &CSerialDebugDlg::OnBnClickedButton2)
	ON_BN_CLICKED(ID_BTN3, &CSerialDebugDlg::OnBnClickedButton3)
	ON_BN_CLICKED(ID_BTN4, &CSerialDebugDlg::OnBnClickedButton4)
	ON_BN_CLICKED(ID_BTN5, &CSerialDebugDlg::OnBnClickedButton5)
	ON_BN_CLICKED(ID_BTN6, &CSerialDebugDlg::OnBnClickedButton6)
	ON_BN_CLICKED(ID_BTN7, &CSerialDebugDlg::OnBnClickedButton7)
	ON_BN_CLICKED(ID_BTN8, &CSerialDebugDlg::OnBnClickedButton8)
	ON_BN_CLICKED(ID_BTN9, &CSerialDebugDlg::OnBnClickedButton9)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CSerialDebugDlg::OnLvnGetdispinfoList1)
	ON_WM_SIZE()
END_MESSAGE_MAP()

EzhFileAttrib CSerialDebugDlg::returnFileAttrib(int attrib)
{
    switch(attrib)
    {
        case _A_ARCH:            
        case _A_ARCH|_A_RDONLY:            
        case _A_ARCH|_A_HIDDEN:
        case _A_ARCH|_A_SYSTEM:            
        case _A_ARCH|_A_SYSTEM|_A_RDONLY:            
        case _A_ARCH|_A_SYSTEM|_A_HIDDEN:            
        case _A_ARCH|_A_HIDDEN|_A_RDONLY:            
        case _A_ARCH|_A_HIDDEN|_A_RDONLY|_A_SYSTEM:            
            return ezhFileAttribFile;
                
        case _A_SUBDIR:
        case _A_SUBDIR|_A_RDONLY:   
        case _A_SUBDIR|_A_HIDDEN:
        case _A_SUBDIR|_A_SYSTEM:
        case _A_SUBDIR|_A_SYSTEM|_A_RDONLY:
        case _A_SUBDIR|_A_SYSTEM|_A_HIDDEN:
        case _A_SUBDIR|_A_HIDDEN|_A_RDONLY:
        case _A_SUBDIR|_A_HIDDEN|_A_RDONLY|_A_SYSTEM:
            return ezhFileAttribDir;           
    }
    return ezhFileAttribUnknow;
}

// CSerialDebugDlg 消息处理程序
BOOL CSerialDebugDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//定时器
	SetTimer(TIMER_AUTO_SEND,1,NULL);
	INIT_CS(&g_sendCS);
	INIT_CS(&g_recvPrintCS);

	////////////
	_dwLogLine=0;
	_isSendStatus=false;

	//网络日志窗体
	_netLogDlg.Create(IDD_DIALOG3);
	//事件日志
	_txtEventLog.setHwnd(this->GetSafeHwnd());
	//隐藏控件
	_progress.ShowWindow(0);
	//
	CString sTmp=_T("");
	_fileHex16String.InsertColumn(0,_T("地址"),LVCFMT_LEFT, 180/PIEXL_BN);
	for(int i=1;i<=16;i++)
	{
		sTmp.Format(_T("%02X"),(BYTE)i-1);
		_fileHex16String.InsertColumn(i,sTmp, LVCFMT_LEFT, 16==i?120/PIEXL_BN:60/PIEXL_BN);
	}
	_fileHex16String.InsertColumn(17,_T("内容"),LVCFMT_LEFT, 230/PIEXL_BN);
	_fileHex16String.GetHeaderCtrl()->EnableWindow(false);
	_fileHex16String.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	//工具栏-----
	//创建图像列表
	m_ImageList.Create(60, 60, ILC_COLOR24|ILC_MASK, 1, 1); 
	//向图像列表中添加图图像
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP1);
	m_ImageList.Add(&bmp, RGB(255, 255, 255));
	bmp.DeleteObject();
	bmp.LoadBitmap(IDB_BITMAP2);
	m_ImageList.Add(&bmp, RGB(255, 255, 255));
	bmp.DeleteObject();
	bmp.LoadBitmap(IDB_BITMAP3);
	m_ImageList.Add(&bmp, RGB(255, 255, 255));
	bmp.DeleteObject();
	bmp.LoadBitmap(IDB_BITMAP4);
	m_ImageList.Add(&bmp, RGB(255, 255, 255));
	bmp.DeleteObject();
	bmp.LoadBitmap(IDB_BITMAP5);
	m_ImageList.Add(&bmp, RGB(255, 255, 255));
	bmp.DeleteObject();
	bmp.LoadBitmap(IDB_BITMAP6);
	m_ImageList.Add(&bmp, RGB(255, 255, 255));
	bmp.DeleteObject();
	bmp.LoadBitmap(IDB_BITMAP7);
	m_ImageList.Add(&bmp, RGB(255, 255, 255));
	bmp.DeleteObject();
	bmp.LoadBitmap(IDB_BITMAP8);
	m_ImageList.Add(&bmp, RGB(255, 255, 255));
	bmp.DeleteObject();
	bmp.LoadBitmap(IDB_BITMAP9);
	m_ImageList.Add(&bmp, RGB(255, 255, 255));
	bmp.DeleteObject();
	 //定义工具栏命令ID数组
	 UINT nArray[9];
	 for(int i=0; i<9; i++)
	 {
	  nArray[i] = ID_BTN1 + i;
	 }
	m_Toolbar.CreateEx(this);
	m_Toolbar.SetButtons(nArray, 9);
	//设置工具栏按钮和按钮图像大小
	m_Toolbar.SetSizes(CSize(80, 100), CSize(60, 60));
	//设置工具栏文本
	m_Toolbar.SetButtonText(0, _T("      打开      "));
	m_Toolbar.SetButtonText(1, _T("      编程      "));
	m_Toolbar.SetButtonText(2, _T("      自动      "));
	m_Toolbar.SetButtonText(3, _T("      配置      "));
	m_Toolbar.SetButtonText(4, _T("      校验      "));
	m_Toolbar.SetButtonText(5, _T("      擦除      "));
	m_Toolbar.SetButtonText(6, _T("      终止      "));
	m_Toolbar.SetButtonText(7, _T("      关于      "));
	m_Toolbar.SetButtonText(8, _T("      退出      "));

	 //设置工具栏按钮显示图标
	 m_Toolbar.GetToolBarCtrl().SetImageList(&m_ImageList);
	 //显示工具栏
	 RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	 /* 位置
	 CRect rx;
	 m_Toolbar.GetClientRect(&rx);
	 rx.top+=20/PIEXL_BN;
	 m_Toolbar.MoveWindow(&rx);*/
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSerialDebugDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSerialDebugDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSerialDebugDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
}

void CSerialDebugDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if(MessageBox(_T("是否退出?"),_T("提示"),MB_ICONQUESTION|MB_YESNO) == IDYES)   
	 {
			// if(_isSerialConnect)
			//{
			//closeSerial();
			// }
			CDialogEx::OnCancel();
	 }  
	 else
	  return;
}

void CSerialDebugDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

LRESULT CSerialDebugDlg::OnEditClearCallback(WPARAM wParam, LPARAM lParam)
{
	_sLog=_T("");
	_txtEventLog.SetWindowText(_sLog);
	return TRUE;
}


//------------------------------------------------------

void CSerialDebugDlg::openSerial()
{
		CString sTmp;

		_cbPort.GetWindowText(sTmp);
		dev=StringUtil::ws2s(sTmp.GetBuffer());
		_cbBute.GetWindowText(sTmp);
		nbrte=_ttoi(sTmp);

		if(dev.length()<=0 || nbrte<=0)
		{
			MessageBox(_T("端口跟速率不能为空!!"),_T("错误"),0);
			setMainLog(_T("端口信息出错"));
		}
		else
		{
			CString str;
			int err;
			_isSerialConnect=!zhSerialOpen(&_sp,(char*)dev.c_str(),nbrte);
			str.Format(_T("连接串口:%s,%d"),CString(dev.c_str()),nbrte);
			setMainLog(str);

			err=GetLastError();
			if(!_isSerialConnect)
			{
				setMainLog(_T("端口打开失败"));
				MessageBox(_T("端口打开失败"),_T("提示"),0);
				return;
			}

			_cbPort.EnableWindow(0);
			_cbBute.EnableWindow(0);
			SetTimer(TIMER1,1,NULL);
			//_btnSerialOpen.SetWindowText(_T("关闭端口"));
			_bIsOpen=true;
			_isShowOpen=false;
		}
}

void CSerialDebugDlg::closeSerial()
{
		_cbPort.EnableWindow(1);
		_cbBute.EnableWindow(1);
		KillTimer(TIMER1);
		_bIsOpen=false;

		zhSerialClose(&_sp); 
		setMainLog(_T("关闭端口"));
}

void CSerialDebugDlg::OnTimer(UINT nIDEvent)
{
	int ret;
	char tcpBuf[1024]={0};

	//serial接收
	ret=zhSerialRead(&_sp,(unsigned char*)tcpBuf,1023);
	if(ret>0)
	{
		CString str;
		str.Format(_T("收到%d字节"),ret);
		setMainLog(str);
		str.Format(_T(" 收[串口] %d字节 : %s"),ret,CzhEncode::trPrint16(ret,(unsigned char*)tcpBuf));
		setNetLog(str);
	}
	else if(-1==ret)
	{
		//串口被断开
		//setMainLog(_T("串口关闭状态"));
		_isSerialConnect=false;
	}

	//-检测串口是否正在正常运行----------------------------------
	if(_isSerialConnect)
	{
		DWORD dwTmp;
		dwTmp=GetTickCount();
		if(dwTmp-dwTime>3000)
		{
			int b;
			dwTime=dwTmp;
			
			b=zhSerialCheck(&_sp);
			if(b)
			{
				if(false==_isShowOpen)
				{
					setMainLog(_T("串口连接成功\n")); 
					_isShowOpen=true;
				}
				_isSerialConnect=true;
			}
			else
			{
				setMainLog(_T("串口关闭状态"));
				closeSerial();
				_isSerialConnect=false;
				_isShowOpen=false;
			}
		}
	}

	CWnd::OnTimer(nIDEvent);
}

int CSerialDebugDlg::send(unsigned char*buf,int len)
{
	int ret=zhSerialSend(&_sp,buf,len);
	if(ret<0)
	{return -1;}
	else
	{
		CString str;
		str.Format(_T(" 发[串口] %d字节 : %s"),len,CzhEncode::trPrint16(len,(unsigned char*)buf));
		setNetLog(str);
		return ret;
	}
}

BOOL CSerialDebugDlg::isEnable()
{
	return _isSerialConnect;
}


void CSerialDebugDlg::OnLvnGetdispinfoList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

    LV_ITEM* pItem= &(pDispInfo)->item;
    int  iItemIndex= pItem->iItem;  //行号
    int col = pItem->iSubItem;  //列号
    if(pItem->mask & LVIF_TEXT)
    {
        if(col == 0)
        {
			CString tmp;
			tmp.Format(_T("%08X"),iItemIndex*16);
            lstrcpyn(pItem->pszText, tmp, pItem->cchTextMax);
        }
		else if(col==17)
		{
			CString tmp;
			for(int n=0;n<g_arylistData[iItemIndex].len;n++)
			{
				unsigned char st=g_arylistData[iItemIndex].buf[n];
				if(st<=' ' || st>'~')
				{
					st='.';
				}
				tmp.AppendFormat(_T("%c"),st);
			}
			lstrcpyn(pItem->pszText,tmp , pItem->cchTextMax);
		}
        else if(col >= 1)
        {
			CString tmp;
			tmp.Format(_T("%02X"),g_arylistData[iItemIndex].buf[col-1]);
			lstrcpyn(pItem->pszText,tmp , pItem->cchTextMax);
        }
    }
}

void CSerialDebugDlg::OnSize(UINT nType, int cx, int cy)
{
	// TODO: 在此处添加消息处理程序代码
	if(_fileHex16String)
	{
		CRect rx,rx2;
		_fileHex16String.GetWindowRect(&rx);
		this->GetClientRect(&rx2);
		rx.right=rx.left+rx2.Width()-rx.left-20/PIEXL_BN;
		rx.bottom=rx.top+rx2.Height()-rx.top-20/PIEXL_BN;
		rx.top=190/PIEXL_BN;
		rx.left=338/PIEXL_BN;
		_fileHex16String.MoveWindow(&rx);

		_txtEventLog.GetWindowRect(&rx);
		rx.bottom=rx.top+rx2.Height()-rx.top-20/PIEXL_BN;
		rx.right=320/PIEXL_BN;
		rx.top=370/PIEXL_BN;
		rx.left=20/PIEXL_BN;
		_txtEventLog.MoveWindow(&rx);
	}
	CDialogEx::OnSize(nType, cx, cy);

}


void CSerialDebugDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE,_T(".exe"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("所有文件(*.*)|"));
	if(dlg.DoModal()==IDOK)   
	{ 
		//_fileHex16String
		CString FilePath = dlg.GetPathName();
		//显示文件16进制内容
		if (!PathFileExists(FilePath))  
		{  
			return;  
		}
		
		CString str;
		str.Format(_T("读取文件%s"),FilePath);
		setMainLog(str);

		try{
				CFile file(FilePath, CFile::modeRead);
				DWORD fsize=0;
				DWORD freadsize=0;
				DWORD filesize=file.GetLength();
				BYTE pData[16]={0};
				CString sTmp;
				TagListData lda;

				if(filesize>1024*1024)
				{
					CString str;
					str.Format(_T("文件不能超过1024KB, 当前%.1fKB"),(float)filesize/1024.0f);
					MessageBox(str,_T(""),0);
			
					str.Format(_T("读取文件失败"));
					setMainLog(str);
					return;
				}

				_fileHex16String.DeleteAllItems();
				g_arylistData.clear();
				int nline=0;
				_progress.ShowWindow(1);
				_progress.SetRange(0,100);
				_progress.SetPos(0);
				while(freadsize=file.Read(pData, 16))
				{
						//_fileHex16String.InsertItem(nline,_T(""));
						//sTmp.Format(_T("%08X"),nline*16);
						//_fileHex16String.SetItemText(nline,0,sTmp);
						sTmp=_T("");
						for(int i =1;i<=freadsize;i++)
						{
								sTmp.Format(_T("%02X"),(BYTE)pData[i]);
								//自动转换 \0 字符
								if(0==pData[i])
								{
									pData[i]='.';
								}
								//_fileHex16String.SetItemText(nline,i,sTmp);
						}
						memset(&lda,0,sizeof(lda));
						memcpy(lda.buf,pData,freadsize);
						lda.len=freadsize;
						g_arylistData.push_back(lda);
						fsize+=freadsize;	
						_progress.SetPos(100*fsize/filesize);
						nline++;
				}
				_fileHex16String.SetItemCount(nline);
				_progress.ShowWindow(0);
				file.Close();
				str.Format(_T("文件读取成功 共%d字节"),filesize);
				setMainLog(str);
		}
		catch(CException* e)
		{
			   e->ReportError();
			   e->Delete();
		}

	}
}

void CSerialDebugDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_arylistData.size()==0)
	{
		MessageBox(_T("未选择烧录固件的文件!!"),_T("错误"),0);
		return;
	}

	if(_bIsOpen)
	{
		closeSerial();
	}
	else
	{
			openSerial();
			//烧录
			setMainLog(_T("开始烧录"));
			//发送数据---------------------------
			int ttSendLen=0;
			_progress.SetPos(0);
			_progress.ShowWindow(1);
			LOCK_CS(&g_sendCS);
			for(int i=0;i<g_arylistData.size();i++)
			{
				send(g_arylistData[i].buf,g_arylistData[i].len);
				ttSendLen+=g_arylistData[i].len;
				_progress.SetPos(ttSendLen);
			}
			
			CString str;
			str.Format(_T("一共发送数据 %d字节"),ttSendLen);
			setMainLog(str);

			UNLOCK_CS(&g_sendCS);
			_progress.ShowWindow(0);
			//-------------------------------------
	}
}
//这里是按键的功能
void CSerialDebugDlg::OnBnClickedButton3()
{
	m_Toolbar.GetToolBarCtrl().EnableButton(ID_BTN3,false);
}

void CSerialDebugDlg::OnBnClickedButton4()
{
	m_Toolbar.GetToolBarCtrl().EnableButton(ID_BTN4,false);
}
void CSerialDebugDlg::OnBnClickedButton5()
{
	m_Toolbar.GetToolBarCtrl().EnableButton(ID_BTN5,false);
}
void CSerialDebugDlg::OnBnClickedButton6()
{
	m_Toolbar.GetToolBarCtrl().EnableButton(ID_BTN6,false);
}
void CSerialDebugDlg::OnBnClickedButton7()
{
	m_Toolbar.GetToolBarCtrl().EnableButton(ID_BTN7,false);
}
void CSerialDebugDlg::OnBnClickedButton8()
{
}
void CSerialDebugDlg::OnBnClickedButton9()
{
	OnBnClickedCancel();
}

/*
// 打开网络数据中转记录
	if(FALSE==_netLogDlg.IsWindowVisible())
	{
		CRect rc,rx;
		this->GetWindowRect(rc);

		_netLogDlg.GetWindowRect(&rx);
		rx.right=rc.left+(rx.right-rx.left);
		rx.left=rc.left;
		rx.bottom=rc.bottom+(rx.bottom-rx.top)+16;
		rx.top=rc.bottom+16;
	
		_netLogDlg.MoveWindow(&rx);
		_netLogDlg.ShowWindow(1);
	}
	else
	{
		_netLogDlg.ShowWindow(0);
	}
*/