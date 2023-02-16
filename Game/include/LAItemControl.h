#pragma once

#include "CoreStandardLink.h"
#include "LACharacterStruct.h"

const int CheckAniSize = 9;

enum class E_CheckAniPostionTick
{
	Rifle_Reload_Sound,
	Rifle_Reload,
	Rifle_Shot,

	Bomb_Reload_Sound,
	Bomb_Reload,
	Bomb_Shot,

	Medic_Reload_Sound,
	Medic_Reload,
	Medic_shot,
};

class LAItem;
class LATopObject;
class LAItemControl
{
public:
	StructInventory m_Inventory;
	ObjectTotalDataInfo* m_pObjectAgregateData;
	LAItem* m_pWeaponList[MAX_WEAPON_SIZE];
	LAItem* m_pArmorList[MAX_ARMOR_SIZE];
	E_WeaponType m_eCurrentWeaponType;
	D3DXMATRIX* m_pmatItemPivot;
	bool m_bIsBombRender;
	bool m_bIsMedicRender;
	bool m_bIsFireIntheHole;
	bool m_bisCheckTick[CheckAniSize];
public:
	bool AddWeapon(std::shared_ptr<LAItem>& pItem);
	bool AddArmor(std::shared_ptr<LAItem>& pItem);
	bool SetWeapon(E_InventoryWeaponType pItem);
	bool SetArmor(E_InventoryArmorType eArmorType);

	void SetBombRender(bool bResult);
	void SetCurrentWeapon(E_WeaponType eWeapontype);
	//void SetCurrentMotion(int iMotion);
	void SetObjectAgregateData(ObjectTotalDataInfo& totaldata);
	void SetWeaponPosition(int iResouceNumber);


	E_WeaponType GetCurrentWeaponType();
	E_InventoryWeaponType GetCurrentWeaponInvenType(int iWeaponType);
	LAItem* GetWeapon(E_WeaponType type);
	LAItem* GetArmor(E_ArmorType type);
	D3DXVECTOR3 GetLeftHandGunPostion(int iResouceNumber);
	D3DXMATRIX* GetLeftHandGunMatrixWorld(int iResouceNumber);
	D3DXVECTOR3 GetRightHandGunPosition(int iResouceNumber);
	D3DXMATRIX* GetRightHandGunMatrixWorld(int iResouceNumber);
	LAItem* GetCurrentWeapon();
	LAItem* GetSearchWeapon(E_InventoryWeaponType type);
	LAItem* GetSearchArmor(E_InventoryArmorType type);

	bool UpdateShot();
	bool UpdateBullet(int ResourceNumber, int iCurrentMotion, float fElpaseFrame, LATopObject* pTopObject);

	bool CheckInvenBullet(E_WeaponType type);
	bool CheckBullet(E_WeaponType type);
	bool BulletLeRoading();

	bool CheckPositionTick(int iPercentage, bool& bisReCheck, int iResourceNumber, int iCurrentMostion, float fElapseFrame);
	void ResetCheckTick();
public:
	void SetMatrix(D3DXMATRIX* pWorldMatrix, D3DXMATRIX* pViewMatrix, D3DXMATRIX* pProjectionMatrix);
	bool Frame();
	bool Render();
	bool RenderShadow();
	bool Release();
public:
	LAItemControl()
	{
		m_pObjectAgregateData = NULL;
		m_pmatItemPivot = NULL;
		m_bIsBombRender = false;
		m_bIsMedicRender = false;
		m_bIsFireIntheHole = false;
		m_eCurrentWeaponType = (E_WeaponType)-1;
		for (int iCnt = 0; iCnt < CheckAniSize; iCnt++)
		{
			m_bisCheckTick[iCnt] = false;
		}
	
		for (int iCnt = 0; iCnt < MAX_WEAPON_SIZE; iCnt++)
		{
			m_pWeaponList[iCnt] = NULL;
		}
		for (int iCnt = 0; iCnt < MAX_ARMOR_SIZE; iCnt++)
		{
			m_pArmorList[iCnt] = NULL;
		}
	}
	~LAItemControl();
};

