#pragma once

#include "LAPSBSingleton.h"
#include "LAItemResource.h"

class LAItemManager : public LAPSBSingleton<LAItemManager>
{
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	friend class LAPSBSingleton<LAItemManager>;
private:
	std::map < std::wstring, std::shared_ptr<LAItemResource>> m_ItemList;
	std::map < std::wstring, std::shared_ptr<LAItemResource>>::iterator m_ItemListItor;
public:
	bool AddItem(LAItemResource& rResource);
	LAItemResource* GetItem(std::wstring wsItemName);
public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool Frame();
	bool Render();
	bool Release();
public:
	LAItemManager();
	virtual~LAItemManager();
};


#define ITEM_MGR LAItemManager::GetInstance()