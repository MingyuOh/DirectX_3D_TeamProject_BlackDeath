#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "BParser.h"
#include "BObject.h"

const enum OBJECTCLASSTYPE {
	CLASS_GEOM = 0,
	CLASS_BONE,
	CLASS_DUMMY,
	CLASS_BIPED,
};

static TCHAR* AseSectionHeaderTokens[] =
{
	_T("*SCENE"),
	_T("*MATERIAL_LIST"),
	_T("*GEOMOBJECT"),		// 리턴용
	_T("*HELPEROBJECT")		// 리턴용
};
enum AseSectionType { SCENE, MATERIAL_LIST, GEOM_OBJECT, HELPER_OBJECT,};

static TCHAR* AseMaterialTokens[] =
{
	_T("*MATERIAL"),
	_T("*SUBMATERIAL"),
	_T("*MAP_SUBNO"),
	_T("*BITMAP"),
	_T("*GEOMOBJECT"),		// 리턴용
	_T("*HELPEROBJECT"),	// 리턴용
};
enum AseMaterialType { MATERIAL = 0, SUBMATERIAL, MAP_SUBNO, BITMAP_FILE, RETURN_GEOM_OBJECT, RETURN_HELPER_OBJECT,};

static TCHAR* AseObjectTokens[] = 
{
	_T("*NODE_NAME"),
	_T("*NODE_PARENT"),
	_T("*NODE_TM"),
	_T("*MESH"),
	_T("*TM_ANIMATION"),
	_T("*TM_VISIBILITY"),
	_T("*MATERIAL_REF"),
	_T("*GEOMOBJECT"),		// 리턴용
	_T("*HELPEROBJECT"),	// 리턴용
};
enum AseObjectType {NODE_NAME = 0, PARENT_NAME, NODE_TM, MESH, TM_ANIMATION, TM_VISIBILITY,	MATERIAL_REF, NEW_GEOM_OBJECT, NEW_HELPER_OBJECT,};

static TCHAR* AseHelperObjectTokens[] =
{
	_T("*NODE_NAME"),
	_T("*NODE_PARENT"),
	_T("*HELPER_CLASS"),
	_T("*NODE_TM"),
	_T("*BOUNDINGBOX_MIN"),
	_T("*TM_ANIMATION"),

	_T("*GEOMOBJECT"),	// Return
	_T("*HELPEROBJECT"), // Return
};
enum AseHelperObjectType {HELPER_NODE_NAME = 0, HELPER_NODE_PARENT, HELPER_CLASS, HELPER_MODE_TM,	BOUNDINGBOX_MIN, HELPER_TM_ANIMATION, HELPER_GEOM_OBJECT, HELPER_HELPER_OBJECT};

static TCHAR* AseAniTokens[] =
{
	_T("*CONTROL_POS_SAMPLE"),
	_T("*CONTROL_ROT_SAMPLE"),
	_T("*CONTROL_SCALE_SAMPLE"),
	_T("*MATERIAL_REF"),
	_T("*GEOMOBJECT"),		// 리턴용
	_T("*HELPEROBJECT"),	// 리턴용
};
enum AseAniType { CONTROL_POS_SAMPLE = 70, CONTROL_ROT_SAMPLE, CONTROL_SCL_SAMPLE, TM_MATERIAL_REF, NEW_GEOM, NEW_HELPER,};


/*애니메이션 정보 구조체*/
struct BScene
{
	int iVersion;		// 버전
	int	iFirstFrame;    // 시작 프레임
	int	iLastFrame;     // 마지막 프레임
	int	iFrameSpeed;    // 1초당 프레임 개수(30)
	int	iTickPerFrame;  // 1프레임의 틱 값(160)
	int iNumMesh;		// 메쉬오브젝트 개수
	int iMaxWeight;		// 정점 당 가중치
	int iBindPose;		// 바인딩 포즈 애니메이션 여부
};

