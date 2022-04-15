#pragma once
#include "BObject.h"

class BCollision
{
public:
	//--------------------------------------------------------------------------------------
	// �� vs �� �浹
	//--------------------------------------------------------------------------------------
	BOOL			CheckSphereInSphere(D3DXVECTOR3 vSphereCenter, D3DXVECTOR3 vOherSphereCenter, float vSphereRadius, float vSphereOherRadius);
	//--------------------------------------------------------------------------------------
	// �� vs �� �浹�� �浹�� ���� ã�Ƽ� �ݴ���� �𷺼� ��ȯ
	//--------------------------------------------------------------------------------------
	D3DXVECTOR3		SphereInSphereOppositeDir(D3DXVECTOR3 vFrontSphereCenter, D3DXVECTOR3 vBackSphereCenter, float vFrontSphereRadius, float vBackSphereRadius);
	//--------------------------------------------------------------------------------------
	// ���� �̲���
	//--------------------------------------------------------------------------------------
	bool			SlipDetection(D3DXVECTOR3& vCurrentPos, D3DXVECTOR3& vBeforePos, float fSpeed);
public:
	BCollision();
	virtual ~BCollision();
};

