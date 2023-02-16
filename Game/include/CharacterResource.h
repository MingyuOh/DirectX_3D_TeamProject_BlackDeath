#pragma once

#include "StandardLink.h"
#include "LABoneObject.h"
#include "LASkinObject.h"
#include "CharacterStruct.h"

class CharacterResource
{
public:
	std::wstring m_CharacterName;
	SceneData m_CharacterSceneData;
	std::vector<LABoneObject*> m_BoneObjectList;
	std::vector<LASkinObject*> m_SkinObjectList;
	std::vector<CheckObject> m_SkinRenderList;
	std::map<int, SectionPlay> m_AnimationList;
	std::vector<MotionKeyInfo> m_MotionKeyStateList;
public:
	CharacterResource();
	~CharacterResource();
};

