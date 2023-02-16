#pragma once

#include "CoreFuncLink.h"
#include "LACharacterStruct.h"

class LASpaceSphere2 : public SPACE_SPHERE
{
public:
	void UpDate(TMatrix& matDest);
public:
	LASpaceSphere2();
	virtual ~LASpaceSphere2();
};

