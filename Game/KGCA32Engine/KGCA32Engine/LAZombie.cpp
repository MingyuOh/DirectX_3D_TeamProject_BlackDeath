
////#include "stdafx.h"


#include "LAZombie.h"
#include "LALoadObjectManager.h"

#ifdef TOOL_MODE
#else
/*===================== UI Include ========================*/

#endif


#ifdef TOOL_MODE
#else 

/*===================== UI 함수 ========================*/
float LAZombie::DistanceMonstertoHero()
{
	float Dis_X = m_vHeroPosition.x - m_vObjectPosition.x;
	float Dis_Y = m_vHeroPosition.y - m_vObjectPosition.y;
	float Dis_Z = m_vHeroPosition.z - m_vObjectPosition.z;

	float Distance = sqrt(pow(Dis_X, 2) + pow(Dis_Y, 2) + pow(Dis_Z, 2));

	return Distance;
}
void  LAZombie::MonsterMontionStateSet(int MotionNumber)
{
	switch (MotionNumber)
	{
	case MONSTER_ZOMBIE_STATE_IDLE:
	{

	}break;
	case MONSTER_ZOMBIE_STATE_MOVE:
	{
		MoveUp();
	}break;
	case MONSTER_ZOMBIE_STATE_DASH:
	{
		MoveRun();
	}break;
	case MONSTER_ZOMBIE_STATE_ATTACK:
	{
		m_vMoveUpdatePosition.x = 0.0f;
		m_vMoveUpdatePosition.y = 0.0f;
		m_vMoveUpdatePosition.z = 0.0f;
		Attack();

		if (LOAD_OBJECT_MGR.m_Collition.CheckSphereInSphere(m_BoundingBox.m_vCenter, LOAD_OBJECT_MGR.GetHero()->m_BoundingBox.m_vCenter,
			m_BoundingBox.m_fRadius, LOAD_OBJECT_MGR.GetHero()->m_BoundingBox.m_fRadius + 1))
		{
			if (CheckPositionTick(10, false))	//프레임의 40퍼센트가 지났을때
			{
				LOAD_OBJECT_MGR.GetHero()->SubtractHpDamage(m_pSkillData->m_SkillTableLoadData.fAttackPower);
				LOAD_OBJECT_MGR.GetHero()->m_bIsDamaged = true;
				LOAD_OBJECT_MGR.GetHero()->m_DamageEffect = I_ParticleSystem.GetValue(HeroDamaged);
				LOAD_OBJECT_MGR.GetHero()->m_DamageEffect.CreateBoundingBox(TVector3(50.0f, 50.0f, 50.0f));

				//m_ObjectData.iHp -= 50;
			}
		}
		else
			LOAD_OBJECT_MGR.GetHero()->m_bIsDamaged = false;

	}break;
	case MONSTER_ZOMBIE_STATE_DEAD:
	{
		Death();
	}break;
	}
}

