#include "BTimer.h"
namespace BBASIS 
{
	float g_fSecPerFrame = 0.0f;
	float g_fDurationTime = 0.0f;
	float g_fElapseTime = 0.0f;
}

bool BTimer::Init()
{
	QueryPerformanceCounter(&m_BeforeFrameTime);
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
	m_FPSCheck = m_BeforeFrameTime;

	m_fSecPerFrame = 0;
	m_fDurationTime = 0;
	m_iFramePerSecond = 0;
	m_iFPSElapse = 0;

	Start();
	return true;
}

bool BTimer::Frame()
{
	QueryPerformanceCounter(&m_Current);
	g_fSecPerFrame = m_fSecPerFrame = static_cast<float>(m_Current.QuadPart -m_BeforeFrameTime.QuadPart) / static_cast<float> (m_Frequency.QuadPart);
	g_fDurationTime = m_fDurationTime = m_fDurationTime + m_fSecPerFrame;
	g_fElapseTime = m_iFPSElapse;

	/*1초당 프레임 체크*/
	if (((m_Current.QuadPart - m_FPSCheck.QuadPart) / m_Frequency.QuadPart) >= 1)
	{
		m_iFramePerSecond = m_iFPSElapse;
		m_iFPSElapse = 0;
		m_FPSCheck = m_Current;
	}
	m_iFPSElapse++;
	m_BeforeFrameTime = m_Current;

	return true;
}

void BTimer::Reset()
{
	m_bStarted = false;
	m_fEventTime = 0.0f;
	memset(&m_Start, 0, sizeof(LARGE_INTEGER));
	memset(&m_Elapse, 0, sizeof(LARGE_INTEGER));
}

void BTimer::Start()
{
	m_bStarted = true;
	m_fEventTime = 0.0f;
	QueryPerformanceCounter((LARGE_INTEGER *)&m_Start);
}

void BTimer::Stop()
{
	GetElapsedTime();
	m_bStarted = false;
}


bool BTimer::IsStarted() const
{
	return m_bStarted;
}

float BTimer::GetElapsedTime()
{
	if (m_bStarted)
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&m_Elapse);
		m_fEventTime = static_cast<float>(m_Elapse.LowPart - m_Start.LowPart) / static_cast<float>(m_Frequency.LowPart);
	}
	return m_fEventTime;
}

bool BTimer::Render()
{
	return true;
}

bool BTimer::Release()
{
	return true;
}

BTimer::BTimer()
{
}


BTimer::~BTimer()
{
}
