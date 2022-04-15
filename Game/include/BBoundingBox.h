#pragma once
#include "BCollision.h"

class BBoundingBox : public BObject
{
public:
	D3DXVECTOR3		m_vPosition;			// 바운딩박스 위치
	D3DXVECTOR4		m_vColor;				// 바운딩박스 컬러	
	UINT			m_Slices;				// 바운딩박스 분할개수
	double			m_fRadius;				// 바운딩박스 반지름
	D3DXVECTOR3		m_vCenter;				// 바운딩박스 센터
	D3DXVECTOR3		m_BoundingBoxMax;		// 바운딩박스 최대값
	D3DXVECTOR3		m_BoundingBoxMin;		// 바운딩박스 최소값
	BCollision		m_Collision;			// 바운딩박스 충돌 객체
public:
	//--------------------------------------------------------------------------------------
	// 바운딩박스 생성 함수
	//--------------------------------------------------------------------------------------
	bool			CreateBoundingBox(D3DXVECTOR3 vMax, D3DXVECTOR3 vMin, D3DXVECTOR3 vPos);
	//--------------------------------------------------------------------------------------
	// 바운딩박스 갱신 함수
	//--------------------------------------------------------------------------------------
	void			UpdateBoundingBox(D3DXVECTOR3 vPos);
public:
	BBoundingBox();
	~BBoundingBox();
};

