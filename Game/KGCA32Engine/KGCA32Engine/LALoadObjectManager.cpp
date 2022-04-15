
//#include "stdafx.h"

#include "LALoadObjectManager.h"

#include "LACoreLinkStd.h"
#include "CoreInputLink.h"
#include "CoreStandardLink.h"

#include "LAResourceManager.h"

#include "LAZombie.h"
#include "LAGunner.h"

#ifdef TOOL_MODE
#include "LAGlobalState.h"
#else

#include "BBullet.h"

#endif


void LALoadObjectManager::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BMapParser* pMap)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
	m_pMap = pMap;

	/*=========== 쉐도우 뎁스스텐실스테이트 생성 ===========*/
	CreateShadowDepthStencilViewAndState();
}

void LALoadObjectManager::SetMatrix(TMatrix* pWorldMatrix, TMatrix* pViewMatrix, TMatrix* pProjectionMatrix)
{
	int ObjectCount = 0; 
	
	//monster
	ObjectCount = m_MonsterObjectList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		m_MonsterObjectList[iCnt]->SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);
	}

	//weapon
	ObjectCount = m_WeaponObjectList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		m_WeaponObjectList[iCnt]->SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);
	}
	
	//armor
	ObjectCount = m_ArmorObjectList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		m_ArmorObjectList[iCnt]->SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);
	}

	//Hero 
	if (m_CharacterObject.get())
	{
		m_CharacterObject->SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);
	}
}

bool LALoadObjectManager::Frame()
{
#ifdef DEBUG_MODE

	//DebugMode KeyCheck
	CkeckKeyState();

#else

#endif

#ifdef TOOL_MODE

#else

	int ObjectCount = 0;
		
	//monster
	ObjectCount = m_MonsterObjectList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		//Hero 위치 셋팅
		if (m_CharacterObject)
		{
			m_MonsterObjectList[iCnt]->SetHeroPostion(m_CharacterObject->GetPosition());
		}

		//타잎에 맞추어 프레임 
		switch (m_MonsterObjectList[iCnt]->m_ObjectData.eSkillType)
		{
		case E_SkillType::SKILL_HITBOX:
		{
			//몬스터AI 업데이트
			LAZombie* pObject = (LAZombie*)m_MonsterObjectList[iCnt].get();
			pObject->Frame();
		}break;

		case E_SkillType::SKILL_BULLET:
		{
			//몬스터AI 업데이트
			LAGunner* pObject = (LAGunner*)m_MonsterObjectList[iCnt].get();
			pObject->Frame();
		}break;
		}

		//높이값 업데이트 
		TVector4 mapdata = m_pMap->GetHeight(m_MonsterObjectList[iCnt]->GetPosition(), 0, &m_MonsterObjectList[iCnt]->m_bMapCheck);
		m_MonsterObjectList[iCnt]->SetHeight(mapdata.w);

		//최종행렬계산
		m_MonsterObjectList[iCnt]->PostFrame();
	}

	// 몬스터 vs 맵 충돌
	MonsterMapCollision();

	// 몬스터 vs 몬스터 충돌
	MonsterCollision();

	//weapon
	ObjectCount = m_WeaponObjectList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		//높이값 업데이트
		TVector4 mapdata = m_pMap->GetHeight(m_WeaponObjectList[iCnt]->GetPosition());
		m_WeaponObjectList[iCnt]->SetHeight(mapdata.w);

		m_WeaponObjectList[iCnt]->Frame();
	}

	//Armor
	ObjectCount = m_ArmorObjectList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		//높이값 업데이트
		TVector4 mapdata = m_pMap->GetHeight(m_ArmorObjectList[iCnt]->GetPosition());
		m_ArmorObjectList[iCnt]->SetHeight(mapdata.w);

		m_ArmorObjectList[iCnt]->Frame();
	}

	//Hero 
	if (m_CharacterObject.get())
	{
		int piTileNumber = -1;

		//높이값 업데이트
		TVector4 mapdata = m_pMap->GetHeight(m_CharacterObject->GetPosition(), &piTileNumber);
		//m_CharacterObject->SetHeight(mapdata.w);
		m_CharacterObject->SetTileNumber(piTileNumber);
		
		m_CharacterObject->Frame();
	}

#endif



#ifdef TOOL_MODE

#else

	/* ================ 충 돌 체 크 테스트(무기 충돌시 사라지면서 데이터 받아오기) ================ */
	bool bCollitionCheck = false;

	//무기
	for (int iCnt = 0; iCnt < (int)m_WeaponObjectList.size(); iCnt++)
	{
		TVector3 Heropos = m_CharacterObject->GetPosition();
		TVector3 weaponpos = m_WeaponObjectList[iCnt]->GetPosition();
		TVector3 Distance = weaponpos - Heropos;
		float fDistance = D3DXVec3Length(&Distance);

		if (fDistance <= 4.0f)
		{
			TVector3 vCameraLook = g_pMainCamera->m_vLook;
			float fdot = D3DXVec3Dot(D3DXVec3Normalize(&vCameraLook, &vCameraLook), D3DXVec3Normalize(&Distance, &Distance));

			if (fdot >= 0.2)
			{
				m_CharacterObject->SetItemDrawMesage(true);

				if (I_Input.KeyCheck(DIK_F) == (DWORD)keyState::KEY_PUSH)
				{
					DeleteVector(m_WeaponObjectList[iCnt]->m_ParticleSystemList);

					CharacterInItem(m_WeaponObjectList[iCnt]);
				}
			}
			break;
		}
		else
		{
			m_CharacterObject->SetItemDrawMesage(false);
		}
	}

	//방어구
	for (int iCnt = 0; iCnt < (int)m_ArmorObjectList.size(); iCnt++)
	{
		TVector3 Heropos = m_CharacterObject->GetPosition();
		TVector3 weaponpos = m_ArmorObjectList[iCnt]->GetPosition();
		TVector3 Distance = weaponpos - Heropos;
		float fDistance = D3DXVec3Length(&Distance);

		if (fDistance <= 4.0f)
		{
			TVector3 vCameraLook = g_pMainCamera->m_vLook;
			float fdot = D3DXVec3Dot(D3DXVec3Normalize(&vCameraLook, &vCameraLook), D3DXVec3Normalize(&Distance, &Distance));

			if (fdot >= 0.2)
			{
				m_CharacterObject->SetItemDrawMesage(true);

				if (I_Input.KeyCheck(DIK_F) == (DWORD)keyState::KEY_PUSH)
				{
					DeleteVector(m_ArmorObjectList[iCnt]->m_ParticleSystemList);

					CharacterInItem(m_ArmorObjectList[iCnt]);
				}
			}
			break;
		}
	}

	/* ============================================= */

	//DeleateList에 등록된 오브젝트 삭제
	UpdateDeleateList();


