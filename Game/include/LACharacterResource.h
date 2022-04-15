#pragma once

#include "LAStandardLink.h"
#include "LABoneObject.h"
#include "LASkinObject.h"
#include "LACharacterStruct.h"

class LACharacterResource
{
public:
	std::wstring m_CharacterName;
	SceneData m_CharacterSceneData;
	std::vector<LABoneObject*> m_BoneObjectList;
	std::vector<LASkinObject*> m_SkinObjectList;
	std::vector<CheckObject> m_SkinRenderList;
	std::map<int, SectionPlay> m_AnimationList;
public:
	LACharacterResource();
	~LACharacterResource();
};