/*맵 구조체*/
struct BTextexMap
{
	DWORD m_dwIndex; // 오브젝트 텍스쳐 관리자에서 메터리얼의 텍스쳐 파일명을 확인 후 DX텍스쳐 생성, 참조 인덱스 저장
	DWORD m_dwType;  // 텍스쳐 맵 타입 인덱스
	T_STR m_strName; // 맵 이름
	T_STR m_strClassName;	 // 맵 클래스 이름
	T_STR m_strTextureName;  // 맵 텍스쳐 이름
	BTextexMap() : m_dwType(1), m_dwIndex(0) {}
};

/*메터리얼 정보 구조체*/
struct BMtrl
{
	T_STR m_strName;		 // 오브젝트명
	T_STR m_strClassName;	 // 오브젝트 클래스(종류) 명
	vector<BTextexMap> m_TexMaps; // 맵 타입들
	vector<BMtrl> m_SubMaterial; // 서브 메터리얼 저장벡터
	DWORD m_dwSubCount; // 서브 메터리얼 수
	DWORD m_dwTexMapCount; // 맵타입 수
	DWORD m_dwIndex; // 오브젝트 순서

	BMtrl() {}
	~BMtrl() { stl_wipe_vector(m_SubMaterial); stl_wipe_vector(m_TexMaps); }
};

struct BFaceList
{
	union
	{
		struct { DWORD _0, _1, _2; };
		DWORD v[3];
	};
	DWORD dwMtrl;

	BFaceList() : dwMtrl(0), _0(0), _1(0), _2(0) {}
};


/*버택스 리스트 구조체*/
struct BVertexList
{
	DWORD dwNumVertex;
	DWORD dwNumFace;
	vector<D3DXVECTOR3> pVertexList;
	vector<BFaceList> pFaceList;
	vector<DWORD> pSubListMtl;

	BVertexList() : dwNumVertex(0), dwNumFace(0) {}
	~BVertexList() { stl_wipe_vector(pVertexList); stl_wipe_vector(pVertexList); }
};

///*HelperObject 구조체*/
//struct B_BOX
//{
//	D3DXVECTOR3 vCenter;
//	D3DXVECTOR3 vPos[8];
//
//	D3DXVECTOR3 vMax;
//	D3DXVECTOR3 vMin;
//
//	D3DXVECTOR3 vAxis[3];
//	float fExtent[3];
//};

/*오브젝트 정보 구조체*/
struct BGeomMesh
{
	T_STR m_strNodeName;
	T_STR m_strParentName;
	OBJECTCLASSTYPE m_ClassType;
	D3DXMATRIX m_matWorld;
	DWORD m_iNumFace;
	int m_iMtrlRef;
	BVertexList			m_PosVertexList;
	BVertexList			m_TexVertexList;
	BVertexList			m_ColVertexList;
	vector<D3DXVECTOR3>	m_pNorVertexList;

	D3DXMATRIX m_matWorldTrans;
	D3DXMATRIX m_matWorldRotate;
	D3DXMATRIX m_matWorldScale;

	vector<BAnimTrack>		m_PosTrack;		// 이동 트랙 
	vector<BAnimTrack>		m_RotTrack;		// 회전 트랙
	vector<BAnimTrack>		m_SclTrack;		// 신축 트랙												
	vector<BAnimTrack>		m_VisTrack;		// 알파 트랙

	B_BOX m_BBox;

	BGeomMesh()
	{
		m_iNumFace = 0;
		m_iMtrlRef = -1;
	}
};

class BMesh2
{
public:
	int							m_iDiffuseTex[10];
	D3DXMATRIX					m_matWorld;
	T_STR						m_strNodeName;
	T_STR						m_strParentName;
	int							m_iNumFace;
	vector<PNCT_Vertex>		m_FaceList;
	vector<PNCT_Vertex>		m_VertexArray;
	vector<DWORD>				m_IndexArray;
	ComPtr<ID3D11Buffer>		m_pVertexBuffer;
	ComPtr<ID3D11Buffer>		m_pIndexBuffer;
	//vector<ComPtr<ID3D11ShaderResourceView>> m_pTextureSRV;
	//vector<ComPtr<ID3D11ShaderResourceView>> m_pNormalTextureSRV;
	vector<shared_ptr<BMesh2>>	m_pSubMesh;

