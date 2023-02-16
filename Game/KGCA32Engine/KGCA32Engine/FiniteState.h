#pragma once
#include "BStd.h"
//어떤 이벤트가 들어오면 그 이벤트에 맞는 상태를 저장하고 반환하는 역할을 하는 클래스
class FiniteState
{
private:
	friend class FiniteStateMachine;
public:
	DWORD m_dwEvent;	//이벤트
	DWORD m_dwState;	//상태
	std::map<DWORD, DWORD>	m_map;		//이벤트와 상태를 저장하는 맵
public:
	void AddTransition(DWORD iEvent, DWORD outState);
	DWORD OutputState(DWORD iEvent);	//봐뀐 상태 반환 함수

	bool Release();
public:
	FiniteState(DWORD dwState, DWORD dwEvent);
	virtual ~FiniteState();
};

