#include "BCore.h"

BFPSCamera* g_pMainCamera;

bool BCore::CreateDxResource()
{
	IDXGISurface1* pBackBuffer = NULL;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (VOID**)&pBackBuffer);
	m_Writer.Create(pBackBuffer);
	S_RELEASE(pBackBuffer);
	return true;
}
bool BCore::DeleteDxResource()
{
	m_Writer.Release();
	return true;
}

bool BCore::Init()
{
	return true;
}
bool BCore::Frame()
{
	return true;
}
bool BCore::Render()
{
	return true;
}
bool BCore::Release()
{
	return true;
}

bool BCore::PreInit()
{
	if (FAILED(InitDevice(m_hWnd, this->m_iWindowWidth, this->m_iWindowHeight)))
	{
		MessageBox(0, _T("CreateDevice  실패"), _T("Fatal error"), MB_OK);
		return false;
	}
	m_pDebugCamera = new BBackViewCamera;
	m_pFPSCamera = new BFPSCamera;
	//m_pDebugCamera->Init(TVector3(0.0f, 50.0f, -3.0f), TVector3(0.0f, 0.0f, 0.0f));
	m_pDebugCamera->CreateFrustum(m_pd3dDevice, L"../../data/shader/NormalShader.hlsl");
	m_pFPSCamera->CreateFrustum(m_pd3dDevice, L"../../data/shader/NormalShader.hlsl");
	m_pMainCamera = m_pFPSCamera;
	g_pMainCamera = m_pMainCamera;
	CreateDepthStencilView();		// 깊이 스텐실 뷰 인터페이스 생성
	CreateDxResource();
	return true;
}

