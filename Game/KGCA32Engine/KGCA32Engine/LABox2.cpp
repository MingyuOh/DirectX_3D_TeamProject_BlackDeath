

//#include "stdafx.h"

#include "LABox2.h"


void LABox2::SetColor(float r, float g, float b, float alpha)
{
	for (int iCnt = 0; iCnt < (int)m_VertexList.size(); iCnt++)
	{
		m_VertexList[iCnt].Color = TVector4(r, g, b, alpha);
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PNCT_VECTOR) * m_VertexList.size();
	int isize = sizeof(PNCT_VECTOR);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = (void*)&m_VertexList.at(0);

	m_pCoreDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
}

HRESULT LABox2::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	//앞
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ -1.0f, 1.0f, -1.0f, 1.0f }, TVector4{0.0f, 0.0f, -1.0f, 1.0f }, TVector4{ 1.0f, 0.0f, 0.0f, 1.0f }, TVector2{ 0.0f, 0.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ 1.0f, 1.0f, -1.0f , 1.0f }, TVector4{ 0.0f, 0.0f, -1.0f, 1.0f }, TVector4{ 1.0f, 0.0f, 0.0f, 1.0f }, TVector2{ 1.0f, 0.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ 1.0f, -1.0f, -1.0f , 1.0f }, TVector4{ 0.0f, 0.0f, -1.0f, 1.0f }, TVector4{ 1.0f, 0.0f, 0.0f, 1.0f }, TVector2{ 1.0f, 1.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ -1.0f, -1.0f, -1.0f , 1.0f }, TVector4{ 0.0f, 0.0f, -1.0f, 1.0f }, TVector4{ 1.0f, 0.0f, 0.0f, 1.0f }, TVector2{ 0.0f, 1.0f }));

	//뒤
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ 1.0f, 1.0f, 1.0f , 1.0f }, TVector4{ 0.0f, 0.0f, 1.0f , 1.0f }, TVector4{ 0.0f, 1.0f, 0.0f, 1.0f }, TVector2{ 0.0f, 0.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ -1.0f, 1.0f, 1.0f , 1.0f }, TVector4{ 0.0f, 0.0f, 1.0f, 1.0f }, TVector4{ 0.0f, 1.0f, 0.0f, 1.0f }, TVector2{ 1.0f, 0.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ -1.0f, -1.0f, 1.0f , 1.0f }, TVector4{ 0.0f, 0.0f, 1.0f, 1.0f }, TVector4{ 0.0f, 1.0f, 0.0f, 1.0f }, TVector2{ 1.0f, 1.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ 1.0f, -1.0f, 1.0f , 1.0f }, TVector4{ 0.0f, 0.0f, 1.0f, 1.0f }, TVector4{ 0.0f, 1.0f, 0.0f, 1.0f }, TVector2{ 0.0f, 1.0f }));

	//오른쪽
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ 1.0f, 1.0f, -1.0f , 1.0f }, TVector4{ 1.0f, 0.0f, 0.0f, 1.0f }, TVector4{ 0.0f, 0.0f, 1.0f, 1.0f }, TVector2{ 0.0f, 0.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ 1.0f, 1.0f, 1.0f , 1.0f }, TVector4{ 1.0f, 0.0f, 0.0f , 1.0f }, TVector4{ 0.0f, 0.0f, 1.0f, 1.0f }, TVector2{ 1.0f, 0.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ 1.0f, -1.0f, 1.0f , 1.0f }, TVector4{ 1.0f, 0.0f, 0.0f , 1.0f }, TVector4{ 0.0f, 0.0f, 1.0f, 1.0f }, TVector2{ 1.0f, 1.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ 1.0f, -1.0f, -1.0f , 1.0f }, TVector4{ 1.0f, 0.0f, 0.0f , 1.0f }, TVector4{ 0.0f, 0.0f, 1.0f, 1.0f }, TVector2{ 0.0f, 1.0f }));

	//왼쪽
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ -1.0f, 1.0f, 1.0f , 1.0f }, TVector4{ -1.0f, 0.0f, 0.0f , 1.0f }, TVector4{ 1.0f, 1.0f, 0.0f, 1.0f }, TVector2{ 0.0f, 0.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ -1.0f, 1.0f, -1.0f , 1.0f }, TVector4{ -1.0f, 0.0f, 0.0f , 1.0f }, TVector4{ 1.0f, 1.0f, 0.0f, 1.0f }, TVector2{ 1.0f, 0.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ -1.0f, -1.0f, -1.0f , 1.0f }, TVector4{ -1.0f, 0.0f, 0.0f , 1.0f }, TVector4{ 1.0f, 1.0f, 0.0f, 1.0f }, TVector2{ 1.0f, 1.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ -1.0f, -1.0f, 1.0f , 1.0f }, TVector4{ -1.0f, 0.0f, 0.0f, 1.0f }, TVector4{ 1.0f, 1.0f, 0.0f, 1.0f }, TVector2{ 0.0f, 1.0f }));

	//위
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ -1.0f, 1.0f, 1.0f , 1.0f }, TVector4{ 0.0f, 1.0f, 0.0f , 1.0f }, TVector4{ 1.0f, 0.0f, 1.0f, 1.0f }, TVector2{ 0.0f, 0.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ 1.0f, 1.0f, 1.0f , 1.0f }, TVector4{ 0.0f, 1.0f, 0.0f , 1.0f }, TVector4{ 1.0f, 0.0f, 1.0f, 1.0f }, TVector2{ 1.0f, 0.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ 1.0f, 1.0f, -1.0f , 1.0f }, TVector4{ 0.0f, 1.0f, 0.0f, 1.0f }, TVector4{ 1.0f, 0.0f, 1.0f, 1.0f }, TVector2{ 1.0f, 1.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ -1.0f, 1.0f, -1.0f , 1.0f }, TVector4{ 0.0f, 1.0f, 0.0f , 1.0f }, TVector4{ 1.0f, 0.0f, 1.0f, 1.0f }, TVector2{ 0.0f, 1.0f }));

	//아래
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ -1.0f, -1.0f, -1.0f , 1.0f }, TVector4{ 0.0f, -1.0f, 0.0f, 1.0f }, TVector4{ 0.0f, 1.0f, 1.0f, 1.0f }, TVector2{ 0.0f, 0.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ 1.0f, -1.0f, -1.0f , 1.0f }, TVector4{ 0.0f, -1.0f, 0.0f, 1.0f }, TVector4{ 0.0f, 1.0f, 1.0f, 1.0f }, TVector2{ 1.0f, 0.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ 1.0f, -1.0f, 1.0f , 1.0f }, TVector4{ 0.0f, -1.0f, 0.0f , 1.0f }, TVector4{ 0.0f, 1.0f, 1.0f, 1.0f }, TVector2{ 1.0f, 1.0f }));
	m_VertexList.push_back(PNCT_VECTOR(TVector4{ -1.0f, -1.0f, 1.0f , 1.0f }, TVector4{ 0.0f, -1.0f, 0.0f , 1.0f }, TVector4{ 0.0f, 1.0f, 1.0f, 1.0f }, TVector2{ 0.0f, 1.0f }));

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PNCT_VECTOR) * m_VertexList.size();
	int isize = sizeof(PNCT_VECTOR);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = (void*)&m_VertexList.at(0);

	hr = m_pCoreDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	if (!SUCCEEDED(hr))
	{
		//ERROR
		return false;
	}

	//SpaceBox Set
	m_SpaceObject.CreateBox();

	m_SpaceObject.m_pSpaceBox->vMin.x = 999999.0f;
	m_SpaceObject.m_pSpaceBox->vMin.z = -999999.0f;
	m_SpaceObject.m_pSpaceBox->vMax.x = -999999.0f;
	m_SpaceObject.m_pSpaceBox->vMax.z = 999999.0f;

	for (int iCnt = 0; iCnt < (int)m_VertexList.size(); iCnt++)
	{
		//min
		if (m_VertexList[iCnt].Vertex.x < m_SpaceObject.m_pSpaceBox->vMin.x)
		{
			m_SpaceObject.m_pSpaceBox->vMin.x = m_VertexList[iCnt].Vertex.x;
		}
		if (m_VertexList[iCnt].Vertex.z > m_SpaceObject.m_pSpaceBox->vMin.z)
		{
			m_SpaceObject.m_pSpaceBox->vMin.z = m_VertexList[iCnt].Vertex.z;
		}
		//max
		if (m_VertexList[iCnt].Vertex.x > m_SpaceObject.m_pSpaceBox->vMax.x)
		{
			m_SpaceObject.m_pSpaceBox->vMax.x = m_VertexList[iCnt].Vertex.x;
		}
		if (m_VertexList[iCnt].Vertex.z < m_SpaceObject.m_pSpaceBox->vMax.z)
		{
			m_SpaceObject.m_pSpaceBox->vMax.z = m_VertexList[iCnt].Vertex.z;
		}
	}
	//vpos
	m_SpaceObject.m_pSpaceBox->vPos[0] = TVector3(m_SpaceObject.m_pSpaceBox->vMin.x, m_SpaceObject.m_pSpaceBox->vMin.y, m_SpaceObject.m_pSpaceBox->vMin.z);
	m_SpaceObject.m_pSpaceBox->vPos[1] = TVector3(m_SpaceObject.m_pSpaceBox->vMin.x, m_SpaceObject.m_pSpaceBox->vMax.y, m_SpaceObject.m_pSpaceBox->vMin.z);
	m_SpaceObject.m_pSpaceBox->vPos[2] = TVector3(m_SpaceObject.m_pSpaceBox->vMax.x, m_SpaceObject.m_pSpaceBox->vMax.y, m_SpaceObject.m_pSpaceBox->vMin.z);
	m_SpaceObject.m_pSpaceBox->vPos[3] = TVector3(m_SpaceObject.m_pSpaceBox->vMax.x, m_SpaceObject.m_pSpaceBox->vMin.y, m_SpaceObject.m_pSpaceBox->vMin.z);

	m_SpaceObject.m_pSpaceBox->vPos[4] = TVector3(m_SpaceObject.m_pSpaceBox->vMin.x, m_SpaceObject.m_pSpaceBox->vMin.y, m_SpaceObject.m_pSpaceBox->vMax.z);
	m_SpaceObject.m_pSpaceBox->vPos[5] = TVector3(m_SpaceObject.m_pSpaceBox->vMin.x, m_SpaceObject.m_pSpaceBox->vMax.y, m_SpaceObject.m_pSpaceBox->vMax.z);
	m_SpaceObject.m_pSpaceBox->vPos[6] = TVector3(m_SpaceObject.m_pSpaceBox->vMax.x, m_SpaceObject.m_pSpaceBox->vMax.y, m_SpaceObject.m_pSpaceBox->vMax.z);
	m_SpaceObject.m_pSpaceBox->vPos[7] = TVector3(m_SpaceObject.m_pSpaceBox->vMax.x, m_SpaceObject.m_pSpaceBox->vMin.y, m_SpaceObject.m_pSpaceBox->vMax.z);

	//center
	m_SpaceObject.m_pSpaceBox->vCenter = m_SpaceObject.m_pSpaceBox->vMax + m_SpaceObject.m_pSpaceBox->vMin;
	m_SpaceObject.m_pSpaceBox->vCenter /= 2.0f;

	return S_OK;
}

