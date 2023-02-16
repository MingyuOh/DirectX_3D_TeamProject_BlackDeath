#include "BWrite.h"

bool BWrite::Init()
{
	return true;
}
bool BWrite::Begin()
{
	if (m_pRT)
	{
		m_pRT->BeginDraw();
		m_pRT->SetTransform(D2D1::IdentityMatrix());
	}
	return true;
}
bool BWrite::End()
{
	HRESULT hr = S_OK;
	if (FAILED(hr = m_pRT->EndDraw()))
	{
		return false;
	}
	return true;
}

bool BWrite::Create(IDXGISurface1* pSurface)
{
	m_pSurface = pSurface;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2D);											// D2D1Factory 인터페이스 생성
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pWriter);		// DWriteFactory 인터페이스 생성
	m_pWriter->CreateTextFormat(L"궁서", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"ko-kr"/*L"en-us"*/, &m_pText); //텍스트 포멧 객체 생성
	
	/*랜더 타겟 생성*/
	float xDpi, yDpi;
	UINT dpi = GetDpiForWindow(g_hWnd);
	xDpi = static_cast<FLOAT>(dpi);
	yDpi = static_cast<FLOAT>(dpi);

	D2D1_RENDER_TARGET_PROPERTIES rtp;
	rtp.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	rtp.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
	rtp.dpiX = xDpi;
	rtp.dpiY = yDpi;
	rtp.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	rtp.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	HRESULT hr = S_OK;
	if (FAILED(hr = m_pD2D->CreateDxgiSurfaceRenderTarget(m_pSurface, &rtp, &m_pRT)))
	{
		return false;
	}

	m_pRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Aqua), &m_pBrush);
	return true;
}

void BWrite::DrawText(RT_F rc, TCHAR* pString, D2D1::ColorF color)
{
	m_pBrush->SetColor(color);
	m_pRT->DrawText(pString, wcslen(pString), m_pText, rc, m_pBrush);
}

bool BWrite::Release()
{
	S_RELEASE(m_pRT);
	S_RELEASE(m_pD2D);
	S_RELEASE(m_pWriter);
	S_RELEASE(m_pText);
	S_RELEASE(m_pBrush);
	return true;
}


BWrite::BWrite()
{
}


BWrite::~BWrite()
{
}
