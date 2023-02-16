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
	//�ӽ�����
	ID3D11Device* m_pCoreDevice;
	ID3D11DeviceContext* m_pCoreContext;

	//����Ʈ
	std::vector<PNCT_VECTOR> m_VertexList;
	std::vector<DWORD> m_IndexList;
	LASpaceObject2 m_SpaceObject;

	//����
	ID3D11Buffer* m_pVertexBuffer;		//���ؽ�����
	ID3D11Buffer* m_pIndexBuffer;		//�ε�������
	ID3D11Buffer* m_pConstantBuffer;	//�������01
	ID3D11Buffer* m_pConstantBufferData;	//�������02
	/*Microsoft::WRL::ComPtr<ID3D11Buffer> m_StreamTo;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_DrawFrom;*/
	ID3D11Buffer* m_StreamTo;
	ID3D11Buffer* m_DrawFrom;

	ID3D11VertexShader* m_pVS;				//���ؽ� ���̴�
	ID3D11PixelShader* m_pPS;				//�ȼ� ���̴�
	ID3D11VertexShader* m_pResourceVS;				//���ؽ� ���̴�
	ID3D11PixelShader* m_pResourcePS;				//�ȼ� ���̴�
	ID3D11PixelShader* m_pLightPS;			//�ȼ� ����Ʈ���� ���̴�
	ID3D11VertexShader* m_pNormalVS;		//���ؽ� �븻�� ���̴�
	ID3D11PixelShader* m_pNormalPS;			//�ȼ� �븻�� ���̴�
	ID3D11GeometryShader* m_pGS;			//������Ʈ�� ���̴�
	ID3D11PixelShader* m_pShadowPS;			//�ȼ� �븻�� ���̴�
	//Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pSO;
	ID3D11GeometryShader* m_pSO;
	ID3D11HullShader* pHullShader;
	ID3D11DomainShader* pDomainShader;

	Texture m_Texture;
	//ID3D11ShaderResourceView* m_pTextureSRV;	//Direct3d 11.0 ���̴� �� �������̽�
	ID3D11InputLayout* m_pVertexLayout;	//���̾ƿ�
	//����Ÿ ����
	TMatrix  m_ObjectWorld;	//����
	TMatrix  m_ObjectView;	//��
	TMatrix  m_ObjectProjection;	//����
	ID3DBlob* pVSBuf;
	ID3DBlob* pPSBuf;
	ID3DBlob* pGSBuf;
	VERTEX_CB m_ObjectCB;		//���
	DataCB m_ObjectDataCB;		//�׿� ������
	TMatrix m_Scale;		//������
	TMatrix m_Rotate;		//ȸ��
	TMatrix m_Move;	//�̵�
	TVector3 m_vUp;
	TVector3 m_vLook;
	TVector3 m_vSide;
	int m_iMultiTextureRender;
	CrateObjectDest m_ObjectDest;

	//Direction Light
	TVector4 m_vLightPos;
	TVector4 m_vLightDirection;
	TVector4 m_vCameraPos;

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
	void DrawLine(std::vector<PNCT5_VERTEX>& rVertexList, std::vector<DWORD>& riIndexList, int iDrawIndexListMaxSize, TMatrix matWorld);
	//Texture ���ҽ� ����
	virtual HRESULT LoadTextureResource();
	//Texture ���̴� ����
	HRESULT ApplyTexture();				
	//���̴� ���� ������
	virtual HRESULT LoadShader();
	//��� ���� ����
	virtual HRESULT CreateConstantBuffer();
	virtual HRESULT CreateVertexBuffer();		//���ؽ� ���� ����
	virtual HRESULT CreateIndexBuffer();		//�ε��� ���� ����
	virtual HRESULT CreateLayout();				//���̾ƿ� ��ü ����

	/*======= Set =======*/
	virtual void SetMatrix(TMatrix* pWorldMatrix, TMatrix* pViewMatrix, TMatrix* pProjectionMatrix);
	void SetPosition(TVector3 vPos);
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

