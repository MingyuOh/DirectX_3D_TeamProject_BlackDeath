#include "LAParticleRifle.h"

#include "LACoreLinkStd.h"

//--------------------------------------------------------------------------------------
// 라이플 총알 / 파티클 인잇
//--------------------------------------------------------------------------------------
void LAParticleRifle::ParticleInit(ObjectTableLoadData& rTableLoadData)
{
	m_LoadTableData = rTableLoadData;
	m_Bullet.resize(m_LoadTableData.iBulletCount);
}

//--------------------------------------------------------------------------------------
// 라이플 파티클 프레임
//--------------------------------------------------------------------------------------
void LAParticleRifle::ParticleFrame()
{
	RifleFrame();
}

//--------------------------------------------------------------------------------------
// 라이플 총알 / 파티클 렌더링
//--------------------------------------------------------------------------------------
void LAParticleRifle::ParticleRender()
{
	//파티클 호출
	for (int iShot = 0; iShot < m_ParticleSystem.size(); iShot++)
	{
		m_ParticleSystem[iShot].Render();
	}
}

//--------------------------------------------------------------------------------------
// 라이플 총알 / 파티클 릴리즈
//--------------------------------------------------------------------------------------
void LAParticleRifle::ParticleRelease()
{
	// 총알 제거
	DeleteVector(m_Bullet);

	// 이펙트 제거
	for (int iShot = 0; iShot < m_ParticleSystem.size(); iShot++)
	{
		m_ParticleSystem[iShot].Release();
	}
}

//--------------------------------------------------------------------------------------
// 라이플 파티클 렌더링
//--------------------------------------------------------------------------------------
void LAParticleRifle::DrawParticle()
{
	// 총구화염 이펙트 포지션 셋팅
	for (int iShot = 0; iShot < m_ParticleSystem.size(); iShot++)
	{
		// 파티클시스템에 파티클이 존재하지않으면 삭제
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
// 라이플 총알 프레임
//--------------------------------------------------------------------------------------
void LAParticleRifle::RifleFrame()
{
	// 총알 발사
	for (int iBullet = 0; iBullet < m_iShotIndex; iBullet++)
	{
		if (m_Bullet[iBullet].m_bTrigger)
		{
			// 총알 실시간 갱신
			m_Bullet[iBullet].m_BoundingBox.CreateBoundingBox(TVector3(0.1f, 0.1f, 0.1f), TVector3(-0.1f, -0.1f, -0.1f), m_Bullet[iBullet].m_vBulletPos);
			m_Bullet[iBullet].UpdateBullet();

			// 총알이 오브젝트와 충돌시 삭제
			TVector4	vMapCheck = m_pMapParser->GetHeight(m_Bullet[iBullet].m_vBulletPos, 0, &m_Bullet[iBullet].m_bMapCheck);
			if (m_Bullet[iBullet].m_bMapCheck == true)
			{
				m_Bullet.erase(m_Bullet.begin() + iBullet);
				m_iShotIndex--;
				continue;
			}

			// 총알 사거리 범위 넘어가면 삭제
			if (!m_Bullet[iBullet].CheckBulletIntersection())
			{
				m_Bullet.erase(m_Bullet.begin() + iBullet);
				m_iShotIndex--;
			}
		}
	}
}

//--------------------------------------------------------------------------------------
// 라이플 발사 함수
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
// 라이플 총알 / 파티클 위치 셋팅 함수
//--------------------------------------------------------------------------------------
void LAParticleRifle::SetParticlePos(TVector3 vParticlePos)
{
	m_vParticlePos = vParticlePos;
}

//--------------------------------------------------------------------------------------
// 라이플 총알 / 파티클 디렉션 셋팅 함수
//--------------------------------------------------------------------------------------
void LAParticleRifle::SetParticleDir(TVector3 vParticleDir)
{
	m_vParticleDir = vParticleDir;
}

//--------------------------------------------------------------------------------------
// 라이플 총알 충돌처리 함수
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
// 라이플 총알 재장전 함수
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