	bool					m_bUsed; // 애니메이션용 변수
	BMesh2*					m_pParent;
	vector<BMesh2*>			m_pChildMesh;
	D3DXMATRIX				m_matInverse;
	OBJECTCLASSTYPE			m_ClassType;

	D3DXMATRIX m_matWorldScale;
	D3DXMATRIX m_matWorldTrans;
	D3DXMATRIX m_matWorldRotate;
	D3DXMATRIX m_matCalculation;

	vector<shared_ptr<BAnimTrack>> m_pPosTrack;
	vector<shared_ptr<BAnimTrack>> m_pRotTrack;
	vector<shared_ptr<BAnimTrack>> m_pSclTrack;
	vector<shared_ptr<BAnimTrack>> m_pVisTrack;
	float m_fVisibility;
	
	int IsEqulVertexList(vector<PNCT_Vertex>&  VertexArray, PNCT_Vertex& Vertex);
	void SetUniqueBuffer();

	vector<D3DXVECTOR3>		m_TangentList;		// 접선벡터 리스트
	ComPtr<ID3D11Buffer>	m_pTangentBuffer;	// 접선벡터 버퍼
	vector<D3DXVECTOR3>		m_FaceNormals;		// 정점노말
	vector<int> m_Normallookuptable;			// 룩업테이블
	
	B_BOX m_tBox;
	BMesh2::BMesh2()
	{
		//m_iDiffuseTex = -1;
		memset(m_iDiffuseTex, -1, sizeof(m_iDiffuseTex));
		D3DXMatrixIdentity(&m_matWorld);
		m_pParent = 0;
		D3DXMatrixIdentity(&m_matWorldTrans);
		D3DXMatrixIdentity(&m_matWorldRotate);
		D3DXMatrixIdentity(&m_matWorldScale);
		D3DXMatrixIdentity(&m_matCalculation);
	}
};

class BAseObj : public BObject
{
public:
	T_STR				m_szDirName; // 경로명
	T_STR				m_szName;	 // 파일명
	BParser				m_Parser;	 // Ase 해석을 위한 클래스
	BScene				m_Scene;	 // 애니메이션 정보
	vector<BMtrl>		m_Material;	  // 메터리얼 정보
	vector<BGeomMesh>	m_GeomObject; // 오브젝트 정보
	vector<shared_ptr<BMesh2>> m_pData;

	/*오브젝트 애니메이션용 변수*/
	float m_fElapseTime;
	int m_iStartFrame;
	int m_iLastFrame;

	// 인스턴싱용 변수
	ComPtr<ID3D11Buffer> m_BufferInstance;
	vector<D3DXMATRIX> m_pInstanceList;
	vector<D3DXMATRIX> m_pInvInstaneList;
	vector<B_BOX> m_tBoundingBox;
	vector<InstancingInfo> m_InstanceInfo;
	int m_iInstanceCount;

	vector<int> m_DrawInstanceList;
	vector<D3DXMATRIX> m_pDrawInstanceList;

	// 노말맵핑용 변수
	NORMAL_CONSTANT_BUFFER  m_cbNormal;
	ID3D11Buffer*			m_pNormalCB;

