

//#include "stdafx.h"

#include "LASpaceSphere2.h"

void LASpaceSphere2::UpDate(TMatrix& matDest)
{
	TVector3 vDecomposeScale;
	TQuaternion vDecomposeRotation;
	TVector3 vDecomposeTrans;

	D3DXMatrixDecompose(&vDecomposeScale, &vDecomposeRotation, &vDecomposeTrans, &matDest);
	//센터값 저장
	vCenter.x = matDest._41;
	vCenter.y = matDest._42;
	vCenter.z = matDest._43;

	fRadius = vDecomposeScale.x;

	//min, max 값 셋팅
	D3DXVec3TransformCoord(&vMax, &vMax, &matDest);
	D3DXVec3TransformCoord(&vMin, &vMin, &matDest);

	//축벡터 저장 및 정규화
	D3DXVec3TransformNormal(&vAxis[0], &vAxis[0], &matDest);
	D3DXVec3TransformNormal(&vAxis[1], &vAxis[1], &matDest);
	D3DXVec3TransformNormal(&vAxis[2], &vAxis[2], &matDest);
	D3DXVec3Normalize(&vAxis[0], &vAxis[0]);
	D3DXVec3Normalize(&vAxis[1], &vAxis[1]);
	D3DXVec3Normalize(&vAxis[2], &vAxis[2]);

	//축길이 저장
	TVector3 fHarf = vMax - vCenter;
	fExtent[0] = D3DXVec3Dot(&vAxis[0], &fHarf);
	fExtent[1] = D3DXVec3Dot(&vAxis[1], &fHarf);
	fExtent[2] = D3DXVec3Dot(&vAxis[2], &fHarf);
}

LASpaceSphere2::LASpaceSphere2()
{
	vCenter = TVector3((float)100 - rand() % 200, (float)0, (float)100 - rand() % 200);
	fRadius = 1.0f;
}


LASpaceSphere2::~LASpaceSphere2()
{
}
