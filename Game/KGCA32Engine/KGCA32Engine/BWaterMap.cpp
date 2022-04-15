#include "BWaterMap.h"

// 워터맵의 높이맵 //
// Create 에서 높이맵을 받아온다.
// 워터맵의 텍스쳐는 워터맵에 해당하는 높이 텍스쳐만을 사용한다. 
// 만약 높이텍스쳐를 사용하지 않는다면 ?
// 워터맵은 맵과 같은 크기의 맵이다. 맵을 읽어올때와 같은 방식으로 y값을 받아와서 워터맵의 높이맵 생성이 가능해진다.
// 맵을 읽어올 때 워터맵도 함께 생성하도록 한다.

void BWaterMap::ComputeBoundingBox(BNode* pNode)
{
	TVector2 vHeight = GetHeightFromNode(pNode->m_CornerIndex[0], pNode->m_CornerIndex[1], pNode->m_CornerIndex[2], pNode->m_CornerIndex[3]);

	pNode->m_vCorner[0] = m_pMap->m_VertexList[pNode->m_CornerIndex[0]].Position;
	pNode->m_vCorner[1] = m_pMap->m_VertexList[pNode->m_CornerIndex[1]].Position;
	pNode->m_vCorner[2] = m_pMap->m_VertexList[pNode->m_CornerIndex[2]].Position;
	pNode->m_vCorner[3] = m_pMap->m_VertexList[pNode->m_CornerIndex[3]].Position;

	pNode->m_tBox.vMax = m_pMap->m_VertexList[pNode->m_CornerIndex[1]].Position;
	pNode->m_tBox.vMin = m_pMap->m_VertexList[pNode->m_CornerIndex[2]].Position;
	pNode->m_tBox.vMax.y = m_fWaterHeight;
	pNode->m_tBox.vMin.y = 0;

	pNode->m_tBox.vCenter = (pNode->m_tBox.vMax + pNode->m_tBox.vMin);
	pNode->m_tBox.vCenter /= 2.0f;
	pNode->m_tBox.fExtent[0] = pNode->m_tBox.vMax.x - pNode->m_tBox.vCenter.x;
	pNode->m_tBox.fExtent[1] = pNode->m_tBox.vMax.y - pNode->m_tBox.vCenter.y + 4.0f + m_fWaterWave;
	pNode->m_tBox.fExtent[2] = pNode->m_tBox.vMax.z - pNode->m_tBox.vCenter.z;
}

HRESULT BWaterMap::TextureLoad(const TCHAR* strWaterTex, const TCHAR* strNormalTex, const TCHAR* strHeightTex)
{
	HRESULT hr = S_OK;
	if (strWaterTex)
	{
		hr = SetWaterTexture(strWaterTex);
	}
	if (strNormalTex)
	{
		hr = SetWaterNormalTexture(strNormalTex);
	}
	if (strHeightTex)
	{
		hr = SetHeightTexture(strHeightTex);
	}
	return hr;
}

// 워터의 텍스쳐 질감을 표현해 줄 노말맵 ( 아주 중요 )
HRESULT BWaterMap::SetWaterTexture(const TCHAR* strFilePath)
{
	HRESULT hr = S_OK;
	string ext = GetFileExtension((TCHAR*)strFilePath);
	m_Water.LoadTexture(strFilePath, mtw(ext));
	return hr;
}
HRESULT BWaterMap::SetWaterNormalTexture(const TCHAR* strFilePath)
{
	HRESULT hr = S_OK;
	string ext = GetFileExtension((TCHAR*)strFilePath);
	m_WaterNormal.LoadTexture(strFilePath, mtw(ext));
	return hr;
}
HRESULT BWaterMap::SetHeightTexture(const TCHAR* strFilePath)
{
	HRESULT hr = S_OK;
	string ext = GetFileExtension((TCHAR*)strFilePath);
	m_WaterHeight.LoadTexture(strFilePath, mtw(ext));
	return hr;
}

