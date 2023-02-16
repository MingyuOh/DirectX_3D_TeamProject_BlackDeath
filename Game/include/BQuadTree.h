#pragma once
#include "BNode.h"
#include "BCamera.h"
#include "BMapLOD.h"

class BQuadTree : public BMapLOD
{
public:
	BNode* m_pRootNode;
	float m_fWidth;  // ����Ʈ���� ���ι���
	float m_fHeight; // ����Ʈ���� ���ι���
	int m_iMaxDepth; // ��������� ����
	int m_iMaxDepthLimit; // ��� ������ �ִ��ѵ�
	int m_iRenderDepth; // �ִ� ���� Ƚ��
	float m_fMinDivideSize; // �ּ� ����ũ��

	//vector<BNode*> m_DrawNodeList;

	//�� ������ ��ġ����� ����
	vector<D3DXVECTOR3> m_ItemPosition;

	vector<int> m_DrawObjList;	
	queue<BNode*> m_QuadTreeQueue;

	vector<MapObjInfo> m_DrawMapObjList;
	int m_iTotalMapObj;
	/*���̸ʿ� ����*/
	bool m_bUsedIndexList; // ���� �ε����� ����Ͽ� ��������
public:
	bool Build(float fWidth, float fHeight);
	virtual bool BuildTree(BNode* pNode);
	BNode* CreateNode(BNode* pParentNode, float fLeft, float fRight, float fBottom, float fTop); // ��� �����Լ�
	virtual bool SubDivide(BNode* pNode); // ���� ���ɿ��θ� �Ǻ��ϴ� �Լ�
	int AddObject(); // ����Ʈ�� ���� ������Ʈ �߰�
	int CheckRect(BNode* pNode, BObject* pObj);// ���ο� ������Ʈ ���� �Ǻ�
	BNode* FindNode(BNode* pNode, D3DXVECTOR3 pObj); // ������Ʈ�� � ��忡 �� �ִ��� �к��ϴ� �Լ�
	virtual void DrawFindNode(BNode* pNode, BCamera* pCamera); // ���ν��� �ȿ� ���� ������� Ȯ���ϴ� �Լ�
	void VisibleObject(BNode* pNode, BCamera* pCamera); // �����ؾ��� ������Ʈ�� ���� ������ �Լ�
	void VisibleNode(BNode* pNode, BCamera* pCamera);
	int CheckPosition(BNode* pNode, D3DXVECTOR3 pObj);
	void ClearObject(BNode* pNode);

	virtual bool Frame(BCamera* pCamera);
	virtual bool Release();

	/*���̸ʿ� �Լ�*/
	void SetNeighborNode(BNode* pNode);
	void FindNeighborNode(BNode* pNode);
	DWORD CheckSize(DWORD dwSize);

	// �� ������Ʈ�� �Լ�
	void AddMapObject(BNode* pNode, vector<InstancingInfo> MapInfo);

public:
	BQuadTree();
	virtual ~BQuadTree();
};
