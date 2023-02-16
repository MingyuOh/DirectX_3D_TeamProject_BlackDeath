#pragma once

#include "CoreFuncLink.h"
#include "LACharacterStruct.h"

class LASpaceBox2 : public SPACE_BOX
{
public:
	D3DXVECTOR3 m_vUpdateMin;
	D3DXVECTOR3 m_vUpdateMax;
public:
	void UpDate(D3DXMATRIX& matDest);
	void UpDateMinMax(std::vector<PNCT_VECTOR>& vertexList);
public:
	LASpaceBox2();
	virtual~LASpaceBox2();
};

