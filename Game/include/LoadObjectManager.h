#pragma once

#include "PSBSingleton.h"
#include "StandardLink.h"
#include "LALoadObject.h"
#include "PSBObject.h"

class LoadObjectManager : public PSBSingleton<LoadObjectManager>
{
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	CrateObjectDest m_BoneShaderDest;
	CrateObjectDest m_SkinShaderDest;
	int iCount;
	friend class PSBSingleton<LoadObjectManager>;
private:
	std::map < int, std::shared_ptr<PSBObject>> m_PsbObjectList;
	std::map < int, std::shared_ptr<PSBObject>>::iterator m_PsbObjectListItor;
public:
	PSBObject* CreateObject(std::wstring wsObjectName, E_LoadObjectType eCreateType, CrateObjectDest* pMatrixDest = NULL, CrateObjectDest* pSkinDest = NULL);
	PSBObject* GetCharacter(int iPsbObjectIndex);
public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool Frame();
	bool Render();
	bool Release();
public:
	LoadObjectManager()
	{
		iCount = 0;
	}
	virtual ~LoadObjectManager();
};

#define LOAD_OBJECT_MGR LoadObjectManager::GetInstance()