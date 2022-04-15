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
//===========멀티 스레드============
	HANDLE			Thread_FirstInit;		// 스레드 핸들
	DWORD			Thread_FirstInit_ID;	// 스레드 아이디
	bool			ThreadFunctionCall;		// 스레드가 생성됐는지를 체크
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

