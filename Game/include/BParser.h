#pragma once
#include "BStd.h"

enum SET_DATA_TYPES
{
	NULL_DATA = 0,
	INT_DATA,
	FLOAT_DATA,
	STRING_DATA,
	VECTOR_DATA,
	POS_DATA,
	TEX_DATA,
	COLOR_DATA,
	ANIPOS_DATA,
	ANIROT_DATA,
	ANISCL_DATA,
};

struct BAnimTrack
{
	int iTick;
	D3DXQUATERNION qRotate;
	D3DXVECTOR3 vVector;
	BAnimTrack* pNext;
	BAnimTrack* pPrev;
};

class BParser
{
public:
	FILE*		m_pStream;		//ase파일에 접근하기 위한 변수
	fpos_t		m_FilePosition; //파일 위치 지시자
	TCHAR m_pBuffer[256];
	TCHAR m_pString[256];
	int m_iData;


public:
	bool OpenStream(const TCHAR* pFileName);									 // 파일 실행 함수
	int GetSearchStringArray(TCHAR** pObjectType, int iNumObjType, bool bLoop);	 // 문자열 배열(pObjectType) 검색 함수
	bool CloseStream(TCHAR* pMsg = NULL);
	
	void SaveFilePosition();
	void RestoreFilePosition();
	TCHAR* GetNextTokenString();
	TCHAR* GetNextLine();
	bool GetData(TCHAR* pString = NULL, VOID* pData = NULL, INT DataType = NULL_DATA);
	bool GetData(VOID* pData, INT DataType);


public:
	BParser();
	~BParser();
};

