#pragma once

#include "LAPSBSingleton.h"
#include "LAAnimaionData.h"
#include "CoreStandardLink.h"

class LAAnimationManager : public LAPSBSingleton<LAAnimationManager>
{
private:
	friend class LAPSBSingleton<LAAnimationManager>;
private:
	int m_NumTotalIndex;
	std::map<int, std::shared_ptr<LAAnimaionData>> m_AnimationList;
	std::map<int, std::shared_ptr<LAAnimaionData>>::iterator m_Animationiterator;
public:
	int AddAnimaion(LAAnimaionData& rAnimation);
	LAAnimaionData* GetAnimaion(int iAnimationindex);
	bool Release();
private:
	LAAnimationManager()
	{
		m_NumTotalIndex = 0;
	}
public:
	virtual ~LAAnimationManager();
};

#define ANIMATION_MGR LAAnimationManager::GetInstance()
