#include "BQuadTree.h"

bool BQuadTree::Build(float fWidth, float fHeight)
{
	m_iTotalMapObj = 0;
	m_fWidth = fWidth, m_fHeight = fHeight;
	m_pRootNode = CreateNode(NULL, -fWidth / 2, fWidth / 2, -fHeight / 2, fHeight / 2);

	if (BuildTree(m_pRootNode))
	{
		return true;
	}
	return false;
}

bool BQuadTree::BuildTree(BNode* pNode)
{
	if (SubDivide(pNode)) // 분할가능 여부
	{
		if (pNode->m_pChild[0] != NULL)
		{
			for (int iNode = 0; iNode < 4; iNode++)
			{
				if (m_bUsedIndexList)
				{
					DWORD dwIndex = pNode->m_pChild[iNode]->m_dwPositionIndex[1] * pow(2.0f, (float)pNode->m_pChild[iNode]->m_dwDepth) + pNode->m_pChild[iNode]->m_dwPositionIndex[0];
					m_LevelList[pNode->m_pChild[iNode]->m_dwDepth][dwIndex] = pNode->m_pChild[iNode];
				}
				BuildTree(pNode->m_pChild[iNode]);
			}
		}
	}
	return true;
}

bool BQuadTree::SubDivide(BNode* pNode)
{
	if (pNode == NULL)
	{
		return false;
	}
	
	/*최대 깊이한도 초과시 리프노드로 지정*/
	if (m_iMaxDepthLimit <= pNode->m_dwDepth)
	{
		pNode->m_isLeaf = TRUE;
		return false;
	}

	/*현재 노드의 실제 크기를 계산*/
	float fWidthSplit = (pNode->m_vCorner[1].x - pNode->m_vCorner[0].x) / 2;
	float fHeightSplit = (pNode->m_vCorner[0].z - pNode->m_vCorner[2].z) / 2;

	/*자식 노드가 지정된 분할크기보다 작다면 더이상 분할하지 않는다*/
	if (fWidthSplit < m_fMinDivideSize || fHeightSplit < m_fMinDivideSize)
	{
		pNode->m_isLeaf = TRUE;
		return false;
	}

	pNode->m_pChild[0] = CreateNode(pNode, pNode->m_vCorner[0].x, pNode->m_vCorner[0].x + fWidthSplit, pNode->m_vCorner[0].z - fHeightSplit, pNode->m_vCorner[0].z);
	pNode->m_pChild[1] = CreateNode(pNode, pNode->m_vCorner[0].x + fWidthSplit, pNode->m_vCorner[1].x, pNode->m_vCorner[0].z - fHeightSplit, pNode->m_vCorner[0].z);
	pNode->m_pChild[2] = CreateNode(pNode, pNode->m_vCorner[0].x + fWidthSplit, pNode->m_vCorner[1].x, pNode->m_vCorner[2].z, pNode->m_vCorner[1].z - fHeightSplit);
	pNode->m_pChild[3] = CreateNode(pNode, pNode->m_vCorner[0].x, pNode->m_vCorner[0].x + fWidthSplit, pNode->m_vCorner[2].z, pNode->m_vCorner[1].z - fHeightSplit);
	pNode->m_pChild[0]->m_pParent = pNode;
	pNode->m_pChild[1]->m_pParent = pNode;
	pNode->m_pChild[2]->m_pParent = pNode;
	pNode->m_pChild[3]->m_pParent = pNode;
	return true;
}


BNode* BQuadTree::CreateNode(BNode* pParentNode, float fLeft, float fRight, float fBottom, float fTop)
{	
	BNode* pNode = 0;
	S_NEW(pNode, BNode);
	assert(pNode);

	pNode->m_tBox.vMin = TVector3(fLeft, 0.0f, fBottom);
	pNode->m_tBox.vMax = TVector3(fRight, 0.0f, fTop);
	pNode->m_tBox.vCenter = pNode->m_tBox.vMax + pNode->m_tBox.vMin;
	pNode->m_tBox.vCenter /= 2.0f;
	pNode->m_tBox.fExtent[0] = pNode->m_tBox.vMax.x - pNode->m_tBox.vCenter.x;
	pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
	pNode->m_tBox.fExtent[2] = pNode->m_tBox.vMax.z - pNode->m_tBox.vCenter.z;

	// 0  1
 	// 3  2 
 
	pNode->m_vCorner[0] = TVector3(fLeft, 0, fTop);
	pNode->m_vCorner[1] = TVector3(fRight, 0, fTop);
	pNode->m_vCorner[2] = TVector3(fRight, 0, fBottom);
	pNode->m_vCorner[3] = TVector3(fLeft, 0, fBottom);

	if (pParentNode != NULL)
	{
		pNode->m_dwDepth = pParentNode->m_dwDepth + 1;
		if (m_iMaxDepth < pNode->m_dwDepth)
		{
			m_iMaxDepth = pNode->m_dwDepth;
		}
	}
	return pNode;
}

