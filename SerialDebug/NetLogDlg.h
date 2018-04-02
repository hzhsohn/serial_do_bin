#pragma once

#ifndef __CNetLogDlg__H__

#include "resource.h"
#include "afxwin.h"
#include "NetLogDlgMenuEdit.h"

// CNetLogDlg 对话框

class CNetLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNetLogDlg)

public:
	CNetLogDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNetLogDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void setlog(CString buf);

	DWORD _dwLogLine; //日志行数
	CString _sLog;
	CNetLogDlgMenuEdit _txtEventLog;

	LRESULT OnEditClearCallback(WPARAM wParam, LPARAM lParam);
	CRect CNetLogDlg::MoveFrm(int top,int left,CRect winRect);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};


#define __CNetLogDlg__H__
#endif