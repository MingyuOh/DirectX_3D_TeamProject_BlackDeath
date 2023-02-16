#pragma once
#include "BCamera.h"

class BBackViewCamera : public BCamera
{
public:
	/*ī�޶� �����̵��� ��ǥ����*/
	D3DXVECTOR3 m_vCameraDestination;
	// ���� ���� ��ȯ ����� ���⺤��
	D3DXVECTOR3 m_vObjectVector[3];
	float m_fPrevRadius;
	D3DXVECTOR3 m_vHeroLook;
	D3DXVECTOR3 m_vHeroUp;
	D3DXVECTOR3 m_vHeroSide;

	D3DXVECTOR3 m_vDefaultEye;
	D3DXVECTOR3 m_vDefaultLookAt;

public:
	/* Hero ������ �߰��Լ�*/
	void SetHeroPosition(D3DXVECTOR3* pHeroPos);

	/*����� �����Լ�*//*������Ʈ�� ���⺤�͸� �������� �̵�*/
	virtual bool SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp);
	virtual bool Init(D3DXVECTOR3 vTarget);
	virtual bool Frame();
	virtual bool UpdateVector();
	virtual bool UpdateQuaternion();

public:
	BBackViewCamera();
	virtual ~BBackViewCamera();
};
