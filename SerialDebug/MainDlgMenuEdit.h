#pragma once

#include "stdafx.h"

// CNetDlgMenuEdit

#define WM_MYEDIT_MESSAGE 10001

class CMainDlgMenuEdit : public CEdit
{
	DECLARE_DYNAMIC(CMainDlgMenuEdit)

	
	HWND _targetHwnd;

public:
	CMainDlgMenuEdit();
	virtual ~CMainDlgMenuEdit();

	
protected:
	DECLARE_MESSAGE_MAP()

public:
	void OnContextMenu(CWnd* pWnd, CPoint point);
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	void setHwnd(HWND h);
};


