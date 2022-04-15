#include "FiniteStateMachine.h"


void	FiniteStateMachine::AddStateTransition(DWORD state, DWORD iEvent, DWORD outState)
{
	std::map<DWORD, FiniteState*>::iterator iter;
	FiniteState* pState = NULL;
	iter = m_mapState.begin();
	while (iter != m_mapState.end())
	{
		pState = iter->second;
		if (pState->m_dwState == state)
			break;
		iter++;
	}
	//���� ������ state�� �������� �ʴ´ٸ� ���� �����Ѵ�.
	if (iter == m_mapState.end())
	{
		pState = new FiniteState(state, iEvent);
		m_mapState[state] = pState;
	}

	//���� ���� ������ �߰��Ѵ�.
	if (pState != NULL)
	{
		pState->AddTransition(iEvent, outState);
	}
}
DWORD	FiniteStateMachine::GetOutputState(DWORD iState, DWORD iEvent)
{
	return m_mapState[iState]->OutputState(iEvent);
}
bool FiniteStateMachine::Release()
{
	std::map<DWORD, FiniteState*>::iterator iter;
	for (iter = m_mapState.begin(); iter != m_mapState.end(); iter++)
	{
		FiniteState* rData = iter->second;
		rData->Release();
		delete rData;
	}
	m_mapState.clear();
	return true;
}
FiniteStateMachine::FiniteStateMachine()
{

}


FiniteStateMachine::~FiniteStateMachine()
{
	std::map<DWORD, FiniteState*>::iterator iter;
	for (iter = m_mapState.begin(); iter != m_mapState.end(); iter++)
	{
		FiniteState* rData = iter->second;
		rData->Release();
		delete rData;
	}
	m_mapState.clear();
}