void BQuadTree::ClearObject(BNode* pNode)
{
	if (pNode->m_pChild[0] != NULL)
	{
		for (DWORD dwChild = 0; dwChild < 4; dwChild++)
		{
			ClearObject(pNode->m_pChild[dwChild]);
		}
	}
	else
	{
		pNode->m_ObjectList.clear();
		m_iTotalMapObj = 0;
	}
}


int BQuadTree::AddObject()
{
	for (int iItem = 0; iItem < m_ItemPosition.size(); iItem++)
	{
		if (CheckPosition(m_pRootNode, m_ItemPosition[iItem]))
		{
			BNode* pNode = FindNode(m_pRootNode, m_ItemPosition[iItem]);
			if (pNode)
			{
				pNode->m_ObjectList.push_back(iItem);
			}
		}
	}
	return 0;
}

// 나무, 집, 돌등을 추가
void BQuadTree::AddMapObject(BNode* pNode, vector<InstancingInfo> MapInfo)
{
	if (pNode->m_pChild[0] != NULL)
	{
		for (DWORD dwChild = 0; dwChild < 4; dwChild++)
		{
			AddMapObject(pNode->m_pChild[dwChild], MapInfo);
		}
	}
	else
	{
		int iNodeObjCount = 0;
		for (int iMapObj = 0; iMapObj < MapInfo.size(); iMapObj++)
		{
			if (CheckPosition(pNode, MapInfo[iMapObj].Translate))
			{
				iNodeObjCount++;
				pNode->m_vMapObjInfo.resize(iNodeObjCount);
				pNode->m_vMapObjInfo[iNodeObjCount - 1].m_iObjType = MapInfo[iMapObj].ObjName;
				pNode->m_vMapObjInfo[iNodeObjCount - 1].m_iObjNum = MapInfo[iMapObj].iInstanceNumber;
				m_iTotalMapObj++;
			}
		}
	}
}

BNode* BQuadTree::FindNode(BNode* pNode, TVector3 pObj)
{
	assert(pNode);
	do
	{
		if (pNode->m_pChild[0] != NULL)
		{
			for (DWORD dwChild = 0; dwChild < 4; dwChild++)
			{
				if (pNode->m_pChild[dwChild] && CheckPosition(pNode->m_pChild[dwChild], pObj))
				{
					m_QuadTreeQueue.push(pNode->m_pChild[dwChild]);
				}
			}
		}
		if (m_QuadTreeQueue.empty())break;
		pNode = m_QuadTreeQueue.front();
		m_QuadTreeQueue.pop();
	} while (pNode);
	return pNode;
}

int BQuadTree::CheckRect(BNode* pNode, BObject* pObj)
{
	if (pNode->m_tBox.vMin.x <= pObj->m_tBox.vMin.x && pNode->m_tBox.vMax.x >= pObj->m_tBox.vMax.x)
	{
		if (pNode->m_tBox.vMin.z <= pObj->m_tBox.vMin.z && pNode->m_tBox.vMax.z >= pObj->m_tBox.vMax.z)
		{
			return 1; // true;
		}
	}
	return 0; // false;
}

int BQuadTree::CheckPosition(BNode* pNode, TVector3 pObj)
{
	if (pNode->m_tBox.vMin.x <= pObj.x && pNode->m_tBox.vMax.x >= pObj.x)
	{
		if (pNode->m_tBox.vMin.z <= pObj.z && pNode->m_tBox.vMax.z >= pObj.z)
		{
			return 1; // true;
		}
	}
	return 0; // false;
}


void BQuadTree::DrawFindNode(BNode* pNode, BCamera* pCamera)
{
	assert(pNode);
	m_pCamera = pCamera;

	T_POSITION t_Pos = pCamera->CheckPositionOBBInPlane(&pNode->m_tBox);
	GetLodSubIndex(pNode);
	if (pNode->m_dwDepth < m_iRenderDepth) return;

	if (pNode->m_isLeaf && t_Pos != P_BACK)
	{
		m_DrawNodeList.push_back(pNode);
		for (int iMapObj = 0; iMapObj < pNode->m_vMapObjInfo.size(); iMapObj++)
		{
			m_DrawMapObjList.push_back(pNode->m_vMapObjInfo[iMapObj]);
		}
		VisibleObject(pNode, pCamera);
		return;
	}
	if (t_Pos == P_FRONT)
	{
		m_DrawNodeList.push_back(pNode);
		for (int iMapObj = 0; iMapObj < pNode->m_vMapObjInfo.size(); iMapObj++)
		{
			m_DrawMapObjList.push_back(pNode->m_vMapObjInfo[iMapObj]);
		}
		VisibleNode(pNode, pCamera);
		return;
	}
	if (t_Pos == P_SPANNING)
	{
		VisibleObject(pNode, pCamera);
	}
	if (pNode->m_pChild[0] != NULL)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			DrawFindNode(pNode->m_pChild[iNode], pCamera);
		}
	}
}

