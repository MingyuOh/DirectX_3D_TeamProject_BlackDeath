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
	/*===================== UI ��� ���� ========================*/
	bool isReverse;
#endif

public:
#ifdef TOOL_MODE
#else
	/*===================== UI �Լ� ========================*/
	float DistanceMonstertoHero();	//���Ϳ� ������� �Ÿ����
	void  MonsterMontionStateSet(int MotionNumber);	//������ ��ǻ��¸� �����ϴ� �Լ�
	void  MonsterEventOccurrence();					//��̺�Ʈ�� �߻��ߴ��� Ȯ���ϴ� �Լ�
													//�ʸ�� �߰� �Լ�
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

