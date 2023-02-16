
//#include "stdafx.h"


#include "LAMeshManager.h"


int LAMeshManager::AddMesh(LAMeshData& pMesh)
{
	m_MeshList.insert(std::make_pair(m_NumTotalIndex++, std::make_shared<LAMeshData>(pMesh)));

	return m_NumTotalIndex - 1;
}

LAMeshData* LAMeshManager::GetMesh(int iMeshindex)
{
	if (iMeshindex == -1)
	{
		return NULL;
	}

	m_Meshiterator = m_MeshList.find(iMeshindex);

	return (m_Meshiterator->second).get();
}

bool LAMeshManager::Release()
{
	for (m_Meshiterator = m_MeshList.begin(); m_Meshiterator != m_MeshList.end(); m_Meshiterator++)
	{
		(m_Meshiterator->second).reset();
	}
	m_MeshList.clear();
	m_NumTotalIndex = 0;

	return true;
}


LAMeshManager::~LAMeshManager()
{
}
