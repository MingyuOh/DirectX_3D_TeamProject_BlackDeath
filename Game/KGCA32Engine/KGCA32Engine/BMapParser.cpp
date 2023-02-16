#include "BMapParser.h"
#include "CharacterParser.h"
#include "K_UIParser.h"
#include "LALoadObjectManager.h"

bool BMapParser::Init()
{
	// 쉐도우 랜더플랜
	m_ShadowPlane.Create(g_pd3dDevice, L"../../data/shader/TestShader.hlsl", DXGI_FORMAT_R32_TYPELESS); // DXGI_FORMAT_R32_TYPELESS
	m_ShadowPlane.Set(g_pd3dDevice, 200, 0, 200, 200);
	LoadShadowShader();
	CreateShadowCB();
	m_vLightPos = TVector3(128, 80, -128);
	m_vLightLookat = TVector3(0.0f, 0.0f, 0.0f);
	m_vLightUp = TVector3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matShadowView, &m_vLightPos, &m_vLightLookat, &m_vLightUp);
	float fMaxViewDistance = sqrt(131072);
	D3DXMatrixOrthoOffCenterLH(&m_matShadowProj, -fMaxViewDistance / 2, fMaxViewDistance / 2, -fMaxViewDistance / 2, fMaxViewDistance / 2, 0.0f, 1000.0f);
	m_matTexture = TMatrix(0.5f, 0.0f, 0.0f, 0.0f
		, 0.0f, -0.5f, 0.0f, 0.0f
		, 0.0f, 0.0f, 1.0f, 0.0f
		, 0.5f, 0.5f, 0.0f, 1.0f);

	/*MFC용*/
	m_VCtrlState = 0;
	m_EnemyPos = 1;
	m_fHeightValue = 1.0f;
	bParallel = false;
	m_fRadius = 3.0f;
	m_fSpreatingRadius = 3.0f;
	m_VSpreatState = vSpreatNone;
	m_Exporter.SetExporter(g_pImmediateContext, L"MapExportFile.txt");
	WeaponParser(L"../../data/Weapon.txt");
	MonsterParser(L"../../data/Monster.txt");
	fullStageCnt = 5;
	TCHAR* ImportMap = L"../../data/SaveExport.txt";
	MapImport(ImportMap);
	
	// 쿼드트리화
	vector<InstancingInfo> TempMapInfo;
	int TempTotalsize = 0;
	for (int iTObj = 0; iTObj < m_MapObjMgr.size(); iTObj++)
	{
		TempTotalsize += m_MapObjMgr[iTObj]->m_InstanceInfo.size();
	}
	TempMapInfo.reserve(TempTotalsize);
	for (int iTObj = 0; iTObj < m_MapObjMgr.size(); iTObj++)
	{
		for (int iInfoObj = 0; iInfoObj < m_MapObjMgr[iTObj]->m_InstanceInfo.size(); iInfoObj++)
		{
			TempMapInfo.push_back(m_MapObjMgr[iTObj]->m_InstanceInfo[iInfoObj]);
		}
	}
	m_vMapQuadTree[0]->AddMapObject(m_vMapQuadTree[0]->m_pRootNode, TempMapInfo);
	m_bItemInited = false;
	MapObjCount = 0;

	// 워터맵 라이트
	m_vInitLightPos = TVector3(300.0f, 300.0f, 0.0f);

	// 이펙트
	BStage1 = false;
	BStage2 = false;
	BStage3 = false;
	BStage4 = false;
	
	ParticleSystem addParticle;
	TVector3 vPos(31.0f, 6.0f, -76.0f);
	addParticle = I_ParticleSystem.GetValue(CarFire);
	addParticle.SetFrameParticleSystemInfo(&vPos);
	m_ParticleSystemList.push_back(addParticle);

	vPos = TVector3(65.0f, 6.0f, -71.0f);
	addParticle.SetFrameParticleSystemInfo(&vPos);
	m_ParticleSystemList.push_back(addParticle);


	//임시
	/*TestLine.resize(m_MapSetting.ItemPosition.size());
	for (int iLine = 0; iLine < TestLine.size(); iLine++)
	{
	TestLine[iLine].Create(g_pd3dDevice, L"../../data/shader/LineShader.hlsl");
	}*/
	bIsMonsterAllDel = false;
	iForRandom = 0;

	// 드럼 바운딩 박스 제작
	vPos = TVector3(-68.0f, 0.0f, -13.0f);
	m_DrumBoundingBox[0].CreateBoundingBox(TVector3(-60.0f, 10.0f, -11.0f) - vPos, TVector3(-77.0f, 0.0f, -16.0f) - vPos, vPos);

	vPos = TVector3(-7.6f, 0.0f, 125.0f);
	m_DrumBoundingBox[1].CreateBoundingBox(TVector3(-5.0f, 10.0f, 133.0f) - vPos, TVector3(-10.0f, 0.0f, 117.0f) - vPos, vPos);

	vPos = TVector3(113.0f, 0.0f, -14.0f);
	m_DrumBoundingBox[2].CreateBoundingBox(TVector3(127.0f, 10.0f, -12.15f) - vPos, TVector3(100.0f, 0.0f, -17.4f) - vPos, vPos);
	HeroDem = 0;

	/*맵 오브젝트 랜더링 최적화 작업*/
	m_RenderSettingASE.m_pd3dDevice = g_pd3dDevice;
	m_RenderSettingASE.LoadShaderAndInputLayout(L"../../data/shader/AseShader.hlsl");

	return true;
}


bool BMapParser::SettingParser(CharacterParser* CharParser, K_UIParser* UIParser)
{
	m_CharParser = CharParser;
	m_UIParser = UIParser;
	return true;
}

