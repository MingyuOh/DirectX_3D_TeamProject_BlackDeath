#include "BMapSetting.h"

void BMapSetting::Init(BCamera* MainCamera, ID3D11Device* m_pd3dDevice, ID3D11DeviceContext* pContext)
{
	m_pMainCamera = MainCamera;
	m_pImmediateContext = pContext;
}

void BMapSetting::Render()
{

}

void BMapSetting::SettingMap(BMapTileRender* vMapQuadTree)
{
	m_SelectedMap = vMapQuadTree;
}

//void BMapSetting::FirstPointFind(BMapTileRender* vMapQuadTree)
//{
//	m_InfoSelect.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj); //좌표계 일치 + 마우스 레이 받아오기
//	m_SelectedMap = vMapQuadTree;
//	TVector3 v0 = TVector3(vMapQuadTree->m_pRootNode->m_tBox.vMin.x, 0.0f, vMapQuadTree->m_pRootNode->m_tBox.vMax.z);
//	TVector3 v1 = TVector3(vMapQuadTree->m_pRootNode->m_tBox.vMax.x, 0.0f, vMapQuadTree->m_pRootNode->m_tBox.vMax.z);
//	TVector3 v2 = TVector3(vMapQuadTree->m_pRootNode->m_tBox.vMin.x, 0.0f, vMapQuadTree->m_pRootNode->m_tBox.vMin.z);
//	TVector3 v3 = TVector3(vMapQuadTree->m_pRootNode->m_tBox.vMax.x, 0.0f, vMapQuadTree->m_pRootNode->m_tBox.vMin.z);
//	m_fPickedY = vMapQuadTree->m_pRootNode->m_tBox.vMax.z;
//
//	if (m_InfoSelect.CheckPick(v0, v1, v2)) // 위쪽 면
//	{
//		if (m_InfoSelect.CrashPoint.x < 0)
//		{
//			m_iFirstPickedX = m_InfoSelect.CrashPoint.x - 1;
//		}
//		else
//		{
//			m_iFirstPickedX = m_InfoSelect.CrashPoint.x;
//		}
//		if (m_InfoSelect.CrashPoint.z > 0)
//		{
//			m_iFirstPickedZ = m_InfoSelect.CrashPoint.z + 1;
//		}
//		else
//		{
//			m_iFirstPickedZ = m_InfoSelect.CrashPoint.z;
//		}
//		BCreatePlane = false;
//		BLastCheck = true;
//		return;
//	}
//	else if (m_InfoSelect.CheckPick(v2, v1, v3)) // 아래쪽 면
//	{
//		if (m_InfoSelect.CrashPoint.x < 0)
//		{
//			m_iFirstPickedX = m_InfoSelect.CrashPoint.x - 1;
//		}
//		else
//		{
//			m_iFirstPickedX = m_InfoSelect.CrashPoint.x;
//		}
//		if (m_InfoSelect.CrashPoint.z > 0)
//		{
//			m_iFirstPickedZ = m_InfoSelect.CrashPoint.z + 1;
//		}
//		else
//		{
//			m_iFirstPickedZ = m_InfoSelect.CrashPoint.z;
//		}
//		BCreatePlane = false;
//		BLastCheck = true;
//		return;
//	}
//}
//
//void BMapSetting::LastPointFind(BMapTileRender* vMapQuadTree)
//{
//	m_InfoSelect.SetMatrix(NULL, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj); //좌표계 일치 + 마우스 레이 받아오기
//	TVector3 v0 = TVector3(vMapQuadTree->m_pRootNode->m_tBox.vMin.x, 0.0f, vMapQuadTree->m_pRootNode->m_tBox.vMax.z);
//	TVector3 v1 = TVector3(vMapQuadTree->m_pRootNode->m_tBox.vMax.x, 0.0f, vMapQuadTree->m_pRootNode->m_tBox.vMax.z);
//	TVector3 v2 = TVector3(vMapQuadTree->m_pRootNode->m_tBox.vMin.x, 0.0f, vMapQuadTree->m_pRootNode->m_tBox.vMin.z);
//	TVector3 v3 = TVector3(vMapQuadTree->m_pRootNode->m_tBox.vMax.x, 0.0f, vMapQuadTree->m_pRootNode->m_tBox.vMin.z);
//
//	if (m_InfoSelect.CheckPick(v0, v1, v2)) // 위쪽 면
//	{
//		if (m_InfoSelect.CrashPoint.x < 0)
//		{
//			m_iLastPickedX = m_InfoSelect.CrashPoint.x;
//		}
//		else
//		{
//			m_iLastPickedX = m_InfoSelect.CrashPoint.x + 1;
//		}
//		if (m_InfoSelect.CrashPoint.z > 0)
//		{
//			m_iLastPickedZ = m_InfoSelect.CrashPoint.z;
//		}
//		else
//		{
//			m_iLastPickedZ = m_InfoSelect.CrashPoint.z - 1;
//		}
//		BCreatePlane = true;
//		BLastCheck = false;
//		return;
//	}
//	else if (m_InfoSelect.CheckPick(v2, v1, v3)) // 아래쪽 면
//	{
//		if (m_InfoSelect.CrashPoint.x < 0)
//		{
//			m_iLastPickedX = m_InfoSelect.CrashPoint.x;
//		}
//		else
//		{
//			m_iLastPickedX = m_InfoSelect.CrashPoint.x + 1;
//		}
//		if (m_InfoSelect.CrashPoint.z < 0)
//		{
//			m_iLastPickedZ = m_InfoSelect.CrashPoint.z - 1;
//		}
//		else
//		{
//			m_iLastPickedZ = m_InfoSelect.CrashPoint.z;
//		}
//		BCreatePlane = true;
//		BLastCheck = false;
//		return;
//	}
//}

