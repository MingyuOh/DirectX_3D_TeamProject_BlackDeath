#pragma once
#include "BObject.h"

class BPlane : public BObject
{
public:
	float fA, fB, fC, fD; // 평면의 방정식용 변수

public:
	virtual HRESULT CreateVertexBuffer();		// 버텍스 버퍼 생성
	virtual HRESULT CreateIndexBuffer();		// 인덱스 버퍼 생성

	/*프로스텀용 함수*/
	bool CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2);	// 평면의 방정식을 얻는 함수
	bool CreatePlane(D3DXVECTOR3 vNormal, D3DXVECTOR3 v0);				// 평면의 방정식을 얻는 함수
	void UpdateVertexMinMax();
public:
	BPlane();
	~BPlane();
};

