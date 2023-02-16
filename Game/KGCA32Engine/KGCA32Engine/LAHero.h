#pragma once

#include "LAResource.h"

#include "LATopObject.h"
#include "LAKeyStateCommand.h"
#include "LACharacterStruct.h"
#include "LAItemControl.h"

#ifdef TOOL_MODE
#else

#include "LASound2.h"
#include "ParticleSystem.h"

#endif



class LAHero : public LATopObject
{
public:
	//Human
	LAItemControl* m_pItemControl;
	float m_fAnimationMoveSpeed;
	bool m_bIsReLoad;
	bool m_bIsInItem;

	//Hero
	ObjectTotalDataInfo m_ObjectAgregateData;
	LACharacterUIData m_CharacterUiData;
	LAObjectUIDataInfo* m_HeroHpProjectileData;
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;
	float Theta;
	float m_fMoveElapseFrame;
	E_WeaponType m_eSetWillWeapon;
	
#ifdef TOOL_MODE
#else
	/*====================== 임펙트관련 멤버 변수 ========================*/
	// 히어로 피격 이펙트
	ParticleSystem m_DamageEffect;
	bool	m_bIsDamaged;
#endif

public:
	//check
	bool CheckIsHeroLife();
	void ResetCheck();
	bool CheckInItem();

	//Set
	void SetDirection(TVector3* vData);
	void SetRotation(float x, float y, float z);
	void SetHeroHpProjectileData();
	bool SetObjectABilityData();	//아이템 추가시 삭제시 한번씩 호출
	bool AddWeapon(std::shared_ptr<LAItem>& pItem);
	bool AddArmor(std::shared_ptr<LAItem>& pItem);
	bool SetWeapon(E_InventoryWeaponType ItemType);
	void SetCurrentMotion(int isetMotion);
	void SetHealPower(float fHealPower);
	void SetSubtractDamage(int& iTargetHp, float fDefense, int iDamage);
	void SetItemDrawMesage(bool bDrawMesage);
	//Get
	const LACharacterUIData* GetUiCharacterData();	//캐릭터 데이터 반환
	StructInventory* GetInventoryData();
	TVector3 GetCameraPosition();
	E_InventoryWeaponType GetCurrenWeaponType(int iWeaponType);
	//Update
	void UpdateMove();
	void SubtractHpDamage(int iDamage);
	void UpdateUi();
	void UpdateAddAnimationSpeed();
	void UpdateBullet();

#ifdef TOOL_MODE

#else
	bool UpdateBulletCollition();
#endif

	//Motion
	void Up(int ikeyState);
	void Down(int ikeyState);
	void Left(int ikeyState);
	void Right(int ikeyState);
	void Idle(int ikeyState);
	void Jump(int ikeyState);
	void Work(int ikeyState);
	void Run(int ikeyState);
	void BulletReLoad(int ikeyState);
	void Attack(int ikeyState);	//Attack
	void SetWeaponRifle(int ikeyState);
	void SetWeaponHandgun(int ikeyState);
	void SetWeaponKnife(int ikeyState);
	void SetWeaponBomb(int ikeyState);
	void SetWeaponMedic(int ikeyState);
public:
	void SetMatrix(TMatrix* pWorldMatrix, TMatrix* pViewMatrix, TMatrix* pProjectionMatrix);
	bool Frame();
	bool Render();
	bool RenderShadow();
	bool Release();
public:
	LAHero();
	virtual~LAHero();
};