// MFC 에서 해당함수를 불러와서 정보를 세팅해야 한다. 최소값 최대값은 셀렉팅된 지점이 될 것이다.
void BMapSetting::MapInfoSetting()
{
	int MaxX = m_SelectedMap->m_pRootNode->m_tBox.vMax.x - m_SelectedMap->m_pRootNode->m_tBox.vMin.x;
	int MaxZ = m_SelectedMap->m_pRootNode->m_tBox.vMax.z - m_SelectedMap->m_pRootNode->m_tBox.vMin.z;

	m_MapAddress.resize(MaxZ);
	m_TileInfo.resize(MaxX*MaxZ);
	for (int ZCnt = 0; ZCnt < MaxZ; ZCnt++)
	{
		m_MapAddress[ZCnt].resize(MaxX);
	}
	SettingPositionInfo(m_SelectedMap->m_pRootNode, m_SelectedMap->m_pMap->m_iNumCellCols);
	BApply = false;
}

void BMapSetting::SettingPositionInfo(BNode* pNode, int iTotalRow)
{
	if (pNode->m_pChild[0] != NULL)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			SettingPositionInfo(pNode->m_pChild[iNode], iTotalRow);
		}
	}
	else
	{
		int iChangePoint = m_SelectedMap->m_pRootNode->m_tBox.fExtent[0];
		int iVerCnt = 0;
		int iRowStart = ((pNode->m_tBox.vMax.z - iChangePoint) * -1.0f) / m_SelectedMap->m_pMap->m_fCellDistance;
		int iColStart = (pNode->m_tBox.vMin.x + iChangePoint) / m_SelectedMap->m_pMap->m_fCellDistance;
		int iRowEnd = ((pNode->m_tBox.vMin.z - iChangePoint) * -1.0f) / m_SelectedMap->m_pMap->m_fCellDistance; // +1;
		int iColEnd = (pNode->m_tBox.vMax.x + iChangePoint) / m_SelectedMap->m_pMap->m_fCellDistance; // +1;

		for (int z = iRowStart; z < iRowEnd; z++)
		{
			for (int x = iColStart; x < iColEnd; x++)
			{
				int iIndex = iTotalRow * z + x;
				m_MapAddress[z][x] = iIndex;
				m_TileInfo[iIndex].TileEdge[0] = pNode->m_VertexList[(z - iRowStart) * (iColEnd - iColStart + 1) + (x - iColStart)].Position;
				m_TileInfo[iIndex].TileEdge[1] = pNode->m_VertexList[(z - iRowStart) * (iColEnd - iColStart + 1) + (x - iColStart) + 1].Position;
				m_TileInfo[iIndex].TileEdge[2] = pNode->m_VertexList[((z - iRowStart) + 1) * (iColEnd - iColStart + 1) + (x - iColStart) + 0].Position;
				m_TileInfo[iIndex].TileEdge[3] = pNode->m_VertexList[((z - iRowStart) + 1) * (iColEnd - iColStart + 1) + (x - iColStart) + 1].Position;
				m_TileInfo[iIndex].TileCenter = m_TileInfo[iIndex].TileEdge[0] + m_TileInfo[iIndex].TileEdge[1] + m_TileInfo[iIndex].TileEdge[2] + m_TileInfo[iIndex].TileEdge[3];
				m_TileInfo[iIndex].TileCenter /= 4.0f;
				
				TVector3 e0 = m_TileInfo[iIndex].TileEdge[1] - m_TileInfo[iIndex].TileEdge[0];
				TVector3 e1 = m_TileInfo[iIndex].TileEdge[2] - m_TileInfo[iIndex].TileEdge[0];

				TVector3 TempNormal;
				D3DXVec3Cross(&TempNormal, &e0, &e1);
				D3DXVec3Normalize(&TempNormal, &TempNormal);
				m_TileInfo[iIndex].TileNormal[0] = TempNormal;

				e0 = m_TileInfo[iIndex].TileEdge[1] - m_TileInfo[iIndex].TileEdge[2];
				e1 = m_TileInfo[iIndex].TileEdge[3] - m_TileInfo[iIndex].TileEdge[2];

				D3DXVec3Cross(&TempNormal, &e0, &e1);
				D3DXVec3Normalize(&TempNormal, &TempNormal);
				m_TileInfo[iIndex].TileNormal[1] = TempNormal;
			}									
		}										
	}											
	BApply = false;
}


