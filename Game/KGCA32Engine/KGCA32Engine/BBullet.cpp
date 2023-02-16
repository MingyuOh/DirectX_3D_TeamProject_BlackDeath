#include "BBullet.h"

//--------------------------------------------------------------------------------------
// 총알 요소 셋팅 함수
//--------------------------------------------------------------------------------------
void BBullet::SetBullet(float fBulletIntersection, float fOffensePower, float fBulletSpeed)
{
	m_fBulletIntersection = fBulletIntersection;
	m_fOffensePower = fOffensePower;
	m_fBulletSpeed = fBulletSpeed;
}

//--------------------------------------------------------------------------------------
// 총알 위치 셋팅 함수
//--------------------------------------------------------------------------------------
void BBullet::SetBulletPos(TVector3 vPos)
{
	m_vBulletPos = vPos;
	m_vBulletFirstPos = vPos;
}

//--------------------------------------------------------------------------------------
// 총알 방향 셋팅 함수
//--------------------------------------------------------------------------------------
void BBullet::SetBulletDir(TVector3 vDir)
{
	m_vBulletDir = vDir;
}

//--------------------------------------------------------------------------------------
// 총알 실시간 업데이트 함수
//--------------------------------------------------------------------------------------
void BBullet::UpdateBullet()
{
	TVector3 vCenter = m_BoundingBox.m_BoundingBoxMax + m_BoundingBox.m_BoundingBoxMin;
	vCenter /= 2.0f;

	// 속도 전진
	m_vBulletPos += m_vBulletDir * m_fBulletSpeed * g_fSecPerFrame;
	m_BoundingBox.m_vCenter = m_vBulletPos + vCenter;
}

//--------------------------------------------------------------------------------------
// 총알 사거리 체크
//--------------------------------------------------------------------------------------
bool BBullet::CheckBulletIntersection()
{
	if (m_vBulletDir.z < 0.0f)
	{
		// 총알 사거리 처리
		if (m_vBulletFirstPos.z + (m_vBulletDir.z * m_fBulletIntersection) > m_vBulletPos.z)
		{
			return false; // 사거리를 넘어감
		}
	}
	else
	{
		// 총알 사거리 처리
		if (m_vBulletFirstPos.z + (m_vBulletDir.z * m_fBulletIntersection) < m_vBulletPos.z)
		{
			return false; // 사거리를 넘어감
		}
	}
	return true; // 사거리 내에 존재
}

BBullet::BBullet()
{
	// 기본 소총 능력치
	m_fBulletSpeed			= 70.0f;
	m_fBulletIntersection	= 250.0f;
	m_fOffensePower			= 30.0f;
	m_vBulletFirstPos		= TVector3(0.0f, 0.0f, 0.0f);
	m_vBulletPos			= TVector3(0.0f, 0.0f, 0.0f);
	m_vBulletDir			= TVector3(0.0f, 0.0f, 0.0f);
	m_bTrigger				= false;
	m_bMapCheck				= false;
}


BBullet::~BBullet()
{
}
