#pragma once
#include "BBoundingBox.h"
#include "BBox.h"

class BBullet
{
public:
	BBoundingBox	m_BoundingBox;			// �Ѿ� �浹�ٿ���ڽ�
	float			m_fBulletSpeed;			// �Ѿ� �ӵ�
	float			m_fBulletIntersection;  // �Ѿ� ��Ÿ�
	float			m_fOffensePower;		// �Ѿ� ���ݷ�
	bool			m_bTrigger;				// �Ѿ� Ʈ����
	TVector3		m_vBulletFirstPos;		// �Ѿ� z���� �ʱ� ��ġ
	TVector3		m_vBulletPos;			// �Ѿ� ��ġ
	TVector3		m_vBulletDir;			// �Ѿ� ����
	bool			m_bMapCheck;			// �Ѿ� �� üũ
public:
	//--------------------------------------------------------------------------------------
	// �Ѿ� ��� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			SetBullet(float fBulletIntersection,
							  float fOffensePower, float fBulletSpeed);
	//--------------------------------------------------------------------------------------
	// �Ѿ� ��ġ ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			SetBulletPos(TVector3 vPos);
	//--------------------------------------------------------------------------------------
	// �Ѿ� ���� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			SetBulletDir(TVector3 vDir);
	//--------------------------------------------------------------------------------------
	// �Ѿ� �ǽð� ������Ʈ �Լ�
	//--------------------------------------------------------------------------------------
	void			UpdateBullet();
	//--------------------------------------------------------------------------------------
	// �Ѿ� ��Ÿ� üũ
	//--------------------------------------------------------------------------------------
	bool			CheckBulletIntersection();
public:
	BBullet();
	virtual ~BBullet();
};

