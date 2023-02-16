#pragma once

#include "StandardLink.h"

//CharacterResource

struct SectionPlay
{
	int ifirst;
	int ilast;
	int iCurrentframe;
	int iCurrentTick;
	float fElpaseTime;
	bool bReplay;
	SectionPlay()
	{
		ifirst = 0;
		ilast = 0;
		iCurrentTick = 0;
		iCurrentframe = 0;
		fElpaseTime = 0.0f;
		bReplay = true;
	}
};


//enum List

enum class E_MaxNewMotion
{
	IDLE = 0,
	WORK,
	RUN,
	JUMP,
	PUNCH,
	DUMBLING01,
	DUMBLING02,
	DIE,
};

struct MotionKeyInfo
{
	DWORD dwkey;
	int iMotionNumber;

	MotionKeyInfo()
	{
		dwkey = 0;
		iMotionNumber = 0;
	}
};