HRESULT LABox2::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

	m_IndexList =
	{
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23,
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = (void*)&m_IndexList.at(0);

	hr = m_pCoreDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
	if (!SUCCEEDED(hr))
	{
		//ERROR
		return false;
	}

	return S_OK;
}

HRESULT LABox2::Create(CrateObjectDest& ObjectDest)
{
	HRESULT hr = S_OK;

	m_ObjectDest = ObjectDest;

	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();

	LoadTextureResource();

	LoadShader();

	CreateLayout();

	if (ObjectDest.GiometryShaderName != NULL)
	{
		CreateGeometryWithStreamOut();
		CreateStreamOutBuffer(100000, &m_StreamTo);
		CreateStreamOutBuffer(100000, &m_DrawFrom);

		HandleEffects(m_pVertexBuffer);
		HandleEffects(m_DrawFrom);
	}

	return S_OK;
}

bool LABox2::Render()
{
	LAObject2::PrevRender();

	m_pCoreContext->VSSetShader(m_pVS, NULL, 0);
	m_pCoreContext->HSSetShader(NULL, NULL, 0);
	m_pCoreContext->DSSetShader(NULL, NULL, 0);
	m_pCoreContext->PSSetShader(m_pPS, NULL, 0);

	if (m_Texture.m_pTextureSRV[0] != NULL)
		ApplyTexture();

	UINT stride = sizeof(PNCT_VECTOR);
	UINT offset = 0;

	if (m_ObjectDest.GiometryShaderName != NULL)
	{
		m_pCoreContext->GSSetShader(NULL, NULL, 0);
		m_pCoreContext->IASetVertexBuffers(0, 1, &m_DrawFrom, &stride, &offset);
		m_pCoreContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
	}
	else
	{
		m_pCoreContext->GSSetShader(NULL, NULL, 0);
		m_pCoreContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
		m_pCoreContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	}

	m_pCoreContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pCoreContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferData);
	m_pCoreContext->IASetInputLayout(m_pVertexLayout);

	m_ObjectDataCB.vLightPos.x = m_ObjectWorld._41;
	m_ObjectDataCB.vLightPos.y = m_ObjectWorld._42;
	m_ObjectDataCB.vLightPos.z = m_ObjectWorld._43;
	m_ObjectDataCB.vLIghtDirection.x = 0.0f;
	m_ObjectDataCB.vLIghtDirection.y = -1.0f;
	m_ObjectDataCB.vLIghtDirection.z = 0.0f;

	D3DXMatrixTranspose(&m_ObjectCB.g_matWorld, &m_ObjectWorld);
	D3DXMatrixTranspose(&m_ObjectCB.g_matView, &m_ObjectView);
	D3DXMatrixTranspose(&m_ObjectCB.g_matProj, &m_ObjectProjection);

	m_ObjectCB.g_matWorldInverse = m_ObjectWorld;
	m_pCoreContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_ObjectCB, 0, 0);
	m_pCoreContext->UpdateSubresource(m_pConstantBufferData, 0, NULL, &m_ObjectDataCB, 0, 0);
	m_pCoreContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (m_ObjectDest.GiometryShaderName != NULL)
	{
		m_pCoreContext->DrawAuto();
	}
	else
	{
		m_pCoreContext->DrawIndexed(m_IndexList.size(), 0, 0);
	}


	return true;
}


LABox2::LABox2()
{
}


LABox2::~LABox2()
{
}
