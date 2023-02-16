#pragma once


#include "LACoreLinkStd.h"


#ifdef TOOL_MODE

#include "LAGlobalFunc.h"
#define  S_RELEASE SAFE_RELEASE 

	static void ErrorQuit(TCHAR *msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(TCHAR*)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (TCHAR*)lpMsgBuf, msg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		exit(-1);
	}

#else
	#include "BStd.h"
#endif

//별도로 추가
#ifndef SAFE_ZERO_POINT
#define SAFE_ZERO_POINT(A)				{ A = NULL; }
#endif

#ifndef S_DEL
#define S_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 