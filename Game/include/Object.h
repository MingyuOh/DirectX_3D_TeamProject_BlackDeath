#pragma once
#include "BObjStd.h"

class Object
{
public:
	//--------------------------------------------------------------------------------------
	// ���ؽ� ���� �� ���̾ƿ�
	//--------------------------------------------------------------------------------------
	ID3D11InputLayout*							m_pVertexLayout;
	ID3D11Buffer*								m_pVertexBuffer;
	ID3D11Buffer*								m_pIndexBuffer;
	ID3D11Buffer*								m_pConstantBuffer;
	//--------------------------------------------------------------------------------------
	// ���ؽ� �� �ȼ� ���̴�
	//--------------------------------------------------------------------------------------
	ID3D11VertexShader*							m_pVS;
	ID3D11PixelShader*							m_pPS;
	ID3D11GeometryShader*						m_pGS;
	//--------------------------------------------------------------------------------------
	// Vertex List
	//--------------------------------------------------------------------------------------
	vector<PNCT_Vertex>							m_VertexList;
	vector<DWORD>								m_IndexList;
	VSConstantB									m_cbData;
	UINT										m_iNumVertex;
	UINT										m_iNumIndex;
	//--------------------------------------------------------------------------------------
	// �ؽ��� �ε� �� ���ҽ����� ����
	//--------------------------------------------------------------------------------------
	vector<ComPtr<ID3D11ShaderResourceView>>	m_pTextureSRV;
	//--------------------------------------------------------------------------------------
	// �ؽ��� �̸� / ����
	//--------------------------------------------------------------------------------------
	vector<basic_string<TCHAR>>					m_szTextureName;
	int											m_iTextureCount;
	//--------------------------------------------------------------------------------------
	// ��, ���� ��� 
	//--------------------------------------------------------------------------------------
	D3DXMATRIX									m_matWorld;
	D3DXMATRIX									m_matView;
	D3DXMATRIX									m_matProj;
	//--------------------------------------------------------------------------------------
	// ������Ʈ ��ġ
	//--------------------------------------------------------------------------------------
	D3DXVECTOR3									m_vPos;
	D3DXVECTOR3									m_vLook;
	//--------------------------------------------------------------------------------------
	// ����̽� ��ġ
	//--------------------------------------------------------------------------------------
	ID3D11Device*								m_pd3dDevice;
	ID3D11DeviceContext*						m_pContext;
public:
	//--------------------------------------------------------------------------------------
	// �ﰢ�� ���� �� �ʱ�ȭ
	//--------------------------------------------------------------------------------------
	virtual		HRESULT			Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
									    TCHAR* TextureName = NULL, TCHAR* vertexShaderName = NULL, 
									    TCHAR* PixelShaderName = NULL, TCHAR* geometryShaderName = NULL);
	//--------------------------------------------------------------------------------------
	// ���� ���� ����
	//--------------------------------------------------------------------------------------
	virtual		HRESULT			CreateVertexBuffer();
	//--------------------------------------------------------------------------------------
	// �ε��� ���� ����
	//--------------------------------------------------------------------------------------
	virtual		HRESULT			CreateIndexBuffer();
	//--------------------------------------------------------------------------------------
	// ��� ���� ����
	//--------------------------------------------------------------------------------------
	virtual		HRESULT			CreateConstantBuffer();
	//--------------------------------------------------------------------------------------
	// ���� �� �ȼ� ���̵� �ε� �� ����
	//--------------------------------------------------------------------------------------
	virtual		HRESULT			LoadShaderAndInputLayout(TCHAR* vertexShaderName = NULL,
											 TCHAR* PixelShaderName = NULL, TCHAR* geometryShaderName = NULL);
	//--------------------------------------------------------------------------------------
	// �ؽ�ó �ҷ����� / ����
	//--------------------------------------------------------------------------------------
	HRESULT						LoadTexture(ID3D11Device* pDevice, TCHAR* strFilePath, int iIndex = 0);
	bool						ApplyTexture(ID3D11DeviceContext* pImmediateContext, int iIndex = 0);
	//--------------------------------------------------------------------------------------
	// ���(��, ����) ����
	//--------------------------------------------------------------------------------------
	bool						SetMatrix(D3DXMATRIX* matWorld, D3DXMATRIX* matView, D3DXMATRIX* matProj);
	//--------------------------------------------------------------------------------------
	// ������Ʈ ��ġ ����
	//--------------------------------------------------------------------------------------
	void						SetPosition(D3DXVECTOR3 vPos);
public:
	bool						Init();
	bool						Frame();
	virtual bool				Render();
	bool						Release();
public:
	Object();
	~Object();
};
