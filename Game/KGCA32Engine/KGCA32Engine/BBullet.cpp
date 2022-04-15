#include "BBullet.h"

//--------------------------------------------------------------------------------------
// �Ѿ� ��� ���� �Լ�
//--------------------------------------------------------------------------------------
void BBullet::SetBullet(float fBulletIntersection, float fOffensePower, float fBulletSpeed)
{
	m_fBulletIntersection = fBulletIntersection;
	m_fOffensePower = fOffensePower;
	m_fBulletSpeed = fBulletSpeed;
}

//--------------------------------------------------------------------------------------
// �Ѿ� ��ġ ���� �Լ�
//--------------------------------------------------------------------------------------
void BBullet::SetBulletPos(TVector3 vPos)
{
	m_vBulletPos = vPos;
	m_vBulletFirstPos = vPos;
}

//--------------------------------------------------------------------------------------
// �Ѿ� ���� ���� �Լ�
//--------------------------------------------------------------------------------------
void BBullet::SetBulletDir(TVector3 vDir)
{
	m_vBulletDir = vDir;
}

//--------------------------------------------------------------------------------------
// �Ѿ� �ǽð� ������Ʈ �Լ�
//--------------------------------------------------------------------------------------
void BBullet::UpdateBullet()
{
	TVector3 vCenter = m_BoundingBox.m_BoundingBoxMax + m_BoundingBox.m_BoundingBoxMin;
	vCenter /= 2.0f;

	// �ӵ� ����
	m_vBulletPos += m_vBulletDir * m_fBulletSpeed * g_fSecPerFrame;
	m_BoundingBox.m_vCenter = m_vBulletPos + vCenter;
}

//--------------------------------------------------------------------------------------
// �Ѿ� ��Ÿ� üũ
//--------------------------------------------------------------------------------------
bool BBullet::CheckBulletIntersection()
{
	if (m_vBulletDir.z < 0.0f)
	{
		// �Ѿ� ��Ÿ� ó��
		if (m_vBulletFirstPos.z + (m_vBulletDir.z * m_fBulletIntersection) > m_vBulletPos.z)
		{
			return false; // ��Ÿ��� �Ѿ
		}
	}
	else
	{
		// �Ѿ� ��Ÿ� ó��
		if (m_vBulletFirstPos.z + (m_vBulletDir.z * m_fBulletIntersection) < m_vBulletPos.z)
		{
			return false; // ��Ÿ��� �Ѿ
		}
	}
	return true; // ��Ÿ� ���� ����
}

BBullet::BBullet()
{
	// �⺻ ���� �ɷ�ġ
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
