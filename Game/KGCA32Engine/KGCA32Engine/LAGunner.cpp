
////#include "stdafx.h"

#include "LAGunner.h"

#include "LALoadObjectManager.h"

#ifdef TOOL_MODE
#else
/*===================== UI Include ========================*/

#endif



#ifdef TOOL_MODE
#else
/*===================== UI �Լ� ========================*/
float LAGunner::DistanceMonstertoHero()
{
	float Dis_X = m_vHeroPosition.x - m_vObjectPosition.x;
	float Dis_Y = m_vHeroPosition.y - m_vObjectPosition.y;
	float Dis_Z = m_vHeroPosition.z - m_vObjectPosition.z;

	float Distance = sqrt(pow(Dis_X, 2) + pow(Dis_Y, 2) + pow(Dis_Z, 2));

	return Distance;
}
void  LAGunner::MonsterMotionStateSet(int MotionNumber)
{
	switch (MotionNumber)
	{
	case MONSTER_GUNNER_STATE_IDLE:
	{

	}break;
	case MONSTER_GUNNER_STATE_MOVE:
	{
		MoveUp();
	}break;
	case MONSTER_GUNNER_STATE_ATTACK:
	{
		m_vMoveUpdatePosition.x = 0.0f;
		m_vMoveUpdatePosition.y = 0.0f;
		m_vMoveUpdatePosition.z = 0.0f;
		Attack();

		//if (LOAD_OBJECT_MGR.m_Collition.CheckSphereInSphere(m_BoundingBox.m_vCenter, LOAD_OBJECT_MGR.GetHero()->m_BoundingBox.m_vCenter,
		//	m_BoundingBox.m_fRadius, LOAD_OBJECT_MGR.GetHero()->m_BoundingBox.m_fRadius + 1))
		//{
		//	if (CheckPositionTick(10, false))	//�������� 40�ۼ�Ʈ�� ��������
		//	{
		//		LOAD_OBJECT_MGR.GetHero()->SubtractHpDamage(m_pSkillData->m_SkillTableLoadData.fAttackPower);
		//		LOAD_OBJECT_MGR.GetHero()->m_bIsDamaged = true;
		//		LOAD_OBJECT_MGR.GetHero()->m_DamageEffect = I_ParticleSystem.GetValue(HeroDamaged);
		//		LOAD_OBJECT_MGR.GetHero()->m_DamageEffect.CreateBoundingBox(TVector3(50.0f, 50.0f, 50.0f));

		//		//m_ObjectData.iHp -= 50;
		//	}
		//}
		//else
		//	LOAD_OBJECT_MGR.GetHero()->m_bIsDamaged = false;

	}break;
	case MONSTER_GUNNER_STATE_DEAD:
	{
		Death();
	}break;
	}
}

