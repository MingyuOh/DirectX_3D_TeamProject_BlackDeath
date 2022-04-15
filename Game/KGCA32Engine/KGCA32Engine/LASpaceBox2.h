#pragma once

#include "CoreFuncLink.h"
#include "LACharacterStruct.h"

class LASpaceBox2 : public SPACE_BOX
{
public:
	TVector3 m_vUpdateMin;
	TVector3 m_vUpdateMax;
public:
	void UpDate(TMatrix& matDest);
	void UpDateMinMax(std::vector<PNCT_VECTOR>& vertexList);
public:
	LASpaceBox2();
	virtual~LASpaceBox2();
};

