#include "BObject.h"
#include "BInput.h"

HRESULT BObject::Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, TCHAR** pLoadTextureString, int TextureCnt)
{
	m_pd3dDevice = pDevice;

	HRESULT hr = S_OK;

	S_RETURN(LoadShaderAndInputLayout(pLoadShaderFile));
	S_RETURN(CreateVertexBuffer());
	S_RETURN(CreateIndexBuffer());
	S_RETURN(CreateConstantBuffer());
	if (pLoadTextureString != NULL)
	{
		if (TextureCnt < 2)
		{
			S_RETURN(TextureLoad(pDevice, pLoadTextureString[0]));
		}
		else
		{
			S_RETURN(TextureLoad(pDevice, pLoadTextureString, TextureCnt));
		}
	}
	else
	{
		m_iCurIndex = 0;
	}
	return hr;
}

HRESULT BObject::Create(ID3D11Device* pDevice, const TCHAR* pLoadShaderFile, const TCHAR* pLoadTextureString)
{
	m_pd3dDevice = pDevice;

	HRESULT hr = S_OK;

	S_RETURN(LoadShaderAndInputLayout(pLoadShaderFile));
	S_RETURN(CreateVertexBuffer());
	S_RETURN(CreateIndexBuffer());
	S_RETURN(CreateConstantBuffer());
	if (pLoadTextureString != NULL)
	{
		S_RETURN(TextureLoad(pDevice, pLoadTextureString));
	}
	else
	{
		m_iCurIndex = 0;
	}
	return hr;
}

HRESULT BObject::TextureLoad(ID3D11Device* pDevice, const TCHAR* strFilePath)
{
	HRESULT hr = S_OK;
	string Ext = GetFileExtension((TCHAR*)strFilePath);

	m_Texture.m_pTextureSRV.resize(1);
	if (strFilePath == NULL) 
		m_Texture.m_pTextureSRV[0].Attach(nullptr);

	m_Texture.LoadTexture(strFilePath, mtw(Ext));

	return hr;
}

HRESULT BObject::TextureLoad(ID3D11Device* pDevice, TCHAR* strFilePathes[], int strCount)
{
	HRESULT hr = S_OK;
	string Ext;

	m_Texture.m_pTextureSRV.resize(strCount);
	for (int iTex = 0; iTex < strCount; iTex++)
	{
		Ext = GetFileExtension(strFilePathes[iTex]);
		m_Texture.LoadTexture(strFilePathes[iTex], mtw(Ext));
		m_iCurIndex++;
	}

	return hr;
}

HRESULT BObject::TextureLoad(ID3D11Device* pDevice, vector<T_STR> strFilePathes)
{
	HRESULT hr = S_OK;
	string Ext;

	m_Texture.m_pTextureSRV.resize(m_iTextureCount);
	for (int iTex = 0; iTex < m_iTextureCount; iTex++)
	{
		Ext = GetFileExtension((TCHAR*)strFilePathes[iTex].c_str());
		m_Texture.LoadTexture(strFilePathes[iTex], mtw(Ext));
		m_iCurIndex++;
	}

	return hr;
}

HRESULT BObject::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	return hr;
}
HRESULT BObject::CreateIndexBuffer()
{
	HRESULT hr = S_OK;
	return hr;
}
HRESULT BObject::CreateConstantBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;					//CPU에서 GPU로 Access 가능하게 해주는 플래그
	cbDesc.CPUAccessFlags = 0;							//CPU에서 GPU로 Access 가능하게 해주는 플래그
	cbDesc.ByteWidth = sizeof(VSConstantB);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;

	S_RETURN(m_pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pConstantBuffer));
	return hr;
}
HRESULT BObject::LoadShaderAndInputLayout(const TCHAR* pShaderFile)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // 쉐이더 디버그 허용
	ID3DBlob* pVSBUF = NULL;
	ID3DBlob* pPSBUF = NULL;
	ID3DBlob* pGSBUF = NULL;
	
	ID3DBlob* pError = NULL; // 이것을 넣음으로써 오류가 난 이유를 받아 낼 수 있다.
							 /* vs_5_0 == direct11 은 5.0을 사용 * pVSBuf 결과가 담김 */
	
	if (FAILED(D3DCompileFromFile(pShaderFile, NULL, NULL, "VS", "vs_5_0", NULL, NULL, &pVSBUF, &pError)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		MessageBox(NULL, buffer, _T("VS"), MB_OK);
	}
	if (pVSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreateVertexShader((DWORD*)pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), NULL, &m_pVS));
	}

	if (FAILED(D3DCompileFromFile(pShaderFile, NULL, NULL, "GS", "gs_5_0", NULL, NULL, &pGSBUF, &pError)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		//MessageBox(NULL, buffer, _T("GS"), MB_OK);
	}
	if (pGSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreateGeometryShader((DWORD*)pGSBUF->GetBufferPointer(), pGSBUF->GetBufferSize(), NULL, &m_pGS));
	}

	if (FAILED(D3DCompileFromFile(pShaderFile, NULL, NULL, "PS", "ps_5_0", NULL, NULL, &pPSBUF, &pError)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		//MessageBox(NULL, buffer, _T("PS"), MB_OK);
	}
	if (pPSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreatePixelShader((DWORD*)pPSBUF->GetBufferPointer(), pPSBUF->GetBufferSize(), NULL, &m_pPS));
	}

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT iSize = sizeof(layout) / sizeof(layout[0]);

	S_RETURN(m_pd3dDevice->CreateInputLayout(layout, iSize, pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), &m_pVertexLayout)); // m_pVertexLayout 제작

	S_RELEASE(pVSBUF);
	S_RELEASE(pPSBUF);
	S_RELEASE(pGSBUF);
	return hr;
}