// 맵의 알파텍스쳐에서 소리를 가져오는 함수
void BMapSetting::SettingSoundInfo()
{
	//m_SelectedMap->ObjPosTexture
	//m_TileInfo
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;

	if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)m_SelectedMap->StagingTexTure, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		int iRange = (m_SelectedMap->desc.Width / 10) - 1;
		// 현재 맵의 알파텍스쳐는 10배 크기로 되어있기 때문에 10배 줄여준다.
		for (int y = 0; y < iRange; y++)
		{
			BYTE* pDest = pDestBytes;
			for (int x = 0; x < iRange; x++)
			{
				// 땅 철 나무 모래 카펫 0 2 4 6 8 순으로 이루어져 있다.
				m_TileInfo[m_MapAddress[y][x]].TileSound = 0;
				if (*pDest > 127)
				{
					m_TileInfo[m_MapAddress[y][x]].TileSound = 6;
				}
				*pDest++;
				if (*pDest > 127)
				{
					m_TileInfo[m_MapAddress[y][x]].TileSound = 0;
				}
				*pDest++;
				if (*pDest > 127)
				{
					m_TileInfo[m_MapAddress[y][x]].TileSound = 2;
				}
				*pDest++;
				*pDest++;
				pDest += 36; // 9칸을 넘어가야 하는데 각 색상이 4개씩 있으므로 36
			}
			pDestBytes += (MappedFaceDest.RowPitch * 10);
		}
		g_pImmediateContext->Unmap(m_SelectedMap->StagingTexTure, 0);
	}
}

void BMapSetting::SettingObjInfo()
{
	//m_SelectedMap->ObjPosTexture
	//m_TileInfo
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	ItemPosition.reserve(48);
	if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)m_SelectedMap->StagingObjPosTexture, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		int iRange = m_SelectedMap->Objdesc.Width - 1;
		for (int y = 0; y < iRange; y++)
		{
			BYTE* pDest = pDestBytes;
			for (int x = 0; x < iRange; x++)
			{
				//
				if (*pDest > 0)
				{
					m_TileInfo[m_MapAddress[y][x]].TileMonsterRespawn = true;
					if (x >= 128 && y <= 128)
					{
						EnemyPosition1.push_back(m_TileInfo[m_MapAddress[y][x]].TileCenter);
					}
					if (x >= 128 && y >= 128)
					{
						EnemyPosition2.push_back(m_TileInfo[m_MapAddress[y][x]].TileCenter);
					}
					if (x <= 128 && y >= 128)
					{
						EnemyPosition3.push_back(m_TileInfo[m_MapAddress[y][x]].TileCenter);
					}
					if (x <= 128 && y <= 128)
					{
						EnemyPosition4.push_back(m_TileInfo[m_MapAddress[y][x]].TileCenter);
					}
				}
				else
					m_TileInfo[m_MapAddress[y][x]].TileMonsterRespawn = false;
				*pDest++;
				if (*pDest > 0)
					m_TileInfo[m_MapAddress[y][x]].TileMoveUnable = true;
				else
					m_TileInfo[m_MapAddress[y][x]].TileMoveUnable = false;
				*pDest++;
				if (*pDest > 0)
				{
					m_TileInfo[m_MapAddress[y][x]].TileiTemRespawn = true;
					ItemPosition.push_back(m_TileInfo[m_MapAddress[y][x]].TileCenter);
				}
				else
					m_TileInfo[m_MapAddress[y][x]].TileiTemRespawn = false;
				*pDest++;
				*pDest++;
			}
			pDestBytes += MappedFaceDest.RowPitch;
		}
		g_pImmediateContext->Unmap(m_SelectedMap->StagingObjPosTexture, 0);
	}
}


BMapSetting::BMapSetting()
{
	BLastCheck = false;
	BMapSettingMode = true;
	BApply = true;
}


BMapSetting::~BMapSetting()
{
}
