#include "K_Object.h"

void K_Object::SetMatrix(TMatrix* matWorld,
	TMatrix* matView, TMatrix* matProj)
{
	if (matWorld != NULL)
		m_matWorld = *matWorld;
	if (matView != NULL)
		m_matView = *matView;
	if (matProj != NULL)
		m_matProj = *matProj;

	D3DXMatrixTranspose(&m_cbData.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_cbData.matProj, &m_matProj);
}
bool K_Object::MultiTextureLoad(TCHAR* pName)
{
	m_TextArray.resize(m_TextArray.size() + 1);
	m_TextArray[m_TextArray.size() - 1].assign(pName);
	return true;
}
bool K_Object::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	TCHAR* vs,
	TCHAR* ps,
	TCHAR* pTexName)
{
	m_pd3dDevice = pDevice;
	m_pContext = pContext;
	CreateConstanceBuffer();
	LoadShader(vs, ps);
	//LoadTexture(pTexName);

	CreateVertexBuffer();
	CreateIndexBuffer();

	CreateInputLayout();
	return true;
}
bool K_Object::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT iSize = sizeof(layout) / sizeof(layout[0]);
	m_pd3dDevice->CreateInputLayout(layout, iSize,
		m_pVSBuffer->GetBufferPointer(),
		m_pVSBuffer->GetBufferSize(), &m_pIL);

	S_RELEASE(m_pVSBuffer);
	return true;
}

bool K_Object::CreateConstanceBuffer()
{
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(CB);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	//D3D11_SUBRESOURCE_DATA sd;
	//sd.pSysMem = &cbConstance;
	m_pd3dDevice->CreateBuffer(&bd, NULL,
		&m_pConstantBuffer);

	return true;
}
bool K_Object::CreateIndexBuffer()
{
	return true;
}
bool K_Object::CreateVertexBuffer()
{
	return true;
}

bool K_Object::LoadShader(TCHAR* vs, TCHAR* ps)
{
	//¹úÅØ½º ½¦ÀÌ´õ
	D3DCompileFromFile(vs, NULL, NULL, "VS", "vs_5_0", NULL, NULL, &m_pVSBuffer, NULL);
	//ÇÈ¼¿ ½¦ÀÌ´õ
	ID3DBlob*   pPSBuffer = 0;
	D3DCompileFromFile(ps, NULL, NULL, "PS", "ps_5_0", NULL, NULL, &pPSBuffer, NULL);

	ID3DBlob*   pError = 0;

	m_pd3dDevice->CreateVertexShader(
		m_pVSBuffer->GetBufferPointer(),
		m_pVSBuffer->GetBufferSize(),
		NULL, &m_pVS);
	m_pd3dDevice->CreatePixelShader(
		pPSBuffer->GetBufferPointer(),
		pPSBuffer->GetBufferSize(),
		NULL, &m_pPS);

	//S_RELEASE(pPSBuffer);
	return true;
}

bool K_Object::LoadTexture()
{
	m_Texture.m_pTextureSRV.resize(m_TextArray.size());
	string ext;
	//½¦ÀÌ´õ¸®¼Ò½ººä »ý¼º
	for (int iCnt = 0; iCnt < m_Texture.m_pTextureSRV.size(); iCnt++)
	{
		ext = GetFileExtension((TCHAR*)m_TextArray[iCnt].c_str());
		m_Texture.LoadTexture(m_TextArray[iCnt], mtw(ext));
	}
	return true;
}

bool K_Object::Init()
{
	//´ÜÀ§Çà·Ä·Î
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	return true;
}

bool K_Object::Frame()
{
	return true;
}

bool K_Object::PreRender()
{
	UINT stride = sizeof(PNCT_VERTEX);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0, 1,
		&m_pVertexBuffer, &stride, &offset);
	m_pContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pContext->PSSetShaderResources(0, 1, m_Texture.m_pTextureSRV[0].GetAddressOf());
	return true;
}

bool K_Object::Render()
{
	m_pContext->VSSetShader(m_pVS, 0, 0);
	m_pContext->PSSetShader(m_pPS, 0, 0);
	m_pContext->IASetInputLayout(m_pIL);
	m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	PreRender();

	m_pContext->IASetIndexBuffer(m_pIndexBuffer,
		DXGI_FORMAT_R32_UINT, 0);

	
	//TMatrixTranspose(&m_cbData.matWorld, &m_matWorld);
	//TMatrixTranspose(&m_cbData.matView , &m_matView);
	//TMatrixTranspose(&m_cbData.matProj ,  &m_matProj);

	/*m_cbData.matWVP =
		m_matWorld * m_matView * m_matProj;*/
	//m_cbData.fTime = timeGetTime()* 0.001f;

	m_pContext->UpdateSubresource(
		m_pConstantBuffer, 0, NULL,
		&m_cbData, 0, 0);

	m_pContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}

bool K_Object::Release()
{
	S_RELEASE(m_pConstantBuffer);
	S_RELEASE(m_pVertexBuffer);
	S_RELEASE(m_pIndexBuffer);
	//S_RELEASE(m_pCB);
	S_RELEASE(m_pVS);
	S_RELEASE(m_pPS);
//	S_RELEASE(m_pSRV);
	S_RELEASE(m_pIL);
	//S_RELEASE(m_pVertexLayout);
	S_RELEASE(m_pVSBuffer);
	return true;
}
K_Object::K_Object()
{
}  


K_Object::~K_Object()
{
}
