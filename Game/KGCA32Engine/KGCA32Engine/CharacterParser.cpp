
#include "CharacterParser.h"

#include "LAInputHandler.h"

#include "LAManagerOfManager.h"
#include "LAPaser.h"
#include "BCore.h"
#include "LAGrobalValue.h"
#include "K_UIParser.h"

LAPaser g_paserCharacter;
LAPaser g_paserMonster;
LAPaser g_paserItem;
LAPaser g_paserSkill;

unsigned int WINAPI LoadCharacter(void*avg)
{
	g_paserCharacter.LoadTable(E_LoadTableType::CHARACTER);
	return 0;
}

unsigned int WINAPI LoadMonster(void*avg)
{
	g_paserMonster.LoadTable(E_LoadTableType::MONSTER);
	return 0;
}

unsigned int WINAPI LoadItem(void*avg)
{
	g_paserItem.LoadTable(E_LoadTableType::WEAPON);
	g_paserItem.LoadTable(E_LoadTableType::ARMOR);
	return 0;
}

unsigned int WINAPI LoadSkill(void*avg)
{
	g_paserSkill.LoadTable(E_LoadTableType::SKILL_HITBOX);
	g_paserSkill.LoadTable(E_LoadTableType::SKILL_BULLET);
	g_paserSkill.LoadTable(E_LoadTableType::SKILL_BUFF);
	g_paserSkill.LoadTable(E_LoadTableType::SKILL_TRAP);
	return 0;
}

TVector3 CharacterParser::GetHeroPosition()
{
	return pHero->GetPosition();
}

TVector3 CharacterParser::GetHeroDirection()
{
	return pHero->GetDirection();
}

//Test�ӽ�
static struct iteminfo
{
	int iIndex;
	TVector3 vPosition;
};