bool BMapParser::Frame()
{
	///////////////
	//TMatrix World;
	//TMatrixIdentity(&World);
	//for (int iLine = 0; iLine < TestLine.size(); iLine++)
	//{
	//	TestLine[iLine].SetMatrix(World, g_pMainCamera->m_matView, g_pMainCamera->m_matProj);
	//	TVector3 SetYLine = m_MapSetting.ItemPosition[iLine];
	//	SetYLine.y = 1000.0f;
	//	TestLine[iLine].SetLineStartToEnd(g_pImmediateContext, m_MapSetting.ItemPosition[iLine], SetYLine, TVector4(1, 0, 0, 1));
	//}
	//////////////////

	if (!m_bItemInited)
	{
		RandomWeaponSpawn();
		m_bItemInited = true;
	}

	if (m_UIParser->FarmingOrBattle % 2 == 1 && bIsMonsterAllDel == false)
	{
		// 모든 몬스터 삭제
		LOAD_OBJECT_MGR.DeleateMonsterList();
		bIsMonsterAllDel = true;
	}

	if (m_UIParser->FarmingOrBattle > 0 && m_UIParser->FarmingOrBattle % 2 == 0 && m_MapSetting.EnemyPosition1.size() > LOAD_OBJECT_MGR.GetMonsterCount())
	{
		RandomMonsterSpawn(m_UIParser->FarmingOrBattle / 2);
		bIsMonsterAllDel = false;
	}

	for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
	{
		m_vMapQuadTree[iMap]->Frame(g_pMainCamera);
	}
	for (int iMap = 0; iMap < m_vWaterQuadTree.size(); iMap++)
	{
		m_vWaterQuadTree[iMap]->Frame(g_pMainCamera);
		m_vWaterQuadTree[iMap]->WaterFrame(*g_ViewPort, g_pDepthStencilView, g_pRenderTargetView);
	}

	/*라이트 계산*/
	// 라이트 클래스가 만들어지면 이를 추가시켜야 한다.
	// 라이트 계산 & 상수 데이터 생성
	float t = g_fDurationTime * PI;
	TMatrix mTranslate, mRotation;
	D3DXMatrixTranslation(&mTranslate, m_vInitLightPos.x, m_vInitLightPos.y, m_vInitLightPos.z);
	D3DXMatrixRotationY(&mRotation, t*0.02f);
	D3DXMatrixMultiply(&m_mLightWorld, &mTranslate, &mRotation);
	m_LightPos.x = m_mLightWorld._41;
	m_LightPos.y = m_mLightWorld._42;
	m_LightPos.z = m_mLightWorld._43;
	D3DXVec3Normalize(&m_vDirection, &m_LightPos);
	m_vDirection *= -1.0f;
	if (m_vWaterQuadTree.size())
	{
		m_vWaterQuadTree[0]->m_TanLight.matNormal = m_vWaterQuadTree[0]->m_matNormal;
		m_vWaterQuadTree[0]->m_TanLight.vLightPos = TVector4(m_vDirection.x, m_vDirection.y, m_vDirection.z, 0.0f);
		m_vWaterQuadTree[0]->m_TanLight.vEye = TVector4(g_pMainCamera->m_vPos.x, g_pMainCamera->m_vPos.y, g_pMainCamera->m_vPos.z, 0.0f);
		m_vWaterQuadTree[0]->m_TanLight.vEyeDir = TVector4(g_pMainCamera->m_vLook.x, g_pMainCamera->m_vLook.y, g_pMainCamera->m_vLook.z, 0.0f);
		g_pImmediateContext->UpdateSubresource(m_vWaterQuadTree[0]->m_pTanLightCB.Get(), 0, NULL, &m_vWaterQuadTree[0]->m_TanLight, 0, 0);
	}

	if (m_UIParser->FarmingOrBattle == 1 && BStage1 == true)
	{
		m_MapObjMgr[12]->m_InstanceInfo.clear();
		m_MapObjMgr[12]->m_pInstanceList.clear();
		m_MapObjMgr[12]->m_InstanceInfo.resize(m_StageDrumInfo.size());
		m_MapObjMgr[12]->m_pInstanceList.resize(m_StageDrumMat.size());
		m_MapObjMgr[12]->m_InstanceInfo = m_StageDrumInfo;
		m_MapObjMgr[12]->m_pInstanceList = m_StageDrumMat;
		m_MapObjMgr[12]->m_iInstanceCount = m_StageDrumInfo.size();
		m_StageDrumInfo.clear();
		m_StageDrumMat.clear();
		BStage1 = false;
		BStage2 = false;
		BStage3 = false;
		BStage4 = false;
	}

	// 1스테이지
	if (m_UIParser->FarmingOrBattle == 3 && BStage2 == false)
	{
		ParticleSystem addParticle;
		TVector3 vPos(-68.0f, 3.0f, -13.0f);
		addParticle = I_ParticleSystem.GetValue(StageBoom);
		addParticle.SetFrameParticleSystemInfo(&vPos);
		m_ParticleSystemList.push_back(addParticle);
		BStage2 = true;

		m_StageDrumInfo.resize(m_MapObjMgr[12]->m_InstanceInfo.size());
		m_StageDrumMat.resize(m_MapObjMgr[12]->m_pInstanceList.size());
		m_StageDrumInfo = m_MapObjMgr[12]->m_InstanceInfo;
		m_StageDrumMat = m_MapObjMgr[12]->m_pInstanceList;

		//vector<InstancingInfo>::iterator iter;
		//vector<TMatrix>::iterator iListter;
		//for (int iDrum = 0; iDrum < 10; iDrum++)
		//{
		//	iter = m_MapObjMgr[12]->m_InstanceInfo.begin();
		//	m_MapObjMgr[12]->m_InstanceInfo.erase(iter);
		//	iListter = m_MapObjMgr[12]->m_pInstanceList.begin();
		//	m_MapObjMgr[12]->m_pInstanceList.erase(iListter);
		//	m_MapObjMgr[12]->m_iInstanceCount--;
		//}
		m_MapObjMgr[12]->m_InstanceInfo.erase(m_MapObjMgr[12]->m_InstanceInfo.begin(), m_MapObjMgr[12]->m_InstanceInfo.begin() + 10);
		m_MapObjMgr[12]->m_pInstanceList.erase(m_MapObjMgr[12]->m_pInstanceList.begin(), m_MapObjMgr[12]->m_pInstanceList.begin() + 10);
		m_MapObjMgr[12]->m_iInstanceCount = m_MapObjMgr[12]->m_iInstanceCount - 10;
		BStage1 = true;
	}

	// 2스테이지
	if (m_UIParser->FarmingOrBattle == 5 && BStage3 == false)
	{
		ParticleSystem addParticle;
		TVector3 vPos(-7.81, 3.0f, 122.58f);
		addParticle = I_ParticleSystem.GetValue(StageBoom);
		addParticle.SetFrameParticleSystemInfo(&vPos);
		m_ParticleSystemList.push_back(addParticle);
		BStage3 = true;

		//vector<InstancingInfo>::iterator iter;
		//vector<TMatrix>::iterator iListter;
		//for (int iDrum = 0; iDrum < 10; iDrum++)
		//{
		//	iter = m_MapObjMgr[12]->m_InstanceInfo.begin();
		//	m_MapObjMgr[12]->m_InstanceInfo.erase(iter);
		//	iListter = m_MapObjMgr[12]->m_pInstanceList.begin();
		//	m_MapObjMgr[12]->m_pInstanceList.erase(iListter);
		//	m_MapObjMgr[12]->m_iInstanceCount--;
		//}
		m_MapObjMgr[12]->m_InstanceInfo.erase(m_MapObjMgr[12]->m_InstanceInfo.begin(), m_MapObjMgr[12]->m_InstanceInfo.begin() + 10);
		m_MapObjMgr[12]->m_pInstanceList.erase(m_MapObjMgr[12]->m_pInstanceList.begin(), m_MapObjMgr[12]->m_pInstanceList.begin() + 10);
		m_MapObjMgr[12]->m_iInstanceCount = m_MapObjMgr[12]->m_iInstanceCount - 10;
	}

	// 3스테이지
	if (m_UIParser->FarmingOrBattle == 7 && BStage4 == false)
	{
		ParticleSystem addParticle;
		TVector3 vPos(117.65f, 3.0f, -13.0f);
		addParticle = I_ParticleSystem.GetValue(StageBoom);
		addParticle.SetFrameParticleSystemInfo(&vPos);
		m_ParticleSystemList.push_back(addParticle);

		vPos = TVector3(113.65f, 3.0f, -13.0f);
		addParticle = I_ParticleSystem.GetValue(StageBoom);
		addParticle.SetFrameParticleSystemInfo(&vPos);
		m_ParticleSystemList.push_back(addParticle);
		BStage4 = true;

		m_MapObjMgr[12]->m_InstanceInfo.clear();
		m_MapObjMgr[12]->m_pInstanceList.clear();
		m_MapObjMgr[12]->m_iInstanceCount = 0;
	}


	for (int iParticle = 0; iParticle < m_ParticleSystemList.size(); iParticle++)
	{
		// 파티클시스템에 파티클이 존재하지않으면 삭제
		if (m_ParticleSystemList[iParticle].m_Particle.size() <= 0)
		{
			m_ParticleSystemList.erase(m_ParticleSystemList.begin() + iParticle);
			continue;
		}
		m_ParticleSystemList[iParticle].Frame();
	}

	return true;
}

