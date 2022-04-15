#include "BMouseShape.h"
#include "BCore.h"

bool BMouseShape::Init()
{
	HRESULT hr = S_OK;
	// 평면 생성
	m_MouseShape.Init();
	m_MouseShape.Create(g_pd3dDevice, g_pImmediateContext, L"../../data/UIShader/VertexShader.vsh", L"../../data/UIShader/PixelShader.psh", NULL);
	m_MouseShape.MultiTextureLoad(L"../../data/Cursor_bullet.png");
	m_MouseShape.LoadTexture();

	// 알파 테스팅
	D3D11_BLEND_DESC bdTesting;
	//ZeroMemory(&bdTesting, sizeof(D3D11_BLEND_DESC));
	bdTesting.IndependentBlendEnable = FALSE;
	bdTesting.AlphaToCoverageEnable = TRUE;
	bdTesting.RenderTarget[0].BlendEnable = TRUE;
	bdTesting.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bdTesting.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;//D3D11_BLEND_SRC_ALPHA;
	bdTesting.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	bdTesting.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bdTesting.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bdTesting.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bdTesting.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	if (FAILED(hr = g_pd3dDevice->CreateBlendState(&bdTesting, m_pAlphaTesting.GetAddressOf())))
	{
		return false;
	}

	return true;
}

bool BMouseShape::Frame()
{
	if (m_bRenderMouseShape)
	{
		GetCursorPos(&m_MousePoint);
		ScreenToClient(g_hWnd, &m_MousePoint);

		TVector3 vEye = TVector3(0.0f, 0.0f, -10.0f);
		TVector3 vAt = TVector3(0.0f, 0.0f, 0.0f);
		TVector3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&m_MouseShape.m_matView, &vEye, &vAt, &vUp);

		int iRectWidth = g_iWindowWidth / 2;
		int iRectHeight = g_iWindowHeight / 2;

		// 화면 중앙이 원점으로 계산되기 때문에 넓이 및 높이가 -1 ~ 1 범위로 직교투영된다. 
		D3DXMatrixOrthoOffCenterLH(&m_MouseShape.m_matProj, -iRectWidth, iRectWidth, iRectHeight, -iRectHeight, 0.0f, 1000.0f);

		TMatrix matScale, matRot;
		D3DXMatrixScaling(&matScale, 13.0f, 13.0f, 13.0f);
		D3DXMatrixRotationZ(&matRot, PI / 3.0f);
		D3DXMatrixMultiply(&m_MouseShape.m_matWorld, &matScale, &matRot);

		m_MouseShape.m_matWorld._41 = m_MousePoint.x - iRectWidth;
		m_MouseShape.m_matWorld._42 = m_MousePoint.y - iRectHeight;
	}
	return true;
}

bool BMouseShape::Render()
{
	if (m_bRenderMouseShape)
	{
		// 샘플링 스테이트 / 뎁스스텐실 적용
		g_pImmediateContext->PSSetSamplers(0, 1, &BDxState::g_pSSWrapLinear);
		g_pImmediateContext->OMSetDepthStencilState(BDxState::g_pDSSDepthDisable, 0);

		// 알파블렌딩
		g_pImmediateContext->OMSetBlendState(m_pAlphaTesting.Get(), 0, -1);

		// 마우스 렌더링
		m_MouseShape.SetMatrix(&m_MouseShape.m_matWorld, &m_MouseShape.m_matView, &m_MouseShape.m_matProj);
		m_MouseShape.Render(0);

		g_pImmediateContext->OMSetBlendState(0, 0, -1);
		g_pImmediateContext->OMSetDepthStencilState(BDxState::g_pDSSDepthEnable, 0);
	}
	return true;
}

bool BMouseShape::Release()
{
	return true;
}

BMouseShape::BMouseShape()
{
	m_bRenderMouseShape = true;
}


BMouseShape::~BMouseShape()
{
}
