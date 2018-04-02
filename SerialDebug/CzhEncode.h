#pragma once
#ifndef __CzhEncode_H__

class CzhEncode
{
public:
	CzhEncode(void);
	~CzhEncode(void);

	static int urldecode(char *str, int len);
	static char * urlencode(char const *s, int len, int *new_length);
	static CString trPrint16(int len,unsigned char* buf);
};

#define __CzhEncode_H__
#endif
