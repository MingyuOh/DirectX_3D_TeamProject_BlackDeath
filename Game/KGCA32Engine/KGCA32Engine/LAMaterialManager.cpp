
//#include "stdafx.h"

#include "LACoreLinkStd.h"
#include "LAMaterialManager.h"

HRESULT LAMaterialManager::ConvertTextureResource(ID3D11Device* pDevice)
{
	HRESULT hr = S_OK;
	
	if (m_MaterialList.size() <= 0)
		return S_FALSE;

	if (pDevice == NULL)
		return S_FALSE;

	for (m_Materialiterator = m_MaterialList.begin(); m_Materialiterator != m_MaterialList.end(); m_Materialiterator++)
	{
		LAMaterialData* pRoot = (m_Materialiterator->second).get();

		for (int iCnt = 0; iCnt < (int)pRoot->m_SubMaterialArray.size(); iCnt++)
		{
			for(int jCnt = 0; jCnt < (int)pRoot->m_SubMaterialArray[iCnt].MapData.size(); jCnt++)
			{
				if (pRoot->m_SubMaterialArray[iCnt].MapData[jCnt].iTypeNumber != -1)
				{
					if (pRoot->m_SubMaterialArray[iCnt].MapData[jCnt].texture.m_pTextureSRV[0] == NULL)
					{
#ifdef TOOL_MODE

						std::wstring dir = L"../data/texture/";
#else
						std::wstring dir = L"../../data/CharacterLoadFile/texture/";
#endif
						dir += pRoot->m_SubMaterialArray[iCnt].MapData[jCnt].pBitmapName;

						string ext = GetFileExtension((TCHAR*)pRoot->m_SubMaterialArray[iCnt].MapData[jCnt].pBitmapName.c_str());
						pRoot->m_SubMaterialArray[iCnt].MapData[jCnt].texture.LoadTexture(dir, mtw(ext));
					}
				}
			}
		}

	}


	//for (int iCnt = 0; iCnt < (int)MaterialList.size(); iCnt++)
	//{
	//	for (int jCnt = 0; jCnt < (int)MaterialList[iCnt].m_SubMaterialArray.size(); jCnt++)
	//	{
	//		for (int kCnt = 0; kCnt < (int)MaterialList[iCnt].m_SubMaterialArray[jCnt].MapData.size(); kCnt++)
	//		{
	//			if (MaterialList[iCnt].m_SubMaterialArray[jCnt].MapData[kCnt].iTypeNumber < 0)
	//				continue;
	//			std::wstring dir = L"../data/texture/";
	//			dir += MaterialList[iCnt].m_SubMaterialArray[jCnt].MapData[kCnt].pBitmapName;

	//			/*hr = D3DX11CreateShaderResourceViewFromFile(m_pCoreDevice, dir.c_str(), NULL, NULL,
	//			m_AseMaterialDataList[iCnt].SubData[jCnt].MapData[kCnt].pTextureSRV.GetAddressOf(), NULL);*/
	//			hr = D3DX11CreateShaderResourceViewFromFile(pDevice, dir.c_str(), NULL, NULL,
	//				MaterialList[iCnt].m_SubMaterialArray[jCnt].MapData[kCnt].pTextureSRV.GetAddressOf(), NULL);

	//			if (FAILED(hr))
	//			{
	//				return S_FALSE;
	//			}
	//		}

	//	}

	//}

	return S_OK;
}

int LAMaterialManager :: AddMaterial(LAMaterialData& pMaterial)
{
	m_MaterialList.insert(std::make_pair(m_NumTotalIndex++, std::make_shared<LAMaterialData>(pMaterial)));

	return m_NumTotalIndex - 1;
}

int LAMaterialManager::AddSubMaterial(int iRootMaterialindex, SubMaterialData& pSubMaterial)
{
	LAMaterialData* pSub = GetRootMaterial(iRootMaterialindex);

	pSub->m_SubMaterialArray.push_back(pSubMaterial);
	
	return pSub->m_SubMaterialArray.size() - 1;
}

LAMaterialData* LAMaterialManager ::GetRootMaterial(int iMaterialindex)
{
	if (iMaterialindex == -1)
	{
		return NULL;
	}

	m_Materialiterator =  m_MaterialList.find(iMaterialindex);

	//¾ø´Ù¸é 
	if (m_Materialiterator == m_MaterialList.end())
	{
		int inum;
		
		LAMaterialData* pData = new LAMaterialData;
		inum =  AddMaterial(*pData);

		m_Materialiterator = m_MaterialList.find(inum);

		return (m_Materialiterator->second).get();
	}

	return (m_Materialiterator->second).get();
}

SubMaterialData* LAMaterialManager::GetSubMaterial(int iRootMaterialindex, int iSubMaterialIndex)
{
	LAMaterialData* pRootData = NULL;

	pRootData = GetRootMaterial(iRootMaterialindex);

	if (pRootData == NULL)
		return NULL;

	std::vector<SubMaterialData>& pSubData = pRootData->m_SubMaterialArray;

	int iCount = pSubData.size();

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		if (iSubMaterialIndex == pSubData[iCnt].iSubMaterialNumber)
		{
			return &pSubData[iCnt];
		}
	}

	return NULL;
}

bool LAMaterialManager :: Release()
{
	for (m_Materialiterator = m_MaterialList.begin(); m_Materialiterator != m_MaterialList.end(); m_Materialiterator++)
	{
		(m_Materialiterator->second).reset();
	}
	m_MaterialList.clear();
	m_NumTotalIndex = 0;

	return true;
}

LAMaterialManager::~LAMaterialManager()
{
}
