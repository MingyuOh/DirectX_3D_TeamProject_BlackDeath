#pragma once
#include "BQuadTree.h"
#include "BNMap.h"

typedef vector<DWORD> DWORD_VECTOR;
class BStaticData
{
public:
	DWORD dwLevel;
	vector<DWORD_VECTOR> IndexList;
	vector<ComPtr<ID3D11Buffer>> IndexBufferList;
public:
	BStaticData();
	~BStaticData();
};


class BMapTileRender : public BQuadTree
{
public:
	BNMap* m_pMap;
	PNCT_Vertex* m_pVertexList;
	vector<DWORD> m_IndexList;
	int m_iNumFace;
	int m_iMinSplitDistance; // 최소분할크기
	DWORD m_dwWidth;
	DWORD m_dwHeight;
	bool m_bSharedVertexBuffer; // 공유정점 버퍼사용

	ComPtr<ID3D11Buffer> m_pTileIndexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	size_t m_dstibOffset;
	size_t m_ibOffset;
	
	ID3D11DeviceContext* m_pImmediateContext;

	/*LOD Index용 변수*/
	vector<BStaticData*> m_pdwLodIndexArray;

	// Spreating용 변수
	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D* StagingTexTure;
	ID3D11Texture2D* SpreatingTexTure;

	D3D11_TEXTURE2D_DESC Objdesc;
	ID3D11Texture2D* StagingObjPosTexture;
	ID3D11Texture2D* ObjPosTexture;

	/*ComPtr<ID3D11ShaderResourceView> SpreatingSRV;
	ComPtr<ID3D11ShaderResourceView> SpreatingObjSRV;
	map<int, ComPtr<ID3D11ShaderResourceView>> m_pSplattingSRV;*/
	Texture Spreating;
	Texture SpreatingObj;
	map<int, Texture> m_pSplatting;

	map<int, TCHAR*> m_pSplattingTexName;
public:		   
	bool Build(BMap* pMap, DWORD dwWidth, DWORD dwHeight, ID3D11DeviceContext* pImmediateContext);
	BNode* CreateNode(BNode* pParentNode, DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	virtual void ComputeBoundingBox(BNode* pNode); // 노드의 바운딩박스와 코너값을 채워주는 함수
	TVector2 GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR); // 해당 구역의 최대값과 최소값을 받아오는 함수
	
	ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pd3dDevice, void *indices, UINT iNumIndex, UINT iSize);
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pd3dDevice, void *indices, UINT iNumIndex, UINT iSize);

	void UpdateBoundingBox(BNode* pNode);
	bool SubDivide(BNode* pNode);

	virtual bool Render(ID3D11DeviceContext* pContext);
	bool AlphaRender(ID3D11DeviceContext* pContext);
	virtual bool PostRender(ID3D11DeviceContext*    pContext);
	bool OMSetState(ID3D11DeviceContext* pContext, ID3D11RenderTargetView* m_pRTV, ID3D11DepthStencilView* m_pDSV, ID3D11DepthStencilState* m_pDSS, ID3D11BlendState* m_pBS);
	bool CreateVertexList(BNode* pNode);
	int	 CreateIndexList(BNode* pNode);

	void SetRenderDepth(DWORD dwDepth);
	bool ComputeStaticLodIndex(int iMaxCells);

	/*LOD Index용 함수*/
	HRESULT CreateLODIndexBuffer(BStaticData* pData);
	DWORD GetIndexCounter(DWORD dwData, DWORD dwNumCell);
	virtual bool PreFrame();
	bool Frame(BCamera* pCamera);
	int UpdateStaticIndexList(BNode* pNode, DWORD dwCurrentIndex, DWORD dwLod);
	int GetIndex(DWORD dwIndex, DWORD dwTL);
	
	/*스프레팅 관련함수*/
	HRESULT CreateSpreatingTexture(); //스프레팅용 텍스쳐 생성
	void Spreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, TVector3 CrashPoint, float fRadius = 1.0f, int iColor = 0); // 스프레팅
	HRESULT SplattingTextureLoad(ID3D11Device* pDevice, const TCHAR* strFilePath, int iColor);

	// 맵세팅 관련
	void ObjSpreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, TVector3 CrashPoint, float fRadius, int iColor);
	HRESULT CreateSRV(ID3D11Texture2D* SpreatingTex, D3D11_TEXTURE2D_DESC SpreatingDescV);
public:
	BMapTileRender();
	~BMapTileRender();
};

