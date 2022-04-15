#include "BMapTileRender.h"

BStaticData::BStaticData()
{
}

BStaticData::~BStaticData()
{
	for (int iList = 0; iList < IndexBufferList.size(); iList++)
	{
		IndexList[iList].clear();
	}
	IndexBufferList.clear();
	IndexList.clear();
}


bool BMapTileRender::Frame(BCamera* pCamera)
{
	PreFrame();
	m_DrawPatchNodeList.clear();
	if (!BQuadTree::Frame(pCamera))return false;

	GetDrawPatchNode();

	// 본 코드에서 적용하는 인덱스 방식은 실시간 인덱스리스트를 갱신하는 방법으로 미리 생성해둔 인덱스버퍼를 사용하지 않습니다.
	// 따라서 정적으로 생성해둔 십자형 인덱스리스트를 통해 한개의 큰 버퍼를 만들고 랜더링을 하게 됩니다.
	m_iNumFace = 0;
	/*이웃노드와의 LOD레벨차이가 2이상일때 stop*/
	for (int iNode = 0; iNode < m_DrawPatchNodeList.size(); iNode++)
	{
		for (int iNeighbor = 0; iNeighbor < m_DrawPatchNodeList[iNode]->m_NeighborList.size(); iNeighbor++)
		{
			if (m_DrawPatchNodeList[iNode]->m_NeighborList[iNeighbor])
			{
				DWORD dwValue = abs((float)(m_DrawPatchNodeList[iNode]->m_NeighborList[iNeighbor]->m_dwLodLevel - m_DrawPatchNodeList[iNode]->m_dwLodLevel));
				if (dwValue > 1)
				{
      					int LevelSecondDifferent = 0;
				}
			}
		}
		GetLodType(m_DrawPatchNodeList[iNode]);
		m_iNumFace += UpdateStaticIndexList(m_DrawPatchNodeList[iNode], m_iNumFace * 3, m_DrawPatchNodeList[iNode]->m_dwLodLevel);
	}
	
	return true;
}

bool BMapTileRender::PreFrame()
{
	return true;
}

int BMapTileRender::UpdateStaticIndexList(BNode* pNode, DWORD dwCurrentIndex, DWORD dwLod)
{
	if (!m_IndexList.size()) return false;
	int iNumFaces = 0;
	DWORD dwTL = pNode->m_CornerIndex[0];

	DWORD dwIndexCnt = m_pdwLodIndexArray[dwLod]->IndexList[pNode->m_dwLodType].size();
	for (DWORD dwIndex = 0; dwIndex < dwIndexCnt; dwIndex += 3)
	{
		DWORD dw0 = GetIndex(m_pdwLodIndexArray[dwLod]->IndexList[pNode->m_dwLodType][dwIndex + 0], dwTL);
		DWORD dw1 = GetIndex(m_pdwLodIndexArray[dwLod]->IndexList[pNode->m_dwLodType][dwIndex + 1], dwTL);
		DWORD dw2 = GetIndex(m_pdwLodIndexArray[dwLod]->IndexList[pNode->m_dwLodType][dwIndex + 2], dwTL);

		m_IndexList[dwCurrentIndex + 0] = dw0;
		m_IndexList[dwCurrentIndex + 1] = dw1;
		m_IndexList[dwCurrentIndex + 2] = dw2;
		iNumFaces += 1;
		dwCurrentIndex += 3;
	}
	return iNumFaces;
}

int BMapTileRender::GetIndex(DWORD dwIndex, DWORD dwTL)
{
	DWORD dwReturn = 0;
	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwStartCol = dwTL % m_dwWidth;

	DWORD dwRow = dwIndex / (int)(pow(2.0f, (float)m_iPatchLodCount) + 1);
	DWORD dwCol = dwIndex % (int)(pow(2.0f, (float)m_iPatchLodCount) + 1);

	dwReturn = (dwStartRow + dwRow) * m_dwWidth + dwStartCol + dwCol;
	return dwReturn;
}

bool BMapTileRender::Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pImmediateContext)
{
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_pMap = (BNMap*)pMap;
	m_iNumFace = (dwWidth - 1) *(dwHeight - 1) * 2;
	m_bUsedIndexList = true;
	m_pImmediateContext = pImmediateContext;
	
	m_pRootNode = CreateNode(NULL, 0, dwWidth - 1, dwWidth * (dwHeight - 1), dwWidth * dwHeight - 1);

	InitLevelOrder(m_pRootNode, m_iMaxDepthLimit); // LOD용 함수 LOD 레벨 resize
	m_pIndexBuffer.Attach(CreateIndexBuffer(m_pd3dDevice, NULL, m_iNumFace * 3, sizeof(DWORD)));

	if (BuildTree(m_pRootNode))
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			SetNeighborNode(m_pRootNode->m_pChild[iNode]);
		}
		SetLOD(m_dwWidth, m_iMaxDepth);

		// LOD용 인덱스 제작 함수
		ComputeStaticLodIndex(m_iNumCell);

		// 공용 인덱스 버퍼 생성
		//int iNumTileRender = pow(4.0f, m_iMaxDepth);
		//m_iNumCell = (dwWidth - 1) / pow(2.0f, (float)m_iMaxDepth);
		//m_IndexList.resize(m_iNumCell * m_iNumCell * 2 * 3); // 2면 * 버택스 3개 

		//int iIndex = 0;
		//for (DWORD dwRow = 0; dwRow < m_iNumCell; dwRow++)
		//{
		//	for (DWORD dwCol = 0; dwCol < m_iNumCell; dwCol++)
		//	{
		//		// 0  1      4
		//		// 2      3  5
		//		DWORD dwNextRow = dwRow + 1;
		//		DWORD dwNextCol = dwCol + 1;
		//		m_IndexList[iIndex++] = (dwRow * (m_iNumCell + 1) + dwCol);
		//		m_IndexList[iIndex++] = (dwRow * (m_iNumCell + 1) + dwNextCol);
		//		m_IndexList[iIndex++] = (dwNextRow * (m_iNumCell + 1) + dwCol);
		//		m_IndexList[iIndex++] = (dwNextRow * (m_iNumCell + 1) + dwCol);
		//		m_IndexList[iIndex++] = (dwRow * (m_iNumCell + 1) + dwNextCol);
		//		m_IndexList[iIndex++] = (dwNextRow * (m_iNumCell + 1) + dwNextCol);
		//	}
		//}
		//// 정점 인덱스 버퍼 생성
		//m_pTileIndexBuffer.Attach(CreateIndexBuffer(m_pd3dDevice, &m_IndexList.at(0), m_IndexList.size(), sizeof(DWORD)));
		return true;
	}
	return false;
}

