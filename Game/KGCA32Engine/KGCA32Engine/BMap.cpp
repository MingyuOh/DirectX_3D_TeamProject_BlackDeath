#include "BMap.h"

/* 
create & SetMapDesc ���� ��
Render�� ����
*/
bool BMap::UpdateNormal()
{
	InitFaceNormal();
	GenNormalLookupTable();
	CalcPerVertexNormalsFastLookup();
	UpDateTangentBuffer();
	return true;
}

void BMap::CalcVertexColor()
{
	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int  iVertexIndex = iRow * m_iNumCols + iCol;
			m_VertexList[iVertexIndex].Color.x = m_VertexList[iVertexIndex].Normal.x;
			m_VertexList[iVertexIndex].Color.y = m_VertexList[iVertexIndex].Normal.y;
			m_VertexList[iVertexIndex].Color.z = m_VertexList[iVertexIndex].Normal.z;
			m_VertexList[iVertexIndex].Color.w = 1.0f;
		}
	}
}

TVector3 BMap::GetNormalOfVertex(UINT Index)
{
	return m_FaceNormals[Index];
}

float BMap::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart*fTangent) + (fEnd*fTangent);
}

float BMap::GetHeightMap(int row, int col)
{
	return m_VertexList[row * m_iNumRows + col].Position.y;
		//m_fHeightList[row * m_iNumRows + col] * m_MapDesc.fScaleHeight;
}


float BMap::GetHeightMap(float fPosX, float fPosZ)
{
	//fPosX/fPosZ�� ��ġ�� �ش��ϴ� ���� �ʼ��� ã�´�.
	//m_iNumCols�� m_iNumRows�� ���� ������ ���� ũ�Ⱚ
	float fCellX = (float)((float)m_iNumCellCols * (float)m_fCellDistance / 2.0f + fPosX);
	float fCellZ = (float)((float)m_iNumCellRows * (float)m_fCellDistance / 2.0f - fPosZ);

	/*���� ũ��� ������ 1������ ������ �ٲ� ���̸� �迭�� ����*/
	fCellX /= (float)m_fCellDistance;
	fCellZ /= (float)m_fCellDistance;

	// fCellX, fCellZ������ �۰ų� ���� �ִ� ���� (�Ҽ��κ��� �߶󳽴�)
	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);

	// ���̸��� ������ ����� ���� �ʱ�ȭ
	if (fVertexCol < 0.0f) fVertexCol = 0.0f;
	if (fVertexRow < 0.0f) fVertexRow = 0.0f;
	if ((float)(m_iNumCols - 2) < fVertexCol) fVertexCol = (float)(m_iNumCols - 2);
	if ((float)(m_iNumRows - 2) < fVertexRow) fVertexRow = (float)(m_iNumRows - 2);

	// ���� ���� �÷��� �����ϴ� 4�� ������ ���̰��� ã�´�. 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D
	float A = GetHeightMap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightMap((int)fVertexRow, (int)fVertexCol + 1);
	float C = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol);
	float D = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol + 1);

	// A������ ��ġ���� ������ ��(������)�� ����Ѵ�.
	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;
	
	//�����۾��� ���� ���� ���̽��� ã�´�
	float fHeight = 0.0f;
	if (fDeltaZ < (1.0f - fDeltaX))
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	else
	{
		float uy = C - D;
		float vy = B - D;
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}
	return fHeight;
}

HRESULT BMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* m_pImmediateContext, TCHAR** pLoadTextureString, int TextureCnt, TCHAR* HeightMapString, const TCHAR* ShaderString)
{
	m_pd3dDevice = pDevice;
	m_pContext = m_pImmediateContext;

	HRESULT hr = S_OK;
	
	if (HeightMapString != NULL)
	{
		CreateHeightMap(HeightMapString);
	}
	S_RETURN(CreateVertexBuffer());
	S_RETURN(CreateIndexBuffer());
	S_RETURN(LoadShaderAndInputLayout(ShaderString));
	S_RETURN(CreateConstantBuffer());
	if (pLoadTextureString != NULL)
	{
		if (TextureCnt < 2)
		{
			m_MapTexture = pLoadTextureString[0];
			S_RETURN(TextureLoad(pDevice, pLoadTextureString[0]));
		}
		else
		{
			S_RETURN(TextureLoad(pDevice, pLoadTextureString, TextureCnt));
		}
	}
	UpdateMinMaxBox();
	return hr;
}

