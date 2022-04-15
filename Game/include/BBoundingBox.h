#pragma once
#include "BCollision.h"

class BBoundingBox : public BObject
{
public:
	D3DXVECTOR3		m_vPosition;			// �ٿ���ڽ� ��ġ
	D3DXVECTOR4		m_vColor;				// �ٿ���ڽ� �÷�	
	UINT			m_Slices;				// �ٿ���ڽ� ���Ұ���
	double			m_fRadius;				// �ٿ���ڽ� ������
	D3DXVECTOR3		m_vCenter;				// �ٿ���ڽ� ����
	D3DXVECTOR3		m_BoundingBoxMax;		// �ٿ���ڽ� �ִ밪
	D3DXVECTOR3		m_BoundingBoxMin;		// �ٿ���ڽ� �ּҰ�
	BCollision		m_Collision;			// �ٿ���ڽ� �浹 ��ü
public:
	//--------------------------------------------------------------------------------------
	// �ٿ���ڽ� ���� �Լ�
	//--------------------------------------------------------------------------------------
	bool			CreateBoundingBox(D3DXVECTOR3 vMax, D3DXVECTOR3 vMin, D3DXVECTOR3 vPos);
	//--------------------------------------------------------------------------------------
	// �ٿ���ڽ� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			UpdateBoundingBox(D3DXVECTOR3 vPos);
public:
	BBoundingBox();
	~BBoundingBox();
};

