#pragma once

#include "LAGlobalFuncLink.h"


class LASpacePlane : public SPACE_PLANE
{
public:
	bool CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2);
	bool CreatePlane(D3DXVECTOR3 vNormal, D3DXVECTOR3 v0);
	void Normalize();
public:
	LASpacePlane();
	virtual ~LASpacePlane();
};

