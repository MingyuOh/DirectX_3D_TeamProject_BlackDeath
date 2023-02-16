
//#include "stdafx.h"


#include "LAHero.h"

#include "CoreFuncLink.h"
#include "CoreInputLink.h"

#include "LAItem.h"
#include "LALoadObjectManager.h"
#include "LAResourceManager.h"

static float g_HeroTotalSpeed;


bool LAHero::CheckIsHeroLife()
{
	return m_bIsObjectLife;
}

void LAHero::ResetCheck()
{
	LATopObject::ResetCheck();

	m_bIsReLoad = false;
	m_pItemControl->ResetCheckTick();
	m_eSetWillWeapon = (E_WeaponType) - 1;
}

bool LAHero::CheckInItem()
{
	return m_bIsInItem;
}

void LAHero::SetDirection(TVector3* vData)
{
#ifdef TOOL_MODE

	
#else
	//m_vObjectLook = g_pMainCamera->m_vLook;
	//m_vObjectSide = g_pMainCamera->m_vSide;
	if (vData != NULL)
	{
	Theta = acos(D3DXVec3Dot(&m_vObjectLook, vData));
	return;
	}
#endif
}

void LAHero::SetRotation(float x, float y, float z)
{
	//x값만 저장
	m_fYaw = x;// +Theta;
	m_fRoll = z;
}

void LAHero::SetHeroHpProjectileData()
{
	m_HeroHpProjectileData = new LAObjectUIDataInfo;

	//처음 맥스hp로 셋팅
	m_ObjectAgregateData.iCurrentHp = m_ObjectAgregateData.iTotalMaxHp;
	m_HeroHpProjectileData->iCurrentCount = m_ObjectAgregateData.iCurrentHp;
	m_HeroHpProjectileData->iMaxCount = m_ObjectAgregateData.iTotalMaxHp;

	m_HeroHpProjectileData->fHeroHp = m_ObjectData.iHp;
	m_HeroHpProjectileData->fHeroTotalHp = m_ObjectAgregateData.iCurrentHp;

	m_HeroHpProjectileData->fHeroDef = m_ObjectData.fDef;
	m_HeroHpProjectileData->fHeroTotalDef = m_ObjectAgregateData.fTotalDef;

	m_HeroHpProjectileData->fHeroSpeed = m_ObjectData.fRunSpeed;
	m_HeroHpProjectileData->fHeroTotalSpeed = m_ObjectAgregateData.fTotalMoveSpeed;;

	m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA] = m_HeroHpProjectileData;
}

bool LAHero::SetObjectABilityData()
{
	//Total 값 초기화
	m_ObjectAgregateData.iTotalMaxHp = 0;
	m_ObjectAgregateData.fTotalDef = 0.0f;
	m_ObjectAgregateData.fTotalMoveSpeed = 0.0f;

	m_ObjectAgregateData.fTotalAddAttackPower = 0.0f;
	m_ObjectAgregateData.fTotalAddAttackSpeed = 0.0f;
	m_ObjectAgregateData.fTotalAddRunSpeed = 0.0f;

	//누적 Hero
	m_ObjectAgregateData.iTotalMaxHp += m_ObjectData.iHp;
	m_ObjectAgregateData.fTotalDef += m_ObjectData.fDef;
	m_ObjectAgregateData.fTotalMoveSpeed += m_ObjectData.fRunSpeed;

	//Weapon

	//Armor
	for (int iCnt = 0; iCnt < MAX_ARMOR_SIZE; iCnt++)
	{
		if (m_pItemControl)
		{
			if (m_pItemControl->m_pArmorList[iCnt])
			{
				switch (m_pItemControl->m_pArmorList[iCnt]->m_ObjectData.e_ArmorAbilityType)
				{
				case E_ArmorAbilityType::ATTACK_POWER:
				{
					m_ObjectAgregateData.fTotalAddAttackPower += m_pItemControl->m_pArmorList[iCnt]->m_ObjectData.fObjectWeight;
				}break;

				case E_ArmorAbilityType::DEFENCE:
				{
					m_ObjectAgregateData.fTotalDef += m_pItemControl->m_pArmorList[iCnt]->m_ObjectData.fObjectWeight;
				}break;

				case E_ArmorAbilityType::HP:
				{
					m_ObjectAgregateData.iTotalMaxHp += m_pItemControl->m_pArmorList[iCnt]->m_ObjectData.fObjectWeight;
				}break;
				
				case E_ArmorAbilityType::ATTACK_SPEED:
				{
					m_ObjectAgregateData.fTotalAddAttackSpeed += m_pItemControl->m_pArmorList[iCnt]->m_ObjectData.fObjectWeight;
				}break;

				case E_ArmorAbilityType::RUN_SPEED:
				{
					m_ObjectAgregateData.fTotalMoveSpeed += m_pItemControl->m_pArmorList[iCnt]->m_ObjectData.fObjectWeight;
				}break;
				}
			}
		}
	}

	g_HeroTotalSpeed = m_ObjectAgregateData.fTotalMoveSpeed;

	//추가 사항 입력
	//m_pItemControl = new LAItemControl;
	m_pItemControl->SetObjectAgregateData(m_ObjectAgregateData);
	return true;
}

