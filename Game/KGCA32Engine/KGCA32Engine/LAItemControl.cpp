
//#include "stdafx.h"



#include "LAItemControl.h"

#include "LAResourceManager.h"
#include "LAItem.h"
#include "LAHero.h"

bool LAItemControl::AddWeapon(std::shared_ptr<LAItem>& pItem)
{
	/*================================ 예외 처리 ====================================*/
	if (pItem == NULL)
		return false;

	bool bReturn = false;

	/*================================ Take Sound Paly ====================================*/
#ifdef TOOL_MODE

#else
	if (pItem->m_SondIndexList.size())
	{
		LA_SOUND2_MGR.playeffect(pItem->m_SondIndexList[(int)E_WeaponSound::SNDEFFECT_TAKE]);
	}
#endif

	/*================================ 아이템 검색하여 추가 ====================================*/
	E_WeaponType  eWeaponType = pItem->m_ObjectData.eWeaponType;
	E_InventoryWeaponType  eInvenWeaponType = pItem->GetWeaponType();

	if (m_pWeaponList[(int)eWeaponType])
	{
		/*================================ 착용 아이템 있다면 Inventory 중복 체크 ====================================*/
		LAItem* pSearchItem = GetSearchWeapon(pItem->GetWeaponType());

		//없다면
		if (pSearchItem == NULL)
		{
			//인벤아이템 등록
			m_Inventory.m_pInvenWeaponList.push_back(pItem);

			return false;	//아이템 지우지 않음 
		}

		//있다면 인벤에 누적 후 삭제
		switch (eInvenWeaponType)
		{
		case E_InventoryWeaponType::WEAPON_AK47:
		{
			m_Inventory.dwTotalCount_AK47 += pItem->m_ObjectData.iBulletCount;
		}break;

		case E_InventoryWeaponType::WEAPON_K2:
		{
			m_Inventory.dwTotalCount_K2 += pItem->m_ObjectData.iBulletCount;
		}break;

		case E_InventoryWeaponType::WEAPON_H416:
		{
			m_Inventory.dwTotalCount_HK416 += pItem->m_ObjectData.iBulletCount;
		}break;

		case E_InventoryWeaponType::WEAPON_HANDGUN:
		{
			m_Inventory.dwTotalCount_HANDGUN += pItem->m_ObjectData.iBulletCount;
		}break;

		case E_InventoryWeaponType::WEAPON_KNIFE:
		{
			
		}break;

		case E_InventoryWeaponType::WEAPON_FRAG_GRENADE:
		{
			m_Inventory.dwTotalCount_FRAG_GRENADE += pItem->m_ObjectData.iBulletCount;
		}break;

		case E_InventoryWeaponType::WEAPON_PORTABLE_GRENADE:
		{
			m_Inventory.dwTotalCount_PORTABLE_GRENADE += pItem->m_ObjectData.iBulletCount;
		}break;

		case E_InventoryWeaponType::WEAPON_MEDIC:
		{
			m_Inventory.dwTotalCount_MEDICBOX += pItem->m_ObjectData.iBulletCount;
		}break;
		}

		return true;	//아이템 지움 true
	}
	else
	{
		/*================================ 아이템 없다면 등록 (아이템 지우지 않음) ====================================*/
		switch (eInvenWeaponType)
		{
		case E_InventoryWeaponType::WEAPON_AK47:
		case E_InventoryWeaponType::WEAPON_K2:
		case E_InventoryWeaponType::WEAPON_H416:
		{
			//인벤아이템 등록
			m_Inventory.m_pInvenWeaponList.push_back(pItem);

			//아이템 착용
			m_pWeaponList[(int)E_WeaponType::RIFLE] = pItem.get();

			//현제 무기상태 없다면 등록
			if (m_eCurrentWeaponType == (E_WeaponType)-1)
			{
				SetWeapon(eInvenWeaponType);
			}
		}break;

		case E_InventoryWeaponType::WEAPON_HANDGUN:
		{
			//인벤아이템 등록
			m_Inventory.m_pInvenWeaponList.push_back(pItem);

			//아이템 착용
			m_pWeaponList[(int)E_WeaponType::HANDGUN] = pItem.get();

			//현제 무기상태 없다면 등록
			/*if (m_eCurrentWeaponType == (E_WeaponType)-1)
			{
			SetWeapon(eWeaponType);
			}*/
		}break;

		case E_InventoryWeaponType::WEAPON_KNIFE:
		{
			//인벤아이템 등록
			m_Inventory.m_pInvenWeaponList.push_back(pItem);

			//아이템 착용
			m_pWeaponList[(int)E_WeaponType::KNIFE] = pItem.get();

			//현제 무기상태 없다면 등록
			/*if (m_eCurrentWeaponType == (E_WeaponType)-1)
			{
			SetWeapon(eWeaponType);
			}*/
		}break;

		case E_InventoryWeaponType::WEAPON_FRAG_GRENADE:
		{
			m_Inventory.dwTotalCount_FRAG_GRENADE += pItem->m_ProjectileData->iMaxCount;
			pItem->m_ProjectileData->iCurrentCount = 0; 

			//인벤아이템 등록
			m_Inventory.m_pInvenWeaponList.push_back(pItem);

			//아이템 착용
			m_pWeaponList[(int)E_WeaponType::BOMB] = pItem.get();

			//현제 무기상태 없다면 등록
			if (m_eCurrentWeaponType == (E_WeaponType)-1)
			{
				SetWeapon(eInvenWeaponType);
			}
		}break;

		case E_InventoryWeaponType::WEAPON_PORTABLE_GRENADE:
		{
			m_Inventory.dwTotalCount_PORTABLE_GRENADE += pItem->m_ProjectileData->iMaxCount;
			pItem->m_ProjectileData->iCurrentCount = 0;

			//인벤아이템 등록
			m_Inventory.m_pInvenWeaponList.push_back(pItem);

			//아이템 착용
			m_pWeaponList[(int)E_WeaponType::BOMB] = pItem.get();

			//현제 무기상태 없다면 등록
			if (m_eCurrentWeaponType == (E_WeaponType)-1)
			{
				SetWeapon(eInvenWeaponType);
			}
		}break;

		case E_InventoryWeaponType::WEAPON_MEDIC:
		{
			m_Inventory.dwTotalCount_MEDICBOX += pItem->m_ProjectileData->iMaxCount;
			pItem->m_ProjectileData->iCurrentCount = 0;

			//인벤아이템 등록
			m_Inventory.m_pInvenWeaponList.push_back(pItem);

			//아이템 착용
			m_pWeaponList[(int)E_WeaponType::MEDIC] = pItem.get();

			//현제 무기상태 없다면 등록
			if (m_eCurrentWeaponType == (E_WeaponType)-1)
			{
				SetWeapon(eInvenWeaponType);
			}

		}break;
		}

		return false;	//아이템 지우지 않음 false
	}

	return bReturn;

}

