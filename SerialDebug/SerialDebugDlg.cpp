
// SerialDebugDlg.cpp : ʵ���ļ�
/*
    By ���Ǻ�-Han.zh
	���ڵ�������,���ܾ��ǵ�����~
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
//�߼�����
bool _isSendStatus;	
TYPE_CS g_sendCS;  //���ͻ�����
TYPE_CS g_recvPrintCS;  //��ӡ������
	
//Э�鼯���б�
CListBox _ProtocolList;

//
#define PIEXL_BN		2

// CSerialDebugDlg �Ի���
#define TIMER_AUTO_SEND		2

//��־
DWORD _dwLogLine; //��־����
CString _sLog;
CMainDlgMenuEdit _txtEventLog;

//
CNetLogDlg _netLogDlg;
//
//���幤������ť����ID
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

// CSerialDebugDlg ��Ϣ�������
BOOL CSerialDebugDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//��ʱ��
	SetTimer(TIMER_AUTO_SEND,1,NULL);
	INIT_CS(&g_sendCS);
	INIT_CS(&g_recvPrintCS);

	////////////
	_dwLogLine=0;
	_isSendStatus=false;

	//������־����
	_netLogDlg.Create(IDD_DIALOG3);
	//�¼���־
	_txtEventLog.setHwnd(this->GetSafeHwnd());
	//���ؿؼ�
	_progress.ShowWindow(0);
	//
	CString sTmp=_T("");
	_fileHex16String.InsertColumn(0,_T("��ַ"),LVCFMT_LEFT, 180/PIEXL_BN);
	for(int i=1;i<=16;i++)
	{
		sTmp.Format(_T("%02X"),(BYTE)i-1);
		_fileHex16String.InsertColumn(i,sTmp, LVCFMT_LEFT, 16==i?120/PIEXL_BN:60/PIEXL_BN);
	}
	_fileHex16String.InsertColumn(17,_T("����"),LVCFMT_LEFT, 230/PIEXL_BN);
	_fileHex16String.GetHeaderCtrl()->EnableWindow(false);
	_fileHex16String.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	//������-----
	//����ͼ���б�
	m_ImageList.Create(60, 60, ILC_COLOR24|ILC_MASK, 1, 1); 
	//��ͼ���б������ͼͼ��
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
	 //���幤��������ID����
	 UINT nArray[9];
	 for(int i=0; i<9; i++)
	 {
	  nArray[i] = ID_BTN1 + i;
	 }
	m_Toolbar.CreateEx(this);
	m_Toolbar.SetButtons(nArray, 9);
	//���ù�������ť�Ͱ�ťͼ���С
	m_Toolbar.SetSizes(CSize(80, 100), CSize(60, 60));
	//���ù������ı�
	m_Toolbar.SetButtonText(0, _T("      ��      "));
	m_Toolbar.SetButtonText(1, _T("      ���      "));
	m_Toolbar.SetButtonText(2, _T("      �Զ�      "));
	m_Toolbar.SetButtonText(3, _T("      ����      "));
	m_Toolbar.SetButtonText(4, _T("      У��      "));
	m_Toolbar.SetButtonText(5, _T("      ����      "));
	m_Toolbar.SetButtonText(6, _T("      ��ֹ      "));
	m_Toolbar.SetButtonText(7, _T("      ����      "));
	m_Toolbar.SetButtonText(8, _T("      �˳�      "));

	 //���ù�������ť��ʾͼ��
	 m_Toolbar.GetToolBarCtrl().SetImageList(&m_ImageList);
	 //��ʾ������
	 RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	 /* λ��
	 CRect rx;
	 m_Toolbar.GetClientRect(&rx);
	 rx.top+=20/PIEXL_BN;
	 m_Toolbar.MoveWindow(&rx);*/
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSerialDebugDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSerialDebugDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSerialDebugDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}

void CSerialDebugDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(MessageBox(_T("�Ƿ��˳�?"),_T("��ʾ"),MB_ICONQUESTION|MB_YESNO) == IDYES)   
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
			MessageBox(_T("�˿ڸ����ʲ���Ϊ��!!"),_T("����"),0);
			setMainLog(_T("�˿���Ϣ����"));
		}
		else
		{
			CString str;
			int err;
			_isSerialConnect=!zhSerialOpen(&_sp,(char*)dev.c_str(),nbrte);
			str.Format(_T("���Ӵ���:%s,%d"),CString(dev.c_str()),nbrte);
			setMainLog(str);

			err=GetLastError();
			if(!_isSerialConnect)
			{
				setMainLog(_T("�˿ڴ�ʧ��"));
				MessageBox(_T("�˿ڴ�ʧ��"),_T("��ʾ"),0);
				return;
			}

			_cbPort.EnableWindow(0);
			_cbBute.EnableWindow(0);
			SetTimer(TIMER1,1,NULL);
			//_btnSerialOpen.SetWindowText(_T("�رն˿�"));
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
		setMainLog(_T("�رն˿�"));
}

void CSerialDebugDlg::OnTimer(UINT nIDEvent)
{
	int ret;
	char tcpBuf[1024]={0};

	//serial����
	ret=zhSerialRead(&_sp,(unsigned char*)tcpBuf,1023);
	if(ret>0)
	{
		CString str;
		str.Format(_T("�յ�%d�ֽ�"),ret);
		setMainLog(str);
		str.Format(_T(" ��[����] %d�ֽ� : %s"),ret,CzhEncode::trPrint16(ret,(unsigned char*)tcpBuf));
		setNetLog(str);
	}
	else if(-1==ret)
	{
		//���ڱ��Ͽ�
		//setMainLog(_T("���ڹر�״̬"));
		_isSerialConnect=false;
	}

	//-��⴮���Ƿ�������������----------------------------------
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
					setMainLog(_T("�������ӳɹ�\n")); 
					_isShowOpen=true;
				}
				_isSerialConnect=true;
			}
			else
			{
				setMainLog(_T("���ڹر�״̬"));
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
		str.Format(_T(" ��[����] %d�ֽ� : %s"),len,CzhEncode::trPrint16(len,(unsigned char*)buf));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

    LV_ITEM* pItem= &(pDispInfo)->item;
    int  iItemIndex= pItem->iItem;  //�к�
    int col = pItem->iSubItem;  //�к�
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
	// TODO: �ڴ˴������Ϣ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE,_T(".exe"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("�����ļ�(*.*)|"));
	if(dlg.DoModal()==IDOK)   
	{ 
		//_fileHex16String
		CString FilePath = dlg.GetPathName();
		//��ʾ�ļ�16��������
		if (!PathFileExists(FilePath))  
		{  
			return;  
		}
		
		CString str;
		str.Format(_T("��ȡ�ļ�%s"),FilePath);
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
					str.Format(_T("�ļ����ܳ���1024KB, ��ǰ%.1fKB"),(float)filesize/1024.0f);
					MessageBox(str,_T(""),0);
			
					str.Format(_T("��ȡ�ļ�ʧ��"));
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
								//�Զ�ת�� \0 �ַ�
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
				str.Format(_T("�ļ���ȡ�ɹ� ��%d�ֽ�"),filesize);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_arylistData.size()==0)
	{
		MessageBox(_T("δѡ����¼�̼����ļ�!!"),_T("����"),0);
		return;
	}

	if(_bIsOpen)
	{
		closeSerial();
	}
	else
	{
			openSerial();
			//��¼
			setMainLog(_T("��ʼ��¼"));
			//��������---------------------------
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
			str.Format(_T("һ���������� %d�ֽ�"),ttSendLen);
			setMainLog(str);

			UNLOCK_CS(&g_sendCS);
			_progress.ShowWindow(0);
			//-------------------------------------
	}
}
//�����ǰ����Ĺ���
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
// ������������ת��¼
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