void  LAZombie::MonsterEventOccurrence()
{
	//몬스터 체력 0이하시 데드이벤트
	if (m_ObjectAgregateData.iCurrentHp <= 0)
	{
		m_MonsterAI.SetTransition(MONSTER_EVENT_DEAD);
		return;
	}
	//
	if (isCollision)	//충돌이 발생했을때
	{
		//if (CollisionTick < 2.0f)	//2초동안 반대방향으로 2초동안 이동
		//{
		CollisionTick += g_fSecPerFrame;
		//경로를 캐릭터의 가장 가까운곳으로 재설정
		IsFindTarget = false;
		if (!isReverse)
		{
			SetDirection(-m_vObjectLook);
			isReverse = true;
		}
		m_MonsterAI.SetTransition(MONSTER_EVENT_PATROL);
		if (CollisionTick > 2.0f)	//1초가 지났을때
		{
			isReverse = false;
			if (!GotoPlayer)
			{
				//가장 가까운 스폰지역을 탐색
				for (int iPath = 0; iPath < 10; iPath++)	//기존의 경로를 -1로 초기화
				{
					m_MonsterAI.m_Path[iPath] = -1;
				}
				m_MonsterAI.m_Path[0] = m_MonsterAI.ReturnMostCloseHerotoSpawnPoint(m_vObjectPosition);
				GotoPlayer = true;
			}
			SetDirection(m_MonsterAI.SetMonsterLook(m_MonsterAI.MonsterPathing(m_MonsterAI.m_Path[0]), m_vObjectPosition));
			m_MonsterAI.SetTransition(MONSTER_EVENT_PATROL);
			if ((int)m_vObjectPosition.x == (int)m_MonsterAI.MonsterPathing(m_MonsterAI.m_Path[0]).x
				&& (int)m_vObjectPosition.z == (int)m_MonsterAI.MonsterPathing(m_MonsterAI.m_Path[0]).z)
			{
				CollisionTick = 0.0f;
				isCollision = false;
				GotoPlayer = false;
			}
			else if (IsDamaged)
			{
				CollisionTick = 0.0f;
				isCollision = false;
				GotoPlayer = false;
				IsFindTarget = true;
				IsDamaged = false;
			}
		}
	}
	else {
		if (IsDamaged)
		{
			IsFindTarget = true;
			IsDamaged = false;
		}
		DistanceToHero = DistanceMonstertoHero();
		if (DistanceToHero < 30.0f && DistanceToHero > 3.0f && !isCollision)
		{
			m_MonsterAI.SetTransition(MONSTER_EVENT_FINDTARGET);
			SetDirection(m_MonsterAI.SetMonsterLook(m_vHeroPosition, m_vObjectPosition));
			IsFindTarget = true;
		}
		else if (DistanceToHero > 30.0f && !IsFindTarget)
		{
			if (!m_MonsterAI.HavePath)
			{
				//if (m_MonsterAI.NoSerachTarget)	//제일 처음 캐릭터와 가장 가까운 스폰지역으로 경로
				//{
				//캐릭터와 제일 가까운 스폰지역을 찾도록
				m_MonsterAI.m_Graph->Dijkstra_AG(m_MonsterAI.ReturnMostCloseHerotoSpawnPoint(m_vObjectPosition), m_MonsterAI.ReturnRandomSpawn(m_vObjectPosition), m_MonsterAI.m_Path);
				//m_MonsterAI.m_Graph->Dijkstra_AG(m_MonsterAI.ReturnMostCloseHerotoSpawnPoint(m_vObjectPosition), m_MonsterAI.ReturnMostCloseHerotoSpawnPoint(m_vHeroPosition), m_MonsterAI.m_Path);
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
				// 0번까지 이동후 0번 삭제 후 1번부터 복사
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
		else if (DistanceToHero > 30.0f && IsFindTarget)	//이미 캐릭터를 한번 발견한 경우
		{
			m_MonsterAI.SetTransition(MONSTER_EVENT_FINDTARGET);
			SetDirection(m_MonsterAI.SetMonsterLook(m_vHeroPosition, m_vObjectPosition));
		}
		else if (DistanceToHero < 3.0f)
			m_MonsterAI.SetTransition(MONSTER_EVENT_ATTACK);
	}
}



#endif

void LAZombie::MoveUp()
{
#ifdef TOOL_MODE
#else
	LA_SOUND2_MGR.play(m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_MOV]);
	LA_SOUND2_MGR.stop(m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_RUN]);
	LA_SOUND2_MGR.stop(m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_DEAD]);
	LA_SOUND2_MGR.stop(m_pSkillData->m_SkillSoundIndex[0]);
#endif

	m_iCurrentMotion = (int)E_MonsterMotion::WORK;

	LAUnit::MoveUp();
}

void LAZombie::MoveRun()
{
	if (!m_bisMotion)
	{
#ifdef TOOL_MODE
#else
		LA_SOUND2_MGR.play(m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_RUN]);
		LA_SOUND2_MGR.stop(m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_MOV]);
		LA_SOUND2_MGR.stop(m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_DEAD]);
		LA_SOUND2_MGR.stop(m_pSkillData->m_SkillSoundIndex[0]);
#endif

		m_iCurrentMotion = (int)E_MonsterMotion::WORK;
	}



	LAUnit::MoveRun();
}

void LAZombie::Attack()
{
#ifdef TOOL_MODE
#else
	//몬스터 공격 사운드 루프로
	LA_SOUND2_MGR.m_pSound[m_pSkillData->m_SkillSoundIndex[0]]->setMode(FMOD_LOOP_NORMAL);
	LA_SOUND2_MGR.play(m_pSkillData->m_SkillSoundIndex[0]);
	//
	LA_SOUND2_MGR.stop(m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_RUN]);
	LA_SOUND2_MGR.stop(m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_MOV]);
	LA_SOUND2_MGR.stop(m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_DEAD]);
#endif

	m_iCurrentMotion = (int)E_MonsterMotion::ATTACK;
	m_bisMotion = true;

	LAUnit::Attack();
}

void LAZombie::Death()
{
#ifdef TOOL_MODE
#else
	LA_SOUND2_MGR.m_pSound[m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_DEAD]]->setMode(FMOD_LOOP_OFF);
	LA_SOUND2_MGR.play(m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_DEAD]);
	LA_SOUND2_MGR.stop(m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_RUN]);
	LA_SOUND2_MGR.stop(m_SondIndexList[(int)E_MonsterSound::SNDEFFECT_MOB1_MOV]);
	LA_SOUND2_MGR.stop(m_pSkillData->m_SkillSoundIndex[0]);
#endif

	m_iCurrentMotion = (int)E_MonsterMotion::DEATH;
	m_bIsObjectLife = false;

	LAUnit::Death();
}

bool LAZombie::Frame()
{

#ifdef TOOL_MODE
#else
	//UI 프레임 
	MonsterEventOccurrence();
	MonsterMontionStateSet(m_MonsterAI.m_dwCurrentState);

#endif

	LAUnit::Frame();

	return true;
}

LAZombie::LAZombie()
{
#ifdef TOOL_MODE
#else
	isReverse = false;
#endif
}


LAZombie::~LAZombie()
{
}
