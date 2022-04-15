#include "MapImporter.h"

bool MapImporter::OpenStream(const TCHAR* pFileName, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, BCamera* MainCamera)
{
	m_pStream = _tfopen(pFileName, _T("rt"));
	if (m_pStream == NULL)
	{
		if (!m_pStream) return false;
		fclose(m_pStream);
		MessageBox(0, _T("파일을 찾지 못했습니다."), _T("Fatal Error"), MB_OK);
	}
	m_pd3dDevice = pd3dDevice;
	m_pImmediateContext = pImmediateContext;
	m_MainCamera = MainCamera;
	return true;
}

bool MapImporter::LoadMap(vector<BMap*>* m_vMFCMap, vector<BMapDesc*>* m_vMFCMapInfo, vector<BMapTileRender*>* m_vMapQuadTree, vector<TCHAR*>* m_vMapTexture, map<int, TCHAR*>* m_mMapSplattingTexture, vector<BWaterMap*>* m_vWaterQuadTree)
{
	// 알파맵 알파값텍스쳐 문자열 저장
	TCHAR StagingTex[256];
	TCHAR StagingSpawnTex[256];
	ZeroMemory(StagingTex, sizeof(StagingTex));
	ZeroMemory(StagingSpawnTex, sizeof(StagingSpawnTex));
	ZeroMemory(MapTexture, sizeof(MapTexture));
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %s"), m_pString, StagingTex);

	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %s"), m_pString, StagingSpawnTex);

	// 베이스 텍스쳐
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %s"), m_pString, MapTexture);
	m_vMapTexture->clear();
	m_vMapTexture->push_back(MapTexture);

	// 알파 텍스쳐
	m_mMapSplattingTexture->clear();
	map<int, TCHAR*> SplattingTex;
	_fgetts(m_pBuffer, 256, m_pStream);
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %s"), m_pString, m_pSplatting[0]);
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %s"), m_pString, m_pSplatting[1]);
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %s"), m_pString, m_pSplatting[2]);
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %s"), m_pString, m_pSplatting[3]);
	m_mMapSplattingTexture[0][0] = m_pSplatting[0];
	m_mMapSplattingTexture[0][1] = m_pSplatting[1];
	m_mMapSplattingTexture[0][2] = m_pSplatting[2];
	m_mMapSplattingTexture[0][3] = m_pSplatting[3];

	// WaterMap
	_fgetts(m_pBuffer, 256, m_pStream);
	BWaterMap* LoadWaterMap = new BWaterMap;
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %d\n"), m_pString, &LoadWaterMap->m_iSelectedMap);
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %s\n"), m_pString, m_WaterMapTex);
	LoadWaterMap->m_strWaterTex = m_WaterMapTex;
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %s\n"), m_pString, m_WaterNormalMapTex);
	LoadWaterMap->m_strWaterNormalTex = m_WaterNormalMapTex;
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %f\n"), m_pString, &LoadWaterMap->m_fWaterHeight);
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %f\n"), m_pString, &LoadWaterMap->m_fWaterWave);
	m_vWaterQuadTree->push_back(LoadWaterMap);

	// CellCount & PatchCount & CellDistance
	BMapDesc* LoadMapdesc = new BMapDesc;
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %d"), m_pString, &LoadMapdesc->iCellCount);
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %d"), m_pString, &LoadMapdesc->iPatchCount);
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %f"), m_pString, &LoadMapdesc->fCellDistance);

	m_vMFCMapInfo->push_back(LoadMapdesc);

	// 맵 생성
	BMap* LoadMap = new BMap;
	TCHAR* BaseMap[1];
	BaseMap[0] = MapTexture;
	LoadMap->SetMapDesc(*LoadMapdesc);
	LoadMap->Create(m_pd3dDevice, m_pImmediateContext, BaseMap, 1, NULL, L"../../data/shader/NormalShader.hlsl");
	LoadMap->UpdateNormal();

	m_vMFCMap->push_back(LoadMap);

	// 쿼드트리 생성
	BMapTileRender* LoadMapQuadTree = new BMapTileRender;
	LoadMapQuadTree->m_iMaxDepthLimit = 2;
	LoadMapQuadTree->m_fMinDivideSize = 1.0f;
	LoadMapQuadTree->Update(m_pd3dDevice, m_MainCamera);
	LoadMapQuadTree->Build(LoadMap, LoadMap->m_iNumCols, LoadMap->m_iNumRows, m_pImmediateContext);

	GetHeightFromTex(LoadMapQuadTree->m_pRootNode);
	LoadMapQuadTree->CreateSpreatingTexture();
	LoadMapQuadTree->StagingTexTure = LoadAlphaTexture(StagingTex, LoadMap);
	LoadMapQuadTree->StagingObjPosTexture = LoadSpawnTexture(StagingSpawnTex, LoadMap);

	m_pImmediateContext->CopyResource(LoadMapQuadTree->SpreatingTexTure, LoadMapQuadTree->StagingTexTure);
	m_pImmediateContext->CopyResource(LoadMapQuadTree->ObjPosTexture, LoadMapQuadTree->StagingObjPosTexture);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	SRVDesc.Format = LoadMapQuadTree->desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = LoadMapQuadTree->desc.MipLevels;
	m_pd3dDevice->CreateShaderResourceView(LoadMapQuadTree->SpreatingTexTure, &SRVDesc, LoadMapQuadTree->SpreatingSRV.GetAddressOf());

	m_vMapQuadTree->push_back(LoadMapQuadTree);
	LoadMapQuadTree->SplattingTextureLoad(m_pd3dDevice, m_pSplatting[0], 0);
	LoadMapQuadTree->SplattingTextureLoad(m_pd3dDevice, m_pSplatting[1], 1);
	LoadMapQuadTree->SplattingTextureLoad(m_pd3dDevice, m_pSplatting[2], 2);
	LoadMapQuadTree->SplattingTextureLoad(m_pd3dDevice, m_pSplatting[3], 3);

	return true;
}

