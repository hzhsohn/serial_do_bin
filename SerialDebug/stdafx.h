
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��
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
//��ҳ
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