bool BMapParser::Render()
{
	/*for (int iLine = 0; iLine < TestLine.size(); iLine++)
	{
		TestLine[iLine].m_iPrimitiveType = 2;
		TestLine[iLine].Render(g_pImmediateContext);
	}*/

	// 쉐도우
	ID3D11ShaderResourceView *const pSRVs[16] = { 0, };
	g_pImmediateContext->PSSetShaderResources(0, 16, pSRVs);
	if (m_ShadowPlane.BeginRender(g_pImmediateContext, *g_ViewPort, g_pDepthStencilView, g_pRenderTargetView, TVector4(1.0f, 1.0f, 1.0f, 1.0f)))
	{
		RenderShadow();
		m_ShadowPlane.AfterRender(g_pImmediateContext);
	}
	//m_ShadowPlane.Render(m_pImmediateContext);
	//m_pImmediateContext->RSSetViewports(1, &m_ViewPort);

	/*MFC맵 띄우기*/
	for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
	{
		g_pImmediateContext->PSSetSamplers(1, 1, &BDxState::g_pSSClampLinear);
		g_pImmediateContext->PSSetSamplers(2, 1, &BDxState::g_pSSShadowMap);
		g_pImmediateContext->RSSetState(BDxState::g_pRSBackCullSolid);
		m_matWVPT = m_matShadowView * m_matShadowProj * m_matTexture;
		D3DXMatrixTranspose(&m_cbShadow.g_matShadow, &m_matWVPT);
		m_cbShadow.g_ShadowID = 0; // ID맵쉐도우를 보여주기 위한 변수 ( 지금은 필요 없다 )
		m_cbShadow.g_iNumKernel = 3;
		g_pImmediateContext->UpdateSubresource(m_pShadowCB, 0, NULL, &m_cbShadow, 0, 0);
		g_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pShadowCB);
		g_pImmediateContext->PSSetConstantBuffers(1, 1, &m_pShadowCB);
		g_pImmediateContext->PSSetShaderResources(6, 1, m_ShadowPlane.m_pMiniMapRT->m_pDsvSRV.GetAddressOf());
		for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
		{
			m_vMapQuadTree[iMap]->m_pMap->SetMatrix(&m_vMapQuadTree[iMap]->m_pMap->m_matWorld, &g_pMainCamera->m_matView, &g_pMainCamera->m_matProj);
			m_vMapQuadTree[iMap]->m_pMap->PreRender(g_pImmediateContext);
			// 알파텍스쳐에 입힐 이미지 등록
			if (m_vMapQuadTree[iMap]->m_pSplattingSRV.size())
			{
				for (int iTex = 0; iTex < 4; iTex++)
				{
					g_pImmediateContext->PSSetShaderResources((iTex + 2), 1, m_vMapQuadTree[iMap]->m_pSplattingSRV[iTex].GetAddressOf()); // 텍스쳐 세팅
				}
			}
			// 알파텍스쳐 등록
			if (m_vMapQuadTree[iMap]->SpreatingSRV.Get())
			{
				g_pImmediateContext->PSSetShaderResources(1, 1, m_vMapQuadTree[iMap]->SpreatingSRV.GetAddressOf()); // 텍스쳐 세팅
			}
			m_vMapQuadTree[iMap]->PostRender(g_pImmediateContext);
			//m_vMapQuadTree[iMap]->Render(m_pImmediateContext);
		}
	}
	/*MFC워터맵 띄우기*/
	for (int iWaterMap = 0; iWaterMap < m_vWaterQuadTree.size(); iWaterMap++)
	{
		// 수면의 높이 변화
		float w = -m_vWaterQuadTree[0]->m_fWaterHeight + cosf(g_fDurationTime) * m_vWaterQuadTree[0]->m_fWaterWave * 0.2f;
		TMatrix matWaterWorld;
		D3DXMatrixIdentity(&matWaterWorld);
		matWaterWorld._42 = -w;

		m_vWaterQuadTree[iWaterMap]->m_Fog.g_ClipPlane = TVector4(0.0f, 1.0f, 0.0f, m_vWaterQuadTree[iWaterMap]->m_fWaterHeight);
		m_vWaterQuadTree[iWaterMap]->m_WaterBottom.m_pTextureSRV[0] = m_vWaterQuadTree[iWaterMap]->m_MiniMap[1].m_pMiniMapRT->m_pSRV;
		m_vWaterQuadTree[iWaterMap]->m_WaterMapUpside.m_pTextureSRV[0] = m_vWaterQuadTree[iWaterMap]->m_MiniMap[2].m_pMiniMapRT->m_pSRV;
		g_pImmediateContext->UpdateSubresource(m_vWaterQuadTree[iWaterMap]->m_pFogCB.Get(), 0, NULL, &m_vWaterQuadTree[iWaterMap]->m_Fog, 0, 0);
		m_vWaterQuadTree[iWaterMap]->SetWaterMatrix(&matWaterWorld, &g_pMainCamera->m_matView, &g_pMainCamera->m_matProj);
		m_vWaterQuadTree[iWaterMap]->Render();

		/*확인용 랜더*/
	/*	m_vWaterQuadTree[iWaterMap]->m_MiniMap[0].Render(g_pImmediateContext);
		g_pImmediateContext->RSSetViewports(1, g_ViewPort);
		m_vWaterQuadTree[iWaterMap]->m_MiniMap[1].Render(g_pImmediateContext);
		g_pImmediateContext->RSSetViewports(1, g_ViewPort);
		m_vWaterQuadTree[iWaterMap]->m_MiniMap[2].Render(g_pImmediateContext);
		g_pImmediateContext->RSSetViewports(1, g_ViewPort);*/
	}
	/*맵 오브젝트*/
	if (m_MapObjMgr.size())
	{
		g_pImmediateContext->PSSetSamplers(1, 1, &BDxState::g_pSSClampLinear);
		g_pImmediateContext->PSSetSamplers(2, 1, &BDxState::g_pSSShadowMap);
		g_pImmediateContext->RSSetState(BDxState::g_pRSBackCullSolid);
		m_matWVPT = m_matShadowView * m_matShadowProj * m_matTexture;
		D3DXMatrixTranspose(&m_cbShadow.g_matShadow, &m_matWVPT);
		m_cbShadow.g_ShadowID = 0; // ID맵쉐도우를 보여주기 위한 변수 ( 지금은 필요 없다 )
		m_cbShadow.g_iNumKernel = 3;
		g_pImmediateContext->UpdateSubresource(m_pShadowCB, 0, NULL, &m_cbShadow, 0, 0);
		g_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pShadowCB);
		g_pImmediateContext->PSSetConstantBuffers(1, 1, &m_pShadowCB);
		g_pImmediateContext->PSSetShaderResources(1, 1, m_ShadowPlane.m_pMiniMapRT->m_pDsvSRV.GetAddressOf());
		for (int iMapObj = 0; iMapObj < m_MapObjMgr.size(); iMapObj++)
		{
			if (m_MapObjMgr[iMapObj]->m_InstanceInfo.size())
			{
				m_MapObjMgr[iMapObj]->SetCamera(g_pMainCamera->m_matView, g_pMainCamera->m_matProj);
				m_MapObjMgr[iMapObj]->SettingCameraForNormal(g_pMainCamera->m_vPos, g_pMainCamera->m_vLook);
				g_pImmediateContext->VSSetConstantBuffers(2, 1, &m_MapObjMgr[iMapObj]->m_pNormalCB);
				/*최적화 세팅*/
				g_pImmediateContext->VSSetShader(m_RenderSettingASE.m_pVS, NULL, 0);
				g_pImmediateContext->PSSetShader(m_RenderSettingASE.m_pPS, NULL, 0);
				g_pImmediateContext->IASetInputLayout(m_RenderSettingASE.m_pVertexLayout);
				m_MapObjMgr[iMapObj]->Render(g_pImmediateContext, m_MapObjMgr[iMapObj]->m_InstanceInfo.size());
			}
		}
	}

	for (int iParticle = 0; iParticle < m_ParticleSystemList.size(); iParticle++)
	{
		m_ParticleSystemList[iParticle].Render();
	}

	return true;
}

bool BMapParser::Release()
{
	for (int iRel = 0; iRel < m_vMFCMapInfo.size(); iRel++)
	{
		S_DEL(m_vMFCMapInfo[iRel]);
	}
	m_vMFCMapInfo.clear();
	for (int iRel = 0; iRel < m_vMFCMap.size(); iRel++)
	{
		S_DEL(m_vMFCMap[iRel]);
	}
	m_vMFCMap.clear();
	for (int iRel = 0; iRel < m_vMapQuadTree.size(); iRel++)
	{
		S_DEL(m_vMapQuadTree[iRel]);
	}
	for (int iRel = 0; iRel < m_vMFCWaterMap.size(); iRel++)
	{
		S_DEL(m_vMFCWaterMap[iRel]);
	}
	for (int iRel = 0; iRel < m_vWaterQuadTree.size(); iRel++)
	{
		S_DEL(m_vWaterQuadTree[iRel]);
	}

	m_vMFCWaterMap.clear();
	m_vWaterQuadTree.clear();
	m_vMapQuadTree.clear();
	m_vMapTexture.clear();
	m_viClosedIndex.clear();
	S_DEL(m_MapQuadTree);
	S_DEL(m_MFCMapInfo);
	S_DEL(m_MFCMap);
	S_DEL(m_MFCWaterMapInfo);
	S_DEL(m_WaterMapQuadTree);
	DeleteVector(m_ParticleSystemList);

	return true;
}


