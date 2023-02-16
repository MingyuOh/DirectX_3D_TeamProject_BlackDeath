#pragma once
#include "BMapParser.h"
#include "EffectParser.h"
#include "CharacterParser.h"
#include "K_UIParser.h"
#include <process.h>

class Sample : public BCore
{
public:
	BMapParser		m_Map;
	CharacterParser m_Character;
	EffectParser	m_Effect;
	K_UIParser		m_UI;

public:
//===========��Ƽ ������============
	HANDLE			Thread_FirstInit;		// ������ �ڵ�
	DWORD			Thread_FirstInit_ID;	// ������ ���̵�
	bool			ThreadFunctionCall;		// �����尡 �����ƴ����� üũ
//==================================
public:
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
public:
	Sample();
	~Sample();
};

