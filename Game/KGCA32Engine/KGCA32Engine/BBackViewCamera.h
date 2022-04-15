#pragma once
#include "BCamera.h"

class BBackViewCamera : public BCamera
{
public:
	/*카메라 보간이동의 목표지점*/
	TVector3 m_vCameraDestination;
	// 모델의 월드 변환 행렬의 방향벡터
	TVector3 m_vObjectVector[3];
	float m_fPrevRadius;
	TVector3 m_vHeroLook;
	TVector3 m_vHeroUp;
	TVector3 m_vHeroSide;

	TVector3 m_vDefaultEye;
	TVector3 m_vDefaultLookAt;

public:
	/* Hero 포지션 추가함수*/
	void SetHeroPosition(TVector3* pHeroPos);

	/*뷰행렬 생성함수*//*오브젝트의 방향벡터를 기준으로 이동*/
	virtual bool SetViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp);
	virtual bool Init(TVector3 vTarget);
	virtual bool Frame();
	virtual bool UpdateVector();
	virtual bool UpdateQuaternion();

public:
	BBackViewCamera();
	virtual ~BBackViewCamera();
};

