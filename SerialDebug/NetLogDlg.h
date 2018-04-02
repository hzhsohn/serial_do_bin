#pragma once

#ifndef __CNetLogDlg__H__

#include "resource.h"
#include "afxwin.h"
#include "NetLogDlgMenuEdit.h"

// CNetLogDlg �Ի���

class CNetLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNetLogDlg)

public:
	CNetLogDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNetLogDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void setlog(CString buf);

	DWORD _dwLogLine; //��־����
	CString _sLog;
	CNetLogDlgMenuEdit _txtEventLog;

	LRESULT OnEditClearCallback(WPARAM wParam, LPARAM lParam);
	CRect CNetLogDlg::MoveFrm(int top,int left,CRect winRect);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};


#define __CNetLogDlg__H__
#endif