	D3DXMATRIX m_matShadowView;
	D3DXMATRIX m_matShadowProj;
	D3DXMATRIX m_matShadowTex;

public:
	bool Init();
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext, int DrawCount);
	bool Release();

	/*파일을 읽는 함수*/
	bool Load(ID3D11Device* pd3dDevice, const TCHAR* strFileName, const TCHAR* strShaderName = NULL);
	bool Load(const TCHAR* strFileName, ID3D11Device* pd3dDevice);
	int SearchTokenArray(INT NumObjType, TCHAR** pObjectType);

	/*읽어온 파일을 오브젝트로 생성하는 함수*/
	bool Convert(ID3D11Device* pd3dDevice);
	bool InheriteCollect();
	BMesh2* SearchToCollects(T_STR m_strParentName);


	/*읽어온 파일 정리 함수*/
	bool LoadScene();
	bool LoadMaterial();
	bool LoadObject();
	bool LoadHelperObject();
	bool LoadTM(BGeomMesh* pObject);
	bool LoadAnimation(BGeomMesh* pGeomObj);


	/*오브젝트 로드에 사용되는 함수*/
	bool GetVertexListFromString(TCHAR* strVertexList,DWORD dwNumFace,vector<D3DXVECTOR3>& VertexList,TCHAR* strToken = _T("*"));
	bool GetFaceListFromString(TCHAR* strFaceList,DWORD dwNumFace,BVertexList& VertexList,TCHAR* strToken = _T("*"));
	bool GetPositionFaceListFromString(TCHAR* strFaceList,DWORD dwNumFace,BVertexList& VertexList,TCHAR* strToken = _T("*"));
	bool GetNormalListFromString(TCHAR* strFaceList,DWORD dwNumFace,BGeomMesh* GeomObject,TCHAR* strToken = _T("*"));

	/*랜더용 함수*/
	void DrawMesh(ID3D11DeviceContext* pContext, BMesh2* pMesh);

	/* 인덱스에 대한 최적화 */
	bool SetTriangleBuffer(BMesh2* pMesh, BGeomMesh* pAseMesh, DWORD dwSubMtrl = 0, DWORD dwMask = 0x00000000);
	bool SetTriangleSubBuffer(vector<shared_ptr<BMesh2>>& pSubMesh, BGeomMesh* pAseMesh);

	/*랜더용 함수*/
	HRESULT Create(ID3D11Device* pDevice);
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT TextureLoad(ID3D11Device* pDevice);
	bool SetCamera(D3DXMATRIX Camera_matView, D3DXMATRIX Camera_matProj);
	void SetMatrix(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj);
	bool Draw(ID3D11DeviceContext* pContext, BAseObj* pParent);

	/*텍스쳐 로드 함수*/
	HRESULT TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePath);
	HRESULT TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePathes[], int strCount);

	/*애니메이션용 함수*/
	BAnimTrack* SetDoublyLinkedList(BAnimTrack* pCurrentTrack, BAnimTrack* pPrev);
	void Interpolate(BMesh2* pMesh, D3DXMATRIX* matParent, float fFrameTick);
	bool GetAnimationTrack(float fFrame, vector<shared_ptr<BAnimTrack>> pTrackList, BAnimTrack** pStartTrack, BAnimTrack** pEndTrack);
	// 맵툴용 함수
	bool CreateBox();
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile); // 인스턴싱용
	void InitInstanceBuffer(int iInstanceCount);
	HRESULT CreateInstanceBuffer(UINT ListSize, UINT InstnaceSize); // m_pInstatnceList, (UINT)m_ParticleList.size(), sizeof(TInstance)
	bool DrawInstance(ID3D11DeviceContext* pContext, BAseObj* pParent, int DrawCount);
	void UpdateInstanceInfo(ID3D11DeviceContext* pContext, vector<InstancingInfo> InstanceInfo, BMesh2* MeshData);

	// 노말맵핑용 함수
	HRESULT CreateNormalCB();
	void SettingCameraForNormal(D3DXVECTOR3 Pos, D3DXVECTOR3 Look);
	bool UpDateTangentBuffer();
	bool InitFaceNormal();
	void GenNormalLookupTable();
	void CalcPerVertexNormalsFastLookup();
	void NormalTextureLoad(ID3D11Device* pDevice, TCHAR* strFilePath);

public:
	BAseObj();
	virtual ~BAseObj();
};

