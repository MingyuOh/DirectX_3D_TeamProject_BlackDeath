#include "BRenderPlaneHeight.h"

bool BRenderPlaneHeight::Create(ID3D11Device* pd3dDevice, const TCHAR* pLoadShaderFile, FLOAT Width, FLOAT Height)
{
	m_pPlaneShape = make_shared<BNPlane>();
	m_pMiniMapRT = make_shared<BDxRTHeight>();
	TCHAR* NullTex = NULL;
	m_pPlaneShape->Create(pd3dDevice, pLoadShaderFile, NullTex);
	m_pMiniMapRT->Create(pd3dDevice, DXGI_FORMAT_R8G8B8A8_UNORM, Width, Height);

	DWORD dwWidth = m_pMiniMapRT->m_TexDesc.Width;
	DWORD dwHeight = m_pMiniMapRT->m_TexDesc.Height;
	TVector3 vUpVector(0.0f, 1.0f, 0.0f);
	SetViewMatrix(TVector3(0.0f, 100.0f, 0.0f), TVector3(0.0f, 0.0f, 1.0f), vUpVector);
	SetProjMatrix(PI * 0.25f, (float)dwWidth / (float)dwHeight, 0.1f, 1000.0f);

	return true;
}

BRenderPlaneHeight::BRenderPlaneHeight()
{
}


BRenderPlaneHeight::~BRenderPlaneHeight()
{
}
