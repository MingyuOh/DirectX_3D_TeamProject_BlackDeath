#pragma once
#include "BWindowLib.h"
#include "BObjStd.h"

class BObject
{
public:
	vector<T_STR>  m_szTextureName;   // �ؽ��� �̸� �迭
	int            m_iTextureCount;   // �ؽ��� ����
	vector<PNCT_Vertex> m_VertexList;			 // ���ؽ� ����
	vector<DWORD> m_IndexList;					 // ���ؽ� �ε���
	UINT m_VertexCnt;							 // ���ؽ� ����
	VSConstantB m_cbData;						 // ������� �ɹ�
	B_BOX m_tBox;								 // �ִ� �ּҰ�

	ID3D11Buffer*			m_pVertexBuffer;	 // ���ؽ� ����
	ID3D11Buffer*			m_pIndexBuffer;		 // �ε��� ����
	ID3D11Buffer*			m_pConstantBuffer;   // ��� ����
	ID3D11InputLayout*		m_pVertexLayout;	 // ���ؽ� ���̾ƿ�
	
	vector<D3DXVECTOR3>		m_TangentList;		// �������� ����Ʈ
	ComPtr<ID3D11Buffer>	m_pTangentBuffer;	// �������� ����
	UINT m_TangentCnt;

	//ID3D11ShaderResourceView* m_pTextureSRV; // �ؽ��� ���ҽ� ��
	//vector<ComPtr<ID3D11ShaderResourceView>> m_pTextureSRV;
	//vector<ComPtr<ID3D11ShaderResourceView>> m_pNormalTextureSRV;
	INT m_iCurIndex; // �ؽ��� �ε����� ����

	ID3D11VertexShader*		m_pVS; // ���ؽ� ���̴�
	ID3D11PixelShader*		m_pPS; // �ȼ� ���̴�
	ID3D11GeometryShader*	m_pGS; // ���� ���̴�

	ID3D11Device* m_pd3dDevice;					 // ����̽� �������̽�
	ID3D11DeviceContext* m_pContext;

	D3DXMATRIX m_matWorld;  // ������ǥ (���Ͱ�) * ���� ��� = ������ǥ
	D3DXMATRIX m_matView;	// ������ǥ * ����� = ����ǥ (ī�޶��� �̵��� ���� ��ǥ)
	D3DXMATRIX m_matProj;	// ����ǥ * ������� = ������ǥ (ī�޶��� ������ ���� ��ǥ)

	D3DXMATRIX m_matTrans;
	D3DXMATRIX m_matRotate;
	D3DXMATRIX m_matScale;


	/*������Ʈ�� ��ġ ����*/
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vTarget;
	D3DXVECTOR3 m_vUp;

	/*��ġ�� ��Ÿ���� ���⺤��*/
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vSide;

	/*������Ʈ ȸ�� ����*/
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;
	float m_SpeedUp;

	// �������� ����
	UINT m_iPrimitiveType;
	int m_iNumFace;     // �� �ﰢ���� ����
	int m_iNumVertices; // �� ���ؽ��� ����
	vector<int> m_Normallookuptable;
	vector<D3DXVECTOR3> m_FaceNormals; // �����븻

public:
	void UpdateMinMaxBox();
	virtual HRESULT Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, TCHAR** pLoadTextureString = NULL, int TextureCnt= 1);	// ���� ���� �Լ�
	HRESULT Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString);
	virtual HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);							// ���� ���̾ƿ����� ���� �� �ȼ� ���̴� �ε� �� ����
	virtual HRESULT CreateVertexBuffer();		// ���ؽ� ���� ����
	virtual HRESULT CreateIndexBuffer();		// �ε��� ���� ����
	virtual HRESULT CreateConstantBuffer();		// ��� ���� ����
	virtual HRESULT TextureLoad(ID3D11Device* pDevice, const TCHAR* strFilePath); // ���� �ؽ��� �ε�
	virtual HRESULT TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePathes[], int strCount); // ���� �ؽ��� �ε�;
	HRESULT TextureLoad(ID3D11Device* pDevice, vector<T_STR> strFilePathes);
	virtual bool UpdateBuffer(); // ���� ���ſ� �Լ�

	virtual void SetMatrix(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj);
	virtual void UpdateVertexMinMax() { return; }; // m_tBox ���� �Լ�

	/*������Ʈ �̵��Լ�*/
	virtual bool UpdateVector();
	virtual bool UpdateQuaternion();

	

	virtual bool Init();
	virtual bool Frame();
	virtual bool PostFrame();
	bool OMSetState(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* m_pRTV, ID3D11DepthStencilView* m_pDSV, ID3D11DepthStencilState* m_pDSS, ID3D11BlendState* m_pBS);
	virtual bool PreRender(ID3D11DeviceContext* pContext);
	virtual bool PostRender(ID3D11DeviceContext* pContext);
	virtual bool Render(ID3D11DeviceContext* pContext);
	virtual bool Release();

	//�������� ������ �Լ�
	void UpDateTangentSpaceVectors(D3DXVECTOR3 *v0, D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR2 uv0, D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR3 *vTangent, D3DXVECTOR3 *vBiNormal, D3DXVECTOR3 *vNormal);
	virtual bool UpDateTangentBuffer();
	virtual bool InitFaceNormal();
	virtual void GenNormalLookupTable();
	virtual void CalcPerVertexNormalsFastLookup();
	virtual bool UpdateNormal();

public:
	BObject();
	virtual ~BObject();
};
