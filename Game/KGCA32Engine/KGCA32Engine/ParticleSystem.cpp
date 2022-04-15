#include "ParticleSystem.h"

//--------------------------------------------------------------------------------------
// �ν��Ͻ�ȭ ���� ����
//--------------------------------------------------------------------------------------
bool ParticleSystem::CreateInstanceBuffer()
{
	m_InstDataArray.resize(m_iEffectCount);
	for (int iCnt = 0; iCnt < m_iEffectCount; iCnt++)
	{
		D3DXMatrixTranslation(&m_InstDataArray[iCnt].matWorld, m_Particle[iCnt].m_vPos.x, m_Particle[iCnt].m_vPos.y, m_Particle[iCnt].m_vPos.z);
		D3DXMatrixTranspose(&m_InstDataArray[iCnt].matWorld, &m_InstDataArray[iCnt].matWorld);
		m_InstDataArray[iCnt].m_vColor = m_Particle[iCnt].m_vColor;
	}

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Instance) * m_iEffectCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = (void*)&m_InstDataArray.at(0);
	m_pd3dDevice->CreateBuffer(&bd, &InitData, m_pInstanceBuffer.GetAddressOf());
	return true;
}

//--------------------------------------------------------------------------------------
// ���̾ƿ� �� ���̴� ����
//--------------------------------------------------------------------------------------
bool ParticleSystem::LoadShaderAndInputLayout(TCHAR* vertexShaderName, TCHAR* PixelShaderName, TCHAR* geometryShaderName)
{
	HRESULT hr = S_OK;
	

	ComPtr<ID3DBlob> pVSBuf = NULL;
	ID3DBlob* pError = 0;
	if (FAILED(D3DCompileFromFile(vertexShaderName, NULL, NULL, "VS", "vs_5_0", NULL, NULL, &pVSBuf, &pError)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer,
			(char*)pError->GetBufferPointer(),
			256);
		MessageBox(NULL, buffer, _T("pVSBuffer"), MB_OK);
	}
	m_pd3dDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_Effect.m_pVS);

	// Compile the PS from the file
	ComPtr<ID3DBlob> pPSBuf = NULL;
	if (FAILED(D3DCompileFromFile(PixelShaderName, NULL, NULL, "PS", "ps_5_0", NULL, NULL, &pPSBuf, &pError)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer,
			(char*)pError->GetBufferPointer(),
			256);
		MessageBox(NULL, buffer, _T("pVSBuffer"), MB_OK);
	}
	m_pd3dDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_Effect.m_pPS);

	//// Compile the GS from the file
	//ComPtr<ID3DBlob> pGSBuf = NULL;
	//D3DCompileFromFile(geometryShaderName, NULL, NULL, "GS", "gs_5_0", 0, NULL, NULL, &pGSBuf, NULL, NULL);
	//m_pd3dDevice->CreateGeometryShader(pGSBuf->GetBufferPointer(), pGSBuf->GetBufferSize(), NULL, &m_Effect->m_pGS);
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL" ,   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",   0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		// �ν��Ͻ� ����
		{ "mTransform",		 0, DXGI_FORMAT_R32G32B32A32_FLOAT,	   1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "mTransform",		 1, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "mTransform",		 2, DXGI_FORMAT_R32G32B32A32_FLOAT,	   1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "mTransform",		 3, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "PARTICLECOLOR",	 0, DXGI_FORMAT_R32G32B32A32_FLOAT,    1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	
	if (FAILED(hr = m_pd3dDevice->CreateInputLayout(layout, numElements, pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), m_pLayout.GetAddressOf())))
	{
		hr = S_FALSE;
	}

	return true;
}

//--------------------------------------------------------------------------------------
// ����̽� / ī�޶� �ʱ�ȭ �Լ�
//--------------------------------------------------------------------------------------
bool ParticleSystem::Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext, BCamera* pMainCamera)
{
	m_pd3dDevice.Attach(pDevice);
	m_pImmediateContext.Attach(pImmediateContext);
	m_pCamera = pMainCamera;
	return true;
}

//--------------------------------------------------------------------------------------
// Create
//--------------------------------------------------------------------------------------
bool ParticleSystem::Create()
{
	m_Particle.resize(m_iEffectCount);
	D3DXMatrixIdentity(&m_matWorld);

	CreateInstanceBuffer();
	LoadShaderAndInputLayout(L"../../data/Shader/EffectInstanceShader.hlsl", L"../../data/Shader/EffectShader.hlsl", NULL);

	return true;
}

