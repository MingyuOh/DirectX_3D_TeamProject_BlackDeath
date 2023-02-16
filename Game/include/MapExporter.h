#pragma once
#include "BMapTileRender.h"

class MapExporter
{
public:
	FILE* m_pStream;	// 저장할 파일 포인터
	ID3D11DeviceContext* m_pContext;

public:
	bool SetExporter(ID3D11DeviceContext* pContext, const TCHAR* m_szFileName);
	void ExpAlphaMapTex(ID3D11Texture2D* pTextureDest);
	void ExpTexName(TCHAR* BaseTexName, map<int, TCHAR*> pSplattingTexName);
	void ExpMapInfo(BMapDesc* pMapInfo);
	void ExpHeightTotal(BNode* RootNode);
	void ExpHeight(BNode* Node);

public:
	MapExporter();
	~MapExporter();
};

