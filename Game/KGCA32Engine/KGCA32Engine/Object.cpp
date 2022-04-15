#include "Object.h"

Object::Object() :m_iTextureCount(0) {}
Object::~Object() {}

HRESULT	Object::Create(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext,
					   TCHAR* TextureName, TCHAR* vertexShaderName, TCHAR* PixelShaderName, TCHAR* geometryShaderName)
{
	HRESULT hr = S_OK;

	// 디바이스 설정 및 쉐이더 텍스쳐 이름 저장
	m_pd3dDevice = pd3dDevice;
	m_pContext = pImmediateContext;

	if (FAILED(CreateVertexBuffer()))
	{
		MessageBox(0, _T("CreateVertexBuffer  실패"), _T("Fatal error"), MB_OK);
		return hr;
	}

	if (FAILED(CreateIndexBuffer()))
	{
		MessageBox(0, _T("CreateIndexBuffer  실패"), _T("Fatal error"), MB_OK);
		return hr;
	}

	if (FAILED(CreateConstantBuffer()))
	{
		MessageBox(0, _T("CreateConstantBuffer  실패"), _T("Fatal error"), MB_OK);
		return hr;
	}

	if (FAILED(LoadShaderAndInputLayout(vertexShaderName, PixelShaderName, geometryShaderName)))
	{
		MessageBox(0, _T("LoadShaderAndInputLayout  실패"), _T("Fatal error"), MB_OK);
		return hr;
	}

	if (FAILED(LoadTexture(m_pd3dDevice, TextureName)))
	{
		//MessageBox(0, _T("Texture Load 실패"), _T("Fatal error"), MB_OK);
		return hr;
	}

	return hr;
}

HRESULT	Object::CreateVertexBuffer()
{
	return S_OK;
}

HRESULT	Object::CreateIndexBuffer()
{
	return S_OK;
}

HRESULT	Object::CreateConstantBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
	cbDesc.ByteWidth = sizeof(VSConstantB);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	hr = m_pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pConstantBuffer);
	return hr;
}

HRESULT	Object::LoadShaderAndInputLayout(TCHAR* vertexShaderName, TCHAR* PixelShaderName, TCHAR* geometryShaderName)
{
	HRESULT hr = S_OK;

	ID3DBlob* pVSBuf = NULL;
	D3DX11CompileFromFile(vertexShaderName, NULL, NULL, "VS", "vs_5_0", 0, NULL, NULL, &pVSBuf, NULL, NULL);
	m_pd3dDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_pVS);

	// Compile the PS from the file
	ID3DBlob* pPSBuf = NULL;
	D3DX11CompileFromFile(PixelShaderName, NULL, NULL, "PS", "ps_5_0", 0, NULL, NULL, &pPSBuf, NULL, NULL);
	m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS);

	//// Compile the GS from the file
	/*ID3DBlob* pGSBuf = NULL;
	if (geometryShaderName != NULL)
	{
		D3DX11CompileFromFile(geometryShaderName, NULL, NULL, "GS", "gs_5_0", 0, NULL, NULL, &pGSBuf, NULL, NULL);
		m_pd3dDevice->CreateGeometryShader(pGSBuf->GetBufferPointer(), pGSBuf->GetBufferSize(), NULL, &m_pGS);
	}*/

	// Create our vertex input layout
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" ,   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	m_pd3dDevice->CreateInputLayout(layout, numElements, pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), &m_pVertexLayout);

	S_RELEASE(pVSBuf);
	S_RELEASE(pPSBuf);
	return hr;
}

HRESULT	Object::LoadTexture(ID3D11Device* pDevice, TCHAR* strFilePath, int iIndex)
{
	HRESULT hr = S_OK;

	if (FAILED(hr = D3DX11CreateShaderResourceViewFromFile(pDevice, strFilePath, NULL, NULL, m_pTextureSRV[iIndex].GetAddressOf(), NULL)))
	{
		return hr;
	}
	return hr;
}

bool Object::ApplyTexture(ID3D11DeviceContext* pImmediateContext, int iIndex)
{

	if (m_pTextureSRV[iIndex].Get() == NULL)
	{
		return false;
	}
	pImmediateContext->PSSetShaderResources(0, 1, m_pTextureSRV[iIndex].GetAddressOf());
	return true;
}

bool Object::SetMatrix(D3DXMATRIX* matWorld, D3DXMATRIX* matView, D3DXMATRIX* matProj)
{
	if (matWorld != NULL)
	{
		m_matWorld = *matWorld;
	}
	
	if (matView != NULL)
	{
		m_matView = *matView;
	}

	if (matProj != NULL)
	{
		m_matProj = *matProj;
	}

	D3DXMatrixTranspose(&m_cbData.m_matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_cbData.m_matView, &m_matView);
	D3DXMatrixTranspose(&m_cbData.m_matProj, &m_matProj);
	return true;
}

void Object::SetPosition(D3DXVECTOR3 vPos)
{
	m_matWorld._41 = vPos.x;
	m_matWorld._42 = vPos.y;
	m_matWorld._43 = vPos.z;
	//m_vPos = vPos;
}

bool Object::Init()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	// 리소스 뷰 / 텍스처 이름 기본 1개 생성
	m_pTextureSRV.resize(1);
	//m_pTextureName.resize(1);
	return true;
}

bool Object::Frame()
{
	m_vLook.x = m_matWorld._13;
	m_vLook.y = m_matWorld._23;
	m_vLook.z = m_matWorld._33;

	return true;
}

bool Object::Render()
{
	m_pContext->IASetInputLayout(m_pVertexLayout);
	m_pContext->VSSetShader(m_pVS, NULL, 0);
	m_pContext->PSSetShader(m_pPS, NULL, 0);
	if(m_pGS != NULL)
		m_pContext->GSSetShader(m_pGS, NULL, 0);

	UINT stride = sizeof(PNCT_Vertex);
	UINT offset = 0;

	// 상수버퍼 갱신
	ApplyTexture(m_pContext);
	m_pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
	m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	/*if(m_pGS != NULL)
		m_pContext->GSSetConstantBuffers(0, 1, &m_pConstantBuffer);*/
	m_pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pContext->DrawIndexed(m_IndexList.size() , 0, 0);
	return true;
}

bool Object::Release()
{
	S_RELEASE(m_pVertexLayout);		// 정점레이아웃 소멸
	S_RELEASE(m_pVertexBuffer);		// 정점버퍼 소멸
	S_RELEASE(m_pIndexBuffer);		// 인덱스버퍼 소멸
	S_RELEASE(m_pConstantBuffer);	// 상수버퍼 소멸
	S_RELEASE(m_pd3dDevice);			// 오브젝트 디바이스 소멸
	S_RELEASE(m_pContext);			// 오브젝트 컨텍스트 소멸
	S_RELEASE(m_pVS);				// 정점쉐이더 소멸
	S_RELEASE(m_pPS);				// 픽셀쉐이더 소멸
	// S_RELEASE(m_pGS);				// 기하쉐이더 소멸
	DeleteVector(m_szTextureName);
	DeleteVector(m_pTextureSRV);
	return true;
}