bool BMapParser::MapSpreating()
{
	m_Select.SetMatrix(NULL, &g_pMainCamera->m_matView, &g_pMainCamera->m_matProj); //좌표계 일치 + 마우스 레이 받아오기
	m_viClosedIndex.clear();
	for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
	{
		for (int iCheckNode = 0; iCheckNode < m_vMapQuadTree[iMap]->m_DrawPatchNodeList.size(); iCheckNode++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[iCheckNode];
			if (m_Select.IntersectBox(&pNode->m_tBox))
			{
				int iIndex = iCheckNode;
				m_viClosedIndex.push_back(iIndex);
			}
		}
		if (!m_viClosedIndex.size())
		{
			return false;
		}

		/*충돌로 처리된 노드들을 거리에 따라 순서대로 정렬*/
		vector<int> m_iOrgiClosedIndex;
		while (m_viClosedIndex.size())
		{
			float PLength = 99999.9f;
			int iApplyIndex = 0;
			for (int iCheckNode = 0; iCheckNode < m_viClosedIndex.size(); iCheckNode++)
			{
				TVector3 Pvec = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_viClosedIndex[iCheckNode]]->m_tBox.vCenter - m_Select.m_Ray.vOrigin;
				float BLength = D3DXVec3Length(&Pvec);
				if (PLength > BLength)
				{
					iApplyIndex = iCheckNode;
				}
			}
			m_iOrgiClosedIndex.push_back(m_viClosedIndex[iApplyIndex]);
			vector<int>::iterator itor = m_viClosedIndex.begin();
			itor += iApplyIndex;
			m_viClosedIndex.erase(itor);
		}

		// m_iOrgiClosedIndex = 가까운 순서대로 정렬된 충돌된 노드
		for (int iApply = 0; iApply < m_iOrgiClosedIndex.size(); iApply++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_iOrgiClosedIndex[iApply]];
			m_vMapQuadTree[iMap]->GetLodType(pNode);
			TVector3 v0, v1, v2;
			int indexsize = m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType].size() / 3;
			for (int iFace = 0; iFace < indexsize; iFace++)
			{
				v0 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 0]].Position;
				v1 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 1]].Position;
				v2 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 2]].Position;
				if (m_Select.CheckPick(v0, v1, v2)) // 충돌지점이 판별 됐다면
				{
					float RootNodeSize = m_vMapQuadTree[iMap]->m_pRootNode->m_tBox.vMax.x - m_vMapQuadTree[iMap]->m_pRootNode->m_tBox.vMin.x;
					TVector3 Crash = m_Select.CrashPoint;
					Crash.z *= -1.0f;
					Crash.x = Crash.x + (RootNodeSize / 2);
					Crash.z = Crash.z + (RootNodeSize / 2);

					SpreatingBox.vMin.x = Crash.x - m_fSpreatingRadius;
					SpreatingBox.vMin.z = Crash.z - m_fSpreatingRadius;
					SpreatingBox.vMax.x = Crash.x + m_fSpreatingRadius;
					SpreatingBox.vMax.z = Crash.z + m_fSpreatingRadius;

					Crash.x = Crash.x / RootNodeSize;
					Crash.z = Crash.z / RootNodeSize;

					SpreatingBox.vMin.x = SpreatingBox.vMin.x / RootNodeSize;
					SpreatingBox.vMin.z = SpreatingBox.vMin.z / RootNodeSize;
					SpreatingBox.vMax.x = SpreatingBox.vMax.x / RootNodeSize;
					SpreatingBox.vMax.z = SpreatingBox.vMax.z / RootNodeSize;

					// uv값까지 생성완료
					m_vMapQuadTree[iMap]->Spreating(g_pImmediateContext, SpreatingBox, Crash, m_fSpreatingRadius, m_iSpreatingColor);
				}
			}
		}
	}
	return false;
}

bool BMapParser::MapHeightControl()
{
	//pNode->m_VertexList;
	// 이게 버텍스
	//m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType];
	// 이게 인덱스

	// 버텍스는 m_DrawNodeList가 들고 있고
	// 인덱스는 m_pdwLodIndexArray가 들고 있는데 몇번째 인덱스형태를 쓰는지 알아야 한다.
	// 몇번째 인덱스 레벨의 몇번째 타입을 쓰고있는지 알아야 한다.
	// 인덱스버퍼와 버텍스버퍼가 존재한다.
	m_Select.SetMatrix(NULL, &g_pMainCamera->m_matView, &g_pMainCamera->m_matProj); //좌표계 일치가 필요하다.
	TVector2 ControlBox;
	m_viClosedIndex.clear();
	for (int iMap = 0; iMap < m_vMapQuadTree.size(); iMap++)
	{
		for (int iCheckNode = 0; iCheckNode < m_vMapQuadTree[iMap]->m_DrawPatchNodeList.size(); iCheckNode++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[iCheckNode];
			if (m_Select.IntersectBox(&pNode->m_tBox))
			{
				int iIndex = iCheckNode;
				m_viClosedIndex.push_back(iIndex);
			}
		}
		if (!m_viClosedIndex.size())
		{
			return false;
		}

		/*충돌로 처리된 노드들을 거리에 따라 순서대로 정렬*/
		vector<int> m_iOrgiClosedIndex;
		while (m_viClosedIndex.size())
		{
			float PLength = 99999.9f;
			int iApplyIndex = 0;
			for (int iCheckNode = 0; iCheckNode < m_viClosedIndex.size(); iCheckNode++)
			{
				TVector3 Pvec = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_viClosedIndex[iCheckNode]]->m_tBox.vCenter - m_Select.m_Ray.vOrigin;
				float BLength = D3DXVec3Length(&Pvec);
				if (PLength > BLength)
				{
					iApplyIndex = iCheckNode;
				}
			}
			m_iOrgiClosedIndex.push_back(m_viClosedIndex[iApplyIndex]);
			vector<int>::iterator itor = m_viClosedIndex.begin();
			itor += iApplyIndex;
			m_viClosedIndex.erase(itor);
		}

		for (int iApply = 0; iApply < m_iOrgiClosedIndex.size(); iApply++)
		{
			BNode* pNode = m_vMapQuadTree[iMap]->m_DrawPatchNodeList[m_iOrgiClosedIndex[iApply]];
			m_vMapQuadTree[iMap]->GetLodType(pNode);
			TVector3 v0, v1, v2;
			int indexsize = m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType].size() / 3;
			for (int iFace = 0; iFace < indexsize; iFace++)
			{
				v0 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 0]].Position;
				v1 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 1]].Position;
				v2 = pNode->m_VertexList[m_vMapQuadTree[iMap]->m_pdwLodIndexArray[pNode->m_dwLodLevel]->IndexList[pNode->m_dwLodType][iFace * 3 + 2]].Position;
				if (m_Select.CheckPick(v0, v1, v2))
				{
					if (m_VCtrlState == vUp || m_VCtrlState == vDown) // up Down일 경우
					{
						ControlBox = ChangeYPosition(m_Select.CrashPoint, &pNode->m_VertexList);
						if (pNode->m_tBox.vMax.y < ControlBox.x)
						{
							pNode->m_tBox.vMax.y = ControlBox.x;
						}
						if (pNode->m_tBox.vMin.y > ControlBox.y)
						{
							pNode->m_tBox.vMin.y = ControlBox.y;
						}
						ControlBox.x = 0;
						ControlBox.y = 0;
						pNode->m_pVertexBuffer.Attach(m_vMapQuadTree[iMap]->CreateVertexBuffer(g_pd3dDevice, &pNode->m_VertexList.at(0), pNode->m_VertexList.size(), sizeof(PNCT_Vertex)));
						for (int iNeighbor = 0; iNeighbor < pNode->m_NeighborList.size(); iNeighbor++)
						{
							if (pNode->m_NeighborList[iNeighbor] != NULL)
							{
								ControlBox = ChangeYPosition(m_Select.CrashPoint, &pNode->m_NeighborList[iNeighbor]->m_VertexList);
								if (pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y < ControlBox.x)
								{
									pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y = ControlBox.x;
								}
								if (pNode->m_NeighborList[iNeighbor]->m_tBox.vMin.y > ControlBox.y)
								{
									pNode->m_NeighborList[iNeighbor]->m_tBox.vMin.y = ControlBox.y;
								}
								ControlBox.x = 0;
								ControlBox.y = 0;
								pNode->m_NeighborList[iNeighbor]->m_pVertexBuffer.Attach(m_vMapQuadTree[iMap]->
									CreateVertexBuffer(g_pd3dDevice, &pNode->m_NeighborList[iNeighbor]->
										m_VertexList.at(0), pNode->m_NeighborList[iNeighbor]->m_VertexList.size(), sizeof(PNCT_Vertex)));
							}
						}
						return true;
					}
					else // Parallel일 경우
					{
						ControlBox = ChangeYParallel(m_Select.CrashPoint, &pNode->m_VertexList);
						if (pNode->m_tBox.vMax.y < ControlBox.x)
						{
							pNode->m_tBox.vMax.y = ControlBox.x;
						}
						if (pNode->m_tBox.vMin.y > ControlBox.y)
						{
							pNode->m_tBox.vMin.y = ControlBox.y;
						}
						ControlBox.x = 0;
						ControlBox.y = 0;
						pNode->m_pVertexBuffer.Attach(m_vMapQuadTree[iMap]->CreateVertexBuffer(g_pd3dDevice, &pNode->m_VertexList.at(0), pNode->m_VertexList.size(), sizeof(PNCT_Vertex)));
						for (int iNeighbor = 0; iNeighbor < pNode->m_NeighborList.size(); iNeighbor++)
						{
							if (pNode->m_NeighborList[iNeighbor] != NULL)
							{
								ControlBox = ChangeYParallel(m_Select.CrashPoint, &pNode->m_NeighborList[iNeighbor]->m_VertexList);
								if (pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y < ControlBox.x)
								{
									pNode->m_NeighborList[iNeighbor]->m_tBox.vMax.y = ControlBox.x;
								}
								if (pNode->m_NeighborList[iNeighbor]->m_tBox.vMin.y > ControlBox.y)
								{
									pNode->m_NeighborList[iNeighbor]->m_tBox.vMin.y = ControlBox.y;
								}
								ControlBox.x = 0;
								ControlBox.y = 0;
								pNode->m_NeighborList[iNeighbor]->m_pVertexBuffer.Attach(m_vMapQuadTree[iMap]->
									CreateVertexBuffer(g_pd3dDevice, &pNode->m_NeighborList[iNeighbor]->
										m_VertexList.at(0), pNode->m_NeighborList[iNeighbor]->m_VertexList.size(), sizeof(PNCT_Vertex)));
							}
						}
						bParallel = true;
						return true;
					}
				}
			}
		}
	}
	return false;
}


