
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
	TVector3 vObjectRightLook; D3DXVec3Cross(&vObjectRightLook, &m_vObjectLook, &yVector);	//������ ���⺤��
	TVector3 vObjectLeftLook = -vObjectRightLook;	//���� ���⺤��

	float DotV = D3DXVec3Dot(&vNewLook, &m_vObjectLook);	//���� ���⺤�Ϳ� ���ο���⺤���� ����
															////�������� -1~1�� �Ѿ��
	if (DotV > 1.0f)
	{
		DotV = 1.0f;
	}
	else if (DotV < -1.0f)
	{
		DotV = -1.0f;
	}
	//ȸ����
	float	Theta = acosf(DotV);

	if (D3DXVec3Dot(&vObjectLeftLook, &vNewLook) > 0.0f)	//���� ��������
	{
		//���� ���⺤�Ϳ� ���ο� ������⺤���� �������� ����ϴ� ���͸� �������� ȸ��
		Thetafull += Theta;
	}
	else if (D3DXVec3Dot(&vObjectRightLook, &vNewLook) >= 0.0f)	//������ ��������
	{
		//������ ���⺤�Ϳ� ���ο� ������⺤���� �������� ����ϴ� ���͸� ���������� ȸ��
		Thetafull -= Theta;
	}
	D3DXMatrixRotationY(&m_matObjectrotation, Thetafull);	//ȸ��
	m_vObjectLook = vNewLook;	//���� ���⺤�͸� ���� ���� ������⺤�ͷ�
}

bool LAUnit::SetObjectABilityData()
{
	//�ʱ�ȭ
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
	//���� ��� ������Ʈ
	UpdateMotion();

	//�̵��� ������Ʈ
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
