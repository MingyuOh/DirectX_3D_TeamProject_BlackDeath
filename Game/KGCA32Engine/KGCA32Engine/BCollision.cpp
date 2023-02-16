#include "BCollision.h"

BOOL BCollision::CheckSphereInSphere(TVector3 vSphereCenter, 
	TVector3 vOtherSphereCenter, float vSphereRadius, float vSphereOherRadius)
{
	float fDistance;
	TVector3 vDiff;

	vDiff = vSphereCenter - vOtherSphereCenter;
	fDistance = D3DXVec3Length(&vDiff);

	if (fDistance <= (vSphereRadius + vSphereOherRadius))
		return TRUE;
	return FALSE;
}

TVector3 BCollision::SphereInSphereOppositeDir(TVector3 vFrontSphereCenter, 
	TVector3 vBackSphereCenter, float vFrontSphereRadius, float vBackSphereRadius)
{
	TVector3 vDir;
	vDir = (vFrontSphereCenter - vBackSphereCenter);
	float fDistance = (vFrontSphereRadius + vBackSphereRadius) - D3DXVec3Length(&vDir);
	D3DXVec3Normalize(&vDir, &vDir);
	vDir = vDir * fDistance;
	return vDir;
}

bool BCollision::SlipDetection(TVector3& vCurrentPos, TVector3& vBeforePos, float fSpeed)
{
	TVector3 vSlip = vBeforePos - vCurrentPos;
	vCurrentPos = vCurrentPos + (vSlip * fSpeed);
	return true;
}

BCollision::BCollision()
{
}


BCollision::~BCollision()
{
}