HRESULT BMap::LoadShaderAndInputLayout(const TCHAR* pShaderFile)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS; // ���̴� ����� ���
	ID3DBlob* pVSBUF = NULL;
	ID3DBlob* pPSBUF = NULL;
	ID3DBlob* pAlphaPSBUF = NULL;
	ID3DBlob* pGSBUF = NULL;

	ID3DBlob* pError = NULL; // �̰��� �������ν� ������ �� ������ �޾� �� �� �ִ�.
							 /* vs_5_0 == direct11 �� 5.0�� ��� * pVSBuf ����� ��� */

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

	if (FAILED(D3DCompileFromFile(pShaderFile, NULL, NULL, "AlphaMap_PS", "ps_5_0", NULL, NULL, &pAlphaPSBUF, &pError)))
	{
		TCHAR buffer[256] = { 0, };
		mbstowcs(buffer, (char*)pError->GetBufferPointer(), 256);
		//MessageBox(NULL, buffer, _T("PS"), MB_OK);
	}
	if (pAlphaPSBUF != nullptr)
	{
		S_RETURN(m_pd3dDevice->CreatePixelShader((DWORD*)pAlphaPSBUF->GetBufferPointer(), pAlphaPSBUF->GetBufferSize(), NULL, &m_pAlphaPS));
	}

	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTURE",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT iSize = sizeof(layout) / sizeof(layout[0]);

	S_RETURN(m_pd3dDevice->CreateInputLayout(layout, iSize, pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), &m_pVertexLayout)); // m_pVertexLayout ����

	S_RELEASE(pVSBUF);
	S_RELEASE(pPSBUF);
	S_RELEASE(pAlphaPSBUF);
	S_RELEASE(pGSBUF);
	return hr;
}


bool BMap::CreateHeightMap(TCHAR* strHeightMapText)
{
	HRESULT hr = S_OK;
	size_t maxsize = 0;
	ComPtr<ID3D11Resource> pResource = NULL;

	if (FAILED(hr = DirectX::CreateWICTextureFromFileEx(g_pd3dDevice,
		strHeightMapText,
		maxsize,
		D3D11_USAGE_STAGING,
		0,
		D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
		0,
		WIC_LOADER_DEFAULT,
		pResource.GetAddressOf(), nullptr)))
	{
		return hr;
	}

	ID3D11Texture2D *pTexture2D = NULL;
	if (FAILED(pTexture2D->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	} // ���ҽ��κ��� �ؽ��ĸ� ����.
	S_RELEASE(pTexture2D);

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);
	m_fHeightList.resize(desc.Height*desc.Width);
	
	if (pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(m_pContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1)/*�ְ� �ػ��� �ؽ���*/, D3D11_MAP_READ, 0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			PNCT_Vertex Vertex;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT RowStart = row * MappedFaceDest.RowPitch;
				for (UINT col = 0; col < desc.Width; col++)
				{
					UINT ColStart = col * 4;
					UINT uRed = pTexels[RowStart + ColStart + 0];
					UINT uGreen = pTexels[RowStart + ColStart + 1];
					UINT uBrue = pTexels[RowStart + ColStart + 2];
					m_fHeightList[row * desc.Width + col] = uRed;	/// DWORD�̹Ƿ� pitch/4	
				}
			}
			m_pContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
		}
	}

	BMapDesc MainMap;
	MainMap.iNumCols = desc.Width;
	MainMap.iNumRows = desc.Height;
	MainMap.fScaleHeight = 0.1f;
	MainMap.fCellDistance = m_fCellDistance; // ���⼭ ����
	SetMapDesc(MainMap);
	S_RELEASE(pTexture2D);
	return true;
}

