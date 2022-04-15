#pragma once

#include "LAPSBSingleton.h"
#include "LAMaterialData.h"

class LAMaterialManager : public LAPSBSingleton<LAMaterialManager>
{
private:
	friend class LAPSBSingleton<LAMaterialManager>;
private:
	int m_NumTotalIndex;
	std::map<int, std::shared_ptr<LAMaterialData>> m_MaterialList;
	std::map<int, std::shared_ptr<LAMaterialData>>::iterator m_Materialiterator;
public:
	int AddMaterial(LAMaterialData& pMaterial);
	int AddSubMaterial(int iRootMaterialindex, SubMaterialData& pSubMaterial);
	LAMaterialData* GetRootMaterial(int iMaterialindex);
	SubMaterialData* GetSubMaterial(int iRootMaterialindex, int iSubMaterialIndex);
	HRESULT ConvertTextureResource(ID3D11Device* pDevice);
	bool Release();
private:
	LAMaterialManager()
	{
		m_NumTotalIndex = 0;
	}
public:
	virtual ~LAMaterialManager();
};


#define MATERIAL_MGR LAMaterialManager::GetInstance()
