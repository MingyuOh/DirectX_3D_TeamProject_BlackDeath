#pragma once
#include "BMap.h"

class BNMap : public BMap
{
public:
	ID3D11VertexShader*		m_pNoneVS; // 버텍스 쉐이더

public:
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
	void SetMatrix(TMatrix* World, TMatrix* View, TMatrix* Proj);
	bool RenderNoneClip(ID3D11DeviceContext* pContext);
public:
	BNMap();
	~BNMap();
};

