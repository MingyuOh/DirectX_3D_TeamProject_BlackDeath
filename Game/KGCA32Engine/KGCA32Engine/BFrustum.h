#pragma once
#include "BStd.h"
#include "BPlane.h"
#include "BSphere.h"
#include "BBox.h"

enum T_POSITION
{
	P_BACK = 0,
	P_FRONT,
	P_ONPLANE,
	P_SPANNING
};

class BFrustum
{
public:
	/*ºä, Åõ¿µÇà·Ä*/
	TMatrix m_matWorld;
	TMatrix m_matView;
	TMatrix m_matProj;

	TVector3 m_vFrustum[8]; 
	TMatrix m_matViewProj;
	BPlane m_Plane[6];
	BBox* m_pBoxShape;

public:
	bool CreateFrustum(ID3D11Device* m_pd3dDevice, const TCHAR* pShaderFile);
	BOOL CheckPoint(TVector3* pv);
	BOOL CheckSphere(BSphere* pSphere);
	BOOL CheckOBB(B_BOX* pBox);
	T_POSITION CheckPositionOBBInPlane(B_BOX* pBox);
	bool FrustRender(ID3D11DeviceContext* pContext);


public:
	BFrustum();
	virtual ~BFrustum();
};