bool CharacterParser::Init(BMapParser* pMap)
{
	srand((unsigned int) time(NULL));
	/* ================ ���ļ� ���� ================ */
	m_pMap = pMap;

	/* ================ �ֻ��� �Ŵ��� ================ */
	MGR_OF_MGR.Init(g_pd3dDevice, g_pImmediateContext, pMap);

	/* ================ ���ҽ� ���� �ε�(���ʿ�� �ּ�ó��) ================ */
	LAPaser paser;
	paser.Init(g_pd3dDevice, g_pImmediateContext);
	m_hTread[0] = (HANDLE)_beginthreadex(NULL, 0, LoadCharacter, NULL, 0, (unsigned*)&m_thread_Character_ID);
	m_hTread[1] = (HANDLE)_beginthreadex(NULL, 0, LoadMonster, NULL, 0, (unsigned*)&m_thread_Monster_ID);
	m_hTread[2] = (HANDLE)_beginthreadex(NULL, 0, LoadItem, NULL, 0, (unsigned*)&m_thread_Item_ID);
	m_hTread[3] = (HANDLE)_beginthreadex(NULL, 0, LoadSkill, NULL, 0, (unsigned*)&m_thread_Skill_ID);
	WaitForMultipleObjects(4, m_hTread, TRUE, INFINITE);

	/* ================ �ӽ� ���� ������Ʈ ���� ================ */
	//Monster ����
	//iteminfo Monster[5];
	//for (int iUnit = 0; iUnit < 5; iUnit++)
	//{
	//	Monster[iUnit].vPosition = TVector3((float)50 - rand() % 100, 0, (float)50 - rand() % 100);

	//	//���� ����(�����ε���, ������, ���ҽ��ѹ�)
	//	if (LOAD_OBJECT_MGR.CreateMonster(Monster[iUnit].vPosition, 100300))
	//	{

	//	}
	//	else
	//	{
	//		//�ε����
	//		std::wstring ErrorMGG = L"Not Create Monster Table Data : Index ";
	//		ErrorMGG += std::to_wstring(100100);
	//		OutputDebugString(ErrorMGG.c_str());

	//		break;
	//	}
	//}

	/*=============================== ���� �� ���� ���� ===================================*/
	//ak47
	if (LOAD_OBJECT_MGR.CreateWeapon(TVector3(-123, 0.0f, -119.0f), 6, 1)){}
	else
	{
		//�ε����
		std::wstring ErrorMGG = L"Not Create Monster Table Data : Index ";
		ErrorMGG += std::to_wstring(100100);
		OutputDebugString(ErrorMGG.c_str());
	}
	//k2
	if (LOAD_OBJECT_MGR.CreateWeapon(TVector3(-119, 0.0f, -119.0f), 7, 1)) {}
	else
	{
		//�ε����
		std::wstring ErrorMGG = L"Not Create Monster Table Data : Index ";
		ErrorMGG += std::to_wstring(100100);
		OutputDebugString(ErrorMGG.c_str());
	}
	//hk416
	if (LOAD_OBJECT_MGR.CreateWeapon(TVector3(-115, 0.0f, -119.0f), 8, 1)) {}
	else
	{
		//�ε����
		std::wstring ErrorMGG = L"Not Create Monster Table Data : Index ";
		ErrorMGG += std::to_wstring(100100);
		OutputDebugString(ErrorMGG.c_str());
	}
	//Frag_grenade
	if (LOAD_OBJECT_MGR.CreateWeapon(TVector3(-111, 0.0f, -119.0f), 9, 1)) {}
	else
	{
		//�ε����
		std::wstring ErrorMGG = L"Not Create Monster Table Data : Index ";
		ErrorMGG += std::to_wstring(100100);
		OutputDebugString(ErrorMGG.c_str());
	}
	//Portable_grenade
	if (LOAD_OBJECT_MGR.CreateWeapon(TVector3(-107, 0.0f, -119.0f), 10, 1)) {}
	else
	{
		//�ε����
		std::wstring ErrorMGG = L"Not Create Monster Table Data : Index ";
		ErrorMGG += std::to_wstring(100100);
		OutputDebugString(ErrorMGG.c_str());
	}
	//Medic
	if (LOAD_OBJECT_MGR.CreateWeapon(TVector3(-103, 0.0f, -119.0f), 5, 1)) {}
	else
	{
		//�ε����
		std::wstring ErrorMGG = L"Not Create Monster Table Data : Index ";
		ErrorMGG += std::to_wstring(100100);
		OutputDebugString(ErrorMGG.c_str());
	}
	//Head
	if (LOAD_OBJECT_MGR.CreateArmor(TVector3(-99, 0.0f, -119.0f), 1)) {}
	else
	{
		//�ε����
		std::wstring ErrorMGG = L"Not Create Monster Table Data : Index ";
		ErrorMGG += std::to_wstring(100100);
		OutputDebugString(ErrorMGG.c_str());
	}
	//Body
	if (LOAD_OBJECT_MGR.CreateArmor(TVector3(-95, 0.0f, -119.0f), 2)) {}
	else
	{
		//�ε����
		std::wstring ErrorMGG = L"Not Create Monster Table Data : Index ";
		ErrorMGG += std::to_wstring(100100);
		OutputDebugString(ErrorMGG.c_str());
	}
	//hand
	if (LOAD_OBJECT_MGR.CreateArmor(TVector3(-91, 0.0f, -119.0f), 3)) {}
	else
	{
		//�ε����
		std::wstring ErrorMGG = L"Not Create Monster Table Data : Index ";
		ErrorMGG += std::to_wstring(100100);
		OutputDebugString(ErrorMGG.c_str());
	}
	//foot
	if (LOAD_OBJECT_MGR.CreateArmor(TVector3(-87, 0.0f, -119.0f), 4)) {}
	else
	{
		//�ε����
		std::wstring ErrorMGG = L"Not Create Monster Table Data : Index ";
		ErrorMGG += std::to_wstring(100100);
		OutputDebugString(ErrorMGG.c_str());
	}
	/* ================ ���� ������Ʈ ���� �� ================ */

	/*=============================== Hero ���� ===================================*/
	if (LOAD_OBJECT_MGR.CreateHero(TVector3(-83, 0, -112), 1))
	{
		pHero = LOAD_OBJECT_MGR.GetHero();
	}
	else
	{
		//�ε����
		std::wstring ErrorMGG = L"Not Create Character Table Data : Index ";
		ErrorMGG += std::to_wstring(1);
		OutputDebugString(ErrorMGG.c_str());
	}
	
	/* ================ Ű ���� , ����� ���� ================ */
	LAINPUT_HANDLER.Init();
	LAINPUT_HANDLER.SetHeroUser(pHero);



	/*================================= ī�޶� ĳ���� �̵��� ���� ===================================*/
	g_pMainCamera->m_SpeedUp = g_HeroTotalSpeed;

	return true;
}

