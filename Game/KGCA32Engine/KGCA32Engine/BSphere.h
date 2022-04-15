#pragma once
#include "BObject.h"


class BSphere : public BObject
{
public:
	vector<TVector3> m_VertexArray;
	UINT m_Slices;
	double m_fRadius;
	TVector3 m_vCenter;

public:
	virtual HRESULT CreateVertexBuffer();		// ���ؽ� ���� ����
	bool UpdateQuaternion();
	virtual bool PostRender(ID3D11DeviceContext* pContext); 

public:
	BSphere();
	~BSphere();
};

