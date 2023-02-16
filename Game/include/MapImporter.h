#pragma once
#include "BMapTileRender.h"
#include "BWaterMap.h"
#include "BAseObj.h"

class MapImporter
{
public:
	FILE* m_pStream;
	fpos_t m_FilePosition;
	TCHAR m_pBuffer[256];
	TCHAR m_pString[256];
	TCHAR m_pSplatting[4][256];
	TCHAR m_WaterMapTex[256];
	TCHAR m_WaterNormalMapTex[256];


	ID3D11DeviceContext* m_pImmediateContext;
	ID3D11Device* m_pd3dDevice;
	BCamera* m_MainCamera;
	ID3D11Texture2D* StagingTexTure;

	int iCellCount;
	int iPatchCount;
	float fCellDistance;
	TCHAR MapTexture[256];

	BAseObj* m_MapObj;


public:
	bool OpenStream(const TCHAR* pFileName, ID3D11Device* pd3dDevice, ID3D11DeviceContext* pImmediateContext, BCamera* MainCamera);
	bool LoadMap(vector<BMap*>* m_vMFCMap, vector<BMapDesc*>* m_vMFCMapInfo, vector<BMapTileRender*>* m_vMapQuadTree, vector<TCHAR*>* m_vMapTexture, map<int, TCHAR*>* m_mMapSplattingTexture, vector<BWaterMap*>* m_vWaterQuadTree);
	ID3D11Texture2D* LoadAlphaTexture(TCHAR* texture, BMap* Map);
	ID3D11Texture2D* LoadSpawnTexture(TCHAR* texture, BMap* Map);
	void GetHeightFromTex(BNode* Node);
	bool LoadMapObj(vector<BAseObj*>* MapObjmgr);

public:
	MapImporter();
	~MapImporter();
};

