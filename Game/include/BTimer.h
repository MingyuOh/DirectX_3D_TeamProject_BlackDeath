#include "BStd.h"
#pragma once
class BTimer
{
public:
	LARGE_INTEGER m_Frequency;		 // 초당 주파수
	LARGE_INTEGER m_Current;		 // 현재 시간
	LARGE_INTEGER m_BeforeFrameTime; // 이전 프레임의 시간
	LARGE_INTEGER m_FPSCheck;		 // FPS 체크용 타이머

	float m_fSecPerFrame;    // 프레임 경과 시간 ( 한 프레임을 돌 때 걸리는 시간 )
	float m_fDurationTime;	 // 실행된 경과 시간
	
	int m_iFramePerSecond;	 // 초당 프레임
	int m_iFPSElapse;		 // FPS체크용 초당 경과된 프레임의 갯수


	/*이벤트 타이머 관련*/

	LARGE_INTEGER	m_Start;		// 이벤트 타이머 시작 시간
	LARGE_INTEGER	m_Elapse;		// 이벤트 타이머 경과 시간
	bool			m_bStarted;		// 이벤트 타이머 작동여부
	float			m_fEventTime;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	// 경과한 시간에 대한 지원 함수들 (이벤트)
	void	Reset();
	void	Start();
	void	Stop();
	bool	IsStarted() const;
	float GetElapsedTime(); // 이벤트 시간 출력 함수

public:
	BTimer();
	~BTimer();
};