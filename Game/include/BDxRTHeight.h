#pragma once
#include "BDxRT.h"
#include "BMap.h"
#include "BMapTileRender.h"

class BDxRTHeight : public BDxRT
{
public:
	ComPtr<ID3D11Texture2D> m_pHeightTexture;

public:
	HRESULT Create(ID3D11Device* pd3dDevice, DXGI_FORMAT format, FLOAT Width = 1024, FLOAT Height = 1024);
	HRESULT DrawHeightTex(ID3D11Device* pd3dDevice, ID3D11DeviceContext* m_pContext, BMap* m_pMap);
	HRESULT DrawQuadTreeHeightTex(ID3D11Device* pd3dDevice, ID3D11DeviceContext* m_pContext, BMapTileRender* m_pMap);
	void DrawNodeHeightTex(BNode* pNode, BYTE* pDestBytes, int iChangePoint, float fCellDistance, UINT RowPitch, float fRatio, float fMin);
	void SetRootNodeYPosition(BNode* pNode, float* MaxY, float* MinY);
public:
	BDxRTHeight();
	~BDxRTHeight();
};

