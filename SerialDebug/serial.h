/********************************************************

sample:::::::::: ����:::::::::::::::::

SERIAL sp;
int nLen;
BYTE btRecv[20];

//�򿪴���
if(0==zhSerialOpen(&sp,dev,115200))
{ printf("open serial success"); }
else
{ printf("open serial fail"); }

//����
nLen=zhSerialSend(&sp,start,sizeof(start))
if(-1!=nLen)
{ printf("send ok,len=%d",nLen); }

//����
nLen=zhSerialRead(&sp,btRecv,sizeof(btRecv));
if(nLen>0)
{
	printf("recvd->%d ",nLen);
}else if(-1==nLen)
{
	printf("disconnect");
}

//�ر�
zhSerialClose(&sp);

*/

#ifndef __ZH_SERIAL_MAGE_H__
#define __ZH_SERIAL_MAGE_H__
/*////////////////////////////////////////////////////////////*/
#ifdef  __cplusplus
extern "C" {
#endif


#ifdef _WIN32
	#include <windows.h> 
	#include <tchar.h>
#else
	#include     <stdio.h>      /*��׼�����������*/
	#include     <stdlib.h>     /*��׼�����ⶨ��*/
	#include     <unistd.h>     /*Unix��׼��������*/
	#include     <sys/types.h>  /**/
	#include     <sys/stat.h>   /**/
	#include     <fcntl.h>      /*�ļ����ƶ���*/
	#include     <termios.h>    /*PPSIX�ն˿��ƶ���*/
	#include     <errno.h>      /*����Ŷ���*/
	#include     <sys/ioctl.h> //ioctl
#endif
#include <stdio.h>
#include <memory.h>

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifdef _WIN32
	typedef struct _TzhSerial{
	  char bOpen;
	  char szComName[8];      // �˿�����
	  int nbaudrate;
	  HANDLE hCom;            // �˿ڴ򿪾��
	  OVERLAPPED overlappedR; // ��ȡ�ṹ��
	  OVERLAPPED overlappedW; // ���ͽṹ��
	} TzhSerial;
	
	int SetDev(TzhSerial* sport);
	int OpenDev(TzhSerial* sport, char* szComName, DWORD baudrate);
	DWORD SendDevBuf( TzhSerial* sport, BYTE *btBuf, DWORD dwBytesWritten);
	DWORD ReadDevBuf( TzhSerial* sport, BYTE *btBuf, DWORD dwBytesRead);
	void CloseDev(TzhSerial* sport);
	int CheckStatus(TzhSerial* sport);

	#define SERIAL		TzhSerial
	/* ����0���ӳɹ� */
	#define zhSerialOpen	OpenDev
	#define zhSerialSend	SendDevBuf
	#define zhSerialRead	ReadDevBuf
	#define zhSerialClose	CloseDev
	/* ����1�������� 0���ڲ����� */
	#define zhSerialCheck	CheckStatus
#else

	#define TIMEOUT_SEC(buflen,baud) (buflen*20/baud+2)
	#define TIMEOUT_USEC 0

	typedef struct _TzhSerial{
	  char bOpen;
	  char szComName[26]; //serial info
	  int hCom;   //handle
	  int nbaudrate;
	} TzhSerial;
	int init_serial (int fd,int nSpeed, int nBits, char nEvent, int nStop);
	int OpenDev(TzhSerial* sport,char *Dev ,int nbaudrate);
	int WriteDev(TzhSerial* sport,unsigned char *szBuf,int nBufSize);
	int ReadDev(TzhSerial* sport,unsigned char *szBuf,int nBufSize);
	void CloseDev(TzhSerial* sport);

	#define SERIAL		TzhSerial
	/* ����0���ӳɹ� */
	#define zhSerialOpen	OpenDev
	#define zhSerialSend	WriteDev
	#define zhSerialRead	ReadDev
	#define zhSerialClose	CloseDev
	/* ����1�������� 0���ڲ����� */
	#define zhSerialCheck	CheckStatus
#endif
/*////////////////////////////////////////////////////////////*/

#ifdef  __cplusplus
}
#endif

#endif // 

