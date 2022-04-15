#pragma once
#include "BStd.h"
#include "Texture.h"

struct PC_VERTEX	//벌텍스와 컬러값
{
	TVector3 v;
	TVector4 c;
	//생성자
	PC_VERTEX() {}
	PC_VERTEX(TVector3 v0, TVector4 c0)
	{
		v = v0;
		c = c0;
	}
};

struct PNCT_VERTEX	//벌텍스 노말 컬러값 텍스쳐
{
	TVector3 v;
	TVector3 n;
	TVector4 c;
	TVector2 t;
	//생성자
	PNCT_VERTEX() {}
	PNCT_VERTEX(TVector3 v0, TVector3 n0,TVector4 c0, TVector2 t0)
	{
		v = v0;
		n = n0;
		c = c0;
		t = t0;
	}
};

struct CB
{
	TMatrix matWorld;// c0						
	TMatrix matView;	// c4						
	TMatrix matProj;	// c8						
	TVector4 Color; // 12
};

//struct CB
//{
//	TMatrix matWVP;
//	//TMatrix matWorld; // c0 ~ c3
//	//TMatrix matView; // c0 ~ c3
//	//TMatrix matProj; // c0 ~ c3
//	float a; // c4
//	float fTime; // 4배수
//	float c, d;
//};

class K_Object
{
public:
	TMatrix m_matView;
	TMatrix m_matProj;
	TMatrix m_matWorld;
	
	TVector3	m_vCenter;	//센터좌표

	ID3D11Device* m_pd3dDevice;     
	ID3D11DeviceContext* m_pContext;

	ID3D11Buffer*  m_pConstantBuffer;
	ID3D11Buffer*  m_pVertexBuffer;
	ID3D11Buffer*  m_pIndexBuffer;
	ID3D11Buffer*  m_pCB;
	CB             m_cbData;
	ID3D11VertexShader*  m_pVS;
	ID3D11PixelShader*  m_pPS;
	ID3D11ShaderResourceView* m_pSRV;
	//멀티텍스쳐
	Texture m_Texture;
	//vector<ComPtr<ID3D11ShaderResourceView>>	m_pSRVArray;
	vector<basic_string<TCHAR>>	m_TextArray;
	//
	ID3D11InputLayout*  m_pIL;
	ID3D11InputLayout*	m_pVertexLayout;
	ID3DBlob*   m_pVSBuffer;

	std::vector<PNCT_VERTEX> m_VertexList;//벌텍스 리스트
	std::vector<DWORD>      	m_IndexList;//인덱스 리스트
public:
	//생성함수
	virtual bool MultiTextureLoad(TCHAR* pName);
	virtual bool Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		TCHAR* vs,
		TCHAR* ps,
		TCHAR* pTexName);
	virtual bool CreateInputLayout();
	virtual bool CreateIndexBuffer();
	virtual bool CreateVertexBuffer();
	bool CreateConstanceBuffer();
	bool LoadShader(TCHAR* vs, TCHAR* ps);//쉐이더를 로드하는 함수
	virtual bool LoadTexture();//텍스쳐를 로드하는 함수

	virtual void SetMatrix(TMatrix* matWorld,
		TMatrix* matView, TMatrix* matProj);
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool PreRender();
	virtual bool Render();
	virtual bool Release();
public:
	K_Object();  
	~K_Object();
};

