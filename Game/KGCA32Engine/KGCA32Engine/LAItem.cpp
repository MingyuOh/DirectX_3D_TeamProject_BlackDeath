
//#include "stdafx.h"

#include "LAItem.h"

#include "LALoadObjectManager.h"
#include "LAResourceManager.h"

#ifdef TOOL_MODE

#else

#include "LAParticleRifle.h"
#include "LAParticleHandgun.h"
#include "LAParticleKnife.h"
#include "LAParticleBomb.h"
#include "LAParticleMedic.h"

#endif

void LAItem::SetProjectileData()
{
	m_ProjectileData = new LAObjectUIDataInfo;

	m_ProjectileData->iCurrentCount = m_ObjectData.iBulletCount;
	m_ProjectileData->iMaxCount = m_ObjectData.iAbilityReloadMax;
	m_ProjectileData->wsiConFileName = m_ObjectData.wsIconNameList[0];
}

LAObjectUIDataInfo* LAItem::GetProjectileData()
{
	if (m_ProjectileData)
	{
		return m_ProjectileData;
	}

	return NULL;
}

E_InventoryWeaponType LAItem::GetWeaponType()
{
	E_InventoryWeaponType type;

	if (m_ObjectData.eWeaponType != (E_WeaponType)-1)
	{
		/*======================== Weapon type ==========================*/
		switch (m_ObjectData.eWeaponType)
		{
		case E_WeaponType::RIFLE:
		{
			if (m_ObjectData.wsCodeName.compare(L"TEST_RIPLE") == 0)
			{
				type = E_InventoryWeaponType::WEAPON_AK47;
			}
			else if (m_ObjectData.wsCodeName.compare(L"AK-47") == 0)
			{
				type = E_InventoryWeaponType::WEAPON_AK47;
			}
			else if (m_ObjectData.wsCodeName.compare(L"K2") == 0)
			{
				type = E_InventoryWeaponType::WEAPON_K2;
			}
			else if (m_ObjectData.wsCodeName.compare(L"HK416") == 0)
			{
				type = E_InventoryWeaponType::WEAPON_H416;
			}
		}break;

		case E_WeaponType::HANDGUN:
		{
			type = E_InventoryWeaponType::WEAPON_HANDGUN;
		}break;

		case E_WeaponType::KNIFE:
		{
			type = E_InventoryWeaponType::WEAPON_KNIFE;
		}break;

		case E_WeaponType::BOMB:
		{
			if (m_ObjectData.wsCodeName.compare(L"TEST_BOMB") == 0)
			{
				type = E_InventoryWeaponType::WEAPON_FRAG_GRENADE;
			}
			else if (m_ObjectData.wsCodeName.compare(L"FRAG_GRENADE") == 0)
			{
				type = E_InventoryWeaponType::WEAPON_FRAG_GRENADE;
			}
			else if (m_ObjectData.wsCodeName.compare(L"PORTABLE_GRENADE") == 0)
			{
				type = E_InventoryWeaponType::WEAPON_PORTABLE_GRENADE;
			}
		}break;

		case E_WeaponType::MEDIC:
		{
			type = E_InventoryWeaponType::WEAPON_MEDIC;
		}break;

		default:
		{
			type = (E_InventoryWeaponType)-1;
		}break;
		}
	}

	return type;
}

E_InventoryArmorType LAItem::GetArmorType()
{
	E_InventoryArmorType type;

	if (m_ObjectData.eArmorType != (E_ArmorType)-1)
	{
		/*======================== Armor type ==========================*/
		switch (m_ObjectData.eArmorType)
		{
		case E_ArmorType::HEAD:
		{
			type = E_InventoryArmorType::ARMOR_HEAD;
		}break;

		case E_ArmorType::BODY:
		{
			type = E_InventoryArmorType::ARMOR_BODY;
		}break;

		case E_ArmorType::HAND:
		{
			type = E_InventoryArmorType::ARMOR_HAND;
		}break;

		case E_ArmorType::FOOT:
		{
			type = E_InventoryArmorType::ARMOR_FOOT;
		}break;

		case E_ArmorType::ACE:
		{
			type = E_InventoryArmorType::ARMOR_ACE;
		}break;

		default:
		{
			type = (E_InventoryArmorType)-1;
		}break;
		}
	}
	else
	{
		type = (E_InventoryArmorType)-1;
	}

	return type;
}

