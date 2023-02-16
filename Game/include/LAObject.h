#pragma once

#include "LAGlobalFuncLink.h"
#include "LASpaceObject.h"

class LAObject
{
public:
	//================================================================================
	//	** Create Variable ** 
	//================================================================================
	//�ӽ�����
	ID3D11Device* m_pCoreDevice;
	ID3D11DeviceContext* m_pCoreContext;

	//����Ʈ
	std::vector<PNCT_VECTOR> m_VertexList;
	std::vector<DWORD> m_IndexList;
	LASpaceObject m_SpaceObject;

	//����
	ID3D11Buffer* m_pVertexBuffer;		//���ؽ�����
	ID3D11Buffer* m_pIndexBuffer;		//�ε�������
	ID3D11Buffer* m_pConstantBuffer;	//�������01
	ID3D11Buffer* m_pConstantBufferData;	//�������02
	/*Microsoft::WRL::ComPtr<ID3D11Buffer> m_StreamTo;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DrawFrom;*/
	ID3D11Buffer* m_StreamTo;
	ID3D11Buffer* m_DrawFrom;

	ID3D11VertexShader* m_pVS;			//���ؽ� ���̴�
	ID3D11PixelShader* m_pPS;			//�ȼ� ���̴�
	ID3D11GeometryShader* m_pGS;		//������Ʈ�� ���̴�
	//Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pSO;
	ID3D11GeometryShader* m_pSO;

	ID3D11ShaderResourceView* m_pTextureSRV;	//Direct3d 11.0 ���̴� �� �������̽�
	ID3D11InputLayout* m_pVertexLayout;	//���̾ƿ�
	//����Ÿ ����
	D3DXMATRIX  m_ObjectWorld;	//����
	D3DXMATRIX  m_ObjectView;	//��
	D3DXMATRIX  m_ObjectProjection;	//����
	ID3DBlob* pVSBuf;
	ID3DBlob* pPSBuf;
	ID3DBlob* pGSBuf;
	VERTEX_CB m_ObjectCB;		//���
	DataCB m_ObjectDataCB;		//�׿� ������
	D3DXMATRIX m_Scale;		//������
	D3DXMATRIX m_Rotate;		//ȸ��
	D3DXMATRIX m_Move;	//�̵�
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vSide;
	int m_iMultiTextureRender;
	CrateObjectDest m_ObjectDest;

	//================================================================================
	//	** ��Ÿ �Լ� ** 
	//================================================================================
	//CheckRelease
	bool m_bCheckReleaseObject;
	
public:
	//================================================================================
	//	** Create  Function ** 
	//================================================================================
	//Create
	virtual void SetMatrix(D3DXMATRIX* pWorldMatrix, D3DXMATRIX* pViewMatrix, D3DXMATRIX* pProjectionMatrix);
	void SetPosition(D3DXVECTOR3 vPos);
	HRESULT Create(CrateObjectDest& ObjectDest);
	//��� ���� ����
	virtual HRESULT CreateConstantBuffer();		
	//Texture ���ҽ� ����
	virtual HRESULT LoadTextureResource();
	//Texture ���̴� ����
	HRESULT ApplyTexture();				
	//���̴� ���� ������
	virtual HRESULT LoadShader();
	virtual HRESULT CreateVertexBuffer();		//���ؽ� ���� ����
	virtual HRESULT CreateIndexBuffer();		//�ε��� ���� ����
	virtual HRESULT InputLayout();				//���̾ƿ� ��ü ����

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
	LAObject();
	~LAObject();
};