bool LAItemControl::AddArmor(std::shared_ptr<LAItem>& pItem)
{
	/*================================ 예외 처리 ====================================*/
	if (pItem == NULL)
		return false;

	E_InventoryArmorType  eItemType = pItem->GetArmorType();

	/*================================ Take Sound Paly ====================================*/
#ifdef TOOL_MODE

#else
	//TAKE 사운드 호출
	if (pItem->m_SondIndexList.size())
	{
		LA_SOUND2_MGR.playeffect(pItem->m_SondIndexList[(int)E_ArmorSound::SNDEFFECT_ARMOR_TAKE]);
	}
#endif


	/*================================ 아이템 검색하여 추가 ====================================*/
	LAItem* pData = GetSearchArmor(eItemType);
	if (pData)
	{
		/*================================ 아이템 있다면 bullet 누적 후 지움 ====================================*/
		switch (eItemType)
		{
		case E_InventoryArmorType::ARMOR_HEAD:
		{
		
		}break;

		case E_InventoryArmorType::ARMOR_BODY:
		{
			
		}break;

		case E_InventoryArmorType::ARMOR_HAND:
		{
			
		}break;

		case E_InventoryArmorType::ARMOR_FOOT:
		{
			
		}break;

		case E_InventoryArmorType::ARMOR_ACE:
		{

		}break;
		}

		return true;	//아이템 지움 true
	}
	else
	{
		/*================================ 아이템 없다면 등록 (아이템 지우지 않음) ====================================*/
		//인벤아이템 등록
		m_Inventory.m_pInvenArmorList.push_back(pItem);

		SetArmor(eItemType);

		return false;	//아이템 지우지 않음 false
	}

	return false;
}

bool LAItemControl::SetWeapon(E_InventoryWeaponType pItemtype)
{
	/*========================= 예외 처리 ==========================*/
	if (m_Inventory.m_pInvenWeaponList.size() <= 0)
		return false;

	LAItem* pItem = GetSearchWeapon(pItemtype);
	if (pItem == NULL)
		return false;

	/*========================= 셋팅 사운드 플레이 ==========================*/
#ifdef TOOL_MODE

#else
	if (pItem->m_SondIndexList.size())
	{
		LA_SOUND2_MGR.playeffect(pItem->m_SondIndexList[(int)E_WeaponSound::SNDEFFECT_SWAP]);
	}
#endif

	/*========================= 아이템 셋팅 ==========================*/
	bool bReturn = false;
	E_InventoryWeaponType  eItemType = pItem->GetWeaponType();

	switch (eItemType)
	{
	case E_InventoryWeaponType::WEAPON_AK47:
	case E_InventoryWeaponType::WEAPON_K2:
	case E_InventoryWeaponType::WEAPON_H416:
	{
		if (m_pWeaponList[(int)E_WeaponType::RIFLE])
		{
			//아이템이 존재한다면 수량만 추가하고 아이템 지워주기
			m_pWeaponList[(int)E_WeaponType::RIFLE] = NULL;
		}

		m_pWeaponList[(int)E_WeaponType::RIFLE] = pItem;
		m_eCurrentWeaponType = E_WeaponType::RIFLE;
	}break;

	case E_InventoryWeaponType::WEAPON_HANDGUN:
	{
		if (m_pWeaponList[(int)E_WeaponType::HANDGUN])
		{
			//아이템이 존재한다면 수량만 추가하고 아이템 지워주기
			m_pWeaponList[(int)E_WeaponType::HANDGUN] = NULL;
		}

		m_pWeaponList[(int)E_WeaponType::HANDGUN] = pItem;
		m_eCurrentWeaponType = E_WeaponType::HANDGUN;
	}break;

	case E_InventoryWeaponType::WEAPON_KNIFE:
	{
		if (m_pWeaponList[(int)E_WeaponType::KNIFE])
		{
			//아이템이 존재한다면 수량만 추가하고 아이템 지워주기
			m_pWeaponList[(int)E_WeaponType::KNIFE] = NULL;
		}

		m_pWeaponList[(int)E_WeaponType::KNIFE] = pItem;
		m_eCurrentWeaponType = E_WeaponType::KNIFE;
	}break;

	case E_InventoryWeaponType::WEAPON_FRAG_GRENADE:
	case E_InventoryWeaponType::WEAPON_PORTABLE_GRENADE:
	{
		if (m_pWeaponList[(int)E_WeaponType::BOMB])
		{
			//아이템이 존재한다면 수량만 추가하고 아이템 지워주기
			m_pWeaponList[(int)E_WeaponType::BOMB] = NULL;
		}

		m_pWeaponList[(int)E_WeaponType::BOMB] = pItem;
		m_eCurrentWeaponType = E_WeaponType::BOMB;
	}break;

	case E_InventoryWeaponType::WEAPON_MEDIC:
	{
		if (m_pWeaponList[(int)E_WeaponType::MEDIC])
		{
			//아이템이 존재한다면 수량만 추가하고 아이템 지워주기
			m_pWeaponList[(int)E_WeaponType::MEDIC] = NULL;
		}

		m_pWeaponList[(int)E_WeaponType::MEDIC] = pItem;
		m_eCurrentWeaponType = E_WeaponType::MEDIC;
	}break;
	}

	return true;
}

