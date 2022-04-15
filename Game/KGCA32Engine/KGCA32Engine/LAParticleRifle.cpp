#include "LAParticleRifle.h"

#include "LACoreLinkStd.h"

//--------------------------------------------------------------------------------------
// ������ �Ѿ� / ��ƼŬ ����
//--------------------------------------------------------------------------------------
void LAParticleRifle::ParticleInit(ObjectTableLoadData& rTableLoadData)
{
	m_LoadTableData = rTableLoadData;
	m_Bullet.resize(m_LoadTableData.iBulletCount);
}

//--------------------------------------------------------------------------------------
// ������ ��ƼŬ ������
//--------------------------------------------------------------------------------------
void LAParticleRifle::ParticleFrame()
{
	RifleFrame();
}

//--------------------------------------------------------------------------------------
// ������ �Ѿ� / ��ƼŬ ������
//--------------------------------------------------------------------------------------
void LAParticleRifle::ParticleRender()
{
	//��ƼŬ ȣ��
	for (int iShot = 0; iShot < m_ParticleSystem.size(); iShot++)
	{
		m_ParticleSystem[iShot].Render();
	}
}

//--------------------------------------------------------------------------------------
// ������ �Ѿ� / ��ƼŬ ������
//--------------------------------------------------------------------------------------
void LAParticleRifle::ParticleRelease()
{
	// �Ѿ� ����
	DeleteVector(m_Bullet);

	// ����Ʈ ����
	for (int iShot = 0; iShot < m_ParticleSystem.size(); iShot++)
	{
		m_ParticleSystem[iShot].Release();
	}
}

//--------------------------------------------------------------------------------------
// ������ ��ƼŬ ������
//--------------------------------------------------------------------------------------
void LAParticleRifle::DrawParticle()
{
	// �ѱ�ȭ�� ����Ʈ ������ ����
	for (int iShot = 0; iShot < m_ParticleSystem.size(); iShot++)
	{
		// ��ƼŬ�ý��ۿ� ��ƼŬ�� �������������� ����
		if (m_ParticleSystem[iShot].m_Particle.size() <= 0)
		{
			m_ParticleSystem.erase(m_ParticleSystem.begin() + iShot);
			continue;
		}

		for (int iEffect = 0; iEffect < m_ParticleSystem[iShot].m_Particle.size(); iEffect++)
		{
			m_ParticleSystem[iShot].m_Particle[iEffect].m_vPos = m_vParticlePos;
			m_ParticleSystem[iShot].m_Particle[iEffect].m_vDirection = m_vParticleDir;
		}
		m_ParticleSystem[iShot].Frame();
	}

	ParticleRender();

}

//--------------------------------------------------------------------------------------
// ������ �Ѿ� ������
//--------------------------------------------------------------------------------------
void LAParticleRifle::RifleFrame()
{
	// �Ѿ� �߻�
	for (int iBullet = 0; iBullet < m_iShotIndex; iBullet++)
	{
		if (m_Bullet[iBullet].m_bTrigger)
		{
			// �Ѿ� �ǽð� ����
			m_Bullet[iBullet].m_BoundingBox.CreateBoundingBox(TVector3(0.1f, 0.1f, 0.1f), TVector3(-0.1f, -0.1f, -0.1f), m_Bullet[iBullet].m_vBulletPos);
			m_Bullet[iBullet].UpdateBullet();

			// �Ѿ��� ������Ʈ�� �浹�� ����
			TVector4	vMapCheck = m_pMapParser->GetHeight(m_Bullet[iBullet].m_vBulletPos, 0, &m_Bullet[iBullet].m_bMapCheck);
			if (m_Bullet[iBullet].m_bMapCheck == true)
			{
				m_Bullet.erase(m_Bullet.begin() + iBullet);
				m_iShotIndex--;
				continue;
			}

			// �Ѿ� ��Ÿ� ���� �Ѿ�� ����
			if (!m_Bullet[iBullet].CheckBulletIntersection())
			{
				m_Bullet.erase(m_Bullet.begin() + iBullet);
				m_iShotIndex--;
			}
		}
	}
}

//--------------------------------------------------------------------------------------
// ������ �߻� �Լ�
//--------------------------------------------------------------------------------------
void LAParticleRifle::Shot()
{
	m_Bullet[m_iShotIndex].m_bTrigger = true;
	m_Bullet[m_iShotIndex].SetBulletDir(m_vParticleDir);
	m_Bullet[m_iShotIndex].SetBulletPos(m_vParticlePos);
	m_iShotIndex++;
	ParticleSystem AddParticle;
	AddParticle = I_ParticleSystem.GetValue(RifleShotEffect);
	AddParticle.m_bRotationEffect = true;
	m_ParticleSystem.push_back(AddParticle);

	if(m_bHeroShot == true)
		g_pMainCamera->CameraRebound();
}

//--------------------------------------------------------------------------------------
// ������ �Ѿ� / ��ƼŬ ��ġ ���� �Լ�
//--------------------------------------------------------------------------------------
void LAParticleRifle::SetParticlePos(TVector3 vParticlePos)
{
	m_vParticlePos = vParticlePos;
}

//--------------------------------------------------------------------------------------
// ������ �Ѿ� / ��ƼŬ �𷺼� ���� �Լ�
//--------------------------------------------------------------------------------------
void LAParticleRifle::SetParticleDir(TVector3 vParticleDir)
{
	m_vParticleDir = vParticleDir;
}

//--------------------------------------------------------------------------------------
// ������ �Ѿ� �浹ó�� �Լ�
//--------------------------------------------------------------------------------------
bool LAParticleRifle::ProcessingCollision(BBoundingBox& bRelativeBox, TVector3& vPos)
{
	for (int iShot = 0; iShot < m_iShotIndex; iShot++)
	{
		if (m_Bullet[iShot].m_bTrigger)
		{
			if (m_Bullet[iShot].m_BoundingBox.m_Collision.CheckSphereInSphere(
				m_Bullet[iShot].m_BoundingBox.m_vCenter,
				bRelativeBox.m_vCenter,
				m_Bullet[iShot].m_BoundingBox.m_fRadius,
				bRelativeBox.m_fRadius))
			{
				vPos = m_Bullet[iShot].m_BoundingBox.m_Collision.SphereInSphereOppositeDir(
					m_Bullet[iShot].m_BoundingBox.m_vCenter,
					bRelativeBox.m_vCenter,
					m_Bullet[iShot].m_BoundingBox.m_fRadius,
					bRelativeBox.m_fRadius);
				m_Bullet.erase(m_Bullet.begin() + iShot);
				m_iShotIndex--;
				return true;
			}
		}
	}
	return false;
}

//--------------------------------------------------------------------------------------
// ������ �Ѿ� ������ �Լ�
//--------------------------------------------------------------------------------------
void LAParticleRifle::ReloadBullet(int iBulletCount)
{
	for (int iCnt = 0; iCnt < iBulletCount; iCnt++)
	{
		BBullet addBullet;
		m_Bullet.push_back(addBullet);
	}
}

LAParticleRifle::LAParticleRifle() :m_iShotIndex(0)
{
}


LAParticleRifle::~LAParticleRifle()
{
}
