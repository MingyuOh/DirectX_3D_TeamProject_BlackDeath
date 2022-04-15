#include "BSphere.h"

HRESULT BSphere::CreateVertexBuffer()		// 버텍스 버퍼 생성
{
	HRESULT hr = S_OK;

	m_VertexArray.clear();
	m_VertexArray.resize(m_Slices*m_Slices);

	for (DWORD jCnt = 0; jCnt < m_Slices; jCnt++)
	{
		FLOAT theta = (PI*jCnt) / (m_Slices);
		for (DWORD iCnt = 0; iCnt < m_Slices; iCnt++)
		{
			int iPos = jCnt * m_Slices + iCnt;
			FLOAT phi = (2 * PI*iCnt) / (m_Slices);
			m_VertexArray[iPos].x = m_fRadius * (float)(sin(theta)*cos(phi)) + m_vCenter.x;
			m_VertexArray[iPos].z = m_fRadius * (float)(sin(theta)*sin(phi)) + m_vCenter.y;
			m_VertexArray[iPos].y = m_fRadius * (float)(cos(theta)) + m_vCenter.z;
		}
	}
	//const double PI = 3.141592653589793238462643383279502884197;
	int iPos = 0;
	int iNext = 0;
	for (DWORD j = 0; j < m_Slices; j++)
	{
		for (DWORD i = 0; i < m_Slices; i++)
		{
			if (i == m_Slices - 1) iNext = 0;
			else iNext = i + 1;
			PNCT_Vertex v0, v1, v2, v3, v4, v5;
			iPos = (j*m_Slices * 6) + (i * 6);

			v0.Position = m_VertexArray[j*m_Slices + i];
			v1.Position = m_VertexArray[j*m_Slices + iNext];

			if (j != m_Slices - 1) v2.Position = m_VertexArray[((j + 1)*m_Slices) + i];
			else  v2.Position = TVector3(0, -1, 0);

			v3.Position = v2.Position;
			v4.Position = v1.Position;

			if (j != m_Slices - 1) v5.Position = m_VertexArray[((j + 1)*m_Slices) + iNext];
			else  v5.Position = TVector3(0, -1, 0);

			D3DXVec3Normalize(&v0.Normal, &v0.Position);
			D3DXVec3Normalize(&v1.Normal, &v1.Position);
			D3DXVec3Normalize(&v2.Normal, &v2.Position);
			D3DXVec3Normalize(&v3.Normal, &v3.Position);
			D3DXVec3Normalize(&v4.Normal, &v4.Position);
			D3DXVec3Normalize(&v5.Normal, &v5.Position);

			v0.Color = TVector4(0.0f, 0.0f, 0.0f, 1.0f);
			v1.Color = TVector4(0.0f, 0.0f, 0.0f, 1.0f);
			v2.Color = TVector4(0.0f, 0.0f, 0.0f, 1.0f);
			v3.Color = TVector4(0.0f, 0.0f, 0.0f, 1.0f);
			v4.Color = TVector4(0.0f, 0.0f, 0.0f, 1.0f);
			v5.Color = TVector4(0.0f, 0.0f, 0.0f, 1.0f);

			v0.Texture = TVector2(atan2(v0.Normal.z, v0.Normal.x) / (PI*2.0f) + 0.5f, asinf(-v0.Normal.y) / PI + 0.5f);
			v1.Texture = TVector2(atan2(v1.Normal.z, v1.Normal.x) / (PI*2.0f) + 0.5f, asinf(-v1.Normal.y) / PI + 0.5f);
			v2.Texture = TVector2(atan2(v2.Normal.z, v2.Normal.x) / (PI*2.0f) + 0.5f, asinf(-v2.Normal.y) / PI + 0.5f);
			v3.Texture = TVector2(atan2(v3.Normal.z, v3.Normal.x) / (PI*2.0f) + 0.5f, asinf(-v3.Normal.y) / PI + 0.5f);
			v4.Texture = TVector2(atan2(v4.Normal.z, v4.Normal.x) / (PI*2.0f) + 0.5f, asinf(-v4.Normal.y) / PI + 0.5f);
			v5.Texture = TVector2(atan2(v5.Normal.z, v5.Normal.x) / (PI*2.0f) + 0.5f, asinf(-v5.Normal.y) / PI + 0.5f);

			if (fabs(v1.Texture.x - v0.Texture.x) > 0.9f)
			{
				if (fabs(v1.Texture.x) <= 0.01f)
				{
					v1.Texture.x = 1.0f;
				}
				if (fabs(v0.Texture.x) <= 0.01f)
				{
					v0.Texture.x = 1.0f;
				}
			}
			if (fabs(v2.Texture.x - v0.Texture.x) > 0.9f)
			{
				if (fabs(v2.Texture.x) <= 0.01f)
				{
					v2.Texture.x = 1.0f;
				}
				if (fabs(v0.Texture.x) <= 0.01f)
				{
					v0.Texture.x = 1.0f;
				}
			}
			if (fabs(v4.Texture.x - v3.Texture.x) > 0.9f)
			{
				if (fabs(v4.Texture.x) <= 0.01f)
				{
					v4.Texture.x = 1.0f;
				}
				if (fabs(v3.Texture.x) <= 0.01f)
				{
					v3.Texture.x = 1.0f;
				}
			}
			if (fabs(v5.Texture.x - v3.Texture.x) > 0.9f)
			{
				if (fabs(v5.Texture.x) <= 0.01f)
				{
					v5.Texture.x = 1.0f;
				}
				if (fabs(v3.Texture.x) <= 0.01f)
				{
					v3.Texture.x = 1.0f;
				}
			}
			m_VertexList.push_back(v0);
			m_VertexList.push_back(v1);
			m_VertexList.push_back(v2);
			m_VertexList.push_back(v3);
			m_VertexList.push_back(v4);
			m_VertexList.push_back(v5);
		}
	}
	m_VertexArray.clear();
	m_VertexCnt = m_VertexList.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						// [메모리 할당 위치] GPU 메모리에 할당해라
	bd.ByteWidth = sizeof(PNCT_Vertex) * m_VertexCnt;	// [메모리 할당 크기] 점 X개를 할당 할 것이기 때문에 *X ( 수가 늘어나면 증가 할 것이다 )
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// [메모리 사용 용도] 버텍스 버퍼용으로 사용하겠다.
	bd.CPUAccessFlags = 0;								// CPU로 부터는 액세스 하지 않겠다.
	bd.MiscFlags = 0;									// 추가 설정 없음

	D3D11_SUBRESOURCE_DATA InitData;			// 소스 리소스 초기화에 필요한 데이터 지정
	InitData.pSysMem = (void*)&m_VertexList.at(0);				// 해당 버텍스를 소스로 사용하겠다.
	S_RETURN(m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer));
	return hr;
}

