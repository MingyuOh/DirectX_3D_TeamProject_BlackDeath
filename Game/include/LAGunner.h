#pragma once

#include "LAUnit.h"

#include "LAItemControl.h"

#ifdef TOOL_MODE
#else
/*===================== UI Include ========================*/

#endif

class LAGunner : public LAUnit
{
public:
	LAItemControl* m_pItemControl;
	float m_fAnimationMoveSpeed;
	bool m_bIsReLoad;

#ifdef TOOL_MODE
#else
	/*===================== UI 멤버 변수 ========================*/

#endif

public:
#ifdef TOOL_MODE
#else
	/*===================== UI 함수 ========================*/
	float DistanceMonstertoHero();
	void MonsterMotionStateSet(int MotionNumber);
	void  MonsterEventOccurrence();
#endif

public:
	void MoveUp();
	void MoveRun();
	void Attack();
	void Death();
	bool UpdateBulletCollition();
	void ResetCheck();
	void SetCurrentMotion(int isetMotion);
	void UpdateAddAnimationSpeed();
public:
	void SetMatrix(D3DXMATRIX* pWorldMatrix, D3DXMATRIX* pViewMatrix, D3DXMATRIX* pProjectionMatrix);
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool Frame();
	bool Render();
	bool Release();
public:
	LAGunner()
	{
		m_pItemControl = new LAItemControl;
		m_fAnimationMoveSpeed = WORK_MOVE_SPEED;
		m_bIsReLoad = false;
	}
	virtual ~LAGunner();
};

