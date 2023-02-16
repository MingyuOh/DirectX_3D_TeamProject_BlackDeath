#include "FiniteState.h"

void FiniteState::AddTransition(DWORD iEvent, DWORD outState)
{
	//이벤트를 인덱스로하여 그 이벤트에 맞게 변화하는 상태를 저장
	//std::map<DWORD, DWORD>::iterator iter;
	//DWORD* pState;
	//iter = m_map.begin();
	//while (iter != m_map.end())
	//{
	//	pState = &iter->second;
	//	if (/**pState == iEvent && */*pState == outState)
	//		break;
	//	iter++;
	//}
	//if (iter == m_map.end())
	m_map[iEvent] = outState;
}

DWORD FiniteState::OutputState(DWORD iEvent)
{
	//전이된 상태를 반환
	return m_map[iEvent];
}
bool FiniteState::Release()
{
	//맵을 해제
	std::map<DWORD, DWORD>::iterator iter;
	/*for (iter = m_map.begin(); iter != m_map.end(); iter++)
	{
	DWORD *rData = &iter ->second;
	delete rData;
	}
	m_map.clear();*/
	for (iter = m_map.begin(); iter != m_map.end();)
	{
		m_map.erase(iter++);
	}
	return true;
}

FiniteState::FiniteState(DWORD dwState, DWORD dwEvent)
{
	m_dwState = dwState;
	m_dwEvent = dwEvent;
}


FiniteState::~FiniteState()
{
}
