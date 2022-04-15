#pragma once

#include "BStd.h"

#include "LAHero.h"
#include <process.h>

class BMapParser;
class CharacterParser
{
public:
	//Hero
	LAHero* pHero;
	BMapParser* m_pMap;

	//KeyState
	// Thread
	HANDLE m_hTread[4];

	DWORD m_thread_Character_ID;
	DWORD m_thread_Monster_ID;
	DWORD m_thread_Item_ID;
	DWORD m_thread_Skill_ID;
public:
	D3DXVECTOR3 GetHeroPosition();
	D3DXVECTOR3 GetHeroDirection();
public:
	bool Init(BMapParser* pMap);
	bool Frame();
	bool Render();
	bool GameReset();
	bool Release();
public:
	CharacterParser();
	~CharacterParser();
};

