
//#include "stdafx.h"

#include "LAUnit.h"

#include "LACoreLinkStd.h"
#include "LALoadObjectManager.h"

#ifdef TOOL_MODE
#else
/*===================== UI Include ========================*/

#endif
void LAUnit::SetDirection(TVector3 vNewLook)
{
	TVector3 yVector(0.0f, 1.0f, 0.0f);
	TVector3 vObjectRightLook; D3DXVec3Cross(&vObjectRightLook, &m_vObjectLook, &yVector);	//오른쪽 방향벡터
	TVector3 vObjectLeftLook = -vObjectRightLook;	//왼쪽 방향벡터

	float DotV = D3DXVec3Dot(&vNewLook, &m_vObjectLook);	//기존 방향벡터와 새로운방향벡터의 내적
															////내적값이 -1~1을 넘어갈때
	if (DotV > 1.0f)
	{
		DotV = 1.0f;
	}
	else if (DotV < -1.0f)
	{
		DotV = -1.0f;
	}
	//회전각
	float	Theta = acosf(DotV);

	if (D3DXVec3Dot(&vObjectLeftLook, &vNewLook) > 0.0f)	//왼쪽 방향으로
	{
		//왼쪽 방향벡터와 새로운 진행방향벡터의 내적값이 양수일대 몬스터를 왼쪽으로 회전
		Thetafull += Theta;
	}
	else if (D3DXVec3Dot(&vObjectRightLook, &vNewLook) >= 0.0f)	//오른쪽 방향으로
	{
		//오른쪽 방향벡터와 새로운 진행방향벡터의 내적값이 양수일대 몬스터를 오른쪽으로 회전
		Thetafull -= Theta;
	}
	D3DXMatrixRotationY(&m_matObjectrotation, Thetafull);	//회전
	m_vObjectLook = vNewLook;	//현재 방향벡터를 새로 들어온 진행방향벡터로
}

bool LAUnit::SetObjectABilityData()
{
	//초기화
	m_ObjectAgregateData.iTotalMaxHp = 0;
	m_ObjectAgregateData.fTotalDef = 0.0f;
	m_ObjectAgregateData.fTotalMoveSpeed = 0.0f;

	//Hero
	m_ObjectAgregateData.iTotalMaxHp += m_ObjectData.iHp;
	m_ObjectAgregateData.iCurrentHp = m_ObjectData.iHp;
	m_ObjectAgregateData.fTotalDef += m_ObjectData.fDef;
	m_ObjectAgregateData.fTotalMoveSpeed += m_ObjectData.fRunSpeed;

	//Weapon

	//Armor

	return true;
}

void LAUnit::SetHeroPostion(TVector3 vHeroPosition)
{
	m_vHeroPosition = vHeroPosition;
}

void LAUnit::MoveUp()
{
	float value = m_ObjectAgregateData.fTotalMoveSpeed * g_fSecPerFrame;
	MoveLook(value);
}

void LAUnit::MoveRun()
{
	if (!m_bisMotion)
	{
		float value = m_ObjectAgregateData.fTotalMoveSpeed * g_fSecPerFrame;
		MoveLook(value * 3);
	}
}

void LAUnit::Attack()
{
}

void LAUnit::Death()
{
}

void LAUnit::SubtractHpDamage(int iDamage)
{
	m_ObjectAgregateData.iCurrentHp -= iDamage;
}

#ifdef TOOL_MODE
#else
void  LAUnit::InsertMapMember(BMapParser* pMap)
{

	m_MonsterAI.InsertMapMember(pMap);

}
#endif

void LAUnit::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	LATopObject::Init(pDevice, pContext);

#ifdef TOOL_MODE
#else
	m_MonsterAI.NumOfMonster = 0;
	m_MonsterAI.Init();
#endif

}

bool LAUnit::Frame()
{
	//현제 모션 업데이트
	UpdateMotion();

	//이동값 업데이트
	PrevFrame();

	return true;
}

bool LAUnit::Delete()
{
	LOAD_OBJECT_MGR.AddDeleteList(m_ObjectData.eObjectType, this);

	return true;
}

bool LAUnit::Release()
{
	LATopObject::Release();

#ifdef TOOL_MODE
#else
	m_MonsterAI.Release();
#endif

	return true;
}

LAUnit::LAUnit()
{
#ifdef TOOL_MODE
#else
	Thetafull = 0.0f;
	IsFindTarget = false;
	isCollision = false;
	CollisionTick = 0.0f;
	IsDamaged = false;
	GotoPlayer = false;
#endif
}


LAUnit::~LAUnit()
{

}
