#pragma once

#include "CoreStandardLink.h"
#include "CoreFuncLink.h"
#include "LACharacterStruct.h"

class LAMeshData
{
public:
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_SubVertexBufferList;
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_SubNormalMapVertexBufferList;
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_SubIndexBufferList;
	std::vector<SUBVECTOR> m_SubUniqueVectorList;
	std::vector<SUBVECTOR> m_SubVectorList;
	std::vector<SKIN_SUB_VERTEX> m_SubSkinVecterList; 
	std::vector<int> m_NewUniqueVertexListCount;		//�߰�
	std::vector<PNCT5_VERTEX*> m_NewUniqueVecterList;	//�߰�
	std::vector<SKIN_SUB_VERTEX> m_SubSkinUniqueVecterList;
	std::vector<SUBINDEX> m_SubIndexList;
	std::vector<int> m_NewUniqueIndexListCount;	//�߰�
	std::vector<DWORD*> m_NewUniqueIndexList;		//�߰�
	std::vector<int> m_SubTextureNumber;
public:
	LAMeshData()
	{
		
	}
	~LAMeshData();
};

