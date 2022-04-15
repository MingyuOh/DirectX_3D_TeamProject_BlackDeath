
//#include "stdafx.h"


#include "LAObject2.h"

#include "CoreFuncLink.h"
#include "CoreInputLink.h"


HRESULT LAObject2::Create(CrateObjectDest& ObjectDest)
{
	HRESULT hr = S_OK;

	m_ObjectDest = ObjectDest;

	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();

	LoadTextureResource();

	//LoadShader();

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


void LAObject2::CreateDrawLine()
{
#ifdef TOOL_MODE

	if (m_pDrawLine == NULL)
	{
		m_pDrawLine = new LAVertexDirLine;

		m_pDrawLine->Init(m_pCoreDevice, m_pCoreContext);

		CrateObjectDest VertexLineObjectDest;

		VertexLineObjectDest.vertexShaderName = L"../data/shader/LineVertexShader.vsh";
		VertexLineObjectDest.vertexFuntionNmae = NULL;
		VertexLineObjectDest.PixelShaderName = L"../data/shader/LineVertexShader.vsh";
		VertexLineObjectDest.PixelFuntionNmae = NULL;
		VertexLineObjectDest.GiometryShaderName = NULL;
		VertexLineObjectDest.GiometryFuntionNmae = NULL;
		VertexLineObjectDest.pTextureName = NULL;

		m_pDrawLine->Create(VertexLineObjectDest);
	}

#else
#endif
}


void LAObject2::DrawLine(std::vector<PNCT5_VERTEX>& rVertexList, std::vector<DWORD>& riIndexList, int iDrawIndexListMaxSize, TMatrix matWorld)
{
	if ((int)riIndexList.size() < iDrawIndexListMaxSize)
		return;

	for (int iCnt = 0; iCnt < iDrawIndexListMaxSize; iCnt++)
	{
		TVector3 vVertex = TVector3(rVertexList[riIndexList[iCnt]].Vertex.x, rVertexList[riIndexList[iCnt]].Vertex.y, rVertexList[riIndexList[iCnt]].Vertex.z);
		TVector3 vNormal = TVector3(rVertexList[riIndexList[iCnt]].Nomal.x, rVertexList[riIndexList[iCnt]].Nomal.y, rVertexList[riIndexList[iCnt]].Nomal.z);
		D3DXVec3Normalize(&vNormal, &vNormal);
		TVector3 vEndNormal = vVertex;

		vEndNormal.x += vNormal.x * 3.0f;
		vEndNormal.y += vNormal.y * 3.0f;
		vEndNormal.z += vNormal.z * 3.0f;


		D3DXVec3TransformCoord(&vVertex, &vVertex, &matWorld);
		D3DXVec3TransformCoord(&vEndNormal, &vEndNormal, &matWorld);

#ifdef TOOL_MODE

		m_pDrawLine->DrawLine(TVector4(vVertex, 1.0f), TVector4(vEndNormal, 1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f));

#else
#endif
		
	}
}


void LAObject2::Translation(float x, float y, float z)
{
	D3DXMatrixTranslation(&m_Move, x, y, z);

	m_ObjectWorld._41 = m_Move._41;
	m_ObjectWorld._42 = m_Move._42;
	m_ObjectWorld._43 = m_Move._43;
}

void LAObject2::RotationX(float fangle)
{
	D3DXMatrixRotationX(&m_Rotate, fangle);

	m_ObjectWorld = m_ObjectWorld *  m_Rotate;
}

void LAObject2::RotationY(float fangle)
{
	D3DXMatrixRotationY(&m_Rotate, fangle);

	m_ObjectWorld = m_ObjectWorld * m_Rotate;
}

void LAObject2::RotationZ(float fangle)
{
	D3DXMatrixRotationZ(&m_Rotate, fangle);

	m_ObjectWorld = m_ObjectWorld *  m_Rotate;
}

void LAObject2::Scaling(float x, float y, float z)
{
	D3DXMatrixScaling(&m_Scale, x, y, z);

	/*switch ((int)m_SpaceObject.m_CurrentSpaceType)
	{
		case (int)E_SpaceType::Sphere:
		{
			m_SpaceObject.m_pSpaceSpher->fRadius *= x;
		}break;
	}*/

	m_ObjectWorld = m_Scale;
}

HRESULT LAObject2::DeleteResource()
{
	HRESULT hr = S_OK;

	return hr;
}
HRESULT LAObject2::CreateResource()
{
	HRESULT hr = S_OK;

	return hr;
}



HRESULT LAObject2::CreateGeometryWithStreamOut()
{
	HRESULT hr = S_OK;
	//================================================================================
	//	** 스트림 출력 오브젝터 생성 ** 
	//================================================================================
	D3D11_SO_DECLARATION_ENTRY SDEDest[] =
	{
		{ 0, "SV_POSITION", 0, 0, 4, 0 },
		{ 0, "NORMAL", 0, 0, 4, 0 },
		{ 0, "COLOR", 0, 0, 4, 0 },
		{ 0, "TEXCOORD", 0, 0, 2, 0 }
	};

	UINT stride = 14 * sizeof(float);

	hr = m_pCoreDevice->CreateGeometryShaderWithStreamOutput(
		(void*)pGSBuf->GetBufferPointer(),
		pGSBuf->GetBufferSize(),
		SDEDest,
		ARRAYSIZE(SDEDest),
		&stride,
		ARRAYSIZE(SDEDest),
		0,
		NULL,
		&m_pSO
	);
	if (FAILED(hr))
	{
		ErrorQuit(L"Error CreateGeometryShaderWithStreamOutput");
		return false;
	}


	return hr;
}

bool LAObject2::HandleEffects(ID3D11Buffer* pBuffer)
{
	m_pCoreContext->VSSetShader(m_pVS, NULL, 0);
	m_pCoreContext->HSSetShader(NULL, NULL, 0);
	m_pCoreContext->DSSetShader(NULL, NULL, 0);
	m_pCoreContext->GSSetShader(m_pSO, NULL, 0);
	m_pCoreContext->PSSetShader(NULL, NULL, 0);
	m_pCoreContext->IASetInputLayout(m_pVertexLayout);

	UINT stride = sizeof(PNCT_VECTOR);
	UINT offset[1] = { 0 };
	ID3D11Buffer* pVB[1] = { pBuffer };

	m_pCoreContext->SOSetTargets(1, &m_StreamTo, offset);
	m_pCoreContext->IASetVertexBuffers(0, 1, pVB, &stride, offset);
	m_pCoreContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pCoreContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//if (INSTANCE_CUR_STATE.GetDssCurState() != E_DSS::DSSEnable)
	//{
	//	m_pCoreContext->OMSetDepthStencilState(INSTANCE_CUR_STATE.SetDssState((int)E_DSS::DSSEnable), 0x00);
	//}

	ApplyTexture();

	if (pBuffer == m_pVertexBuffer)
	{
		m_pCoreContext->DrawIndexed(m_IndexList.size(), 0, 0);
	}
	else
	{
		m_pCoreContext->DrawAuto();
	}

	ID3D11Buffer* pTemp = m_StreamTo;
	m_StreamTo = m_DrawFrom;
	m_DrawFrom = pTemp;

	pVB[0] = NULL;

	m_pCoreContext->SOSetTargets(0, pVB, offset);
	
	return true;
}

HRESULT LAObject2::CreateStreamOutBuffer(int iBufferSize, ID3D11Buffer** pTargetBuffer)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc =
	{
		(UINT)iBufferSize,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT,
		0,
		0
	};

	hr = m_pCoreDevice->CreateBuffer(&bufferDesc, NULL, pTargetBuffer);
	if (FAILED(hr))
	{
		ErrorQuit(L"Error CreateStreamOutBuffer");
		return hr;
	}

	return hr;
}