//--------------------------------------------------------------------------------------
// ��ƼŬ ������Ʈ �Լ�
//--------------------------------------------------------------------------------------
void ParticleSystem::ParticleUpdate()
{
	m_Effect.Update();

	m_Effect.m_VertexList[0].Texture = m_Effect.GetDrawRect(0);
	m_Effect.m_VertexList[1].Texture = m_Effect.GetDrawRect(1);
	m_Effect.m_VertexList[2].Texture = m_Effect.GetDrawRect(2);
	m_Effect.m_VertexList[3].Texture = m_Effect.GetDrawRect(3);
	m_pImmediateContext->UpdateSubresource(m_Effect.m_pVertexBuffer, 0, NULL, &m_Effect.m_VertexList.at(0), 0, 0);

	TMatrix matWorld, matBillboard;
	TMatrix matScale, matRotation;
	TMatrix matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matBillboard);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matRotation);

	// ��������� ����
	if (m_bBillboard == TRUE)
	{
		matBillboard = m_pCamera->m_matView;
		D3DXMatrixInverse(&matBillboard, NULL, &matBillboard);
		matBillboard._41 = 0;
		matBillboard._42 = 0;
		matBillboard._43 = 0;
		matBillboard._44 = 1;
	}

	// �������� ����
	if (m_bOrthogonal == TRUE)
	{
		TVector3 vEye = TVector3(0.0f, 0.0f, -10.0f);
		TVector3 vAt = TVector3(0.0f, 0.0f, 0.0f);
		TVector3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
		int iRectWidth = g_iWindowWidth / 2;
		int iRectHeight = g_iWindowHeight / 2;
		// ȭ�� �߾��� �������� ���Ǳ� ������ ���� �� ���̰� -1 ~ 1 ������ ���������ȴ�. 
		D3DXMatrixOrthoOffCenterLH(&matProj, -iRectWidth, iRectWidth, -iRectHeight, iRectHeight, 0.0f, 1000.0f);
	}
	

	TVector3 vPos = { 0.0f, 0.0f, 0.0f };
	for (int iCnt = 0; iCnt < m_iEffectCount; iCnt++)
	{
		if (m_EffectType == EFFECT_TYPE::Straight)
		{
			// ���⺤�Ϳ� �ӵ��κ��� ��ġ ����
			vPos = m_Particle[iCnt].m_vPos + (m_Particle[iCnt].m_vDirection * (m_Particle[iCnt].m_vSpeed * g_fSecPerFrame));
		}
		else if(m_EffectType == EFFECT_TYPE::Circle)
		{
			vPos = m_Particle[iCnt].m_vPos + 
				((Circle(m_Particle[iCnt].m_fRadian, m_fEffectElapseTime * m_Particle[iCnt].m_vSpeed.x) + m_Particle[iCnt].m_vDirection) * (m_Particle[iCnt].m_vSpeed * g_fSecPerFrame));
		}
		m_Particle[iCnt].m_vPos = vPos;

		// ����Ʈ ũ�� ����
		D3DXMatrixScaling(&matScale, m_Particle[iCnt].m_vScale.x, m_Particle[iCnt].m_vScale.y, m_Particle[iCnt].m_vScale.z);
		
		// ����Ʈ ȸ�� ����
		if (m_bRotationEffect == true)
		{
			m_Particle[iCnt].m_fRotationAngle += cosf(g_fSecPerFrame);
			D3DXMatrixRotationZ(&matRotation, m_Particle[iCnt].m_fRotationAngle);
		}

		// ������� S, R, T ���� 
		matWorld = matScale * matRotation;
		m_Effect.m_matWorld = matWorld * matBillboard;

		m_Effect.m_matWorld._41 = vPos.x;
		m_Effect.m_matWorld._42 = vPos.y;
		m_Effect.m_matWorld._43 = vPos.z;
	
		// ������� ����
		if (m_bOrthogonal == TRUE)
			m_Effect.SetMatrix(m_Effect.m_matWorld, matView, matProj);
		else
			m_Effect.SetMatrix(m_Effect.m_matWorld, m_pCamera->m_matView, m_pCamera->m_matProj);

		m_Effect.m_cbData.f_W = m_Effect.m_iTextureCount;

		// �ν��Ͻ� ������� ����
		m_InstDataArray[iCnt].m_vColor = m_Particle[iCnt].m_vColor;
		D3DXMatrixTranspose(&m_InstDataArray[iCnt].matWorld, &m_Effect.m_matWorld);
		vPos = { 0.0f, 0.0f, 0.0f };
	}
}