TVector2 BMapParser::ChangeYPosition(TVector3 CPos, vector<PNCT_Vertex>* VertexList)
{
	TVector2 ControlBox;
	ControlBox.x = -99999.9f; // MAX
	ControlBox.y = 99999.9f; // MIN
	float HeightControl = 0.0f;
	if (m_VCtrlState == vUp)
	{
		HeightControl = 1.0f * g_fSecPerFrame * m_fHeightValue;
	}
	else //(m_VCtrlState == vDown)
	{
		HeightControl = -1.0f * g_fSecPerFrame * m_fHeightValue;
	}
	for (int iVer = 0; iVer < VertexList->size(); iVer++)
	{
		TVector3 Va = CPos;
		TVector3 Vb = VertexList[0][iVer].Position;
		Va.y = 0.0f;
		Vb.y = 0.0f;
		TVector3 LengthTemp = Va - Vb;
		float length = D3DXVec3Length(&LengthTemp);
		length = length / m_fRadius;
		if (length < 1.0f)
		{
			length *= 90;
			double pi = 3.141592;
			length = pi * length / 180.0;
			length = cosf(length);
			length *= HeightControl; // 높이 변화율 추가요망

			VertexList[0][iVer].Position.y += length;

			if (ControlBox.x < VertexList[0][iVer].Position.y)
			{
				ControlBox.x = VertexList[0][iVer].Position.y;
			}
			if (ControlBox.y > VertexList[0][iVer].Position.y)
			{
				ControlBox.y = VertexList[0][iVer].Position.y;
			}
		}
	}
	return ControlBox;
}

TVector2 BMapParser::ChangeYParallel(TVector3 CPos, vector<PNCT_Vertex>* VertexList)
{
	TVector2 ControlBox;
	ControlBox.x = -99999.9f; // MAX
	ControlBox.y = 99999.9f; // MIN
	float HeightControl = 0.0f;
	if (m_VCtrlState == vUp)
	{
		HeightControl = 1.0f;
	}
	else //(m_VCtrlState == vDown)
	{
		HeightControl = -1.0f;
	}
	for (int iVer = 0; iVer < VertexList->size(); iVer++)
	{
		TVector3 Va = CPos;
		TVector3 Vb = VertexList[0][iVer].Position;
		Va.y = 0.0f;
		Vb.y = 0.0f;
		TVector3 LengthTemp = Va - Vb;
		float length = D3DXVec3Length(&LengthTemp);
		length = length / m_fRadius;
		if (length < 1.0f)
		{
			if (bParallel) //이전과 같은값을 적용
			{
				VertexList[0][iVer].Position.y = fParallel;
				if (ControlBox.x < VertexList[0][iVer].Position.y)
				{
					ControlBox.x = VertexList[0][iVer].Position.y;
				}
				if (ControlBox.y > VertexList[0][iVer].Position.y)
				{
					ControlBox.y = VertexList[0][iVer].Position.y;
				}
			}
			else
			{
				VertexList[0][iVer].Position.y = CPos.y;
				fParallel = CPos.y;
				if (ControlBox.x < VertexList[0][iVer].Position.y)
				{
					ControlBox.x = VertexList[0][iVer].Position.y;
				}
				if (ControlBox.y > VertexList[0][iVer].Position.y)
				{
					ControlBox.y = VertexList[0][iVer].Position.y;
				}
			}
		}
	}
	return ControlBox;
}

bool BMapParser::CreateMap(int iCellCount, int iPatchCount, float fCellDistance, TCHAR** MapTexture, int TextureCount, TCHAR* HeightMapTex)
{
	m_MFCMapInfo = new BMapDesc;
	m_MFCMapInfo->iCellCount = iCellCount;
	m_MFCMapInfo->iPatchCount = iPatchCount;
	m_MFCMapInfo->fCellDistance = fCellDistance;
	m_vMFCMapInfo.push_back(m_MFCMapInfo);

	m_MFCMap = new BMap;
	m_MFCMap->SetMapDesc(*m_MFCMapInfo);
	m_MFCMap->Create(g_pd3dDevice, g_pImmediateContext, MapTexture, TextureCount, HeightMapTex, L"../../data/shader/NormalShader.hlsl");
	m_vMapTexture.push_back(MapTexture[0]);
	m_MFCMap->UpdateNormal();
	m_vMFCMap.push_back(m_MFCMap);

	CreateQuadTree(1, 10.0f, m_MFCMap); // 여기서 최대 깊이설정 & 최소 분할사이즈를 조정 가능 == MFC에서 넘겨주도록 해야한다.
	return true;
}

bool BMapParser::CreateQuadTree(int iMaxDepthLimit, float fMinDivideSize, BMap* Map)
{
	m_MapQuadTree = new BMapTileRender;
	m_MapQuadTree->m_iMaxDepthLimit = iMaxDepthLimit;
	m_MapQuadTree->m_fMinDivideSize = fMinDivideSize;
	m_MapQuadTree->Update(g_pd3dDevice, g_pMainCamera);
	m_MapQuadTree->Build(Map, Map->m_iNumCols, Map->m_iNumRows, g_pImmediateContext);
	m_MapQuadTree->CreateSpreatingTexture();
	m_vMapQuadTree.push_back(m_MapQuadTree);
	return true;
}

bool BMapParser::MapImport(TCHAR* ImportFile)
{
	if (!m_Importer.OpenStream(ImportFile, g_pd3dDevice, g_pImmediateContext, g_pMainCamera))
	{
		return false;
	}
	m_Importer.LoadMapObj(&m_MapObjMgr);
	m_Importer.LoadMap(&m_vMFCMap, &m_vMFCMapInfo, &m_vMapQuadTree, &m_vMapTexture, &m_mMapSplattingTexture, &m_vWaterQuadTree);

	m_MapSetting.SettingMap(m_vMapQuadTree[0]);
	m_MapSetting.MapInfoSetting();
	m_MapSetting.SettingSoundInfo();
	m_MapSetting.SettingObjInfo();

	//워터맵용 임시코드
	m_bSelectedMap = m_vMapQuadTree[0];
	m_MFCWaterMapInfo = new BMapDesc;
	m_MFCWaterMapInfo->iCellCount = m_vMFCMapInfo[0]->iCellCount;
	m_MFCWaterMapInfo->iPatchCount = m_vMFCMapInfo[0]->iPatchCount;
	m_MFCWaterMapInfo->fCellDistance = m_vMFCMapInfo[0]->fCellDistance;

	m_MFCWaterMap = new BNMap;
	m_MFCWaterMap->SetMapDesc(*m_MFCWaterMapInfo);
	m_MFCWaterMap->Create(g_pd3dDevice, g_pImmediateContext, &m_mMapSplattingTexture[0], 1, NULL, L"../../data/shader/MapShader.hlsl");
	m_MFCWaterMap->UpdateNormal();
	m_vMFCWaterMap.push_back(m_MFCWaterMap);

	m_vWaterQuadTree[0]->m_iMaxDepthLimit = m_vMapQuadTree[m_vWaterQuadTree[0]->m_iSelectedMap]->m_iMaxDepthLimit;
	m_vWaterQuadTree[0]->m_fMinDivideSize = m_vWaterQuadTree[0]->m_fWaterWave;
	m_vWaterQuadTree[0]->Update(g_pd3dDevice, g_pMainCamera, m_vMapQuadTree[m_vWaterQuadTree[0]->m_iSelectedMap]);
	m_vWaterQuadTree[0]->Build(m_vMFCWaterMap[m_vWaterQuadTree[0]->m_iSelectedMap], m_vMFCWaterMap[m_vWaterQuadTree[0]->m_iSelectedMap]->m_iNumCols,
		m_vMFCWaterMap[m_vWaterQuadTree[0]->m_iSelectedMap]->m_iNumRows, g_pImmediateContext, m_vWaterQuadTree[0]->m_strWaterTex, m_vWaterQuadTree[0]->m_strWaterNormalTex);
	m_vWaterQuadTree[0]->m_pWaterReflectMap = m_vMapQuadTree[m_vMFCWaterMap.size() - 1];
	for (int imap = 0; imap < 3; imap++)
	{
		if (!m_vWaterQuadTree[0]->m_MiniMap[imap].Create(g_pd3dDevice, L"../../data/shader/TestShader.hlsl"))
		{
			return false;
		}
		m_vWaterQuadTree[0]->m_MiniMap[imap].Set(g_pd3dDevice, 0, imap * 200, 200, 200);
	}

	m_vMapQuadTree[0]->m_DrawObjList;

	return true;
}