bool LAItemControl::SetArmor(E_InventoryArmorType eArmorType)
{
	/*========================= 예외 처리 ==========================*/
	if (m_Inventory.m_pInvenArmorList.size() <= 0)
		return false;

	LAItem* pItem = GetSearchArmor(eArmorType);
	if (pItem == NULL)
		return false;

	/*========================= 셋팅 사운드 플레이 ==========================*/
#ifdef TOOL_MODE
#else
	if (pItem->m_SondIndexList.size())
	{
		LA_SOUND2_MGR.playeffect(pItem->m_SondIndexList[(int)E_ArmorSound::SNDEFFECT_ARMOR_WEAR]);
	}
#endif

	/*========================= 아이템 셋팅 ==========================*/
	bool bReturn = false;
	E_InventoryArmorType  eItemType = pItem->GetArmorType();

	switch (eItemType)
	{
	case E_InventoryArmorType::ARMOR_HEAD:
	case E_InventoryArmorType::ARMOR_BODY:
	case E_InventoryArmorType::ARMOR_HAND:
	case E_InventoryArmorType::ARMOR_FOOT:
	case E_InventoryArmorType::ARMOR_ACE:
	{
		if (m_pArmorList[(int)eItemType])
		{
			//아이템이 존재한다면 수량만 추가하고 아이템 지워주기
			m_pArmorList[(int)eItemType] = NULL;
		}

		m_pArmorList[(int)eItemType] = pItem;
	}break;
	}
}

void LAItemControl::SetBombRender(bool bResult)
{
	//m_bIsBombRender = bResult;
}

void LAItemControl::SetCurrentWeapon(E_WeaponType eWeapontype)
{
	m_eCurrentWeaponType = eWeapontype;
}

void LAItemControl::SetObjectAgregateData(ObjectTotalDataInfo& totaldata)
{
	m_pObjectAgregateData = &totaldata;
}

void LAItemControl::SetWeaponPosition(int iResouceNumber)
{
	if ((int)m_eCurrentWeaponType != -1)
	{
		LAItem* pData = m_pWeaponList[(int)m_eCurrentWeaponType];

		if (pData)
		{
			switch (m_eCurrentWeaponType)
			{
			case E_WeaponType::RIFLE:
			{
				m_pmatItemPivot = GetRightHandGunMatrixWorld(iResouceNumber);
				pData->SetObjectWorldMatrixToResource(m_pmatItemPivot);

			}break;

			case E_WeaponType::BOMB:
			{
				m_pmatItemPivot = GetLeftHandGunMatrixWorld(iResouceNumber);
				pData->SetObjectWorldMatrixToResource(m_pmatItemPivot);
			}break;

			case E_WeaponType::MEDIC:
			{
				m_pmatItemPivot = GetLeftHandGunMatrixWorld(iResouceNumber);
				pData->SetObjectWorldMatrixToResource(m_pmatItemPivot);
			}break;
			}
		}
	}
}

E_WeaponType LAItemControl::GetCurrentWeaponType()
{
	return m_eCurrentWeaponType;
}

E_InventoryWeaponType LAItemControl::GetCurrentWeaponInvenType(int iWeaponType)
{
	E_InventoryWeaponType type;

	if (m_pWeaponList[iWeaponType] != NULL)
	{
		type = m_pWeaponList[iWeaponType]->GetWeaponType();
	}
	else
	{
		type = (E_InventoryWeaponType)-1;
	}

	return type;
}

LAItem* LAItemControl::GetWeapon(E_WeaponType type)
{
	if (m_pWeaponList[(int)type])
	{
		return m_pWeaponList[(int)type];
	}

	return NULL;
}

LAItem* LAItemControl::GetArmor(E_ArmorType type)
{
	if (m_pArmorList[(int)type])
	{
		return m_pArmorList[(int)type];
	}

	return NULL;
}

TVector3 LAItemControl::GetLeftHandGunPostion(int iResouceNumber)
{
	TVector3 vGunPos;

	LAResource* pResource = RESOURCE_MGR.GetResourceAdress(iResouceNumber);
	if (pResource)
	{
		for (int iCnt = 0; iCnt < (int)pResource->m_BoneObjectList.size(); iCnt++)
		{
			if (pResource->m_BoneObjectList[iCnt]->m_MeshData->wsNodeName.compare(L"LeftHandGun") == 0)
			{
				vGunPos.x = pResource->m_BoneObjectList[iCnt]->m_matCalculation._41;
				vGunPos.y = pResource->m_BoneObjectList[iCnt]->m_matCalculation._42;
				vGunPos.z = pResource->m_BoneObjectList[iCnt]->m_matCalculation._43;

				return vGunPos;
			}
		}
	}

	ZeroMemory(&vGunPos, sizeof(TVector3));

	return vGunPos;
}

TMatrix* LAItemControl::GetLeftHandGunMatrixWorld(int iResouceNumber)
{
	LAResource* pResource = RESOURCE_MGR.GetResourceAdress(iResouceNumber);
	if (pResource)
	{
		for (int iCnt = 0; iCnt < (int)pResource->m_BoneObjectList.size(); iCnt++)
		{
			if (pResource->m_BoneObjectList[iCnt]->m_MeshData->wsNodeName.compare(L"LeftHandGun") == 0)
			{
				return &pResource->m_BoneObjectList[iCnt]->m_matCalculation;
			}
		}
	}

	return NULL;
}

TVector3 LAItemControl::GetRightHandGunPosition(int iResouceNumber)
{
	TVector3 vGunPos;

	LAResource* pResource = RESOURCE_MGR.GetResourceAdress(iResouceNumber);
	if (pResource)
	{
		for (int iCnt = 0; iCnt < (int)pResource->m_BoneObjectList.size(); iCnt++)
		{
			if (pResource->m_BoneObjectList[iCnt]->m_MeshData->wsNodeName.compare(L"RightHandGun") == 0)
			{
				vGunPos.x = pResource->m_BoneObjectList[iCnt]->m_matCalculation._41;
				vGunPos.y = pResource->m_BoneObjectList[iCnt]->m_matCalculation._42;
				vGunPos.z = pResource->m_BoneObjectList[iCnt]->m_matCalculation._43;

				return vGunPos;
			}
		}
	}

	ZeroMemory(&vGunPos, sizeof(TVector3));

	return vGunPos;
}

TMatrix* LAItemControl::GetRightHandGunMatrixWorld(int iResouceNumber)
{
	LAResource* pResource = RESOURCE_MGR.GetResourceAdress(iResouceNumber);
	if (pResource)
	{
		for (int iCnt = 0; iCnt < (int)pResource->m_BoneObjectList.size(); iCnt++)
		{
			if (pResource->m_BoneObjectList[iCnt]->m_MeshData->wsNodeName.compare(L"RightHandGun") == 0)
			{
				return &pResource->m_BoneObjectList[iCnt]->m_matCalculation;
			}
		}
	}

	return NULL;
}

LAItem* LAItemControl::GetCurrentWeapon()
{
	if (m_pWeaponList[(int)m_eCurrentWeaponType])
	{
		return m_pWeaponList[(int)m_eCurrentWeaponType];
	}

	return NULL;
}

