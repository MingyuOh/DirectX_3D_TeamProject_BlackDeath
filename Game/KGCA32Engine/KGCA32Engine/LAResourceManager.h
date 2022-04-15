#pragma once

#include "LAPSBSingleton.h"
#include "LAResource.h"

class LAResourceManager : public LAPSBSingleton<LAResourceManager>
{
private:
	friend class LAPSBSingleton<LAResourceManager>;
private:
	int m_iResourceCount;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	CrateObjectDest m_SkinShaderDest;
	CrateObjectDest m_BoneShaderDest;
	CrateObjectDest m_BoneNormalMappingShaderDest;
	CrateObjectDest m_SkinNormalMappingShaderDest;
	std::map < int, std::shared_ptr<LAResource>> m_ResourceList;
	std::map < int, std::shared_ptr<LAResource>>::iterator m_ResourceListItor;
public:
	FIlePath m_LAFilePath;
public:
	bool AddResource(LAResource& rResource, CrateObjectDest* pMatrixDest = NULL, CrateObjectDest* pSkinDest = NULL);
	void CreateResource(int iResourceNumber, CrateObjectDest* pMatrixDest, CrateObjectDest* pSkinDest, bool bLoadShadow);
	bool CreateResourceShader(LAResource* pResource, CrateObjectDest* pMatrixDest, CrateObjectDest* pSkinDest);
	int GetResourceIndex(std::wstring codeName, std::wstring matrixname, std::wstring skinName);
	int GetResourceCodeNameIndex(std::wstring codeName);
	int GetResourceMatirxNameIndex(std::wstring matrixname);
	int GetResourceSkinNameIndex(std::wstring skinName);
	int GetResourceCodeNameIndex(int iResourceIndex);
	LAResource* GetResourceAdress(std::wstring wsCharacterName);
	LAResource* GetResourceAdressSkinName(std::wstring wsCharacterName);
	LAResource* GetResourceAdress(int iResourceNumber);
	CrateObjectDest GetCreateObjectDest(E_LoadObjectType e_Type);	//toolMode 에서 사용
public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool Frame();
	bool Render();
	bool Release();
public:
	LAResourceManager();
	virtual~LAResourceManager();
};

#define RESOURCE_MGR LAResourceManager::GetInstance()