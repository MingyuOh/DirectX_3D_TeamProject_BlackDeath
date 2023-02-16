#pragma once
#include "BObjStd.h"

class Particle
{
public:
	D3DXVECTOR3		m_vPos;							// Effect position value
	D3DXVECTOR4		m_vColor;						// Effect color value
	D3DXVECTOR3		m_vSpeed;						// Effect speed value
	D3DXVECTOR3		m_vDirection;					// Effect direction value
	D3DXVECTOR3		m_vScale;						// Effect scale value
	D3DXVECTOR3		m_vCenter;						// Effect center position
	D3DXVECTOR3		m_vFirstPos;					// Effect frist position
	float			m_fRadian;						// Effect radian value
	float			m_fRotationAngle;				// Effect rotation angle
public:
	Particle();
	~Particle();
};

