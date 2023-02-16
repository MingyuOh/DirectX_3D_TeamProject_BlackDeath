#pragma once
#include "BSphere.h"
#include "BQuadTree.h"

struct B_RAY
{
	float fExtent;
	D3DXVECTOR3 vOrigin; // 시작점 = 뷰좌표계의 카메라의 원점
	D3DXVECTOR3 vDirection; // RAY의 방향백터
};

class BSelect
{
public:
	B_RAY m_Ray;
	D3DXVECTOR3 m_vIntersection;
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXVECTOR3 m_vDxR;
	D3DXVECTOR3 m_vCenter;

	D3DXVECTOR3 m_vSrcVertex[3];
	float m_fPickDistance;
	D3DXMATRIX m_matWorldPick;
	D3DXVECTOR3 Qvec;
	D3DXVECTOR3 Tvec;
	D3DXVECTOR3 Pvec;

	D3DXVECTOR3 CrashPoint;

public:
	bool CheckPick(D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2);
	bool IntersectTriangle(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir, D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2, FLOAT* t, FLOAT *u, FLOAT *v);
	bool IntersectRect(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,B_BOX m_tBox, FLOAT* t, FLOAT *u, FLOAT *v);
	void Update();
	void SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	bool IntersectBox(B_BOX* pBox, B_RAY* pRay = NULL);
	bool CheckOBBToRay(B_BOX* pBox, B_RAY* pRay = NULL);
	bool IntersectRayToSphere(BSphere* pSphere, B_RAY* pRay);
	bool CheckOBBToOBB(B_BOX& box0, B_BOX& box1);
	BNode* CheckRootNode(BNode* RootNode, B_RAY* pRay = NULL);
	BNode* CheckNode(BNode* pNode, B_RAY* pRay = NULL);

public:
	BSelect();
	~BSelect();
};