bool BMapTileRender::ComputeStaticLodIndex(int iMaxCells)
{
	m_pdwLodIndexArray.reserve(m_iPatchLodCount + 1); // m_iPatchLodCount = 리프의 단계이므로 0번까지 들어가기 위해 +1을 해준다.

	// case : Level 0
	BStaticData* pStaticData = NULL;
	S_NEW(pStaticData, BStaticData);
	pStaticData->dwLevel = 0;
	pStaticData->IndexList.resize(1);

	for (DWORD dwRow = 0; dwRow < iMaxCells; dwRow++)
	{
		for (DWORD dwCol = 0; dwCol < iMaxCells; dwCol++)
		{
			// 0 1   4
			// 2   3 5
			DWORD dwNextRow = dwRow + 1;
			DWORD dwNextCol = dwCol + 1;
			pStaticData->IndexList[0].push_back(dwRow * (iMaxCells + 1) + dwCol);
			pStaticData->IndexList[0].push_back(dwRow * (iMaxCells + 1) + dwNextCol);
			pStaticData->IndexList[0].push_back(dwNextRow * (iMaxCells + 1) + dwCol);
			pStaticData->IndexList[0].push_back(dwNextRow * (iMaxCells + 1) + dwCol);
			pStaticData->IndexList[0].push_back(dwRow * (iMaxCells + 1) + dwNextCol);
			pStaticData->IndexList[0].push_back(dwNextRow * (iMaxCells + 1) + dwNextCol);
		}
	}
	m_pdwLodIndexArray.push_back(pStaticData);
	CreateLODIndexBuffer(pStaticData);

	//레벨 1부터 ~ 마지막 레벨까지
	for (DWORD dwLevel = 0; dwLevel < m_iPatchLodCount; dwLevel++)
	{
		//최소 패치의 크기가 (0, 1 ,2) 정점 3개이기 때문에 level = 0일 경우 오프셋 = 2 여야 한다.
		DWORD dwOffset = pow(2.0f, float(dwLevel)+1); // dwOffset = 한 열의 셀 갯수
		DWORD dwNumCell = (iMaxCells / dwOffset);

		BStaticData* pStaticData = NULL;
		S_NEW(pStaticData, BStaticData);
		pStaticData->dwLevel = dwLevel+1;
		pStaticData->IndexList.resize(16); // 16 = 8 4 2 1 의 합 

		/*십자형*/
		for (int iData = 0; iData < 16; iData++)
		{
			DWORD dwIndexCounter = GetIndexCounter(iData, dwNumCell);
			pStaticData->IndexList[iData].resize(dwIndexCounter);
			int iIndex = 0;

			for (DWORD dwRow = 0; dwRow < iMaxCells; dwRow += dwOffset)
			{
				for (DWORD dwCol = 0; dwCol < iMaxCells; dwCol += dwOffset)
				{
					DWORD dwTL = dwRow * (iMaxCells + 1) + dwCol;
					DWORD dwTR = dwTL + dwOffset;
					DWORD dwBL = dwOffset * (iMaxCells + 1) + dwTL;
					DWORD dwBR = dwBL + dwOffset;
					DWORD dwCP = (dwTL + dwBR) / 2;
					// 데이터가 없거나, 왼쪽 끝이거나 , 오른쪽 끝이거나, 맨 위거나, 맨 아래라면
					// 1 = 위 2 = 오른쪽 4 = 아래 8 = 왼쪽
					if (iData != 0 && (dwRow == 0 || dwRow == ((dwNumCell - 1)*dwOffset) || dwCol == 0 || dwCol == ((dwNumCell - 1)*dwOffset)))
					{
						if ((iData & 1) && dwRow == 0)
						{
							DWORD dwUpperCenter = (dwTL + dwTR) / 2;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwTL;
							pStaticData->IndexList[iData][iIndex++] = dwUpperCenter;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwUpperCenter;
							pStaticData->IndexList[iData][iIndex++] = dwTR;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwTL;
							pStaticData->IndexList[iData][iIndex++] = dwTR;
						}
						if ((iData & 2) && (dwCol == (dwNumCell - 1)*dwOffset))
						{
							DWORD dwRightCenter = (dwTR + dwBR) / 2;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwTR;
							pStaticData->IndexList[iData][iIndex++] = dwRightCenter;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwRightCenter;
							pStaticData->IndexList[iData][iIndex++] = dwBR;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwTR;
							pStaticData->IndexList[iData][iIndex++] = dwBR;
						}
						if ((iData & 4) && (dwRow == (dwNumCell - 1)*dwOffset))
						{
							DWORD dwLowerCenter = (dwBR + dwBL) / 2;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwBR;
							pStaticData->IndexList[iData][iIndex++] = dwLowerCenter;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwLowerCenter;
							pStaticData->IndexList[iData][iIndex++] = dwBL;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwBR;
							pStaticData->IndexList[iData][iIndex++] = dwBL;
						}
						if ((iData & 8) && (dwCol == 0))
						{
							DWORD dwLeftCenter = (dwTL + dwBL) / 2;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwBL;
							pStaticData->IndexList[iData][iIndex++] = dwLeftCenter;
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwLeftCenter;
							pStaticData->IndexList[iData][iIndex++] = dwTL;
						}
						else
						{
							pStaticData->IndexList[iData][iIndex++] = dwCP;
							pStaticData->IndexList[iData][iIndex++] = dwBL;
							pStaticData->IndexList[iData][iIndex++] = dwTL;
						}
					}
					else
					{
						pStaticData->IndexList[iData][iIndex++] = dwCP;
						pStaticData->IndexList[iData][iIndex++] = dwTL;
						pStaticData->IndexList[iData][iIndex++] = dwTR;

						pStaticData->IndexList[iData][iIndex++] = dwCP;
						pStaticData->IndexList[iData][iIndex++] = dwTR;
						pStaticData->IndexList[iData][iIndex++] = dwBR;

						pStaticData->IndexList[iData][iIndex++] = dwCP;
						pStaticData->IndexList[iData][iIndex++] = dwBR;
						pStaticData->IndexList[iData][iIndex++] = dwBL;

						pStaticData->IndexList[iData][iIndex++] = dwCP;
						pStaticData->IndexList[iData][iIndex++] = dwBL;
						pStaticData->IndexList[iData][iIndex++] = dwTL;
					}
				}
			}
		}
		CreateLODIndexBuffer(pStaticData);
		m_pdwLodIndexArray.push_back(pStaticData);
	}
	return true;
}


