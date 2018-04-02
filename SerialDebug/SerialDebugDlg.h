
// SerialDebugDlg.h : 头文件
//

#pragma once

#include "serial.h"
#include "afxwin.h"

#include <string>
#include <vector>

#include "NetLogDlg.h"
#include <io.h>
#include "afxcmn.h"
#include "MainDlgMenuEdit.h"

using namespace std;

//搜索文件结构
typedef enum _EzhFileAttrib{
	ezhFileAttribUnknow,
	ezhFileAttribFile,
	ezhFileAttribDir
}EzhFileAttrib;

// CSerialDebugDlg 对话框
class CSerialDebugDlg : public CDialogEx
{
// 构造
public:
	CSerialDebugDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERIALDEBUG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	
	//-----------------------------
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton3();
	
	
	void trSendStringToHex();
	void trSendHexToString();
	
	EzhFileAttrib returnFileAttrib(int attrib);

	LRESULT OnEditClearCallback(WPARAM wParam, LPARAM lParam);

public:
	bool _bIsOpen;//是否打开串口连接
	
	CComboBox _cbPort;
	CComboBox _cbBute;	
	string dev;   //打开的端口
	int nbrte;    //打开的速率	
	int _isShowOpen;	  //是否已经显示串口连接成功的名字

	SERIAL _sp;
	BOOL _isSerialConnect;//串口是否连接成功
	DWORD dwTime;//COM口检查连接的最后时间
	
public:
	afx_msg void OnBnClickedButton2();
	void OnTimer(UINT nIDEvent);

	void openSerial();
	void closeSerial();

	/* 发送失败回复 -1 否则返回发送字节*/
	int send(unsigned char*buf,int len);
	BOOL isEnable();
	CStatic _lbCurLinkCount;
	int _nCurLinkCount;

	afx_msg void OnBnClickedButton1();
	CProgressCtrl _progress;
	CListCtrl _fileHex16String;
	afx_msg void OnLvnGetdispinfoList1(NMHDR *pNMHDR, LRESULT *pResult);
	//
	CImageList m_ImageList;
	CToolBar m_Toolbar;
	//
	void OnBnClickedButton4();
	void OnBnClickedButton5();
	void OnBnClickedButton6();
	void OnBnClickedButton7();
	void OnBnClickedButton8();
	void OnBnClickedButton9();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

//查询串口状态
BOOL checkStatus();
//发送命令
void WINAPI SendCmd();