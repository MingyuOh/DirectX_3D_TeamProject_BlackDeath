
//#include "stdafx.h"

#include "LAInputHandler.h"

#include "CoreInputLink.h"
#include "LAKeyStateCommand.h"
#include "LACharacterInputState.h"

void LAInputHandler::Init()
{
	//================================================================================
	//	** HeroUser ** 
	//================================================================================
	pButtonUp = new LAUp;
	pButtonDown = new LADown;
	pButtonLeft = new LALeft;
	pButtonRight = new LARight;
	pButtonRun = new LARun;
	pButtonJump = new LAJump;
	pMouseLeft = new LAAttack;

	pBulletReLoad = new LABulletReLoad;
	pButtonWeaponRifle = new LARifle;
	pButtonWeaponHandgun = new LAHandgun;;
	pButtonWeaponKnife = new LAKnife;;
	pButtonWeaponBomb = new LABomb;;
	pButtonWeaponMedic = new LAMedic;;

	//캐릭터 이동키값 셋팅
	LAKeyState Upkey;
	Upkey.dwKey = DIK_W;
	Upkey.pCommand = pButtonUp;
	dwUserKeyState.push_back(Upkey);

	LAKeyState Downkey;
	Downkey.dwKey = DIK_S;
	Downkey.pCommand = pButtonDown;
	dwUserKeyState.push_back(Downkey);

	LAKeyState Leftkey;
	Leftkey.dwKey = DIK_A;
	Leftkey.pCommand = pButtonLeft;
	dwUserKeyState.push_back(Leftkey);

	LAKeyState Rightkey;
	Rightkey.dwKey = DIK_D;
	Rightkey.pCommand = pButtonRight;
	dwUserKeyState.push_back(Rightkey);

	LAKeyState Runkey;
	Runkey.dwKey = DIK_LSHIFT;
	Runkey.pCommand = pButtonRun;
	dwUserKeyState.push_back(Runkey);

	LAKeyState Jumpkey;
	Jumpkey.dwKey = DIK_SPACE;
	Jumpkey.pCommand = pButtonJump;
	dwUserKeyState.push_back(Jumpkey);

	LAKeyState ActionAttack;
	ActionAttack.dwKey = 0;
	ActionAttack.pCommand = pMouseLeft;
	dwUserKeyState.push_back(ActionAttack);



	LAKeyState ChangeRifleReload;
	ChangeRifleReload.dwKey = DIK_R;
	ChangeRifleReload.pCommand = pBulletReLoad;
	dwUserKeyState.push_back(ChangeRifleReload);

	LAKeyState ChangeRifle;
	ChangeRifle.dwKey = DIK_1;
	ChangeRifle.pCommand = pButtonWeaponRifle;
	dwUserKeyState.push_back(ChangeRifle);

	LAKeyState ChangeHandgun;
	ChangeHandgun.dwKey = DIK_2;
	ChangeHandgun.pCommand = pButtonWeaponHandgun;
	dwUserKeyState.push_back(ChangeHandgun);

	LAKeyState ChangeKnife;
	ChangeKnife.dwKey = DIK_3;
	ChangeKnife.pCommand = pButtonWeaponKnife;
	dwUserKeyState.push_back(ChangeKnife);

	LAKeyState ChangeBomb;
	ChangeBomb.dwKey = DIK_4;
	ChangeBomb.pCommand = pButtonWeaponBomb;
	dwUserKeyState.push_back(ChangeBomb);

	LAKeyState ChangeMedic;
	ChangeMedic.dwKey = DIK_5;
	ChangeMedic.pCommand = pButtonWeaponMedic;
	dwUserKeyState.push_back(ChangeMedic);
}

void LAInputHandler::SetHeroUser(LAHero* pUser)
{
	this->pUser = pUser;
}

void LAInputHandler::LAHandleInput()
{
	//캐릭터 이동값 체크
	for (int iCnt = 0; iCnt < MAX_MOVE_KEY_COUNT; iCnt++)
	{
		dwUserKeyState[iCnt].keyState = I_Input.KeyCheck(dwUserKeyState[iCnt].dwKey);
		if (dwUserKeyState[iCnt].keyState)
		{
			if (pUser != NULL)
			{
				dwUserKeyState[iCnt].pCommand->execute(pUser, dwUserKeyState[iCnt].keyState);
			}
		}
		
	}

	//캐릭터 모션값 체크
	for (int iCnt = MAX_MOVE_KEY_COUNT; iCnt < MAX_MOVE_KEY_COUNT + MAX_ACTION_KEY_COUNT; iCnt++)
	{
		if (iCnt == MAX_MOVE_KEY_COUNT + 2)
		{
#ifdef TOOL_MODE

			if (I_Input.m_byteMouseState[0])
			{
				if (pUser != NULL)
				{
					dwUserKeyState[iCnt].keyState = I_Input.m_byteMouseState[0];
#else

			if (I_Input.m_MouseState[0])
			{
				if (pUser != NULL)
				{
					dwUserKeyState[iCnt].keyState = I_Input.m_MouseState[0];

#endif

			
					dwUserKeyState[iCnt].pCommand->execute(pUser, dwUserKeyState[iCnt].keyState);
				}
			}
			continue;
		}

		dwUserKeyState[iCnt].keyState = I_Input.KeyCheck(dwUserKeyState[iCnt].dwKey);
		if (dwUserKeyState[iCnt].keyState)
		{
			if (pUser != NULL)
			{
				dwUserKeyState[iCnt].pCommand->execute(pUser, dwUserKeyState[iCnt].keyState);
			}
		}
	}
}

LAInputHandler::~LAInputHandler()
{
}
