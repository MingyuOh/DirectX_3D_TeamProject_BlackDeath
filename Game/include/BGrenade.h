#pragma once
#include "BBoundingBox.h"
#include "BBox.h"

#define GRAVITY 9.80665f	// ����ź �߷�
#define	THROWPOWER 5.0f		// ����ź ������ ��

class BGrenade
{
public:
	BBoundingBox	m_BoundingBox;				// ����ź �浹�ٿ���ڽ�(�׽�Ʈ��)
	float			m_fHeight;					// ����ź ����
	float			m_fVelocity;				// ����ź �ӵ�
	float			m_fAngle;					// ����ź ���ư��� ����
	bool			m_bTrigger;					// ����ź Ʈ����
	D3DXVECTOR3		m_vGrenadeDir;				// ����ź ����
	D3DXVECTOR3		m_vGrenadePos;				// ����ź ��ġ

	D3DXVECTOR3		m_vVelocity;				// ����ź ���� �ӵ�
	D3DXVECTOR3		m_vFirstGrenadePos;			// ����ź �ʱ� ��ġ
	float			m_fShootTime;				// ����ź �������� �� �ð�
	float			m_fGrenadeSeatedFloorTime;	// ����ź �ٴڿ� �����ǰ� �� �ð�
	float			m_fExplosionDelayTime;		// ����ź ������ �ð�
	float			m_fGrenadeRadius;			// ����ź �ݰ�
	float			m_fGrenadeAttack;			// ����ź ���ݷ�
	bool			m_bMapCheck;				// ����ź �� üũ
public:
	//--------------------------------------------------------------------------------------
	// ����ź ��� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			SetGrenade(float fVelocity, float fAngle);
	//--------------------------------------------------------------------------------------
	// ����ź ��ġ ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			SetGrenadePos(D3DXVECTOR3 vPos);
	//--------------------------------------------------------------------------------------
	// ����ź ���� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			SetGrenadeDir(D3DXVECTOR3 vDir);
	//--------------------------------------------------------------------------------------
	// ����ź ������ �Լ�
	//--------------------------------------------------------------------------------------
	void			ThrowingAGrenade();
public:
	BGrenade();
	virtual ~BGrenade();
};