void BObject::SetMatrix(TMatrix World, TMatrix View, TMatrix Proj)
{
	m_matWorld = World;
	m_matView = View;
	m_matProj = Proj;

	D3DXMatrixTranspose(&m_matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_matView, &m_matView);
	D3DXMatrixTranspose(&m_matProj, &m_matProj);

	m_cbData.m_matWorld = m_matWorld;
	m_cbData.m_matView = m_matView;
	m_cbData.m_matProj = m_matProj;
	m_cbData.f_Time = cosf(timeGetTime()*0.001f) * 0.5f + 0.5f;

	D3DXMatrixTranspose(&m_matWorld, &m_matWorld);
}
bool BObject::Init()
{
	m_vPos = TVector3(0.0f, 0.0f, 0.0f);
	m_vTarget = TVector3(0.0f, 0.0f, 1.0f);
	m_vUp = TVector3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_matWorld, &m_vPos, &m_vTarget, &m_vUp);
	UpdateVector();

	return true;
}

bool BObject::Frame()
{
	return true;
}

bool BObject::PostFrame()
{
	// 위
	if (g_InputData.bWKey)
		m_vPos += m_vLook * 10.0f * (g_fSecPerFrame + m_SpeedUp);
	// 아래
	if (g_InputData.bSKey)
		m_vPos -= m_vLook * 10.0f * (g_fSecPerFrame + m_SpeedUp);
	// 왼쪽
	if (g_InputData.bAKey)
		m_vPos -= m_vSide * 10.0f * (g_fSecPerFrame + m_SpeedUp);
	// 오른쪽
	if (g_InputData.bDKey)
		m_vPos += m_vSide * 10.0f * (g_fSecPerFrame + m_SpeedUp);
	// 위
	if (g_InputData.bQKey)
		m_vPos += m_vUp * 10.0f * (g_fSecPerFrame);
	// 아래
	if (g_InputData.bEKey)
		m_vPos -= m_vUp * 10.0f * (g_fSecPerFrame);
	/*카메라 이동가속도*/
	if (g_InputData.bSpace)
		m_SpeedUp += 0.00001;
	else
		m_SpeedUp = 0;

	if (I_Input.m_DIMouseState.rgbButtons[1] & 0x80)
	{
		m_fPitch += XMConvertToRadians(I_Input.m_DIMouseState.lY) * 0.1;
		m_fYaw += XMConvertToRadians(I_Input.m_DIMouseState.lX) * 0.1;
	}
	UpdateQuaternion();

	return true;
}

