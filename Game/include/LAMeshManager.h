#pragma once

#include "LAPSBSingleton.h"
#include "LAMeshData.h"

class LAMeshManager : public LAPSBSingleton<LAMeshManager>
{
private:
	friend class LAPSBSingleton<LAMeshManager>;
private:
	int m_NumTotalIndex;
	std::map<int, std::shared_ptr<LAMeshData>> m_MeshList;
	std::map<int, std::shared_ptr<LAMeshData>>::iterator m_Meshiterator;
public:
	int AddMesh(LAMeshData& pMesh);
	LAMeshData* GetMesh(int iMeshindex);
	bool Release();
private:
	LAMeshManager()
	{
		m_NumTotalIndex = 0;
	}
public:
	virtual ~LAMeshManager();
};


#define MESH_MGR LAMeshManager::GetInstance()