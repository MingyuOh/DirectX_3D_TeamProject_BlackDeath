#include "BMapLOD.h"

void BMapLOD::InitLevelOrder(BNode* pRootNode, int iMaxDepth)
{
	m_LevelList.resize(iMaxDepth + 1);
	for (int iLevel = 1; iLevel < iMaxDepth + 1; iLevel++)
	{
		m_LevelList[iLevel].resize(pow(4.0f, iLevel));
	}
	m_LevelList[0].push_back(pRootNode);
}
//m_LevelList 깊이에 따른 노드 리스트

void	BMapLOD::SetLOD(DWORD dwWidth, int iNumDepth)
{
	m_iNumCell = (dwWidth - 1) / pow(2.0f, (float)iNumDepth); // 트리의 레벨에 따른 총 셀의 갯수
	m_iPatchLodCount = (log((float)m_iNumCell) / log(2.0f)); // LOD레벨 갯수
	if (m_iPatchLodCount > 2)
	{
		m_iPatchLodCount = 2; // 0 , 1 , 2 단계의 3개의 LOD생성
	}
}

DWORD	BMapLOD::GetLodType(BNode* pNode)
{
	if (pNode->m_NeighborList.size() <= 0) return 0;
	DWORD dwType = 0;
	if (pNode->m_NeighborList[0] && pNode->m_NeighborList[0]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 1;
	if (pNode->m_NeighborList[1] && pNode->m_NeighborList[1]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 4;
	if (pNode->m_NeighborList[2] && pNode->m_NeighborList[2]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 8;
	if (pNode->m_NeighborList[3] && pNode->m_NeighborList[3]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 2;

	pNode->m_dwLodType = dwType; // == 사용해야 하는 인덱스 버퍼의 번호
	return dwType;
 }
 
 float	BMapLOD::GetExpansionRatio(TVector3 vCenter, float fRadius)
 {
	 float fRatio = 0.0f;
	 
	 TVector3 vCenterViewPos, vSideViewPos;
	 vSideViewPos = vCenter + (m_pCamera->m_vSide * fRadius);
	 D3DXVec3TransformCoord(&vCenterViewPos, &vCenter, &m_pCamera->m_matView);
	 D3DXVec3TransformCoord(&vSideViewPos, &vSideViewPos, &m_pCamera->m_matView);
	 
	 TVector3 vCenterProjPos, vSideProjPos;
	 D3DXVec3TransformCoord(&vCenterProjPos, &vCenterViewPos, &m_pCamera->m_matProj);
	 D3DXVec3TransformCoord(&vSideProjPos, &vSideViewPos, &m_pCamera->m_matProj);
	 
	 D3DXVec3Normalize(&vCenterProjPos, &vCenterProjPos);
	 TVector2 vA = TVector2(vCenterProjPos.x * 0.5f + 0.5f, vCenterProjPos.y * 0.5f + 0.5f);
	 TVector2 vB = TVector2(vSideProjPos.x * 0.5f + 0.5f, vSideProjPos.y * 0.5f + 0.5f);

	 float fDistance = D3DXVec3Length(&(m_pCamera->m_vPos - vCenter));
	 if (fDistance > 2000.0f)
	 {
		 return 1.0f;
	 }
	 fRatio = 1.0f - fDistance / 2000.0f;
	 return fRatio;
 }
 
 float	BMapLOD::GetExpansionRatio(TVector3 vMax, TVector3 vMin)
 {
	 float fRatio = 0.0f;

	 TVector3 vCenterViewPos, vSideViewPos;
	 D3DXVec3TransformCoord(&vCenterViewPos, &vMax, &m_pCamera->m_matView);
	 D3DXVec3TransformCoord(&vSideViewPos, &vMin, &m_pCamera->m_matView);

	 TVector3 vCenterProjPos, vSideProjPos;
	 D3DXVec3TransformCoord(&vCenterProjPos, &vCenterViewPos, &m_pCamera->m_matProj);
	 D3DXVec3TransformCoord(&vSideProjPos, &vSideViewPos, &m_pCamera->m_matProj);

	 TVector2 vA = TVector2(vCenterProjPos.x * 0.5f + 0.5f, vCenterProjPos.y * 0.5f + 0.5f);
	 TVector2 vB = TVector2(vSideProjPos.x * 0.5f + 0.5f, vSideProjPos.y * 0.5f + 0.5f);

	 fRatio = D3DXVec3Length(&(vCenterProjPos - vSideProjPos));
	 return fRatio;
 }
 
 float	BMapLOD::GetExpansionRatio(TVector3 vCenter)
 {
	 TVector3 DistanceCamera = m_pCamera->m_vPos;
	 TVector3 DistancevCenter = vCenter;
	 DistanceCamera.y = 0.0f;
	 DistancevCenter.y = 0.0f;

	 float fDistance = D3DXVec3Length(&(DistanceCamera - DistancevCenter));
	 if (fDistance > 1000.0f)
	 {
		 return 1.0f;
	 }
	 float fRatio = fDistance / 1000.0f;
	 return fRatio;
 }



void BMapLOD::GetDrawPatchNode()
{
	for (int iNode = 0; iNode < m_DrawNodeList.size(); iNode++)
	{
		AddDrawPatchNode(m_DrawNodeList[iNode]);
	}
}

void BMapLOD::AddDrawPatchNode(BNode* pNode)
{
	if (pNode->m_isLeaf)
	{
		m_DrawPatchNodeList.push_back(pNode);
		GetLodSubIndex(pNode);
		return;
	}
	for (int iNode = 0; iNode < 4; iNode++)
	{
		AddDrawPatchNode(pNode->m_pChild[iNode]);
	}
}

DWORD BMapLOD::GetLodSubIndex(BNode* pNode)
{
	int iNumPatchIndex = m_iNumCell;
	float fRatio = GetExpansionRatio(pNode->m_tBox.vCenter);
	DWORD dwCurrentRatio = fRatio * m_iPatchLodCount;
	DWORD dwMaxRatio = fRatio * m_iPatchLodCount + 0.5f;
	DWORD dwMinRatio = fRatio * m_iPatchLodCount - 0.5f;

	// 문턱효과
	if (m_bThresHoldValue)
	{
		if (pNode->m_dwLodLevel < dwCurrentRatio)
		{
			if (pNode->m_dwLodLevel < dwMinRatio)
			{
				pNode->m_dwLodLevel = dwCurrentRatio;
			}
		}
		else if (pNode->m_dwLodLevel > dwCurrentRatio)
		{
			if (pNode->m_dwLodLevel > dwMaxRatio)
			{
				pNode->m_dwLodLevel = dwCurrentRatio;
			}
		}
	}
	else
	{
		pNode->m_dwLodLevel = dwCurrentRatio;
	}
	if (m_iPatchLodCount < pNode->m_dwLodLevel)
	{
		pNode->m_dwLodLevel -= 1;
	}
	return pNode->m_dwLodLevel;
}

void BMapLOD::Update(ID3D11Device* pd3dDevice, BCamera* pCamera)
{
	m_pd3dDevice = pd3dDevice;
	m_pCamera = pCamera;
}

BMapLOD::BMapLOD()
{
	m_fExpansionRatio = 0.1f;
	m_iNumCell = 1;
	m_iPatchLodCount = 0;
	m_pCamera = NULL;
	m_bThresHoldValue = true;
}


BMapLOD::~BMapLOD()
{
}