LAItem* LAItemControl::GetSearchWeapon(E_InventoryWeaponType type)
{
	int iItemSize = m_Inventory.m_pInvenWeaponList.size();
	if (iItemSize <= 0)
		return NULL;

	for (int iCnt = 0; iCnt < iItemSize; iCnt++)
	{
		if (m_Inventory.m_pInvenWeaponList[iCnt]->GetWeaponType() == type)
		{
			return m_Inventory.m_pInvenWeaponList[iCnt].get();
		}
	}

	return NULL;
}

LAItem* LAItemControl::GetSearchArmor(E_InventoryArmorType type)
{
	int iItemSize = m_Inventory.m_pInvenArmorList.size();
	if (iItemSize <= 0)
		return NULL;

	for (int iCnt = 0; iCnt < iItemSize; iCnt++)
	{
		if (m_Inventory.m_pInvenArmorList[iCnt]->GetArmorType() == type)
		{
			return m_Inventory.m_pInvenArmorList[iCnt].get();
		}
	}

	return NULL;
}

bool LAItemControl::UpdateShot()
{
	switch (m_eCurrentWeaponType)
	{
	case E_WeaponType::RIFLE:
	{
		if (m_pWeaponList[(int)E_WeaponType::RIFLE])
		{
			if (m_pWeaponList[(int)E_WeaponType::RIFLE]->m_ProjectileData->iCurrentCount <= 0)
			{
				//m_bZeroBulletCount = true;
				//if (m_pWeaponList[(int)E_WeaponType::RIFLE]->m_ProjectileData->iCurrentCount < 0)
				{
					return false;
				}
			}
			m_pWeaponList[(int)E_WeaponType::RIFLE]->m_ProjectileData->iCurrentCount -= 1;

			/*====================== 파티클 시스템 호출 ======================*/
			m_pWeaponList[(int)E_WeaponType::RIFLE]->m_ParticleRender->m_bHeroShot = true;
			m_pWeaponList[(int)E_WeaponType::RIFLE]->m_ParticleRender->Shot();

			return true;
		}
	}break;

	case E_WeaponType::HANDGUN:
	{

	}break;

	case E_WeaponType::BOMB:
	{
		if (m_pWeaponList[(int)E_WeaponType::BOMB])
		{
			if (m_pWeaponList[(int)E_WeaponType::BOMB]->m_ProjectileData->iCurrentCount <= 0)
			{
				return false;
			}

			m_pWeaponList[(int)E_WeaponType::BOMB]->m_ProjectileData->iCurrentCount -= 1;
			return true;
		}
	}break;

	case E_WeaponType::MEDIC:
	{
		if (m_pWeaponList[(int)E_WeaponType::MEDIC])
		{
			if (m_pWeaponList[(int)E_WeaponType::MEDIC]->m_ProjectileData->iCurrentCount <= 0)
			{
				return false;
			}

			m_pWeaponList[(int)E_WeaponType::MEDIC]->m_ProjectileData->iCurrentCount -= 1;
			m_bIsMedicRender = false;

			return true;
		}
	}break;
}

	return false;
}


