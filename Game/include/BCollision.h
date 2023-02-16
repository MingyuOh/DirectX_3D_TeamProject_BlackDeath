#pragma once
#include "BObject.h"

class BCollision
{
public:
	//--------------------------------------------------------------------------------------
	// 구 vs 구 충돌
	//--------------------------------------------------------------------------------------
	BOOL			CheckSphereInSphere(D3DXVECTOR3 vSphereCenter, D3DXVECTOR3 vOherSphereCenter, float vSphereRadius, float vSphereOherRadius);
	//--------------------------------------------------------------------------------------
	// 구 vs 구 충돌시 충돌된 지점 찾아서 반대방향 디렉션 반환
	//--------------------------------------------------------------------------------------
	D3DXVECTOR3		SphereInSphereOppositeDir(D3DXVECTOR3 vFrontSphereCenter, D3DXVECTOR3 vBackSphereCenter, float vFrontSphereRadius, float vBackSphereRadius);
	//--------------------------------------------------------------------------------------
	// 벽면 미끌림
	//--------------------------------------------------------------------------------------
	bool			SlipDetection(D3DXVECTOR3& vCurrentPos, D3DXVECTOR3& vBeforePos, float fSpeed);
public:
	BCollision();
	virtual ~BCollision();
};

