#pragma once
#include "BStd.h"
#include "BCamera.h"
#include "BDxRTHeight.h"
#include "BNPlane.h"

/*미니맵과 화면 전체를 랜더링 하기위해 쓰일 클래스*/
class BRenderPlane : public BCamera
{
public:
	shared_ptr<BNPlane>  m_pPlaneShape;
	shared_ptr<BDxRTHeight> m_pMiniMapRT;
	D3D11_VIEWPORT m_vp;

	ID3D11BlendState* m_pMiniMapAlphaBlend;

public:
	/*Create로 인자값을 생성한 후 Set으로 ViewPort를 어떻게 잡을것인지 변환*/
	virtual bool Create(ID3D11Device* pd3dDevice, const TCHAR* pLoadShaderFile, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, FLOAT Width = 1024, FLOAT Height = 1024);

	/*Sample에서 뷰포트 세팅을 위해 선언*/
	void Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth = 0, FLOAT MaxDepth = 1);

	/*Rander*/
	bool BeginRender(ID3D11DeviceContext* pContext, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView, D3DXVECTOR4 vColor);
	bool Render(ID3D11DeviceContext* pContext);
	bool AfterRender(ID3D11DeviceContext* pContext);
public:


public:
	BRenderPlane();
	virtual ~BRenderPlane();
};

