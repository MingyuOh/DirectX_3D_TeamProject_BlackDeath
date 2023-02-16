#include "BStd.h"
#pragma once

struct RT_F : public D2D_RECT_F
{
	RT_F(){}
	RT_F(FLOAT l, FLOAT t, FLOAT r, FLOAT b)
	{
		left = l;
		top = t;
		right = r;
		bottom = b;
	}
};

class BWrite
{
public:
	ID2D1RenderTarget* m_pRT;
	ID2D1Factory* m_pD2D;
	IDWriteFactory* m_pWriter;
	IDWriteTextFormat* m_pText;
	ID2D1SolidColorBrush* m_pBrush;
	IDXGISurface1* m_pSurface;


public:
	bool Init();
	bool Begin();
	bool End();

	bool Create(IDXGISurface1* pSurface);
	void DrawText(RT_F rc, TCHAR* pString, D2D1::ColorF color = D2D1::ColorF(1, 1, 1, 1));
	bool Release();

public:
	BWrite();
	~BWrite();
};