bool BMapParser::MapExport()
{
	// 이하 코드는 1개의 맵만을 export하는 코드입니다.
	// 다수의 맵을 export하기 위해서는 변경이 필요합니다.
	m_Exporter.SetExporter(g_pImmediateContext, L"../../data/SaveExport.txt");
	m_Exporter.ExpAlphaMapTex(m_vMapQuadTree[0]->SpreatingTexTure);
	if (m_vMapTexture.size())
	{
		m_vMapTexture.resize(1);
	}
	m_Exporter.ExpTexName(m_vMapTexture[0], m_mMapSplattingTexture);
	m_Exporter.ExpMapInfo(m_vMFCMapInfo[0]);
	m_Exporter.ExpHeightTotal(m_vMapQuadTree[0]->m_pRootNode);
	if (!m_Exporter.m_pStream) return false;
	fclose(m_Exporter.m_pStream);
	return true;
}

vector<TVector3> BMapParser::GetEnemyPosition()
{
	vector<TVector3> NullPos;
	switch (m_EnemyPos)
	{
	case 1:
		return m_MapSetting.EnemyPosition1;
		break;
	case 2:
		return m_MapSetting.EnemyPosition2;
		break;
	case 3:
		return m_MapSetting.EnemyPosition3;
		break;
	case 4:
		return m_MapSetting.EnemyPosition4;
		break;
	default:
		return NullPos;
		break;
	}
}

TVector4 BMapParser::GetHeight(TVector3 HeroPos, int* HeroSound, bool* MoveAble)
{
	/*================== 디버그 임시 한계치 설정 =======================*/
	if (fabs(HeroPos.x) >= 127.0f)
	{
		HeroPos.x = 127.0f;
	}

	if (fabs(HeroPos.z) >= 127.0f)
	{
		HeroPos.z = 127.0f;
	}
	/*================================================================*/


	int iXindex;
	int iZindex;
	float fXDetailPoint;
	float fZDetailPoint;
	float fCellDistance = m_MapSetting.m_SelectedMap->m_pMap->m_fCellDistance;
	int iChangePoint = m_MapSetting.m_SelectedMap->m_pRootNode->m_tBox.fExtent[0];
	int HeroAddress;
	m_EnemyPos = 1;

	iXindex = (HeroPos.x + iChangePoint) / fCellDistance;
	iZindex = ((HeroPos.z - iChangePoint) * -1.0f) / fCellDistance;
	fXDetailPoint = HeroPos.x - (int)HeroPos.x;
	fZDetailPoint = HeroPos.z - (int)HeroPos.z;

	if (fXDetailPoint > 0 && fZDetailPoint < 0) // 2사분면
	{
		fZDetailPoint = 1 + fZDetailPoint;
		m_EnemyPos = 2;
	}

	if (fXDetailPoint < 0 && HeroPos.z < 0) // 3사분면
	{
		fXDetailPoint = 1 + fXDetailPoint;
		fZDetailPoint = 1 + fZDetailPoint;
		m_EnemyPos = 3;
	}

	if (fXDetailPoint < 0 && HeroPos.z > 0) // 4사분면
	{
		fXDetailPoint = 1 + fXDetailPoint;
		m_EnemyPos = 4;
	}

	HeroAddress = m_MapSetting.m_MapAddress[iZindex][iXindex];

	float fResultHeight = 0.0f;
	float Left = m_MapSetting.m_TileInfo[HeroAddress].TileEdge[0].y - m_MapSetting.m_TileInfo[HeroAddress].TileEdge[2].y;
	float ResultLeft = Left*fZDetailPoint + m_MapSetting.m_TileInfo[HeroAddress].TileEdge[2].y;
	float Right = m_MapSetting.m_TileInfo[HeroAddress].TileEdge[1].y - m_MapSetting.m_TileInfo[HeroAddress].TileEdge[3].y;
	float ResultRight = Right*fZDetailPoint + m_MapSetting.m_TileInfo[HeroAddress].TileEdge[3].y;
	float CenterResult = ResultLeft + (ResultRight - ResultLeft) * fXDetailPoint;

	TVector3 TempNormal;
	if (fXDetailPoint + fZDetailPoint < 1.0f)
		TempNormal = m_MapSetting.m_TileInfo[HeroAddress].TileNormal[0];
	else
		TempNormal = m_MapSetting.m_TileInfo[HeroAddress].TileNormal[1];

	TVector4 Result;
	Result.x = TempNormal.x;
	Result.y = TempNormal.y;
	Result.z = TempNormal.z;
	Result.w = CenterResult;

	if (HeroSound)
	{
		*HeroSound = m_MapSetting.m_TileInfo[HeroAddress].TileSound;
	}
	if (MoveAble)
	{
		*MoveAble = m_MapSetting.m_TileInfo[HeroAddress].TileMoveUnable;
	}
	return Result;
}

bool BMapParser::CreateWaterMap(BMapTileRender* SelectedMap, int iMaxDepthLimit, float fMinDivideSize, int iSelectedMap, TCHAR* WaterTex, TCHAR* WaterNormalTex, float fHeight, float fWave)
{
	if (m_vMFCWaterMap.size())
	{
		m_WaterMapQuadTree = new BWaterMap;
		m_WaterMapQuadTree->m_fWaterHeight = fHeight;
		m_WaterMapQuadTree->m_fWaterWave = fWave;
		m_WaterMapQuadTree->m_iMaxDepthLimit = iMaxDepthLimit;
		m_WaterMapQuadTree->m_fMinDivideSize = fMinDivideSize;
		m_WaterMapQuadTree->Update(g_pd3dDevice, g_pMainCamera, SelectedMap);
		m_WaterMapQuadTree->Build(m_vMFCWaterMap[iSelectedMap], m_vMFCWaterMap[iSelectedMap]->m_iNumCols,
			m_vMFCWaterMap[iSelectedMap]->m_iNumRows, g_pImmediateContext, WaterTex, WaterNormalTex);
		m_WaterMapQuadTree->m_pWaterReflectMap = m_vMapQuadTree[m_vMFCWaterMap.size() - 1];
		for (int imap = 0; imap < 3; imap++)
		{
			if (!m_WaterMapQuadTree->m_MiniMap[imap].Create(g_pd3dDevice, L"../../data/shader/TestShader.hlsl"))
			{
				return false;
			}
			m_WaterMapQuadTree->m_MiniMap[imap].Set(g_pd3dDevice, 0, imap * 200, 200, 200);
		}
		m_vWaterQuadTree.push_back(m_WaterMapQuadTree);
	}
	return false;
}

void BMapParser::WeaponParser(TCHAR* WeaponText)
{
	m_pStream = _tfopen(WeaponText, _T("rt"));
	if (m_pStream == NULL)
	{
		if (!m_pStream) return;
		fclose(m_pStream);
		MessageBox(0, _T("파일을 찾지 못했습니다."), _T("Fatal Error"), MB_OK);
	}
	_fgetts(m_pBuffer, 256, m_pStream);
	while (m_pBuffer != NULL)
	{
		NewWeapon = new SpawnWeaponInfo;
		_stscanf(m_pBuffer, _T("%d %s %d %d %d %s %d %d %d %s %d %d %d"), &NewWeapon->WeaponIndex,
			NewWeapon->WeaponName, &NewWeapon->MinBullet[0], &NewWeapon->MaxBullet[0], &NewWeapon->GetChance[0],
			NewWeapon->WeaponName, &NewWeapon->MinBullet[1], &NewWeapon->MaxBullet[1], &NewWeapon->GetChance[1],
			NewWeapon->WeaponName, &NewWeapon->MinBullet[2], &NewWeapon->MaxBullet[2], &NewWeapon->GetChance[2]);
		m_WeaponInfo.push_back(NewWeapon);
		_fgetts(m_pBuffer, 256, m_pStream);
		if (feof(m_pStream) != 0)
		{
			NewWeapon = new SpawnWeaponInfo;
			_stscanf(m_pBuffer, _T("%d %s %d %d %d %s %d %d %d %s %d %d %d"), &NewWeapon->WeaponIndex,
				NewWeapon->WeaponName, &NewWeapon->MinBullet[0], &NewWeapon->MaxBullet[0], &NewWeapon->GetChance[0],
				NewWeapon->WeaponName, &NewWeapon->MinBullet[1], &NewWeapon->MaxBullet[1], &NewWeapon->GetChance[1],
				NewWeapon->WeaponName, &NewWeapon->MinBullet[2], &NewWeapon->MaxBullet[2], &NewWeapon->GetChance[2]);
			m_WeaponInfo.push_back(NewWeapon);
			return;
		}
	}
	return;
}