bool BObject::PreRender(ID3D11DeviceContext* pContext)
{
	pContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_iPrimitiveType);		//토폴로지 세팅
	pContext->IASetInputLayout(m_pVertexLayout);										//레이아웃 세팅
																						/*버텍스 버퍼 세팅*/
	UINT stride = sizeof(PNCT_Vertex);		 // 버텍스 하나 전체의 크기
	UINT offset = 0;						 // 버퍼 시작위치
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);  // 버텍스 버퍼 바인딩
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);	 // 인덱스 버퍼 바인딩
	pContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);				 // 버텍스 쉐이더 상수버퍼 세팅
	
	/*쉐이더 적용*/
	pContext->VSSetShader(m_pVS, NULL, 0);
	pContext->HSSetShader(NULL, NULL, 0);
	pContext->DSSetShader(NULL, NULL, 0);
	pContext->GSSetShader(NULL, NULL, 0);
	pContext->PSSetShader(m_pPS, NULL, 0);
	if (m_iCurIndex != 0)
	{
		pContext->PSSetShaderResources(0, m_iCurIndex, m_Texture.m_pTextureSRV[0].GetAddressOf()); // 텍스쳐 세팅
	}
	return true;
}
bool BObject::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_cbData, 0, 0);
	pContext->DrawIndexed(m_IndexList.size(), 0, 0);
	// 초기화
	vector<ComPtr<ID3D11ShaderResourceView>> pSRV;
	pSRV.resize(m_iCurIndex);
	pSRV = { NULL, };
	pContext->PSSetShaderResources(0, m_iCurIndex, pSRV[0].GetAddressOf());
	return true;
}

/*Device에서 DX내의 DSV, DSS, BS를 전부 선언해 두었기 때문에 해당하는 값과 Core나 Sample에서 제작한 m_pRTV를 넘겨주기만 하면 된다.*/
/*해당 함수는 랜더링 전에 이루어져야 한다.*/
bool BObject::OMSetState(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* m_pRTV, ID3D11DepthStencilView* m_pDSV, ID3D11DepthStencilState* m_pDSS, ID3D11BlendState* m_pBS)
{
	pContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV);		// 랜더타겟뷰, 깊이버퍼 설정
	pContext->OMSetDepthStencilState(m_pDSS, 0x00);			// 깊이 스텐실 뷰설정
	pContext->OMSetBlendState(m_pBS, 0, -1);				// 알파블랜드 값 설정
	return true;
}

bool BObject::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	return true;
}

bool BObject::UpdateVector()
{
	m_vSide.x = m_matWorld._11;
	m_vSide.y = m_matWorld._12;
	m_vSide.z = m_matWorld._13;

	m_vUp.x = m_matWorld._21;
	m_vUp.y = m_matWorld._22;
	m_vUp.z = m_matWorld._23;

	m_vLook.x = m_matWorld._31;
	m_vLook.y = m_matWorld._32;
	m_vLook.z = m_matWorld._33;

	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vSide, &m_vSide);
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	return true;
}

bool BObject::UpdateQuaternion()
{
	UpdateVector();

	m_matWorld._41 = m_vPos.x;
	m_matWorld._42 = m_vPos.y;
	m_matWorld._43 = m_vPos.z;
	return true;
}

bool BObject::UpdateBuffer() { return true; }

bool BObject::Release()
{
	S_RELEASE(m_pVertexLayout);
	S_RELEASE(m_pVertexBuffer);
	S_RELEASE(m_pIndexBuffer);
	S_RELEASE(m_pConstantBuffer);
	S_RELEASE(m_pVS);
	S_RELEASE(m_pPS);
	S_RELEASE(m_pGS);
	return true;
}

bool BObject::UpDateTangentBuffer()
{
	TVector3 vTangent, vBiNormal, vNormal;
	int iIndex = 0;
	int i0, i1, i2, i3, i4, i5;

	m_TangentList.resize(m_VertexList.size());

	for (int iIndex = 0; iIndex < m_IndexList.size(); iIndex += 3)
	{
		i0 = m_IndexList[iIndex + 0];
		i1 = m_IndexList[iIndex + 1];
		i2 = m_IndexList[iIndex + 2];

		UpDateTangentSpaceVectors(&m_VertexList[i0].Position, &m_VertexList[i1].Position, &m_VertexList[i2].Position,
			m_VertexList[i0].Texture, m_VertexList[i1].Texture, m_VertexList[i2].Texture,
			&vTangent, &vBiNormal, &m_VertexList[i0].Normal);

		m_TangentList[i0] += vTangent;

		i0 = m_IndexList[iIndex + 1];
		i1 = m_IndexList[iIndex + 2];
		i2 = m_IndexList[iIndex + 0];

		UpDateTangentSpaceVectors(&m_VertexList[i0].Position, &m_VertexList[i1].Position, &m_VertexList[i2].Position,
			m_VertexList[i0].Texture, m_VertexList[i1].Texture, m_VertexList[i2].Texture,
			&vTangent, &vBiNormal, &m_VertexList[i0].Normal);

		m_TangentList[i0] += vTangent;

		i0 = m_IndexList[iIndex + 2];
		i1 = m_IndexList[iIndex + 0];
		i2 = m_IndexList[iIndex + 1];

		UpDateTangentSpaceVectors(&m_VertexList[i0].Position, &m_VertexList[i1].Position, &m_VertexList[i2].Position,
			m_VertexList[i0].Texture, m_VertexList[i1].Texture, m_VertexList[i2].Texture,
			&vTangent, &vBiNormal, &m_VertexList[i0].Normal);

		m_TangentList[i0] += vTangent;
	}
	for (int iIndex = 0; iIndex < m_TangentList.size(); iIndex ++)
	{
		D3DXVec3Normalize(&m_TangentList[iIndex], &m_TangentList[iIndex]);
	}

	m_TangentCnt = m_TangentList.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						
	bd.ByteWidth = sizeof(TVector3) * m_TangentCnt;	
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			
	bd.CPUAccessFlags = 0;								
	bd.MiscFlags = 0;									

	D3D11_SUBRESOURCE_DATA InitData;	
	InitData.pSysMem = (void*)&m_TangentList.at(0);		
	m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pTangentBuffer); // 접선벡터버퍼 생성

	return true;
}