#endif

	return true;
}

bool LALoadObjectManager::Render()
{
	int ObjectCount = 0;
	int iRenderIndex = 0;

	//Monster
	InstanceMonsterRender();
	/*ObjectCount = m_RenderMonsterList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		iRenderIndex = m_RenderMonsterList[iCnt];
		m_MonsterObjectList[iRenderIndex]->Render();
	}*/

	//Weapon
	ObjectCount = m_RenderWeaponList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		iRenderIndex = m_RenderWeaponList[iCnt];
		m_WeaponObjectList[iRenderIndex]->Render();
	}

	//Armor
	ObjectCount = m_RenderArmorList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		iRenderIndex = m_RenderArmorList[iCnt];
		m_ArmorObjectList[iRenderIndex]->Render();
	}

	//Hero 
	if (m_CharacterObject.get())
	{
		m_CharacterObject->Render();
	}
	
	//RenderShadow();
	ClearRenderList();

	return true;
}

bool LALoadObjectManager::RenderShadow()
{
	/*================= 뎁스 상태값 없다면 그림자 렌더 안함 ================*/
	if (m_pShadowStateDepthStencil == NULL)
	{
		//렌더리스트 클리어
		ClearRenderList();

		return false;
	}

	/*================= Depth, Solid, Blend 설정 ================*/
#ifdef TOOL_MODE

	//m_pCoreContext->OMSetDepthStencilState(INSTANCE_CUR_STATE.SetDssState((int)E_DSS::DSSDisable), 1);
	//m_pCoreContext->RSSetState(INSTANCE_CUR_STATE.SetRrsState((int)E_RRS::RRSSolidCullBack));
	m_pContext->OMSetBlendState(INSTANCE_CUR_STATE.SetBsState((int)E_BS::AlphaBlendEnable), 0, 1);// 0xffffffff);

#else

	m_pContext->OMSetDepthStencilState(m_pShadowStateDepthStencil, 0x01); ;
	m_pContext->OMSetBlendState(BDxState::g_pAlphaBlend, 0, 1);// 0xffffffff);

#endif

	int ObjectCount = 0;
	int iRenderIndex = 0;

	 /*================= 오브젝트 쉐도우 렌더 ================*/
	//Monster
	ObjectCount = m_RenderMonsterList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		iRenderIndex = m_RenderMonsterList[iCnt];
		m_MonsterObjectList[iRenderIndex]->RenderShadow();
	}

	//Weapon
	ObjectCount = m_RenderWeaponList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		//iRenderIndex = m_RenderWeaponList[iCnt];
		//m_WeaponObjectList[iRenderIndex]->RenderShadow();
	}

	//Armor
	ObjectCount = m_RenderArmorList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		//iRenderIndex = m_RenderArmorList[iCnt];
		//m_ArmorObjectList[iRenderIndex]->RenderShadow();
	}

	//Hero 
	if (m_CharacterObject.get())
	{
		//m_CharacterObject->RenderShadow();
	}

	/*================= 렌더리스트 클리어 ================*/
	ClearRenderList();

	/*================= 상태값 복원 ================*/
#ifdef TOOL_MODE
#else
	m_pContext->OMSetDepthStencilState(BDxState::g_pDSSDepthEnable, 0x00);
#endif

	return true;
}

bool LALoadObjectManager::Release()
{
	int ObjectCount;

	//Monster
	ObjectCount = m_MonsterObjectList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		m_MonsterObjectList[iCnt]->Release();
		m_MonsterObjectList[iCnt] = NULL;
	}
	m_MonsterObjectList.clear();

	//Weapon
	ObjectCount = m_WeaponObjectList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		m_WeaponObjectList[iCnt]->Release();
		m_WeaponObjectList[iCnt] = NULL;
	}
	m_WeaponObjectList.clear();

	//Armor
	ObjectCount = m_ArmorObjectList.size();
	for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
	{
		m_ArmorObjectList[iCnt]->Release();
		m_ArmorObjectList[iCnt] = NULL;
	}
	m_ArmorObjectList.clear();

	//Hero 
	if (m_CharacterObject.get())
	{
		m_CharacterObject->Release();
	}
	m_CharacterObject = NULL;

	return true;
}

bool LALoadObjectManager::CreateHero(TVector3 vHeroPosition, int iTableIndex)
{
	LAHero pData;
	ObjectTableLoadData* objectdata = NULL;

	//오브젝트 데이터 검색
	m_DataInfoItor = m_CharacterDataInfoList.find(iTableIndex);
	if (m_DataInfoItor != m_CharacterDataInfoList.end())
	{
		objectdata = &m_DataInfoItor->second;
	}
	else
	{
		return false;
	}

	/*for (int icnt = 0; icnt < (int)m_CharacterDataInfoList.size(); icnt++)
	{
		if (m_CharacterDataInfoList[icnt].iTableIndex == iTableIndex)
		{
			objectdata = &m_CharacterDataInfoList[icnt];
			break;
		}
	}*/

	if (objectdata == NULL)
		return false;

	//캐릭터 생성
	m_CharacterObject = std::make_shared<LAHero>(pData);
	LAHero* pObject = GetHero();

	//리소스 인덱스저장
	int iResourceIndex = -1;
	bool CheckReturn;
	iResourceIndex = RESOURCE_MGR.GetResourceIndex(objectdata->wsCodeName, objectdata->wsMatrixName, objectdata->wsSkinName);

	if (iResourceIndex != -1)
	{
		CheckReturn = pObject->SetMainResource(iResourceIndex);
	}
	else
	{
		CheckReturn = false;
	}
	//Init()
	pObject->Init(m_pDevice, m_pContext);
	//데이터 저장
	pObject->SetObjectData(*objectdata);
	//포지션셋팅
	pObject->SetPosition(vHeroPosition.x, vHeroPosition.y, vHeroPosition.z);
	//데이터 업데이트
	pObject->UpdateObjectDataInfo();
	pObject->SetObjectABilityData();
	pObject->SetHeroHpProjectileData();
	//기본동작 셋팅
	pObject->SetMotionIdle((int)E_CharacterMotion::IDLE);
	//사운드 셋팅
	pObject->SetSound(objectdata->eObjectType, 0);
	//디렉션 라이트 셋팅
	pObject->SetDirectionLight(TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector4(1280.0f, 800.0f, -1280.0f, 1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f));


#ifdef TOOL_MODE

#else

	//카메라 셋팅
	TVector3 vheropos = vHeroPosition;
	vheropos.y += 1.5;
	g_pMainCamera->m_vPos = TVector3(vheropos.x, vheropos.y, vheropos.z - 1);
	g_pMainCamera->Init(vheropos);
	g_pMainCamera->SetHeroPosition(&pObject->m_vObjectPosition);
	pObject->SetDirection(&g_pMainCamera->m_vHeroLook);

#endif

	return true;
}