bool LAHero::AddWeapon(std::shared_ptr<LAItem>& pItem)
{
	bool check = false;

	if (m_pItemControl)
	{
		switch (pItem->m_ObjectData.eWeaponType)
		{
		case E_WeaponType::RIFLE:
		{
			check = m_pItemControl->AddWeapon(pItem);
			if (check == false)
			{
				if (m_CharacterUiData.CharacterUIData[(int)E_WeaponType::RIFLE] == NULL)
				{
					m_CharacterUiData.CharacterUIData[(int)E_WeaponType::RIFLE] = pItem->GetProjectileData();
				}
			}
		}break;

		case E_WeaponType::HANDGUN:
		{
			check = m_pItemControl->AddWeapon(pItem);
			if (check == false)
			{
				if (m_CharacterUiData.CharacterUIData[(int)E_WeaponType::HANDGUN] == NULL)
				{
					m_CharacterUiData.CharacterUIData[(int)E_WeaponType::HANDGUN] = pItem->GetProjectileData();
				}
			}
		}break;

		case E_WeaponType::BOMB:
		{
			check = m_pItemControl->AddWeapon(pItem);
			if (check == false)
			{
				if (m_CharacterUiData.CharacterUIData[(int)E_WeaponType::BOMB] == NULL)
				{
					m_CharacterUiData.CharacterUIData[(int)E_WeaponType::BOMB] = pItem->GetProjectileData();
				}
			}
		}break;

		case E_WeaponType::MEDIC:
		{
			check = m_pItemControl->AddWeapon(pItem);
			if (check == false)
			{
				if (m_CharacterUiData.CharacterUIData[(int)E_WeaponType::MEDIC] == NULL)
				{
					m_CharacterUiData.CharacterUIData[(int)E_WeaponType::MEDIC] = pItem->GetProjectileData();
				}
			}
		}break;

		default:
		{
			//그외에 것 우선 지움
			check = true;
		}break;
		}
		
	}

	return check;
}

bool LAHero::AddArmor(std::shared_ptr<LAItem>& pItem)
{
	bool check = false;

	if (m_pItemControl)
	{
		check = m_pItemControl->AddArmor(pItem);

		if (check == false)
		{
			SetObjectABilityData();
		}
	}

	return check;
}

bool LAHero::SetWeapon(E_InventoryWeaponType ItemType)
{
	bool check = false;

	/*=========================== 아이템 컨트롤 클래스에 아이템 등록 ================================*/
	check = m_pItemControl->SetWeapon(ItemType);

	switch (ItemType)
	{
	case E_InventoryWeaponType::WEAPON_AK47:
	case E_InventoryWeaponType::WEAPON_K2:
	case E_InventoryWeaponType::WEAPON_H416:
	case E_InventoryWeaponType::WEAPON_KNIFE:
	case E_InventoryWeaponType::WEAPON_HANDGUN:
	case E_InventoryWeaponType::WEAPON_FRAG_GRENADE:
	case E_InventoryWeaponType::WEAPON_PORTABLE_GRENADE:
	case E_InventoryWeaponType::WEAPON_MEDIC:
	{
		LAItem* pData = m_pItemControl->GetCurrentWeapon();

		if (pData)
		{
			//UI 데이터 생성
			m_CharacterUiData.CharacterUIData[(int)m_pItemControl->GetCurrentWeaponType()] = pData->GetProjectileData();
		}
	}break;
	}

	return check;
}