/*BCore의 오브젝트를 만들기 위한 Init함수입니다.*/
bool BCore::PostInit()
{
	TCHAR* skyTextture[]
	{
		L"../../data/BlueNebular_front.jpg",
		L"../../data/BlueNebular_top.jpg",
		L"../../data/BlueNebular_left.jpg",
		L"../../data/BlueNebular_bottom.jpg",
		L"../../data/BlueNebular_back.jpg",
		L"../../data/BlueNebular_right.jpg",
	};
	m_SkyBox.Create(m_pd3dDevice, L"../../data/shader/SkyShader.hlsl",&skyTextture[0],6);
	D3DXMatrixScaling(&m_SkyBox.m_matScale, 500.0f, 500.0f, 500.0f);
	m_SkyBox.m_matWorld = m_SkyBox.m_matWorld * m_SkyBox.m_matScale;
	
	m_DirectionLine.Create(m_pd3dDevice, L"../../data/shader/LineShader.hlsl");
	D3DXMatrixScaling(&m_DirectionLine.m_matScale, 1000.0f, 1000.0f, 1000.0f);
	m_DirectionLine.m_matWorld = m_DirectionLine.m_matWorld * m_DirectionLine.m_matScale;
	m_DirectionLineOn = false;

	return true;
}
bool BCore::PreFrame()
{
	/*카메라 & 투영 행렬설정*/
	// =========================================================== //
	/*카메라 설정*/
	// 카메라 행렬
	// 카메라 위치 , 바라볼 위치 , 카메라 상향벡터(외적으로 Y축을 파악하게 해줄 벡터이므로 보통 0.001 정도를 더한다.)
	/*투영 설정*/
	// 투영 행렬
	// 시야각, 투영할 화면의 크기, 최소 출력거리, 최대 출력거리
	//TMatrixIdentity(&m_matWorld);
	//TMatrixLookAtLH(&m_matView, &TVector3(0.0f, 6.0f, -5.0f), &TVector3(0.0f, 0.0f, 0.0f), &TVector3(0.0f, 1.0f, 0.0f));
	//TMatrixPerspectiveFovLH(&m_matProj, D3DX_PI*0.25f, m_iWindowWidth / m_iWindowHeight, 1.0f, 500.0f);
	// =========================================================== //

	//m_pMainCamera->Frame();


	// 마우스 커서 클라이언트 안에 위치 고정하기
	RECT rc;
	POINT LeftTop, RightBottom;
	GetClientRect(g_hWnd, &rc);
	LeftTop.x = rc.left; LeftTop.y = rc.top;
	RightBottom.x = rc.right; RightBottom.y = rc.bottom;
	ClientToScreen(g_hWnd, &LeftTop);
	ClientToScreen(g_hWnd, &RightBottom);
	rc.left = LeftTop.x; rc.top = LeftTop.y;
	rc.right = RightBottom.x; rc.bottom = RightBottom.y;
	ClipCursor(&rc);

	return true;
}
bool BCore::PostFrame()
{

	if (I_Input.KeyCheck(DIK_F1) == KEY_PUSH)
	{
		m_DirectionLineOn = !m_DirectionLineOn;
	}

	if (I_Input.KeyCheck(DIK_F2) == KEY_PUSH)
	{
		m_bWireFrameRender = !m_bWireFrameRender;
		(m_bWireFrameRender) ? m_pRasterizerState = BDxState::g_pRSWireFrame : m_pRasterizerState = BDxState::g_pRSNoneCullSolid;
	}
	return true;
}
bool BCore::PreRender()
{
	float BackColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, BackColor);
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	m_pImmediateContext->OMSetDepthStencilState(BDxState::g_pDSSDepthEnable, 0x00);
	m_pImmediateContext->PSSetSamplers(0, 1, &BDxState::g_pSSMirrorLinear);
	m_pImmediateContext->RSSetState(m_pRasterizerState);

	m_pMainCamera->FrustRender(m_pImmediateContext);

	m_SkyBox.SetMatrix(m_SkyBox.m_matWorld, m_pMainCamera->m_matView, m_pMainCamera->m_matProj);
	m_SkyBox.OMSetState(m_pImmediateContext, m_pRenderTargetView, m_pDepthStencilView, BDxState::g_pDSSDepthEnable, BDxState::g_pNoAlphaBlend);
	m_SkyBox.Render(m_pImmediateContext);

	if (m_DirectionLineOn)
	{
		m_DirectionLine.SetMatrix(m_DirectionLine.m_matWorld, m_pMainCamera->m_matView, m_pMainCamera->m_matProj);
		m_DirectionLine.Render(m_pImmediateContext);
	}

	return true;
}
bool BCore::PostRender()
{
	HRESULT hr = S_OK;
	if (m_Writer.Begin())
	{
		m_Writer.m_pText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_Writer.m_pText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

		TCHAR buffer[256] = { 0, };
		_stprintf_s(buffer, L"FPS:%d, SPF:%10.4f", m_Timer.m_iFramePerSecond, m_Timer.m_fDurationTime);

		RT_F rc1(0, 0, m_iWindowWidth, m_iWindowHeight);
		m_Writer.DrawText(rc1, buffer, D2D1::ColorF(D2D1::ColorF::BlueViolet));
		m_Writer.End();
	}

	if (FAILED(hr = m_pSwapChain->Present(0, 0))) // 최종으로 윈도우에 플리핑
	{
		return false;
	}
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);	// 깊이 스텐실 버퍼 초기화
	return true;
}

bool BCore::Run()
{
	if (!BInit()) return false;
	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			BFrame();
			BRender();
		}
	}
	if (!BRelease()) return false;
	return true;
}

bool BCore::BInit()
{
	m_Timer.Init();
	I_Input.Init();
	m_Writer.Init();

	m_bWireFrameRender = false;

	if (!PreInit()) return false;
	if (!Init()) return false;
	if (!PostInit()) return false;
	return true;
}
bool BCore::BFrame()
{
	m_Timer.Frame();
	I_Input.frame();

	PreFrame();
	Frame();
	PostFrame();
	return true;
}
bool BCore::BRender()
{
	PreRender();
	Render();
	PostRender();
	return true;
}
bool BCore::BRelease()
{
	DeleteDxResource();
	CleanupDevice();
	I_Input.Release();
	m_Writer.Release();

	m_SkyBox.Release();
	m_DirectionLine.Release();
	S_RELEASE(m_pBackViewCamera);
	S_RELEASE(m_pDebugCamera);
	S_RELEASE(m_pTreeCamera);
	S_RELEASE(m_pFPSCamera);

	delete m_pTreeCamera;
	delete m_pBackViewCamera;
	delete m_pDebugCamera;
	
	if (!Release()) return false;
	return true;
}

BCore::BCore()
{
}

BCore::~BCore()
{
}