DWORD BMapTileRender::GetIndexCounter(DWORD dwData, DWORD dwNumCell)
{
	// 총 개수 = (셀 가로 개수 * 셀 세로 개수 * 4) + (셀 가로 개수 및 셀 세로 개수 * dwLevelCount) * 3;
	// dwLevelCount는 8 4 2 1 방향으로 각각 추가 될 수 있는 페이스
	DWORD dwNumFaces = (dwNumCell * dwNumCell * 4);
	
	DWORD dwLevelCount = 0;
	if (dwData & 8) dwLevelCount++;
	if (dwData & 4) dwLevelCount++;
	if (dwData & 2) dwLevelCount++;
	if (dwData & 1) dwLevelCount++;

	dwNumFaces = (dwNumFaces + (dwNumCell * dwLevelCount));
	return dwNumFaces * 3;
}

ID3D11Buffer* BMapTileRender::CreateIndexBuffer(ID3D11Device* pd3dDevice, void *indices, UINT iNumIndex, UINT iSize)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pBuffer = nullptr;
	// 인덱스 버퍼 생성
	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = iNumIndex * iSize;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // 인덱스 버퍼로 사용하겠다는 선언
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ibInitData;
	ZeroMemory(&ibInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibInitData.pSysMem = indices;//(void*)&m_IndexList.at(0);
	//hr = pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &pBuffer);
	if (indices != NULL)
	{
		hr = pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &pBuffer);
	}
	else
	{
		hr = pd3dDevice->CreateBuffer(&ibDesc, NULL, &pBuffer);
	}
	if (hr != S_OK)
	{
		return nullptr;
	}
	return pBuffer;
}

HRESULT BMapTileRender::CreateLODIndexBuffer(BStaticData* pData)
{
	HRESULT hr = S_OK;
	pData->IndexBufferList.resize(pData->IndexList.size());
	for (int iData = 0; iData < pData->IndexList.size(); iData++)
	{
		if (pData->IndexList[iData].size() <= 0)
			break;
		pData->IndexBufferList[iData].Attach(CreateIndexBuffer(m_pd3dDevice, &pData->IndexList[iData].at(0), pData->IndexList[iData].size(), sizeof(DWORD)));
	}
	return hr;
}

/*여기부터 버택스 버퍼가 잘못 만들어지고 있다.*/
// 변경된 y값을 가지고 제작
ID3D11Buffer* BMapTileRender::CreateVertexBuffer(ID3D11Device* pd3dDevice, void *indices, UINT iNumIndex, UINT iSize)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pBuffer = nullptr;

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						// [메모리 할당 위치] GPU 메모리에 할당해라
	bd.ByteWidth = iNumIndex * iSize;	// [메모리 할당 크기] 점 X개를 할당 할 것이기 때문에 *X ( 수가 늘어나면 증가 할 것이다 )
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// [메모리 사용 용도] 버텍스 버퍼용으로 사용하겠다.
	bd.CPUAccessFlags = 0;								// CPU로 부터는 액세스 하지 않겠다.
	bd.MiscFlags = 0;									// 추가 설정 없음

	D3D11_SUBRESOURCE_DATA InitData;			// 소스 리소스 초기화에 필요한 데이터 지정
	InitData.pSysMem = indices;//(void*)&m_VertexList.at(0);				// 해당 버텍스를 소스로 사용하겠다.
	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer);
	if (hr != S_OK)
	{
		return nullptr;
	}
	return pBuffer;
}