LAHero* LALoadObjectManager::GetHero()
{
	if (m_CharacterObject.get())
	{
		return m_CharacterObject.get();
	}

	return NULL;
}

bool LALoadObjectManager::DeleteHero(int iIndex)
{
	if (m_CharacterObject)
	{
		m_CharacterObject->Release();
	}
	m_CharacterObject = NULL;

	return true;
}

bool LALoadObjectManager::CreateMonster(TVector3 vHeroPosition, int iTableIndex)
{
	
	bool CheckReturn;
	ObjectTableLoadData* objectdata = NULL;
	SkillTableLoadData* SkillData = NULL;

	//오브젝트 데이터 검색
	m_DataInfoItor = m_MonsterDataInfoList.find(iTableIndex);
	if (m_DataInfoItor != m_MonsterDataInfoList.end())
	{
		objectdata = &m_DataInfoItor->second;
	}
	else
	{
		return false;
	}
	/*for (int icnt = 0; icnt < (int)m_MonsterDataInfoList.size(); icnt++)
	{
		if (m_MonsterDataInfoList[icnt].iTableIndex == iTableIndex)
		{
			objectdata = &m_MonsterDataInfoList[icnt];
			break;
		}
	}*/
	if (objectdata == NULL)
		return false;

	switch (objectdata->eSkillType)
	{
	case E_SkillType::SKILL_HITBOX:
	{
		m_MonsterObjectList.push_back(std::make_shared<LAZombie>());
	}break;

	case E_SkillType::SKILL_BULLET:
	{
		m_MonsterObjectList.push_back(std::make_shared<LAGunner>());
	}break;
	}
	
	LAUnit* pObject = m_MonsterObjectList[(int)m_MonsterObjectList.size() - 1].get();

	//리소스 인덱스저장
	int iResourceIndex = -1;
	iResourceIndex = RESOURCE_MGR.GetResourceIndex(objectdata->wsCodeName, objectdata->wsMatrixName, objectdata->wsSkinName);

	if (iResourceIndex != -1)
	{
		CheckReturn = pObject->SetMainResource(iResourceIndex);
	}
	else
	{
		CheckReturn = false;
	}
	//데이터 저장
	pObject->SetObjectData(*objectdata);
	//Init()
	pObject->Init(m_pDevice, m_pContext);

#ifdef TOOL_MODE
#else
	//AI에 맵멤버 추가
	pObject->InsertMapMember(m_pMap);
#endif

	//포지션셋팅
	pObject->SetPosition(vHeroPosition.x, vHeroPosition.y, vHeroPosition.z);
	//데이터 업데이트
	pObject->UpdateObjectDataInfo();
	pObject->SetObjectABilityData();
	//사운드 셋팅
	pObject->SetSound(objectdata->eObjectType, 1);
	//스킬타입 셋팅
	pObject->SetSkillData(SetObjectSkillType(*objectdata));
	//기본동작셋팅
	pObject->SetMotionIdle((int)E_MonsterMotion::WORK);
	//디렉션 라이트 셋팅
	pObject->SetDirectionLight(TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector4(1280.0f, 800.0f, -1280.0f, 1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f));


	if (CheckReturn == false)
	{
		return false;
	}

	return true;
}

LAUnit* LALoadObjectManager::GetMonsterObject(int iPsbObjectIndex)
{
	if ((m_MonsterObjectList.size() == 0) || 
		(m_MonsterObjectList.size() <= iPsbObjectIndex))
		return NULL;

	return m_MonsterObjectList[iPsbObjectIndex].get();
}

int LALoadObjectManager::GetMonsterCount()
{
	return m_MonsterObjectList.size();
}

bool LALoadObjectManager::AddRenderMonster(int iaddIndex)
{
	if (((int)m_MonsterObjectList.size() == 0) ||
		((int)m_MonsterObjectList.size() <= iaddIndex))
	{
		return false;
	}

	m_RenderMonsterList.push_back(iaddIndex);

	return true;
}

bool LALoadObjectManager::DeleteMonster(int iIndex)
{
	int iSize = m_MonsterObjectList.size();

	if (iSize && iSize > iIndex)
	{
		m_MonsterObjectList[iIndex]->Release();

		m_MonsterObjectList.erase(m_MonsterObjectList.begin() + iIndex);

		return true;
	}

	return false;
}

// 몬스터 vs 몬스터 충돌처리
void LALoadObjectManager::MonsterCollision()
{
#ifdef TOOL_MODE

#else
	for (int iFront = 0; iFront < m_MonsterObjectList.size(); iFront++)
	{
		for (int iBack = 0; iBack < m_MonsterObjectList.size(); iBack++)
		{
			if (iFront == iBack)
				continue;

			// 충돌체크
			if (m_MonsterObjectList[iFront]->m_BoundingBox.m_Collision.CheckSphereInSphere(
				m_MonsterObjectList[iFront]->m_BoundingBox.m_vCenter,
				m_MonsterObjectList[iBack]->m_BoundingBox.m_vCenter,
				m_MonsterObjectList[iFront]->m_BoundingBox.m_fRadius,
				m_MonsterObjectList[iBack]->m_BoundingBox.m_fRadius))
			{
				// 뒤에있는 몬스터를 충돌된위치에서 뒤로 뺀다.
				TVector3 vPos = m_MonsterObjectList[iFront]->m_BoundingBox.m_Collision.SphereInSphereOppositeDir(
					m_MonsterObjectList[iBack]->m_BoundingBox.m_vCenter,
					m_MonsterObjectList[iFront]->m_BoundingBox.m_vCenter,
					m_MonsterObjectList[iBack]->m_BoundingBox.m_fRadius,
					m_MonsterObjectList[iFront]->m_BoundingBox.m_fRadius);

				// 뒤에있는 몬스터 위치 변경
				m_MonsterObjectList[iBack]->m_vObjectPosition =
					m_MonsterObjectList[iBack]->m_vObjectPosition + vPos * m_MonsterObjectList[iBack]->m_ObjectAgregateData.fTotalMoveSpeed * g_fSecPerFrame;
			}
		}
	}
#endif

}

std::vector<std::shared_ptr<LAUnit>>& LALoadObjectManager::GetMonsterList()
{
	return m_MonsterObjectList;
}

