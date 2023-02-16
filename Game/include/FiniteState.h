#pragma once
#include "BStd.h"
//� �̺�Ʈ�� ������ �� �̺�Ʈ�� �´� ���¸� �����ϰ� ��ȯ�ϴ� ������ �ϴ� Ŭ����
class FiniteState
{
private:
	friend class FiniteStateMachine;
public:
	DWORD m_dwEvent;	//�̺�Ʈ
	DWORD m_dwState;	//����
	std::map<DWORD, DWORD>	m_map;		//�̺�Ʈ�� ���¸� �����ϴ� ��
public:
	void AddTransition(DWORD iEvent, DWORD outState);
	DWORD OutputState(DWORD iEvent);	//���� ���� ��ȯ �Լ�

	bool Release();
public:
	FiniteState(DWORD dwState, DWORD dwEvent);
	virtual ~FiniteState();
};

