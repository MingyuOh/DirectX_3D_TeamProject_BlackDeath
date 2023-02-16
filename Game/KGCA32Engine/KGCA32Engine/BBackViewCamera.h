#pragma once
#include "BCamera.h"

class BBackViewCamera : public BCamera
{
public:
	/*ī�޶� �����̵��� ��ǥ����*/
	TVector3 m_vCameraDestination;
	// ���� ���� ��ȯ ����� ���⺤��
	TVector3 m_vObjectVector[3];
	float m_fPrevRadius;
	TVector3 m_vHeroLook;
	TVector3 m_vHeroUp;
	TVector3 m_vHeroSide;

	TVector3 m_vDefaultEye;
	TVector3 m_vDefaultLookAt;

public:
	/* Hero ������ �߰��Լ�*/
	void SetHeroPosition(TVector3* pHeroPos);

	/*����� �����Լ�*//*������Ʈ�� ���⺤�͸� �������� �̵�*/
	virtual bool SetViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp);
	virtual bool Init(TVector3 vTarget);
	virtual bool Frame();
	virtual bool UpdateVector();
	virtual bool UpdateQuaternion();

public:
	BBackViewCamera();
	virtual ~BBackViewCamera();
};