void BMapParser::MonsterParser(TCHAR* MonsterText)
{
	m_pStream = _tfopen(MonsterText, _T("rt"));
	if (m_pStream == NULL)
	{
		if (!m_pStream) return;
		fclose(m_pStream);
		MessageBox(0, _T("파일을 찾지 못했습니다."), _T("Fatal Error"), MB_OK);
	}
	TCHAR Temp[255];
	int iTemp = 0;
	int MonsterCnt = 0;
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s %d"), Temp, &MonsterCnt);

	for (int iMon = 0; iMon < MonsterCnt; iMon++)
	{
		int TempCnt = 0;
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%d %s"), &TempCnt, m_TempMonsterName[iMon]);
		m_MonsterInfo[m_TempMonsterName[iMon]] = TempCnt;
	}

	_fgetts(m_pBuffer, 256, m_pStream);
	int iStage = 0;
	while (feof(m_pStream) == 0)
	{
		m_StageForMonster.resize(5);
		m_StageForMonster[iStage] = new StageForMonsterInfo;	
		for (int iMonster = 0; iMonster < 3; iMonster++)
		{
			NewMonsterInfo = new MonsterInfo;
			_stscanf(m_pBuffer, _T("%d %s %d %d %d"),
				&iTemp, Temp, &NewMonsterInfo->iStage, &NewMonsterInfo->iRespawnMinCount, &NewMonsterInfo->iRespawnMaxCount);
			TCHAR strMonsterName[255];
			int iMonsterChance = 0;
			for (int iStageMonster = 0; iStageMonster < 3; iStageMonster++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s %d"), strMonsterName, &iMonsterChance);
				NewMonsterInfo->iMonsterIndex[iStageMonster] = m_MonsterInfo[strMonsterName];
				NewMonsterInfo->iRespawnChance[iStageMonster] = iMonsterChance;
			}
			m_StageForMonster[iStage]->MonsterChance[iMonster] = NewMonsterInfo;
			_fgetts(m_pBuffer, 256, m_pStream);
		}
		iStage++;
	}
	int k = 0;
	return;
}

void BMapParser::RandomWeaponSpawn()
{
	int iRandomValue = g_fDurationTime;
	for (int iSection = 0; iSection < m_MapSetting.ItemPosition.size(); iSection++)
	{
		srand(iRandomValue);
		iRandomValue++;

		int iWpOrDef = (rand() % 10);
		if (iWpOrDef < 9) // 무기
		{
			int iRandomiTem = (rand() % 100); // 아이템의 종류 결정

			 /*임시*/
			if (iRandomiTem < 10)
			{
				iRandomiTem = 5;
			}
			else if (iRandomiTem > 10 && iRandomiTem < 80)
			{
				iRandomiTem = iRandomiTem % 3;
				iRandomiTem += 6;
			}
			else
			{
				iRandomiTem = iRandomiTem % 2;
				iRandomiTem += 9;
			}
			int iChance = rand() % 10000 + 1;
			int iBullet = 0;
			for (int iRandomChance = 0; iRandomChance < 3; iRandomChance++)
			{
				if (iChance - m_WeaponInfo[iRandomiTem - 5]->GetChance[iRandomChance] > 0)
				{
					iChance = iChance - m_WeaponInfo[iRandomiTem - 5]->GetChance[iRandomChance];
				}
				else
				{
					iBullet = iRandomChance;
					break;
				}
			}
			int AddBulletChance = m_WeaponInfo[iRandomiTem - 5]->MaxBullet[iBullet] - m_WeaponInfo[iRandomiTem - 5]->MinBullet[iBullet];
			int AddBullet = rand() % (AddBulletChance + 1);
			int OutputBullet = m_WeaponInfo[iRandomiTem - 5]->MinBullet[iBullet] + AddBullet;
			LOAD_OBJECT_MGR.CreateWeapon(m_MapSetting.ItemPosition[iSection], iRandomiTem, OutputBullet);
			//m_CharParser->웨폰크리에이트 함수 ( 아이템 번호 = iRandomiTem / 총알 수 = OutputBullet / 포지션 = m_MapSetting.m_TileInfo[ItemSection[iSection]].TileCenter  )
		}
		else // 방어구
		{
			int iRandomiTem = (rand() % 5); // 아이템의 종류 결정
			LOAD_OBJECT_MGR.CreateArmor(m_MapSetting.ItemPosition[iSection], iRandomiTem);
		}
	}
}

// 본 함수는 리스폰 가능지역을 세팅하는 함수이다.
void BMapParser::MonsterRespawnPostionSetting(TVector3 HeroPos)
{
	MonsterRespawnable.clear();
	// 몬스터 스폰 에어리어는 총 4군데로 나뉘어진다.
	// 1  2
	// 3  4
	int CheckArea = 0;
	if (HeroPos.x < 0 && HeroPos.z > 0)
		CheckArea = 1;
	if (HeroPos.x > 0 && HeroPos.z > 0)
		CheckArea = 2;
	if (HeroPos.x > 0 && HeroPos.z < 0)
		CheckArea = 3;
	if (HeroPos.x < 0 && HeroPos.z < 0)
		CheckArea = 4;

	if (CheckArea == 1)
	{
		for (int iCheckZ = 0; iCheckZ < 128; iCheckZ++)
		{
			for (int iCheckX = 0; iCheckX < 128; iCheckX++)
			{
				if (MonsterRespawnable.size() > 8)
					break;
				if (m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[iCheckZ][iCheckX]].TileMonsterRespawn == true)
					MonsterRespawnable.push_back(m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[iCheckZ][iCheckX]].TileCenter);
			}
		}
	}
	if (CheckArea == 2)
	{
		for (int iCheckZ = 0; iCheckZ < 128; iCheckZ++)
		{
			for (int iCheckX = 128; iCheckX < 256; iCheckX++)
			{
				if (MonsterRespawnable.size() > 8)
					break;
				if (m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[iCheckZ][iCheckX]].TileMonsterRespawn == true)
					MonsterRespawnable.push_back(m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[iCheckZ][iCheckX]].TileCenter);
			}
		}
	}
	if (CheckArea == 3)
	{
		for (int iCheckZ = 128; iCheckZ < 256; iCheckZ++)
		{
			for (int iCheckX = 0; iCheckX < 128; iCheckX++)
			{
				if (MonsterRespawnable.size() > 8)
					break;
				if (m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[iCheckZ][iCheckX]].TileMonsterRespawn == true)
					MonsterRespawnable.push_back(m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[iCheckZ][iCheckX]].TileCenter);
			}
		}
	}
	if (CheckArea == 4)
	{
		for (int iCheckZ = 128; iCheckZ < 256; iCheckZ++)
		{
			for (int iCheckX = 128; iCheckX < 256; iCheckX++)
			{
				if (MonsterRespawnable.size() > 8)
					break;
				if (m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[iCheckZ][iCheckX]].TileMonsterRespawn == true)
					MonsterRespawnable.push_back(m_MapSetting.m_TileInfo[m_MapSetting.m_MapAddress[iCheckZ][iCheckX]].TileCenter);
			}
		}
	}
}

