#pragma once
#include "BObject.h"

class BCollision
{
public:
	//--------------------------------------------------------------------------------------
	// 구 vs 구 충돌
	//--------------------------------------------------------------------------------------
	BOOL CheckSphereInSphere(TVector3 vSphereCenter, 
		TVector3 vOtherSphereCenter, float vSphereRadius, float vSphereOherRadius);
	//--------------------------------------------------------------------------------------
	// 구 vs 구 충돌시 충돌된 지점 찾아서 반대 방향 반환
	//--------------------------------------------------------------------------------------
	TVector3 SphereInSphereOppositeDir(TVector3 vFrontSphereCenter, 
		TVector3 vBackSphereCenter, float vFrontSphereRadius, float vBackSphereRadius);
	//--------------------------------------------------------------------------------------
	// 벽면 미끌림
	//--------------------------------------------------------------------------------------
	bool SlipDetection(TVector3& vCurrentPos, TVector3& vBeforePos, float fSpeed);
public:
	BCollision();
	virtual ~BCollision();
};

