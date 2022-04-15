#include "SpriteManager.h"

int SpriteManager::CheckOverlap(TCHAR* spriteName)
{
	std::map<DWORD, Sprite*>::iterator iter;
	for (iter = m_SpriteList.begin(); iter != m_SpriteList.end(); iter++)
	{
		Sprite* pData = iter->second;
		/*if (wcscmp(pData->m_pTextureName, spriteName) == 0)
		{
			return pData->m_iIndex;
		}*/
		return pData->m_iIndex;
	}
}

Sprite* SpriteManager::GetPtr(int iIndex)
{
	std::map<DWORD, Sprite*>::iterator iter;
	iter = m_SpriteList.find(iIndex);
	if (iter == m_SpriteList.end())
		return NULL;
	Sprite *pSprite = iter->second;
	return pSprite;
}

int SpriteManager::LoadSprite(TCHAR* spriteName)
{
	if (CheckOverlap(spriteName) != -1)
		return CheckOverlap(spriteName);
	
	Sprite*	pData = new Sprite;
	if (pData->TextureLoad(this->GetInstance().m_pd3dDevice, spriteName))
	{ 
		m_SpriteList.insert(make_pair(m_iCount++, pData));
	}
	return m_iCount - 1;
}

bool SpriteManager::Release()
{
	std::map<DWORD, Sprite*>::iterator itor;
	for (itor = m_SpriteList.begin(); itor != m_SpriteList.end(); itor++)
	{
		Sprite *rData = itor->second;
		delete rData;
	}
	m_SpriteList.clear();
	return true;
}

SpriteManager::SpriteManager() : m_iCount(0)
{
}


SpriteManager::~SpriteManager()
{
}
