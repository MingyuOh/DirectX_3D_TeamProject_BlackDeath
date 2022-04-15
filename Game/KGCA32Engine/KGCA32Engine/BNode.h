#pragma once
#include "BCamera.h"

struct MapObjInfo
{
	int m_iObjType; // 오브젝트의 종류
	int m_iObjNum;  // 오브젝트의 순서
};

class BNode
{
public:
	DWORD m_dwDepth;
	DWORD m_isLeaf;
	TVector3 m_vCorner[4];
	BNode* m_pChild[4];
	BNode* m_pParent;

	vector<int> m_ObjectList; // 무기 
	vector<MapObjInfo> m_vMapObjInfo; // 나무, 집등

	/*바운드 박스*/
	B_BOX m_tBox;
	vector <DWORD> m_CornerIndex;

	/*높이맵 쿼드트리용 변수*/
	DWORD m_dwPositionIndex[2];// X,Y	
	vector<BNode*>		m_NeighborList;

	D3D11_BOX			m_BoxVB;
	D3D11_BOX			m_BoxIB;
	UINT				m_iBeginVB;
	UINT				m_iBeginIB;
	vector<DWORD>		m_IndexList;
	vector<PNCT_Vertex>	m_VertexList;
	vector<TVector3> m_TangentList;
	
	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
	ComPtr<ID3D11Buffer>	m_pTangentBuffer;

	DWORD m_dwLodLevel;
	DWORD m_dwLodType;
public:
	BNode();
	~BNode();
};

