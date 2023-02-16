#include "BDevice.h"

ID3D11Device* g_pd3dDevice;
ID3D11DeviceContext* g_pImmediateContext;
D3D11_VIEWPORT* g_ViewPort;
ID3D11RenderTargetView* g_pRenderTargetView;
ID3D11DepthStencilView* g_pDepthStencilView;
IDXGISwapChain*			g_pSwapChain;


HRESULT BDevice::InitDevice(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen)
{
	HRESULT hr = S_OK;
	if (FAILED(hr = CreateDevice()))
	{
		MessageBox(0, _T("CreateDevice  실패"), _T("Fatal error"), MB_OK);
		return hr;
	}
	if (FAILED(hr = CreateGIFactory()))
	{
		MessageBox(0, _T("CreateGIFactory  실패"), _T("Fatal error"), MB_OK);
		return false;
	}
	if (FAILED(hr = CreateSwapChain(hWnd, iWidth, iHeight, IsFullScreen)))
	{
		MessageBox(0, _T("CreateSwapChain  실패"), _T("Fatal error"), MB_OK);
		return false;
	}
	if (FAILED(hr = SetRenderTargetView()))
	{
		MessageBox(0, _T("SetRenderTargetView  실패"), _T("Fatal error"), MB_OK);
		return false;
	}
	if (FAILED(hr = SetViewPort()))
	{
		MessageBox(0, _T("SetViewPort  실패"), _T("Fatal error"), MB_OK);
		return false;
	}
	if (FAILED(hr = m_pGIFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER)))
	{
		MessageBox(0, _T("MakeWindowAssociation  실패"), _T("Fatal error"), MB_OK);
		return false;
	}
	return true;
}

HRESULT BDevice::CreateDepthStencilView()
{
	HRESULT hr = S_OK;
	if (m_pDepthStencilView != NULL)
	{
		S_RELEASE(m_pDepthStencilView);
	}

	/*깊이 스텐실 뷰(이하 DSV)를 사용하기 위해서 텍스쳐 리소스가 필요*/
	ID3D11Texture2D*		pDepthStencilTexture = NULL;		// 텍스쳐 리소스
	D3D11_TEXTURE2D_DESC	descDepth;							// 텍스쳐 리소스 생성을 위해 필요한 인자값

	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = g_iWindowWidth;
	descDepth.Height = g_iWindowHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;			// Depth를 24비트 / stencil을 8비트로 사용하겠다.
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	m_pd3dDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencilTexture);

	// Create depth StencilView
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;  // 2D 텍스쳐를 사용하겠다.
	descDSV.Texture2D.MipSlice = 0;

	m_pd3dDevice->CreateDepthStencilView(pDepthStencilTexture, &descDSV, &m_pDepthStencilView);
	g_pDepthStencilView = m_pDepthStencilView;
	S_RELEASE(pDepthStencilTexture);

	return hr;
}

HRESULT BDevice::CreateDevice()
{
	HRESULT hr;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	//D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS; // D2D 쓰기를 허용한다 / 복수의 스레드 생성을 불허한다.
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes); // == sizeof(driverType) / sizeof(driverTypes[0])
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(driverTypes); // == sizeof(featureLevels) / sizeof(featureLevels[0]);
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_DriverType = driverTypes[driverTypeIndex];
		if (SUCCEEDED(hr = D3D11CreateDevice( // 디바이스 생성과 스왑체인을 한번에 해주는 함수
			NULL,					// 기본 그래픽 드라이버를 사용한다 == NULL
			m_DriverType,			// m_DriverType = D3D_DRIVER_TYPE_HARDWARE * 하드웨어 타입의 드라이버를 사용하겠다.
			NULL,					// 드라이버 타입이 소프트웨어인 경우 넣는 인자이므로 NULL
			createDeviceFlags,		// flag값이므로 default인 0을 사용한다 (위에서 변수로 선언)
			featureLevels,			// 다이렉트 버전의 구조체
			numFeatureLevels,		// 다이렉트 버전의 구조체의 변수 갯수
			D3D11_SDK_VERSION,		// SDK버전 ( 무조건 이걸로 넣어야 함 )
			&m_pd3dDevice,			// 디바이스와 스왑체인이 생성되었을 때 디바이스 인터페이스를 받을 수 있다.
			&m_FeatureLevel,		// 디바이스와 스왑체인이 생성되었을 때 적용된 다이렉트 버전을 받을 수 있다.
			&m_pImmediateContext	// 디바이스와 스왑체인이 생성되었을 때 디바이스 컨텍스트 인터페이스를 받을 수 있다.
		)))
		{
			if (FAILED(hr) || m_FeatureLevel < D3D_FEATURE_LEVEL_11_0)
			{
				if (m_pImmediateContext) m_pImmediateContext->Release();
				if (m_pd3dDevice) m_pd3dDevice->Release();
				continue;
			}
			break;
		}
	}
	if (FAILED(hr)) return hr;
	g_pd3dDevice = m_pd3dDevice;
	g_pImmediateContext = m_pImmediateContext;

	if (FAILED(hr = BDxState::SetState(m_pd3dDevice))) return hr;
	m_pRasterizerState = BDxState::g_pRSNoneCullSolid;
	return S_OK;
}