void  LAGunner::MonsterEventOccurrence()
{
	if (m_ObjectAgregateData.iCurrentHp <= 0)
	{
		m_MonsterAI.SetTransition(MONSTER_EVENT_DEAD);
		return;
	}
	if (IsDamaged)
	{
		IsFindTarget = true;
	}

	if (isCollision)	//�浹�� �߻�������
	{
		//��θ� ĳ������ ���� ���������� �缳��
		IsFindTarget = false;
		IsDamaged = false;
		if (!GotoPlayer)
		{
			DistanceToHero = DistanceMonstertoHero();
			if (m_MonsterAI.m_Path[0] == -1)
			{
				m_MonsterAI.m_Graph->Dijkstra_AG(m_MonsterAI.ReturnMostCloseHerotoSpawnPoint(m_vObjectPosition), m_MonsterAI.ReturnMostCloseHerotoSpawnPoint(m_vHeroPosition), m_MonsterAI.m_Path);
			}
			GotoPlayer = true;
		}
		SetDirection(m_MonsterAI.SetMonsterLook(m_MonsterAI.MonsterPathing(m_MonsterAI.m_Path[0]), m_vObjectPosition));
		m_MonsterAI.SetTransition(MONSTER_EVENT_PATROL);
		if ((int)m_vObjectPosition.x == (int)m_MonsterAI.MonsterPathing(m_MonsterAI.m_Path[0]).x
			&& (int)m_vObjectPosition.z == (int)m_MonsterAI.MonsterPathing(m_MonsterAI.m_Path[0]).z)
		{
			isCollision = false;
			GotoPlayer = false;
		}
	}
	else
	{
		DistanceToHero = DistanceMonstertoHero();
		if (DistanceToHero < 30.0f)
		{
			m_MonsterAI.SetTransition(MONSTER_EVENT_FINDTARGET);
			SetDirection(m_MonsterAI.SetMonsterLook(m_vHeroPosition, m_vObjectPosition));
			IsFindTarget = true;
		}
		else if (DistanceToHero > 30.0f && !IsFindTarget)
		{
			if (!m_MonsterAI.HavePath)
			{
				//if (m_MonsterAI.NoSerachTarget)	//���� ó�� ĳ���Ϳ� ���� ����� ������������ ���
				//{
				//ĳ���Ϳ� ���� ����� ���������� ã����
				m_MonsterAI.m_Graph->Dijkstra_AG(m_MonsterAI.ReturnMostCloseHerotoSpawnPoint(m_vObjectPosition), m_MonsterAI.ReturnMostCloseHerotoSpawnPoint(m_vHeroPosition), m_MonsterAI.m_Path);
				m_MonsterAI.NoSerachTarget = !m_MonsterAI.NoSerachTarget;
				/*}
				else
				{
				m_MonsterAI.m_Graph->Dijkstra_AG(m_MonsterAI.ReturnMostCloseHerotoSpawnPoint(m_vObjectPosition), m_MonsterAI.ReturnRandomSpawn(m_vObjectPosition), m_MonsterAI.m_Path);
				m_MonsterAI.NoSerachTarget = !m_MonsterAI.NoSerachTarget;
				}*/
				m_MonsterAI.HavePath = true;
			}
			if ((int)m_vObjectPosition.x == (int)m_MonsterAI.MonsterPathing(m_MonsterAI.m_Path[0]).x
				&& (int)m_vObjectPosition.z == (int)m_MonsterAI.MonsterPathing(m_MonsterAI.m_Path[0]).z)
			{
				// 0������ �̵��� 0�� ���� �� 1������ ����
				//int Num = sizeof(m_MonsterAI.m_Path) / sizeof(m_MonsterAI.m_Path[0])-1;
				if (m_MonsterAI.m_Path[1] == -1)
				{
					m_MonsterAI.HavePath = false;
					return;
				}
				else
				{
					int m_PathCopy[10];
					for (int i = 0; i < 10; i++)
					{
						m_PathCopy[i] = -1;
					}
					for (int i = 0; i < 10; i++)
					{
						if (i != 9)
							m_PathCopy[i] = m_MonsterAI.m_Path[i + 1];
						else if (i == 9)
						{
							m_PathCopy[i] = -1;
						}
					}
					for (int i = 0; i < 10; i++)
					{
						m_MonsterAI.m_Path[i] = -1;
					}
					for (int i = 0; i < 10; i++)
					{
						m_MonsterAI.m_Path[i] = m_PathCopy[i];
					}
				}
			}
			if (m_MonsterAI.m_Path[0] != -1)
			{
				SetDirection(m_MonsterAI.SetMonsterLook(m_MonsterAI.MonsterPathing(m_MonsterAI.m_Path[0]), m_vObjectPosition));
				m_MonsterAI.SetTransition(MONSTER_EVENT_PATROL);
			}
		}
		else if (DistanceToHero > 30.0f && IsFindTarget)
		{
			m_MonsterAI.SetTransition(MONSTER_EVENT_PATROL);
			SetDirection(m_MonsterAI.SetMonsterLook(m_vHeroPosition, m_vObjectPosition));
		}
	}
}
#endif