HRESULT LAObject2::CreateConstantBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	//Matrix
	cbDesc.ByteWidth = sizeof(VERTEX_CB);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	hr = m_pCoreDevice->CreateBuffer(&cbDesc, NULL, &m_pConstantBuffer);
	if (!SUCCEEDED(hr))
	{
		return false;
	}

	//그외 데이터
	cbDesc.ByteWidth = sizeof(DataCB);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	hr = m_pCoreDevice->CreateBuffer(&cbDesc, NULL, &m_pConstantBufferData);
	if (!SUCCEEDED(hr))
	{
		return false;
	}
	return S_OK;
}


HRESULT LAObject2::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

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

	return S_OK;
}

HRESULT LAObject2::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

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


HRESULT LAObject2::LoadTextureResource()
{
	HRESULT hr = S_OK;

	if (m_ObjectDest.pTextureName == NULL)
		return S_FALSE;

	string ext = GetFileExtension(m_ObjectDest.pTextureName);
	m_Texture.LoadTexture(m_ObjectDest.pTextureName, mtw(ext));

	return hr;
}

HRESULT LAObject2::ApplyTexture()
{
	HRESULT hr = S_OK;

	m_pCoreContext->PSSetShaderResources(0, 1, &m_Texture.m_pTextureSRV[0]);
	
	return S_OK;
}


