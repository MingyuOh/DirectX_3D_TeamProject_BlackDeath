#pragma once

#include "LAPSBSingleton.h"
#include "CoreStandardLink.h"

#include "LALoadObjectManager.h"
#include "LAResourceManager.h"

class BMapParser;
class LAManagerOfManager : public LAPSBSingleton<LAManagerOfManager>
{
private:
	friend class LAPSBSingleton<LAManagerOfManager>;
public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BMapParser* pMap);
	bool Frame();
	bool Render();
	bool Release();
public:
	LAManagerOfManager();
	virtual~LAManagerOfManager();
};

#define MGR_OF_MGR LAManagerOfManager::GetInstance()