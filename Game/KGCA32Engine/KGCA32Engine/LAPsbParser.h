#pragma once



#include "CoreFuncLink.h"

#include "LALoadObject.h"
#include "LAMaterialData.h"
#include "LAResource.h"


class LAPsbParser
{
public:
	FILE* m_pPsbFile;
	ID3D11Device* m_pCoreDevice;
	ID3D11DeviceContext* m_pCoreContext;
	std::vector<LAMesh> m_pMeshList;
	std::wstring m_Loadmatrixname;
	SceneData m_SceneData;
	bool m_bIsSkining;
	bool m_bIsNormalMapping;
	//Save
	TCHAR m_pBuffer[256];
	TCHAR m_pString[256];

public:
	//Load
	LALoadObject* Convert(int iCodeIndex, std::wstring CodeName, std::wstring wsParentMatrixName, E_LoadObjectType eLoadObjectType, CrateObjectDest* pMatrixDest, CrateObjectDest* pSkinDest, bool bLoadShadow);	//Resource

	void LoadSceneData(E_LoadObjectType eLoadObjectType, bool bBinary);
	void LoadNodeData(LAMesh& pMesh, bool bBinary);
	void LoadMeshData(LAMesh& pMesh, bool bBinary);
	void LoadAnimationData(LAMesh& pMesh, bool bBinary);
	void LoadMaterialData(LAMesh& pMesh, bool bBinary);
	bool LoadData(E_LoadObjectType eLoadObjectType, bool bBinary);
	bool Load(FILE* pStream, E_LoadObjectType eLoadObjectType, bool bBinary);

	void LoadPNCT(std::vector<PNCT_VECTOR>& vVector, int iLoadCount);
	void LoadPNCT(std::vector<PNCT5_VERTEX>& vVector, int iLoadCount, bool bBinary);
	bool SetUniqueBuffer(std::vector<PNCT_VECTOR>& UniqueDataList, std::vector<PNCT_VECTOR>& VertexDataList, std::vector<DWORD>& IndexList);
	bool SetUniqueBuffer(std::vector<PNCT5_VERTEX>& UniqueDataList, std::vector<PNCT5_VERTEX>& VertexDataList, std::vector<DWORD>& IndexList);
	int IsEqualVertexList(std::vector<PNCT_VECTOR>& UniqueDataArray, PNCT_VECTOR& pnctData);
	int IsEqualVertexList(std::vector<PNCT5_VERTEX>& UniqueDataArray, PNCT5_VERTEX& pnctData);
	void SearchToObjectParent(LAResource& rResource);

	//Save
	bool Save(const TCHAR* pFileName, LALoadObject& LoadObjectdata, E_ObjectType ObjectType, bool bBinary);
	bool SaveData(FILE* pFile, std::vector<LAMesh*>& SaveMeshList, E_ObjectType objectType, bool bBinary);
	void SaveSceneData(FILE* pFile, LALoadObject& rMeshData, int iMeshCount, E_ObjectType objectType, bool bBinary);
	//void SaveMatrixObjectData(LAMesh& rMeshData, int iMeshCount, E_ObjectType objectType);
	void SaveNodeData(FILE* pFile, LAMesh& pMesh, int iMeshCount, bool bBinary);
	void SaveMeshData(FILE* pFile, LAMesh& pMesh, E_ObjectType objectType, bool bBinary);
	void SaveMaterialData(FILE* pFile, LAMesh& pMesh, E_ObjectType objectType, bool bBinary);
	void SaveAnimationData(FILE* pFile, LALoadObject& LoadObjectdata, LAMesh& pMesh, bool bBinary);

public:
	bool Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool Release();
public:
	LAPsbParser()
	{
		m_pPsbFile = NULL;
		m_bIsSkining = false;
		m_bIsNormalMapping = false;
		ZeroMemory(m_pBuffer, sizeof(TCHAR) * 256);
		ZeroMemory(m_pString, sizeof(TCHAR) * 256);
	}
	~LAPsbParser();
};

