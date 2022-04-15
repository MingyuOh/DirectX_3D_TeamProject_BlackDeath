#include "Sprite.h"

void Sprite::SetRect(int iRow, int iCol, float fOffset)
{
	m_fLifeTime = fOffset;
	m_fOffsetTime = fOffset / (iRow * iCol);
	RectUV rt;
	float fOffsetX = 1.0f / iRow;
	float fOffsetY = 1.0f / iCol;
	m_uvList.resize(iRow * iCol);
	int iIndex = 0;
	
	for (int iV = 0; iV < iRow; iV++)
	{
		for (int iU = 0; iU < iCol; iU++)
		{
			rt.uv[0].x = iU * fOffsetX;
			rt.uv[0].y = iV * fOffsetY;
			rt.uv[1].x = rt.uv[0].x + fOffsetX;
			rt.uv[1].y = rt.uv[0].y;
			rt.uv[2].x = rt.uv[1].x;
			rt.uv[2].y = rt.uv[1].y + fOffsetY;
			rt.uv[3].x = rt.uv[0].x;
			rt.uv[3].y = rt.uv[2].y;

			m_uvList[iIndex++] = rt;
		}
	}
}

void Sprite::Update()
{
	m_fCurrentTime += g_fSecPerFrame;
	m_iCurrentFrame = min(m_uvList.size() - 1, m_fCurrentTime / m_fOffsetTime);
	if (m_fCurrentTime >= m_fLifeTime)
	{
		m_fCurrentTime = m_fCurrentTime - m_fLifeTime;
		m_iCurrentFrame = 0;
	}
}

TVector2 Sprite::GetDrawRect(int iUV)
{
	return m_uvList[m_iCurrentFrame].uv[iUV];
}

bool Sprite::Release()
{
	DeleteVector(m_uvList);
	BObject::Release();
	return true;
}

Sprite::Sprite()
{
	m_fCurrentTime		= 0.0f;
	m_fLifeTime			= 0.0f;
	m_fOffsetTime		= 0.0f;
	m_iCurrentFrame		= 0;
	m_iIndex			= 0;
	m_bExistUV			= false;
}


Sprite::~Sprite()
{
}
