
// SerialDebug.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSerialDebugApp:
// �йش����ʵ�֣������ SerialDebug.cpp
//

class CSerialDebugApp : public CWinApp
{
public:
	CSerialDebugApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSerialDebugApp theApp;