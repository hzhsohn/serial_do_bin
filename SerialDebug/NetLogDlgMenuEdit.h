#pragma once

#include "stdafx.h"

// CNetDlgMenuEdit

#define WM_MYMESSAGE 10000

class CNetLogDlgMenuEdit : public CEdit
{
	DECLARE_DYNAMIC(CNetLogDlgMenuEdit)

	
	HWND _targetHwnd;

public:
	CNetLogDlgMenuEdit();
	virtual ~CNetLogDlgMenuEdit();

	
protected:
	DECLARE_MESSAGE_MAP()

public:
	void OnContextMenu(CWnd* pWnd, CPoint point);
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	void setHwnd(HWND h);
};


