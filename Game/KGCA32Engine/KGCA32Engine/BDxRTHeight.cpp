#include "BDxRTHeight.h"

HRESULT BDxRTHeight::Create(ID3D11Device* pd3dDevice, DXGI_FORMAT format, FLOAT Width, FLOAT Height)
{
	HRESULT hr = S_OK;
	Set(pd3dDevice, 0, 0, Width, Height, 0.0f, 1.0f);

	m_TexDesc.Width = (UINT)Width;
	m_TexDesc.Height = (UINT)Height;
	m_TexDesc.MipLevels = 1;
	m_TexDesc.Format = format;
	m_TexDesc.SampleDesc.Count = 1;
	m_TexDesc.SampleDesc.Quality = 0;
	m_TexDesc.Usage = D3D11_USAGE_DEFAULT;
	m_TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_TexDesc.CPUAccessFlags = 0;
	m_TexDesc.MiscFlags = 0;
	m_TexDesc.ArraySize = 1;

	hr = pd3dDevice->CreateTexture2D(&m_TexDesc, NULL, m_pTexture.GetAddressOf());
	hr = pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), NULL, m_pSRV.GetAddressOf());
	hr = pd3dDevice->CreateRenderTargetView(m_pTexture.Get(), NULL, m_pRenderTargetView.GetAddressOf());

	m_TexDesc.Usage = D3D11_USAGE_STAGING;
	m_TexDesc.BindFlags = 0;
	m_TexDesc.MiscFlags = 0;
	m_TexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	hr = pd3dDevice->CreateTexture2D(&m_TexDesc, NULL, &m_pHeightTexture);

	//백버퍼의 크기가 변경되면 깊이스텐실뷰도 재생성
	if (FAILED(hr = UpdateDepthStencilView(pd3dDevice, (UINT)Width, (UINT)Height)))
	{
		return hr;
	}
	return hr;
}

// 메인 맵의 높이가 변경되면 넘겨줘야 합니다.
// 맵 생성시에도 호출되어야 합니다.
// 현재 물의 색상은 맵의 전체 높이크기에 비례하여 생성되게 되어 있습니다.
// 물의 높이에 따른 비교가 필요합니다.
HRESULT BDxRTHeight::DrawQuadTreeHeightTex(ID3D11Device* pd3dDevice, ID3D11DeviceContext* m_pContext, BMapTileRender* m_pMap)
{
	SetRootNodeYPosition(m_pMap->m_pRootNode, &m_pMap->m_pRootNode->m_tBox.vMax.y, &m_pMap->m_pRootNode->m_tBox.vMin.y);
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(m_pContext->Map((ID3D11Resource*)m_pHeightTexture.Get(), 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		/*맵의 크기변경을 제한합니다.*/
		/*정수형 이외의 크기변경시 높이맵 제작에 문제가 생길 수 있습니다.*/
		int iChangePoint = m_pMap->m_pRootNode->m_tBox.fExtent[0];
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		float fRatio = 255.0f / (m_pMap->m_pRootNode->m_tBox.vMax.y - m_pMap->m_pRootNode->m_tBox.vMin.y);
		float fMin = m_pMap->m_pRootNode->m_tBox.vMin.y;
		DrawNodeHeightTex(m_pMap->m_pRootNode, pDestBytes, iChangePoint, m_pMap->m_pMap->m_fCellDistance, MappedFaceDest.RowPitch, fRatio, fMin);
	}
	m_pContext->Unmap(m_pHeightTexture.Get(), 0);
	m_pContext->CopyResource(m_pTexture.Get(), m_pHeightTexture.Get());
	FAILED(hr = pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), NULL, m_pSRV.GetAddressOf()));
	return hr;
}

// 루트노드의 MaxY값을 얻기위한 임시함수
void BDxRTHeight::SetRootNodeYPosition(BNode* pNode, float* MaxY, float* MinY)
{
	if (pNode->m_pChild[0] != NULL)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			SetRootNodeYPosition(pNode->m_pChild[iNode], MaxY, MinY);
		}
	}
	else
	{
		if (pNode->m_tBox.vMax.y > *MaxY)
		{
			*MaxY = pNode->m_tBox.vMax.y;
		}
		if (pNode->m_tBox.vMin.y < *MinY)
		{
			*MinY = pNode->m_tBox.vMin.y;
		}
	}
	return;
}

void BDxRTHeight::DrawNodeHeightTex(BNode* pNode, BYTE* pDestBytes, int iChangePoint, float fCellDistance, UINT RowPitch, float fRatio, float fMin)
{
	if (pNode->m_pChild[0] != NULL)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			DrawNodeHeightTex(pNode->m_pChild[iNode], pDestBytes, iChangePoint, fCellDistance, RowPitch, fRatio, fMin);
		}
	}
	else
	{
		int iRowStart = ((pNode->m_tBox.vMax.z - iChangePoint) * -1.0f) / fCellDistance;
		int iColStart = (pNode->m_tBox.vMin.x + iChangePoint) / fCellDistance;
		int iRowEnd = ((pNode->m_tBox.vMin.z - iChangePoint) * -1.0f) / fCellDistance + 1;
		int iColEnd = (pNode->m_tBox.vMax.x + iChangePoint) / fCellDistance + 1;
		int iVerCnt = 0;
		for (int y = iRowStart; y < iRowEnd; y++)
		{
			BYTE* pDest = pDestBytes + (UINT)(RowPitch * y);
			pDest = pDest + (UINT)(4 * iColStart);
			for (int x = iColStart; x < iColEnd; x++)
			{
				float fHeight = (pNode->m_VertexList[iVerCnt].Position.y - fMin) * fRatio;
				*pDest++ = fHeight;
				*pDest++ = fHeight;
				*pDest++ = fHeight;
				*pDest++ = fHeight;
				iVerCnt++;
			}
		}
	}
}

HRESULT BDxRTHeight::DrawHeightTex(ID3D11Device* pd3dDevice, ID3D11DeviceContext* m_pContext, BMap* m_pMap)
{
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if (SUCCEEDED(m_pContext->Map((ID3D11Resource*)m_pHeightTexture.Get(), 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		int Row = m_TexDesc.Height;
		int Col = m_TexDesc.Width;
		float Ratio = 255.0f / m_pMap->m_tBox.vMax.y;
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		for (int y = 0; y < Row; y++)
		{
			BYTE* pDest = pDestBytes;
			for (int x = 0; x < Col; x++)
			{
				float fDest = m_pMap->m_VertexList[y * Row + x].Position.y * Ratio;
				*pDest++ = fDest;
				*pDest++ = fDest;
				*pDest++ = fDest;
				*pDest++ = fDest;
			}
			pDestBytes = pDestBytes + MappedFaceDest.RowPitch;
		}
	}
	m_pContext->Unmap(m_pHeightTexture.Get(), 0);
	m_pContext->CopyResource(m_pTexture.Get(), m_pHeightTexture.Get());
	FAILED(hr = pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), NULL, m_pSRV.GetAddressOf()));
	return hr;
}

BDxRTHeight::BDxRTHeight()
{
}


BDxRTHeight::~BDxRTHeight()
{
}