void LAGunner::MoveUp()
{

#ifdef TOOL_MODE
#else
	/*=================== UI ���� ========================*/


#endif

	m_iCurrentMotion = (int)E_CharacterMotion::WORK_UP;

	LAUnit::MoveUp();
}

void LAGunner::MoveRun()
{

#ifdef TOOL_MODE
#else
	/*=================== UI ���� ========================*/


#endif

	m_iCurrentMotion = (int)E_CharacterMotion::RUN;

	LAUnit::MoveRun();
}

void LAGunner::Attack()
{

#ifdef TOOL_MODE
#else
	/*=================== UI ���� ========================*/


#endif

	m_iCurrentMotion = (int)E_CharacterMotion::RIFLE_ACTION;

	LAUnit::Attack();
}

void LAGunner::Death()
{

#ifdef TOOL_MODE
#else
	/*=================== UI ���� ========================*/


#endif

	//�״¸�� ����.

	LAUnit::Death();
}

bool LAGunner::UpdateBulletCollition()
{
	/*=================== Bullet �浹 ========================*/



	return true;
}

void LAGunner::ResetCheck()
{
	LATopObject::ResetCheck();

	m_bIsReLoad = false;
	m_pItemControl->ResetCheckTick();
}

void LAGunner::SetCurrentMotion(int isetMotion)
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
	case (int)E_CharacterMotion::RIFLE_AFTER_SET:
	{
		m_iCurrentMotion = isetMotion;
		//��Ʈ�ѽý��ۿ� ������ ����
		m_pItemControl->SetCurrentWeapon(E_WeaponType::RIFLE);
	}break;

	case (int)E_CharacterMotion::RIFLE_RELOAD:
	{
		//�κ��� �Ѿ��� ������쿡�� ��������
		if (m_pItemControl->CheckInvenBullet(E_WeaponType::RIFLE))
		{
			m_iCurrentMotion = isetMotion;
			m_bIsReLoad = true;

#ifdef TOOL_MODE

#else
			//ReLoad �������
			LA_SOUND2_MGR.playeffect(m_pItemControl->m_pWeaponList[(int)m_pItemControl->GetCurrentWeaponType()]->m_SondIndexList[(int)E_WeaponSound::SNDEFFECT_RELOAD]);
#endif

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
			}
		}

	}break;
	/*================================== Handgun ====================================*/
	case (int)E_CharacterMotion::HANDGUN_AFTER_SET:
	{
		m_iCurrentMotion = isetMotion;
		//��Ʈ�ѽý��ۿ� ������ ����
		m_pItemControl->SetCurrentWeapon(E_WeaponType::HANDGUN);
	}break;

	/*================================== Knife ====================================*/
	case (int)E_CharacterMotion::KNIFE_AFTER_SET:
	{
		m_iCurrentMotion = isetMotion;
		//��Ʈ�ѽý��ۿ� ������ ����
		m_pItemControl->SetCurrentWeapon(E_WeaponType::KNIFE);
	}break;

	/*================================== Bomb ====================================*/
	case (int)E_CharacterMotion::BOMB_AFTER_SET:
	{
		m_iCurrentMotion = isetMotion;
		//��Ʈ�ѽý��ۿ� ������ ����
		m_pItemControl->SetCurrentWeapon(E_WeaponType::BOMB);

		//����ź üũ
		/*if (m_ObjectAgregateData.iInventoryBombCount < 0)
		{
		m_bZeroBulletCount = true;
		}
		else
		{
		m_bZeroBulletCount = false;
		}*/


	}break;

	case (int)E_CharacterMotion::BOMB_RELOAD:
	{
		//�κ��� �Ѿ��� ������쿡�� ��������
		if (m_pItemControl->m_bIsBombRender == false)
		{
			if (m_pItemControl->CheckInvenBullet(E_WeaponType::RIFLE))
			{
				m_iCurrentMotion = isetMotion;
				m_bIsReLoad = true;

#ifdef TOOL_MODE

#else
				//ReLoad �������
				LA_SOUND2_MGR.playeffect(m_pItemControl->m_pWeaponList[(int)m_pItemControl->GetCurrentWeaponType()]->m_SondIndexList[(int)E_WeaponSound::SNDEFFECT_RELOAD]);
#endif
			}
			/*else
			{
			m_bZeroBulletCount = true;
			}*/
		}
	}break;

	case (int)E_CharacterMotion::BOMB_ACTION:
	{
		if ((!m_bIsReLoad) && (!m_bIsShot))
		{
			if (m_pItemControl->CheckInvenBullet(E_WeaponType::BOMB))
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
		//��Ʈ�ѽý��ۿ� ������ ����
		m_pItemControl->SetCurrentWeapon(E_WeaponType::MEDIC);
	}break;

	default:
	{
		m_iCurrentMotion = (int)E_CharacterMotion::IDLE;
	}break;
	}
}

