#include "LAParticleBomb.h"

#include "LACoreLinkStd.h"
#include "LALoadObjectManager.h"

//--------------------------------------------------------------------------------------
// 수류탄 / 파티클 인잇
//--------------------------------------------------------------------------------------
void LAParticleBomb::ParticleInit(ObjectTableLoadData& rTableLoadData)
{
	m_LoadTableData = rTableLoadData;
}

//--------------------------------------------------------------------------------------
// 수류탄 프레임
//--------------------------------------------------------------------------------------
void LAParticleBomb::GrenadeFrame()
{
	for (int iGrenade = 0; iGrenade < m_Grenade.size(); iGrenade++)
	{
		// 수류탄이 던져졌는지 체크
		if (m_Grenade[iGrenade].m_bTrigger)
		{
			// 수류탄이 오브젝트와 충돌 체크
			TVector4	vMapCheck = m_pMapParser->GetHeight(m_Grenade[iGrenade].m_vGrenadePos, 0, &m_Grenade[iGrenade].m_bMapCheck);

			if (m_Grenade[iGrenade].m_vGrenadePos.y <= 0.0f)
			{
				// 수류탄 바닥에 안착(충돌시 언덕에 떨어지면 처리 추가)
				m_Grenade[iGrenade].m_vGrenadePos.y = 0.0f;
				// 수류탄 바닥에 안착시 폭파 시간까지 시간 추가
				m_Grenade[iGrenade].m_fGrenadeSeatedFloorTime += g_fSecPerFrame;
				// 수류탄 터지는 시간
				if (m_Grenade[iGrenade].m_fGrenadeSeatedFloorTime >= m_Grenade[iGrenade].m_fExplosionDelayTime)
				{
					// 수류탄 이펙트 추가
					m_bIsBomb = true;
					m_ParticleSystem.push_back(I_ParticleSystem.GetValue(GrenadeEffect));
					for (int iExp = 0; iExp < m_ParticleSystem.size(); iExp++)
					{
						// 수류탄 바운딩 박스 생성
						m_ParticleSystem[iExp].CreateBoundingBox(m_Grenade[iGrenade].m_vGrenadePos);
						for (int iEffect = 0; iEffect < m_ParticleSystem[iExp].m_Particle.size(); iEffect++)
						{
							// 수류탄 이펙트 위치 셋팅
							m_ParticleSystem[iExp].m_Particle[iEffect].m_vPos = m_Grenade[iGrenade].m_vGrenadePos;
							m_ParticleSystem[iExp].m_Particle[iEffect].m_vPos.y += m_ParticleSystem[iExp].m_Particle[iEffect].m_vScale.x - 1.0f;
						}
					}

					std::vector<std::shared_ptr<LAUnit>>& data = LOAD_OBJECT_MGR.GetMonsterList();
					int iMonsterCount = data.size();
					for (int iObj = 0; iObj < iMonsterCount; iObj++)
					{
						TVector3 vDamaged;
						if (CollisionObjectInBombRadian(data[iObj]->m_vObjectPosition))
						{
							data[iObj]->SubtractHpDamage(m_Grenade[iGrenade].m_fGrenadeAttack);
						}
					}
					// 터지면 수류탄 삭제
					m_Grenade.erase(m_Grenade.begin() + iGrenade);
				}
			}
			else
			{
				// 수류탄 이 못가는지역에 도착했을 때 땅으로 떨어짐
				if (m_Grenade[iGrenade].m_bMapCheck == true)
				{	
					m_Grenade[iGrenade].m_vGrenadePos.y -= GRAVITY * g_fSecPerFrame;
				}
				else
				{
					m_Grenade[iGrenade].ThrowingAGrenade();
				}
			}
		}
	}
}

TVector3 LAParticleBomb::GetGrenadePosition()
{
	return m_Grenade[0].m_vGrenadePos;
}

bool LAParticleBomb::GetCheckBomb()
{
	return m_bIsBomb;
}

//--------------------------------------------------------------------------------------
// 수류탄 파티클 프레임 (계속 프레임 돌리는 부분)
//--------------------------------------------------------------------------------------
void LAParticleBomb::ParticleFrame()
{
	GrenadeFrame();
}

