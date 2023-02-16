#include "BPlane.h"

bool BPlane::CreatePlane(TVector3 v0, TVector3 v1, TVector3 v2)
{
	TVector3 vEdge0 = v1 - v0;
	TVector3 vEdge1 = v2 - v0;
	TVector3 vNormal;
	D3DXVec3Cross(&vNormal, &vEdge0, &vEdge1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	
	fA = vNormal.x; fB = vNormal.y; fC = vNormal.z; fD = -(fA* v0.x + fB*v0.y + fC*v0.z);
	return true;
}

bool BPlane::CreatePlane(TVector3 vNormal, TVector3 v0)
{
	D3DXVec3Normalize(&vNormal, &vNormal);

	fA = vNormal.x; fB = vNormal.y; fC = vNormal.z; fD = -(fA* v0.x + fB*v0.y + fC*v0.z);
	return true;
}

HRESULT BPlane::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	m_VertexList.resize(4);

	m_tBox.vMin = TVector3(-1.0f, -1.0f, 0.0f);
	m_tBox.vMax = TVector3(1.0f, 1.0f, 0.0f);
	m_tBox.vCenter = TVector3(0.0f, 0.0f, 0.0f);
	m_tBox.vAxis[0] = TVector3(1.0f, 0.0f, 0.0f);
	m_tBox.vAxis[1] = TVector3(0.0f, 1.0f, 0.0f);
	m_tBox.fExtent[0] = 1.0f;
	m_tBox.fExtent[1] = 1.0f;

	m_VertexList[0].Position = TVector3(-1.0f, 1.0f, 0.0f);
	m_VertexList[1].Position = TVector3(1.0f, 1.0f, 0.0f);
	m_VertexList[2].Position = TVector3(1.0f, -1.0f, 0.0f);
	m_VertexList[3].Position = TVector3(-1.0f, -1.0f, 0.0f);

	m_VertexList[0].Color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[1].Color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[2].Color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_VertexList[3].Color = TVector4(1.0f, 1.0f, 1.0f, 1.0f);

	m_VertexList[0].Texture = TVector2(0.0f, 0.0f);
	m_VertexList[1].Texture = TVector2(1.0f, 0.0f);
	m_VertexList[2].Texture = TVector2(1.0f, 1.0f);
	m_VertexList[3].Texture = TVector2(0.0f, 1.0f);


	m_VertexCnt = m_VertexList.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						// [�޸� �Ҵ� ��ġ] GPU �޸𸮿� �Ҵ��ض�
	bd.ByteWidth = sizeof(PNCT_Vertex) * m_VertexCnt;	// [�޸� �Ҵ� ũ��] �� X���� �Ҵ� �� ���̱� ������ *X ( ���� �þ�� ���� �� ���̴� )
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// [�޸� ��� �뵵] ���ؽ� ���ۿ����� ����ϰڴ�.
	bd.CPUAccessFlags = 0;								// CPU�� ���ʹ� �׼��� ���� �ʰڴ�.
	bd.MiscFlags = 0;									// �߰� ���� ����

	D3D11_SUBRESOURCE_DATA InitData;			// �ҽ� ���ҽ� �ʱ�ȭ�� �ʿ��� ������ ����
	InitData.pSysMem = (void*)&m_VertexList.at(0);				// �ش� ���ؽ��� �ҽ��� ����ϰڴ�.
	S_RETURN(m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer));
	return hr;
}
HRESULT BPlane::CreateIndexBuffer()		// �ε��� ���� ����
{
	HRESULT hr = S_OK;

	/* ���� ���ۿ� ����ִ� �ε����� �д� ������ ����ü */
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; m_IndexList[iIndex++] = 1; m_IndexList[iIndex++] = 2;
	m_IndexList[iIndex++] = 0; m_IndexList[iIndex++] = 2; m_IndexList[iIndex++] = 3;

	UINT iNumIndex = m_IndexList.size();

	// �ε��� ���� ����
	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = iNumIndex * sizeof(DWORD);
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // �ε��� ���۷� ����ϰڴٴ� ����
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ibInitData;
	ZeroMemory(&ibInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibInitData.pSysMem = (void*)&m_IndexList.at(0);
	S_RETURN(m_pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIndexBuffer));

	return hr;
}

void BPlane::UpdateVertexMinMax()
{
	m_tBox.vCenter.x = m_matWorld._41;
	m_tBox.vCenter.y = m_matWorld._42;

	D3DXVec3TransformCoord(&m_tBox.vAxis[0], &m_tBox.vAxis[0], &m_matWorld);
	D3DXVec3TransformCoord(&m_tBox.vAxis[1], &m_tBox.vAxis[1], &m_matWorld);
	D3DXVec3Normalize(&m_tBox.vAxis[0], &m_tBox.vAxis[0]);
	D3DXVec3Normalize(&m_tBox.vAxis[1], &m_tBox.vAxis[1]);

	TVector3 vMax, vMin, vHalf;
	D3DXVec3TransformCoord(&vMax, &m_tBox.vMax, &m_matWorld);
	vHalf = vMax - m_tBox.vCenter;

	m_tBox.fExtent[0] = D3DXVec3Dot(&m_tBox.vAxis[0], &vHalf);
	m_tBox.fExtent[1] = D3DXVec3Dot(&m_tBox.vAxis[1], &vHalf);

	TVector3 vVertex[8];
	vVertex[0] = TVector3(m_tBox.vMin.x, m_tBox.vMax.y, 0);
	vVertex[1] = TVector3(m_tBox.vMax.x, m_tBox.vMax.y, 0);
	vVertex[2] = TVector3(m_tBox.vMax.x, m_tBox.vMin.y, 0);
	vVertex[3] = TVector3(m_tBox.vMin.x, m_tBox.vMin.y, 0);

	vMin = TVector3(100000.0f, 100000.0f, 0.0f);
	vMax = TVector3(-100000.0f, -100000.0f, 0.0f);

	for (int iVer = 0; iVer < 4; iVer++)
	{
		D3DXVec3TransformCoord(&vVertex[iVer], &vVertex[iVer], &m_matWorld);
		if (vVertex[iVer].x > vMax.x) vMax.x = vVertex[iVer].x;
		if (vVertex[iVer].y > vMax.y) vMax.y = vVertex[iVer].y;

		if (vVertex[iVer].x < vMin.x) vMin.x = vVertex[iVer].x;
		if (vVertex[iVer].y < vMin.y) vMin.y = vVertex[iVer].y;
	}

	m_tBox.vMin = vMin;
	m_tBox.vMax = vMax;
}

BPlane::BPlane()
{
}


BPlane::~BPlane()
{
}