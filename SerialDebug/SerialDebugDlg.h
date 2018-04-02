
// SerialDebugDlg.h : ͷ�ļ�
//

#pragma once

#include "serial.h"
#include "afxwin.h"

#include <string>
#include <vector>

#include "NetLogDlg.h"
#include <io.h>
#include "afxcmn.h"
#include "MainDlgMenuEdit.h"

using namespace std;

//�����ļ��ṹ
typedef enum _EzhFileAttrib{
	ezhFileAttribUnknow,
	ezhFileAttribFile,
	ezhFileAttribDir
}EzhFileAttrib;

// CSerialDebugDlg �Ի���
class CSerialDebugDlg : public CDialogEx
{
// ����
public:
	CSerialDebugDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERIALDEBUG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	
	//-----------------------------
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton3();
	
	
	void trSendStringToHex();
	void trSendHexToString();
	
	EzhFileAttrib returnFileAttrib(int attrib);

	LRESULT OnEditClearCallback(WPARAM wParam, LPARAM lParam);

public:
	bool _bIsOpen;//�Ƿ�򿪴�������
	
	CComboBox _cbPort;
	CComboBox _cbBute;	
	string dev;   //�򿪵Ķ˿�
	int nbrte;    //�򿪵�����	
	int _isShowOpen;	  //�Ƿ��Ѿ���ʾ�������ӳɹ�������

	SERIAL _sp;
	BOOL _isSerialConnect;//�����Ƿ����ӳɹ�
	DWORD dwTime;//COM�ڼ�����ӵ����ʱ��
	
public:
	afx_msg void OnBnClickedButton2();
	void OnTimer(UINT nIDEvent);

	void openSerial();
	void closeSerial();

	/* ����ʧ�ܻظ� -1 ���򷵻ط����ֽ�*/
	int send(unsigned char*buf,int len);
	BOOL isEnable();
	CStatic _lbCurLinkCount;
	int _nCurLinkCount;

	afx_msg void OnBnClickedButton1();
	CProgressCtrl _progress;
	CListCtrl _fileHex16String;
	afx_msg void OnLvnGetdispinfoList1(NMHDR *pNMHDR, LRESULT *pResult);
	//
	CImageList m_ImageList;
	CToolBar m_Toolbar;
	//
	void OnBnClickedButton4();
	void OnBnClickedButton5();
	void OnBnClickedButton6();
	void OnBnClickedButton7();
	void OnBnClickedButton8();
	void OnBnClickedButton9();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

//��ѯ����״̬
BOOL checkStatus();
//��������
void WINAPI SendCmd();