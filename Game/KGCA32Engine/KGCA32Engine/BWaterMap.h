#pragma once
#include "BRenderPlaneHeight.h"
#include "BMapTileRender.h"
#include "BNMap.h"

#define FOGMODE_NONE    0
#define FOGMODE_LINEAR  1


struct cbFog
{
	TVector4 g_ClipPlane;
	TVector4 g_vCameraPos;
	TVector4 g_FogColor;
	TVector4 g_FogInfo;
	float		g_fogMode;		//x(������)
	float		g_fogDensity;	//y(��)
	float		g_fogStart;		//z(�������װŸ�)
	float		g_fogEnd;		//w(�������װŸ�)
};

struct cbTanLight
{
	TMatrix      matNormal;
	TVector4		vLightPos;
	TVector4		vEye;
	TVector4		vEyeDir;
};

struct cbLightDefault
{
	TVector4				cb_AmbientLightColor;
	TVector4				cb_DiffuseLightColor;
	TVector4				cb_SpecularLightColor;
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
	TMatrix m_matNormal;

	cbFog m_Fog;
	cbTanLight m_TanLight;
	cbLightDefault m_LightDefault;

	ComPtr<ID3D11Buffer> m_pFogCB;
	ComPtr<ID3D11Buffer> m_pTanLightCB;
	ComPtr<ID3D11Buffer> m_pLightDefaultCB;

	vector<TVector3>		m_vTangentList;
	ComPtr<ID3D11Buffer>	m_pTangentVB;

	Texture m_WaterMapUpside;
	Texture m_WaterBottom;
	Texture m_WaterHeight;
	Texture m_WaterNormal;
	Texture m_Water;
	
	//ComPtr<ID3D11ShaderResourceView> m_WaterMapUpsideSRV; // m_pReflectRV // �ݻ纤��
	//ComPtr<ID3D11ShaderResourceView> m_WaterBottomSRV;	  // m_pRefractRV // ��������
	//ComPtr<ID3D11ShaderResourceView> m_WaterHeightSRV;
	//ComPtr<ID3D11ShaderResourceView> m_WaterNormalSRV;
	//ComPtr<ID3D11ShaderResourceView> m_WaterSRV;

	ID3D11VertexShader*		m_pWaterVS; // ���ؽ� ���̴�
	ID3D11PixelShader*		m_pWaterPS; // �ȼ� ���̴�

	BRenderPlaneHeight m_HeightMap;

	bool bHeightChange; // ���̰� ����Ǿ�����
	bool bRequestCreate; // ���̸� ������ ��û�Ǿ�����

	BMapTileRender* m_SelectedMap; // ���͸��� ������ ���� ����
	BRenderPlane m_MiniMap[3]; // �ݻ� �������͸� ����� ���� �̴ϸ�
	TMatrix* TMatrixReflect_Fixed(__inout TMatrix *pOut, __in const TPlane *pPlane);
	void DrawWaterRT(int iMap, TMatrix* pWorld, TMatrix* pView, TMatrix* pProj, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView);
public:
	bool WaterFrame(D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView);
	bool Render();
	HRESULT SetWaterNormalTexture(const TCHAR* strFilePath);
	HRESULT SetHeightTexture(const TCHAR* strFilePath);
	bool Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pContext, TCHAR* strWaterTex = NULL, TCHAR* strNormalTex = NULL, TCHAR* strHeightTex = NULL);
	bool SetMapHeight(float Height = 0.0f);
	bool SetMapHeight();
	HRESULT CreateWaterMapInputLayout(const TCHAR* pShaderFile);
	void SetWaterMatrix(TMatrix* World, TMatrix* View, TMatrix* Proj);
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