HRESULT LAObject2::LoadShader()
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	//DWORD dwShaderFlags = D3DCOMPILE_DEBUG;

	//CreateVertexShader
	if (m_ObjectDest.vertexFuntionNmae == NULL)
	{
		hr =  D3DCompileFromFile(m_ObjectDest.vertexShaderName, NULL, NULL, "VS", "vs_5_0", NULL, NULL, &pVSBuf, NULL);
	}
	else
	{
		hr = D3DCompileFromFile(m_ObjectDest.vertexShaderName, NULL, NULL, m_ObjectDest.vertexFuntionNmae, "vs_5_0", NULL, NULL, &pVSBuf, NULL);
	}
	m_pCoreDevice->CreateVertexShader((DWORD*)pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_pVS);

	//CreatePixelShader
	if (m_ObjectDest.PixelFuntionNmae == NULL) 
	{
		if (m_Texture.m_pTextureSRV[0] == NULL)
		{
			hr = D3DCompileFromFile(m_ObjectDest.PixelShaderName, NULL, NULL, "PS", "ps_5_0", NULL, NULL, &pPSBuf, NULL);
		}
		else
			hr = D3DCompileFromFile(m_ObjectDest.PixelShaderName, NULL, NULL, "TPS", "ps_5_0", NULL, NULL, &pPSBuf, NULL);
	}
	else
	{
		hr = D3DCompileFromFile(m_ObjectDest.PixelShaderName, NULL, NULL, m_ObjectDest.PixelFuntionNmae, "ps_5_0", NULL, NULL, &pPSBuf, NULL);
	}
	m_pCoreDevice->CreatePixelShader((DWORD*)pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pPS);
		
	//CreateGiometryShader
	if (m_ObjectDest.GiometryShaderName != NULL)
	{
		if (m_ObjectDest.GiometryFuntionNmae == NULL)
		{
			//CKECK_RETURN(GiometryShader Compile Error, D3DCompileFromFile(m_ObjectDest.GiometryShaderName, NULL, NULL, "GS", "gs_5_0", dwShaderFlags, NULL, NULL, &pGSBuf, &g_DebufBuffer, NULL));
			hr = D3DCompileFromFile(m_ObjectDest.GiometryShaderName, NULL, NULL, "GS", "gs_5_0", NULL, NULL, &pGSBuf, NULL);
		}
		else
		{
			hr = D3DCompileFromFile(m_ObjectDest.GiometryShaderName, NULL, NULL, m_ObjectDest.GiometryFuntionNmae, "gs_5_0", NULL, NULL, &pGSBuf, NULL);
		}
			
		m_pCoreDevice->CreateGeometryShader((DWORD*)pGSBuf->GetBufferPointer(), pGSBuf->GetBufferSize(), NULL, &m_pGS);
	}


	
	return S_OK;
}

