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
								TVector3 vCenter = TVector3(0.0f, 0.0f, 0.0f),
								TVector3 vDirX = TVector3(1.0f, 0.0f, 0.0f),
								TVector3 vDirY = TVector3(0.0f, 1.0f, 0.0f),
								TVector3 vDirZ = TVector3(0.0f, 0.0f, 1.0f));
	void				CreateAABBBox(TVector3 vMax = TVector3(1.0f, 1.0f, 1.0f),
								TVector3 vMin = TVector3(-1.0f, -1.0f, -1.0f));
public:
	Shape();
	virtual ~Shape();
};

