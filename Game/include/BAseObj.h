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
	_T("*GEOMOBJECT"),		// ���Ͽ�
	_T("*HELPEROBJECT")		// ���Ͽ�
};
enum AseSectionType { SCENE, MATERIAL_LIST, GEOM_OBJECT, HELPER_OBJECT,};

static TCHAR* AseMaterialTokens[] =
{
	_T("*MATERIAL"),
	_T("*SUBMATERIAL"),
	_T("*MAP_SUBNO"),
	_T("*BITMAP"),
	_T("*GEOMOBJECT"),		// ���Ͽ�
	_T("*HELPEROBJECT"),	// ���Ͽ�
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
	_T("*GEOMOBJECT"),		// ���Ͽ�
	_T("*HELPEROBJECT"),	// ���Ͽ�
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
	_T("*GEOMOBJECT"),		// ���Ͽ�
	_T("*HELPEROBJECT"),	// ���Ͽ�
};
enum AseAniType { CONTROL_POS_SAMPLE = 70, CONTROL_ROT_SAMPLE, CONTROL_SCL_SAMPLE, TM_MATERIAL_REF, NEW_GEOM, NEW_HELPER,};


/*�ִϸ��̼� ���� ����ü*/
struct BScene
{
	int iVersion;		// ����
	int	iFirstFrame;    // ���� ������
	int	iLastFrame;     // ������ ������
	int	iFrameSpeed;    // 1�ʴ� ������ ����(30)
	int	iTickPerFrame;  // 1�������� ƽ ��(160)
	int iNumMesh;		// �޽�������Ʈ ����
	int iMaxWeight;		// ���� �� ����ġ
	int iBindPose;		// ���ε� ���� �ִϸ��̼� ����
};

/*�� ����ü*/
struct BTextexMap
{
	DWORD m_dwIndex; // ������Ʈ �ؽ��� �����ڿ��� ���͸����� �ؽ��� ���ϸ��� Ȯ�� �� DX�ؽ��� ����, ���� �ε��� ����
	DWORD m_dwType;  // �ؽ��� �� Ÿ�� �ε���
	T_STR m_strName; // �� �̸�
	T_STR m_strClassName;	 // �� Ŭ���� �̸�
	T_STR m_strTextureName;  // �� �ؽ��� �̸�
	BTextexMap() : m_dwType(1), m_dwIndex(0) {}
};

/*���͸��� ���� ����ü*/
struct BMtrl
{
	T_STR m_strName;		 // ������Ʈ��
	T_STR m_strClassName;	 // ������Ʈ Ŭ����(����) ��
	vector<BTextexMap> m_TexMaps; // �� Ÿ�Ե�
	vector<BMtrl> m_SubMaterial; // ���� ���͸��� ���庤��
	DWORD m_dwSubCount; // ���� ���͸��� ��
	DWORD m_dwTexMapCount; // ��Ÿ�� ��
	DWORD m_dwIndex; // ������Ʈ ����

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


/*���ý� ����Ʈ ����ü*/
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

///*HelperObject ����ü*/
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

/*������Ʈ ���� ����ü*/
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

	vector<BAnimTrack>		m_PosTrack;		// �̵� Ʈ�� 
	vector<BAnimTrack>		m_RotTrack;		// ȸ�� Ʈ��
	vector<BAnimTrack>		m_SclTrack;		// ���� Ʈ��												
	vector<BAnimTrack>		m_VisTrack;		// ���� Ʈ��

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

	bool					m_bUsed; // �ִϸ��̼ǿ� ����
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

	vector<D3DXVECTOR3>		m_TangentList;		// �������� ����Ʈ
	ComPtr<ID3D11Buffer>	m_pTangentBuffer;	// �������� ����
	vector<D3DXVECTOR3>		m_FaceNormals;		// �����븻
	vector<int> m_Normallookuptable;			// ������̺�
	
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
	T_STR				m_szDirName; // ��θ�
	T_STR				m_szName;	 // ���ϸ�
	BParser				m_Parser;	 // Ase �ؼ��� ���� Ŭ����
	BScene				m_Scene;	 // �ִϸ��̼� ����
	vector<BMtrl>		m_Material;	  // ���͸��� ����
	vector<BGeomMesh>	m_GeomObject; // ������Ʈ ����
	vector<shared_ptr<BMesh2>> m_pData;

