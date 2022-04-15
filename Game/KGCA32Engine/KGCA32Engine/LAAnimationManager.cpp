
//#include "stdafx.h"


#include "LAAnimationManager.h"

int LAAnimationManager::AddAnimaion(LAAnimaionData& rAnimation)
{
	m_AnimationList.insert(std::make_pair(m_NumTotalIndex++, std::make_shared<LAAnimaionData>(rAnimation)));

	return m_NumTotalIndex - 1;
}

LAAnimaionData* LAAnimationManager::GetAnimaion(int iAnimationindex)
{
	if (iAnimationindex == -1)
	{
		return NULL;
	}

	m_Animationiterator = m_AnimationList.find(iAnimationindex);

	return (m_Animationiterator->second).get();
}

bool LAAnimationManager::Release()
{
	for (m_Animationiterator = m_AnimationList.begin(); m_Animationiterator != m_AnimationList.end(); m_Animationiterator++)
	{
		(m_Animationiterator->second).reset();
	}
	m_AnimationList.clear();
	m_NumTotalIndex = 0;
	
	return true;
}


LAAnimationManager::~LAAnimationManager()
{
}
