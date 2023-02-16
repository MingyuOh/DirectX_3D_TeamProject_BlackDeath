#pragma once
#include "BNode.h"
#include "BCamera.h"
#include "BMapLOD.h"

class BQuadTree : public BMapLOD
{
public:
	BNode* m_pRootNode;
	float m_fWidth;  // 쿼드트리의 가로범위
	float m_fHeight; // 쿼드트리의 세로범위
	int m_iMaxDepth; // 리프노드의 레벨
	int m_iMaxDepthLimit; // 노드 분할의 최대한도
	int m_iRenderDepth; // 최대 랜더 횟수
	float m_fMinDivideSize; // 최소 분할크기

	//vector<BNode*> m_DrawNodeList;

	//맵 아이템 위치저장용 변수
	vector<D3DXVECTOR3> m_ItemPosition;

	vector<int> m_DrawObjList;	
	queue<BNode*> m_QuadTreeQueue;

	vector<MapObjInfo> m_DrawMapObjList;
	int m_iTotalMapObj;
	/*높이맵용 변수*/
	bool m_bUsedIndexList; // 정점 인덱스를 사용하여 고간분할
public:
	bool Build(float fWidth, float fHeight);
	virtual bool BuildTree(BNode* pNode);
	BNode* CreateNode(BNode* pParentNode, float fLeft, float fRight, float fBottom, float fTop); // 노드 생성함수
	virtual bool SubDivide(BNode* pNode); // 분할 가능여부를 판별하는 함수
	int AddObject(); // 쿼드트리 내부 오브젝트 추가
	int CheckRect(BNode* pNode, BObject* pObj);// 내부에 오브젝트 유무 판별
	BNode* FindNode(BNode* pNode, D3DXVECTOR3 pObj); // 오브젝트가 어떤 노드에 들어가 있는지 분별하는 함수
	virtual void DrawFindNode(BNode* pNode, BCamera* pCamera); // 프로스텀 안에 들어온 노드인지 확인하는 함수
	void VisibleObject(BNode* pNode, BCamera* pCamera); // 랜더해야할 오브젝트를 따로 모으는 함수
	void VisibleNode(BNode* pNode, BCamera* pCamera);
	int CheckPosition(BNode* pNode, D3DXVECTOR3 pObj);
	void ClearObject(BNode* pNode);

	virtual bool Frame(BCamera* pCamera);
	virtual bool Release();

	/*높이맵용 함수*/
	void SetNeighborNode(BNode* pNode);
	void FindNeighborNode(BNode* pNode);
	DWORD CheckSize(DWORD dwSize);

	// 맵 오브젝트용 함수
	void AddMapObject(BNode* pNode, vector<InstancingInfo> MapInfo);

public:
	BQuadTree();
	virtual ~BQuadTree();
};

