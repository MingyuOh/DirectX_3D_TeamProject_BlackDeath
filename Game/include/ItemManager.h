#pragma once

#include "PSBSingleton.h"
#include "ItemResource.h"

class ItemManager : public PSBSingleton<ItemManager>
{
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	friend class PSBSingleton<ItemManager>;
private:
	std::map < std::wstring, std::shared_ptr<ItemResource>> m_ItemList;
	std::map < std::wstring, std::shared_ptr<ItemResource>>::iterator m_ItemListItor;
public:
	bool AddItem(ItemResource& rResource);
	ItemResource* GetItem(std::wstring wsItemName);
public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool Frame();
	bool Render();
	bool Release();
public:
	ItemManager();
	virtual~ItemManager();
};


#define ITEM_MGR ItemManager::GetInstance()