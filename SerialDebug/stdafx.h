
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
#include <afxdhtml.h>




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


///////////
//网页
#define WEB_URL				_T("http://www.hx-kong.com")
#define WEB_URL_IP			_T("http://www.hx-kong.com/tool/serial_debug/myip.php")
#define WEB_URL_WEBSERIAL	_T("http://www.hx-kong.com/tool/serial_debug/web-serial.php")
#define WEB_URL_ASSISTTOOL	_T("http://www.hx-kong.com/tool/serial_debug/assist.php")
#define WEB_URL_INFO		_T("http://www.hx-kong.com/tool/serial_debug/info.php")
#define WEB_URL_UPDATE		_T("http://www.hx-kong.com/tool/serial_debug/update.php")

#define VERSION_URL			_T("http://www.hx-kong.com/tool/serial_debug/version.interface.php")
#define CURRENT_VERSION		_T("1.1.0.1")


void setMainLog(CString buf);
void setNetLog(CString buf);
void RecvPrint(int len,char*buf);
void addTx(int n);
void addRx(int n);
void setConnectEnable(BOOL b);