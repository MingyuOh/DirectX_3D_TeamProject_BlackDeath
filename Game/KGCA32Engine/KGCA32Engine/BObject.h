#pragma once
#include "BWindowLib.h"
#include "BObjStd.h"
#include "Texture.h"

class BObject
{
public:
	vector<T_STR>  m_szTextureName;   // 텍스쳐 이름 배열
	int            m_iTextureCount;   // 텍스쳐 개수
	vector<PNCT_Vertex> m_VertexList;			 // 버텍스 벡터
	vector<DWORD> m_IndexList;					 // 버텍스 인덱스
	UINT m_VertexCnt;							 // 버텍스 갯수
	VSConstantB m_cbData;						 // 상수버퍼 맴버
	B_BOX m_tBox;								 // 최대 최소값

	ID3D11Buffer*			m_pVertexBuffer;	 // 버텍스 버퍼
	ID3D11Buffer*			m_pIndexBuffer;		 // 인덱스 버퍼
	ID3D11Buffer*			m_pConstantBuffer;   // 상수 버퍼
	ID3D11InputLayout*		m_pVertexLayout;	 // 버텍스 레이아웃
	
	vector<TVector3>		m_TangentList;		// 접선벡터 리스트
	ComPtr<ID3D11Buffer>	m_pTangentBuffer;	// 접선벡터 버퍼
	UINT m_TangentCnt;

	//ID3D11ShaderResourceView* m_pTextureSRV; // 텍스쳐 리소스 뷰
	//vector<ComPtr<ID3D11ShaderResourceView>> m_pTextureSRV;
	//vector<ComPtr<ID3D11ShaderResourceView>> m_pNormalTextureSRV;
	INT m_iCurIndex; // 텍스쳐 인덱스용 변수

	ID3D11VertexShader*		m_pVS; // 버텍스 쉐이더
	ID3D11PixelShader*		m_pPS; // 픽셀 쉐이더
	ID3D11GeometryShader*	m_pGS; // 기하 쉐이더

	ID3D11Device* m_pd3dDevice;					 // 디바이스 인터페이스
	ID3D11DeviceContext* m_pContext;

	TMatrix m_matWorld;  // 로컬좌표 (백터값) * 월드 행렬 = 월드좌표
	TMatrix m_matView;	// 월드좌표 * 뷰행렬 = 뷰좌표 (카메라의 이동에 따른 좌표)
	TMatrix m_matProj;	// 뷰좌표 * 투영행렬 = 투영좌표 (카메라의 투영에 따른 좌표)

	TMatrix m_matTrans;
	TMatrix m_matRotate;
	TMatrix m_matScale;


	/*오브젝트의 위치 벡터*/
	TVector3 m_vPos;
	TVector3 m_vTarget;
	TVector3 m_vUp;

	/*위치를 나타내는 방향벡터*/
	TVector3 m_vLook;
	TVector3 m_vSide;

	/*오브젝트 회전 정보*/
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;
	float m_SpeedUp;

	// 랜더링용 변수
	UINT m_iPrimitiveType;
	int m_iNumFace;     // 총 삼각형의 갯수
	int m_iNumVertices; // 총 버텍스의 갯수
	vector<int> m_Normallookuptable;
	vector<TVector3> m_FaceNormals; // 정점노말

	Texture m_Texture;
	Texture m_NormalTexture;

public:
	void UpdateMinMaxBox();
	virtual HRESULT Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, TCHAR** pLoadTextureString = NULL, int TextureCnt= 1);	// 도형 생성 함수
	HRESULT Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString);
	virtual HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);							// 버퍼 레이아웃에서 정점 및 픽셀 쉐이더 로딩 및 생성
	virtual HRESULT CreateVertexBuffer();		// 버텍스 버퍼 생성
	virtual HRESULT CreateIndexBuffer();		// 인덱스 버퍼 생성
	virtual HRESULT CreateConstantBuffer();		// 상수 버퍼 생성
	virtual HRESULT TextureLoad(ID3D11Device* pDevice, const TCHAR* strFilePath); // 단일 텍스쳐 로드
	virtual HRESULT TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePathes[], int strCount); // 다중 텍스쳐 로드;
	HRESULT TextureLoad(ID3D11Device* pDevice, vector<T_STR> strFilePathes);
	HRESULT	LoadTexture(T_STR szFilePath);
	virtual bool UpdateBuffer(); // 버퍼 갱신용 함수

	virtual void SetMatrix(TMatrix World, TMatrix View, TMatrix Proj);
	virtual void UpdateVertexMinMax() { return; }; // m_tBox 갱신 함수

	/*오브젝트 이동함수*/
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

	//접선벡터 생성용 함수
	void UpDateTangentSpaceVectors(TVector3 *v0, TVector3 *v1, TVector3 *v2, TVector2 uv0, TVector2 uv1, TVector2 uv2, TVector3 *vTangent, TVector3 *vBiNormal, TVector3 *vNormal);
	virtual bool UpDateTangentBuffer();
	virtual bool InitFaceNormal();
	virtual void GenNormalLookupTable();
	virtual void CalcPerVertexNormalsFastLookup();
	virtual bool UpdateNormal();

public:
	BObject();
	virtual ~BObject();
};

