#pragma once

#include "LAUnit.h"

#ifdef TOOL_MODE
#else
/*===================== UI Include ========================*/

#endif

class LAZombie : public LAUnit
{
public:
#ifdef TOOL_MODE
#else
	/*===================== UI 멤버 변수 ========================*/
	bool isReverse;
#endif

public:
#ifdef TOOL_MODE
#else
	/*===================== UI 함수 ========================*/
	float DistanceMonstertoHero();	//몬스터와 히어로의 거리계산
	void  MonsterMontionStateSet(int MotionNumber);	//몬스터의 모션상태를 셋팅하는 함수
	void  MonsterEventOccurrence();					//어떤이벤트가 발생했는지 확인하는 함수
													//맵멤버 추가 함수
#endif

public:
	void MoveUp();
	void MoveRun();
	void Attack();
	void Death();
public:
	bool Frame();
public:
	LAZombie();
	virtual ~LAZombie();
};

