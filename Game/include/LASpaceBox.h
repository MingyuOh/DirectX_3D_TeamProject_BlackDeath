#pragma once

#include "LAGlobalFuncLink.h"

class LASpaceBox : public SPACE_BOX
{
public:
	D3DXVECTOR3 vUpdateMin;
	D3DXVECTOR3 vUpdateMax;
public:
	void UpDate(D3DXMATRIX& matDest);
	void UpDateMinMax(std::vector<PNCT_VECTOR>& vertexList);
public:
	LASpaceBox();
	virtual~LASpaceBox();
};