HRESULT LAObject2::CreateLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA , 0 }
	};
	UINT iSize = sizeof(layout) / sizeof(layout[0]);

	if (FAILED(m_pCoreDevice->CreateInputLayout(layout, iSize, pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), &m_pVertexLayout)))
	{
		ErrorQuit(L"Error m_pd3dDevice->CreateInputLayout");
	}

	return S_OK;
}


void LAObject2::SetMatrix(TMatrix* pWorldMatrix, TMatrix* pViewMatrix, TMatrix* pProjectionMatrix)
{
	if (pWorldMatrix != NULL)
	{
		m_ObjectWorld = *pWorldMatrix;

		switch ((int)m_SpaceObject.m_CurrentSpaceType)
		{
		case (int)E_SpaceType::Sphere:
		{
			m_SpaceObject.m_pSpaceSpher->vCenter.x = pWorldMatrix->_41;
			m_SpaceObject.m_pSpaceSpher->vCenter.y = pWorldMatrix->_42;
			m_SpaceObject.m_pSpaceSpher->vCenter.z = pWorldMatrix->_43;
			m_SpaceObject.m_pSpaceSpher->fRadius = pWorldMatrix->_11;
		}break;

		default:
		{
			break;
		}
		}

	}

	if (pViewMatrix != NULL)
	{
		m_ObjectView = *pViewMatrix;
	}

	if (pProjectionMatrix != NULL)
	{
		m_ObjectProjection = *pProjectionMatrix;
	}


#ifdef TOOL_MODE
	/*============================= DrawLine 사용시 ===============================*/
	if (m_pDrawLine)
	{
		m_pDrawLine->SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);
	}

#else
#endif
}

void LAObject2::SetPosition(TVector3 vPos)
{
	m_ObjectWorld._41 = vPos.x;
	m_ObjectWorld._42 = vPos.y;
	m_ObjectWorld._43 = vPos.z;

	switch ((int)m_SpaceObject.m_CurrentSpaceType)
	{
	case (int)E_SpaceType::Sphere:
	{
		m_SpaceObject.m_pSpaceSpher->vCenter.x = vPos.x;
		m_SpaceObject.m_pSpaceSpher->vCenter.y = vPos.y;
		m_SpaceObject.m_pSpaceSpher->vCenter.z = vPos.z;
	}break;
	}
}

void LAObject2::SetNormalMapping(bool bCheckNormal)
{
	m_bIsNormalMap = bCheckNormal;
}

void LAObject2::SetLight(bool bCheckLight)
{
	m_bisLight = bCheckLight;
}

void LAObject2::SetResourceVS(ID3D11VertexShader* pVS)
{
	if (pVS)
	{
		m_pResourceVS = pVS;
	}
}

void LAObject2::SetResourcePS(ID3D11PixelShader* pPS)
{
	if (pPS)
	{
		m_pResourcePS = pPS;
	}
}

void LAObject2::SetBlob(ID3DBlob* pVSBuf)
{
	this->pVSBuf = pVSBuf;
}

bool LAObject2::GetNormalMapping()
{
	return m_bIsNormalMap;
}

bool LAObject2::GetLight()
{
	return m_bisLight;
}

bool LAObject2::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pCoreDevice = pDevice;
	m_pCoreContext = pContext;
	D3DXMatrixIdentity(&m_ObjectWorld);
	D3DXMatrixIdentity(&m_ObjectView);
	D3DXMatrixIdentity(&m_ObjectProjection);

	return true;
}

bool LAObject2::Frame()
{
	m_vUp.x = m_ObjectWorld._12;
	m_vUp.y = m_ObjectWorld._22;
	m_vUp.z = m_ObjectWorld._32;
	m_vLook.x = m_ObjectWorld._13;
	m_vLook.y = m_ObjectWorld._23;
	m_vLook.z = m_ObjectWorld._33;
	m_vSide.x = m_ObjectWorld._11;
	m_vSide.x = m_ObjectWorld._21;
	m_vSide.x = m_ObjectWorld._31;

	return true;
}

