#pragma once
#include "BStd.h"
#include "BCamera.h"
#include "BDxRTHeight.h"
#include "BNPlane.h"

/*�̴ϸʰ� ȭ�� ��ü�� ������ �ϱ����� ���� Ŭ����*/
class BRenderPlane : public BCamera
{
public:
	shared_ptr<BNPlane>  m_pPlaneShape;
	shared_ptr<BDxRTHeight> m_pMiniMapRT;
	D3D11_VIEWPORT m_vp;

	ID3D11BlendState* m_pMiniMapAlphaBlend;

public:
	/*Create�� ���ڰ��� ������ �� Set���� ViewPort�� ��� ���������� ��ȯ*/
	virtual bool Create(ID3D11Device* pd3dDevice, const TCHAR* pLoadShaderFile, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, FLOAT Width = 1024, FLOAT Height = 1024);

	/*Sample���� ����Ʈ ������ ���� ����*/
	void Set(ID3D11Device* pDevice, FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth = 0, FLOAT MaxDepth = 1);

	/*Rander*/
	bool BeginRender(ID3D11DeviceContext* pContext, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView, TVector4 vColor);
	bool Render(ID3D11DeviceContext* pContext);
	bool AfterRender(ID3D11DeviceContext* pContext);
public:


public:
	BRenderPlane();
	virtual ~BRenderPlane();
};
