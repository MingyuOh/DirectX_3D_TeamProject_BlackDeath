
//#include "stdafx.h"

#include "LASpacePlane2.h"

#include "CoreStandardLink.h"

bool LASpacePlane2::CreatePlane(TVector3 v0, TVector3 v1, TVector3 v2)
{
	TVector3 vD0 = v1 - v0;
	TVector3 vD2 = v2 - v0;

	TVector3 vNormal;
	D3DXVec3Cross(&vNormal, &vD0, &vD2);
	D3DXVec3Normalize(&vNormal, &vNormal);

	fA = vNormal.x;
	fB = vNormal.y;
	fC = vNormal.z;
	fD = 0.0f;

	float FK, FG, FF, FY;
	FK = fA * v0.x;
	FG = fB * v0.y;
	FF = fC * v0.z;
	FY = -(FK + FG + FF);
	fD = -(fA * v0.x + fB * v0.y + fC * v0.z);

	return true;
}

bool LASpacePlane2::CreatePlane(TVector3 vNormal, TVector3 v0)
{
	D3DXVec3Normalize(&vNormal, &vNormal);
	fA = vNormal.x;
	fB = vNormal.y;
	fC = vNormal.z;
	fD = -((fA * v0.x) + (fB * v0.y) + (fC * v0.z));

	return true;
}

void LASpacePlane2::Normalize()
{
	float fMag = sqrt((fA * fA) + (fB * fB) + (fC * fC));
	fA = fA / fMag;
	fB = fB / fMag;
	fC = fC / fMag;
}

LASpacePlane2::LASpacePlane2()
{
}


LASpacePlane2::~LASpacePlane2()
{
}
