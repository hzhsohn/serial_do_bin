// NetLogDlgMenuEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "MainDlgMenuEdit.h"

#define MES_UNDO        _T("&U撤消")
#define MES_CUT         _T("&T剪切")
#define MES_COPY        _T("复制(&C)")
#define MES_PASTE       _T("&P粘贴")
#define MES_DELETE      _T("&D删除")
#define MES_SELECTALL   _T("全选(&A)")
#define ME_SELECTALL    WM_USER + 0x7000

// CNetLogDlgMenuEdit
IMPLEMENT_DYNAMIC(CMainDlgMenuEdit, CEdit)
	
BEGIN_MESSAGE_MAP(CMainDlgMenuEdit, CEdit)
	ON_WM_CONTEXTMENU()


END_MESSAGE_MAP()

CMainDlgMenuEdit::CMainDlgMenuEdit()
{
	_targetHwnd=NULL;
}

CMainDlgMenuEdit::~CMainDlgMenuEdit()
{}

// CNetLogDlgMenuEdit 消息处理程序
void CMainDlgMenuEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
    SetFocus();
    CMenu menu;
    menu.CreatePopupMenu();
    BOOL bReadOnly = GetStyle() & ES_READONLY;
    DWORD flags = CanUndo() && !bReadOnly ? 0 : MF_GRAYED;
  
	/*menu.InsertMenu(0, MF_BYPOSITION | flags, EM_UNDO,
        MES_UNDO);
		*/
	//插入空白线
    menu.InsertMenu(1, MF_BYPOSITION | MF_SEPARATOR);

    DWORD sel = GetSel();
    flags = LOWORD(sel) == HIWORD(sel) ? MF_GRAYED : 0;
    menu.InsertMenu(2, MF_BYPOSITION | flags, WM_COPY,
        MES_COPY);

   /* flags = (flags == MF_GRAYED || bReadOnly) ? MF_GRAYED : 0;
    menu.InsertMenu(3, MF_BYPOSITION | flags, WM_CUT,
        MES_CUT);
    menu.InsertMenu(4, MF_BYPOSITION | flags, WM_CLEAR,
        MES_DELETE);

    flags = IsClipboardFormatAvailable(CF_TEXT) &&
        !bReadOnly ? 0 : MF_GRAYED;
    menu.InsertMenu(5, MF_BYPOSITION | flags, WM_PASTE,
        MES_PASTE);*/

	//插入空白线
    menu.InsertMenu(6, MF_BYPOSITION | MF_SEPARATOR);

    int len = GetWindowTextLength();
    flags = (!len || (LOWORD(sel) == 0 && HIWORD(sel) ==
        len)) ? MF_GRAYED : 0;
    menu.InsertMenu(7, MF_BYPOSITION | flags, ME_SELECTALL,
        MES_SELECTALL);

	//插入空白线
    menu.InsertMenu(8, MF_BYPOSITION | MF_SEPARATOR);

	//清空
	menu.InsertMenu(9, MF_BYPOSITION , 100,_T("清空(&E)"));

    menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON |
        TPM_RIGHTBUTTON, point.x, point.y, this);
}


BOOL CMainDlgMenuEdit::OnCommand(WPARAM wParam, LPARAM lParam)
{
    switch (LOWORD(wParam))
    {
    case EM_UNDO:
    case WM_CUT:
    case WM_COPY:
    case WM_CLEAR:
    case WM_PASTE:
        return SendMessage(LOWORD(wParam));
    case ME_SELECTALL:
        return SendMessage (EM_SETSEL, 0, -1);
	case 100://我是自定义消息
		::SendMessage(_targetHwnd,WM_MYEDIT_MESSAGE,wParam,lParam);
		return TRUE;
    default:
        return CEdit::OnCommand(wParam, lParam);
    }
}


void CMainDlgMenuEdit::setHwnd(HWND h)
{
	_targetHwnd=h;
}