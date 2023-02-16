#pragma once

#include "LAPSBSingleton.h"
#include "LACharacterResource.h"

class LACharacterManager : public LAPSBSingleton<LACharacterManager>
{
private:
	friend class LAPSBSingleton<LACharacterManager>;
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	std::map < std::wstring, std::shared_ptr<LACharacterResource>> m_CharacterList;
	std::map < std::wstring, std::shared_ptr<LACharacterResource>>::iterator m_CharacterListItor;
public:
	bool AddCharacter(LACharacterResource& rResource);
	LACharacterResource* GetCharacter(std::wstring wsCharacterName);
public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool Frame();
	bool Render();
	bool Release();
public:
	LACharacterManager();
	virtual~LACharacterManager();
};

#define CHARACTER_MGR LACharacterManager::GetInstance()