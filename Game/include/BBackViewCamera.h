#pragma once
#include "BCamera.h"

class BBackViewCamera : public BCamera
{
public:
	/*카메라 보간이동의 목표지점*/
	D3DXVECTOR3 m_vCameraDestination;
	// 모델의 월드 변환 행렬의 방향벡터
	D3DXVECTOR3 m_vObjectVector[3];
	float m_fPrevRadius;
	D3DXVECTOR3 m_vHeroLook;
	D3DXVECTOR3 m_vHeroUp;
	D3DXVECTOR3 m_vHeroSide;

	D3DXVECTOR3 m_vDefaultEye;
	D3DXVECTOR3 m_vDefaultLookAt;

public:
	/* Hero 포지션 추가함수*/
	void SetHeroPosition(D3DXVECTOR3* pHeroPos);

	/*뷰행렬 생성함수*//*오브젝트의 방향벡터를 기준으로 이동*/
	virtual bool SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp);
	virtual bool Init(D3DXVECTOR3 vTarget);
	virtual bool Frame();
	virtual bool UpdateVector();
	virtual bool UpdateQuaternion();

public:
	BBackViewCamera();
	virtual ~BBackViewCamera();
};

