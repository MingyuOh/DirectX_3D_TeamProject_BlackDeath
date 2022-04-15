#pragma once

#include "PSBSingleton.h"
#include "StandardLink.h"

#include "LoadObjectManager.h"
#include "CharacterManager.h"
#include "ItemManager.h"

class ManagerOfManager : public PSBSingleton<ManagerOfManager>
{
private:
	friend class PSBSingleton<ManagerOfManager>;
public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool Frame();
	bool Render();
	bool Release();
public:
	ManagerOfManager();
	virtual~ManagerOfManager();
};

#define MGR_OF_MGR ManagerOfManager::GetInstance()