HRESULT BDevice::CreateGIFactory()
{
	HRESULT hr = S_OK;

	IDXGIDevice * pDXGIDevice;
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
	
	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);

	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_pGIFactory);

	S_RELEASE(pDXGIDevice);
	S_RELEASE(pDXGIAdapter);

	return hr;
}

HRESULT BDevice::CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen)
{
	HRESULT hr = S_OK;
	if (m_pGIFactory == NULL) return S_FALSE;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = iWidth;
	sd.BufferDesc.Height = iHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (FAILED(hr = m_pGIFactory->CreateSwapChain(m_pd3dDevice, &sd, &m_pSwapChain)))
	{
		return hr;
	}
	g_pSwapChain = m_pSwapChain;
	return hr;
}

HRESULT BDevice::SetRenderTargetView()
{
	HRESULT hr;
	ID3D11Texture2D* pBackBuffer;
	
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return hr;
	
	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr)) return hr;

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	g_pRenderTargetView = m_pRenderTargetView;
	return S_OK;
}

HRESULT BDevice::SetViewPort()
{
	HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC Desc;
	m_pSwapChain->GetDesc(&Desc);
	/*뷰포트 세팅*/
	m_ViewPort.TopLeftX = (FLOAT)0;
	m_ViewPort.TopLeftY = (FLOAT)0;
	m_ViewPort.Width = (FLOAT)Desc.BufferDesc.Width;
	m_ViewPort.Height = (FLOAT)Desc.BufferDesc.Height;
	m_ViewPort.MinDepth = (FLOAT)0.0f;
	m_ViewPort.MaxDepth = (FLOAT)1.0f;

	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	g_ViewPort = &m_ViewPort;
	return S_OK;
}

bool BDevice::CleanupDevice() // Release
{
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	S_RELEASE(m_pRenderTargetView);
	S_RELEASE(m_pSwapChain);
	S_RELEASE(m_pImmediateContext);
	S_RELEASE(m_pd3dDevice);
	S_RELEASE(m_pGIFactory);
	m_pImmediateContext = NULL;
	return true;
}

HRESULT BDevice::ResizeDevice(UINT iWidth, UINT iHeight)
{
	if (m_pd3dDevice == NULL) return true;
	HRESULT hr = S_OK;
	/*백버퍼와 관련있는 리소스 해제*/
	if (FAILED(DeleteDxResource())) /*이곳에서 폰트를 해제 할 것이다.*/
	{
		return false;
	}
	DeleteDxResource();

	/*랜더타겟과 깊이스텐실 버퍼 해제*/

	m_pImmediateContext->OMSetRenderTargets(0, NULL, NULL);
	S_RELEASE(m_pDepthStencilView);
	S_RELEASE(m_pRenderTargetView);

	/*백버퍼의 크기를 조정한다.*/
	if (FAILED(hr = m_pSwapChain->ResizeBuffers(sd.BufferCount, iWidth, iHeight, sd.BufferDesc.Format, sd.Flags)))
	{
		return hr;
	}

	if (FAILED(hr = SetRenderTargetView()))
	{
		MessageBox(0, _T("SetRenderTargetView  실패"), _T("Fatal error"), MB_OK);
		return false;
	}

	if (FAILED(hr = m_pSwapChain->GetDesc(&sd)))
	{
		return hr;
	}

	g_iWindowWidth  = sd.BufferDesc.Width;
	g_iWindowHeight = sd.BufferDesc.Height;

	CreateDepthStencilView();		// 깊이 스텐실 뷰 인터페이스 생성

	if (FAILED(hr = SetViewPort()))
	{
		MessageBox(0, _T("SetViewPort  실패"), _T("Fatal error"), MB_OK);
		return false;
	}
	CreateDxResource();
	return hr;
}

BDevice::BDevice()
{
	m_pd3dDevice = NULL;
	m_pSwapChain = NULL;
	m_pImmediateContext = NULL;
	m_DriverType = D3D_DRIVER_TYPE_NULL;
	m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	m_pRenderTargetView = NULL;
	m_pGIFactory = NULL;
}


BDevice::~BDevice()
{
}
