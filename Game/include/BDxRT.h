#pragma once
#include "BStd.h"

class BDxRT
{
public:
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView; // 랜더타겟뷰
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView; // 깊이스텐실뷰
	ComPtr<ID3D11ShaderResourceView> m_pSRV;    // 쉐이더 리소스뷰
	ComPtr<ID3D11ShaderResourceView> m_pDsvSRV; // 깊이버퍼 리소스 뷰
	ComPtr<ID3D11Texture2D> m_pTexture;

	D3D11_DEPTH_STENCIL_VIEW_DESC m_DepthStencilDesc; // 깊이버퍼용
	D3D11_TEXTURE2D_DESC m_TexDesc; // 텍스쳐용
	DXGI_FORMAT m_DSFormat; // 깊이 스텐실 버퍼용 포맷
	D3D11_VIEWPORT m_vp;  // 뷰포트
	D3DXMATRIX m_matProj; // 투영행렬

	D3D11_VIEWPORT m_vpOld; // 이전 뷰포트
	UINT m_nViewPorts; // 뷰포트의 수
	ID3D11RenderTargetView* m_pOldRTV;
	ID3D11DepthStencilView* m_pOldDSV;

public:
	// Sample에서 Set함수로 기본 인자 세팅
	void Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth = 0, FLOAT MaxDepth = 1);
	// Sample에서 Create 함수로 생성해낸다 
	virtual HRESULT Create(ID3D11Device* pd3dDevice, FLOAT Width = 1024, FLOAT Height = 1024);
	/*Create 내부에서 사용되는 함수*/
	HRESULT UpdateDepthStencilView(ID3D11Device* pDevice, UINT Width, UINT Height);

	// Sample의 Render에서 적용하는 함수
	bool Begin(ID3D11DeviceContext* pContext, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView, D3DXVECTOR4 vColor,
		         bool bTargetClear = true, bool bDepthClear = true, bool bStencilClear = true);
	// Sample에서 Create로 생성한 인자들을 받아와 적용시키는 함수
	void Apply(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV, bool bDepthClear = true, bool bStencilClear = true);

	// Sample에서 Begin에 저장한 이전 값으로 뷰포트, 깊이스텐실, 랜더타겟을 이전으로 되돌리기 위한 함수
	bool End(ID3D11DeviceContext* pContext);

public:
	BDxRT();
	virtual ~BDxRT();
};

