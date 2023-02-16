#pragma once
#include "BBox.h"

class BSkyObj : public BBox
{
public:
	ID3D11ShaderResourceView* m_pTextureSkySRV[6];

public:
	void BSkyObj::SetMatrix(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj);

public:
	BSkyObj();
	~BSkyObj();
};