//--------------------------------------------------------------------------------------
// ��� �Լ�
//--------------------------------------------------------------------------------------
TVector3 ParticleSystem::Circle(float fRadius, float fRadian)
{
	TVector3 vRet;
	if (m_CoordinateType == RotationX)
	{
		vRet.x = 0.0f;
		vRet.y = fRadius * sinf(fRadian);
		vRet.z = fRadius * cosf(fRadian);
	}
	else if (m_CoordinateType == RotationY)
	{
		vRet.x = fRadius * sinf(fRadian);
		vRet.y = 0.0f;
		vRet.z = fRadius * cosf(fRadian);
	}
	else
	{
		vRet.x = fRadius * sinf(fRadian);
		vRet.y = fRadius * cosf(fRadian);
		vRet.z = 0.0f;
	}
	return vRet;
}

//--------------------------------------------------------------------------------------
// �ٿ���ڽ� ����
//--------------------------------------------------------------------------------------
void ParticleSystem::CreateBoundingBox(TVector3 ObjectPos, int iType)
{
	if (iType == 0)
	{
		m_BoundingBoxMin = ObjectPos - m_BoundingBoxMax;
		m_BoundingBoxMax = ObjectPos + m_BoundingBoxMax;
	}

	m_BoundingBox.CreateAABBBox(
		TVector3(m_BoundingBoxMax.x, m_BoundingBoxMax.y, m_BoundingBoxMax.z),
		TVector3(m_BoundingBoxMin.x, m_BoundingBoxMin.y, m_BoundingBoxMin.z));
	m_Plane[0].CreatePlane(m_BoundingBox.m_tBox.vPos[5], m_BoundingBox.m_tBox.vPos[0], m_BoundingBox.m_tBox.vPos[1]);	// �� ���(left)
	m_Plane[1].CreatePlane(m_BoundingBox.m_tBox.vPos[3], m_BoundingBox.m_tBox.vPos[6], m_BoundingBox.m_tBox.vPos[2]);	// �� ���(right)
	m_Plane[2].CreatePlane(m_BoundingBox.m_tBox.vPos[5], m_BoundingBox.m_tBox.vPos[2], m_BoundingBox.m_tBox.vPos[6]);	// �� ���(top)
	m_Plane[3].CreatePlane(m_BoundingBox.m_tBox.vPos[0], m_BoundingBox.m_tBox.vPos[7], m_BoundingBox.m_tBox.vPos[3]);	// �� ���(bottom)
	m_Plane[4].CreatePlane(m_BoundingBox.m_tBox.vPos[0], m_BoundingBox.m_tBox.vPos[2], m_BoundingBox.m_tBox.vPos[1]);	// �� ���(near)
	m_Plane[5].CreatePlane(m_BoundingBox.m_tBox.vPos[6], m_BoundingBox.m_tBox.vPos[4], m_BoundingBox.m_tBox.vPos[5]);	// �� ���(far)
}

//--------------------------------------------------------------------------------------
// ��ƼŬ �ý��� ������ �� �Լ�
//--------------------------------------------------------------------------------------
void ParticleSystem::SetFrameParticleSystemInfo(TVector3* vPos, TVector3* vDir)
{
	// ��ƼŬ �ý��� �����Ӵ��� ��ƼŬ ���� ����
	for (int iEffect = 0; iEffect < this->m_Particle.size(); iEffect++)
	{
		// ��ġ ����
		if(vPos != NULL)
			this->m_Particle[iEffect].m_vPos = *vPos;

		// ���� ���� ����
		if (vDir != NULL)
			this->m_Particle[iEffect].m_vDirection = *vDir;

		// �ݺ� ����Ʈ�̸� �ѹ��� ó����ġ ����
		if (this->m_bRepeatEffect == true && this->m_bRepetEffectSet == true)
		{
			this->m_Particle[iEffect].m_vFirstPos = this->m_Particle[iEffect].m_vPos;
			this->m_bRepetEffectSet = false;
		}
	}

	// �ٿ���ڽ� ����
	if (vPos != NULL)
		this->CreateBoundingBox(*vPos);
	else
		this->CreateBoundingBox();

}