bool LAItemControl::UpdateBullet(int ResourceNumber, int iCurrentMotion, float fElpaseFrame, LATopObject* pTopObject)
{
	//현제 착용된 무기 체크 샷체크
	switch (iCurrentMotion)
	{
	case (int)E_CharacterMotion::RIFLE_RELOAD:
	{
		/*============== Rifle Reload Sound ==============*/
		if (CheckPositionTick(5, m_bisCheckTick[(int)E_CheckAniPostionTick::Rifle_Reload_Sound], ResourceNumber, iCurrentMotion, fElpaseFrame))
		{

#ifdef TOOL_MODE
#else
			if (m_pWeaponList[(int)E_WeaponType::RIFLE]->m_pSkillData)
			{
				if (m_pWeaponList[(int)E_WeaponType::RIFLE]->m_pSkillData->m_SkillSoundIndex.size())
				{
					//ReLoad 사운드출력
					LA_SOUND2_MGR.playeffect(m_pWeaponList[(int)GetCurrentWeaponType()]->m_SondIndexList[(int)E_WeaponSound::SNDEFFECT_RELOAD]);
				}
			}
#endif

		}

		/*============== Rifle Reload ==============*/
		if (CheckPositionTick(60, m_bisCheckTick[(int)E_CheckAniPostionTick::Rifle_Reload], ResourceNumber, iCurrentMotion, fElpaseFrame))
		{
			BulletLeRoading();
		}
	}break;

	case (int)E_CharacterMotion::RIFLE_ACTION:
	{
		/*============== Rilfe Shot ==============*/
		if (CheckPositionTick(20, m_bisCheckTick[(int)E_CheckAniPostionTick::Rifle_Shot], ResourceNumber, iCurrentMotion, fElpaseFrame))
		{
			UpdateShot();

			/*====================== 임시 임펙트 사운드 플레이 ======================*/
			if (m_pWeaponList[(int)E_WeaponType::RIFLE]->m_pSkillData)
			{
				if (m_pWeaponList[(int)E_WeaponType::RIFLE]->m_pSkillData->m_SkillSoundIndex.size())
				{
#ifdef TOOL_MODE
#else
					int iSoundIndex = m_pWeaponList[(int)E_WeaponType::RIFLE]->m_pSkillData->m_SkillSoundIndex[0];
					LA_SOUND2_MGR.playeffect(iSoundIndex);
#endif
				}
			}
		}
	}break;

	case (int)E_CharacterMotion::BOMB_RELOAD:
	{
		/*============== Bomb Reload Sound ==============*/
		if (CheckPositionTick(5, m_bisCheckTick[(int)E_CheckAniPostionTick::Bomb_Reload_Sound], ResourceNumber, iCurrentMotion, fElpaseFrame))
		{

#ifdef TOOL_MODE
#else
			if (m_pWeaponList[(int)E_WeaponType::BOMB]->m_pSkillData)
			{
				if (m_pWeaponList[(int)E_WeaponType::BOMB]->m_pSkillData->m_SkillSoundIndex.size())
				{
					//ReLoad 사운드출력
					LA_SOUND2_MGR.playeffect(m_pWeaponList[(int)GetCurrentWeaponType()]->m_SondIndexList[(int)E_WeaponSound::SNDEFFECT_RELOAD]);
				}
			}
#endif

		}

		/*============== Bomb Reload ==============*/
		if (CheckPositionTick(60, m_bisCheckTick[(int)E_CheckAniPostionTick::Bomb_Reload], ResourceNumber, iCurrentMotion, fElpaseFrame))
		{
			BulletLeRoading();
		}
	}break;

	case (int)E_CharacterMotion::BOMB_ACTION:
	{
		/*============== Bomb Shot ==============*/
		if (CheckPositionTick(50, m_bisCheckTick[(int)E_CheckAniPostionTick::Bomb_Shot], ResourceNumber, iCurrentMotion, fElpaseFrame))
		{
			UpdateShot();

#ifdef TOOL_MODE
#else
			if (m_pWeaponList[(int)E_WeaponType::BOMB]->m_pSkillData)
			{
				if (m_pWeaponList[(int)E_WeaponType::BOMB]->m_pSkillData->m_SkillSoundIndex.size())
				{
					m_pWeaponList[(int)E_WeaponType::BOMB]->m_ParticleRender->ReloadGrenade(1, g_pMainCamera->m_CameraBoundingBox.m_vCenter, g_pMainCamera->m_vLook);
					/*====================== 임시 임펙트 사운드 플레이 ======================*/
					LA_SOUND2_MGR.playeffect(m_pWeaponList[(int)E_WeaponType::BOMB]->m_pSkillData->m_SkillSoundIndex[(int)E_SkillTrapSound::SNDEFFECT_BOMB_ATTACK1]);
					m_bIsFireIntheHole = true;
					//m_bZeroBulletCount = true;
				}
			}
#endif
		}
	}break;

	case (int)E_CharacterMotion::MEDIC_RELOAD:
	{
		/*============== Bomb Reload Sound ==============*/
		if (CheckPositionTick(5, m_bisCheckTick[(int)E_CheckAniPostionTick::Medic_Reload_Sound], ResourceNumber, iCurrentMotion, fElpaseFrame))
		{
			
#ifdef TOOL_MODE
#else
			if (m_pWeaponList[(int)E_WeaponType::MEDIC]->m_pSkillData)
			{
				if (m_pWeaponList[(int)E_WeaponType::MEDIC]->m_pSkillData->m_SkillSoundIndex.size())
				{
					//ReLoad 사운드출력
					LA_SOUND2_MGR.playeffect(m_pWeaponList[(int)GetCurrentWeaponType()]->m_SondIndexList[(int)E_WeaponSound::SNDEFFECT_RELOAD]);
				}
			}
#endif
		}

		/*============== Bomb Reload ==============*/
		if (CheckPositionTick(60, m_bisCheckTick[(int)E_CheckAniPostionTick::Medic_Reload], ResourceNumber, iCurrentMotion, fElpaseFrame))
		{
			BulletLeRoading();
		}
	}break;

	case (int)E_CharacterMotion::MEDIC_ACTION:
	{
		/*============== Bomb Shot ==============*/
		if (CheckPositionTick(50, m_bisCheckTick[(int)E_CheckAniPostionTick::Medic_shot], ResourceNumber, iCurrentMotion, fElpaseFrame))
		{
			UpdateShot();

			if (pTopObject)
			{
				switch (pTopObject->m_ObjectData.eObjectType)
				{
				case E_LoadTableType::CHARACTER:
				{
					LAHero* pData = (LAHero*)pTopObject;
					pData->SetHealPower(m_pWeaponList[(int)E_WeaponType::MEDIC]->m_pSkillData->m_SkillTableLoadData.fHealPower);


				}break;

				case E_LoadTableType::MONSTER:
				{
					//미사용
				}break;
				}
				
			}


#ifdef TOOL_MODE
#else
			/*====================== 임펙트 ======================*/
			m_pWeaponList[(int)E_WeaponType::MEDIC]->m_ParticleRender->Shot();


			if (m_pWeaponList[(int)E_WeaponType::MEDIC]->m_pSkillData)
			{
				if (m_pWeaponList[(int)E_WeaponType::MEDIC]->m_pSkillData->m_SkillSoundIndex.size())
				{
					/*====================== 임시 임펙트 사운드 플레이 ======================*/
					LA_SOUND2_MGR.playeffect(m_pWeaponList[(int)E_WeaponType::MEDIC]->m_pSkillData->m_SkillSoundIndex[(int)E_SkillBuffSound::SNDEFFECT_ATTACK]);
				}
			}
#endif
		}
	}break;
	}



	return false;
}