TVector3 LAItem::GetParticlePositon()
{
	TVector3 vPos;
	ZeroMemory(&vPos, sizeof(TVector3));

	LAResource* pData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pData)
	{
		for (int iCnt = 0; iCnt < (int)pData->m_BoneObjectList.size(); iCnt++)
		{
			if (pData->m_BoneObjectList[iCnt]->m_MeshData->wsNodeName.compare(L"ParticlePoint") == 0)		//ak47
			//if (pData->m_BoneObjectList[iCnt]->m_MeshData->wsNodeName.compare(L"morden_particlePoint") == 0)	//morden
			{
				TMatrix matCalculation = pData->m_BoneObjectList[iCnt]->m_matCalculation;
				vPos.x = matCalculation._41;
				vPos.y = matCalculation._42;
				vPos.z = matCalculation._43;

				return vPos;
			}
		}
	}

	return vPos;
}

float LAItem::GetAttackSpeed()
{
	if ((m_ObjectData.fAbilityAttackSpeed * 1000) != 0)
	{
		return m_ObjectData.fAbilityAttackSpeed;
	}

	return 1.0f;
}

bool LAItem::Delete()
{
	LOAD_OBJECT_MGR.AddDeleteList(m_ObjectData.eObjectType, this);

	return true;
}


/*======================= Particle 함수 ============================*/

/*======================= Particle Create ============================*/
void LAItem::CreateParticle(BMapParser* pMapData)
{

#ifdef TOOL_MODE
#else

	/*===================== 무기 발사체 생성 =======================*/
	switch (m_ObjectData.eWeaponType)
	{
	case E_WeaponType::RIFLE:
	{
		m_ParticleRender = new LAParticleRifle;
		m_ParticleRender->ParticleInit(m_ObjectData);
		if (pMapData)
		{
			m_ParticleRender->m_pMapParser = pMapData;
		}
	}break;

	case E_WeaponType::HANDGUN:
	{
		m_ParticleRender = new LAParticleHandgun;
		m_ParticleRender->ParticleInit(m_ObjectData);
		if (pMapData)
		{
			m_ParticleRender->m_pMapParser = pMapData;
		}
	}break;

	case E_WeaponType::KNIFE:
	{
		m_ParticleRender = new LAParticleKnife;
		m_ParticleRender->ParticleInit(m_ObjectData);
		if (pMapData)
		{
			m_ParticleRender->m_pMapParser = pMapData;
		}
	}break;

	case E_WeaponType::BOMB:
	{
		m_ParticleRender = new LAParticleBomb;
		m_ParticleRender->ParticleInit(m_ObjectData);
		if (pMapData)
		{
			m_ParticleRender->m_pMapParser = pMapData;
		}
	}break;

	case E_WeaponType::MEDIC:
	{
		m_ParticleRender = new LAParticleMedic;
		m_ParticleRender->ParticleInit(m_ObjectData);
		if (pMapData)
		{
			m_ParticleRender->m_pMapParser = pMapData;
		}
	}break;
	}

	/*===================== 이펙트 생성 =======================*/
	ParticleSystem addParticle;
	addParticle = I_ParticleSystem.GetValue(ItemEffect);
	for (int iCnt = 0; iCnt < addParticle.m_Particle.size(); iCnt++)
	{
		addParticle.m_Particle[iCnt].m_vPos = GetPosition();
		addParticle.m_Particle[iCnt].m_vFirstPos = addParticle.m_Particle[iCnt].m_vPos;
	}
	addParticle.CreateBoundingBox(GetPosition());
	m_ParticleSystemList.push_back(addParticle);

#endif

}


void LAItem::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	LATopObject::Init(pDevice, pContext);

#ifdef TOOL_MODE

#else

#endif

}

bool LAItem::Frame()
{
	LATopObject::Frame();

	return true;
}

bool LAItem::Render()
{
	LATopObject::Render();

	return true;
}

bool LAItem::Release()
{
	if (m_ProjectileData)
	{
		S_DEL(m_ProjectileData);
	}

#ifdef TOOL_MODE

#else

	/*======================= Particle Frame ============================*/
	if (m_ParticleRender)
	{
		//m_ParticleRender->ParticleRelease();
	}

#endif

	LATopObject::Release();

	return true;
}

LAItem::LAItem()
{
	m_ProjectileData = NULL;
	m_ParticleRender = NULL;
}


LAItem::~LAItem()
{
}
