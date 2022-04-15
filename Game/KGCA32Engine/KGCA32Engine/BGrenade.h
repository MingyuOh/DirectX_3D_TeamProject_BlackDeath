#pragma once
#include "BBoundingBox.h"
#include "BBox.h"

#define GRAVITY 9.80665f	// 수류탄 중력
#define	THROWPOWER 5.0f		// 수류탄 던지는 힘

class BGrenade
{
public:
	BBoundingBox	m_BoundingBox;				// 수류탄 충돌바운딩박스
	float			m_fHeight;					// 수류탄 높이
	float			m_fVelocity;				// 수류탄 속도
	float			m_fAngle;					// 수류탄 날아가는 각도
	bool			m_bTrigger;					// 수류탄 트리거
	TVector3		m_vGrenadeDir;				// 수류탄 방향
	TVector3		m_vGrenadePos;				// 수류탄 위치

	TVector3		m_vVelocity;				// 수류탄 현재 속도
	TVector3		m_vFirstGrenadePos;			// 수류탄 초기 위치
	float			m_fShootTime;				// 수류탄 던져지고난 후 시간
	float			m_fGrenadeSeatedFloorTime;	// 수류탄 바닥에 안착되고난 후 시간
	float			m_fExplosionDelayTime;		// 수류탄 터지는 시간
	float			m_fGrenadeRadius;			// 수류탄 반경
	float			m_fGrenadeAttack;			// 수류탄 공경력
	bool			m_bMapCheck;				// 수류탄 맵 체크
public:
	//--------------------------------------------------------------------------------------
	// 수류탄 던지는 함수
	//--------------------------------------------------------------------------------------
	void			ThrowingAGrenade();
	//--------------------------------------------------------------------------------------
	// 수류탄 위치 셋팅 함수
	//--------------------------------------------------------------------------------------
	void			SetGrenadePos(TVector3 vPos);
	//--------------------------------------------------------------------------------------
	// 수류탄 방향 셋팅 함수
	//--------------------------------------------------------------------------------------
	void			SetGrenadeDir(TVector3 vDir);
	//--------------------------------------------------------------------------------------
	// 수류탄 요소 셋팅 함수
	//--------------------------------------------------------------------------------------
	void			SetGrenade(float fVelocity, float fAngle);
public:
	BGrenade();
	virtual ~BGrenade();
};

