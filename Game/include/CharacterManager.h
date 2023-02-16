#pragma once

#include "PSBSingleton.h"
#include "CharacterResource.h"

class CharacterManager : public PSBSingleton<CharacterManager>
{
private:
	friend class PSBSingleton<CharacterManager>;
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	std::map < std::wstring, std::shared_ptr<CharacterResource>> m_CharacterList;
	std::map < std::wstring, std::shared_ptr<CharacterResource>>::iterator m_CharacterListItor;
public:
	bool AddCharacter(CharacterResource& rResource);
	CharacterResource* GetCharacter(std::wstring wsCharacterName);
public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool Frame();
	bool Render();
	bool Release();
public:
	CharacterManager();
	virtual~CharacterManager();
};

#define CHARACTER_MGR CharacterManager::GetInstance()