// 몬스터 vs 맵 충돌처리
void LALoadObjectManager::MonsterMapCollision()
{
	for (int iMonster = 0; iMonster < m_MonsterObjectList.size(); iMonster++)
	{
		if (m_MonsterObjectList[iMonster]->m_bMapCheck == false)
			m_MonsterObjectList[iMonster]->m_vObjectBeforePosition = m_MonsterObjectList[iMonster]->m_vObjectPosition;

		if (m_MonsterObjectList[iMonster]->m_bMapCheck == true)
		{

#ifdef TOOL_MODE
#else
			m_MonsterObjectList[iMonster]->m_BoundingBox.m_Collision.SlipDetection(
				m_MonsterObjectList[iMonster]->m_vObjectPosition,
				m_MonsterObjectList[iMonster]->m_vObjectBeforePosition,
				m_MonsterObjectList[iMonster]->m_ObjectAgregateData.fTotalMoveSpeed);
			m_MonsterObjectList[iMonster]->isCollision = true;
			m_MonsterObjectList[iMonster]->CollisionTick = 0.0f;
#endif
		}
	}
}

bool LALoadObjectManager::CreateWeapon(TVector3 vHeroPosition, int iTableIndex, int iBulletCount)
{
	LAItem pData;
	bool CheckReturn;
	ObjectTableLoadData* objectdata = NULL;

	//오브젝트 데이터 검색
	m_DataInfoItor = m_WeaponDataInfoList.find(iTableIndex);
	if (m_DataInfoItor != m_WeaponDataInfoList.end())
	{
		objectdata = &m_DataInfoItor->second;
	}
	else
	{
		return false;
	}
	/*for (int icnt = 0; icnt < (int)m_WeaponDataInfoList.size(); icnt++)
	{
		if (m_WeaponDataInfoList[icnt].iTableIndex == iTableIndex)
		{
			objectdata = &m_WeaponDataInfoList[icnt];
			break;
		}
	}*/
	if (objectdata == NULL)
		return false;

	

	//등록
	m_WeaponObjectList.push_back(std::make_shared<LAItem>(pData));
	LAItem* pObject = GetWeaponObject(m_WeaponObjectList.size() - 1);
	
	//리소스 인덱스저장
	int iResourceIndex = -1;
	iResourceIndex = RESOURCE_MGR.GetResourceIndex(objectdata->wsCodeName, objectdata->wsMatrixName, objectdata->wsSkinName);

	if (iResourceIndex != -1)
	{
		CheckReturn = pObject->SetMainResource(iResourceIndex);
	}
	else
	{
		CheckReturn = false;
	}
	//Init()
	pObject->Init(m_pDevice, m_pContext);
	//데이터 저장(복사)
	pObject->SetObjectData(*objectdata);
	//아이템 수량 셋팅
	pObject->m_ObjectData.iBulletCount = iBulletCount;
	//포지션셋팅
	pObject->SetPosition(vHeroPosition.x, vHeroPosition.y, vHeroPosition.z);
	//데이터 업데이트
	pObject->UpdateObjectDataInfo();
	//발사체 데이터 생성
	pObject->SetProjectileData();
	//사운드 셋팅
	pObject->SetSound(objectdata->eObjectType, 0);
	//스킬타입 셋팅
	pObject->SetSkillData(SetObjectSkillType(*objectdata));
	//파티클 생성
	pObject->CreateParticle(m_pMap);
	//디렉션 라이트 셋팅
	pObject->SetDirectionLight(TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector4(1280.0f, 800.0f, -1280.0f, 1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f));

	if (CheckReturn == false)
	{
		return false;
	}

	
	return true;
}

int LALoadObjectManager::GetWeaponSize()
{
	return m_WeaponObjectList.size();
}

LAItem* LALoadObjectManager::GetWeaponObject(int iObjectIndex)
{
	if ((m_WeaponObjectList.size() == 0) ||
		(m_WeaponObjectList.size() <= iObjectIndex))
		return NULL;

	return m_WeaponObjectList[iObjectIndex].get();
}

int LALoadObjectManager::GetWeaponCount()
{
	return m_WeaponObjectList.size();
}

bool LALoadObjectManager::AddRenderWeapon(int iaddIndex)
{
	if (((int)m_WeaponObjectList.size() == 0) ||
		((int)m_WeaponObjectList.size() <= iaddIndex))
	{
		return false;
	}

	m_RenderWeaponList.push_back(iaddIndex);
	
	return true;
}

bool LALoadObjectManager::DeleteWeapon(int iIndex)
{
	int iSize = m_WeaponObjectList.size();

	if (iSize && iSize > iIndex)
	{
		m_WeaponObjectList[iIndex]->Release();

		m_WeaponObjectList.erase(m_WeaponObjectList.begin() + iIndex);

		return true;
	}

	return false;
}

bool LALoadObjectManager::CreateArmor(TVector3 vHeroPosition, int iTableIndex)
{
	LAItem pData;
	bool CheckReturn;
	ObjectTableLoadData* objectdata = NULL;

	//오브젝트 데이터 검색
	m_DataInfoItor = m_ArmorDataInfoList.find(iTableIndex);
	if (m_DataInfoItor != m_ArmorDataInfoList.end())
	{
		objectdata = &m_DataInfoItor->second;
	}
	else
	{
		return false;
	}
	/*for (int icnt = 0; icnt < (int)m_ArmorDataInfoList.size(); icnt++)
	{
		if (m_ArmorDataInfoList[icnt].iTableIndex == iTableIndex)
		{
			objectdata = &m_ArmorDataInfoList[icnt];
			break;
		}
	}*/
	if (objectdata == NULL)
		return false;

	//등록
	m_ArmorObjectList.push_back(std::make_shared<LAItem>(pData));
	LAItem* pObject = GetArmorObject(m_ArmorObjectList.size() - 1);

	//리소스 인덱스저장
	int iResourceIndex = -1;
	iResourceIndex = RESOURCE_MGR.GetResourceIndex(objectdata->wsCodeName, objectdata->wsMatrixName, objectdata->wsSkinName);

	if (iResourceIndex != -1)
	{
		CheckReturn = pObject->SetMainResource(iResourceIndex);
	}
	else
	{
		CheckReturn = false;
	}
	//Init()
	pObject->Init(m_pDevice, m_pContext);
	//데이터 저장(복사)
	pObject->SetObjectData(*objectdata);
	//아이템 수량 셋팅
	//pObject->m_ObjectData.iBulletCount = iBulletCount;
	//포지션셋팅
	pObject->SetPosition(vHeroPosition.x, vHeroPosition.y, vHeroPosition.z);
	//데이터 업데이트
	pObject->UpdateObjectDataInfo();
	//발사체 데이터 생성
	//pObject->SetProjectileData();
	//사운드 셋팅
	pObject->SetSound(objectdata->eObjectType, 0);
	//스킬타입 셋팅
	//pObject->SetSkillData(SetObjectSkillType(*objectdata));
	//파티클 생성
	pObject->CreateParticle(m_pMap);
	//디렉션 라이트 셋팅
	pObject->SetDirectionLight(TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector4(1280.0f, 800.0f, -1280.0f, 1.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f));

	if (CheckReturn == false)
	{
		return false;
	}

	return true;
}