/*여기서 넘기는 pMap은 실제 Map이 아니라 WaterPlane 이다.*/
bool BWaterMap::Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pContext, TCHAR* strWaterTex, TCHAR* strNormalTex, TCHAR* strHeightTex)
{
	bHeightChange = false;
	bRequestCreate = false;

	m_strWaterTex = strWaterTex;
	m_strWaterNormalTex = strNormalTex;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_pImmediateContext = pContext;
	//m_pMap은 실제 물이 될 평면
	//받아온 pMap은 텍스쳐로 이용하기 위한 지형
	m_pMap = new BNMap;
	m_pMap->SetMapDesc(pMap->m_MapDesc);
	m_pMap->Create(m_pd3dDevice, m_pImmediateContext, NULL, 0, 0, L"../../data/shader/MapShader.hlsl"); // BN은 ClipPS를 쓰기 때문에 별도로 쉐이더를 생성한다.
	m_pMap->UpdateNormal();
	CreateWaterMapInputLayout(L"../../data/shader/WaterMapShader.hlsl");
	m_pWaterTexMap = pMap;

	// Height 플랜 생성
	m_HeightMap.Create(m_pd3dDevice, L"../../data/shader/TestShader.hlsl", pMap->m_MapDesc.iNumCols, pMap->m_MapDesc.iNumRows);
	m_HeightMap.Set(m_pd3dDevice, 200, 0, m_pMap->m_iNumRows, m_pMap->m_iNumRows);
	if (pMap->m_tBox.vMax.y != 0)
	{
		m_HeightMap.m_pMiniMapRT->DrawHeightTex(m_pd3dDevice, m_pImmediateContext, pMap);
	}
	else
	{
		m_HeightMap.m_pMiniMapRT->DrawQuadTreeHeightTex(m_pd3dDevice, m_pImmediateContext, m_SelectedMap);
	}
	m_WaterHeight.m_pTextureSRV[0] = m_HeightMap.m_pMiniMapRT->m_pSRV;

	// 텍스쳐 생성
	TextureLoad(strWaterTex, strNormalTex, strHeightTex);

	// 상수버퍼 생성
	CreateWaterConstantBuffer();

	m_iNumFace = (dwWidth - 1) *(dwHeight - 1) * 2;
	m_bUsedIndexList = true;

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
		return true;
	}
	return false;
}

// 높이값의 변화로 물의 높낮이 설정
// Init 와는 별개로 설정해주어야 한다.
bool BWaterMap::SetMapHeight(float Height)
{
	m_fWaterHeight = Height;
	m_pMap->m_matWorld._42 = m_fWaterHeight;
	return true;
}

bool BWaterMap::SetMapHeight()
{
	m_pMap->m_matWorld._42 = m_fWaterHeight;
	return true;
}

HRESULT BWaterMap::CreateWaterMapInputLayout(const TCHAR* pShaderFile) // WaterMapShader를 이용
{
	HRESULT hr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // 쉐이더 디버그 허용
	ID3DBlob* pVSBUF = NULL;
	ID3DBlob* pPSBUF = NULL;
	ID3DBlob* pGSBUF = NULL;

	ID3DBlob* pError = NULL; // 이것을 넣음으로써 오류가 난 이유를 받아 낼 수 있다.
							 /* vs_5_0 == direct11 은 5.0을 사용 * pVSBuf 결과가 담김 */

	if (FAILED(D3DCompileFromFile(pShaderFile, NULL, NULL, "VS", "vs_5_0", NULL, NULL, &pVSBUF, &pError)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		MessageBox(NULL, buffer, _T("VS"), MB_OK);
	}
	if (FAILED(D3DCompileFromFile(pShaderFile, NULL, NULL, "GS", "gs_5_0", NULL, NULL, &pGSBUF, &pError)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		//MessageBox(NULL, buffer, _T("GS"), MB_OK);
	}

	if (FAILED(D3DCompileFromFile(pShaderFile, NULL, NULL, "PS", "ps_5_0", NULL, NULL, &pPSBUF, &pError)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		MessageBox(NULL, buffer, _T("PS"), MB_OK);
	}
	if (pVSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreateVertexShader((DWORD*)pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), NULL, &m_pWaterVS));
	}
	if (pPSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreatePixelShader((DWORD*)pPSBUF->GetBufferPointer(), pPSBUF->GetBufferSize(), NULL, &m_pWaterPS));
	}

	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32_FLOAT,       1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iSize = sizeof(layout) / sizeof(layout[0]);

	S_RETURN(m_pd3dDevice->CreateInputLayout(layout, iSize, pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), &m_pMap->m_pVertexLayout)); // m_pVertexLayout 제작
	S_RELEASE(pVSBUF);
	S_RELEASE(pPSBUF);
	S_RELEASE(pGSBUF);
	return hr;
}


