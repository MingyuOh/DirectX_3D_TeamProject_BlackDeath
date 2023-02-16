#pragma once
#include "BObject.h"

class BLine : public BObject
{
public:
	vector<PNCT_Vertex>	m_LineVertexList;

public:
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
	HRESULT CreateVertexBuffer();		// 버텍스 버퍼 생성
	HRESULT CreateIndexBuffer();		// 인덱스 버퍼 생성

	bool SetLineStartToMove(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 Move, D3DXVECTOR4 vColor);
	bool SetLineStartToEnd(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor);
public:
	BLine();
	~BLine();
};