// 배틀페이즈마다 호출되어야 하는 함수
void BMapParser::RandomMonsterSpawn(int StageNum)
{
	if (iForRandom > 8)
	{
		iForRandom = 0;
	}
	int iRandomValue = g_fDurationTime;
	int TotalCnt = 0;
	switch (m_EnemyPos)
	{
	case 1:
		TotalCnt = m_MapSetting.EnemyPosition1.size() - LOAD_OBJECT_MGR.GetMonsterCount();
		for (int iMon = 0; iMon < TotalCnt; iMon++)
		{
			srand(iRandomValue);
			iRandomValue++;
			int iFirstRandomMonster = rand() % 3;
			int MonsterNum = rand() % 10000;
			int iResultMonsterNum;
			int iMonPos = rand() % m_MapSetting.EnemyPosition1.size();
			for (int iCheckMonster = 0; iCheckMonster < 3; iCheckMonster++)
			{
				if (MonsterNum - m_StageForMonster[StageNum]->MonsterChance[iFirstRandomMonster]->iRespawnChance[iCheckMonster] > 0)
					MonsterNum = MonsterNum - m_StageForMonster[StageNum]->MonsterChance[iFirstRandomMonster]->iRespawnChance[iCheckMonster];
				else
					iResultMonsterNum = iCheckMonster;
			}
			if (iForRandom > 8)
			{
				iForRandom = 0;
			}
			LOAD_OBJECT_MGR.CreateMonster(m_MapSetting.EnemyPosition1[iForRandom], 100100); //iMonPos //m_StageForMonster[StageNum]->MonsterChance[iFirstRandomMonster]->iMonsterIndex[iResultMonsterNum]
			iForRandom++;
		}
		break;
	case 2:
		TotalCnt = m_MapSetting.EnemyPosition2.size() - LOAD_OBJECT_MGR.GetMonsterCount();
		for (int iMon = 0; iMon < TotalCnt; iMon++)
		{
			srand(iRandomValue);
			iRandomValue++;
			int iFirstRandomMonster = rand() % 3;
			int MonsterNum = rand() % 10000;
			int iResultMonsterNum;
			int iMonPos = rand() % m_MapSetting.EnemyPosition1.size();
			for (int iCheckMonster = 0; iCheckMonster < 3; iCheckMonster++)
			{
				if (MonsterNum - m_StageForMonster[StageNum]->MonsterChance[iFirstRandomMonster]->iRespawnChance[iCheckMonster] > 0)
					MonsterNum = MonsterNum - m_StageForMonster[StageNum]->MonsterChance[iFirstRandomMonster]->iRespawnChance[iCheckMonster];
				else
					iResultMonsterNum = iCheckMonster;
			}
			if (iForRandom > 8)
			{
				iForRandom = 0;
			}
			LOAD_OBJECT_MGR.CreateMonster(m_MapSetting.EnemyPosition2[iForRandom], 100100);
			iForRandom++;
		}
		break;
	case 3:
		TotalCnt = m_MapSetting.EnemyPosition3.size() - LOAD_OBJECT_MGR.GetMonsterCount();
		for (int iMon = 0; iMon < TotalCnt; iMon++)
		{
			srand(iRandomValue);
			iRandomValue++;
			int iFirstRandomMonster = rand() % 3;
			int MonsterNum = rand() % 10000;
			int iResultMonsterNum;
			int iMonPos = rand() % m_MapSetting.EnemyPosition1.size();
			for (int iCheckMonster = 0; iCheckMonster < 3; iCheckMonster++)
			{
				if (MonsterNum - m_StageForMonster[StageNum]->MonsterChance[iFirstRandomMonster]->iRespawnChance[iCheckMonster] > 0)
					MonsterNum = MonsterNum - m_StageForMonster[StageNum]->MonsterChance[iFirstRandomMonster]->iRespawnChance[iCheckMonster];
				else
					iResultMonsterNum = iCheckMonster;
			}
			if (iForRandom > 8)
			{
				iForRandom = 0;
			}
			LOAD_OBJECT_MGR.CreateMonster(m_MapSetting.EnemyPosition3[iForRandom], 100100);
			iForRandom++;
		}
		break;
	case 4:
		TotalCnt = m_MapSetting.EnemyPosition4.size() - LOAD_OBJECT_MGR.GetMonsterCount();
		for (int iMon = 0; iMon < TotalCnt; iMon++)
		{
			srand(iRandomValue);
			iRandomValue++;
			int iFirstRandomMonster = rand() % 3;
			int MonsterNum = rand() % 10000;
			int iResultMonsterNum;
			int iMonPos = rand() % m_MapSetting.EnemyPosition1.size();
			for (int iCheckMonster = 0; iCheckMonster < 3; iCheckMonster++)
			{
				if (MonsterNum - m_StageForMonster[StageNum]->MonsterChance[iFirstRandomMonster]->iRespawnChance[iCheckMonster] > 0)
					MonsterNum = MonsterNum - m_StageForMonster[StageNum]->MonsterChance[iFirstRandomMonster]->iRespawnChance[iCheckMonster];
				else
					iResultMonsterNum = iCheckMonster;
			}
			if (iForRandom > 8)
			{
				iForRandom = 0;
			}
			LOAD_OBJECT_MGR.CreateMonster(m_MapSetting.EnemyPosition4[iForRandom], 100100);
			iForRandom++;
		}
		break;
	default:
		break;
	}
}

// 캐릭터의 위치로 맵 오브젝트를 보유하고 있는 위치를 알려주는 함수
BNode* BMapParser::GetMapObjNode(TVector3 HeroPos, BNode* pNode)
{
	if (pNode->m_pChild[0] != NULL)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (CheckInNode(HeroPos, pNode->m_pChild[iNode]))
			{
				continue;
			}
			return GetMapObjNode(HeroPos, pNode->m_pChild[iNode]);
		}
	}
	else
	{
		return pNode;
	}
}

bool BMapParser::CheckInNode(TVector3 HeroPos, BNode* pNode)
{
	if (HeroPos.x > pNode->m_tBox.vMin.x && HeroPos.x < pNode->m_tBox.vMax.x)
	{
		if (HeroPos.z > pNode->m_tBox.vMin.z && HeroPos.z < pNode->m_tBox.vMax.z)
		{
			return true;
		}
	}
	return false;
}

HRESULT BMapParser::LoadShadowShader()
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // 쉐이더 디버그 허용
	ID3DBlob* pVSBUF = NULL;
	ID3DBlob* pPSBUF = NULL;

	ID3DBlob* pError = NULL; // 이것을 넣음으로써 오류가 난 이유를 받아 낼 수 있다.
							 /* vs_5_0 == direct11 은 5.0을 사용 * pVSBuf 결과가 담김 */

	if (FAILED(D3DCompileFromFile(L"../../data/Shader/ShadowShader.hlsl", NULL, NULL, "VS", "vs_5_0", NULL, NULL, &pVSBUF, &pError)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		MessageBox(NULL, buffer, _T("VS"), MB_OK);
	}
	if (pVSBUF != nullptr)
	{
		S_RETURN(g_pd3dDevice->CreateVertexShader((DWORD*)pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), NULL, &m_pShadowVS));
	}

	if (FAILED(D3DCompileFromFile(L"../../data/Shader/ShadowShader.hlsl", NULL, NULL, "PS", "ps_5_0", NULL, NULL, &pPSBUF, &pError)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		//MessageBox(NULL, buffer, _T("PS"), MB_OK);
	}
	if (pPSBUF != nullptr)
	{
		S_RETURN(g_pd3dDevice->CreatePixelShader((DWORD*)pPSBUF->GetBufferPointer(), pPSBUF->GetBufferSize(), NULL, &m_pShadowPS));
	}

	S_RELEASE(pVSBUF);
	S_RELEASE(pPSBUF);
	return hr;
}

HRESULT BMapParser::CreateShadowCB()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;					//CPU에서 GPU로 Access 가능하게 해주는 플래그
	cbDesc.CPUAccessFlags = 0;							//CPU에서 GPU로 Access 가능하게 해주는 플래그
	cbDesc.ByteWidth = sizeof(SHADOW_CONSTANT_BUFFER);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;

	S_RETURN(g_pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pShadowCB));
	return hr;
}


void BMapParser::RenderShadow()
{
	g_pImmediateContext->OMSetDepthStencilState(BDxState::g_pDSSDepthEnable, 0x01);
	g_pImmediateContext->OMSetBlendState(BDxState::g_pAlphaBlend, 0, 0xffffffff);
	g_pImmediateContext->RSSetState(BDxState::g_pRSBackCullSolid);
	g_pImmediateContext->RSSetState(BDxState::g_pRSSlopeScaledDepthBias);
	for (int iMapObj = 0; iMapObj < m_MapObjMgr.size(); iMapObj++)
	{
		if (m_MapObjMgr[iMapObj]->m_InstanceInfo.size())
		{
			m_MapObjMgr[iMapObj]->SetCamera(m_matShadowView, m_matShadowProj);
			g_pImmediateContext->VSSetShader(m_pShadowVS, NULL, 0);
			g_pImmediateContext->PSSetShader(NULL, NULL, 0); // NULL
			g_pImmediateContext->VSSetConstantBuffers(0, 1, &m_MapObjMgr[iMapObj]->m_pConstantBuffer);
			g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			g_pImmediateContext->IASetInputLayout(m_RenderSettingASE.m_pVertexLayout);
			// 깊이스텐실 버퍼를 리소스로 전달하면 되기 때문에 픽쉘쉐이더를 사용하지 않아도 된다.
			m_MapObjMgr[iMapObj]->DrawInstance(g_pImmediateContext, m_MapObjMgr[iMapObj], m_MapObjMgr[iMapObj]->m_InstanceInfo.size());
		}
	}
}

BMapParser::BMapParser()
{
}


BMapParser::~BMapParser()
{
}
