#pragma once


#include "CoreStandardLink.h"
#include "LAPSBSingleton.h"

class LAHero;
class LAKeyStateCommand;
struct LAKeyState
{
	DWORD dwKey;
	int keyState;
	LAKeyStateCommand* pCommand;
};

class LAInputHandler : public LAPSBSingleton<LAInputHandler>
{
private:
	friend class LAPSBSingleton<LAInputHandler>;
public:
	std::vector<LAKeyState> dwUserKeyState;
	//================================================================================
	//	** HeroUser Key State ** 
	//================================================================================
	LAHero* pUser;
	//motion
	LAKeyStateCommand* pButtonUp;
	LAKeyStateCommand* pButtonDown;
	LAKeyStateCommand* pButtonLeft;
	LAKeyStateCommand* pButtonRight;

	LAKeyStateCommand* pButtonRun;
	LAKeyStateCommand* pButtonJump;
	LAKeyStateCommand* pMouseLeft;
	//Action
	LAKeyStateCommand* pBulletReLoad;
	LAKeyStateCommand* pButtonWeaponRifle;
	LAKeyStateCommand* pButtonWeaponHandgun;
	LAKeyStateCommand* pButtonWeaponKnife;
	LAKeyStateCommand* pButtonWeaponBomb;
	LAKeyStateCommand* pButtonWeaponMedic;

public:
	void Init();
	void SetHeroUser(LAHero* pUser);
	void LAHandleInput();
public:
	LAInputHandler()
	{
		pUser = NULL;
	}
	virtual~LAInputHandler();
};

#define LAINPUT_HANDLER LAInputHandler::GetInstance()