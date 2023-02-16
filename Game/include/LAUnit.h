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
	D3DXVECTOR3 m_vHeroPosition;
	float Thetafull;	//���� ȸ����
	bool m_bMapCheck;

#ifdef TOOL_MODE
#else
	/*===================== UI ��� ���� ========================*/
	MonsterAIManager m_MonsterAI;	//�߰�
	float			 DistanceToHero;
	bool IsFindTarget;
	bool isCollision;
	float CollisionTick;
	bool IsDamaged;
	bool GotoPlayer;
#endif

public:
	bool SetObjectABilityData();
	void SetHeroPostion(D3DXVECTOR3 vHeroPosition);
	void SetDirection(D3DXVECTOR3 vNewLook);
#ifdef TOOL_MODE
#else
	/*===================== UI �Լ� ========================*/
	void  InsertMapMember(BMapParser* pMap);
#endif
public:
	virtual void MoveUp();
	virtual void MoveRun();
	virtual void Attack();
	virtual void Death();
	void SubtractHpDamage(int iDamage);
public:
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);			//�߰�
	virtual bool Frame();
	virtual bool Delete();
	virtual bool Release();
public:
	LAUnit();
	virtual ~LAUnit();
};