void BObject::UpDateTangentSpaceVectors(TVector3 *v0, TVector3 *v1, TVector3 *v2, TVector2 uv0, TVector2 uv1, TVector2 uv2, TVector3 *vTangent, TVector3 *vBiNormal, TVector3 *vNormal)
{
	TVector3 vEdge1 = *v1 - *v0;
	TVector3 vEdge2 = *v2 - *v0;
	D3DXVec3Normalize(&vEdge1, &vEdge1);
	D3DXVec3Normalize(&vEdge2, &vEdge2);

	// UV Delta
	TVector2 deltaUV1 = uv1 - uv0;
	TVector2 deltaUV2 = uv2 - uv0;
	D3DXVec2Normalize(&deltaUV1, &deltaUV1);
	D3DXVec2Normalize(&deltaUV2, &deltaUV2);

	TVector3 biNormal;
	float fDet = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	if (fabsf(fDet) < 1e-6f)
	{
		*vTangent = TVector3(1.0f, 0.0f, 0.0f);
		biNormal = TVector3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		*vTangent = (vEdge1 * deltaUV2.y - vEdge2 * deltaUV1.y)*fDet;
		biNormal = (vEdge2 * deltaUV1.x - vEdge1 * deltaUV2.x)*fDet;
	}
	D3DXVec3Normalize(vTangent, vTangent);
	D3DXVec3Normalize(&biNormal, &biNormal);

	D3DXVec3Cross(vBiNormal, vNormal, vTangent);
	float crossinv = (D3DXVec3Dot(vBiNormal, &biNormal) < 0.0f) ? -1.0f : 1.0f;
	*vBiNormal *= crossinv;
}

void BObject::UpdateMinMaxBox()
{
	if (m_VertexList.size())
	{
		m_tBox.vCenter.x = m_matWorld._41;
		m_tBox.vCenter.y = m_matWorld._42;
		m_tBox.vCenter.z = m_matWorld._43;

		D3DXVec3TransformCoord(&m_tBox.vAxis[0], &m_tBox.vAxis[0], &m_matWorld);
		D3DXVec3TransformCoord(&m_tBox.vAxis[1], &m_tBox.vAxis[1], &m_matWorld);
		D3DXVec3TransformCoord(&m_tBox.vAxis[2], &m_tBox.vAxis[2], &m_matWorld);
		D3DXVec3Normalize(&m_tBox.vAxis[0], &m_tBox.vAxis[0]);
		D3DXVec3Normalize(&m_tBox.vAxis[1], &m_tBox.vAxis[1]);
		D3DXVec3Normalize(&m_tBox.vAxis[2], &m_tBox.vAxis[2]);

		TVector3 vMax, vMin, vHalf;
	
		vMin = TVector3(100000.0f, 100000.0f, 100000.0f);
		vMax = TVector3(-100000.0f, -100000.0f, -100000.0f);

		vector<TVector3> vVertex;
		vVertex.resize(m_VertexList.size());
		for (int iVer = 0; iVer < m_VertexList.size(); iVer++)
		{
			if (m_VertexList[iVer].Position.x > vMax.x)
			{
				vMax.x = m_VertexList[iVer].Position.x;
			}
			if (m_VertexList[iVer].Position.y > vMax.y)
			{
				vMax.y = m_VertexList[iVer].Position.y;
			}
			if (m_VertexList[iVer].Position.z > vMax.z)
			{
				vMax.z = m_VertexList[iVer].Position.z;
			}
			if (m_VertexList[iVer].Position.x < vMin.x)
			{
				vMin.x = m_VertexList[iVer].Position.x;
			}
			if (m_VertexList[iVer].Position.y < vMin.y)
			{
				vMin.y = m_VertexList[iVer].Position.y;
			}
			if (m_VertexList[iVer].Position.z < vMin.z)
			{
				vMin.z = m_VertexList[iVer].Position.z;
			}
		}
		
		TVector3 TempMax = vMax;
		D3DXVec3TransformCoord(&TempMax, &TempMax, &m_matWorld);
		vHalf = TempMax - m_tBox.vCenter;

		m_tBox.fExtent[0] = D3DXVec3Dot(&m_tBox.vAxis[0], &vHalf);
		m_tBox.fExtent[1] = D3DXVec3Dot(&m_tBox.vAxis[1], &vHalf);
		m_tBox.fExtent[2] = D3DXVec3Dot(&m_tBox.vAxis[2], &vHalf);

		D3DXVec3TransformNormal(&vMax, &vMax, &m_matWorld);
		m_tBox.vMin = vMin;
		m_tBox.vMax = vMax;
	}
}