	/*������Ʈ �ִϸ��̼ǿ� ����*/
	float m_fElapseTime;
	int m_iStartFrame;
	int m_iLastFrame;

	// �ν��Ͻ̿� ����
	ComPtr<ID3D11Buffer> m_BufferInstance;
	vector<D3DXMATRIX> m_pInstanceList;
	vector<D3DXMATRIX> m_pInvInstaneList;
	vector<B_BOX> m_tBoundingBox;
	vector<InstancingInfo> m_InstanceInfo;
	int m_iInstanceCount;

	vector<int> m_DrawInstanceList;
	vector<D3DXMATRIX> m_pDrawInstanceList;

	// �븻���ο� ����
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

	/*������ �д� �Լ�*/
	bool Load(ID3D11Device* pd3dDevice, const TCHAR* strFileName, const TCHAR* strShaderName = NULL);
	bool Load(const TCHAR* strFileName, ID3D11Device* pd3dDevice);
	int SearchTokenArray(INT NumObjType, TCHAR** pObjectType);

	/*�о�� ������ ������Ʈ�� �����ϴ� �Լ�*/
	bool Convert(ID3D11Device* pd3dDevice);
	bool InheriteCollect();
	BMesh2* SearchToCollects(T_STR m_strParentName);


	/*�о�� ���� ���� �Լ�*/
	bool LoadScene();
	bool LoadMaterial();
	bool LoadObject();
	bool LoadHelperObject();
	bool LoadTM(BGeomMesh* pObject);
	bool LoadAnimation(BGeomMesh* pGeomObj);


	/*������Ʈ �ε忡 ���Ǵ� �Լ�*/
	bool GetVertexListFromString(TCHAR* strVertexList,DWORD dwNumFace,vector<D3DXVECTOR3>& VertexList,TCHAR* strToken = _T("*"));
	bool GetFaceListFromString(TCHAR* strFaceList,DWORD dwNumFace,BVertexList& VertexList,TCHAR* strToken = _T("*"));
	bool GetPositionFaceListFromString(TCHAR* strFaceList,DWORD dwNumFace,BVertexList& VertexList,TCHAR* strToken = _T("*"));
	bool GetNormalListFromString(TCHAR* strFaceList,DWORD dwNumFace,BGeomMesh* GeomObject,TCHAR* strToken = _T("*"));

	/*������ �Լ�*/
	void DrawMesh(ID3D11DeviceContext* pContext, BMesh2* pMesh);

	/* �ε����� ���� ����ȭ */
	bool SetTriangleBuffer(BMesh2* pMesh, BGeomMesh* pAseMesh, DWORD dwSubMtrl = 0, DWORD dwMask = 0x00000000);
	bool SetTriangleSubBuffer(vector<shared_ptr<BMesh2>>& pSubMesh, BGeomMesh* pAseMesh);

	/*������ �Լ�*/
	HRESULT Create(ID3D11Device* pDevice);
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT TextureLoad(ID3D11Device* pDevice);
	bool SetCamera(D3DXMATRIX Camera_matView, D3DXMATRIX Camera_matProj);
	void SetMatrix(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj);
	bool Draw(ID3D11DeviceContext* pContext, BAseObj* pParent);

	/*�ؽ��� �ε� �Լ�*/
	HRESULT TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePath);
	HRESULT TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePathes[], int strCount);

	/*�ִϸ��̼ǿ� �Լ�*/
	BAnimTrack* SetDoublyLinkedList(BAnimTrack* pCurrentTrack, BAnimTrack* pPrev);
	void Interpolate(BMesh2* pMesh, D3DXMATRIX* matParent, float fFrameTick);
	bool GetAnimationTrack(float fFrame, vector<shared_ptr<BAnimTrack>> pTrackList, BAnimTrack** pStartTrack, BAnimTrack** pEndTrack);
	// ������ �Լ�
	bool CreateBox();
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile); // �ν��Ͻ̿�
	void InitInstanceBuffer(int iInstanceCount);
	HRESULT CreateInstanceBuffer(UINT ListSize, UINT InstnaceSize); // m_pInstatnceList, (UINT)m_ParticleList.size(), sizeof(TInstance)
	bool DrawInstance(ID3D11DeviceContext* pContext, BAseObj* pParent, int DrawCount);
	void UpdateInstanceInfo(ID3D11DeviceContext* pContext, vector<InstancingInfo> InstanceInfo, BMesh2* MeshData);

	// �븻���ο� �Լ�
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