LAItem* LALoadObjectManager::GetArmorObject(int iObjectIndex)
{
	if ((m_ArmorObjectList.size() == 0) ||
		(m_ArmorObjectList.size() <= iObjectIndex))
		return NULL;

	return m_ArmorObjectList[iObjectIndex].get();
}

int LALoadObjectManager::GetArmorCount()
{
	return m_ArmorObjectList.size();
}

bool LALoadObjectManager::AddRenderArmor(int iaddIndex)
{
	if (((int)m_ArmorObjectList.size() == 0) ||
		((int)m_ArmorObjectList.size() <= iaddIndex))
	{
		return false;
	}

	m_RenderArmorList.push_back(iaddIndex);

	return true;
}

bool LALoadObjectManager::DeleteArmor(int iIndex)
{
	int iSize = m_ArmorObjectList.size();

	if (iSize && iSize > iIndex)
	{
		m_ArmorObjectList[iIndex]->Release();

		m_ArmorObjectList.erase(m_ArmorObjectList.begin() + iIndex);

		return true;
	}

	return false;
}

void LALoadObjectManager::AddObjectDataInfo(ObjectTableLoadData& rObjectData)
{
	switch ((int)rObjectData.eObjectType)
	{
		case (int)E_LoadTableType::CHARACTER:
		{
			m_CharacterDataInfoList.insert(pair<int, ObjectTableLoadData> (rObjectData.iTableIndex, rObjectData));
		}break;
		case (int)E_LoadTableType::MONSTER:
		{
			m_MonsterDataInfoList.insert(pair<int, ObjectTableLoadData>(rObjectData.iTableIndex, rObjectData));
		}break;
		case (int)E_LoadTableType::WEAPON:
		{
			m_WeaponDataInfoList.insert(pair<int, ObjectTableLoadData>(rObjectData.iTableIndex, rObjectData));
		}break;
		case (int)E_LoadTableType::ARMOR:
		{
			m_ArmorDataInfoList.insert(pair<int, ObjectTableLoadData>(rObjectData.iTableIndex, rObjectData));
		}break;
	}
}

void LALoadObjectManager::AddSkillDataInfo(SkillTableLoadData& rSkillData)
{
	switch ((int)rSkillData.eObjectType)
	{
	case (int)E_LoadTableType::SKILL_HITBOX:
	{
		m_SkillHItboxDataInfoList.push_back(rSkillData);
	}break;
	case (int)E_LoadTableType::SKILL_BULLET:
	{
		m_SkillBulletDataInfoList.push_back(rSkillData);
	}break;
	case (int)E_LoadTableType::SKILL_BUFF:
	{
		m_SkillBuffDataInfoList.push_back(rSkillData);
	}break;
	case (int)E_LoadTableType::SKILL_TRAP:
	{
		m_SkillTrapDataInfoList.push_back(rSkillData);
	}break;
	}
}

SkillTableLoadData* LALoadObjectManager::SetObjectSkillType(ObjectTableLoadData& rObjectData)
{
	int iCount = 0; 

	switch ((int)rObjectData.eSkillType)
	{
	case (int)E_SkillType::SKILL_HITBOX:
	{
		iCount = m_SkillHItboxDataInfoList.size();

		for (int iCnt = 0; iCnt < iCount; iCnt++)
		{
			if (rObjectData.wsSkillName.compare(m_SkillHItboxDataInfoList[iCnt].wsCodeName) == 0)
			{
				return &m_SkillHItboxDataInfoList[iCnt];
			}
		}
		return NULL;
	}break;

	case (int)E_SkillType::SKILL_BULLET:
	{
		iCount = m_SkillBulletDataInfoList.size();

		for (int iCnt = 0; iCnt < iCount; iCnt++)
		{
			if (rObjectData.wsSkillName.compare(m_SkillBulletDataInfoList[iCnt].wsCodeName) == 0)
			{
				return &m_SkillBulletDataInfoList[iCnt];
			}
		}
		return NULL;
	}break;

	case (int)E_SkillType::SKILL_BUFF:
	{
		iCount = m_SkillBuffDataInfoList.size();

		for (int iCnt = 0; iCnt < iCount; iCnt++)
		{
			if (rObjectData.wsSkillName.compare(m_SkillBuffDataInfoList[iCnt].wsCodeName) == 0)
			{
				return &m_SkillBuffDataInfoList[iCnt];
			}
		}
		return NULL;
	}break;

	case (int)E_SkillType::SKILL_TRAP:
	{
		iCount = m_SkillTrapDataInfoList.size();

		for (int iCnt = 0; iCnt < iCount; iCnt++)
		{
			if (rObjectData.wsSkillName.compare(m_SkillTrapDataInfoList[iCnt].wsCodeName) == 0)
			{
				return &m_SkillTrapDataInfoList[iCnt];
			}
		}
		return NULL;
	}break;
	}

	return NULL;
}

ID3D11DepthStencilState* LALoadObjectManager::GetShadowDepthStencil()
{
	return m_pShadowStateDepthStencil;
}

void LALoadObjectManager::ClearRenderList()
{
	//monster
	m_RenderMonsterList.clear();

	//weapon
	m_RenderWeaponList.clear();

	//Armor
	m_RenderArmorList.clear();
}

void LALoadObjectManager::CharacterInItem(std::shared_ptr<LAItem >& pAdrees)
{
	
	switch (pAdrees->m_ObjectData.eObjectType)
	{
	case E_LoadTableType::WEAPON:
	{
		if (m_CharacterObject->AddWeapon(pAdrees))
		{
			//true 아이템이 있다면 이미 존재함 지워줌
			AddDeleteList(pAdrees->m_ObjectData.eObjectType, pAdrees.get());
		}
		else
		{
			//아이템이 없다면 이동
			for (int iCnt = 0; iCnt < (int)m_WeaponObjectList.size(); iCnt++)
			{
				if (m_WeaponObjectList[iCnt] == pAdrees)
				{
					//리스트 아이템 삭제
					m_WeaponObjectList.erase(m_WeaponObjectList.begin() + iCnt);

					break;
				}
			}
		}
	}break;

	case E_LoadTableType::ARMOR:
	{
		if (m_CharacterObject->AddArmor(pAdrees))
		{
			//true 아이템이 있다면 이미 존재함 지워줌
			AddDeleteList(pAdrees->m_ObjectData.eObjectType, pAdrees.get());
		}
		else
		{
			//아이템이 없다면 이동
			for (int iCnt = 0; iCnt < (int)m_ArmorObjectList.size(); iCnt++)
			{
				if (m_ArmorObjectList[iCnt] == pAdrees)
				{
					//리스트 아이템 삭제
					m_ArmorObjectList.erase(m_ArmorObjectList.begin() + iCnt);

					break;
				}
			}
		}
	}break;
	}


}

