#pragma once
#include "BRenderPlaneHeight.h"
#include "BMapTileRender.h"
#include "BNMap.h"

#define FOGMODE_NONE    0
#define FOGMODE_LINEAR  1


struct cbFog
{
	D3DXVECTOR4 g_ClipPlane;
	D3DXVECTOR4 g_vCameraPos;
	D3DXVECTOR4 g_FogColor;
	D3DXVECTOR4 g_FogInfo;
	float		g_fogMode;		//x(감쇠방법)
	float		g_fogDensity;	//y(농도)
	float		g_fogStart;		//z(시작포그거리)
	float		g_fogEnd;		//w(종료포그거리)
};

struct cbTanLight
{
	D3DXMATRIX      matNormal;
	D3DXVECTOR4		vLightPos;
	D3DXVECTOR4		vEye;
	D3DXVECTOR4		vEyeDir;
};

struct cbLightDefault
{
	D3DXVECTOR4				cb_AmbientLightColor;
	D3DXVECTOR4				cb_DiffuseLightColor;
	D3DXVECTOR4				cb_SpecularLightColor;
};

class BWaterMap : public BMapTileRender
{
public:
	BMap* m_pWaterTexMap;
	BMapTileRender* m_pWaterReflectMap;
	TCHAR* m_strWaterTex;
	TCHAR* m_strWaterNormalTex;

	int m_iSelectedMap;
	float m_fWaterHeight;
	float m_fWaterWave;
	D3DXMATRIX m_matNormal;

	cbFog m_Fog;
	cbTanLight m_TanLight;
	cbLightDefault m_LightDefault;

	ComPtr<ID3D11Buffer> m_pFogCB;
	ComPtr<ID3D11Buffer> m_pTanLightCB;
	ComPtr<ID3D11Buffer> m_pLightDefaultCB;

	vector<D3DXVECTOR3>		m_vTangentList;
	ComPtr<ID3D11Buffer>	m_pTangentVB;
	
	ComPtr<ID3D11ShaderResourceView> m_WaterMapUpsideSRV; // m_pReflectRV // 반사벡터
	ComPtr<ID3D11ShaderResourceView> m_WaterBottomSRV;	  // m_pRefractRV // 굴절벡터
	ComPtr<ID3D11ShaderResourceView> m_WaterHeightSRV;
	ComPtr<ID3D11ShaderResourceView> m_WaterNormalSRV;
	ComPtr<ID3D11ShaderResourceView> m_WaterSRV;

	ID3D11VertexShader*		m_pWaterVS; // 버텍스 쉐이더
	ID3D11PixelShader*		m_pWaterPS; // 픽셀 쉐이더

	BRenderPlaneHeight m_HeightMap;

	bool bHeightChange; // 높이가 변경되었는지
	bool bRequestCreate; // 높이맵 갱신이 요청되었는지

	BMapTileRender* m_SelectedMap; // 워터맵을 생성할 맵을 저장
	BRenderPlane m_MiniMap[3]; // 반사 굴절벡터를 만들기 위한 미니맵
	D3DXMATRIX* D3DXMatrixReflect_Fixed(__inout D3DXMATRIX *pOut, __in const D3DXPLANE *pPlane);
	void DrawWaterRT(int iMap, D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView);
public:
	bool WaterFrame(D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView);
	bool Render();
	HRESULT SetWaterNormalTexture(const TCHAR* strFilePath);
	HRESULT SetHeightTexture(const TCHAR* strFilePath);
	bool Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pContext, TCHAR* strWaterTex = NULL, TCHAR* strNormalTex = NULL, TCHAR* strHeightTex = NULL);
	bool SetMapHeight(float Height = 0.0f);
	bool SetMapHeight();
	HRESULT CreateWaterMapInputLayout(const TCHAR* pShaderFile);
	void SetWaterMatrix(D3DXMATRIX* World, D3DXMATRIX* View, D3DXMATRIX* Proj);
	HRESULT CreateWaterConstantBuffer();
	HRESULT TextureLoad(const TCHAR* strWaterTex, const TCHAR* strNormalTex, const TCHAR* strHeightTex);
	HRESULT SetWaterTexture(const TCHAR* strFilePath);
	bool PostRender(ID3D11DeviceContext* pContext);
	void Update(ID3D11Device* pd3dDevice, BCamera* pCamera, BMapTileRender* SelectedMap);
	void ComputeBoundingBox(BNode* pNode);
public:
	BWaterMap();
	~BWaterMap();
};

