#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CHtmlView��"
#endif 

// CBrowser Html ��ͼ
#include <afxhtml.h>

class CWebView : public CHtmlView
{
	DECLARE_DYNCREATE(CWebView)

public:
	CWebView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CWebView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


