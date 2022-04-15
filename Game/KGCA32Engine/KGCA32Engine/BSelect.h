#pragma once
#include "BSphere.h"
#include "BQuadTree.h"

struct B_RAY
{
	float fExtent;
	TVector3 vOrigin; // 시작점 = 뷰좌표계의 카메라의 원점
	TVector3 vDirection; // RAY의 방향백터
};

class BSelect
{
public:
	B_RAY m_Ray;
	TVector3 m_vIntersection;
	TMatrix m_matWorld;
	TMatrix m_matView;
	TMatrix m_matProj;
	TVector3 m_vDxR;
	TVector3 m_vCenter;

	TVector3 m_vSrcVertex[3];
	float m_fPickDistance;
	TMatrix m_matWorldPick;
	TVector3 Qvec;
	TVector3 Tvec;
	TVector3 Pvec;

	TVector3 CrashPoint;

public:
	bool CheckPick(TVector3& v0, TVector3& v1, TVector3& v2);
	bool IntersectTriangle(const TVector3& orig, const TVector3& dir, TVector3& v0, TVector3& v1, TVector3& v2, FLOAT* t, FLOAT *u, FLOAT *v);
	bool IntersectRect(const TVector3& orig, const TVector3& dir,B_BOX m_tBox, FLOAT* t, FLOAT *u, FLOAT *v);
	void Update();
	void SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj);
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

