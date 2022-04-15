#pragma once
#include "BObject.h"

class BCollision
{
public:
	//--------------------------------------------------------------------------------------
	// �� vs �� �浹
	//--------------------------------------------------------------------------------------
	BOOL CheckSphereInSphere(TVector3 vSphereCenter, 
		TVector3 vOtherSphereCenter, float vSphereRadius, float vSphereOherRadius);
	//--------------------------------------------------------------------------------------
	// �� vs �� �浹�� �浹�� ���� ã�Ƽ� �ݴ� ���� ��ȯ
	//--------------------------------------------------------------------------------------
	TVector3 SphereInSphereOppositeDir(TVector3 vFrontSphereCenter, 
		TVector3 vBackSphereCenter, float vFrontSphereRadius, float vBackSphereRadius);
	//--------------------------------------------------------------------------------------
	// ���� �̲���
	//--------------------------------------------------------------------------------------
	bool SlipDetection(TVector3& vCurrentPos, TVector3& vBeforePos, float fSpeed);
public:
	BCollision();
	virtual ~BCollision();
};

