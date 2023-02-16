#include "BRenderPlane.h"

void BRenderPlane::Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth, FLOAT MaxDepth)
{
	m_vp.TopLeftX = TopLeftX;
	m_vp.TopLeftY = TopLeftY;
	m_vp.Width = Width;
	m_vp.Height = Height;
	m_vp.MinDepth = MinDepth;
	m_vp.MaxDepth = MaxDepth;
	m_pPlaneShape->m_Texture.m_pTextureSRV.resize(1);

	HRESULT hr = S_OK;
	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	BlendState.AlphaToCoverageEnable = FALSE; // 알파테스팅으로 작업한다.
											  /*총 8개의 랜더타겟을 잡을 수 있으나 지금은 0번 타겟만을 사용*/
											  /*컬러연산*/
											  // FinalColor = DestColor * (1.0f - SrcAlpha) + SrcColor * SrcAlpha // 알파 방정식은 직선의 방정식과 동일하다. y = ax + d 
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;// D3D11_BLEND_SRC_ALPHA;		//소스 알파값을 블랜딩으로 사용
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;   //소스 알파값의 반전을 블랜딩으로 사용

															   /*알파연산*/
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;			//소스 알파값을 그대로 사용
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		//배경 알파값을 사용하지 않음

	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 2마스킹 한 결과만 뿌리는 것
	if (FAILED(hr = pDevice->CreateBlendState(&BlendState, &m_pMiniMapAlphaBlend)))
	{
		return;
	}
	return;
}

bool BRenderPlane::Create(ID3D11Device* pd3dDevice, const TCHAR* pLoadShaderFile, DXGI_FORMAT format, FLOAT Width, FLOAT Height)
{
	m_pPlaneShape = make_shared<BNPlane>();
	m_pMiniMapRT = make_shared<BDxRTHeight>();
	m_pMiniMapRT->m_DSFormat = format;
	TCHAR* NullTex = NULL;
	m_pPlaneShape->Create(pd3dDevice, pLoadShaderFile, NullTex);
	m_pMiniMapRT->Create(pd3dDevice, DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024);

	DWORD dwWidth = m_pMiniMapRT->m_TexDesc.Width;
	DWORD dwHeight = m_pMiniMapRT->m_TexDesc.Height;
	TVector3 vUpVector(0.0f, 1.0f, 0.0f);
	SetViewMatrix(TVector3(0.0f, 100.0f, 0.0f), TVector3(0.0f, 0.0f, 1.0f), vUpVector);
	SetProjMatrix(PI * 0.25f, (float)dwWidth / (float)dwHeight, 0.1f, 1000.0f);

	return true;
}

bool BRenderPlane::BeginRender(ID3D11DeviceContext* pContext, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView, TVector4 vColor)
{
	if (m_pMiniMapRT->Begin(pContext, OldViewPort, OldDepthStencilView, OldRenderTargetView, vColor) == false)
	{
		m_pMiniMapRT->End(pContext);
		return false;
	}
	return true;
}

bool BRenderPlane::Render(ID3D11DeviceContext* pContext)
{
	//m_pPlaneShape->m_pTextureSRV[0] = m_pMiniMapRT->m_pSRV;
	m_pPlaneShape->m_Texture.m_pTextureSRV[0] = m_pMiniMapRT->m_pDsvSRV;
	m_pPlaneShape->m_iCurIndex = 1;
	pContext->RSSetViewports(1, &m_vp);
	TMatrix m_matTemp;
	D3DXMatrixIdentity(&m_matTemp);
	m_pPlaneShape->SetMatrix(m_matTemp, m_matTemp, m_matTemp);
	pContext->UpdateSubresource(m_pPlaneShape->m_pConstantBuffer, 0, NULL, &m_pPlaneShape->m_cbData, 0, 0);
	pContext->OMSetBlendState(m_pMiniMapAlphaBlend, 0, -1);
	m_pPlaneShape->Render(pContext);
	return true;
}

bool BRenderPlane::AfterRender(ID3D11DeviceContext* pContext)
{
	m_pMiniMapRT->End(pContext);
	return true;
}

BRenderPlane::BRenderPlane()
{
	m_pPlaneShape = nullptr;
}


BRenderPlane::~BRenderPlane()
{
}