void LAHero::SetCurrentMotion(int isetMotion)
{
	switch (isetMotion)
	{
	case (int)E_CharacterMotion::IDLE:
	{
		if ((!m_bisMove) && (!m_bisMotion) && (!m_bisJump) && (!m_bIsShot) && (!m_bIsReLoad))
		{
			m_iCurrentMotion = isetMotion;
		}
	}break;
	case (int)E_CharacterMotion::WORK_UP:
	case (int)E_CharacterMotion::WORK_DOWN:
	case (int)E_CharacterMotion::WORK_LEFT:
	case (int)E_CharacterMotion::WORK_RIGHT:
	{
		if (!m_bisRun && !m_bisJump && !m_bIsReLoad && !m_bIsShot)
		{
			m_iCurrentMotion = isetMotion;
			m_fAnimationMoveSpeed = WORK_MOVE_SPEED;
		}
		m_bisMove = true;
	}break;

	case (int)E_CharacterMotion::RUN:
	{
		if (m_bisMove && !m_bisJump && !m_bIsReLoad && !m_bIsShot)
		{
			m_iCurrentMotion = isetMotion;
			m_fAnimationMoveSpeed = RUN_MOVE_SPEED;
			m_bisRun = true;
		}
	}break;

	case (int)E_CharacterMotion::JUMP:
	{
		if (!m_bisMotion && !m_bIsReLoad && !m_bIsShot)
		{
			m_iCurrentMotion = isetMotion;
			m_bisJump = true;
		}
	}break;

	/*================================== Rifle ====================================*/
	//case (int)E_CharacterMotion::RIFLE_BEFORE_SET:
	//{
	//	m_iCurrentMotion = isetMotion;
	//	m_eSetWillWeapon = E_WeaponType::RIFLE;
	//	m_bisMotion = true;
	//	//컨트롤시스템에 아이템 셋팅
	//	//m_pItemControl->SetCurrentWeapon(E_WeaponType::RIFLE);
	//
	//}break;

	case (int)E_CharacterMotion::RIFLE_AFTER_SET:
	{
		m_iCurrentMotion = isetMotion;
		//컨트롤시스템에 아이템 셋팅
		m_pItemControl->SetCurrentWeapon(E_WeaponType::RIFLE);

	}break;

	case (int)E_CharacterMotion::RIFLE_RELOAD:
	{
		//인벤에 총알이 있을경우에만 장정가능
		if ((m_pItemControl->CheckInvenBullet(m_pItemControl->GetCurrentWeaponType())) && 
			(m_pItemControl->m_pWeaponList[(int)E_WeaponType::RIFLE]->m_ProjectileData->iCurrentCount != m_pItemControl->m_pWeaponList[(int)E_WeaponType::RIFLE]->m_ObjectData.iAbilityReloadMax))
		{
			m_iCurrentMotion = isetMotion;
			m_bIsReLoad = true;


		}
	}break;

	case (int)E_CharacterMotion::RIFLE_ACTION:
	{
		if ((!m_bIsReLoad) && (!m_bIsShot))
		{
			if (m_pItemControl->CheckBullet(E_WeaponType::RIFLE))
			{
				m_iCurrentMotion = isetMotion;
				m_bIsShot = true;
				m_bIsRifleShot = true;
			}
		}

	}break;
	/*================================== Handgun ====================================*/
	case (int)E_CharacterMotion::HANDGUN_AFTER_SET:
	{
		m_iCurrentMotion = isetMotion;
		//컨트롤시스템에 아이템 셋팅
		m_pItemControl->SetCurrentWeapon(E_WeaponType::HANDGUN);
	}break;

	/*================================== Knife ====================================*/
	case (int)E_CharacterMotion::KNIFE_AFTER_SET:
	{
		m_iCurrentMotion = isetMotion;
		//컨트롤시스템에 아이템 셋팅
		m_pItemControl->SetCurrentWeapon(E_WeaponType::KNIFE);
	}break;

	/*================================== Bomb ====================================*/
	case (int)E_CharacterMotion::BOMB_AFTER_SET:
	{
		m_iCurrentMotion = isetMotion;
		//컨트롤시스템에 아이템 셋팅
		m_pItemControl->SetCurrentWeapon(E_WeaponType::BOMB);
	}break;

	case (int)E_CharacterMotion::BOMB_RELOAD:
	{
		//인벤에 총알이 있을경우에만 장정가능
		if (m_pItemControl->m_bIsBombRender == false)
		{
			if ((m_pItemControl->CheckInvenBullet(m_pItemControl->GetCurrentWeaponType())) && 
				(m_pItemControl->m_pWeaponList[(int)E_WeaponType::BOMB]->m_ProjectileData->iCurrentCount == 0))
			{
				m_iCurrentMotion = isetMotion;
				m_bIsReLoad = true;



			}
		}
	}break;

	case (int)E_CharacterMotion::BOMB_ACTION:
	{
		if ((!m_bIsReLoad) && (!m_bIsShot))
		{
			if (m_pItemControl->CheckBullet(E_WeaponType::BOMB))
			{
				m_iCurrentMotion = isetMotion;
				m_bIsShot = true;
			}
		}
	}break;

	/*================================== Medic ====================================*/
	case (int)E_CharacterMotion::MEDIC_AFTER_SET:
	{
		m_iCurrentMotion = isetMotion;
		//컨트롤시스템에 아이템 셋팅
		m_pItemControl->SetCurrentWeapon(E_WeaponType::MEDIC);
	}break;

	case (int)E_CharacterMotion::MEDIC_RELOAD:
	{
		//인벤에 총알이 있을경우에만 장정가능
		if (m_pItemControl->m_bIsMedicRender == false)
		{
			if ((m_pItemControl->CheckInvenBullet(m_pItemControl->GetCurrentWeaponType())) &&
				(m_pItemControl->m_pWeaponList[(int)E_WeaponType::MEDIC]->m_ProjectileData->iCurrentCount == 0))
			{
				m_iCurrentMotion = isetMotion;
				m_bIsReLoad = true;
			}
		}
	}break;

	case (int)E_CharacterMotion::MEDIC_ACTION:
	{
		if ((!m_bIsReLoad) && (!m_bIsShot))
		{
			if (m_pItemControl->CheckBullet(E_WeaponType::MEDIC))
			{
				m_iCurrentMotion = isetMotion;
				m_bIsShot = true;
			}
		}
	}break;

	default:
	{
		m_iCurrentMotion = (int)E_CharacterMotion::IDLE;
	}break;
	}
}

