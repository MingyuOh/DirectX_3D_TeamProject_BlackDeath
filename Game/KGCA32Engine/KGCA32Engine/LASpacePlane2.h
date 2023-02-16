#pragma once

#include "CoreFuncLink.h"
#include "LACharacterStruct.h"

class LASpacePlane2 : public SPACE_PLANE
{
public:
	bool CreatePlane(TVector3 v0, TVector3 v1, TVector3 v2);
	bool CreatePlane(TVector3 vNormal, TVector3 v0);
	void Normalize();
public:
	LASpacePlane2();
	virtual ~LASpacePlane2();
};

