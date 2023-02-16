#include "Shape.h"

void Shape::CreateOOBBox(float fLengthX, float fLengthY, float fLengthZ, 
	TVector3 vCenter, TVector3 vDirX,TVector3 vDirY,TVector3 vDirZ)
{
	m_tBox.fExtent[0] = fLengthX;
	m_tBox.fExtent[1] = fLengthY;
	m_tBox.fExtent[2] = fLengthZ;
	m_tBox.vCenter = vCenter;

	m_tBox.vAxis[0] = m_tBox.fExtent[0] * vDirX;
	m_tBox.vAxis[1] = m_tBox.fExtent[1] * vDirY;
	m_tBox.vAxis[2] = m_tBox.fExtent[2] * vDirZ;

	m_tBox.vPos[0] = vCenter - m_tBox.vAxis[0] - m_tBox.vAxis[1] - m_tBox.vAxis[2];
	m_tBox.vPos[1] = vCenter - m_tBox.vAxis[0] + m_tBox.vAxis[1] - m_tBox.vAxis[2];
	m_tBox.vPos[2] = vCenter + m_tBox.vAxis[0] + m_tBox.vAxis[1] - m_tBox.vAxis[2];
	m_tBox.vPos[3] = vCenter + m_tBox.vAxis[0] - m_tBox.vAxis[1] - m_tBox.vAxis[2];
	m_tBox.vPos[4] = vCenter - m_tBox.vAxis[0] - m_tBox.vAxis[1] + m_tBox.vAxis[2];
	m_tBox.vPos[5] = vCenter - m_tBox.vAxis[0] + m_tBox.vAxis[1] + m_tBox.vAxis[2];
	m_tBox.vPos[6] = vCenter + m_tBox.vAxis[0] + m_tBox.vAxis[1] + m_tBox.vAxis[2];
	m_tBox.vPos[7] = vCenter + m_tBox.vAxis[0] - m_tBox.vAxis[1] + m_tBox.vAxis[2];

	m_tBox.vMax = m_tBox.vPos[0];
	m_tBox.vMin = m_tBox.vPos[0];

	for (int iPoint = 1; iPoint < 8; iPoint++)
	{
		if (m_tBox.vMax.x < m_tBox.vPos[iPoint].x)
			m_tBox.vMax.x = m_tBox.vPos[iPoint].x;
		if (m_tBox.vMax.y < m_tBox.vPos[iPoint].y)
			m_tBox.vMax.y = m_tBox.vPos[iPoint].y;
		if (m_tBox.vMax.z < m_tBox.vPos[iPoint].z)
			m_tBox.vMax.z = m_tBox.vPos[iPoint].z;

		if (m_tBox.vMin.x > m_tBox.vPos[iPoint].x)
			m_tBox.vMin.x = m_tBox.vPos[iPoint].x;
		if (m_tBox.vMin.y > m_tBox.vPos[iPoint].y)
			m_tBox.vMin.y = m_tBox.vPos[iPoint].y;
		if (m_tBox.vMin.z > m_tBox.vPos[iPoint].z)
			m_tBox.vMin.z = m_tBox.vPos[iPoint].z;
	}

	D3DXVec3Normalize(&m_tBox.vAxis[0], &m_tBox.vAxis[0]);
	D3DXVec3Normalize(&m_tBox.vAxis[1], &m_tBox.vAxis[1]);
	D3DXVec3Normalize(&m_tBox.vAxis[2], &m_tBox.vAxis[2]);
}

void Shape::CreateAABBBox(TVector3 vMax, TVector3 vMin)
{
	m_tBox.vMax = vMax;
	m_tBox.vMin = vMin;
	m_tBox.vPos[0] = TVector3(vMin.x, vMin.y, vMin.z);
	m_tBox.vPos[1] = TVector3(vMin.x, vMax.y, vMin.z);
	m_tBox.vPos[2] = TVector3(vMax.x, vMax.y, vMin.z);
	m_tBox.vPos[3] = TVector3(vMax.x, vMin.y, vMin.z);

	m_tBox.vPos[4] = TVector3(vMin.x, vMin.y, vMax.z);
	m_tBox.vPos[5] = TVector3(vMin.x, vMax.y, vMax.z);
	m_tBox.vPos[6] = TVector3(vMax.x, vMax.y, vMax.z);
	m_tBox.vPos[7] = TVector3(vMax.x, vMin.y, vMax.z);

	m_tBox.vCenter = vMax + vMin;
	m_tBox.vCenter /= 2.0f;
	m_tBox.vAxis[0] = TVector3(1.0f, 0.0f, 0.0f);
	m_tBox.vAxis[1] = TVector3(0.0f, 1.0f, 0.0f);
	m_tBox.vAxis[2] = TVector3(0.0f, 0.0f, 1.0f);
}

Shape::Shape()
{
}


Shape::~Shape()
{
}