BNode* BMapTileRender::CreateNode(BNode* pParentNode, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{
	BNode* pNode = 0;
	S_NEW(pNode, BNode);

	pNode->m_isLeaf = FALSE;
	
	// vCorner, m_Child 는 배열이라 reserve 필요없음
	pNode->m_CornerIndex.reserve(4);
	
	pNode->m_CornerIndex.push_back(dwTL);
	pNode->m_CornerIndex.push_back(dwTR);
	pNode->m_CornerIndex.push_back(dwBL);
	pNode->m_CornerIndex.push_back(dwBR);

	ComputeBoundingBox(pNode);

	ldiv_t divValue = ldiv(dwTL, m_dwWidth); // ?
	pNode->m_dwPositionIndex[0] = divValue.rem / (dwTR - dwTL);  // 나머지 -> X
	pNode->m_dwPositionIndex[1] = divValue.quot / (dwTR - dwTL); // 몫 -> Y

	if (pParentNode)
	{
		pNode->m_dwDepth = pParentNode->m_dwDepth + 1;
		if (m_iMaxDepth < pNode->m_dwDepth)
		{
			m_iMaxDepth = pNode->m_dwDepth;
		}
	}
	return pNode;
}

void BMapTileRender::ComputeBoundingBox(BNode* pNode)
{
	TVector2 vHeight = GetHeightFromNode(pNode->m_CornerIndex[0], pNode->m_CornerIndex[1], pNode->m_CornerIndex[2], pNode->m_CornerIndex[3]);

	pNode->m_vCorner[0] = m_pMap->m_VertexList[pNode->m_CornerIndex[0]].Position;
	pNode->m_vCorner[1] = m_pMap->m_VertexList[pNode->m_CornerIndex[1]].Position;
	pNode->m_vCorner[2] = m_pMap->m_VertexList[pNode->m_CornerIndex[2]].Position;
	pNode->m_vCorner[3] = m_pMap->m_VertexList[pNode->m_CornerIndex[3]].Position;

	pNode->m_tBox.vMax = m_pMap->m_VertexList[pNode->m_CornerIndex[1]].Position;
	pNode->m_tBox.vMin = m_pMap->m_VertexList[pNode->m_CornerIndex[2]].Position;
	pNode->m_tBox.vMax.y = vHeight.x;
	pNode->m_tBox.vMin.y = vHeight.y;

	pNode->m_tBox.vCenter = pNode->m_tBox.vMax + pNode->m_tBox.vMin;
	pNode->m_tBox.vCenter /= 2.0f;
	pNode->m_tBox.fExtent[0] = pNode->m_tBox.vMax.x - pNode->m_tBox.vCenter.x;
	pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y;
	pNode->m_tBox.fExtent[2] = pNode->m_tBox.vMax.z - pNode->m_tBox.vCenter.z;
}

TVector2 BMapTileRender::GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR)
{
	assert(m_pMap);

	DWORD dwStartRow = dwTL / m_dwWidth; // 시작행
	DWORD dwEndRow = dwBL / m_dwWidth;  // 끝행

	DWORD dwStartCol = dwTL % m_dwWidth; // 시작열
	DWORD dwEndCol = dwTR % m_dwWidth; // 끝열

	TVector2 vHeight;
	vHeight.x = -999999.0f;
	vHeight.y = 999999.0f;
	
	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			if (m_pMap->m_VertexList[dwRow* m_dwWidth + dwCol].Position.y > vHeight.x)
			{
				vHeight.x = m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol].Position.y;
			}
			if (m_pMap->m_VertexList[dwRow* m_dwWidth + dwCol].Position.y < vHeight.y)
			{
				vHeight.y = m_pMap->m_VertexList[dwRow * m_dwWidth + dwCol].Position.y;
			}
		}
	}
	return vHeight;
}

 

void BMapTileRender::UpdateBoundingBox(BNode* pNode)
{
	ComputeBoundingBox(pNode);
	if (pNode->m_pChild[0] != NULL)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			UpdateBoundingBox(pNode->m_pChild[iNode]);
		}
	}
}