bool LAItemControl::CheckInvenBullet(E_WeaponType type)
{
	if (m_pWeaponList[(int)type])
	{
		E_InventoryWeaponType bulletType = m_pWeaponList[(int)type]->GetWeaponType();

		switch (bulletType)
		{
		case E_InventoryWeaponType::WEAPON_AK47:
		{
			if (m_Inventory.dwTotalCount_AK47 > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}break;

		case E_InventoryWeaponType::WEAPON_K2:
		{
			if (m_Inventory.dwTotalCount_K2 > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}break;

		case E_InventoryWeaponType::WEAPON_H416:
		{
			if (m_Inventory.dwTotalCount_HK416 > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}break;

		case E_InventoryWeaponType::WEAPON_HANDGUN:
		{
			if (m_Inventory.dwTotalCount_HANDGUN > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}break;

		case E_InventoryWeaponType::WEAPON_FRAG_GRENADE:
		{
			if (m_Inventory.dwTotalCount_FRAG_GRENADE > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}break;

		case E_InventoryWeaponType::WEAPON_PORTABLE_GRENADE:
		{
			if (m_Inventory.dwTotalCount_PORTABLE_GRENADE > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}break;

		case E_InventoryWeaponType::WEAPON_MEDIC:
		{
			if (m_Inventory.dwTotalCount_MEDICBOX > 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}break;
		}
		
	}

	return false;
}

bool LAItemControl::CheckBullet(E_WeaponType type)
{
	if (m_pWeaponList[(int)type])
	{
		if (m_pWeaponList[(int)type]->m_ProjectileData->iCurrentCount > 0) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

bool LAItemControl::BulletLeRoading()
{
	switch (m_eCurrentWeaponType)
	{
	case E_WeaponType::RIFLE:
	{
		E_InventoryWeaponType itemtype = m_pWeaponList[(int)E_WeaponType::RIFLE]->GetWeaponType();
		DWORD* pInvenBulletTotalCount = NULL;

		switch (itemtype)
		{
		case E_InventoryWeaponType::WEAPON_AK47:
		{
			pInvenBulletTotalCount = &m_Inventory.dwTotalCount_AK47;
		}break;

		case E_InventoryWeaponType::WEAPON_K2:
		{
			pInvenBulletTotalCount = &m_Inventory.dwTotalCount_K2;
		}break;

		case E_InventoryWeaponType::WEAPON_H416:
		{
			pInvenBulletTotalCount = &m_Inventory.dwTotalCount_HK416;
		}break;
		}

		if (*pInvenBulletTotalCount > 0)
		{
			//총에 넣을수 있는 총알 수량 
			int iReLoadCount = m_pWeaponList[(int)E_WeaponType::RIFLE]->m_ObjectData.iAbilityReloadMax - m_pWeaponList[(int)E_WeaponType::RIFLE]->m_ProjectileData->iCurrentCount;
			if (iReLoadCount <= 0)
			{
				return false;
			}

			//인벤총알에에서 가져올수 있는 양을 체크
			if (*pInvenBulletTotalCount >= iReLoadCount)
			{
				*pInvenBulletTotalCount -= iReLoadCount;
				m_pWeaponList[(int)E_WeaponType::RIFLE]->m_ProjectileData->iCurrentCount += iReLoadCount;
			}
			else
			{
				iReLoadCount = *pInvenBulletTotalCount;
				*pInvenBulletTotalCount -= iReLoadCount;
				m_pWeaponList[(int)E_WeaponType::RIFLE]->m_ProjectileData->iCurrentCount += iReLoadCount;
			}

			/*=============================== 파티클 시스템 발사체 업데이트 =================================*/
			m_pWeaponList[(int)E_WeaponType::RIFLE]->m_ParticleRender->ReloadBullet(iReLoadCount);
			return true;
		}
		else
		{
			return false;
		}
	}break;

	case E_WeaponType::HANDGUN:
	{
		E_InventoryWeaponType itemtype = m_pWeaponList[(int)E_WeaponType::HANDGUN]->GetWeaponType();
		DWORD* pInvenBulletTotalCount = NULL;

		switch (itemtype)
		{
		case E_InventoryWeaponType::WEAPON_HANDGUN:
		{
			pInvenBulletTotalCount = &m_Inventory.dwTotalCount_HANDGUN;
		}break;
		}

		if (*pInvenBulletTotalCount > 0)
		{
			//총에 넣을수 있는 총알 수량 
			int iReLoadCount = m_pWeaponList[(int)E_WeaponType::HANDGUN]->m_ObjectData.iAbilityReloadMax - m_pWeaponList[(int)E_WeaponType::HANDGUN]->m_ProjectileData->iCurrentCount;
			if (iReLoadCount <= 0)
			{
				return false;
			}

			//인벤총알에에서 가져올수 있는 양을 체크
			if (*pInvenBulletTotalCount >= iReLoadCount)
			{
				*pInvenBulletTotalCount -= iReLoadCount;
				m_pWeaponList[(int)E_WeaponType::HANDGUN]->m_ProjectileData->iCurrentCount += iReLoadCount;
			}
			else
			{
				iReLoadCount = *pInvenBulletTotalCount;
				*pInvenBulletTotalCount -= iReLoadCount;
				m_pWeaponList[(int)E_WeaponType::HANDGUN]->m_ProjectileData->iCurrentCount += iReLoadCount;
			}

			/*=============================== 파티클 시스템 발사체 업데이트 =================================*/
			m_pWeaponList[(int)E_WeaponType::HANDGUN]->m_ParticleRender->ReloadBullet(iReLoadCount);
			return true;
		}
		else
		{
			return false;
		}
	}break;

	case E_WeaponType::BOMB:
	{
		E_InventoryWeaponType itemtype = m_pWeaponList[(int)E_WeaponType::BOMB]->GetWeaponType();
		DWORD* pInvenBulletTotalCount = NULL;

		switch (itemtype)
		{
		case E_InventoryWeaponType::WEAPON_FRAG_GRENADE:
		{
			pInvenBulletTotalCount = &m_Inventory.dwTotalCount_FRAG_GRENADE;
		}break;

		case E_InventoryWeaponType::WEAPON_PORTABLE_GRENADE:
		{
			pInvenBulletTotalCount = &m_Inventory.dwTotalCount_PORTABLE_GRENADE;
		}break;
		}

		if (*pInvenBulletTotalCount > 0)
		{
			*pInvenBulletTotalCount -= 1;
			m_pWeaponList[(int)E_WeaponType::BOMB]->m_ProjectileData->iCurrentCount = 1;

			//수류탄 렌더 체크
			m_bIsBombRender = true;
			return true;
		}
		else
		{
			return false;
		}
	}break;

	case E_WeaponType::MEDIC:
	{
		E_InventoryWeaponType itemtype = m_pWeaponList[(int)E_WeaponType::MEDIC]->GetWeaponType();
		DWORD* pInvenBulletTotalCount = NULL;

		switch (itemtype)
		{
		case E_InventoryWeaponType::WEAPON_MEDIC:
		{
			pInvenBulletTotalCount = &m_Inventory.dwTotalCount_MEDICBOX;
		}break;
		}

		if (*pInvenBulletTotalCount > 0)
		{
			*pInvenBulletTotalCount -= 1;
			m_pWeaponList[(int)E_WeaponType::MEDIC]->m_ProjectileData->iCurrentCount = 1;

			//수류탄 렌더 체크
			m_bIsMedicRender = true;
			return true;
		}
		else
		{
			return false;
		}
	}break;
	}

	return true;
}

bool LAItemControl::CheckPositionTick(int iPercentage, bool& bisReCheck, int iResourceNumber, int iCurrentMostion, float fElapseFrame)
{
	LAResource* pData = RESOURCE_MGR.GetResourceAdress(iResourceNumber);

	if (pData->m_AnimationList.size())
	{
		SceneData& rData = pData->m_AnimationList[iCurrentMostion];

		int TotalTick = rData.iLastFrame - rData.iFirstFrame;
		int DiviceTick = TotalTick * iPercentage / 100;

		/*std::wstring check;
		check = std::to_wstring(fElapseFrame);
		check += L"\n";
		OutputDebugString(check.c_str());*/
	
		if (((int)fElapseFrame == DiviceTick) && (bisReCheck == false))
		{
			bisReCheck = true;

			return true;
		}
		else
		{
			return false;
		}
		
	}

	return false;
}

void LAItemControl::ResetCheckTick()
{
	for (int iCnt = 0; iCnt < CheckAniSize; iCnt++)
	{
		m_bisCheckTick[iCnt] = false;
	}
}

void LAItemControl::SetMatrix(TMatrix* pWorldMatrix, TMatrix* pViewMatrix, TMatrix* pProjectionMatrix)
{
	//추가 사항 입력
	//무기 
	for (int iCnt = 0; iCnt < 5; iCnt++)
	{
		if (m_pWeaponList[iCnt])
		{
			LAItem* pData = m_pWeaponList[iCnt];
			if (pData)
			{
				pData->SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);
			}
		}
	}

	//방어구
	for (int iCnt = 0; iCnt < 5; iCnt++)
	{
		if (m_pArmorList[iCnt])
		{
			LAItem* pData = m_pArmorList[iCnt]->get();
			if (pData)
			{
				pData->SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);
			}
		}
	}
}

bool LAItemControl::Frame()
{

#ifdef TOOL_MODE

#else

	//무기 프레임
	for (int iCnt = 0; iCnt < 5; iCnt++)
	{
		if (m_pWeaponList[iCnt])
		{
			m_pWeaponList[iCnt]->Frame();

			//ParticleRender 계속 적으로 프레임을 돌려할 함수
			if (m_pWeaponList[iCnt]->m_ParticleRender)
			{
				m_pWeaponList[iCnt]->m_ParticleRender->ParticleFrame();
			}

		}
	}

	//방어구
	for (int iCnt = 0; iCnt < 5; iCnt++)
	{
		if (m_pArmorList[iCnt])
		{
			LAItem* pData = m_pArmorList[iCnt]->get();
			if (pData)
			{
				pData->Frame();
			}
		}
	}

#endif

	return true;
}

bool LAItemControl::Render()
{
	//무기 
	if (m_pWeaponList[(int)E_WeaponType::RIFLE])
	{
		LAItem* pData = m_pWeaponList[(int)E_WeaponType::RIFLE];

		if (m_eCurrentWeaponType == E_WeaponType::RIFLE)
		{
			if (pData)
			{
				//총기 핸드 더미에 연결 
				TMatrix pmatgun = *m_pmatItemPivot;

				TVector3 vCWMscale;
				TQuaternion qCWMrotation;
				TMatrix matConvertRotation;
				TMatrix matNewRotationY;
				TMatrix matNewRotationX;
				TVector3 vCWMtransration;
				TMatrix matCalculation;

				D3DXMatrixDecompose(&vCWMscale, &qCWMrotation, &vCWMtransration, &pmatgun);
				vCWMscale.x = pData->m_matObjectWorld._11;
				vCWMscale.y = pData->m_matObjectWorld._22;
				vCWMscale.z = pData->m_matObjectWorld._33;
				

				D3DXMatrixRotationQuaternion(&matConvertRotation, &qCWMrotation);
				//TMatrixRotationY(&matNewRotationY, 0.2);	//morden
				D3DXMatrixRotationY(&matNewRotationY, 1.7);		//ak47
				matConvertRotation *= matNewRotationY;


				D3DXMatrixScaling(&matCalculation, vCWMscale.x, vCWMscale.y, vCWMscale.z);
				D3DXMatrixMultiply(&matCalculation, &matCalculation, &matConvertRotation);

				matCalculation._41 = vCWMtransration.x;
				matCalculation._42 = vCWMtransration.y + 0.05f;
				//matCalculation._42 = vCWMtransration.y;	//morden
				matCalculation._43 = vCWMtransration.z;

				pData->m_matObjectWorld = matCalculation;

				pData->Render();
			}
		}

#ifdef TOOL_MODE
#else
		/*============================== Rifle bullet ParticleRender ====================================*/
		pData->m_ParticleRender->SetParticlePos(pData->GetParticlePositon());
		pData->m_ParticleRender->SetParticleDir(g_pMainCamera->m_vLook);
		pData->m_ParticleRender->DrawParticle();
#endif

	}


	if (m_pWeaponList[(int)E_WeaponType::BOMB])
	{
		if (((m_bIsBombRender) && (m_eCurrentWeaponType == E_WeaponType::BOMB)) ||
			m_bIsFireIntheHole == true)
		{
			if ((m_pWeaponList[(int)E_WeaponType::BOMB]) && (m_bIsFireIntheHole == false))
			{

				LAItem* pData = m_pWeaponList[(int)E_WeaponType::BOMB];
				if (pData)
				{
					TMatrix pmatgun = *m_pmatItemPivot;

					TVector3 vCWMscale;
					TQuaternion qCWMrotation;
					TMatrix matConvertRotation;
					TMatrix matNewRotationY;
					TMatrix matNewRotationX;
					TVector3 vCWMtransration;
					TMatrix matCalculation;

					D3DXMatrixDecompose(&vCWMscale, &qCWMrotation, &vCWMtransration, &pmatgun);
					vCWMscale.x = pData->m_matObjectWorld._11 * 0.3;
					vCWMscale.y = pData->m_matObjectWorld._22 * 0.3;
					vCWMscale.z = pData->m_matObjectWorld._33 * 0.3;

					D3DXMatrixRotationQuaternion(&matConvertRotation, &qCWMrotation);

					D3DXMatrixScaling(&matCalculation, vCWMscale.x, vCWMscale.y, vCWMscale.z);
					D3DXMatrixMultiply(&matCalculation, &matCalculation, &matConvertRotation);


					matCalculation._41 = vCWMtransration.x;
					matCalculation._42 = vCWMtransration.y;// +0.05f;
					matCalculation._43 = vCWMtransration.z;


					pData->m_matObjectWorld = matCalculation;
				}
			}
			else
			{
				if (m_pWeaponList[(int)E_WeaponType::BOMB]->m_ParticleRender->GetCheckBomb() == false)
				{
					TVector3 vGrenadePos = m_pWeaponList[(int)E_WeaponType::BOMB]->m_ParticleRender->GetGrenadePosition();
					//m_pWeaponList[(int)E_WeaponType::BOMB]->SetPosition(vGrenadePos.x, vGrenadePos.y, vGrenadePos.z);

					m_pWeaponList[(int)E_WeaponType::BOMB]->m_matObjectWorld._41 = vGrenadePos.x;
					m_pWeaponList[(int)E_WeaponType::BOMB]->m_matObjectWorld._42 = vGrenadePos.y;
					m_pWeaponList[(int)E_WeaponType::BOMB]->m_matObjectWorld._43 = vGrenadePos.z;
				}
			}

			
			if (m_bIsFireIntheHole)
			{
				if (m_pWeaponList[(int)E_WeaponType::BOMB]->m_ParticleRender->GetCheckBomb() == true)
				{
#ifdef TOOL_MODE
#else
					/*============================== Bomb bullet Sound Play ====================================*/
					LA_SOUND2_MGR.playeffect(m_pWeaponList[(int)E_WeaponType::BOMB]->m_pSkillData->m_SkillSoundIndex[(int)E_SkillTrapSound::SNDEFFECT_BOMB_ATTACK2]);
					m_bIsBombRender = false;
					m_bIsFireIntheHole = false;
#endif
				}
			}
			m_pWeaponList[(int)E_WeaponType::BOMB]->Render();

		}	//m_bIsBombRender
			/*============================== Bomb bullet Particle Render ====================================*/
		m_pWeaponList[(int)E_WeaponType::BOMB]->m_ParticleRender->DrawParticle();
	}

	if (m_pWeaponList[(int)E_WeaponType::MEDIC])
	{
		if ((m_bIsMedicRender) && (m_eCurrentWeaponType == E_WeaponType::MEDIC))
		{
			if (m_pWeaponList[(int)E_WeaponType::MEDIC])
			{
				LAItem* pData = m_pWeaponList[(int)E_WeaponType::MEDIC];
				if (pData)
				{
					TMatrix pmatgun = *m_pmatItemPivot;

					TVector3 vCWMscale;
					TQuaternion qCWMrotation;
					TMatrix matConvertRotation;
					TMatrix matNewRotationY;
					TMatrix matNewRotationX;
					TVector3 vCWMtransration;
					TMatrix matCalculation;

					D3DXMatrixDecompose(&vCWMscale, &qCWMrotation, &vCWMtransration, &pmatgun);
					vCWMscale.x = pData->m_matObjectWorld._11 * 0.3;
					vCWMscale.y = pData->m_matObjectWorld._22 * 0.3;
					vCWMscale.z = pData->m_matObjectWorld._33 * 0.3;

					D3DXMatrixRotationQuaternion(&matConvertRotation, &qCWMrotation);

					D3DXMatrixScaling(&matCalculation, vCWMscale.x, vCWMscale.y, vCWMscale.z);
					D3DXMatrixMultiply(&matCalculation, &matCalculation, &matConvertRotation);


					matCalculation._41 = vCWMtransration.x;
					matCalculation._42 = vCWMtransration.y;// +0.05f;
					matCalculation._43 = vCWMtransration.z;


					pData->m_matObjectWorld = matCalculation;
				}

			m_pWeaponList[(int)E_WeaponType::MEDIC]->Render();
			}

		}	//m_bIsBombRender
			/*============================== Bomb bullet Particle Render ====================================*/
		m_pWeaponList[(int)E_WeaponType::MEDIC]->m_ParticleRender->DrawParticle();
	}

	//방어구
	for (int iCnt = 0; iCnt < 5; iCnt++)
	{
		if (m_pArmorList[iCnt])
		{
			LAItem* pData = m_pArmorList[iCnt]->get();
			if (pData)
			{
				pData->Render();
			}
		}
	}

	return true;
}

bool LAItemControl::RenderShadow()
{
	//무기 
	if (m_pWeaponList[(int)E_WeaponType::RIFLE])
	{
		LAItem* pData = m_pWeaponList[(int)E_WeaponType::RIFLE];

		if (m_eCurrentWeaponType == E_WeaponType::RIFLE)
		{
			if (pData)
			{
				////총기 핸드 더미에 연결 
				//TMatrix pmatgun = *m_pmatItemPivot;

				//TVector3 vCWMscale;
				//TQuaternion qCWMrotation;
				//TMatrix matConvertRotation;
				//TMatrix matNewRotationY;
				//TMatrix matNewRotationX;
				//TVector3 vCWMtransration;
				//TMatrix matCalculation;

				//TMatrixDecompose(&vCWMscale, &qCWMrotation, &vCWMtransration, &pmatgun);
				//vCWMscale.x = pData->m_matObjectWorld._11;
				//vCWMscale.y = pData->m_matObjectWorld._22;
				//vCWMscale.z = pData->m_matObjectWorld._33;


				//TMatrixRotationQuaternion(&matConvertRotation, &qCWMrotation);
				////TMatrixRotationY(&matNewRotationY, 0.2);	//morden
				//TMatrixRotationY(&matNewRotationY, 1.7);		//ak47
				//matConvertRotation *= matNewRotationY;


				//TMatrixScaling(&matCalculation, vCWMscale.x, vCWMscale.y, vCWMscale.z);
				//TMatrixMultiply(&matCalculation, &matCalculation, &matConvertRotation);

				//matCalculation._41 = vCWMtransration.x;
				//matCalculation._42 = vCWMtransration.y + 0.05f;
				////matCalculation._42 = vCWMtransration.y;	//morden
				//matCalculation._43 = vCWMtransration.z;

				//pData->m_matObjectWorld = matCalculation;

				pData->RenderShadow();
			}
		}
	}

	return true;
}

bool LAItemControl::Release()
{
	m_pObjectAgregateData = NULL;
	m_pmatItemPivot = NULL;
	for (int iCnt = 0; iCnt < MAX_WEAPON_SIZE; iCnt++)
	{
		if (m_pWeaponList[iCnt])
		{
			//m_pWeaponList[iCnt]->Release();
			m_pWeaponList[iCnt] = NULL;
		}
	}
	for (int iCnt = 0; iCnt < MAX_ARMOR_SIZE; iCnt++)
	{
		if (m_pArmorList[iCnt])
		{
			//m_pArmorList[iCnt]->Release();
			m_pArmorList[iCnt] = NULL;
		}
	}

	for (int iCnt = 0; iCnt < m_Inventory.m_pInvenWeaponList.size(); iCnt++)
	{
		m_Inventory.m_pInvenWeaponList[iCnt]->Release();
	}
	m_Inventory.m_pInvenWeaponList.clear();

	for (int iCnt = 0; iCnt < m_Inventory.m_pInvenArmorList.size(); iCnt++)
	{
		m_Inventory.m_pInvenArmorList[iCnt]->Release();
	}
	m_Inventory.m_pInvenArmorList.clear();

	return true;
}

LAItemControl::~LAItemControl()
{
}