bool LAObject2::PrevRender()
{
	//m_pCoreContext->RSSetState(INSTANCE_CUR_STATE.SetRrsState((int)E_RRS::RRSSolidCullNone));

	return true;
}

bool LAObject2::PostRender()
{
	return true;
}

bool LAObject2::Render()
{
	PrevRender();

	m_pCoreContext->VSSetShader(m_pResourceVS, NULL, 0);
	m_pCoreContext->HSSetShader(NULL, NULL, 0);
	m_pCoreContext->DSSetShader(NULL, NULL, 0);
	m_pCoreContext->PSSetShader(m_pResourcePS, NULL, 0);

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

bool LAObject2::Release()
{
	S_RELEASE(m_pVertexBuffer);
	S_RELEASE(m_pIndexBuffer);
	S_RELEASE(m_pConstantBuffer);
	S_RELEASE(m_pPS);
	S_RELEASE(m_pVS);
	S_RELEASE(m_pGS);
	S_RELEASE(pVSBuf);
	S_RELEASE(pPSBuf);
	S_RELEASE(pGSBuf);
	m_Texture.Release();
	//S_RELEASE(m_pTextureSRV);
	S_RELEASE(m_pVertexLayout);
	ZeroMemory(&m_Scale,sizeof(TMatrix));
	ZeroMemory(&m_Rotate, sizeof(TMatrix));
	ZeroMemory(&m_Move, sizeof(TMatrix));
	D3DXMatrixIdentity(&m_ObjectWorld);
	D3DXMatrixIdentity(&m_ObjectView);
	D3DXMatrixIdentity(&m_ObjectProjection);
	m_bCheckReleaseObject = true;

#ifdef TOOL_MODE

	if (m_pDrawLine)
	{
		m_pDrawLine->Release();
		delete m_pDrawLine;
		m_pDrawLine = NULL;
	}

#else
#endif


	return true;
}

LAObject2::LAObject2()
{
	SAFE_ZERO_POINT(m_pVertexBuffer);
	SAFE_ZERO_POINT(m_pIndexBuffer);
	SAFE_ZERO_POINT(m_pConstantBuffer);
	SAFE_ZERO_POINT(m_pPS);
	SAFE_ZERO_POINT(m_pLightPS);
	SAFE_ZERO_POINT(m_pVS);
	SAFE_ZERO_POINT(m_pNormalPS);
	SAFE_ZERO_POINT(m_pNormalVS);
	SAFE_ZERO_POINT(m_pGS);
	SAFE_ZERO_POINT(m_pShadowPS);
	SAFE_ZERO_POINT(pVSBuf);
	SAFE_ZERO_POINT(pPSBuf);
	SAFE_ZERO_POINT(pGSBuf);
	//SAFE_ZERO_POINT(m_pTextureSRV);
	SAFE_ZERO_POINT(m_pVertexLayout);
	SAFE_ZERO_POINT(m_pResourceVS);
	SAFE_ZERO_POINT(m_pResourcePS);
	ZeroMemory(&m_Scale, sizeof(TMatrix));
	ZeroMemory(&m_Rotate, sizeof(TMatrix));
	ZeroMemory(&m_Move, sizeof(TMatrix));
	D3DXMatrixIdentity(&m_ObjectWorld);
	D3DXMatrixIdentity(&m_ObjectView);
	D3DXMatrixIdentity(&m_ObjectProjection);
	m_iMultiTextureRender = 0;
	
	pHullShader = NULL;
	pDomainShader = NULL;

	//Direction Light
	ZeroMemory(&m_vLightPos, sizeof(TVector4));
	ZeroMemory(&m_vLightDirection, sizeof(TVector4));
	ZeroMemory(&m_vCameraPos, sizeof(TVector4));

	//Check
	m_bCheckReleaseObject = false;
	m_bIsNormalMap = false;
	m_bisLight = false;

#ifdef TOOL_MODE

	m_pDrawLine = NULL;

#else
#endif
}


LAObject2::~LAObject2()
{
	if (m_bCheckReleaseObject == false)
		LAObject2::Release();
}
