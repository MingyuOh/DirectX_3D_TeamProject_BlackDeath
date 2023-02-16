#pragma once
#include "LATopObject.h"
#include "LACoreLinkStd.h"


#ifdef TOOL_MODE
#else

#include "LASound2.h"
/*===================== UI Include ========================*/
#include "MonsterAIManager.h"

#endif

class LAUnit : public LATopObject
{
public:
	ObjectTotalDataInfo m_ObjectAgregateData;
	TVector3 m_vHeroPosition;
	float Thetafull;	//최종 회전각
	bool m_bMapCheck;

#ifdef TOOL_MODE
#else
	/*===================== UI 멤버 변수 ========================*/
	MonsterAIManager m_MonsterAI;	//추가
	float			 DistanceToHero;
	bool IsFindTarget;
	bool isCollision;
	float CollisionTick;
	bool IsDamaged;
	bool GotoPlayer;
#endif

public:
	bool SetObjectABilityData();
	void SetHeroPostion(TVector3 vHeroPosition);
	void SetDirection(TVector3 vNewLook);
#ifdef TOOL_MODE
#else
	/*===================== UI 함수 ========================*/
	void  InsertMapMember(BMapParser* pMap);
#endif
public:
	virtual void MoveUp();
	virtual void MoveRun();
	virtual void Attack();
	virtual void Death();
	void SubtractHpDamage(int iDamage);
public:
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);			//추가
	virtual bool Frame();
	virtual bool Delete();
	virtual bool Release();
public:
	LAUnit();
	virtual ~LAUnit();
};

