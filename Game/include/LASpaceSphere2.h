#pragma once

#include "CoreFuncLink.h"
#include "LACharacterStruct.h"

class LASpaceSphere2 : public SPACE_SPHERE
{
public:
	void UpDate(D3DXMATRIX& matDest);
public:
	LASpaceSphere2();
	virtual ~LASpaceSphere2();
};

