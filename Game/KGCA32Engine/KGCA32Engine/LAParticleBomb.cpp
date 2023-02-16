#include "LAParticleBomb.h"

#include "LACoreLinkStd.h"
#include "LALoadObjectManager.h"

//--------------------------------------------------------------------------------------
// ����ź / ��ƼŬ ����
//--------------------------------------------------------------------------------------
void LAParticleBomb::ParticleInit(ObjectTableLoadData& rTableLoadData)
{
	m_LoadTableData = rTableLoadData;
}

//--------------------------------------------------------------------------------------
// ����ź ������
//--------------------------------------------------------------------------------------
void LAParticleBomb::GrenadeFrame()
{
	for (int iGrenade = 0; iGrenade < m_Grenade.size(); iGrenade++)
	{
		// ����ź�� ���������� üũ
		if (m_Grenade[iGrenade].m_bTrigger)
		{
			// ����ź�� ������Ʈ�� �浹 üũ
			TVector4	vMapCheck = m_pMapParser->GetHeight(m_Grenade[iGrenade].m_vGrenadePos, 0, &m_Grenade[iGrenade].m_bMapCheck);

			if (m_Grenade[iGrenade].m_vGrenadePos.y <= 0.0f)
			{
				// ����ź �ٴڿ� ����(�浹�� ����� �������� ó�� �߰�)
				m_Grenade[iGrenade].m_vGrenadePos.y = 0.0f;
				// ����ź �ٴڿ� ������ ���� �ð����� �ð� �߰�
				m_Grenade[iGrenade].m_fGrenadeSeatedFloorTime += g_fSecPerFrame;
				// ����ź ������ �ð�
				if (m_Grenade[iGrenade].m_fGrenadeSeatedFloorTime >= m_Grenade[iGrenade].m_fExplosionDelayTime)
				{
					// ����ź ����Ʈ �߰�
					m_bIsBomb = true;
					m_ParticleSystem.push_back(I_ParticleSystem.GetValue(GrenadeEffect));
					for (int iExp = 0; iExp < m_ParticleSystem.size(); iExp++)
					{
						// ����ź �ٿ�� �ڽ� ����
						m_ParticleSystem[iExp].CreateBoundingBox(m_Grenade[iGrenade].m_vGrenadePos);
						for (int iEffect = 0; iEffect < m_ParticleSystem[iExp].m_Particle.size(); iEffect++)
						{
							// ����ź ����Ʈ ��ġ ����
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
					// ������ ����ź ����
					m_Grenade.erase(m_Grenade.begin() + iGrenade);
				}
			}
			else
			{
				// ����ź �� ������������ �������� �� ������ ������
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
// ����ź ��ƼŬ ������ (��� ������ ������ �κ�)
//--------------------------------------------------------------------------------------
void LAParticleBomb::ParticleFrame()
{
	GrenadeFrame();
}

//--------------------------------------------------------------------------------------
// ����ź ��ƼŬ ������
//--------------------------------------------------------------------------------------
void LAParticleBomb::ParticleRender()
{
	//��ƼŬ ȣ��
	for (int iGrenade = 0; iGrenade < m_ParticleSystem.size(); iGrenade++)
		m_ParticleSystem[iGrenade].Render();
}

//--------------------------------------------------------------------------------------
// ����ź / ��ƼŬ ������
//--------------------------------------------------------------------------------------
void LAParticleBomb::ParticleRelease()
{
	// ����ź ����
	DeleteVector(m_Grenade);

	// ����Ʈ ����
	for (int iGrenade = 0; iGrenade < m_ParticleSystem.size(); iGrenade++)
		m_ParticleSystem[iGrenade].Release();
}

//--------------------------------------------------------------------------------------
// ����ź ��ƼŬ ������ (��� ���� ������ �κ�)
//--------------------------------------------------------------------------------------
void LAParticleBomb::DrawParticle()
{
	// ����ź ����Ʈ
	for (int iGrenade = 0; iGrenade < m_ParticleSystem.size(); iGrenade++)
	{

		// ��ƼŬ�ý��ۿ� ��ƼŬ�� �������������� ����
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

	//��ƼŬ ȣ��
	ParticleRender();
}

//--------------------------------------------------------------------------------------
// ����ź / ��ƼŬ ��ġ ���� �Լ�
//--------------------------------------------------------------------------------------
void LAParticleBomb::SetParticlePos(TVector3 vParticlePos)
{
	m_vParticlePos = vParticlePos;
}

//--------------------------------------------------------------------------------------
// ����ź / ��ƼŬ �𷺼� ���� �Լ�
//--------------------------------------------------------------------------------------
void LAParticleBomb::SetParticleDir(TVector3 vParticleDir)
{
	m_vParticleDir = vParticleDir;
}

//--------------------------------------------------------------------------------------
// ����ź ������ �Լ�
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
// ����ź �浹ó��
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