void LAHero::SetHealPower(float fHealPower)
{
	m_ObjectAgregateData.iCurrentHp += (int)fHealPower;

	if (m_ObjectAgregateData.iCurrentHp > m_ObjectAgregateData.iTotalMaxHp)
	{
		m_ObjectAgregateData.iCurrentHp = m_ObjectAgregateData.iTotalMaxHp;
	}
}

void LAHero::SetSubtractDamage(int& iTargetHp, float fDefense, int iDamage)
{
	int Damage = iDamage * 100 / (100 + (int)fDefense);
	m_ObjectAgregateData.iCurrentHp -= Damage;
}

void LAHero::SetItemDrawMesage(bool bDrawMesage)
{
	m_bIsInItem = bDrawMesage;
}


const LACharacterUIData*  LAHero::GetUiCharacterData()
{
	return &m_CharacterUiData;
}

StructInventory* LAHero::GetInventoryData()
{
	if (m_pItemControl)
	{
		return &m_pItemControl->m_Inventory;
	}

	return NULL;
}

TVector3 LAHero::GetCameraPosition()
{
	TVector3 vCameraPos;

	LAResource* pResource = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pResource)
	{
		for (int iCnt = 0; iCnt < (int)pResource->m_BoneObjectList.size(); iCnt++)
		{
			if (pResource->m_BoneObjectList[iCnt]->m_MeshData->wsNodeName.compare(L"CameraPos01") == 0)
			{
				vCameraPos.x = pResource->m_BoneObjectList[iCnt]->m_matCalculation._41;
				vCameraPos.y = pResource->m_BoneObjectList[iCnt]->m_matCalculation._42;
				vCameraPos.z = pResource->m_BoneObjectList[iCnt]->m_matCalculation._43;

				return vCameraPos;
			}
		}
	}

	ZeroMemory(&vCameraPos, sizeof(TVector3));

	return vCameraPos;
}

E_InventoryWeaponType LAHero::GetCurrenWeaponType(int iWeaponType)
{
	return m_pItemControl->GetCurrentWeaponInvenType(iWeaponType);
}

void LAHero::UpdateMove()
{
	if (m_bisMove)
	{
		LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
		if (pResourceData)
		{
			if (pResourceData->m_AnimationList.size())
			{
				SceneData& rData = pResourceData->m_AnimationList[(int)E_CharacterMotion::WORK_UP];

				m_fMoveElapseFrame += g_fSecPerFrame * rData.iFrameSpeed * m_fAnimationMoveSpeed;
				if ((int)m_fMoveElapseFrame >= rData.iLastFrame - 1)
				{
					m_fMoveElapseFrame = rData.iFirstFrame + 1;

					return;
				}

				int TotalTick = rData.iLastFrame - rData.iFirstFrame;
				int DiviceTick = TotalTick * 25 / 100;
				int SoundIndex = m_iNumTile;

#ifdef TOOL_MODE

#else
				if (DiviceTick == (int)m_fMoveElapseFrame)
				{
					LA_SOUND2_MGR.playeffect(m_SondIndexList[SoundIndex]);
				}
				
				DiviceTick = TotalTick * 75 / 100;

				if (DiviceTick == (int)m_fMoveElapseFrame)
				{
					LA_SOUND2_MGR.playeffect(m_SondIndexList[SoundIndex + 1]);
				}
#endif

			}	//pResourceData->m_AnimationList.size()
		}	//pResourceData
	}	//m_bisMove
	else
	{
		m_fMoveElapseFrame = 0.0f;
	}
}

void LAHero::SubtractHpDamage(int iDamage)
{
	SetSubtractDamage(m_ObjectAgregateData.iCurrentHp, m_ObjectAgregateData.fTotalDef, iDamage);
	
	if (m_ObjectAgregateData.iCurrentHp <= 0)
	{
		m_bIsObjectLife = false;
	}
}