void MapImporter::GetHeightFromTex(BNode* Node)
{
	if (!Node->m_isLeaf)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			GetHeightFromTex(Node->m_pChild[iNode]);
		}
	}
	else
	{
		_fgetts(m_pBuffer, 256, m_pStream); // 1번 리프노드의 경우 *Height 제거, 다른 리프노드의 경우 빈칸 제거
											// m_tBox 입력순서 = Min xyz, Max xyz, Center xyz, fExtent 0, 1, 2
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%f %f %f"), &Node->m_tBox.vMin.x, &Node->m_tBox.vMin.y, &Node->m_tBox.vMin.z);
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%f %f %f"), &Node->m_tBox.vMax.x, &Node->m_tBox.vMax.y, &Node->m_tBox.vMax.z);
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%f %f %f"), &Node->m_tBox.vCenter.x, &Node->m_tBox.vCenter.y, &Node->m_tBox.vCenter.z);
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%f %f %f"), &Node->m_tBox.fExtent[0], &Node->m_tBox.fExtent[1], &Node->m_tBox.fExtent[2]);
		for (int iVer = 0; iVer < Node->m_VertexList.size(); iVer++)
		{
			float Height = 0;
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%f"), &Height);
			Node->m_VertexList[iVer].Position.y = Height;
		}

		HRESULT hr = S_OK;
		ID3D11Buffer* pBuffer = nullptr;

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = Node->m_VertexList.size() * sizeof(PNCT_Vertex);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &Node->m_VertexList.at(0);
		hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &pBuffer);
		Node->m_pVertexBuffer.Attach(pBuffer);
	}
}

ID3D11Texture2D* MapImporter::LoadAlphaTexture(TCHAR* texture, BMap* Map)
{
	HRESULT hr = S_OK;
	ComPtr<ID3D11Resource> pResource = NULL;
	size_t maxsize = 0;

	if (FAILED(hr = DirectX::CreateWICTextureFromFileEx(g_pd3dDevice,
		texture,
		maxsize,
		D3D11_USAGE_STAGING,
		0,
		D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
		0,
		WIC_LOADER_DEFAULT,
		pResource.GetAddressOf(), nullptr)))
	{
		return nullptr;
	}


	D3DX11_IMAGE_LOAD_INFO d3d_image_load_info;
	d3d_image_load_info.Usage = D3D11_USAGE_STAGING;
	d3d_image_load_info.CpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	d3d_image_load_info.BindFlags = 0;
	d3d_image_load_info.MipLevels = 1;
	d3d_image_load_info.Width = Map->m_iNumRows * Map->m_fCellDistance * 10.0f;
	d3d_image_load_info.Height = Map->m_iNumRows * Map->m_fCellDistance * 10.0f;
	d3d_image_load_info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//d3d_image_load_info.Filter = D3DX11_FILTER_LINEAR;
	ID3D11Texture2D* Alphatexture = NULL;
	ID3D11Resource*  pRes = NULL;
	hr = D3DX11CreateTextureFromFile(m_pd3dDevice, texture, &d3d_image_load_info, NULL, &pRes, NULL);
	if (FAILED(hr))
	{
		return NULL;
	}
	pRes->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&Alphatexture);
	pRes->Release();

	return Alphatexture;
}