void BWaterMap::SetWaterMatrix(TMatrix* World, TMatrix* View, TMatrix* Proj)
{
	if (World != NULL)
	{
		m_pMap->m_matWorld = *World;
		m_pRootNode->m_tBox.vCenter.x = World->_41;
		m_pRootNode->m_tBox.vCenter.y = World->_42;
		m_pRootNode->m_tBox.vCenter.z = World->_43;
	}

	// m_matNormal = 접선벡터용
	D3DXMatrixInverse(&m_matNormal, 0, &m_pMap->m_matWorld);

	if (View)
	{
		m_pMap->m_matView = *View;
	}
	if (Proj)
	{
		m_pMap->m_matProj = *Proj;
	}
	D3DXMatrixTranspose(&m_pMap->m_matWorld, &m_pMap->m_matWorld);
	D3DXMatrixTranspose(&m_pMap->m_matView, &m_pMap->m_matView);
	D3DXMatrixTranspose(&m_pMap->m_matProj, &m_pMap->m_matProj);

	m_pMap->m_cbData.m_matWorld = m_pMap->m_matWorld;
	m_pMap->m_cbData.m_matView = m_pMap->m_matView;
	m_pMap->m_cbData.m_matProj = m_pMap->m_matProj;
	m_pMap->m_cbData.f_Time = cosf(timeGetTime()*0.001f) * 0.5f + 0.5f;
	m_pMap->m_cbData.f_Y = m_SelectedMap->m_pRootNode->m_tBox.vMax.y - m_SelectedMap->m_pRootNode->m_tBox.vMin.y;
	m_pMap->m_cbData.f_Z = m_SelectedMap->m_pRootNode->m_tBox.vMin.y;

	D3DXMatrixTranspose(&m_pMap->m_matWorld, &m_pMap->m_matWorld);
}

bool BWaterMap::Render()
{
	m_pMap->PreRender(m_pImmediateContext);
	m_pImmediateContext->VSSetShader(m_pWaterVS, NULL, 0);
	m_pImmediateContext->PSSetShader(m_pWaterPS, NULL, 0);
	m_pImmediateContext->PSSetShaderResources(0, 1, m_Water.m_pTextureSRV[0].GetAddressOf());
	m_pImmediateContext->PSSetShaderResources(1, 1, m_WaterBottom.m_pTextureSRV[0].GetAddressOf());
	m_pImmediateContext->PSSetShaderResources(2, 1, m_WaterMapUpside.m_pTextureSRV[0].GetAddressOf());
	m_pImmediateContext->PSSetShaderResources(3, 1, m_WaterNormal.m_pTextureSRV[0].GetAddressOf());
	m_pImmediateContext->PSSetShaderResources(4, 1, m_WaterHeight.m_pTextureSRV[0].GetAddressOf());

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pMap->m_pConstantBuffer);
	m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pMap->m_pConstantBuffer);
	m_pImmediateContext->VSSetConstantBuffers(1, 1, m_pFogCB.GetAddressOf());
	m_pImmediateContext->PSSetConstantBuffers(1, 1, m_pFogCB.GetAddressOf());
	m_pImmediateContext->VSSetConstantBuffers(2, 1, m_pTanLightCB.GetAddressOf());
	m_pImmediateContext->PSSetConstantBuffers(2, 1, m_pTanLightCB.GetAddressOf());
	m_pImmediateContext->VSSetConstantBuffers(3, 1, m_pLightDefaultCB.GetAddressOf());
	m_pImmediateContext->PSSetConstantBuffers(3, 1, m_pLightDefaultCB.GetAddressOf());
	m_pImmediateContext->OMSetBlendState(BDxState::g_pAlphaBlend, 0, -1);
	PostRender(m_pImmediateContext);
	return true;
}