//--------------------------------------------------------------------------------------
// �浹ó�� �Լ�
// ������κ��� ������ ������ �Ÿ��� ���ϴ� �Լ�
// �� ���� ó�� ( ���� : ���븻�� ������ �ٶ󺸰� �ִ�. )
//--------------------------------------------------------------------------------------
bool ParticleSystem::ClassifyPoint(TVector3* pv)
{
	float fPlaneToPoint;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		fPlaneToPoint = (m_Plane[iPlane].fA * pv->x) +
			(m_Plane[iPlane].fB * pv->y) +
			(m_Plane[iPlane].fC * pv->z) +
			m_Plane[iPlane].fD;
		if (fPlaneToPoint < 0.0f)
			return false;
	}
	return true;
}

bool ParticleSystem::ClassifyPointInInterpolate(TVector3* pv)
{
	float fPlaneToPoint;
	for (int iPlane = 0; iPlane < 6; iPlane++)
	{
		fPlaneToPoint = (m_Plane[iPlane].fA * pv->x) +
			(m_Plane[iPlane].fB * pv->y) +
			(m_Plane[iPlane].fC * pv->z) +
			m_Plane[iPlane].fD;
		if (fPlaneToPoint <= EFFECT_EXTINCTION_DISTANCE)
			return false;
	}
	return true;
}

void ParticleSystem::CheckEffectRangeFromBox()
{
	float time = (EFFECT_EXTINCTION_DISTANCE / 2.0f) * g_fSecPerFrame;
	TVector4 vColor(0.0f, 0.0f, 0.0f, 0.0f);

	for (int iParticle = 0; iParticle < m_iEffectCount; iParticle++)
	{
		// ����
		if (m_bRepeatEffect == false && ClassifyPointInInterpolate(&m_Particle[iParticle].m_vPos) == false)
		{
			D3DXVec4Lerp(&m_Particle[iParticle].m_vColor,
				&m_Particle[iParticle].m_vColor,
				&vColor,
				time);
		}

		// ����
		if (!ClassifyPoint(&m_Particle[iParticle].m_vPos))
		{
			// �ݺ� ����Ʈ
			if (m_bRepeatEffect == TRUE)
			{
				m_Particle[iParticle].m_vPos = m_Particle[iParticle].m_vFirstPos;
				continue;
			}
			m_Particle.erase(m_Particle.begin() + iParticle);
			m_iEffectCount--;
		}
	}
}

void ParticleSystem::EffectLifeTimeCheck()
{
	// ����Ʈ �ð��� -1�̸� ���� �ݺ�
	if (m_fLifeTime == -1)
		return;

	TVector4 vColor(0.0f, 0.0f, 0.0f, 0.0f);

	for (int iParticle = 0; iParticle < m_iEffectCount; iParticle++)
	{
		float time = (m_fLifeTime / 2.0f) * g_fSecPerFrame;
		// ElapseTime�� ���� ������ �ٸ��� ����Ǿ���Ѵ�. 
		// m_EffectTime�� ���� SecTime�� ������Ų ������ ó������!
		// ������Ÿ���� 1���� Ŭ ��츸 ����
		if (m_fLifeTime >= 1.0f)
		{
			// ����
			if (m_fEffectElapseTime >= m_fLifeTime - 1.5f)
			{
				D3DXVec4Lerp(&m_Particle[iParticle].m_vColor,
					&m_Particle[iParticle].m_vColor,
					&vColor,
					time);
			}
		}

		// ����
		if (m_fEffectElapseTime >= m_fLifeTime)
		{
			m_Particle.erase(m_Particle.begin() + iParticle);
			m_iEffectCount--;
		}
	}
}

bool ParticleSystem::Init()
{
	HRESULT hr = S_OK;
	//srand(time(NULL));

	// ���ĺ���
	D3D11_BLEND_DESC bd;
	//ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.IndependentBlendEnable = FALSE;
	bd.AlphaToCoverageEnable = FALSE;
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;//D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	if (FAILED(hr = m_pd3dDevice->CreateBlendState(&bd, m_pAlphaBlend.GetAddressOf())))
	{
		return false;
	}

	// ���� �׽���
	D3D11_BLEND_DESC bdTesting;
	//ZeroMemory(&bdTesting, sizeof(D3D11_BLEND_DESC));
	bdTesting.IndependentBlendEnable = FALSE;
	bdTesting.AlphaToCoverageEnable = TRUE;
	bdTesting.RenderTarget[0].BlendEnable = TRUE;
	bdTesting.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bdTesting.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;//D3D11_BLEND_SRC_ALPHA;
	bdTesting.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	bdTesting.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bdTesting.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bdTesting.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bdTesting.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	if (FAILED(hr = m_pd3dDevice->CreateBlendState(&bdTesting, m_pAlphaTesting.GetAddressOf())))
	{
		return false;
	}
	return true;
}