bool BObject::InitFaceNormal()
{
	m_iNumFace = m_IndexList.size() / 3;
	m_FaceNormals.resize(m_iNumFace);
	int iCnt = 0;
	for (int iFace = 0; iFace < m_iNumFace * 3; iFace += 3)
	{
		DWORD i0 = m_IndexList[iFace];
		DWORD i1 = m_IndexList[iFace + 1];
		DWORD i2 = m_IndexList[iFace + 2];

		TVector3 v0 = m_VertexList[i0].Position;
		TVector3 v1 = m_VertexList[i1].Position;
		TVector3 v2 = m_VertexList[i2].Position;

		TVector3 e0 = v1 - v0;
		TVector3 e1 = v2 - v0;

		D3DXVec3Cross(&m_FaceNormals[iCnt], &e0, &e1);
		D3DXVec3Normalize(&m_FaceNormals[iCnt], &m_FaceNormals[iCnt]);
		iCnt++;
	}
	return true;
}

void BObject::GenNormalLookupTable()
{
	m_Normallookuptable.resize(m_iNumVertices * 6);
	for (int iCnt = 0; iCnt < m_Normallookuptable.size(); iCnt++)
	{
		m_Normallookuptable[iCnt] = -1;
	}
	for (int iCnt = 0; iCnt < m_iNumFace; iCnt++)
	{
		for (int jCnt = 0; jCnt < 3; jCnt++)
		{
			for (int kCnt = 0; kCnt < 6; kCnt++)
			{
				int vertex = m_IndexList[iCnt * 3 + jCnt];
				if (m_Normallookuptable[vertex * 6 + kCnt] == -1)
				{
					m_Normallookuptable[vertex * 6 + kCnt] = iCnt;
					break;
				}
			}
		}
	}
}

void BObject::CalcPerVertexNormalsFastLookup()
{
	int jCnt = 0;
	for (int iCnt = 0; iCnt < m_iNumVertices; iCnt++)
	{
		TVector3 avgNormal;
		avgNormal = TVector3(0.0f, 0.0f, 0.0f);

		for (jCnt = 0; jCnt < 6; jCnt++) // 각 정점당 최대 6개의 페이스
		{
			int triIndex;
			triIndex = m_Normallookuptable[iCnt * 6 + jCnt];

			if (triIndex != -1)
			{
				avgNormal += m_FaceNormals[triIndex];
			}
			else
				break;
		}
		_ASSERT(jCnt > 0);
		avgNormal.x /= (float)jCnt;
		avgNormal.y /= (float)jCnt;
		avgNormal.z /= (float)jCnt;
		D3DXVec3Normalize(&avgNormal, &avgNormal);

		m_VertexList[iCnt].Normal.x = avgNormal.x;
		m_VertexList[iCnt].Normal.y = avgNormal.y;
		m_VertexList[iCnt].Normal.z = avgNormal.z;
	}
}


bool BObject::UpdateNormal()
{
	InitFaceNormal();
	GenNormalLookupTable();
	CalcPerVertexNormalsFastLookup();
	UpDateTangentBuffer();
	return true;
}

BObject::BObject()
{
	m_pVertexLayout = NULL;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pConstantBuffer = NULL;
	m_pVS = NULL;
	m_pPS = NULL;
	m_iCurIndex = 0;
	m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRotate);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matScale);
}


BObject::~BObject()
{
	m_iCurIndex = 0;
}