ID3D11Texture2D* MapImporter::LoadSpawnTexture(TCHAR* texture, BMap* Map)
{
	D3DX11_IMAGE_LOAD_INFO d3d_image_load_info;
	d3d_image_load_info.Usage = D3D11_USAGE_STAGING;
	d3d_image_load_info.CpuAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	d3d_image_load_info.BindFlags = 0;
	d3d_image_load_info.MipLevels = 1;
	d3d_image_load_info.Width = Map->m_iNumRows * Map->m_fCellDistance;
	d3d_image_load_info.Height = Map->m_iNumRows * Map->m_fCellDistance;
	d3d_image_load_info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//d3d_image_load_info.Filter = D3DX11_FILTER_LINEAR;
	ID3D11Texture2D* Alphatexture = NULL;
	ID3D11Resource*  pRes = NULL;
	HRESULT hr = D3DX11CreateTextureFromFile(m_pd3dDevice, texture, &d3d_image_load_info, NULL, &pRes, NULL);
	if (FAILED(hr))
	{
		return NULL;
	}
	pRes->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&Alphatexture);
	pRes->Release();

	return Alphatexture;
}

bool MapImporter::LoadMapObj(vector<BAseObj*>* ObjMgr)
{
	int TotalObjCount = 0;
	int ObjCount = 0;
	TCHAR ObjName[MAX_PATH] = { 0 };
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %d\n"), m_pString, &TotalObjCount);

	//ObjMgr[0].resize(TotalObjCount);
	for (int iObj = 0; iObj < TotalObjCount; iObj++)
	{
		ZeroMemory(ObjName, sizeof(ObjName));
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s %d\n"), m_pString, &ObjCount);
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s\n"), ObjName);
		m_MapObj = new BAseObj;
		m_MapObj->Load(m_pd3dDevice, ObjName);
		m_MapObj->InitInstanceBuffer(ObjCount);
		for (int iInstance = 0; iInstance < ObjCount; iInstance++)
		{
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%d\n"), &m_MapObj->m_InstanceInfo[iInstance].iInstanceNumber);
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%f %f %f\n"), &m_MapObj->m_InstanceInfo[iInstance].Angle[0], &m_MapObj->m_InstanceInfo[iInstance].Angle[1], &m_MapObj->m_InstanceInfo[iInstance].Angle[2]);
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%f %f %f\n"), &m_MapObj->m_InstanceInfo[iInstance].Scale.x, &m_MapObj->m_InstanceInfo[iInstance].Scale.y, &m_MapObj->m_InstanceInfo[iInstance].Scale.z);
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%f %f %f\n"), &m_MapObj->m_InstanceInfo[iInstance].Translate.x, &m_MapObj->m_InstanceInfo[iInstance].Translate.y, &m_MapObj->m_InstanceInfo[iInstance].Translate.z);
			m_MapObj->m_InstanceInfo[iInstance].ObjName = iObj;
		}
		m_MapObj->Frame();
		ObjMgr->push_back(m_MapObj);
	}
	return true;
}
//{
//	int ObjCount = 0;
//	_fgetts(m_pBuffer, 256, m_pStream);
//	_stscanf(m_pBuffer, _T("%s %d"), m_pString, &ObjCount);
//	MapObjPos->resize(ObjCount);
//	for (int ObjCnt = 0; ObjCnt < ObjCount; ObjCnt++)
//	{
//		_fgetts(m_pBuffer, 256, m_pStream);
//		_stscanf(m_pBuffer, _T("%f %f %f"), &MapObjPos[0][ObjCnt].x, &MapObjPos[0][ObjCnt].y, &MapObjPos[0][ObjCnt].z);
//	}
//	return true;
//}

MapImporter::MapImporter()
{
}

MapImporter::~MapImporter()
{
}
