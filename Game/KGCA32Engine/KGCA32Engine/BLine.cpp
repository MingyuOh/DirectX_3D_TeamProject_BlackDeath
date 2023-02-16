#include "BLine.h"

/*레이아웃으로 인해 재정의*/
HRESULT BLine::LoadShaderAndInputLayout(const TCHAR* pShaderFile)
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
		//MessageBox(NULL, buffer, _T("VS"), MB_OK);
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
		{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT iSize = sizeof(layout) / sizeof(layout[0]);

	S_RETURN(m_pd3dDevice->CreateInputLayout(layout, iSize, pVSBUF->GetBufferPointer(), pVSBUF->GetBufferSize(), &m_pVertexLayout)); // m_pVertexLayout 제작

	S_RELEASE(pVSBUF);
	S_RELEASE(pPSBUF);
	S_RELEASE(pGSBUF);
	return hr;
}

HRESULT BLine::CreateVertexBuffer()		// 버텍스 버퍼 생성
{
	HRESULT hr = S_OK;
	m_LineVertexList.resize(2);
	m_LineVertexList[0].Position = TVector3(0.0f, 0.0f, 0.0f);
	m_LineVertexList[0].Color = TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_LineVertexList[1].Position = TVector3(1.0f, 0.0f, 0.0f);
	m_LineVertexList[1].Color = TVector4(1.0f, 0.0f, 0.0f, 1.0f);

	m_VertexCnt = m_LineVertexList.size();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;						// [메모리 할당 위치] GPU 메모리에 할당해라
	bd.ByteWidth = sizeof(PNCT_Vertex) * m_VertexCnt;	// [메모리 할당 크기] 점 X개를 할당 할 것이기 때문에 *X ( 수가 늘어나면 증가 할 것이다 )
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// [메모리 사용 용도] 버텍스 버퍼용으로 사용하겠다.
	bd.CPUAccessFlags = 0;								// CPU로 부터는 액세스 하지 않겠다.
	bd.MiscFlags = 0;									// 추가 설정 없음

	D3D11_SUBRESOURCE_DATA InitData;			// 소스 리소스 초기화에 필요한 데이터 지정
	InitData.pSysMem = (void*)&m_LineVertexList.at(0);				// 해당 버텍스를 소스로 사용하겠다.
	S_RETURN(m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer));
	return hr;
}

HRESULT BLine::CreateIndexBuffer()		// 인덱스 버퍼 생성
{
	HRESULT hr = S_OK;

	/* 정점 버퍼에 들어있는 인덱스를 읽는 순서의 구조체 */
	m_IndexList.resize(2);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; m_IndexList[iIndex++] = 1;

	UINT iNumIndex = m_IndexList.size();

	// 인덱스 버퍼 생성
	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth = iNumIndex * sizeof(DWORD);
	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // 인덱스 버퍼로 사용하겠다는 선언
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA ibInitData;
	ZeroMemory(&ibInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	ibInitData.pSysMem = (void*)&m_IndexList.at(0);
	S_RETURN(m_pd3dDevice->CreateBuffer(&ibDesc, &ibInitData, &m_pIndexBuffer));

	return hr;
}

bool BLine::SetLineStartToMove(ID3D11DeviceContext* pContext, TVector3 vStart, TVector3 Move, TVector4 vColor)
{
	vector<PNCT_Vertex> LineVertices;
	LineVertices.resize(2);
	LineVertices[0].Position = vStart;
	LineVertices[0].Color = vColor;
	LineVertices[1].Position = vStart + Move;
	LineVertices[1].Color = vColor;

	pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, (void*)&LineVertices.at(0), 0, 0);
	return true;
}

bool BLine::SetLineStartToEnd(ID3D11DeviceContext* pContext, TVector3 vStart, TVector3 vEnd, TVector4 vColor)
{
	vector<PNCT_Vertex> LineVertices;
	LineVertices.resize(2);
	LineVertices[0].Position = vStart;
	LineVertices[0].Color = vColor;
	LineVertices[1].Position = vEnd;
	LineVertices[1].Color = vColor;

	pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, (void*)&LineVertices.at(0), 0, 0);
	return true;
}

BLine::BLine()
{
}


BLine::~BLine()
{
}
