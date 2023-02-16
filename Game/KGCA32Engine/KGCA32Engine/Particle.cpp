#include "Particle.h"



Particle::Particle()
{
	// 위치
	m_vPos					= TVector3(0.0f, 0.0f, 0.0f);
	m_vFirstPos				= TVector3(0.0f, 0.0f, 0.0f);

	// 색상
	m_vColor.x				= 1.0f;
	m_vColor.y				= 1.0f;
	m_vColor.z				= 1.0f;
	m_vColor.w				= 1.0f;

	// 속도
	m_vSpeed.x				= randstep(3.0f, 5.0f);
	m_vSpeed.y				= randstep(3.0f, 5.0f);
	m_vSpeed.z				= randstep(3.0f, 5.0f);

	// 방향
	m_vDirection.x			= 0.0f;
	m_vDirection.y			= -1.0f;
	m_vDirection.z			= 0.0f;

	// 크기
	m_vScale				= TVector3(1.0f, 1.0f, 1.0f);

	// 중심값
	m_vCenter				= TVector3(1.0f, 1.0f, 1.0f);

	// 반지름
	m_fRadian				= 1.0f;

	// Angle Value
	m_fRotationAngle		= 0.0f;
}


Particle::~Particle()
{
}
