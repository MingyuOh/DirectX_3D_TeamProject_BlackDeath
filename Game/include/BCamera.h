#pragma once
#include "BFrustum.h"
class BCamera : public BFrustum
{
public:
	/*카메라와 물체의 위치 벡터*/
	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vTarget;
	D3DXVECTOR3 m_vUp;

	/*위치를 나타내는 방향벡터*/
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vSide;

	/* 히어로 포지션주소 */
	D3DXVECTOR3* m_pHeroPos;

	/*카메라 회전 정보*/
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;
	float m_SpeedUp;

	/*MFC 변수*/
	bool bCameraChange;
public:
	virtual bool Init(D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 15.0f, -10.0f), D3DXVECTOR3 Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	bool SetProjMatrix(int iWidth, int iHeight, float fFov = D3DX_PI * 0.25, float fNear = 0.1f, float fFar = 1000.0f);
	virtual bool SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp);
	virtual bool UpdateVector();
	bool UpdateQuaternion();
	
	virtual void MoveLook(float fValue);
	virtual void MoveSide(float fValue);
	virtual void MoveUp(float fValue);


public:
	BCamera();
	virtual ~BCamera();
};

