#pragma once
#include "BObjStd.h"

class Particle
{
public:
	TVector3		m_vPos;							// Effect position value
	TVector4		m_vColor;						// Effect color value
	TVector3		m_vSpeed;						// Effect speed value
	TVector3		m_vDirection;					// Effect direction value
	TVector3		m_vScale;						// Effect scale value
	TVector3		m_vCenter;						// Effect center position
	TVector3		m_vFirstPos;					// Effect frist position
	float			m_fRadian;						// Effect radian value
	float			m_fRotationAngle;				// Effect rotation angle
public:
	Particle();
	~Particle();
};

