#include "ParticleSystemManager.h"

INT	ParticleSystemManager::Add(ParticleSystem* newParticle)
{
	INT iCnt = 0;
	for (m_ParticleNameItor = m_ParticleNameList.begin(); 
		m_ParticleNameItor != m_ParticleNameList.end(); 
		m_ParticleNameItor++, iCnt++)
	{
		TCHAR* pName = (TCHAR*)(*m_ParticleNameItor);
		if (!_tcsicmp(pName, newParticle->m_szParticleName))
		{
			return iCnt;
		}
	}
	
	ParticleSystem *pPoint = NULL;
	S_NEW(pPoint, ParticleSystem);
	assert(pPoint);
	*pPoint = *newParticle;
	m_ParticleList.insert(make_pair(++m_iCurIndex, pPoint));
	m_ParticleNameList.push_back(newParticle->m_szParticleName);

	return m_iCurIndex;
}

ParticleSystem* ParticleSystemManager::GetPtr(INT iIndex)
{
	m_Itor = m_ParticleList.find(iIndex);
	if (m_Itor == m_ParticleList.end())
		return NULL;
	ParticleSystem* pPoint = (*m_Itor).second;
	return pPoint;
}

ParticleSystem ParticleSystemManager::GetValue(INT iIndex)
{
	m_Itor = m_ParticleList.find(iIndex);
	ParticleSystem* pPoint = NULL;
	if (m_Itor == m_ParticleList.end())
		return *pPoint;
	pPoint = (*m_Itor).second;
	return *pPoint;
}

bool ParticleSystemManager::Release()
{
	ParticleSystem* pPoint;
	for (m_Itor = m_ParticleList.begin(); m_Itor != m_ParticleList.end(); m_Itor++)
	{
		pPoint = (*m_Itor).second;
		pPoint->Release();
		S_DEL(pPoint);
	}
	m_ParticleList.clear();
	m_iCurIndex = 0;
	return true;
}

ParticleSystemManager::ParticleSystemManager()
{
	m_iCurIndex = -1;
	m_ParticleList.clear();
}


ParticleSystemManager::~ParticleSystemManager()
{
}
