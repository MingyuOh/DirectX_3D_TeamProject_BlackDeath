#pragma once
#include "FiniteState.h"

class FiniteStateMachine
{
private:
	std::map<DWORD, FiniteState*>	m_mapState;
public:
	void	AddStateTransition(DWORD state, DWORD iEvent, DWORD outState);
	DWORD	GetOutputState(DWORD iState, DWORD iEvent);

	bool Release();
public:
	FiniteStateMachine();
	virtual ~FiniteStateMachine();
};