bool BMapTileRender::SubDivide(BNode* pNode)
{
	if (pNode == NULL) return false;

	// 현재 노드의 실제 크기를 계산
	DWORD dwOffsetX = (pNode->m_CornerIndex[1] - pNode->m_CornerIndex[0]);
	DWORD dwOffsetZ = ((pNode->m_CornerIndex[2] - pNode->m_CornerIndex[0]) / m_dwWidth);

	DWORD dwWidthSplit = CheckSize(dwOffsetX);
	DWORD dwHeightSplit = CheckSize(dwOffsetZ);

	// 최대 깊이 한도로 제한
	if (m_iMaxDepthLimit <= pNode->m_dwDepth)
	{
		pNode->m_isLeaf = TRUE;
		CreateVertexList(pNode);
		ComputeBoundingBox(pNode);
		//CreateIndexList(pNode);
		return false;
	}
	// 4등분 할 수 없으면 더이상 분할하지 않는다.
	if (dwWidthSplit < m_iMinSplitDistance || dwHeightSplit < m_iMinSplitDistance)
	{
		pNode->m_isLeaf = TRUE;
		CreateVertexList(pNode);
		ComputeBoundingBox(pNode);
		//CreateIndexList(pNode);
		return false;
	}
	float fWidthSplit = (pNode->m_vCorner[1].x - pNode->m_vCorner[0].x) / 2;
	float fHeightSplit = (pNode->m_vCorner[0].z - pNode->m_vCorner[2].z) / 2;
	//자식 노드가 지정된 분할크기보다 작다면 더이상 분할하지 않는다.
	if (fWidthSplit < m_fMinDivideSize || fHeightSplit < m_fMinDivideSize)
	{
		pNode->m_isLeaf = TRUE;
		CreateVertexList(pNode);
		ComputeBoundingBox(pNode);
		//CreateIndexList(pNode);
		return false;
	}
	DWORD dwSplitCenter = (pNode->m_CornerIndex[0] + dwWidthSplit) + (dwHeightSplit * m_dwWidth);
	
	DWORD dwEdgeCenter[4];
	dwEdgeCenter[0] = pNode->m_CornerIndex[0] + dwWidthSplit;
	dwEdgeCenter[1] = pNode->m_CornerIndex[1] + dwHeightSplit * m_dwWidth;
	dwEdgeCenter[2] = pNode->m_CornerIndex[2] + dwWidthSplit;
	dwEdgeCenter[3] = pNode->m_CornerIndex[0] + dwHeightSplit * m_dwWidth;

	pNode->m_pChild[0] = CreateNode(pNode, pNode->m_CornerIndex[0], dwEdgeCenter[0], dwEdgeCenter[3], dwSplitCenter);
	pNode->m_pChild[1] = CreateNode(pNode, dwEdgeCenter[0], pNode->m_CornerIndex[1], dwSplitCenter, dwEdgeCenter[1]);
	pNode->m_pChild[2] = CreateNode(pNode, dwEdgeCenter[3], dwSplitCenter, pNode->m_CornerIndex[2], dwEdgeCenter[2]);
	pNode->m_pChild[3] = CreateNode(pNode, dwSplitCenter, dwEdgeCenter[1], dwEdgeCenter[2], pNode->m_CornerIndex[3]);
	
	return true;
}

bool BMapTileRender::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_pMap->m_pConstantBuffer, 0, NULL, &m_pMap->m_cbData, 0, 0);
	UINT stride = sizeof(PNCT_Vertex);		 // 버텍스 하나 전체의 크기
	UINT offset = 0;						 // 버퍼 시작위치
	for (int iNode = 0; iNode < m_DrawPatchNodeList.size(); iNode++)
	{
		BNode* pNode = m_DrawPatchNodeList[iNode];
		GetLodType(pNode);
		pContext->IASetVertexBuffers(0, 1, pNode->m_pVertexBuffer.GetAddressOf(), &stride, &offset);
		pContext->IASetIndexBuffer(m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexBufferList[pNode->m_dwLodType].Get(), DXGI_FORMAT_R32_UINT, 0);
		pContext->UpdateSubresource(m_pMap->m_pConstantBuffer, 0, NULL, &m_pMap->m_cbData, 0, 0);
		pContext->DrawIndexed(m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType].size(), 0, 0);
	}
	return true;
}
/*Device에서 DX내의 DSV, DSS, BS를 전부 선언해 두었기 때문에 해당하는 값과 Core나 Sample에서 제작한 m_pRTV를 넘겨주기만 하면 된다.*/
/*해당 함수는 랜더링 전에 이루어져야 한다.*/
bool BMapTileRender::OMSetState(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* m_pRTV, ID3D11DepthStencilView* m_pDSV, ID3D11DepthStencilState* m_pDSS, ID3D11BlendState* m_pBS)
{
	pContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV);		// 랜더타겟뷰, 깊이버퍼 설정
	pContext->OMSetDepthStencilState(m_pDSS, 0x00);			// 깊이 스텐실 뷰설정
	pContext->OMSetBlendState(m_pBS, 0, -1);				// 알파블랜드 값 설정
	return true;
}

bool BMapTileRender::Render(ID3D11DeviceContext* pContext)
{
	m_pMap->PreRender(pContext);
	// 알파텍스쳐에 입힐 이미지 등록
	if (m_pSplatting.size())
	{
		for (int iMap = 0; iMap < 4; iMap++)
		{
			pContext->PSSetShaderResources((iMap+2), 1, m_pSplatting[iMap].m_pTextureSRV[0].GetAddressOf()); // 텍스쳐 세팅
		}
	}
	// 알파텍스쳐 등록
	if (SpreatingSRV.Get())
	{
		pContext->PSSetShaderResources(1, 1, SpreatingSRV.GetAddressOf()); // 텍스쳐 세팅
	}
	PostRender(pContext);
	return true;
}