HRESULT BWaterMap::CreateWaterConstantBuffer()
{
	HRESULT hr = S_OK;
	// WaterMap ::포그 & 클립플랜 생성 + 맵 랜더링용 모든 상수버퍼 생성
	m_Fog.g_ClipPlane = TVector4(0.0f, 1.0f, 0.0f, 5.0f);
	m_Fog.g_FogColor = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Fog.g_FogInfo.x = FOGMODE_LINEAR;
	m_Fog.g_FogInfo.y = 0.05f;
	m_Fog.g_FogInfo.z = 10.0f;
	m_Fog.g_FogInfo.w = 1000.0f;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.ByteWidth = sizeof(cbFog);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = (void*)&m_Fog;
	if (FAILED(m_pd3dDevice->CreateBuffer(&cbDesc, &InitData, m_pFogCB.GetAddressOf())))
		return hr; 

	cbDesc.ByteWidth = sizeof(cbLightDefault);
	m_LightDefault.cb_AmbientLightColor = TVector4(0.1f, 0.1f, 0.1f, 1.0f);
	m_LightDefault.cb_DiffuseLightColor = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_LightDefault.cb_SpecularLightColor = TVector4(1.0f, 1.0f, 1.0f, 10.0f);
	// cb_SpecularLightColor.w = SpecularPower이다.
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = (void*)&m_LightDefault;
	if (FAILED(m_pd3dDevice->CreateBuffer(&cbDesc, &InitData, m_pLightDefaultCB.GetAddressOf())))
		return hr;

	cbDesc.ByteWidth = sizeof(cbTanLight);
	ZeroMemory(&m_TanLight, sizeof(cbTanLight));
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = (void*)&m_TanLight;
	if (FAILED(m_pd3dDevice->CreateBuffer(&cbDesc, &InitData, m_pTanLightCB.GetAddressOf())))
		return hr;

	return hr;
}

bool BWaterMap::PostRender(ID3D11DeviceContext* pContext)
{
	ID3D11Buffer* Buffer[2];
	UINT stride[2] = { sizeof(PNCT_Vertex), sizeof(TVector3) };		 // 버텍스 하나 전체의 크기
	UINT offset[2] = { 0, 0 };						 // 버퍼 시작위치
	for (int iNode = 0; iNode < m_DrawPatchNodeList.size(); iNode++)
	{
		BNode* pNode = m_DrawPatchNodeList[iNode];
		GetLodType(pNode);
		Buffer[0] = pNode->m_pVertexBuffer.Get();
		Buffer[1] = pNode->m_pTangentBuffer.Get();
		pContext->IASetVertexBuffers(0, 2, Buffer, stride, offset);
		pContext->IASetIndexBuffer(m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexBufferList[pNode->m_dwLodType].Get(), DXGI_FORMAT_R32_UINT, 0);
		pContext->UpdateSubresource(m_pMap->m_pConstantBuffer, 0, NULL, &m_pMap->m_cbData, 0, 0);
		pContext->DrawIndexed(m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType].size(), 0, 0);
	}
	return true;
}