HRESULT BMap::CreateVertexBuffer()		// ���ؽ� ���� ����
{
	HRESULT hr = S_OK;
	m_VertexList.resize(m_iNumVertices);
	float iHalfCols = (m_iNumCols - 1) / 2;
	float iHalfRows = (m_iNumRows - 1) / 2;
	float ftxOffsetU = 1.0f / (m_iNumCols - 1);
	float ftxOffsetV = 1.0f / (m_iNumRows - 1);

	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			int  iVertexIndex = iRow * m_iNumCols + iCol;
			m_VertexList[iVertexIndex].Position.x = ((float)iCol - iHalfCols) * m_fCellDistance;
			m_VertexList[iVertexIndex].Position.z = -(((float)iRow - iHalfRows) * m_fCellDistance);
			
			if (m_fHeightList.size() != 0)
			{
				m_VertexList[iVertexIndex].Position.y = m_fHeightList[iVertexIndex] * m_MapDesc.fScaleHeight;
			}
			else
			{
				m_VertexList[iVertexIndex].Position.y = 0.0f;
			}
			m_VertexList[iVertexIndex].Normal = TVector3(0.0f, 1.0f, 0.0f);
			m_VertexList[iVertexIndex].Color = TVector4(1, 1, 1, 1.0f);
			m_VertexList[iVertexIndex].Texture = TVector2(ftxOffsetU * iCol, ftxOffsetV * iRow); //x = icol * (1/width -1)   y = iRow * (1/height -1)
		}
	}

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

HRESULT BMap::CreateIndexBuffer()		// �ε��� ���� ����
{
	HRESULT hr = S_OK;
	m_IndexList.resize(m_iNumFace * 3);
	int iCurIndex = 0;
	for (int iRow = 0; iRow < m_iNumCellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCellCols; iCol++)
		{
			//0	1    4   
			//2	   3 5
			int iNextRow = iRow + 1;
			int iNextCol = iCol + 1;
			m_IndexList[iCurIndex + 0] = iRow * m_iNumCols + iCol;
			m_IndexList[iCurIndex + 1] = iRow * m_iNumCols + iNextCol;
			m_IndexList[iCurIndex + 2] = iNextRow * m_iNumCols + iCol;
			m_IndexList[iCurIndex + 3] = m_IndexList[iCurIndex + 2];
			m_IndexList[iCurIndex + 4] = m_IndexList[iCurIndex + 1];
			m_IndexList[iCurIndex + 5] = iNextRow * m_iNumCols + iNextCol;

			iCurIndex += 6;
		}
	}

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

bool BMap::SetMapDesc(BMapDesc MapDesc)
{
	if (MapDesc.iNumCols == 0 || MapDesc.iNumRows == 0)
	{
		MapDesc.iNumCols = MapDesc.iCellCount * MapDesc.iPatchCount + 1;
		MapDesc.iNumRows = MapDesc.iCellCount * MapDesc.iPatchCount + 1;
	}
	/*���� ������ �ִ� ������ ����*/
	if (MapDesc.iNumCols > 1025 || MapDesc.iNumRows > 1025)
	{
		MapDesc.iNumCols = 1025; // �������� 105����,
		MapDesc.iNumRows = 1025; // ���̽� ���� 209���� 
	}

	m_MapDesc = MapDesc;
	m_iNumRows = MapDesc.iNumCols;
	m_iNumCols = MapDesc.iNumRows;
	m_iNumCellRows = m_iNumRows - 1;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumVertices = m_iNumRows*m_iNumCols;
	m_iNumFace = m_iNumCellRows*m_iNumCellCols * 2;
	m_fCellDistance = MapDesc.fCellDistance;
	return true;
}

bool BMap::Frame()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRotate);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matScale);
	return true;
}


bool BMap::ReLoadVBuffer()
{
	m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, (void*)&m_VertexList.at(0), 0, 0);
	return true;
}


BMap::BMap()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRotate);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matScale);
	m_bStaticLight = false;
	m_vLightDir = TVector3(0, -1, 0);
}


BMap::~BMap()
{
}