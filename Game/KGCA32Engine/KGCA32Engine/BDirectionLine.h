#pragma once
#include "BLine.h"

class BDirectionLine : public BLine
{
public:
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	bool PreRender(ID3D11DeviceContext* pContext);

public:
	BDirectionLine();
	~BDirectionLine();
};

