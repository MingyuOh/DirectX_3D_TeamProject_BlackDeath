#pragma once

#include "CoreStandardLink.h"
#include "LAAnimaionData.h"

enum class E_ObjectType
{
	None = 0,
	Helper_Bone,
	Helper_Dummy,
	Geom_Default,
	Geom_Biped,
	Skin_Object,
	Animation,
};

//enum class E_ObjectType
//{
//	None = 0,
//	HelperObject,
//	GeomObject,
//};

class LAMesh
{
public:
	std::wstring wsNodeName;
	std::wstring wsHelperClassName;
	std::wstring wsParentName;
	E_ObjectType m_ObjectType;
	D3DXMATRIX matLoadObjectWorld;
	
	int m_RootMaterialDataID;
	std::vector<int> m_AnimationIDList;
	std::vector<SceneData> m_SceneDataList;
	int m_MeshDataID;
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;
	//std::vector<LAMesh> m_SubMesh;
public:
	LAMesh()
	{
		m_RootMaterialDataID = -1;
		m_MeshDataID = -1;
		m_ObjectType = E_ObjectType::None;
		D3DXMatrixIdentity(&matLoadObjectWorld);
		ZeroMemory(&vMin, sizeof(D3DXVECTOR3));
		ZeroMemory(&vMax, sizeof(D3DXVECTOR3));
	}
	virtual ~LAMesh();
};