void LAHero::UpdateUi()
{
	//Hero
	if (m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA])
	{
		m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->iCurrentCount = m_ObjectAgregateData.iCurrentHp;
		m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->iMaxCount = m_ObjectAgregateData.iTotalMaxHp;
		m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->fHeroTotalHp = m_ObjectAgregateData.iTotalMaxHp;
		m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->fHeroTotalDef = m_ObjectAgregateData.fTotalDef;
		m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->fHeroTotalSpeed = m_ObjectAgregateData.fTotalAddRunSpeed;
	}

	if (m_pItemControl)
	{
		//Rifle
		if (m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::RIPLE_UI_DATA])
		{
			E_InventoryWeaponType bomType = m_pItemControl->m_pWeaponList[(int)E_WeaponType::RIFLE]->GetWeaponType();

			switch (bomType)
			{
			case E_InventoryWeaponType::WEAPON_AK47:
			{
				m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::RIPLE_UI_DATA]->iMaxCount = m_pItemControl->m_Inventory.dwTotalCount_AK47;
			}break;

			case E_InventoryWeaponType::WEAPON_K2:
			{
				m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::RIPLE_UI_DATA]->iMaxCount = m_pItemControl->m_Inventory.dwTotalCount_K2;
			}break;

			case E_InventoryWeaponType::WEAPON_H416:
			{
				m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::RIPLE_UI_DATA]->iMaxCount = m_pItemControl->m_Inventory.dwTotalCount_HK416;
			}break;
			}
		}

		//Handgun
		if (m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::HANDGUN_UI_DATA])
		{
			E_InventoryWeaponType bomType = m_pItemControl->m_pWeaponList[(int)E_WeaponType::HANDGUN]->GetWeaponType();

			switch (bomType)
			{
			case E_InventoryWeaponType::WEAPON_HANDGUN:
			{
				m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::HANDGUN_UI_DATA]->iMaxCount = m_pItemControl->m_Inventory.dwTotalCount_HANDGUN;
			}break;
			}
		}

		//Knife
		if (m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::KNIFE_UI_DATA])
		{
			E_InventoryWeaponType bomType = m_pItemControl->m_pWeaponList[(int)E_WeaponType::KNIFE]->GetWeaponType();

			switch (bomType)
			{
			case E_InventoryWeaponType::WEAPON_KNIFE:
			{
				m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::KNIFE_UI_DATA]->iMaxCount = 1;
			}break;
			}
		}

		//Bomb(임시 인벤데이터에서 로드)
		if (m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::BOMB_UI_DATA])
		{ 
			E_InventoryWeaponType bomType = m_pItemControl->m_pWeaponList[(int)E_WeaponType::BOMB]->GetWeaponType();

			switch (bomType)
			{
			case E_InventoryWeaponType::WEAPON_FRAG_GRENADE:
			{
				m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::BOMB_UI_DATA]->iMaxCount = m_pItemControl->m_Inventory.dwTotalCount_FRAG_GRENADE;
			}break;

			case E_InventoryWeaponType::WEAPON_PORTABLE_GRENADE:
			{
				m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::BOMB_UI_DATA]->iMaxCount = m_pItemControl->m_Inventory.dwTotalCount_PORTABLE_GRENADE;
			}break;
			}
		
		}

		//Medic(임시 인벤데이터에서 로드)
		if (m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::MEDIC_UI_DATA])
		{
			E_InventoryWeaponType bomType = m_pItemControl->m_pWeaponList[(int)E_WeaponType::MEDIC]->GetWeaponType();

			switch (bomType)
			{
			case E_InventoryWeaponType::WEAPON_MEDIC:
			{
				m_CharacterUiData.CharacterUIData[(int)E_LACharacterUiDataType::MEDIC_UI_DATA]->iMaxCount = m_pItemControl->m_Inventory.dwTotalCount_MEDICBOX;
			}break;
			}
		}
	}
}

void  LAHero::UpdateAddAnimationSpeed()
{
	if (m_bIsShot)
	{
		SetAddAnimationSpeed(m_pItemControl->GetCurrentWeapon()->GetAttackSpeed());
	}
	else
	{
		SetAddAnimationSpeed(1.0f);
	}
}

void LAHero::UpdateBullet()
{
	/*if (m_iCurrentMotion == (int)E_CharacterMotion::RIFLE_BEFORE_SET)
	{
		if (CheckPositionTick(50, false))
		{
			m_pItemControl->SetCurrentWeapon(m_eSetWillWeapon);
		}

		if (m_eSetWillWeapon == (E_WeaponType)-1)
		{
			SetCurrentMotion((int)E_CharacterMotion::RIFLE_AFTER_SET);
		}
	}*/
	

	m_pItemControl->UpdateBullet(m_MainResourceNumber, m_iCurrentMotion, m_fElapseFrame, this);
}

#ifdef TOOL_MODE

#else