void  LAGunner::UpdateAddAnimationSpeed()
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

void LAGunner::SetMatrix(TMatrix* pWorldMatrix, TMatrix* pViewMatrix, TMatrix* pProjectionMatrix)
{
	LATopObject::SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);

	m_pItemControl->SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);
}

void LAGunner::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	LAUnit::Init(pDevice, pContext);
	if (LOAD_OBJECT_MGR.CreateWeapon(TVector3(0, 0, 0), 6, 1000))
	{
		LOAD_OBJECT_MGR.MonsterInItem(E_LoadTableType::WEAPON, this, LOAD_OBJECT_MGR.GetWeaponSize() - 1);
	}
#ifdef TOOL_MODE
#else
	m_MonsterAI.NumOfMonster = 1;
	m_MonsterAI.Init();
#endif
}

bool LAGunner::Frame()
{

#ifdef TOOL_MODE
#else

	//UI ������ (�𷺼�, ���)

	/*=========================== ��� �ӽ� �׽�Ʈ ===============================*/
	//int iNum = rand() % 1000;
	//if ((iNum > 0) && (iNum < 10))
	//{
	//	SetCurrentMotion((int)E_CharacterMotion::RIFLE_ACTION);
	//}
	//else// if ((iNum > 20) && (iNum < 30))
	//{
	//	SetCurrentMotion((int)E_CharacterMotion::WORK_UP);
	//}
	MonsterEventOccurrence();
	MonsterMotionStateSet(m_MonsterAI.m_dwCurrentState);

#endif

	UpdateAddAnimationSpeed();

	LAUnit::Frame();

	/*========================== ������ ��Ʈ�� Bullet ������Ʈ ==============================*/
	m_pItemControl->UpdateBullet(m_MainResourceNumber, m_iCurrentMotion, m_fElapseFrame, this);

	m_bisMove = false;
	/*========================== ������ ��Ʈ�� �ý��� ������Ʈ ==============================*/
	m_pItemControl->Frame();

	/*========================== Bullet �߻�� �浹 üũ ==============================*/
	UpdateBulletCollition();

	return true;
}

bool LAGunner::Render()
{
	//�������� ������ �θ������Ʈ ��������, 
	LAUnit::Render();

#ifdef TOOL_MODE
#else

	/*========================== ���� ������ ������Ʈ ==============================*/
	m_pItemControl->SetWeaponPosition(m_MainResourceNumber);

	/*========================== ���� ���� ==============================*/
	m_pItemControl->Render();

#endif

	return true;
}

bool LAGunner::Release()
{
	LAUnit::Release();

#ifdef TOOL_MODE
#else

	/*========================== ������ ��Ʈ�� �ý��� ������ ==============================*/
	m_pItemControl->Release();
	delete m_pItemControl;
	m_pItemControl = NULL;

#endif

	return true;
}


LAGunner::~LAGunner()
{
}
