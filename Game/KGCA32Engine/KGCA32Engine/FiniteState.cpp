#include "FiniteState.h"

void FiniteState::AddTransition(DWORD iEvent, DWORD outState)
{
	//�̺�Ʈ�� �ε������Ͽ� �� �̺�Ʈ�� �°� ��ȭ�ϴ� ���¸� ����
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
	//���̵� ���¸� ��ȯ
	return m_map[iEvent];
}
bool FiniteState::Release()
{
	//���� ����
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