bool LAHero::UpdateBulletCollition()
{
	if ((int)m_pItemControl->GetCurrentWeaponType() != -1)
	{
		LAItem* pData = NULL;

		pData = m_pItemControl->GetCurrentWeapon();

		switch (m_pItemControl->GetCurrentWeaponType())
		{
		case E_WeaponType::RIFLE:
		{
			if (pData)
			{
				std::vector<std::shared_ptr<LAUnit>>& data = LOAD_OBJECT_MGR.GetMonsterList();
				int iCount = data.size();
				for (int iCnt = 0; iCnt < iCount; iCnt++)
				{
					TVector3 vDamaged;
					if (pData->m_ParticleRender->ProcessingCollision(data[iCnt]->m_BoundingBox, vDamaged))
					{
						// 피 이펙트 방향 
						TVector3 dir[] =
						{
							{ -1.0f,  1.0f,  1.0f },   // 좌상
							{ 0.0f,  1.0f,  1.0f },   // 상
							{ 1.0f,  1.0f,  1.0f },   // 우상
							{ 1.0f,  0.0f,  1.0f },   // 우
							{ 0.0f,  0.0f,  1.0f },   // 중앙
							{ 1.0f, -1.0f,  1.0f },   // 우하
							{ 0.0f, -1.0f,  1.0f },   // 하
							{ -1.0f, -1.0f,  1.0f },   // 좌하
							{ -1.0f,  0.0f,  1.0f },   // 좌
						};

						// 피 이펙트
						int dirSize = sizeof(dir) / sizeof(dir[0]);
						for (int iEffect = 0; iEffect < dirSize; iEffect++)
						{
							ParticleSystem addParticle;
							addParticle = I_ParticleSystem.GetValue(EnemyBlood2);
							addParticle.m_Particle[0].m_vDirection = dir[iEffect];
							addParticle.m_Particle[0].m_vDirection.z = addParticle.m_Particle[0].m_vDirection.z * data[iCnt]->m_vObjectLook.z;
							data[iCnt]->m_ParticleSystemList.push_back(addParticle);
						}

						for (int iDamaged = 0; iDamaged < data[iCnt]->m_ParticleSystemList.size(); iDamaged++)
						{
							TVector3 vPos = data[iCnt]->m_BoundingBox.m_vCenter + vDamaged;
							data[iCnt]->m_ParticleSystemList[iDamaged].SetFrameParticleSystemInfo(&vPos);
						}

						// 바닥에 피 이펙트
						ParticleSystem addParticle;
						int iBloodrand = randstep(1, 3);
						if (iBloodrand == 1)
						{
							addParticle = I_ParticleSystem.GetValue(BloodFloor1);
							for (int iBlood = 0; iBlood < addParticle.m_Particle.size(); iBlood++)
							{
								addParticle.m_Particle[iBlood].m_vPos = data[iCnt]->m_vObjectPosition;
							}
							addParticle.CreateBoundingBox(data[iCnt]->m_vObjectPosition);
							m_ParticleSystemList.push_back(addParticle);
						}
						else if (iBloodrand == 2)
						{
							addParticle = I_ParticleSystem.GetValue(BloodFloor2);
							for (int iBlood = 0; iBlood < addParticle.m_Particle.size(); iBlood++)
							{
								addParticle.m_Particle[iBlood].m_vPos = data[iCnt]->m_vObjectPosition;
							}
							addParticle.CreateBoundingBox(data[iCnt]->m_vObjectPosition);
							m_ParticleSystemList.push_back(addParticle);
						}
						else
						{
							addParticle = I_ParticleSystem.GetValue(BloodFloor3);
							for (int iBlood = 0; iBlood < addParticle.m_Particle.size(); iBlood++)
							{
								addParticle.m_Particle[iBlood].m_vPos = data[iCnt]->m_vObjectPosition;
							}
							addParticle.CreateBoundingBox(data[iCnt]->m_vObjectPosition);
							m_ParticleSystemList.push_back(addParticle);
						}
						data[iCnt]->SubtractHpDamage(pData->m_pSkillData->m_SkillTableLoadData.fAttackPower);
					}
				}
			}
		}break;

		case E_WeaponType::HANDGUN:
		{

		}break;
		}
	}
	return true;
}
#endif

void LAHero::Up(int ikeyState)
{
	m_MovePushStack++;

	if ((m_bisMotion == false) || 
		(m_bisJump) ||
		(m_bisRun) ||
		(m_bIsReLoad))
	{
		float value = m_ObjectAgregateData.fTotalMoveSpeed * g_fSecPerFrame;

		
		if (m_bisRun)
		{
			value = (m_ObjectAgregateData.fTotalMoveSpeed * 2)* g_fSecPerFrame;
		}

		SetCurrentMotion((int)E_CharacterMotion::WORK_UP);
		//MoveLook(value);
		
	}
}

void LAHero::Down(int ikeyState)
{
	m_MovePushStack++;

	if ((m_bisMotion == false) ||
		(m_bisJump) ||
		(m_bisRun))
	{
		float value = m_ObjectAgregateData.fTotalMoveSpeed * g_fSecPerFrame;

		if (m_bisRun)
		{
			value = (m_ObjectAgregateData.fTotalMoveSpeed * 2)* g_fSecPerFrame;
		}
		SetCurrentMotion((int)E_CharacterMotion::WORK_DOWN);
		//MoveLook(-value);
	}
}

void LAHero::Left(int ikeyState)
{
	m_MovePushStack++;

	if ((m_bisMotion == false) ||
		(m_bisJump) ||
		(m_bisRun))
	{
		float value = m_ObjectAgregateData.fTotalMoveSpeed * g_fSecPerFrame;

		if (m_bisRun)
		{
			value = (m_ObjectAgregateData.fTotalMoveSpeed * 2)* g_fSecPerFrame;
		}
		SetCurrentMotion((int)E_CharacterMotion::WORK_LEFT);
		//MoveSide(-value);
	}
}

void LAHero::Right(int ikeyState)
{
	m_MovePushStack++;

	if ((m_bisMotion == false) ||
		(m_bisJump) ||
		(m_bisRun))
	{
		float value = m_ObjectAgregateData.fTotalMoveSpeed * g_fSecPerFrame;

		if (m_bisRun)
		{
			value = (m_ObjectAgregateData.fTotalMoveSpeed * 2)* g_fSecPerFrame;
		}
		SetCurrentMotion((int)E_CharacterMotion::WORK_RIGHT);
		//MoveSide(value);
	}
}

