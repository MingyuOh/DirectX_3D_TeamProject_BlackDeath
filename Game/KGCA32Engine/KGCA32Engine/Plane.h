#pragma once
#include "Shape.h"

class Plane :public Shape
{
public:
	int							m_iVertexType;
	ID3D11ShaderResourceView*   m_pTexSRV[4];
public:
	//--------------------------------------------------------------------------------------
	// 정점 버퍼 생성
	//--------------------------------------------------------------------------------------
	HRESULT			CreateVertexBuffer();
	//--------------------------------------------------------------------------------------
	// 인덱스 버퍼 생성
	//--------------------------------------------------------------------------------------
	HRESULT			CreateIndexBuffer();
	//--------------------------------------------------------------------------------------
	// 텍스처 배열 초기화( 멀티텍스쳐용)
	//--------------------------------------------------------------------------------------
	bool			SetTextureArray();
public:
	Plane();
	virtual ~Plane();
};

