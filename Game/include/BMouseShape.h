#pragma once
#include "BPlane.h"
#include "K_Plane.h"

/*마우스모양 클래스*/
class BMouseShape
{
public:
	K_Plane						m_MouseShape;
	POINT						m_MousePoint;
	bool						m_bRenderMouseShape;
	ComPtr<ID3D11BlendState>	m_pAlphaTesting;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	BMouseShape();
	~BMouseShape();
};

