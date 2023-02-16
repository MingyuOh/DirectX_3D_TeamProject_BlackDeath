#pragma once
#include "BBox.h"

class BSkyObj : public BBox
{
public:
	ID3D11ShaderResourceView* m_pTextureSkySRV[6];

public:
	void BSkyObj::SetMatrix(TMatrix World, TMatrix View, TMatrix Proj);

public:
	BSkyObj();
	~BSkyObj();
};