void BQuadTree :: VisibleNode(BNode* pNode, BCamera* pCamera)
{
	if (pNode->m_dwDepth < m_iRenderDepth) return;
	if (pCamera->CheckOBB(&pNode->m_tBox))
	{
		VisibleObject(pNode, pCamera);
		if (pNode->m_pChild[0] != NULL)
		{
			for (int iNode = 0; iNode < 4; iNode++)
			{
				VisibleNode(pNode->m_pChild[iNode], pCamera);
			}
		}
	}
}

void BQuadTree::VisibleObject(BNode* pNode, BCamera* pCamera)
{
	for (int iObj = 0; iObj < pNode->m_ObjectList.size(); iObj++)
	{
		if (pCamera->CheckPoint(&m_ItemPosition[pNode->m_ObjectList[iObj]]))
		{
			m_DrawObjList.push_back(pNode->m_ObjectList[iObj]);
		}
	}
}

void BQuadTree::SetNeighborNode(BNode* pNode)
{
	if (pNode == NULL) return;
	FindNeighborNode(pNode);
	if (pNode->m_pChild[0] != NULL)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			SetNeighborNode(pNode->m_pChild[iNode]);
		}
	}
}

void BQuadTree::FindNeighborNode(BNode* pNode)
{
	pNode->m_NeighborList.resize(8);

	DWORD dwNumPatchCount = pow(2.0f, (float)pNode->m_dwDepth);

	DWORD dwNeighborCol, dwNeighborRow;
	if (pNode->m_dwPositionIndex[1] > 0) // 상
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0];
		dwNeighborRow = (pNode->m_dwPositionIndex[1] - 1) * dwNumPatchCount;
		pNode->m_NeighborList[0] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[1] < dwNumPatchCount - 1) // 하
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0];
		dwNeighborRow = (pNode->m_dwPositionIndex[1] + 1) * dwNumPatchCount;
		pNode->m_NeighborList[1] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[0] > 0) // 좌
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0] - 1;
		dwNeighborRow = pNode->m_dwPositionIndex[1] * dwNumPatchCount;
		pNode->m_NeighborList[2] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[0] < dwNumPatchCount - 1) // 우
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0] + 1;
		dwNeighborRow = pNode->m_dwPositionIndex[1] * dwNumPatchCount;
		pNode->m_NeighborList[3] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[1] > 0 && pNode->m_dwPositionIndex[0] > 0) //좌상
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0] - 1;
		dwNeighborRow = (pNode->m_dwPositionIndex[1] - 1) * dwNumPatchCount;
		pNode->m_NeighborList[4] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[1] > 0 && pNode->m_dwPositionIndex[0] < dwNumPatchCount - 1) //우상
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0] + 1;
		dwNeighborRow = (pNode->m_dwPositionIndex[1] - 1) * dwNumPatchCount;
		pNode->m_NeighborList[5] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[1] < dwNumPatchCount - 1 && pNode->m_dwPositionIndex[0] > 0) //좌하
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0] - 1;
		dwNeighborRow = (pNode->m_dwPositionIndex[1] + 1) * dwNumPatchCount;
		pNode->m_NeighborList[6] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[1] < dwNumPatchCount - 1 && pNode->m_dwPositionIndex[0] < dwNumPatchCount - 1) //우하
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0] + 1;
		dwNeighborRow = (pNode->m_dwPositionIndex[1] + 1) * dwNumPatchCount;
		pNode->m_NeighborList[7] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
}

DWORD BQuadTree::CheckSize(DWORD dwSize)
{
	float fCount = 0;
	DWORD dwChecker = dwSize / 2;
	if (dwChecker <= 0) return 0;

	while (dwChecker != 0)
	{
		dwChecker >>= 1;
		fCount += 1.0f;
	}

	float fMinimum = pow(2.0f, fCount - 1.0f);
	float fMaximum = pow(2.0f, fCount);
	float fMin = fabs(fMinimum - fabs(((float)dwSize - fMinimum)));
	float fMax = fabs(fMaximum - fabs(((float)dwSize - fMaximum)));

	//비교값이 같다면 작은 분할크기를 반환
	if (fMin <= fMax)
	{
		return fMinimum;
	}
	return fMaximum;
}

bool BQuadTree::Frame(BCamera* pCamera)
{
	m_DrawObjList.clear();
	m_DrawNodeList.clear();
	m_DrawMapObjList.clear();
	DrawFindNode(m_pRootNode, pCamera);
	return true;
}
bool BQuadTree::Release()
{
	S_DEL(m_pRootNode);
	return true;
}

BQuadTree::BQuadTree()
{
	m_bUsedIndexList = false;
	m_iMaxDepth = 0;
	m_iMaxDepthLimit = 4; // 쿼드트리의 깊이를 나타낸다.
	m_fMinDivideSize = 2;
	m_iRenderDepth = 0;
}


BQuadTree::~BQuadTree()
{
	Release();
}
