#pragma once
#include "BObject.h"
#include "BSphere.h"
#include "BPlane.h"

class Shape : public BObject
{
//--------------------------------------------------------------------------------------
// 충돌 데이터
//--------------------------------------------------------------------------------------
public:
	B_BOX		m_Box;
	BSphere		m_Sphere;
	BPlane		m_Plane;
public:
	void				CreateOOBBox(float fLengthX = 1.0f, float fLengthY = 1.0f, float fLengthZ = 1.0f,
								D3DXVECTOR3 vCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXVECTOR3 vDirX = D3DXVECTOR3(1.0f, 0.0f, 0.0f),
								D3DXVECTOR3 vDirY = D3DXVECTOR3(0.0f, 1.0f, 0.0f),
								D3DXVECTOR3 vDirZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	void				CreateAABBBox(D3DXVECTOR3 vMax = D3DXVECTOR3(1.0f, 1.0f, 1.0f),
								D3DXVECTOR3 vMin = D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
public:
	Shape();
	virtual ~Shape();
};

