#pragma once
#include "BStd.h"

struct PC_VERTEX	//���ؽ��� �÷���
{
	D3DXVECTOR3 v;
	D3DXVECTOR4 c;
	//������
	PC_VERTEX() {}
	PC_VERTEX(D3DXVECTOR3 v0, D3DXVECTOR4 c0)
	{
		v = v0;
		c = c0;
	}
};

struct PNCT_VERTEX	//���ؽ� �븻 �÷��� �ؽ���
{
	D3DXVECTOR3 v;
	D3DXVECTOR3 n;
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
	//������
	PNCT_VERTEX() {}
	PNCT_VERTEX(D3DXVECTOR3 v0, D3DXVECTOR3 n0,D3DXVECTOR4 c0, D3DXVECTOR2 t0)
	{
		v = v0;
		n = n0;
		c = c0;
		t = t0;
	}
};

struct CB
{
	D3DXMATRIX matWorld;// c0						
	D3DXMATRIX matView;	// c4						
	D3DXMATRIX matProj;	// c8						
	D3DXVECTOR4 Color; // 12
};

//struct CB
//{
//	D3DXMATRIX matWVP;
//	//D3DXMATRIX matWorld; // c0 ~ c3
//	//D3DXMATRIX matView; // c0 ~ c3
//	//D3DXMATRIX matProj; // c0 ~ c3
//	float a; // c4
//	float fTime; // 4���
//	float c, d;
//};

class K_Object
{
public:
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXMATRIX m_matWorld;
	
	D3DXVECTOR3	m_vCenter;	//������ǥ

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
	//��Ƽ�ؽ���
	vector<ComPtr<ID3D11ShaderResourceView>>	m_pSRVArray;
	vector<basic_string<TCHAR>>	m_TextArray;
	//
	ID3D11InputLayout*  m_pIL;
	ID3D11InputLayout*	m_pVertexLayout;
	ID3DBlob*   m_pVSBuffer;

	std::vector<PNCT_VERTEX> m_VertexList;//���ؽ� ����Ʈ
	std::vector<DWORD>      	m_IndexList;//�ε��� ����Ʈ
public:
	//�����Լ�
	virtual bool MultiTextureLoad(TCHAR* pName);
	virtual bool Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		TCHAR* vs,
		TCHAR* ps,
		TCHAR* pTexName);
	virtual bool CreateInputLayout();
	virtual bool CreateIndexBuffer();
	virtual bool CreateVertexBuffer();
	bool CreateConstanceBuffer();
	bool LoadShader(TCHAR* vs, TCHAR* ps);//���̴��� �ε��ϴ� �Լ�
	virtual bool LoadTexture();//�ؽ��ĸ� �ε��ϴ� �Լ�

	virtual void SetMatrix(D3DXMATRIX* matWorld,
		D3DXMATRIX* matView, D3DXMATRIX* matProj);
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

