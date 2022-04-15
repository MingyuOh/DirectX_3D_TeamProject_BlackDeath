#pragma once

#include "StandardLink.h"
#include "LABoneObject.h"
#include "LASkinObject.h"

class ItemResource
{
public:
	std::wstring m_MatrixName;
	std::wstring m_ItemName;
	//SceneData m_CharacterSceneData;
	//std::vector<LABoneObject*> m_BoneObjectList;
	std::vector<LASkinObject*> m_SkinObjectList;
	std::vector<CheckObject> m_SkinRenderList;
public:
	ItemResource();
	~ItemResource();
};