void LALoadObjectManager::MonsterInItem(E_LoadTableType eType, LAUnit* pMonsterThis, int iItemIndex)
{
	/*=================== 몬스터 인덱스 검색 ===================*/
	int iMonstIndex = -1;
	for (int iCnt = 0; iCnt < (int)m_MonsterObjectList.size(); iCnt++)
	{
		if (m_MonsterObjectList[iCnt].get() == pMonsterThis)
		{
			iMonstIndex = iCnt;
			break;
		}
	}
	if (iMonstIndex == -1)
		return;

	/*=================== 아이템 인덱스 체크 ===================*/
	if ((m_WeaponObjectList.size() < 0) && (m_WeaponObjectList.size() < iItemIndex))
	{
		return;
	}

#ifdef TOOL_MODE
#else
	/*=================== 아이템 파티클 정리 ===================*/
	DeleteVector(m_WeaponObjectList[iItemIndex]->m_ParticleSystemList);
#endif
	


	LAGunner* pObject = (LAGunner*)m_MonsterObjectList[iMonstIndex].get();
	switch (eType)
	{
	case E_LoadTableType::WEAPON:
	{
		if (m_MonsterObjectList[iMonstIndex]->m_ObjectData.eSkillType == E_SkillType::SKILL_BULLET)
		{
			if (pObject->m_pItemControl->AddWeapon(m_WeaponObjectList[iItemIndex]))
			{
				//true 아이템이 있다면 이미 존재함 지워줌
				AddDeleteList(eType, m_WeaponObjectList[iItemIndex].get());
			}
			else
			{
				//아이템이 없다면 이동
				for (int iCnt = 0; iCnt < (int)m_WeaponObjectList.size(); iCnt++)
				{
					if (m_WeaponObjectList[iCnt] == m_WeaponObjectList[iItemIndex])
					{
						//리스트 아이템 삭제
						m_WeaponObjectList.erase(m_WeaponObjectList.begin() + iCnt);

						break;
					}
				}
			}
			pObject->m_pItemControl->SetWeapon(E_InventoryWeaponType::WEAPON_AK47);
		}
	}break;

	case E_LoadTableType::ARMOR:
	{

	}break;
	}

	

}

void LALoadObjectManager::AllDeleateObject()
{
	DeleateMonsterList();
	DeleateWeaponList();
	DeleateArmorList();
}

void LALoadObjectManager::DeleateMonsterList()
{
	for (int iCnt = 0; iCnt < m_MonsterObjectList.size(); iCnt++)
	{
		m_MonsterObjectList[iCnt]->Release();
	}
	m_MonsterObjectList.clear();

#ifdef TOOL_MODE
#else
	LA_SOUND2_MGR.AllStop();
#endif
}

void LALoadObjectManager::DeleateWeaponList()
{
	for (int iCnt = 0; iCnt < m_WeaponObjectList.size(); iCnt++)
	{
		m_WeaponObjectList[iCnt]->Release();
	}
	m_WeaponObjectList.clear();
}

void LALoadObjectManager::DeleateArmorList()
{
	for (int iCnt = 0; iCnt < m_ArmorObjectList.size(); iCnt++)
	{
		m_ArmorObjectList[iCnt]->Release();
	}
	m_ArmorObjectList.clear();
}


bool LALoadObjectManager::CheckOverlapTableIndex(E_LoadTableType eTybleType, int iTableIndex)
{
	switch (eTybleType)
	{
	case E_LoadTableType::CHARACTER:
	{
		m_DataInfoItor = m_CharacterDataInfoList.find(iTableIndex);

		if (m_DataInfoItor != m_CharacterDataInfoList.end())
		{
			return true;
		}
		return false;

	}break;

	case E_LoadTableType::MONSTER:
	{
		m_DataInfoItor = m_MonsterDataInfoList.find(iTableIndex);

		if (m_DataInfoItor != m_MonsterDataInfoList.end())
		{
			return true;
		}
		return false;
	}break;

	case E_LoadTableType::WEAPON:
	{
		m_DataInfoItor = m_WeaponDataInfoList.find(iTableIndex);

		if (m_DataInfoItor != m_WeaponDataInfoList.end())
		{
			return true;
		}
		return false;
	}break;

	case E_LoadTableType::ARMOR:
	{
		m_DataInfoItor = m_ArmorDataInfoList.find(iTableIndex);

		if (m_DataInfoItor != m_ArmorDataInfoList.end())
		{
			return true;
		}
		return false;
	}break;
	}
}

