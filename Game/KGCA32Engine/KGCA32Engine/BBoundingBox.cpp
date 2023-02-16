#include "BBoundingBox.h"
#include "BCore.h"

bool BBoundingBox::CreateBoundingBox(TVector3 vMax, TVector3 vMin, TVector3 vPos)
{
	m_BoundingBoxMax = vMax;
	m_BoundingBoxMin = vMin;

	// 구의 센터값
	m_vCenter	= vPos;
	m_vCenter.y = vPos.y + ((m_BoundingBoxMax.y + m_BoundingBoxMin.y) / 2.0f);

	// 구의 반지름
	TVector3 vRadius = (m_BoundingBoxMax + vPos) - m_vCenter;
	if (vRadius.x > vRadius.y)
	{
		if (vRadius.x > vRadius.z)
			m_fRadius = vRadius.x;
		else
			m_fRadius = vRadius.z;
	}
	else
	{
		if (vRadius.y > vRadius.z)
			m_fRadius = vRadius.y;
		else
			m_fRadius = vRadius.z;
	}

	m_fRadius = fabs(m_fRadius);

	m_matWorld._41 = vPos.x;
	m_matWorld._42 = vPos.y;
	m_matWorld._43 = vPos.z;
	return true;
}

void BBoundingBox::UpdateBoundingBox(TVector3 vPos)
{
	TMatrix matScale, matRotation;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixRotationYawPitchRoll(&matRotation, g_pMainCamera->m_fYaw, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_matWorld, &matScale, &matRotation);

	m_vCenter = vPos;
	m_vCenter.y = vPos.y + ((m_BoundingBoxMax.y + m_BoundingBoxMin.y) / 2.0f);

	// 구의 반지름
	TVector3 vRadius = (m_BoundingBoxMax + vPos) - m_vCenter;
	if (vRadius.x > vRadius.y)
	{
		if (vRadius.x > vRadius.z)
			m_fRadius = vRadius.x;
		else
			m_fRadius = vRadius.z;
	}
	else
	{
		if (vRadius.y > vRadius.z)
			m_fRadius = vRadius.y;
		else
			m_fRadius = vRadius.z;
	}

	m_matWorld._41 = vPos.x;
	m_matWorld._42 = vPos.y;
	m_matWorld._43 = vPos.z;
}

BBoundingBox::BBoundingBox()
{
	m_tBox.vCenter = TVector3(0.0f, 0.0f, 0.0f);
	m_tBox.vMax = TVector3(1.0f, 1.0f, 1.0f);
	m_tBox.vMin = TVector3(-1.0f, -1.0f, -1.0f);

	//m_vPosition = TVector3(25 - rand() % 50, 0, 25 - rand() % 50);
	m_vPosition = TVector3(320 - rand() % 640, 4, 320 - rand() % 640);
	//m_vColor = TVector4((rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1.0f);
	m_vColor = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	// g_matWorld Matrix = S (로테이션 제외)
	TMatrix matWorld;
	//TMatrixScaling(&matWorld, (rand() % 256) / 255.0f * 2.0f, (rand() % 256) / 255.0f * 2.0f, (rand() % 256) / 255.0f * 2.0f);
	D3DXMatrixScaling(&matWorld, 4.0f, 4.0f, 4.0f);

	m_vCenter = TVector3(m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_fRadius = max(m_vPosition.x, m_vPosition.y);
	m_fRadius = max(m_fRadius, m_vPosition.z);

	m_tBox.vCenter = m_vPosition;
	TVector3 vMax, vMin, vHalf;
	TVector3 vAxis(1.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&m_tBox.vAxis[0], &vAxis, &matWorld);
	vAxis = TVector3(0.0f, 1.0f, 0.0f);
	D3DXVec3TransformCoord(&m_tBox.vAxis[1], &vAxis, &matWorld);
	vAxis = TVector3(0.0f, 0.0f, 1.0f);
	D3DXVec3TransformCoord(&m_tBox.vAxis[2], &vAxis, &matWorld);
	D3DXVec3Normalize(&m_tBox.vAxis[0], &m_tBox.vAxis[0]);
	D3DXVec3Normalize(&m_tBox.vAxis[1], &m_tBox.vAxis[1]);
	D3DXVec3Normalize(&m_tBox.vAxis[2], &m_tBox.vAxis[2]);

	matWorld._41 = m_vPosition.x;
	matWorld._42 = m_vPosition.y;
	matWorld._43 = m_vPosition.z;

	D3DXVec3TransformCoord(&vMax, &m_tBox.vMax, &matWorld);
	D3DXVec3TransformCoord(&vMin, &m_tBox.vMin, &matWorld);
	vHalf = vMax - m_tBox.vCenter;

	m_tBox.fExtent[0] = D3DXVec3Dot(&m_tBox.vAxis[0], &vHalf);
	m_tBox.fExtent[1] = D3DXVec3Dot(&m_tBox.vAxis[1], &vHalf);
	m_tBox.fExtent[2] = D3DXVec3Dot(&m_tBox.vAxis[2], &vHalf);
	//    4    5
	// 0    1
	//    7    6 
	// 3    2
	TVector3 vVertex[8];
	vVertex[0] = TVector3(m_tBox.vMin.x, m_tBox.vMax.y, m_tBox.vMin.z);
	vVertex[1] = TVector3(m_tBox.vMax.x, m_tBox.vMax.y, m_tBox.vMin.z);
	vVertex[2] = TVector3(m_tBox.vMax.x, m_tBox.vMin.y, m_tBox.vMin.z);
	vVertex[3] = TVector3(m_tBox.vMin.x, m_tBox.vMin.y, m_tBox.vMin.z);

	vVertex[4] = TVector3(m_tBox.vMin.x, m_tBox.vMax.y, m_tBox.vMax.z);
	vVertex[5] = TVector3(m_tBox.vMax.x, m_tBox.vMax.y, m_tBox.vMax.z);
	vVertex[6] = TVector3(m_tBox.vMax.x, m_tBox.vMin.y, m_tBox.vMax.z);
	vVertex[7] = TVector3(m_tBox.vMin.x, m_tBox.vMin.y, m_tBox.vMax.z);

	vMin = TVector3(100000.0f, 100000.0f, 100000.0f);
	vMax = TVector3(-100000.0f, -100000.0f, -100000.0f);

	for (int iVer = 0; iVer < 8; iVer++)
	{
		D3DXVec3TransformCoord(&vVertex[iVer], &vVertex[iVer], &matWorld);
		if (vVertex[iVer].x > vMax.x) vMax.x = vVertex[iVer].x;
		if (vVertex[iVer].y > vMax.y) vMax.y = vVertex[iVer].y;
		if (vVertex[iVer].z > vMax.z) vMax.z = vVertex[iVer].z;

		if (vVertex[iVer].x < vMin.x) vMin.x = vVertex[iVer].x;
		if (vVertex[iVer].y < vMin.y) vMin.y = vVertex[iVer].y;
		if (vVertex[iVer].z < vMin.z) vMin.z = vVertex[iVer].z;
	}

	m_tBox.vMin = vMin;
	m_tBox.vMax = vMax;
	m_matWorld = matWorld;
}


BBoundingBox::~BBoundingBox()
{
}
