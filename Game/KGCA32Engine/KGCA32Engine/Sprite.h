#pragma once
#include "Plane.h"

struct RectUV
{
	TVector2 uv[4];
};

class Sprite : public Plane
{
public:
	vector<RectUV>		m_uvList;
	int					m_iIndex;
	bool				m_bExistUV;

	float				m_fOffsetTime;
	float				m_fLifeTime;
	int					m_iCurrentFrame;
	float				m_fCurrentTime;
public:
	//--------------------------------------------------------------------------------------
	// Rect�� �ʱ�ȭ
	//--------------------------------------------------------------------------------------
	void				SetRect(int iRow, int iCol, float fOffset);
	//--------------------------------------------------------------------------------------
	// �ð��� ���� �̹��� ��ȯ �Լ�
	//--------------------------------------------------------------------------------------
	void				Update();
	//--------------------------------------------------------------------------------------
	// ���̾ƿ� �� ���̴� ����
	//--------------------------------------------------------------------------------------
	/*HRESULT			LoadShaderAndInputLayout(TCHAR* vertexShaderName = NULL,
		TCHAR* PixelShaderName = NULL, TCHAR* geometryShaderName = NULL);*/
	//--------------------------------------------------------------------------------------
	// Rect�� ��ȯ �Լ�
	//--------------------------------------------------------------------------------------
	TVector2			GetDrawRect(int iUV);
	bool				Release();
public:
	Sprite();
	~Sprite();
};

