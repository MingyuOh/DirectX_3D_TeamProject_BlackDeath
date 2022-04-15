#pragma once
#include "BObject.h"


class BSphere : public BObject
{
public:
	vector<D3DXVECTOR3> m_VertexArray;
	UINT m_Slices;
	double m_fRadius;
	D3DXVECTOR3 m_vCenter;

public:
	virtual HRESULT CreateVertexBuffer();		// 버텍스 버퍼 생성
	bool UpdateQuaternion();
	virtual bool PostRender(ID3D11DeviceContext* pContext); 

public:
	BSphere();
	~BSphere();
};

