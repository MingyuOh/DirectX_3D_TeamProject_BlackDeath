#include "BFrustum.h"

bool BFrustum::CreateFrustum(ID3D11Device* m_pd3dDevice, const TCHAR* pShaderFile)
{
	S_NEW(m_pBoxShape, BBox);
	m_pBoxShape->m_VertexList.resize(24);
	m_pBoxShape->m_VertexList[0].Normal = TVector3(0.0f, 0.0f, -1.0f);
	m_pBoxShape->m_VertexList[1].Normal = TVector3(0.0f, 0.0f, -1.0f);
	m_pBoxShape->m_VertexList[2].Normal = TVector3(0.0f, 0.0f, -1.0f);
	m_pBoxShape->m_VertexList[3].Normal = TVector3(0.0f, 0.0f, -1.0f);

	m_pBoxShape->m_VertexList[4].Normal = TVector3(0.0f, 0.0f, 1.0f);
	m_pBoxShape->m_VertexList[5].Normal = TVector3(0.0f, 0.0f, 1.0f);
	m_pBoxShape->m_VertexList[6].Normal = TVector3(0.0f, 0.0f, 1.0f);
	m_pBoxShape->m_VertexList[7].Normal = TVector3(0.0f, 0.0f, 1.0f);

	m_pBoxShape->m_VertexList[8].Normal = TVector3(1.0f, 0.0f, 0.0f);
	m_pBoxShape->m_VertexList[9].Normal = TVector3(1.0f, 0.0f, 0.0f);
	m_pBoxShape->m_VertexList[10].Normal = TVector3(1.0f, 0.0f, 0.0f);
	m_pBoxShape->m_VertexList[11].Normal = TVector3(1.0f, 0.0f, 0.0f);

	m_pBoxShape->m_VertexList[12].Normal = TVector3(-1.0f, 0.0f, 0.0f);
	m_pBoxShape->m_VertexList[13].Normal = TVector3(-1.0f, 0.0f, 0.0f);
	m_pBoxShape->m_VertexList[14].Normal = TVector3(-1.0f, 0.0f, 0.0f);
	m_pBoxShape->m_VertexList[15].Normal = TVector3(-1.0f, 0.0f, 0.0f);

	m_pBoxShape->m_VertexList[16].Normal = TVector3(0.0f, 1.0f, 0.0f);
	m_pBoxShape->m_VertexList[17].Normal = TVector3(0.0f, 1.0f, 0.0f);
	m_pBoxShape->m_VertexList[18].Normal = TVector3(0.0f, 1.0f, 0.0f);
	m_pBoxShape->m_VertexList[19].Normal = TVector3(0.0f, 1.0f, 0.0f);

	m_pBoxShape->m_VertexList[20].Normal = TVector3(0.0f, -1.0f, 0.0f);
	m_pBoxShape->m_VertexList[21].Normal = TVector3(0.0f, -1.0f, 0.0f);
	m_pBoxShape->m_VertexList[22].Normal = TVector3(0.0f, -1.0f, 0.0f);
	m_pBoxShape->m_VertexList[23].Normal = TVector3(0.0f, -1.0f, 0.0f);

	m_pBoxShape->m_VertexList[0].Color = TVector4(0.0f, 0.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[1].Color = TVector4(0.0f, 0.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[2].Color = TVector4(0.0f, 0.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[3].Color = TVector4(0.0f, 0.0f, 0.0f, 0.3f);

	m_pBoxShape->m_VertexList[4].Color = TVector4(0.0f, 1.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[5].Color = TVector4(0.0f, 1.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[6].Color = TVector4(0.0f, 1.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[7].Color = TVector4(0.0f, 1.0f, 0.0f, 0.3f);

	m_pBoxShape->m_VertexList[8].Color = TVector4(0.0f, 0.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[9].Color = TVector4(0.0f, 0.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[10].Color = TVector4(0.0f, 0.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[11].Color = TVector4(0.0f, 0.0f, 1.0f, 0.3f);

	m_pBoxShape->m_VertexList[12].Color = TVector4(1.0f, 0.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[13].Color = TVector4(1.0f, 0.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[14].Color = TVector4(1.0f, 0.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[15].Color = TVector4(1.0f, 0.0f, 1.0f, 0.3f);

	m_pBoxShape->m_VertexList[16].Color = TVector4(1.0f, 1.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[17].Color = TVector4(1.0f, 1.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[18].Color = TVector4(1.0f, 1.0f, 0.0f, 0.3f);
	m_pBoxShape->m_VertexList[19].Color = TVector4(1.0f, 1.0f, 0.0f, 0.3f);

	m_pBoxShape->m_VertexList[20].Color = TVector4(0.0f, 1.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[21].Color = TVector4(0.0f, 1.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[22].Color = TVector4(0.0f, 1.0f, 1.0f, 0.3f);
	m_pBoxShape->m_VertexList[23].Color = TVector4(0.0f, 1.0f, 1.0f, 0.3f);

	m_pBoxShape->m_VertexList[0].Texture = TVector2(0.0f, 0.0f);
	m_pBoxShape->m_VertexList[1].Texture = TVector2(1.0f, 0.0f);
	m_pBoxShape->m_VertexList[2].Texture = TVector2(1.0f, 1.0f);
	m_pBoxShape->m_VertexList[3].Texture = TVector2(0.0f, 1.0f);

	m_pBoxShape->m_VertexList[4].Texture = TVector2(0.0f, 0.0f);
	m_pBoxShape->m_VertexList[5].Texture = TVector2(1.0f, 0.0f);
	m_pBoxShape->m_VertexList[6].Texture = TVector2(1.0f, 1.0f);
	m_pBoxShape->m_VertexList[7].Texture = TVector2(0.0f, 1.0f);

	m_pBoxShape->m_VertexList[8].Texture = TVector2(0.0f, 0.0f);
	m_pBoxShape->m_VertexList[9].Texture = TVector2(1.0f, 0.0f);
	m_pBoxShape->m_VertexList[10].Texture = TVector2(1.0f, 1.0f);
	m_pBoxShape->m_VertexList[11].Texture = TVector2(0.0f, 1.0f);

	m_pBoxShape->m_VertexList[12].Texture = TVector2(0.0f, 0.0f);
	m_pBoxShape->m_VertexList[13].Texture = TVector2(1.0f, 0.0f);
	m_pBoxShape->m_VertexList[14].Texture = TVector2(1.0f, 1.0f);
	m_pBoxShape->m_VertexList[15].Texture = TVector2(0.0f, 1.0f);

	m_pBoxShape->m_VertexList[16].Texture = TVector2(0.0f, 0.0f);
	m_pBoxShape->m_VertexList[17].Texture = TVector2(1.0f, 0.0f);
	m_pBoxShape->m_VertexList[18].Texture = TVector2(1.0f, 1.0f);
	m_pBoxShape->m_VertexList[19].Texture = TVector2(0.0f, 1.0f);

	m_pBoxShape->m_VertexList[20].Texture = TVector2(0.0f, 0.0f);
	m_pBoxShape->m_VertexList[21].Texture = TVector2(1.0f, 0.0f);
	m_pBoxShape->m_VertexList[22].Texture = TVector2(1.0f, 1.0f);
	m_pBoxShape->m_VertexList[23].Texture = TVector2(0.0f, 1.0f);


	D3DXMatrixMultiply(&m_matViewProj, &m_matView, &m_matProj);
	D3DXMatrixInverse(&m_matViewProj, NULL, &m_matViewProj);

	// 0123 = 전면 4567 = 후면
	//     5    6
	// 1    2
	//     4    7
	// 0    3  

	m_vFrustum[0] = TVector3(-1.0f, -1.0f, 0.0f);
	m_vFrustum[1] = TVector3(-1.0f,  1.0f, 0.0f);
	m_vFrustum[2] = TVector3( 1.0f,  1.0f, 0.0f);
	m_vFrustum[3] = TVector3( 1.0f, -1.0f, 0.0f);

	m_vFrustum[4] = TVector3(-1.0f, -1.0f, 1.0f);
	m_vFrustum[5] = TVector3(-1.0f,  1.0f, 1.0f);
	m_vFrustum[6] = TVector3( 1.0f,  1.0f, 1.0f);
	m_vFrustum[7] = TVector3( 1.0f, -1.0f, 1.0f);

	for (int iVertex = 0; iVertex < 8; iVertex++)
	{
		D3DXVec3TransformCoord(&m_vFrustum[iVertex], &m_vFrustum[iVertex], &m_matViewProj);
	}

	/*내적했을때 - 가 나오면 밖, + 가 나오면 안쪽이 나오도록 반시계방향으로 판 생성*/
	m_Plane[0].CreatePlane(m_vFrustum[5], m_vFrustum[0], m_vFrustum[1]); // 좌
	m_Plane[1].CreatePlane(m_vFrustum[3], m_vFrustum[6], m_vFrustum[2]); // 우
	m_Plane[2].CreatePlane(m_vFrustum[5], m_vFrustum[2], m_vFrustum[6]); // 상
	m_Plane[3].CreatePlane(m_vFrustum[0], m_vFrustum[7], m_vFrustum[3]); // 하
	m_Plane[4].CreatePlane(m_vFrustum[0], m_vFrustum[2], m_vFrustum[1]); // 앞쪽
	m_Plane[5].CreatePlane(m_vFrustum[6], m_vFrustum[4], m_vFrustum[5]); // 뒤쪽

																		 //앞
	m_pBoxShape->m_VertexList[0].Position = m_vFrustum[1];
	m_pBoxShape->m_VertexList[1].Position = m_vFrustum[2];
	m_pBoxShape->m_VertexList[2].Position = m_vFrustum[3];
	m_pBoxShape->m_VertexList[3].Position = m_vFrustum[0];
	//뒤
	m_pBoxShape->m_VertexList[4].Position = m_vFrustum[6];
	m_pBoxShape->m_VertexList[5].Position = m_vFrustum[5];
	m_pBoxShape->m_VertexList[6].Position = m_vFrustum[4];
	m_pBoxShape->m_VertexList[7].Position = m_vFrustum[7];
	//왼
	m_pBoxShape->m_VertexList[8].Position = m_vFrustum[5];
	m_pBoxShape->m_VertexList[9].Position = m_vFrustum[1];
	m_pBoxShape->m_VertexList[10].Position = m_vFrustum[0];
	m_pBoxShape->m_VertexList[11].Position = m_vFrustum[4];
	//오른
	m_pBoxShape->m_VertexList[12].Position = m_vFrustum[2];
	m_pBoxShape->m_VertexList[13].Position = m_vFrustum[6];
	m_pBoxShape->m_VertexList[14].Position = m_vFrustum[7];
	m_pBoxShape->m_VertexList[15].Position = m_vFrustum[3];
	//위
	m_pBoxShape->m_VertexList[16].Position = m_vFrustum[5];
	m_pBoxShape->m_VertexList[17].Position = m_vFrustum[6];
	m_pBoxShape->m_VertexList[18].Position = m_vFrustum[2];
	m_pBoxShape->m_VertexList[19].Position = m_vFrustum[1];
	//아래
	m_pBoxShape->m_VertexList[20].Position = m_vFrustum[0];
	m_pBoxShape->m_VertexList[21].Position = m_vFrustum[3];
	m_pBoxShape->m_VertexList[22].Position = m_vFrustum[7];
	m_pBoxShape->m_VertexList[23].Position = m_vFrustum[4];

	m_pBoxShape->m_VertexCnt = m_pBoxShape->m_VertexList.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						// [메모리 할당 위치] GPU 메모리에 할당해라
	bd.ByteWidth = sizeof(PNCT_Vertex) * m_pBoxShape->m_VertexCnt;	// [메모리 할당 크기] 점 X개를 할당 할 것이기 때문에 *X ( 수가 늘어나면 증가 할 것이다 )
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// [메모리 사용 용도] 버텍스 버퍼용으로 사용하겠다.
	bd.CPUAccessFlags = 0;								// CPU로 부터는 액세스 하지 않겠다.
	bd.MiscFlags = 0;									// 추가 설정 없음

	D3D11_SUBRESOURCE_DATA InitData;			// 소스 리소스 초기화에 필요한 데이터 지정
	InitData.pSysMem = (void*)&m_pBoxShape->m_VertexList.at(0);				// 해당 버텍스를 소스로 사용하겠다.
	m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pBoxShape->m_pVertexBuffer);

	m_pBoxShape->m_pd3dDevice = m_pd3dDevice;
	m_pBoxShape->CreateIndexBuffer();
	m_pBoxShape->LoadShaderAndInputLayout(pShaderFile);
	m_pBoxShape->CreateConstantBuffer();

	return true;
}

BOOL BFrustum::CheckPoint(TVector3* pv)
{
	float fPlaneToPoint;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		fPlaneToPoint = m_Plane[iPlane].fA * pv->x + m_Plane[iPlane].fB * pv->y + m_Plane[iPlane].fC * pv->z + m_Plane[iPlane].fD;
		if (fPlaneToPoint < 0.0f)
			return FALSE;
	}
	return TRUE;
}

BOOL BFrustum::CheckSphere(BSphere* pSphere)
{
	float fPlaneToCenter;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		fPlaneToCenter = m_Plane[iPlane].fA * pSphere->m_vCenter.x + m_Plane[iPlane].fB * pSphere->m_vCenter.y + m_Plane[iPlane].fC * pSphere->m_vCenter.z + m_Plane[iPlane].fD;
		if (fPlaneToCenter <= -pSphere->m_fRadius)
			return FALSE;
	}
	return TRUE;
}

BOOL BFrustum::CheckOBB(B_BOX* pBox)
{
	float fPlaneToCenter = 0.0f;
	float fDistance = 0.0f;
	TVector3 vDir;

	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		fDistance = fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB * vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB * vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB * vDir.y + m_Plane[iPlane].fC * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].fA * pBox->vCenter.x + m_Plane[iPlane].fB * pBox->vCenter.y + m_Plane[iPlane].fC * pBox->vCenter.z + m_Plane[iPlane].fD;

		if (fPlaneToCenter <= -fDistance)
		{
			return FALSE;
		}
	}
	return TRUE;
}

T_POSITION BFrustum::CheckPositionOBBInPlane(B_BOX* pBox)
{
	float fPlaneToCenter = 0.0f;
	float fDistance = 0.0f;
	TVector3 vDir;
	T_POSITION t_Position;

	t_Position = P_FRONT;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		vDir = pBox->vAxis[0] * pBox->fExtent[0];
		fDistance = fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[1] * pBox->fExtent[1];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);
		vDir = pBox->vAxis[2] * pBox->fExtent[2];
		fDistance += fabs(m_Plane[iPlane].fA * vDir.x + m_Plane[iPlane].fB*vDir.y + m_Plane[iPlane].fC * vDir.z);

		fPlaneToCenter = m_Plane[iPlane].fA * pBox->vCenter.x + m_Plane[iPlane].fB*pBox->vCenter.y +
			m_Plane[iPlane].fC * pBox->vCenter.z + m_Plane[iPlane].fD;

		if (fPlaneToCenter <= fDistance)
		{
			t_Position = P_SPANNING;
		}
		if (fPlaneToCenter + 2.0f < -fDistance)
		{
			return P_BACK;
		}
	}
	return t_Position;
}

bool BFrustum::FrustRender(ID3D11DeviceContext* pContext)
{
	D3DXMatrixMultiply(&m_matViewProj, &m_matView, &m_matProj);
	D3DXMatrixInverse(&m_matViewProj, NULL, &m_matViewProj);

	// 0123 = 전면 4567 = 후면
	//     5    6
	// 1    2
	//     4    7
	// 0    3  

	m_vFrustum[0] = TVector3(-1.0f, -1.0f, 0.0f);
	m_vFrustum[1] = TVector3(-1.0f, 1.0f, 0.0f);
	m_vFrustum[2] = TVector3(1.0f, 1.0f, 0.0f);
	m_vFrustum[3] = TVector3(1.0f, -1.0f, 0.0f);

	m_vFrustum[4] = TVector3(-1.0f, -1.0f, 1.0f);
	m_vFrustum[5] = TVector3(-1.0f, 1.0f, 1.0f);
	m_vFrustum[6] = TVector3(1.0f, 1.0f, 1.0f);
	m_vFrustum[7] = TVector3(1.0f, -1.0f, 1.0f);

	for (int iVertex = 0; iVertex < 8; iVertex++)
	{
		D3DXVec3TransformCoord(&m_vFrustum[iVertex], &m_vFrustum[iVertex], &m_matViewProj);
	}

	m_Plane[0].CreatePlane(m_vFrustum[5], m_vFrustum[0], m_vFrustum[1]); // 좌
	m_Plane[1].CreatePlane(m_vFrustum[3], m_vFrustum[6], m_vFrustum[2]); // 우
	m_Plane[2].CreatePlane(m_vFrustum[5], m_vFrustum[2], m_vFrustum[6]); // 상
	m_Plane[3].CreatePlane(m_vFrustum[0], m_vFrustum[7], m_vFrustum[3]); // 하
	m_Plane[4].CreatePlane(m_vFrustum[0], m_vFrustum[2], m_vFrustum[1]); // 앞쪽
	m_Plane[5].CreatePlane(m_vFrustum[6], m_vFrustum[4], m_vFrustum[5]); // 뒤쪽

	//앞
	m_pBoxShape->m_VertexList[0].Position = m_vFrustum[1];
	m_pBoxShape->m_VertexList[1].Position = m_vFrustum[2];
	m_pBoxShape->m_VertexList[2].Position = m_vFrustum[3];
	m_pBoxShape->m_VertexList[3].Position = m_vFrustum[0];
	//뒤
	m_pBoxShape->m_VertexList[4].Position = m_vFrustum[6];
	m_pBoxShape->m_VertexList[5].Position = m_vFrustum[5];
	m_pBoxShape->m_VertexList[6].Position = m_vFrustum[4];
	m_pBoxShape->m_VertexList[7].Position = m_vFrustum[7];
	//왼
	m_pBoxShape->m_VertexList[8].Position = m_vFrustum[5];
	m_pBoxShape->m_VertexList[9].Position = m_vFrustum[1];
	m_pBoxShape->m_VertexList[10].Position = m_vFrustum[0];
	m_pBoxShape->m_VertexList[11].Position = m_vFrustum[4];
	//오른
	m_pBoxShape->m_VertexList[12].Position = m_vFrustum[2];
	m_pBoxShape->m_VertexList[13].Position = m_vFrustum[6];
	m_pBoxShape->m_VertexList[14].Position = m_vFrustum[7];
	m_pBoxShape->m_VertexList[15].Position = m_vFrustum[3];
	//위
	m_pBoxShape->m_VertexList[16].Position = m_vFrustum[5];
	m_pBoxShape->m_VertexList[17].Position = m_vFrustum[6];
	m_pBoxShape->m_VertexList[18].Position = m_vFrustum[2];
	m_pBoxShape->m_VertexList[19].Position = m_vFrustum[1];
	//아래
	m_pBoxShape->m_VertexList[20].Position = m_vFrustum[0];
	m_pBoxShape->m_VertexList[21].Position = m_vFrustum[3];
	m_pBoxShape->m_VertexList[22].Position = m_vFrustum[7];
	m_pBoxShape->m_VertexList[23].Position = m_vFrustum[4];
	
	pContext->UpdateSubresource(m_pBoxShape->m_pVertexBuffer, 0, NULL, &m_pBoxShape->m_VertexList[0], 0, 0);
	
	return true;
}

BFrustum::BFrustum()
{
}


BFrustum::~BFrustum()
{
}