bool CharacterParser::Frame()
{
	/* ================ Ű üũ ================ */
	LAINPUT_HANDLER.LAHandleInput();

	/* ================ ���� ���� / �븻�� ī�޶� ���� ================ */
	TVector4 fMapNormalandHeight = m_pMap->GetHeight(g_pMainCamera->m_vPos, NULL, &g_pMainCamera->m_bCheckMap);
	g_pMainCamera->SetCameraPosY(fMapNormalandHeight.w);
	g_pMainCamera->SetMapNormal(TVector3(fMapNormalandHeight.x, fMapNormalandHeight.y, fMapNormalandHeight.z));

	/* ================ ī�޶� ĳ���� �ٿ���ڽ� ���� ================ */
	g_pMainCamera->SetHeroBoundingBoxInfo(pHero->m_BoundingBox);

	/* ================ ī�޶� ���� �浹ó�� ================ */
	std::vector<std::shared_ptr<LAUnit>>& MonsterList = LOAD_OBJECT_MGR.GetMonsterList();
	int iObjCnt = MonsterList.size();
	for (int iObj = 0; iObj < iObjCnt; iObj++)
		g_pMainCamera->CameraToObjectCollision(MonsterList[iObj]->m_BoundingBox);

	/* ================ ī�޶� �������� �巳�� �浹ó�� ================ */
	if (m_pMap->m_UIParser->FarmingOrBattle == 1 || m_pMap->m_UIParser->FarmingOrBattle == 2)
	{
		g_pMainCamera->CameraToObjectCollision(m_pMap->m_DrumBoundingBox[0]);
	}
	else if (m_pMap->m_UIParser->FarmingOrBattle == 3 || m_pMap->m_UIParser->FarmingOrBattle == 4)
	{
		g_pMainCamera->CameraToObjectCollision(m_pMap->m_DrumBoundingBox[1]);
		if (m_pMap->m_EnemyPos == 3)
		{
			m_pMap->HeroDem++;
			if (m_pMap->HeroDem > 60)
			{
				pHero->SubtractHpDamage(2);
				m_pMap->HeroDem = 0;

				pHero->m_DamageEffect = I_ParticleSystem.GetValue(HeroElectroDamaged);
				pHero->m_bIsDamaged = true;
			}
		}
	}
	else if (m_pMap->m_UIParser->FarmingOrBattle == 5 || m_pMap->m_UIParser->FarmingOrBattle == 6)
	{
		g_pMainCamera->CameraToObjectCollision(m_pMap->m_DrumBoundingBox[2]);
		if (m_pMap->m_EnemyPos == 4 || m_pMap->m_EnemyPos == 3)
		{
			m_pMap->HeroDem++;
			if (m_pMap->HeroDem > 60)
			{
				pHero->SubtractHpDamage(2);
				m_pMap->HeroDem = 0;

				pHero->m_DamageEffect = I_ParticleSystem.GetValue(HeroElectroDamaged);
				pHero->m_bIsDamaged = true;
			}
		}
	}
	else if (m_pMap->m_UIParser->FarmingOrBattle == 6 || m_pMap->m_UIParser->FarmingOrBattle == 7)
	{
		if (m_pMap->m_EnemyPos == 1 || m_pMap->m_EnemyPos == 4 || m_pMap->m_EnemyPos == 3)
		{
			m_pMap->HeroDem++;
			if (m_pMap->HeroDem > 60)
			{
				pHero->SubtractHpDamage(2);
				m_pMap->HeroDem = 0;

				pHero->m_DamageEffect = I_ParticleSystem.GetValue(HeroElectroDamaged);
				pHero->m_bIsDamaged = true;
			}
		}
	}

	/* ================ ī�޶� ������ ================ */
	g_pMainCamera->Frame();

	/* ================ ����� ���⺤��, ������ ������Ʈ ================ */
	pHero->m_vObjectLook = g_pMainCamera->m_vLook;
	pHero->m_vObjectSide = g_pMainCamera->m_vSide;
	pHero->SetPosition(g_pMainCamera->m_vPos.x - pHero->m_vObjectLook.x * 0.25f, g_pMainCamera->m_vPos.y - 1.8f, g_pMainCamera->m_vPos.z - pHero->m_vObjectLook.z * 0.25f);

	/* ================ ������Ʈ ī�޶� ������Ʈ �� ������ ================ */
	LOAD_OBJECT_MGR.SetMatrix(NULL, &g_pMainCamera->m_matView, &g_pMainCamera->m_matProj);
	LOAD_OBJECT_MGR.Frame();

	/* ====================������ ������Ʈ ���� ����==================== */
	if (LOAD_OBJECT_MGR.GetWeaponCount() != m_pMap->m_vMapQuadTree[0]->m_ItemPosition.size())
	{
		m_pMap->m_vMapQuadTree[0]->m_ItemPosition.clear();
		m_pMap->m_vMapQuadTree[0]->m_ItemPosition.resize(LOAD_OBJECT_MGR.GetWeaponCount());
		for (int iItem = 0; iItem < LOAD_OBJECT_MGR.GetWeaponCount(); iItem++)
		{
			LAItem* TempItem = LOAD_OBJECT_MGR.GetWeaponObject(iItem);
			m_pMap->m_vMapQuadTree[0]->m_ItemPosition[iItem] = TempItem->GetPosition();
		}
		m_pMap->m_vMapQuadTree[0]->ClearObject(m_pMap->m_vMapQuadTree[0]->m_pRootNode);
		m_pMap->m_vMapQuadTree[0]->AddObject();
	}
	///////////////////////////////////////////////////////////////////////

	/* ================ ���� �ý��� ������Ʈ ================ */
	LA_SOUND2_MGR.m_pSystem->update();

	return true;
}

bool CharacterParser::Render()
{
	/* ================ ������Ʈ ���� ����Ʈ ���� ================ */
	for (int iCnt = 0; iCnt < LOAD_OBJECT_MGR.GetMonsterCount(); iCnt++)
	{
		LOAD_OBJECT_MGR.AddRenderMonster(iCnt);
	}

	for (int iCnt = 0; iCnt < m_pMap->m_vMapQuadTree[0]->m_DrawObjList.size(); iCnt++)
	{
		LOAD_OBJECT_MGR.AddRenderWeapon(m_pMap->m_vMapQuadTree[0]->m_DrawObjList[iCnt]);
	}

	for (int iCnt = 0; iCnt < 20; iCnt++)
	{
		LOAD_OBJECT_MGR.AddRenderArmor(iCnt);
	}

	/* ================ ������Ʈ ���� ================ */
	LOAD_OBJECT_MGR.Render();

	return true;
}

bool CharacterParser::GameReset()
{
	LOAD_OBJECT_MGR.Release();
	LA_SOUND2_MGR.Release();

	return true;
}

bool CharacterParser::Release()
{
	MGR_OF_MGR.Release();

	return true;
}

CharacterParser::CharacterParser()
{
}


CharacterParser::~CharacterParser()
{
}
