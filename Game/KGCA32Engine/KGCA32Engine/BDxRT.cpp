#include "BDxRT.h"

void BDxRT::Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth, FLOAT MaxDepth)
{
	m_vp.TopLeftX = TopLeftX;
	m_vp.TopLeftY = TopLeftY;
	m_vp.Width = Width;
	m_vp.Height = Height;
	m_vp.MinDepth = MinDepth;
	m_vp.MaxDepth = MaxDepth;
	D3DXMatrixPerspectiveFovLH(&m_matProj, (float)PI * 0.25f, Width / Height, 0.1f, 1000.0f);
}

HRESULT BDxRT::Create(ID3D11Device* pd3dDevice, FLOAT Width, FLOAT Height)
{
	HRESULT hr = S_OK;
	Set(pd3dDevice, 0, 0, Width, Height, 0.0f, 1.0f);

	m_TexDesc.Width = (UINT)Width;
	m_TexDesc.Height = (UINT)Height;
	m_TexDesc.MipLevels = 1;
	m_TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_TexDesc.SampleDesc.Count = 1;
	m_TexDesc.SampleDesc.Quality = 0;
	m_TexDesc.Usage = D3D11_USAGE_DEFAULT;
	m_TexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_TexDesc.CPUAccessFlags = 0;
	m_TexDesc.MiscFlags = 0;
	m_TexDesc.ArraySize = 1;

	if (FAILED(hr = pd3dDevice->CreateTexture2D(&m_TexDesc, NULL, m_pTexture.GetAddressOf())))
	{
		return hr;
	}
	if (FAILED(hr = pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), NULL, m_pSRV.GetAddressOf()))) // 
	{
		return hr;
	}
	if (FAILED(hr = pd3dDevice->CreateRenderTargetView(m_pTexture.Get(), NULL, m_pRenderTargetView.GetAddressOf()))) // 랜더타겟뷰 객체를 생성한다.
	{
		return hr;
	}

	//백버퍼의 크기가 변경되면 깊이스텐실뷰도 재생성
	if (FAILED(hr = UpdateDepthStencilView(pd3dDevice, (UINT)Width, (UINT)Height)))
	{
		return hr;
	}
	return hr;
}

HRESULT BDxRT::UpdateDepthStencilView(ID3D11Device* pDevice, UINT Width, UINT Height)
{
	HRESULT hr;
	if (m_pDepthStencilView != nullptr)
	{
		m_pDepthStencilView.ReleaseAndGetAddressOf();
	}

	m_vp.Width = Width;
	m_vp.Height = Height;

	ComPtr<ID3D11Texture2D> pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC DescDepth;
	DescDepth.Width = Width;
	DescDepth.Height = Height;
	DescDepth.MipLevels = 1;
	DescDepth.ArraySize = 1;
	DescDepth.Format = m_DSFormat; // = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DescDepth.SampleDesc.Count = 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.Usage = D3D11_USAGE_DEFAULT;
	DescDepth.CPUAccessFlags = 0;
	DescDepth.MiscFlags = 0;

	/*백버퍼 깊이 및 스텐실 버퍼 생성*/
	if (DescDepth.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	else // D32로 전용 깊이맵 생성
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	if (FAILED(hr = pDevice->CreateTexture2D(&DescDepth, NULL, &pDSTexture)))
	{
		return hr;
	}

	////////////// 쉐이더 리소스 생성 : 깊이 맵 쉐도우에서 사용 ////////////////
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	switch (DescDepth.Format)
	{
	case DXGI_FORMAT_R32_TYPELESS: // 전용 깊이맵일 경우
	{
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		break;
	}
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	{
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	}
	}
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	if (FAILED(hr = pDevice->CreateDepthStencilView(pDSTexture.Get(), &dsvDesc, &m_pDepthStencilView)))
	{
		return hr;
	}

	/*깊이맵일 경우*/
	if (DescDepth.Format ==DXGI_FORMAT_R32_TYPELESS)
	{
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		pDevice->CreateShaderResourceView(pDSTexture.Get(), &srvDesc, &m_pDsvSRV);
	}
	m_DepthStencilDesc = dsvDesc;
	return hr;
}

bool BDxRT::Begin(ID3D11DeviceContext* pContext,D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView,
					TVector4 vColor, bool bTargetClear, bool bDepthClear, bool bStencilClear)
{
	m_nViewPorts = 1;
	m_vpOld = OldViewPort;
	m_pOldDSV = OldDepthStencilView;
	m_pOldRTV = OldRenderTargetView;

	ID3D11RenderTargetView* pNullRTV = NULL;
	pContext->OMSetRenderTargets(1, &pNullRTV, NULL);
	pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	if (bTargetClear)
		pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), (FLOAT*)&vColor);
	if (bDepthClear)
		pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);
	if (bStencilClear)
		pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_STENCIL, 1.0, 0);

	pContext->RSSetViewports(1, &m_vp); // 뷰포트를 m_vp로 설정한다.
	return true;
}

void BDxRT::Apply(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV, bool bDepthClear, bool bStencilClear)
{
	/*초기화*/
	ID3D11RenderTargetView* pNullRTV = NULL;
	pContext->OMSetRenderTargets(1, &pNullRTV, NULL); 

	if (pRTV != NULL)
	{
		if (m_pDepthStencilView != NULL)
			pContext->OMSetRenderTargets(1, &pRTV, pDSV);
		else
			pContext->OMSetRenderTargets(1, &pRTV, m_pDepthStencilView.Get());
	}
	else
	{
		if (pDSV != NULL)
			pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), pDSV);
		else
			pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	}
	if (bDepthClear)
	{
		if (pDSV != NULL)
			pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);
		else
			pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);
	}
	if (bStencilClear)
	{
		if (pDSV != NULL)
			pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_STENCIL, 1.0, 0);
		else
			pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_STENCIL, 1.0, 0);
	}
	pContext->RSSetViewports(1, &m_vp);
}

bool BDxRT::End(ID3D11DeviceContext* pContext)
{
	pContext->RSSetViewports(m_nViewPorts, &m_vpOld);
	pContext->OMSetRenderTargets(1, &m_pOldRTV, m_pOldDSV);
	return true;
}

BDxRT::BDxRT()
{
	m_pRenderTargetView = nullptr;
	m_pDepthStencilView = nullptr;
	m_pSRV = nullptr;
	m_pTexture = nullptr;
	m_DSFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
}


BDxRT::~BDxRT()
{
}