//--------------------------------------------------------------------------------------
// 수류탄 파티클 렌더링
//--------------------------------------------------------------------------------------
void LAParticleBomb::ParticleRender()
{
	//파티클 호출
	for (int iGrenade = 0; iGrenade < m_ParticleSystem.size(); iGrenade++)
		m_ParticleSystem[iGrenade].Render();
}

//--------------------------------------------------------------------------------------
// 수류탄 / 파티클 릴리즈
//--------------------------------------------------------------------------------------
void LAParticleBomb::ParticleRelease()
{
	// 수류탄 제거
	DeleteVector(m_Grenade);

	// 이펙트 제거
	for (int iGrenade = 0; iGrenade < m_ParticleSystem.size(); iGrenade++)
		m_ParticleSystem[iGrenade].Release();
}

//--------------------------------------------------------------------------------------
// 수류탄 파티클 렌더링 (계속 렌더 돌리는 부분)
//--------------------------------------------------------------------------------------
void LAParticleBomb::DrawParticle()
{
	// 수류탄 이펙트
	for (int iGrenade = 0; iGrenade < m_ParticleSystem.size(); iGrenade++)
	{

		// 파티클시스템에 파티클이 존재하지않으면 삭제
		if (m_ParticleSystem[iGrenade].m_Particle.size() <= 0)
		{
			m_ParticleSystem.erase(m_ParticleSystem.begin() + iGrenade);
			continue;
		}

		for (int iEffect = 0; iEffect < m_ParticleSystem[iGrenade].m_Particle.size(); iEffect++)
		{
			if (m_Grenade.size())
			{
				m_ParticleSystem[iGrenade].m_Particle[iEffect].m_vPos = m_Grenade[0].m_vGrenadePos;
				m_ParticleSystem[iGrenade].m_Particle[iEffect].m_vDirection = m_Grenade[0].m_vGrenadeDir;
			}
			
		}
		m_ParticleSystem[iGrenade].Frame();
	}

	//파티클 호출
	ParticleRender();
}

//--------------------------------------------------------------------------------------
// 수류탄 / 파티클 위치 셋팅 함수
//--------------------------------------------------------------------------------------
void LAParticleBomb::SetParticlePos(TVector3 vParticlePos)
{
	m_vParticlePos = vParticlePos;
}

//--------------------------------------------------------------------------------------
// 수류탄 / 파티클 디렉션 셋팅 함수
//--------------------------------------------------------------------------------------
void LAParticleBomb::SetParticleDir(TVector3 vParticleDir)
{
	m_vParticleDir = vParticleDir;
}

//--------------------------------------------------------------------------------------
// 수류탄 재장전 함수
//--------------------------------------------------------------------------------------
void LAParticleBomb::ReloadGrenade(int iGrenadeCount, TVector3 vParticlePos, TVector3 vParticleDir)
{
	for (int iGrenade = 0; iGrenade < iGrenadeCount; iGrenade++)
	{
		BGrenade addGrenade;
		addGrenade.m_bTrigger = true;
		addGrenade.SetGrenadePos(vParticlePos);
		addGrenade.SetGrenadeDir(vParticleDir);
		m_Grenade.push_back(addGrenade);
		m_bIsBomb = false;
	}
}

//--------------------------------------------------------------------------------------
// 수류탄 충돌처리
//--------------------------------------------------------------------------------------
bool LAParticleBomb::CollisionObjectInBombRadian(TVector3 vObjPos)
{
	for (int iGrenade = 0; iGrenade < m_Grenade.size(); iGrenade++)
	{
		float fEquation = pow(vObjPos.x - m_Grenade[iGrenade].m_vGrenadePos.x, 2) 
			+ pow(vObjPos.y - m_Grenade[iGrenade].m_vGrenadePos.y, 2) 
			+ pow(vObjPos.z - m_Grenade[iGrenade].m_vGrenadePos.z, 2);

		if (fEquation <= pow(m_Grenade[iGrenade].m_fGrenadeRadius, 2))
		{
			return true;
		}
	}
	return false;
}

LAParticleBomb::LAParticleBomb()
{
	m_bIsBomb = false;
}


LAParticleBomb::~LAParticleBomb()
{
}
