#pragma once
#include "BNode.h"
typedef vector<BNode*> BNODE_VECTOR;


class BMapLOD
{
public:
	ID3D11Device* m_pd3dDevice;
	BCamera* m_pCamera;
	vector<BNODE_VECTOR> m_LevelList;
	vector<BNode*> m_DrawPatchNodeList;
	vector<BNode*> m_DrawNodeList;

	// LOD 비율 (화면 투영 비율( 0 ~ 1 범위 사용 ))
	float m_fExpansionRatio;
	// 트리의 레벨의 깊이에 따른 셀 개수
	int m_iNumCell;
	// LOD 레벨 개수 ( z = pow (x,y) 에서 y = log(z) / log(x) )
	int m_iPatchLodCount;
	
	bool m_bThresHoldValue; // 문턱 값 사용
	void SetThresHold(bool bFlag) { m_bThresHoldValue = bFlag; };

public:
	virtual void	InitLevelOrder(BNode* pRootNode, int iMaxDepth);
	virtual void	SetLOD(DWORD dwWidth, int iNumDepth);
	virtual void	SetExpansionRatio(int iRatio) { m_fExpansionRatio = iRatio; }
	virtual DWORD	GetLodSubIndex(BNode* pNode);
	virtual DWORD	GetLodType(BNode* pNode);
	virtual float	GetExpansionRatio(D3DXVECTOR3 vCenter, float fRadius);
	virtual float	GetExpansionRatio(D3DXVECTOR3 vMax, D3DXVECTOR3 vMin);
	virtual float	GetExpansionRatio(D3DXVECTOR3 vCenter);
	virtual void	GetDrawPatchNode();
	virtual void	AddDrawPatchNode(BNode* pNode);
	virtual void	Update(ID3D11Device* pd3dDevice, BCamera* pCamera = NULL);

public:
	BMapLOD();
	~BMapLOD();
};