void LALoadObjectManager::CkeckKeyState()
{
	//if (I_Input.KeyCheck(DIK_G) == (DWORD)keyState::KEY_PUSH)
	//{
	//	LAObjectUIDataInfo* pdata = m_CharacterObject->m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::RIPLE_UI_DATA];// .CharacterUIData[(int)E_LACharacterUiDataType::RIPLE_UI_DATA];
	//	if (pdata)
	//	{
	//		float iCurrentcount = pdata->iCurrentCount;
	//		float iMaxCount = pdata->iMaxCount;

	//		std::wstring debugString;
	//		debugString = L"Riple Bullet 현제갯수 : ";
	//		debugString += std::to_wstring(iCurrentcount);
	//		debugString += L"Riple Bullet 최대갯수 : ";
	//		debugString += std::to_wstring(iMaxCount);

	//		OutputDebugString(L"\n");
	//		OutputDebugString(debugString.c_str());
	//		OutputDebugString(L"\n");
	//	}

	//	pdata = NULL;
	//	pdata = m_CharacterObject->m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA];// .CharacterUIData[(int)E_LACharacterUiDataType::RIPLE_UI_DATA];
	//	if (pdata)
	//	{
	//		float iCurrentcount = pdata->iCurrentCount;
	//		float iMaxCount = pdata->iMaxCount;

	//		std::wstring debugString;
	//		debugString = L"캐릭터 현제 HP : ";
	//		debugString += std::to_wstring(iCurrentcount);
	//		debugString += L"최대 HP : ";
	//		debugString += std::to_wstring(iMaxCount);

	//		OutputDebugString(debugString.c_str());
	//		OutputDebugString(L"\n");
	//	}

	//	float iinvenripleCount = m_CharacterObject->m_ObjectAgregateData.iInventoryRipleBulletCount;
	//	float iinvenbombCount = m_CharacterObject->m_ObjectAgregateData.iInventoryBombCount;

	//	std::wstring debugString;
	//	debugString = L"인벤토리 소총 총알갯수 : ";
	//	debugString += std::to_wstring(iinvenripleCount);
	//	debugString += L"인벤토리 폭탄 갯수 : ";
	//	debugString += std::to_wstring(iinvenbombCount);

	//	OutputDebugString(debugString.c_str());
	//	OutputDebugString(L"\n");

	//	TVector3 HeroPos = m_CharacterObject->GetCameraPosition();
	//}

	if (I_Input.KeyCheck(DIK_F9) == (DWORD)keyState::KEY_UP)
	{
		m_bLoadManagerBipedRender = !m_bLoadManagerBipedRender;

		int ObjectCount = 0;
		//Character 
		if (m_CharacterObject.get())
		{
			m_CharacterObject->m_bBipedRender = m_bLoadManagerBipedRender;

			for (int iCnt = 0; iCnt < MAX_WEAPON_SIZE; iCnt++)
			{
				if (m_CharacterObject->m_pItemControl->m_pWeaponList[iCnt])
				{
					m_CharacterObject->m_pItemControl->m_pWeaponList[iCnt]->m_bBipedRender = m_bLoadManagerBipedRender;
				}
			}
			for (int iCnt = 0; iCnt < MAX_ARMOR_SIZE; iCnt++)
			{
				if (m_CharacterObject->m_pItemControl->m_pArmorList[iCnt])
				{
					m_CharacterObject->m_pItemControl->m_pArmorList[iCnt]->m_bBipedRender = m_bLoadManagerBipedRender;
				}
			}
		}

		//Monster
		ObjectCount = m_MonsterObjectList.size();
		for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
		{
			m_MonsterObjectList[iCnt]->m_bBipedRender = m_bLoadManagerBipedRender;
		}

		//weapon
		ObjectCount = m_WeaponObjectList.size();
		for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
		{
			m_WeaponObjectList[iCnt]->m_bBipedRender = m_bLoadManagerBipedRender;
		}

		//Armor
		ObjectCount = m_ArmorObjectList.size();
		for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
		{
			m_ArmorObjectList[iCnt]->m_bBipedRender = m_bLoadManagerBipedRender;
		}
	}

	if (I_Input.KeyCheck(DIK_F10) == (DWORD)keyState::KEY_UP)
	{
		m_bLoadManagerSkinRender = !m_bLoadManagerSkinRender;

		int ObjectCount = 0;
		//Character 
		if (m_CharacterObject.get())
		{
			m_CharacterObject->m_bSkinRender = m_bLoadManagerSkinRender;

			for (int iCnt = 0; iCnt < MAX_WEAPON_SIZE; iCnt++)
			{
				if (m_CharacterObject->m_pItemControl->m_pWeaponList[iCnt])
				{
					m_CharacterObject->m_pItemControl->m_pWeaponList[iCnt]->m_bSkinRender = m_bLoadManagerSkinRender;
				}
			}
			for (int iCnt = 0; iCnt < MAX_ARMOR_SIZE; iCnt++)
			{
				if (m_CharacterObject->m_pItemControl->m_pArmorList[iCnt])
				{
					m_CharacterObject->m_pItemControl->m_pArmorList[iCnt]->m_bSkinRender = m_bLoadManagerSkinRender;
				}
			}
		}

		//Monster
		ObjectCount = m_MonsterObjectList.size();
		for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
		{
			m_MonsterObjectList[iCnt]->m_bSkinRender = m_bLoadManagerSkinRender;
		}

		//weapon
		ObjectCount = m_WeaponObjectList.size();
		for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
		{
			m_WeaponObjectList[iCnt]->m_bSkinRender = m_bLoadManagerSkinRender;
		}

		//Armor
		ObjectCount = m_ArmorObjectList.size();
		for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
		{
			m_ArmorObjectList[iCnt]->m_bSkinRender = m_bLoadManagerSkinRender;
		}
	}

#ifdef TOOL_MODE

#else

	if (I_Input.KeyCheck(DIK_F3) == (DWORD)keyState::KEY_UP)
	{
		m_bLoadManagerDrawLineRender = !m_bLoadManagerDrawLineRender;

		int ObjectCount = 0;
		//Character 
		if (m_CharacterObject.get())
		{
			m_CharacterObject->m_bDrawCollitionLine = m_bLoadManagerDrawLineRender;

			for (int iCnt = 0; iCnt < MAX_WEAPON_SIZE; iCnt++)
			{
				if (m_CharacterObject->m_pItemControl->m_pWeaponList[iCnt])
				{
					m_CharacterObject->m_pItemControl->m_pWeaponList[iCnt]->m_bDrawCollitionLine = m_bLoadManagerDrawLineRender;
				}
			}
			for (int iCnt = 0; iCnt < MAX_ARMOR_SIZE; iCnt++)
			{
				if (m_CharacterObject->m_pItemControl->m_pArmorList[iCnt])
				{
					m_CharacterObject->m_pItemControl->m_pArmorList[iCnt]->m_bDrawCollitionLine = m_bLoadManagerDrawLineRender;
				}
			}
		}

		//Monster
		ObjectCount = m_MonsterObjectList.size();
		for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
		{
			m_MonsterObjectList[iCnt]->m_bDrawCollitionLine = m_bLoadManagerDrawLineRender;
		}

		//weapon
		ObjectCount = m_WeaponObjectList.size();
		for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
		{
			m_WeaponObjectList[iCnt]->m_bDrawCollitionLine = m_bLoadManagerDrawLineRender;
		}

		//Armor
		ObjectCount = m_ArmorObjectList.size();
		for (int iCnt = 0; iCnt < ObjectCount; iCnt++)
		{
			m_ArmorObjectList[iCnt]->m_bDrawCollitionLine = m_bLoadManagerDrawLineRender;
		}
	}

#endif
}

void LALoadObjectManager::AddDeleteList(E_LoadTableType eType, void* pAdrees)
{
	DeleteObjectInfo deletedata;

	deletedata.type = eType;
	deletedata.pObjectAdrees = pAdrees;

	m_DeleteObjectList.push_back(deletedata);
}

