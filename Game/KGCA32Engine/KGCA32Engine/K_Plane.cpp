#include "K_Plane.h"

bool K_Plane::CreateVertexBuffer()
{
	int iVertexList = sizeof(PNCT_VERTEX);

	m_VertexList.resize(4);
	m_VertexList[0] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, 0.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(TVector3(1.0f, 1.0f, 0.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(TVector3(1.0f, -1.0f, 0.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, 0.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(PNCT_VERTEX) * m_VertexList.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = (void*)&m_VertexList.at(0);
	m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);

	return true;
}
bool K_Plane::CreateIndexBuffer()
{
	m_IndexList.resize(6);
	m_IndexList[0] = 0;
	m_IndexList[1] = 1;
	m_IndexList[2] = 2;
	m_IndexList[3] = 0;
	m_IndexList[4] = 2;
	m_IndexList[5] = 3;
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(DWORD) * m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = (void*)&m_IndexList.at(0);
	m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);

	return true;
}

bool K_Plane::Render(int Num)
{
	m_pContext->VSSetShader(m_pVS, 0, 0);
	m_pContext->PSSetShader(m_pPS, 0, 0);
	m_pContext->IASetInputLayout(m_pIL);
	m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	UINT stride = sizeof(PNCT_VERTEX);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &stride, &offset);
	m_pContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pContext->PSSetShaderResources(0, 1, m_Texture.m_pTextureSRV[Num].GetAddressOf());

	m_pContext->IASetIndexBuffer(m_pIndexBuffer,
		DXGI_FORMAT_R32_UINT, 0);

	m_pContext->UpdateSubresource(
		m_pConstantBuffer, 0, NULL,
		&m_cbData, 0, 0);
	m_pContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}
K_Plane::K_Plane()
{

}


K_Plane::~K_Plane()
{

}