bool BMapTileRender::AlphaRender(ID3D11DeviceContext* pContext)
{
	m_pMap->PreRender(pContext);
	//// 알파텍스쳐에 입힐 이미지 등록
	//if (m_pSplattingSRV.size())
	//{
	//	for (int iMap = 0; iMap < 4; iMap++)
	//	{
	//		pContext->PSSetShaderResources((iMap + 2), 1, m_pSplattingSRV[iMap].GetAddressOf()); // 텍스쳐 세팅
	//	}
	//}
	// 알파텍스쳐 등록
	if (SpreatingSRV.Get())
	{
		pContext->PSSetShaderResources(1, 1, SpreatingSRV.GetAddressOf()); // 텍스쳐 세팅
	}
	pContext->PSSetShader(m_pMap->m_pAlphaPS, NULL, 0);
	PostRender(pContext);
	return true;
}


bool BMapTileRender::CreateVertexList(BNode* pNode)
{
	DWORD dwTL = pNode->m_CornerIndex[0];
	DWORD dwTR = pNode->m_CornerIndex[1];
	DWORD dwBL = pNode->m_CornerIndex[2];
	DWORD dwBR = pNode->m_CornerIndex[3];

	DWORD dwSize = ((dwTR - dwTL) + 1)*((dwTR - dwTL) + 1);
	pNode->m_VertexList.resize(dwSize);
	pNode->m_TangentList.resize(dwSize);

	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow = dwBL / m_dwWidth;

	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol = dwTR % m_dwWidth;

	DWORD iVertexIndex = 0;

	float MinX, MaxX, MinY, MaxY, MinZ, MaxZ;
	MinX = 99999.0f;
	MinY = 99999.0f;
	MinZ = 99999.0f;
	MaxX = -99999.0f;
	MaxY = -99999.0f;
	MaxZ = -99999.0f;

	for (DWORD dwRow = dwStartRow; dwRow <= dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol <= dwEndCol; dwCol++)
		{
			pNode->m_VertexList[iVertexIndex] = m_pMap->m_VertexList[dwRow*m_dwWidth + dwCol];
			pNode->m_TangentList[iVertexIndex++] = m_pMap->m_TangentList[dwRow*m_dwWidth + dwCol];

			/*if (pNode->m_VertexList[iVertexIndex].Position.x < MinX)
				MinX = pNode->m_VertexList[iVertexIndex].Position.x;
			if (pNode->m_VertexList[iVertexIndex].Position.y < MinY)
				MinY = pNode->m_VertexList[iVertexIndex].Position.y;
			if (pNode->m_VertexList[iVertexIndex].Position.z < MinZ)
				MinZ = pNode->m_VertexList[iVertexIndex].Position.z;
			if (pNode->m_VertexList[iVertexIndex].Position.x > MaxX)
				MaxX = pNode->m_VertexList[iVertexIndex].Position.x;
			if (pNode->m_VertexList[iVertexIndex].Position.y > MaxY)
				MaxY = pNode->m_VertexList[iVertexIndex].Position.y;
			if (pNode->m_VertexList[iVertexIndex].Position.z > MaxZ)
				MaxZ = pNode->m_VertexList[iVertexIndex].Position.z;
			
			pNode->m_tBox.vMax = TVector3(MaxX, MaxY, MaxZ);
			pNode->m_tBox.vMin = TVector3(MinX, MinY, MinZ);
			pNode->m_tBox.vCenter = TVector3((MaxX - MinX) / 2, (MaxY - MinY) / 2, (MaxZ - MinZ) / 2);
			pNode->m_tBox.fExtent[0] = MaxX - pNode->m_tBox.vCenter.x;
			pNode->m_tBox.fExtent[1] = MaxY - pNode->m_tBox.vCenter.y;
			pNode->m_tBox.fExtent[2] = MaxZ - pNode->m_tBox.vCenter.z;*/
		}
	}
	pNode->m_pVertexBuffer.Attach(CreateVertexBuffer(m_pd3dDevice, &pNode->m_VertexList.at(0), pNode->m_VertexList.size(), sizeof(PNCT_Vertex)));
	pNode->m_pTangentBuffer.Attach(CreateVertexBuffer(m_pd3dDevice, &pNode->m_TangentList.at(0), pNode->m_TangentList.size(), sizeof(TVector3)));

	return true;
}

int	BMapTileRender::CreateIndexList(BNode* pNode)
{
	vector<DWORD> Indexlist;
	DWORD dwTL = pNode->m_CornerIndex[0];
	DWORD dwTR = pNode->m_CornerIndex[1];
	DWORD dwBL = pNode->m_CornerIndex[2];
	DWORD dwBR = pNode->m_CornerIndex[3];

	DWORD dwSize = (dwTR - dwTL) * (dwTR - dwTL) * 2 * 3;
	pNode->m_IndexList.resize(dwSize);

	int dwCurrentIndex = 0;

	DWORD dwStartRow = dwTL / m_dwWidth;
	DWORD dwEndRow = dwBL / m_dwWidth;
	DWORD dwStartCol = dwTL % m_dwWidth;
	DWORD dwEndCol = dwTR % m_dwWidth;

	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol < dwEndCol; dwCol++)
		{
			// 0 1   4
			// 2   3 5
			DWORD dwNextRow = dwRow + 1;
			DWORD dwNextCol = dwCol + 1;
			pNode->m_IndexList[dwCurrentIndex++] = dwRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurrentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_IndexList[dwCurrentIndex++] = dwNextRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurrentIndex++] = dwNextRow * m_dwWidth + dwCol;
			pNode->m_IndexList[dwCurrentIndex++] = dwRow * m_dwWidth + dwNextCol;
			pNode->m_IndexList[dwCurrentIndex++] = dwNextRow * m_dwWidth + dwNextCol;
		}
	}
	pNode->m_pIndexBuffer.Attach(CreateIndexBuffer(m_pd3dDevice, &pNode->m_IndexList.at(0), pNode->m_IndexList.size(), sizeof(DWORD)));

	pNode->m_BoxIB.left = m_dstibOffset;
	pNode->m_BoxIB.right = m_dstibOffset + pNode->m_IndexList.size() * sizeof(DWORD);
	pNode->m_BoxIB.top = 0; 
	pNode->m_BoxIB.bottom = 1;
	pNode->m_BoxIB.front = 0;
	pNode->m_BoxIB.back = 1;

	m_pImmediateContext->UpdateSubresource(m_pIndexBuffer.Get(), 0,&pNode->m_BoxIB, (void*)&pNode->m_IndexList.at(0), 0, 0);
	pNode->m_iBeginIB = m_ibOffset;
	m_ibOffset += pNode->m_IndexList.size();
	m_dstibOffset = pNode->m_BoxIB.right;

	return Indexlist.size() / 3;
}

