#pragma once

#include "CoreStandardLink.h"
#include "LACharacterStruct.h"

class LASkill
{
public:
	SkillTableLoadData m_SkillTableLoadData;
	std::vector<int> m_SkillSoundIndex;
//public:
//	virtual bool SkillInit();
//	virtual bool SkillFrame();
//	virtual bool SkillRender();
//	virtual bool SkillRelease();
public:
	LASkill();
	virtual ~LASkill();
};

