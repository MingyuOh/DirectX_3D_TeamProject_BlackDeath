#pragma once
#include "BStd.h"
#include "BDxState.h"
using namespace DX;

class BDevice
{
public:
	
	HRESULT InitDevice(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen = FALSE);
	
	/*디바이스 & 스왑체인 생성*/
	HRESULT CreateDevice();
	HRESULT CreateGIFactory();
	HRESULT CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeight, BOOL IsFullScreen = FALSE);

	/*랜더타겟 뷰 & 뷰포트 생성*/
	HRESULT SetRenderTargetView();
	HRESULT SetViewPort();

	bool CleanupDevice();

	HRESULT CreateDepthStencilView();	// 깊이, 스텐실 뷰 리소스 생성
	
	/*크기 재 설정*/
	HRESULT ResizeDevice(UINT iWidth, UINT iHeight);
	/*크기 재 설정을 위한 리소스 제거함수*/
	virtual bool CreateDxResource() { return true; };
	virtual bool DeleteDxResource() { return true; };

public:
	ID3D11Device*			m_pd3dDevice;
	IDXGISwapChain*			m_pSwapChain;
	IDXGIFactory*			m_pGIFactory;
	ID3D11DeviceContext*	m_pImmediateContext;

	DXGI_SWAP_CHAIN_DESC sd;

	D3D11_VIEWPORT m_ViewPort;
	D3D_FEATURE_LEVEL m_FeatureLevel;
	D3D_DRIVER_TYPE m_DriverType;

	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11RasterizerState* m_pRasterizerState;

public:
	BDevice();
	~BDevice();
};

