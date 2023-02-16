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
	int m_iMinSplitDistance; // �ּҺ���ũ��
	DWORD m_dwWidth;
	DWORD m_dwHeight;
	bool m_bSharedVertexBuffer; // �������� ���ۻ��

	ComPtr<ID3D11Buffer> m_pTileIndexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	size_t m_dstibOffset;
	size_t m_ibOffset;
	
	ID3D11DeviceContext* m_pImmediateContext;

	/*LOD Index�� ����*/
	vector<BStaticData*> m_pdwLodIndexArray;

	// Spreating�� ����
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
	virtual void ComputeBoundingBox(BNode* pNode); // ����� �ٿ���ڽ��� �ڳʰ��� ä���ִ� �Լ�
	TVector2 GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR); // �ش� ������ �ִ밪�� �ּҰ��� �޾ƿ��� �Լ�
	
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

	/*LOD Index�� �Լ�*/
	HRESULT CreateLODIndexBuffer(BStaticData* pData);
	DWORD GetIndexCounter(DWORD dwData, DWORD dwNumCell);
	virtual bool PreFrame();
	bool Frame(BCamera* pCamera);
	int UpdateStaticIndexList(BNode* pNode, DWORD dwCurrentIndex, DWORD dwLod);
	int GetIndex(DWORD dwIndex, DWORD dwTL);
	
	/*�������� �����Լ�*/
	HRESULT CreateSpreatingTexture(); //�������ÿ� �ؽ��� ����
	void Spreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, TVector3 CrashPoint, float fRadius = 1.0f, int iColor = 0); // ��������
	HRESULT SplattingTextureLoad(ID3D11Device* pDevice, const TCHAR* strFilePath, int iColor);

	// �ʼ��� ����
	void ObjSpreating(ID3D11DeviceContext* pImmediateContext, B_BOX Area, TVector3 CrashPoint, float fRadius, int iColor);
	HRESULT CreateSRV(ID3D11Texture2D* SpreatingTex, D3D11_TEXTURE2D_DESC SpreatingDescV);
public:
	BMapTileRender();
	~BMapTileRender();
};
