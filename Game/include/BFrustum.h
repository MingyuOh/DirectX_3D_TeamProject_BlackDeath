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
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;

	D3DXVECTOR3 m_vFrustum[8]; 
	D3DXMATRIX m_matViewProj;
	BPlane m_Plane[6];
	BBox* m_pBoxShape;

public:
	bool CreateFrustum(ID3D11Device* m_pd3dDevice, const TCHAR* pShaderFile);
	BOOL CheckPoint(D3DXVECTOR3* pv);
	BOOL CheckSphere(BSphere* pSphere);
	BOOL CheckOBB(B_BOX* pBox);
	T_POSITION CheckPositionOBBInPlane(B_BOX* pBox);
	bool FrustRender(ID3D11DeviceContext* pContext);


public:
	BFrustum();
	virtual ~BFrustum();
};