void BMapTileRender::SetRenderDepth(DWORD dwDepth)
{
	if (m_iMaxDepth <= dwDepth) m_iRenderDepth = -1;
	else m_iRenderDepth = dwDepth;
}

HRESULT BMapTileRender::CreateSpreatingTexture()
{
	HRESULT hr = S_OK;
	desc.Width = m_pMap->m_iNumRows * m_pMap->m_fCellDistance* 10.0f;
	desc.Height = m_pMap->m_iNumCols * m_pMap->m_fCellDistance* 10.0f;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	hr = m_pd3dDevice->CreateTexture2D(&desc, NULL, &SpreatingTexTure);

	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	hr = m_pd3dDevice->CreateTexture2D(&desc, NULL, &StagingTexTure);


	Objdesc.Width = m_pMap->m_iNumRows * m_pMap->m_fCellDistance;
	Objdesc.Height = m_pMap->m_iNumCols * m_pMap->m_fCellDistance;
	Objdesc.MipLevels = 1;
	Objdesc.ArraySize = 1;
	Objdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Objdesc.SampleDesc.Count = 1;
	Objdesc.SampleDesc.Quality = 0;
	Objdesc.Usage = D3D11_USAGE_DEFAULT;
	Objdesc.CPUAccessFlags = 0;
	Objdesc.MiscFlags = 0;
	Objdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	hr = m_pd3dDevice->CreateTexture2D(&Objdesc, NULL, &ObjPosTexture);

	Objdesc.Usage = D3D11_USAGE_STAGING;
	Objdesc.BindFlags = 0;
	Objdesc.MiscFlags = 0;
	Objdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	hr = m_pd3dDevice->CreateTexture2D(&Objdesc, NULL, &StagingObjPosTexture);

	return hr;
}


//1. 레드 - 적 몬스터의 리스폰 가능여부
//2. 그린 - 이동불가지역
//3. 블루 - 아이템 리스폰 가능여부
void BMapTileRender::ObjSpreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, TVector3 CrashPoint, float fRadius, int iColor)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	TVector3 CrashDot = CrashPoint;
	
	CrashDot.x = (int)CrashDot.x;
	CrashDot.y = 0.0f;
	CrashDot.z = (int)CrashDot.z;

	B_BOX FixedArea;

	FixedArea.vMax.x = CrashDot.x + fRadius;
	FixedArea.vMax.z = CrashDot.z + fRadius;
	FixedArea.vMin.x = CrashDot.x - fRadius + 1.0f;
	FixedArea.vMin.z = CrashDot.z - fRadius + 1.0f;

	if (FixedArea.vMin.x < 0.0f)
	{
		FixedArea.vMin.x = 0.0f;
	}
	if (FixedArea.vMin.z < 0.0f)
	{
		FixedArea.vMin.z = 0.0f;
	}
	if (FixedArea.vMax.x > Objdesc.Width)
	{
		FixedArea.vMax.x = Objdesc.Width;
	}
	if (FixedArea.vMax.z > Objdesc.Height)
	{
		FixedArea.vMax.z = Objdesc.Height;
	}

	// 한점만을 특정하기 위한 변수
	float fCheckRadius = fRadius * 0.9f;

	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)StagingObjPosTexture, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		pDestBytes = pDestBytes + MappedFaceDest.RowPitch * (UINT)FixedArea.vMin.z;
		for (int y = FixedArea.vMin.z ; y < FixedArea.vMax.z; y++)
		{
			BYTE* pDest = pDestBytes;
			pDest = pDest + (UINT)(4 * FixedArea.vMin.x);
			for (int x = FixedArea.vMin.x ; x < FixedArea.vMax.x; x++)
			{
				switch (iColor)
				{
				case 0:
					*pDest++ = 255;
					*pDest++ = 0;
					*pDest++ = 0;
					*pDest++ = 0;
					break;
				case 1:
					*pDest++ = 0;
					*pDest++ = 255;
					*pDest++ = 0;
					*pDest++ = 0;
					break;
				case 2:
					*pDest++ = 0;
					*pDest++ = 0;
					*pDest++ = 255;
					*pDest++ = 0;
					break;
				case 3:
					*pDest++ = 0;
					*pDest++ = 0;
					*pDest++ = 0;
					*pDest++ = 255;
					break;
				default:
					break;
				}
			}
			pDestBytes += MappedFaceDest.RowPitch;
		}
		pImmediateContext->Unmap(StagingObjPosTexture, 0);
		pImmediateContext->CopyResource(ObjPosTexture, StagingObjPosTexture);
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.Format = Objdesc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = Objdesc.MipLevels;
		FAILED(hr = m_pd3dDevice->CreateShaderResourceView(ObjPosTexture, &SRVDesc, SpreatingSRV.GetAddressOf()));
	}
}