void LALoadObjectManager::UpdateDeleateList()
{
	int deleteSize = m_DeleteObjectList.size();

	if (deleteSize)
	{
		for (int iCnt = 0; iCnt < deleteSize; iCnt++)
		{
			DeleteObjectInfo data = m_DeleteObjectList[iCnt];

			switch (data.type)
			{
			case E_LoadTableType::CHARACTER:
			{
				m_CharacterObject->Release();
				m_CharacterObject = NULL;

			}break;

			case E_LoadTableType::MONSTER:
			{
				for (int iCnt = 0; iCnt < m_MonsterObjectList.size(); iCnt++)
				{
					LAUnit* pData = m_MonsterObjectList[iCnt].get();
					if (pData == data.pObjectAdrees)
					{
						pData->Release();
						m_MonsterObjectList.erase(m_MonsterObjectList.begin() + iCnt);

						break;
					}
				}
			}break;

			case E_LoadTableType::WEAPON:
			{
				for (int iCnt = 0; iCnt < m_WeaponObjectList.size(); iCnt++)
				{
					LAItem* pData = m_WeaponObjectList[iCnt].get();
					if (pData == data.pObjectAdrees)
					{
						m_WeaponObjectList[iCnt]->Release();
						m_WeaponObjectList.erase(m_WeaponObjectList.begin() + iCnt);

						break;
					}
				}
			}break;

			case E_LoadTableType::ARMOR:
			{
				for (int iCnt = 0; iCnt < m_ArmorObjectList.size(); iCnt++)
				{
					LAItem* pData = m_ArmorObjectList[iCnt].get();
					if (pData == data.pObjectAdrees)
					{
						m_ArmorObjectList[iCnt]->Release();
						m_ArmorObjectList.erase(m_ArmorObjectList.begin() + iCnt);

						break;
					}
				}
			}break;

			case E_LoadTableType::OBJECT:
			{

			}break;

			case E_LoadTableType::MAP_OBJECT:
			{

			}break;
			}
		}

		m_DeleteObjectList.clear();
	}
	
}

bool LALoadObjectManager::CreateShadowDepthStencilViewAndState()
{
	/*============ 뎁스 스텐실 상태값 생성 =============*/
	HRESULT hr;

	D3D11_DEPTH_STENCIL_DESC dsd;
	memset(&dsd, 0, sizeof(dsd));
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	dsd.StencilEnable = TRUE;
	dsd.StencilReadMask = 1;
	dsd.StencilWriteMask = 1;

	dsd.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
	dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	// 디폴트 값
	dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	hr = m_pDevice->CreateDepthStencilState(&dsd, &m_pShadowStateDepthStencil);
	if (hr != S_OK)
	{
		OutputDebugString(L"\n Not!! Create Shadow DepthStenciState00\n");
		return false;
	}


	return true;
}

bool LALoadObjectManager::InstanceMonsterRender()
{
	if (m_RenderMonsterList.size() <= 0)
		return true;

	
	InstancingDataInfo ZomebieInstancingData[10];

	/*========== 몬스터 리소스별로 인스턴싱 렌더 ==========*/
	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdressSkinName(L"zombie01_body_binary.PSB");
	if (pResourceData)
	{
		int iRenderMonsterCount = m_RenderMonsterList.size();
		
		if (iRenderMonsterCount)
		{
			int iRenderIndex = 0;
			int InstanceSaveCount = 0;
			int InstanceMatrixCount = 0;

			/*========== 쉐이더에 넘길 데이터 생성(몬스터별 데이터) ==========*/
			for (int iCnt = 0; iCnt < iRenderMonsterCount; iCnt++)
			{
				if ((iCnt != 0) && (iCnt % 3 == 0))
				{
					InstanceSaveCount++;
					InstanceMatrixCount = 0;
				}

				iRenderIndex = m_RenderMonsterList[iCnt];
				m_MonsterObjectList[iRenderIndex]->GetInstancingData(&ZomebieInstancingData[InstanceSaveCount], InstanceMatrixCount++);
				ZomebieInstancingData[InstanceSaveCount].InstancingCount++;
			}

			/*========== 스킨 인스턴싱 렌더 ==========*/
			int iSkinCount = pResourceData->m_SkinObjectList.size();
			for (int iObjCnt = 0; iObjCnt < iSkinCount; iObjCnt++)
			{
				pResourceData->m_SkinObjectList[iObjCnt]->Frame();

				if (iRenderMonsterCount < 3)
				{
					pResourceData->m_SkinObjectList[iObjCnt]->InstancingRender(ZomebieInstancingData[0]);
				}
				else
				{
					for (int RenderCnt = 0; RenderCnt <= InstanceSaveCount; RenderCnt++)
					{
						pResourceData->m_SkinObjectList[iObjCnt]->InstancingRender(ZomebieInstancingData[RenderCnt]);
					}
				}
			}

			/*========== 스킨 인스턴싱 쉐도우 렌더 ==========*/
			//뎁스 스텐실 설정
#ifdef TOOL_MODE
			//m_pCoreContext->OMSetDepthStencilState(INSTANCE_CUR_STATE.SetDssState((int)E_DSS::DSSDisable), 1);
			//m_pCoreContext->RSSetState(INSTANCE_CUR_STATE.SetRrsState((int)E_RRS::RRSSolidCullBack));
			m_pContext->OMSetBlendState(INSTANCE_CUR_STATE.SetBsState((int)E_BS::AlphaBlendEnable), 0, 1);// 0xffffffff);
#else
			m_pContext->OMSetDepthStencilState(m_pShadowStateDepthStencil, 0x01); ;
			m_pContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_STENCIL, 1.0f, 0);	// 깊이 스텐실 버퍼 초기화
			m_pContext->OMSetBlendState(BDxState::g_pAlphaBlend, 0, 1);// 0xffffffff);
#endif
			for (int iObjCnt = 0; iObjCnt < iSkinCount; iObjCnt++)
			{
				if (iRenderMonsterCount < 3)
				{
					pResourceData->m_SkinObjectList[iObjCnt]->InstancingShadowRender(ZomebieInstancingData[0]);
				}
				else
				{
					for (int RenderCnt = 0; RenderCnt <= InstanceSaveCount; RenderCnt++)
					{
						pResourceData->m_SkinObjectList[iObjCnt]->InstancingShadowRender(ZomebieInstancingData[RenderCnt]);
					}
				}
			}
			//뎁스 스텐실 복구
#ifdef TOOL_MODE
			//m_pCoreContext->OMSetDepthStencilState(INSTANCE_CUR_STATE.SetDssState((int)E_DSS::DSSDisable), 1);
			//m_pCoreContext->RSSetState(INSTANCE_CUR_STATE.SetRrsState((int)E_RRS::RRSSolidCullBack));
			m_pContext->OMSetBlendState(INSTANCE_CUR_STATE.SetBsState((int)E_BS::AlphaBlendEnable), 0, 1);// 0xffffffff);
#else
			m_pContext->OMSetDepthStencilState(BDxState::g_pDSSDepthEnable, 0x01); ;
#endif

			/*========== 파티클 렌더 ==========*/
			for (int iCnt = 0; iCnt < iRenderMonsterCount; iCnt++)
			{
				iRenderIndex = m_RenderMonsterList[iCnt];
				m_MonsterObjectList[iRenderIndex]->PostRender();
			}
		}
	}

	return true;
}

LALoadObjectManager::~LALoadObjectManager()
{
}
