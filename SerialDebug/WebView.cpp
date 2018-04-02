#include "stdafx.h"
#include "WebView.h"

IMPLEMENT_DYNCREATE(CWebView, CHtmlView)

CWebView::CWebView()
{
}

CWebView::~CWebView()
{
}

void CWebView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWebView, CHtmlView)
END_MESSAGE_MAP()


// CBrowser 诊断

#ifdef _DEBUG
void CWebView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CWebView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

// CBrowser 消息处理程序
