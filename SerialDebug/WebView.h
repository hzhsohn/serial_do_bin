#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CHtmlView。"
#endif 

// CBrowser Html 视图
#include <afxhtml.h>

class CWebView : public CHtmlView
{
	DECLARE_DYNCREATE(CWebView)

public:
	CWebView();           // 动态创建所使用的受保护的构造函数
	virtual ~CWebView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};


