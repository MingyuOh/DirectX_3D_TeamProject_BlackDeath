#pragma once
#include "BCollision.h"

class BBoundingBox : public BObject
{
public:
	TVector3		m_vPosition;			// �ٿ���ڽ� ��ġ
	TVector4		m_vColor;				// �ٿ���ڽ� �÷�	
	UINT			m_Slices;				// �ٿ���ڽ� ���Ұ���
	double			m_fRadius;				// �ٿ���ڽ� ������
	TVector3		m_vCenter;				// �ٿ���ڽ� ����
	TVector3		m_BoundingBoxMax;		// �ٿ���ڽ� �ִ밪
	TVector3		m_BoundingBoxMin;		// �ٿ���ڽ� �ּҰ�
	BCollision		m_Collision;			// �ٿ���ڽ� �浹 ��ü
public:
	//--------------------------------------------------------------------------------------
	// �ٿ���ڽ� ���� �Լ�
	//--------------------------------------------------------------------------------------
	bool			CreateBoundingBox(TVector3 vMax, TVector3 vMin, TVector3 vPos);
	//--------------------------------------------------------------------------------------
	// �ٿ���ڽ� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			UpdateBoundingBox(TVector3 vPos);
public:
	BBoundingBox();
	~BBoundingBox();
};

