#pragma once

#include "LAGlobalFuncLink.h"

class LASpaceSphere : public SPACE_SPHERE
{
public:
	void UpDate(D3DXMATRIX& matDest);
public:
	LASpaceSphere();
	virtual ~LASpaceSphere();
};

