#include "BDirectionLine.h"

HRESULT BDirectionLine::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	m_LineVertexList.resize(6);
	m_LineVertexList[0].Position = TVector3(0.0f, 0.0f, 0.0f);
	m_LineVertexList[0].Color = TVector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_LineVertexList[1].Position = TVector3(1.0f, 0.0f, 0.0f);
	m_LineVertexList[1].Color = TVector4(1.0f, 0.0f, 0.0f, 1.0f);
	m_LineVertexList[2].Position = TVector3(0.0f, 0.0f, 0.0f);
	m_LineVertexList[2].Color = TVector4(0.0f, 1.0f, 0.0f, 1.0f);
	m_LineVertexList[3].Position = TVector3(0.0f, 1.0f, 0.0f);
	m_LineVertexList[3].Color = TVector4(0.0f, 1.0f, 0.0f, 1.0f);
	m_LineVertexList[4].Position = TVector3(0.0f, 0.0f, 0.0f);
	m_LineVertexList[4].Color = TVector4(0.0f, 0.0f, 1.0f, 1.0f);
	m_LineVertexList[5].Position = TVector3(0.0f, 0.0f, 1.0f);
	m_LineVertexList[5].Color = TVector4(0.0f, 0.0f, 1.0f, 1.0f);

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

HRESULT BDirectionLine::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

	/* 정점 버퍼에 들어있는 인덱스를 읽는 순서의 구조체 */
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; m_IndexList[iIndex++] = 1;
	m_IndexList[iIndex++] = 2; m_IndexList[iIndex++] = 3;
	m_IndexList[iIndex++] = 4; m_IndexList[iIndex++] = 5;

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

bool BDirectionLine::PreRender(ID3D11DeviceContext* pContext)
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
	return true;
}

BDirectionLine::BDirectionLine()
{
	m_iPrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}


BDirectionLine::~BDirectionLine()
{
}