void BMapTileRender::Spreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, TVector3 CrashPoint, float fRadius, int iColor)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	TVector3 CrashDot = CrashPoint;
	CrashDot.x = CrashDot.x * desc.Width;
	CrashDot.y = 0.0f;
	CrashDot.z = CrashDot.z * desc.Height;
	B_BOX FixedArea;
	FixedArea.vMax.x = (int)(Area.vMax.x * desc.Width);
	FixedArea.vMax.z = (int)(Area.vMax.z * desc.Height);
	FixedArea.vMin.x = (int)(Area.vMin.x * desc.Width);
	FixedArea.vMin.z = (int)(Area.vMin.z * desc.Height);

	if (FixedArea.vMin.x < 0.0f)
	{
		FixedArea.vMin.x = 0.0f;
	}
	if (FixedArea.vMin.z < 0.0f)
	{
		FixedArea.vMin.z = 0.0f;
	}
	if (FixedArea.vMax.x > desc.Width)
	{
		FixedArea.vMax.x = desc.Width;
	}
	if (FixedArea.vMax.z > desc.Height)
	{
		FixedArea.vMax.z = desc.Height;
	}

	float CheckRadius = fRadius * 10.0f; 
	if (SUCCEEDED(pImmediateContext->Map((ID3D11Resource*)StagingTexTure, 0, D3D11_MAP_READ_WRITE, 0, &MappedFaceDest)))
	{
		BYTE* pDestBytes = (BYTE*)MappedFaceDest.pData;
		pDestBytes = pDestBytes + MappedFaceDest.RowPitch * (UINT)FixedArea.vMin.z;
		for (int y = FixedArea.vMin.z; y < FixedArea.vMax.z; y++)
		{
			BYTE* pDest = pDestBytes;
			pDest = pDest + (UINT)(4 * FixedArea.vMin.x);
			for (int x = FixedArea.vMin.x; x < FixedArea.vMax.x; x++)
			{
				TVector3 Dot = TVector3(x, 0.0f, y);
				if (CheckRadius >= D3DXVec3Length(&(Dot - CrashDot)))
				{
					switch (iColor)
					{
					case 0:
						*pDest++ = 255;
						*pDest++ = 0;
						*pDest++ = 0;
						*pDest++ = 0;
						break;
					case 1:
						*pDest++ = 0;
						*pDest++ = 255;
						*pDest++ = 0;
						*pDest++ = 0;
						break;
					case 2:
						*pDest++ = 0;
						*pDest++ = 0;
						*pDest++ = 255;
						*pDest++ = 0;
						break;
					case 3:
						*pDest++ = 0;
						*pDest++ = 0;
						*pDest++ = 0;
						*pDest++ = 255;
						break;
					default:
						break;
					}
				}
				else
				{
					pDest += 4;
				}
			}
			pDestBytes += MappedFaceDest.RowPitch;
		}
		pImmediateContext->Unmap(StagingTexTure, 0);
		pImmediateContext->CopyResource(SpreatingTexTure, StagingTexTure);
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		SRVDesc.Format = desc.Format;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = desc.MipLevels;
		FAILED(hr = m_pd3dDevice->CreateShaderResourceView(SpreatingTexTure, &SRVDesc, SpreatingSRV.GetAddressOf()));
	}
}

HRESULT BMapTileRender::CreateSRV(ID3D11Texture2D* SpreatingTex, D3D11_TEXTURE2D_DESC SpreatingDesc)
{
	HRESULT hr = S_OK;
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	SRVDesc.Format = SpreatingDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = SpreatingDesc.MipLevels;
	FAILED(hr = m_pd3dDevice->CreateShaderResourceView(SpreatingTex, &SRVDesc, SpreatingSRV.GetAddressOf()));
	return hr;
}

HRESULT BMapTileRender::SplattingTextureLoad(ID3D11Device* pDevice, const TCHAR* strFilePath, int iColor)
{
	HRESULT hr = S_OK;
	if (strFilePath == NULL) 
		m_pSplattingSRV[iColor].Attach(nullptr);
	ID3D11ShaderResourceView* pSRV = nullptr;
	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(pDevice, strFilePath, NULL, NULL, &pSRV, NULL)))
	{
		m_pSplattingSRV[iColor].Attach(nullptr);
	}
	m_pSplattingSRV[iColor].Attach(pSRV);
	return hr;
}

BMapTileRender::BMapTileRender()
{
	m_dwWidth = 0;
	m_dwHeight = 0;
	m_iNumFace = 0;
	m_iMinSplitDistance = 2;
	m_bSharedVertexBuffer = true;
	m_dstibOffset = 0;
	m_ibOffset = 0;
}


BMapTileRender::~BMapTileRender()
{
}