bool BWaterMap::WaterFrame(D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView)
{
	if (bHeightChange) // 높이가 변경되었고 갱신이 요청되었다면
	{
		m_HeightMap.m_pMiniMapRT->DrawQuadTreeHeightTex(m_pd3dDevice, m_pImmediateContext, m_SelectedMap);
		bHeightChange = false;
	}
	m_Fog.g_vCameraPos.x = m_pCamera->m_vPos.x;
	m_Fog.g_vCameraPos.y = m_pCamera->m_vPos.y;
	m_Fog.g_vCameraPos.z = m_pCamera->m_vPos.z;
	m_Fog.g_vCameraPos.w = 1.0f;
	m_Fog.g_FogColor = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	
	m_pImmediateContext->OMSetBlendState(BDxState::g_pAlphaBlend, 0, 0xffffffff);
	float w = -m_fWaterHeight + cosf(g_fDurationTime) * m_fWaterWave * 0.2f;
	TMatrix matReflectView;
	D3DXMatrixIdentity(&matReflectView);
	TMatrixReflect_Fixed(&matReflectView, &TPlane(0.0f, 1.0f, 0.0f, w));
	TMatrix matView = m_pCamera->m_matView;
	D3DXMatrixMultiply(&matReflectView, &matReflectView, &matView);

	// 상단
	m_pImmediateContext->RSSetState(BDxState::g_pRSBackCullSolid);
	TVector4 m_ClipPlaneFront = TVector4(0.0f, 1.0f, 0.0f, w);
	m_Fog.g_ClipPlane = m_ClipPlaneFront;
	m_pImmediateContext->UpdateSubresource(m_pFogCB.Get(), 0, NULL, &m_Fog, 0, 0);
	DrawWaterRT(0, NULL, &m_pCamera->m_matView, &m_pCamera->m_matProj, OldViewPort, OldDepthStencilView, OldRenderTargetView);
	// 하단
	TVector4 m_ClipPlaneBack = TVector4(0.0f, -1.0f, 0.0f, -w);
	m_Fog.g_ClipPlane = m_ClipPlaneBack;
	m_pImmediateContext->UpdateSubresource(m_pFogCB.Get(), 0, NULL, &m_Fog, 0, 0);
	DrawWaterRT(1, NULL, &m_pCamera->m_matView, &m_pCamera->m_matProj, OldViewPort, OldDepthStencilView, OldRenderTargetView);
	// 반사
	m_pImmediateContext->RSSetState(BDxState::g_pRSFrontCullSolid);
	TVector4 m_ClipPlaneReflect = TVector4(0.0f, 1.0f, 0.0f, w);
	m_Fog.g_ClipPlane = m_ClipPlaneReflect;
	m_pImmediateContext->UpdateSubresource(m_pFogCB.Get(), 0, NULL, &m_Fog, 0, 0);
	DrawWaterRT(2, NULL, &matReflectView, &m_pCamera->m_matProj, OldViewPort, OldDepthStencilView, OldRenderTargetView);

	return false;
}

void BWaterMap::DrawWaterRT(int iMap, TMatrix* pWorld, TMatrix* pView, TMatrix* pProj, D3D11_VIEWPORT OldViewPort, ID3D11DepthStencilView* OldDepthStencilView, ID3D11RenderTargetView* OldRenderTargetView)
{
	ID3D11ShaderResourceView *const pSRV[8] = { NULL, };
	m_pImmediateContext->PSSetShaderResources(0, 8, pSRV);
	m_pImmediateContext->OMSetBlendState(BDxState::g_pAlphaBlend, 0, 0xffffffff);
	m_pImmediateContext->OMSetDepthStencilState(BDxState::g_pDSSDepthEnable, 0x01);

	if (m_MiniMap[iMap].BeginRender(m_pImmediateContext, OldViewPort, OldDepthStencilView, OldRenderTargetView, TVector4(1.0f, 1.0f, 1.0f, 1.0f)))
	{
		m_pWaterReflectMap->m_pMap->SetMatrix(pWorld, pView, pProj);
		m_pWaterReflectMap->Render(m_pImmediateContext);
		m_MiniMap[iMap].AfterRender(m_pImmediateContext);
	}
}

TMatrix* BWaterMap::TMatrixReflect_Fixed(__inout TMatrix *pOut, __in const TPlane *pPlane)
{
	TPlane P;
	D3DXPlaneNormalize(&P, pPlane);

	*pOut = TMatrix
	(
		-2.0f * P.x * P.x + 1.0f, -2.0f * P.y * P.x, -2.0f * P.z * P.x, 0.0f,
		-2.0f * P.x * P.y, -2.0f * P.y * P.y + 1.0f, -2.0f * P.z * P.y, 0.0f,
		-2.0f * P.x * P.z, -2.0f * P.y * P.z, -2.0f * P.z * P.z + 1.0f, 0.0f,
		-2.0f * P.x * P.w, -2.0f * P.y * P.w, -2.0f * P.z * P.w, 1.0f
	);

	return pOut;
}

void BWaterMap::Update(ID3D11Device* pd3dDevice, BCamera* pCamera, BMapTileRender* SelectedMap)
{
	m_pd3dDevice = pd3dDevice;
	m_pCamera = pCamera;
	m_SelectedMap = SelectedMap;
}

BWaterMap::BWaterMap()
{
}

BWaterMap::~BWaterMap()
{
}