void LAHero::Idle(int ikeyState)
{

}

void LAHero::Work(int ikeyState)
{

}

void LAHero::Jump(int ikeyState)
{
	SetCurrentMotion((int)E_CharacterMotion::JUMP);
}

void LAHero::Run(int ikeyState)
{
	if (ikeyState == keyState::KEY_UP)
	{
		m_bisRun = false;

		return;
	}

	SetCurrentMotion((int)E_CharacterMotion::RUN);
}

void LAHero::BulletReLoad(int ikeyState)
{
	if ((int)m_pItemControl->GetCurrentWeaponType() != -1)
	{
		switch (m_pItemControl->GetCurrentWeaponType())
		{
		case E_WeaponType::RIFLE:
		{
			//모션변경(현제 선택된 무기가 존재한다면)
			if (m_pItemControl->m_pWeaponList[(int)m_pItemControl->GetCurrentWeaponType()])
			{
				if (!m_bIsReLoad)
				{
					SetCurrentMotion((int)E_CharacterMotion::RIFLE_RELOAD);
				}
			}
		}break;

		case E_WeaponType::BOMB:
		{		
			if (!m_bIsReLoad)
			{
				if (m_iCurrentMotion != (int)E_CharacterMotion::BOMB_RELOAD)
				{
					SetCurrentMotion((int)E_CharacterMotion::BOMB_RELOAD);
				}
			}			
		}break;

		case E_WeaponType::MEDIC:
		{
			if (!m_bIsReLoad)
			{
				if (m_iCurrentMotion != (int)E_CharacterMotion::MEDIC_RELOAD)
				{
					SetCurrentMotion((int)E_CharacterMotion::MEDIC_RELOAD);
				}
			}
		}break;
		}
	}
}

void LAHero::Attack(int ikeyState)
{
	//모션 셋팅
	switch (m_pItemControl->GetCurrentWeaponType())
	{
	case E_WeaponType::RIFLE:
	{
		if (m_pItemControl->GetWeapon(E_WeaponType::RIFLE))
		{			
			SetCurrentMotion((int)E_CharacterMotion::RIFLE_ACTION);		
		}
	}break;

	case E_WeaponType::BOMB:
	{
		if (ikeyState == keyState::KEY_UP)
		{
			if (m_pItemControl->GetWeapon(E_WeaponType::BOMB))
			{
				SetCurrentMotion((int)E_CharacterMotion::BOMB_ACTION);
			}
		}
	}break;

	case E_WeaponType::MEDIC:
	{
		if (ikeyState == keyState::KEY_PUSH)
		{
			if (m_pItemControl->GetWeapon(E_WeaponType::MEDIC))
			{
				SetCurrentMotion((int)E_CharacterMotion::MEDIC_ACTION);
			}
		}
	}break;
	}
}

void LAHero::SetWeaponRifle(int ikeyState)
{
	//SetCurrentMotion((int)E_CharacterMotion::RIFLE_BEFORE_SET);

	if (m_pItemControl->GetWeapon(E_WeaponType::RIFLE))
	{
		SetCurrentMotion((int)E_CharacterMotion::RIFLE_AFTER_SET);
	}
}

void LAHero::SetWeaponHandgun(int ikeyState)
{
	if (m_pItemControl->GetWeapon(E_WeaponType::HANDGUN))
	{
		SetCurrentMotion((int)E_CharacterMotion::HANDGUN_AFTER_SET);
	}
}

void LAHero::SetWeaponKnife(int ikeyState)
{
	if (m_pItemControl->GetWeapon(E_WeaponType::KNIFE))
	{
		SetCurrentMotion((int)E_CharacterMotion::KNIFE_AFTER_SET);
	}
}

void LAHero::SetWeaponBomb(int ikeyState)
{
	if (m_pItemControl->GetWeapon(E_WeaponType::BOMB))
	{
		SetCurrentMotion((int)E_CharacterMotion::BOMB_AFTER_SET);
	}
	
}

void LAHero::SetWeaponMedic(int ikeyState)
{
	if (m_pItemControl->GetWeapon(E_WeaponType::MEDIC))
	{
		SetCurrentMotion((int)E_CharacterMotion::MEDIC_AFTER_SET);
	}
}

void LAHero::SetMatrix(TMatrix* pWorldMatrix, TMatrix* pViewMatrix, TMatrix* pProjectionMatrix)
{
	LATopObject::SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);

	m_pItemControl->SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);
}