bool ParticleSystem::Frame()
{
	m_fEffectElapseTime += g_fSecPerFrame;
	if (m_iRow >= m_iCol)
		m_Effect.SetRect(m_iRow, m_iCol, (float)m_iRow / 2);
	else
		m_Effect.SetRect(m_iRow, m_iCol, (float)m_iCol / 2);
	ParticleUpdate();

	// ����Ʈ �ð� üũ
	EffectLifeTimeCheck();

	// ����Ʈ �ڽ� ���� �Ѿ�� ����
	CheckEffectRangeFromBox();
	return true;
}

bool ParticleSystem::Render()
{
	// ���ø� ������Ʈ / �������ٽ� ����
	m_pImmediateContext->PSSetSamplers(0, 1, &BDxState::g_pSSWrapLinear);
	m_pImmediateContext->OMSetDepthStencilState(BDxState::g_pDSSDepthEnableNoWrite, 0);

	// ���� ���� / �׽��� ����
	if (m_bAlphaBlending == TRUE)
		m_pImmediateContext->OMSetBlendState(m_pAlphaBlend.Get(), 0, -1);
	if (m_bAlphaTesting == TRUE)
		m_pImmediateContext->OMSetBlendState(m_pAlphaTesting.Get(), 0, -1);

	m_pImmediateContext->VSSetShader(m_Effect.m_pVS, NULL, 0);
	m_pImmediateContext->PSSetShader(m_Effect.m_pPS, NULL, 0);
	//m_pImmediateContext->GSSetShader(m_Effect->m_pGS, NULL, 0);
	m_pImmediateContext->IASetInputLayout(m_pLayout.Get());

	ID3D11Buffer* pBuffer[2] = { m_Effect.m_pVertexBuffer, m_pInstanceBuffer.Get() };

	UINT stride[2] = { sizeof(PNCT_Vertex), sizeof(Instance) };
	UINT offset[2] = { 0, 0 };

	m_pImmediateContext->PSSetShaderResources(0, m_Effect.m_iTextureCount, m_Effect.m_Texture.m_pTextureSRV.at(0).GetAddressOf());
	m_pImmediateContext->UpdateSubresource(m_Effect.m_pConstantBuffer, 0, NULL, &m_Effect.m_cbData, 0, 0);
	m_pImmediateContext->UpdateSubresource(m_pInstanceBuffer.Get(), 0, NULL, &m_InstDataArray.at(0), 0, 0);
	// �ΰ��� ���۰� �Ѿ��.
	m_pImmediateContext->IASetVertexBuffers(0, 2, pBuffer, stride, offset);
	m_pImmediateContext->IASetIndexBuffer(m_Effect.m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_Effect.m_pConstantBuffer);
	m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_Effect.m_pConstantBuffer);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pImmediateContext->DrawIndexedInstanced(m_Effect.m_IndexList.size(), m_iEffectCount, 0, 0, 0);

	// ���� / �������ٽ� ����
	m_pImmediateContext->OMSetBlendState(0, 0, -1);
	m_pImmediateContext->OMSetDepthStencilState(BDxState::g_pDSSDepthEnable, 0);
	return true;
}

bool ParticleSystem::Release()
{
	DeleteVector(m_InstDataArray);
	DeleteVector(m_Particle);
	m_Effect.Release();
	return true;
}

ParticleSystem::ParticleSystem()
{
	m_iEffectCount		= 0;
	m_EffectType		= EFFECT_TYPE::Straight;
	m_CoordinateType	= COORDINATE_TYPE::RotationZ;
	m_iRow				= 1;
	m_iCol				= 1;
	m_fLifeTime			= 1.0f;
	m_szParticleName	= NULL;
	m_iIndex			= 0;
	m_bAlphaBlending	= FALSE;
	m_bAlphaTesting		= FALSE;
	m_fEffectElapseTime = 0.0f;
	m_bBillboard		= TRUE;
	m_bPerspective		= TRUE;
	m_bOrthogonal		= FALSE;
	m_bRepeatEffect		= FALSE;
	m_bRepetEffectSet	= TRUE;
	m_bRotationEffect	= FALSE; 
}


ParticleSystem::~ParticleSystem()
{
}


TVector3 operator*(TVector3 v1, TVector3 v2)
{
	TVector3 vec;
	vec.x = v1.x * v2.x;
	vec.y = v1.y * v2.y;
	vec.z = v1.z * v2.z;

	return vec;
}