bool BSphere::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
	pContext->Draw(m_VertexList.size(), 0);
	return true;
}

/*샘플의 프레임에서 선언하여 구의 움직임을 가능케 한다.*/
bool BSphere::UpdateQuaternion()
{
	/*공의 공중컨트롤 요소*/
	//TQuaternion qRotation;
	//// x축 행렬과 y축 행렬이 결합되어 새로운 행렬을 생성
	//TQuaternionRotationYawPitchRoll(&qRotation, m_fYaw, m_fPitch, m_fRoll);
	//TMatrix matRotation;
	//ZeroMemory(&matRotation, sizeof(matRotation));
	//TMatrixRotationQuaternion(&matRotation, &qRotation);
	//m_matWorld = m_matWorld * matRotation;
	//m_fYaw = 0.0f;
	//m_fPitch = 0.0f;
	//m_fRoll = 0.0f;
	// 사원수를 행렬로 변환해주는 함수 ( 상호 변환 가능 )
	// 이는 오브젝트 행렬이므로 카메라 행렬로의 변환이 필요 ( 역행렬 )

	// 회전행렬을 만들었으므로 이동행렬을 추가
	UpdateVector();

	m_matWorld._41 = m_vPos.x;// D3DXVec3Dot(&m_vPos, &m_vSide);
	m_matWorld._42 = m_vPos.y;// D3DXVec3Dot(&m_vPos, &m_vUp);
	m_matWorld._43 = m_vPos.z;// D3DXVec3Dot(&m_vPos, &m_vLook);
	return true;
}

BSphere::BSphere()
{
	m_fRadius = 1;
	m_vCenter = TVector3(0, 0, 0);
	m_Slices = 10;
}


BSphere::~BSphere()
{
}
