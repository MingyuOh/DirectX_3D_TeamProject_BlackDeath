#pragma once

#include "LAPSBSingleton.h"
#include "CoreStandardLink.h"
#include "LAHero.h"
#include "LAUnit.h"
#include "LAItem.h"
#include "LADeco.h"

#ifdef TOOL_MODE
#else

//충돌체크
#include "BCollision.h"

#endif



struct DeleteObjectInfo
{
	E_LoadTableType type;
	void* pObjectAdrees;

	DeleteObjectInfo()
	{
		type = (E_LoadTableType)-1;
		pObjectAdrees = NULL;
	}
};

class BMapParser;
class LALoadObjectManager : public LAPSBSingleton<LALoadObjectManager>
{
private:
	friend class LAPSBSingleton<LALoadObjectManager>;
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	BMapParser* m_pMap;
	bool m_bLoadManagerBipedRender;
	bool m_bLoadManagerSkinRender;
	bool m_bLoadManagerDrawLineRender;
	int iCount;

#ifdef TOOL_MODE
#else

public:
	//충돌체크클래스
	BCollision m_Collition;

#endif

private:
	std::map<int, ObjectTableLoadData>::iterator m_DataInfoItor;
	//character
	std::map<int, ObjectTableLoadData> m_CharacterDataInfoList;
	//std::vector<ObjectTableLoadData> m_CharacterDataInfoList;
	std::shared_ptr<LAHero> m_CharacterObject;
	//monster
	std::map<int, ObjectTableLoadData> m_MonsterDataInfoList;
	//std::vector<ObjectTableLoadData> m_MonsterDataInfoList;
	std::vector <std::shared_ptr<LAUnit>> m_MonsterObjectList;			//유저캐릭터와 모든씬의 캐릭터 생성리스트
	std::vector<int> m_RenderMonsterList;									//현제 씬의 카메라 안에 들어오는 캐릭터들 출력리스트  Rander();
	//weapon
	std::map<int, ObjectTableLoadData> m_WeaponDataInfoList;
	//std::vector<ObjectTableLoadData> m_WeaponDataInfoList;
	std::vector <std::shared_ptr<LAItem>> m_WeaponObjectList;
	std::vector<int> m_RenderWeaponList;
	//armor
	std::map<int, ObjectTableLoadData> m_ArmorDataInfoList;
	//std::vector<ObjectTableLoadData> m_ArmorDataInfoList;
	std::vector <std::shared_ptr<LAItem>> m_ArmorObjectList;
	std::vector<int> m_RenderArmorList;

	//SkillTable Load Data
	std::vector<SkillTableLoadData> m_SkillHItboxDataInfoList;
	std::vector<SkillTableLoadData> m_SkillBulletDataInfoList;
	std::vector<SkillTableLoadData> m_SkillBuffDataInfoList;
	std::vector<SkillTableLoadData> m_SkillTrapDataInfoList;

	//Delete
	std::vector<DeleteObjectInfo> m_DeleteObjectList;

	//Shadow Depth
	ID3D11DepthStencilState* m_pShadowStateDepthStencil;
public:
	//hero
	bool CreateHero(D3DXVECTOR3 vHeroPosition, int iTableIndex);
	LAHero* GetHero();
	bool DeleteHero(int iIndex);

	//monster
	bool CreateMonster(D3DXVECTOR3 vHeroPosition, int iTableIndex);
	LAUnit* GetMonsterObject(int iObjectIndex);
	int GetMonsterCount();
	bool AddRenderMonster(int iaddIndex);
	bool DeleteMonster(int iIndex);
	void MonsterCollision();
	void MonsterMapCollision();
	std::vector<std::shared_ptr<LAUnit>>& GetMonsterList();

	//Weapon
	bool CreateWeapon(D3DXVECTOR3 vHeroPosition, int iTableIndex, int iBulletCount);
	int GetWeaponSize();
	LAItem* GetWeaponObject(int iObjectIndex);
	int GetWeaponCount();
	bool AddRenderWeapon(int iaddIndex);
	bool DeleteWeapon(int iIndex);

	//Armor
	bool CreateArmor(D3DXVECTOR3 vHeroPosition, int iTableIndex);
	LAItem* GetArmorObject(int iObjectIndex);
	int GetArmorCount();
	bool AddRenderArmor(int iaddIndex);
	bool DeleteArmor(int iIndex);

	//Set, Add
	void AddObjectDataInfo(ObjectTableLoadData& rObjectData);
	void AddSkillDataInfo(SkillTableLoadData& rSkillData);
	SkillTableLoadData* SetObjectSkillType(ObjectTableLoadData& rObjectData);

	//Get
	ID3D11DepthStencilState* GetShadowDepthStencil();

	//func
	void ClearRenderList();
	void CharacterInItem(std::shared_ptr<LAItem >& pAdrees);
	void MonsterInItem(E_LoadTableType eType, LAUnit* pMonsterThis, int iItemIndex);
	void AllDeleateObject();
	void DeleateMonsterList();
	void DeleateWeaponList();
	void DeleateArmorList();
	bool CheckOverlapTableIndex(E_LoadTableType eTybleType, int iTableIndex);

	//Update
	void CkeckKeyState();
	void AddDeleteList(E_LoadTableType eType, void* pAdrees);
	void UpdateDeleateList();

	//Depth
	bool CreateShadowDepthStencilViewAndState();

	//Instance Render
	bool InstanceMonsterRender();

public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BMapParser* pMap);
	void SetMatrix(D3DXMATRIX* pWorldMatrix, D3DXMATRIX* pViewMatrix, D3DXMATRIX* pProjectionMatrix);
	bool Frame();
	bool Render();
	bool RenderShadow();
	bool Release();
public:
	LALoadObjectManager()
	{
		m_pDevice = NULL;
		m_pContext = NULL;
		m_pMap = NULL;
		iCount = 0;
		m_bLoadManagerBipedRender = false;
		m_bLoadManagerSkinRender = true;
		m_bLoadManagerDrawLineRender = false;
		m_pShadowStateDepthStencil = NULL;
	}
	virtual ~LALoadObjectManager();
};

#define LOAD_OBJECT_MGR LALoadObjectManager::GetInstance()