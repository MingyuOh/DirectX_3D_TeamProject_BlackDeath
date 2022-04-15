
//#include "stdafx.h"


#include "LASpaceBox2.h"

void LASpaceBox2::UpDate(TMatrix& matDest)
{
	//센터값 저장
	vCenter.x = matDest._41;
	vCenter.y = matDest._42;
	vCenter.z = matDest._43;

	//min, max 값 셋팅
	D3DXVec3TransformCoord(&m_vUpdateMax, &vMax, &matDest);
	D3DXVec3TransformCoord(&m_vUpdateMin, &vMin, &matDest);

	//TVector3 vVertex[8];
	vPos[0] = TVector3(m_vUpdateMin.x, m_vUpdateMax.y, m_vUpdateMin.z);
	vPos[1] = TVector3(m_vUpdateMax.x, m_vUpdateMax.y, m_vUpdateMin.z);
	vPos[2] = TVector3(m_vUpdateMax.x, m_vUpdateMin.y, m_vUpdateMin.z);
	vPos[3] = TVector3(m_vUpdateMin.x, m_vUpdateMin.y, m_vUpdateMin.z);

	vPos[4] = TVector3(m_vUpdateMin.x, m_vUpdateMax.y, m_vUpdateMax.z);
	vPos[5] = TVector3(m_vUpdateMax.x, m_vUpdateMax.y, m_vUpdateMax.z);
	vPos[6] = TVector3(m_vUpdateMax.x, m_vUpdateMin.y, m_vUpdateMax.z);
	vPos[7] = TVector3(m_vUpdateMin.x, m_vUpdateMin.y, m_vUpdateMax.z);

	/*vPos[0] = TVector3(vMin.x, vMax.y, vMin.z);
	vPos[1] = TVector3(vMax.x, vMax.y, vMin.z);
	vPos[2] = TVector3(vMax.x, vMin.y, vMin.z);
	vPos[3] = TVector3(vMin.x, vMin.y, vMin.z);

	vPos[4] = TVector3(vMin.x, vMax.y, vMax.z);
	vPos[5] = TVector3(vMax.x, vMax.y, vMax.z);
	vPos[6] = TVector3(vMax.x, vMin.y, vMax.z);
	vPos[7] = TVector3(vMin.x, vMin.y, vMax.z);*/

	//축벡터 저장 및 정규화
	D3DXVec3TransformNormal(&vAxis[0], &vbaseAxis[0], &matDest);
	D3DXVec3TransformNormal(&vAxis[1], &vbaseAxis[1], &matDest);
	D3DXVec3TransformNormal(&vAxis[2], &vbaseAxis[2], &matDest);
	D3DXVec3Normalize(&vAxis[0], &vAxis[0]);
	D3DXVec3Normalize(&vAxis[1], &vAxis[1]);
	D3DXVec3Normalize(&vAxis[2], &vAxis[2]);

	//축길이 저장
	TVector3 fHarf = m_vUpdateMax - vCenter;
	fExtent[0] = D3DXVec3Dot(&vAxis[0], &fHarf);
	fExtent[1] = D3DXVec3Dot(&vAxis[1], &fHarf);
	fExtent[2] = D3DXVec3Dot(&vAxis[2], &fHarf);
	/*fExtent[0] = fabsf(vCenter.x - m_vUpdateMax.x);
	fExtent[1] = fabsf(vCenter.y - m_vUpdateMax.y);
	fExtent[2] = fabsf(vCenter.z - m_vUpdateMax.z);*/
}

void LASpaceBox2::UpDateMinMax(std::vector<PNCT_VECTOR>& vertexList)
{
	TVector3 min = TVector3(9999999, 9999999, 9999999);
	TVector3 max = TVector3(-9999999, -9999999, -9999999);

	for (int iCnt = 0; iCnt < (int)vertexList.size(); iCnt++)
	{
		if (min.x > vertexList[iCnt].Vertex.x)	min.x = vertexList[iCnt].Vertex.x;
		if (min.y > vertexList[iCnt].Vertex.y)	min.y = vertexList[iCnt].Vertex.y;
		if (min.z > vertexList[iCnt].Vertex.z)	min.z = vertexList[iCnt].Vertex.z;

		if (max.x < vertexList[iCnt].Vertex.x)	max.x = vertexList[iCnt].Vertex.x;
		if (max.y < vertexList[iCnt].Vertex.y)	max.y = vertexList[iCnt].Vertex.y;
		if (max.z < vertexList[iCnt].Vertex.z)	max.z = vertexList[iCnt].Vertex.z;
	}

	vMax = max;
	vMin = min;
}

LASpaceBox2::LASpaceBox2()
{
	//회전 미적용
	
	vMax = TVector3(1.0f, 1.0f, 1.0f);
	vMin = TVector3(-1.0f, -1.0f, -1.0f);

	vCenter = TVector3((float)100 - rand() % 200, (float)0, (float)100 - rand() % 200);

	//TVector3 vVertex[8];
	vPos[0] = TVector3(vMin.x, vMax.y, vMin.z);
	vPos[1] = TVector3(vMax.x, vMax.y, vMin.z);
	vPos[2] = TVector3(vMax.x, vMin.y, vMin.z);
	vPos[3] = TVector3(vMin.x, vMin.y, vMin.z);

	vPos[4] = TVector3(vMin.x, vMax.y, vMax.z);
	vPos[5] = TVector3(vMax.x, vMax.y, vMax.z);
	vPos[6] = TVector3(vMax.x, vMin.y, vMax.z);
	vPos[7] = TVector3(vMin.x, vMin.y, vMax.z);

	//회전 미적용 단위벡터
	vAxis[0] = TVector3(1.0f, 0.0f, 0.0f);
	vAxis[1] = TVector3(0.0f, 1.0f, 0.0f);
	vAxis[2] = TVector3(0.0f, 0.0f, 1.0f);
	vbaseAxis[0] = TVector3(1.0f, 0.0f, 0.0f);
	vbaseAxis[1] = TVector3(0.0f, 1.0f, 0.0f);
	vbaseAxis[2] = TVector3(0.0f, 0.0f, 1.0f);

	TVector3 fHarf = vMax - vCenter;
	fExtent[0] = D3DXVec3Dot(&vbaseAxis[0], &fHarf);
	fExtent[1] = D3DXVec3Dot(&vbaseAxis[1], &fHarf);
	fExtent[2] = D3DXVec3Dot(&vbaseAxis[2], &fHarf);
	
	fRadius = D3DXVec3Length(&fHarf);
}

LASpaceBox2::~LASpaceBox2()
{
}
