#pragma once
#include "BBoundingBox.h"
#include "BBox.h"

#define GRAVITY 9.80665f	// ����ź �߷�
#define	THROWPOWER 5.0f		// ����ź ������ ��

class BGrenade
{
public:
	BBoundingBox	m_BoundingBox;				// ����ź �浹�ٿ���ڽ�
	float			m_fHeight;					// ����ź ����
	float			m_fVelocity;				// ����ź �ӵ�
	float			m_fAngle;					// ����ź ���ư��� ����
	bool			m_bTrigger;					// ����ź Ʈ����
	TVector3		m_vGrenadeDir;				// ����ź ����
	TVector3		m_vGrenadePos;				// ����ź ��ġ

	TVector3		m_vVelocity;				// ����ź ���� �ӵ�
	TVector3		m_vFirstGrenadePos;			// ����ź �ʱ� ��ġ
	float			m_fShootTime;				// ����ź �������� �� �ð�
	float			m_fGrenadeSeatedFloorTime;	// ����ź �ٴڿ� �����ǰ� �� �ð�
	float			m_fExplosionDelayTime;		// ����ź ������ �ð�
	float			m_fGrenadeRadius;			// ����ź �ݰ�
	float			m_fGrenadeAttack;			// ����ź �����
	bool			m_bMapCheck;				// ����ź �� üũ
public:
	//--------------------------------------------------------------------------------------
	// ����ź ������ �Լ�
	//--------------------------------------------------------------------------------------
	void			ThrowingAGrenade();
	//--------------------------------------------------------------------------------------
	// ����ź ��ġ ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			SetGrenadePos(TVector3 vPos);
	//--------------------------------------------------------------------------------------
	// ����ź ���� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			SetGrenadeDir(TVector3 vDir);
	//--------------------------------------------------------------------------------------
	// ����ź ��� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			SetGrenade(float fVelocity, float fAngle);
public:
	BGrenade();
	virtual ~BGrenade();
};

