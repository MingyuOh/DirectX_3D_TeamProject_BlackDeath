#include "BGrenade.h"
#include "BInput.h"

//--------------------------------------------------------------------------------------
// ����ź ��� ���� �Լ�
//--------------------------------------------------------------------------------------
void BGrenade::SetGrenade(float fVelocity, float fAngle)
{
	m_fVelocity = fVelocity;
	m_fAngle = fAngle;
}

//--------------------------------------------------------------------------------------
// ����ź ��ġ ���� �Լ�
//--------------------------------------------------------------------------------------
void BGrenade::SetGrenadePos(TVector3 vPos)
{
	m_vGrenadePos = vPos;
	m_vFirstGrenadePos = vPos;
	m_vVelocity = TVector3(m_fVelocity, m_fVelocity, m_fVelocity);
}

//--------------------------------------------------------------------------------------
// ����ź ���� ���� �Լ�
//--------------------------------------------------------------------------------------
void BGrenade::SetGrenadeDir(TVector3 vDir)
{
	m_vGrenadeDir = vDir;
}

//--------------------------------------------------------------------------------------
// ����ź ������ �Լ�
//--------------------------------------------------------------------------------------
void BGrenade::ThrowingAGrenade()
{
	TVector3 vFloorDir = TVector3(m_vGrenadeDir.x, 0.0f, m_vGrenadeDir.z);
	float fTheta = acosf(D3DXVec3Dot(&m_vGrenadeDir, &vFloorDir) 
							/ (D3DXVec3Length(&m_vGrenadeDir) * D3DXVec3Length(&vFloorDir)));
	m_fShootTime += g_fSecPerFrame;

	// THROWPOWER = 5.0f / GRAVITY = 9.80665f
	m_vVelocity.x = m_fVelocity * THROWPOWER * cosf(fTheta);
	m_vVelocity.y = m_fVelocity * THROWPOWER * sinf(fTheta);
	m_vVelocity.z = m_fVelocity * THROWPOWER * cosf(fTheta);

	m_vGrenadePos.x = m_vFirstGrenadePos.x + (m_vGrenadeDir.x * m_vVelocity.x * m_fShootTime); 
	m_vGrenadePos.y = m_vFirstGrenadePos.y + (m_vGrenadeDir.y * m_vVelocity.y * m_fShootTime 
						+ (0.5f * -GRAVITY * m_fShootTime * m_fShootTime));
	m_vGrenadePos.z = m_vFirstGrenadePos.z + (m_vGrenadeDir.z * m_vVelocity.z * m_fShootTime);
}

BGrenade::BGrenade()
{
	m_fHeight						= 0.0f;
	m_fVelocity						= 10.0f;
	m_fAngle						= PI / 3.0f;
	m_bTrigger						= false;
	m_vGrenadeDir					= TVector3(0.0f, 0.0f, 0.0f);
	m_vGrenadePos					= TVector3(0.0f, 0.0f, 0.0f);
	m_fShootTime					= 0.0f;
	m_fGrenadeSeatedFloorTime		= 0.0f;
	m_fExplosionDelayTime			= 2.5f;
	m_fGrenadeRadius				= 5.0f;
	m_fGrenadeAttack				= 200.0f;
	m_bMapCheck						= false;
}


BGrenade::~BGrenade()
{
}