bool LAHero::Frame()
{
	if (I_Input.KeyCheck(DIK_J) == keyState::KEY_PUSH)
	{
		LAItem* pItem = m_pItemControl->GetSearchWeapon(E_InventoryWeaponType::WEAPON_AK47);

		if (pItem != NULL)
		{
			if (m_pItemControl->m_pWeaponList[(int)E_WeaponType::RIFLE]->GetWeaponType() != E_InventoryWeaponType::WEAPON_AK47)
			{
				SetWeapon(E_InventoryWeaponType::WEAPON_AK47);
			}
		}
	}

	if (I_Input.KeyCheck(DIK_K) == keyState::KEY_PUSH)
	{
		LAItem* pItem = m_pItemControl->GetSearchWeapon(E_InventoryWeaponType::WEAPON_K2);

		if (pItem != NULL)
		{
			if (m_pItemControl->m_pWeaponList[(int)E_WeaponType::RIFLE]->GetWeaponType() != E_InventoryWeaponType::WEAPON_K2)
			{
				SetWeapon(E_InventoryWeaponType::WEAPON_K2);
			}
		}
	}

	if (I_Input.KeyCheck(DIK_L) == keyState::KEY_PUSH)
	{
		LAItem* pItem = m_pItemControl->GetSearchWeapon(E_InventoryWeaponType::WEAPON_H416);

		if (pItem != NULL)
		{
			if (m_pItemControl->m_pWeaponList[(int)E_WeaponType::RIFLE]->GetWeaponType() != E_InventoryWeaponType::WEAPON_H416)
			{
				SetWeapon(E_InventoryWeaponType::WEAPON_H416);
			}
		}
	}


#ifdef TOOL_MODE

	m_bisMove = false;
	LATopObject::PostFrame();

#else

	//모션체크 아무런 행동이 없을경우 IDLE
	SetCurrentMotion((int)E_CharacterMotion::IDLE);

	//모션 스피드 업데이트
	UpdateAddAnimationSpeed();

	//모션 업데이트
	UpdateMotion();

	//아이템 업데이트
	UpdateBullet();
	

	//캐릭터 이동값 업데이트(캐릭터 별도체크)
	UpdateMove();

	//UI업데이트(UI 데이터 전송 업데이트)
	UpdateUi();

	//마우스 회전값 체크
	m_fYaw += I_Input.m_DIMouseState.lX * g_fSecPerFrame;

	// 캐릭터 회전
	D3DXMatrixRotationY(&m_matObjectrotation, g_pMainCamera->m_fYaw + Theta);


	m_bisMove = false;
	LATopObject::PostFrame();

	/*========================== 아이템 컨트롤 시스템 업데이트 ==============================*/
	m_pItemControl->Frame();

	/*========================== 아이템 컨트롤 시스템 무기 오브젝트 충돌 업데이트 ==============================*/
	UpdateBulletCollition();

	/*========================== Hero 데미지 이펙트 업데이트 ==============================*/
	if (m_bIsDamaged)
		m_DamageEffect.Frame();

#endif

	return true;
}

bool LAHero::Render()
{
	//렌더변경 없을시 부모오브젝트 렌더실행, 
	LATopObject::Render();

#ifdef TOOL_MODE
#else

	/*========================== 무기 오브젝트 포지션 업데이트 ==============================*/
	m_pItemControl->SetWeaponPosition(m_MainResourceNumber);

	/*========================== 아이템 컨트롤 시스템 렌더 ==============================*/
	m_pItemControl->Render();

	/*========================== 이펙트 렌더 ==============================*/
	if (m_bIsDamaged)
		m_DamageEffect.Render();

#endif

	return true;
}

bool LAHero::RenderShadow()
{
	//PrevRender();

	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pResourceData)
	{
		if (m_bSkinRender == true)
		{
			for (int iSkinObj = 0; iSkinObj < (int)pResourceData->m_SkinObjectList.size(); iSkinObj++)
			{
				//기본값 true 툴에서사용
				if (pResourceData->m_SkinObjectList[iSkinObj]->m_bisRander)
				{
					pResourceData->m_SkinObjectList[iSkinObj]->RenderShadow();
				}
			}
		}
	}

	return true;
}

bool LAHero::Release()
{
	LATopObject::Release();

#ifdef TOOL_MODE
#else

	/*========================== 아이템 컨트롤 시스템 릴리즈 ==============================*/
	m_pItemControl->Release();
	delete m_pItemControl;
	m_pItemControl = NULL;

	/*========================== 이펙트 릴리즈 ==============================*/
	//m_DamageEffect.Release();

#endif

	delete m_HeroHpProjectileData;
	m_HeroHpProjectileData = NULL;

	return true;
}

LAHero::LAHero()
{
	m_pItemControl = NULL;
	m_pItemControl = new LAItemControl;
	Theta = 0.0f;
	m_fYaw = 0.0f;
	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fMoveElapseFrame = 0.0f;
	m_iNumTile = -1;
	m_fAnimationMoveSpeed = WORK_MOVE_SPEED;
	m_HeroHpProjectileData = NULL;
	m_bIsReLoad = false;
	m_bisMove = false;
	m_bIsInItem = false;
	m_eSetWillWeapon = (E_WeaponType) - 1;

#ifdef TOOL_MODE

#else
	m_bIsDamaged = false;
#endif

}

LAHero::~LAHero()
{
}
