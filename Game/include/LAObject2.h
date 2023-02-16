#pragma once

#include "CoreStandardLink.h"
#include "LACharacterStruct.h"
#include "LASpaceObject2.h"

#ifdef TOOL_MODE

#include "LAVertexDirLine.h"

#else
#endif

class LAObject2
{
public:
	//================================================================================
	//	** Create Variable ** 
	//================================================================================
	//임시저장
	ID3D11Device* m_pCoreDevice;
	ID3D11DeviceContext* m_pCoreContext;

	//리스트
	std::vector<PNCT_VECTOR> m_VertexList;
	std::vector<DWORD> m_IndexList;
	LASpaceObject2 m_SpaceObject;

	//생성
	ID3D11Buffer* m_pVertexBuffer;		//버텍스버퍼
	ID3D11Buffer* m_pIndexBuffer;		//인덱스버퍼
	ID3D11Buffer* m_pConstantBuffer;	//상수버퍼01
	ID3D11Buffer* m_pConstantBufferData;	//상수버퍼02
	/*Microsoft::WRL::ComPtr<ID3D11Buffer> m_StreamTo;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DrawFrom;*/
	ID3D11Buffer* m_StreamTo;
	ID3D11Buffer* m_DrawFrom;

	ID3D11VertexShader* m_pVS;				//버텍스 쉐이더
	ID3D11PixelShader* m_pPS;				//픽셀 쉐이더
	ID3D11VertexShader* m_pResourceVS;				//버텍스 쉐이더
	ID3D11PixelShader* m_pResourcePS;				//픽셀 쉐이더
	ID3D11PixelShader* m_pLightPS;			//픽셀 라이트적용 쉐이더
	ID3D11VertexShader* m_pNormalVS;		//버텍스 노말맵 쉐이더
	ID3D11PixelShader* m_pNormalPS;			//픽셀 노말맵 쉐이더
	ID3D11GeometryShader* m_pGS;			//지오메트리 쉐이더
	ID3D11PixelShader* m_pShadowPS;			//픽셀 노말맵 쉐이더
	//Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pSO;
	ID3D11GeometryShader* m_pSO;
	ID3D11HullShader* pHullShader;
	ID3D11DomainShader* pDomainShader;

	ID3D11ShaderResourceView* m_pTextureSRV;	//Direct3d 11.0 쉐이더 뷰 인터페이스
	ID3D11InputLayout* m_pVertexLayout;	//레이아웃
	//데이타 저장
	D3DXMATRIX  m_ObjectWorld;	//월드
	D3DXMATRIX  m_ObjectView;	//뷰
	D3DXMATRIX  m_ObjectProjection;	//투영
	ID3DBlob* pVSBuf;
	ID3DBlob* pPSBuf;
	ID3DBlob* pGSBuf;
	VERTEX_CB m_ObjectCB;		//행렬
	DataCB m_ObjectDataCB;		//그외 데이터
	D3DXMATRIX m_Scale;		//스케일
	D3DXMATRIX m_Rotate;		//회전
	D3DXMATRIX m_Move;	//이동
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vSide;
	int m_iMultiTextureRender;
	CrateObjectDest m_ObjectDest;

	//Direction Light
	D3DXVECTOR4 m_vLightPos;
	D3DXVECTOR4 m_vLightDirection;
	D3DXVECTOR4 m_vCameraPos;

	//================================================================================
	//	** Other Value ** 
	//================================================================================
	//CheckRelease
	bool m_bCheckReleaseObject;
	bool m_bIsNormalMap;	//NormalMap
	bool m_bisLight;		//Light
#ifdef TOOL_MODE

	LAVertexDirLine* m_pDrawLine;

#else
#endif
	
public:
	//================================================================================
	//	** Create  Function ** 
	//================================================================================
	virtual HRESULT Create(CrateObjectDest& ObjectDest);
	void CreateDrawLine();
	void DrawLine(std::vector<PNCT5_VERTEX>& rVertexList, std::vector<DWORD>& riIndexList, int iDrawIndexListMaxSize, D3DXMATRIX matWorld);
	//Texture 리소스 생성
	virtual HRESULT LoadTextureResource();
	//Texture 쉐이더 셋팅
	HRESULT ApplyTexture();				
	//쉐이더 파일 컴파일
	virtual HRESULT LoadShader();
	//상수 버퍼 생성
	virtual HRESULT CreateConstantBuffer();
	virtual HRESULT CreateVertexBuffer();		//버텍스 버퍼 생성
	virtual HRESULT CreateIndexBuffer();		//인덱스 버퍼 생성
	virtual HRESULT CreateLayout();				//레이아웃 객체 생성

	/*======= Set =======*/
	virtual void SetMatrix(D3DXMATRIX* pWorldMatrix, D3DXMATRIX* pViewMatrix, D3DXMATRIX* pProjectionMatrix);
	void SetPosition(D3DXVECTOR3 vPos);
	void SetNormalMapping(bool bCheckNormal);
	void SetLight(bool bCheckLight);
	void SetResourceVS(ID3D11VertexShader* pVS);
	void SetResourcePS(ID3D11PixelShader* pPS);
	void SetBlob(ID3DBlob* pVSBuf);

	/*======= Get =======*/
	bool GetNormalMapping();
	bool GetLight();

	//================================================================================
	//	** CreateGeometryShaderWithStreamOutput ** 
	//================================================================================
	HRESULT CreateGeometryWithStreamOut();
	HRESULT CreateStreamOutBuffer(int iBufferSize = 100000, ID3D11Buffer** pTargetBuffer = nullptr);
	bool HandleEffects(ID3D11Buffer* pBuffer);

	//================================================================================
	//	** ReSize  Function ** 
	//================================================================================
	HRESULT DeleteResource();
	HRESULT CreateResource();

	//================================================================================
	//	** Transform Function  ** 
	//================================================================================
	void Translation(float x, float y, float z);
	void RotationX(float fangle);
	void RotationY(float fangle);
	void RotationZ(float fangle);
	void Scaling(float x, float y, float z);

	
public:
	virtual bool Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual bool Frame();
	virtual bool PrevRender();
	virtual bool PostRender();
	virtual bool Render();
	virtual bool Release();
public:
	LAObject2();
	~LAObject2();
};

