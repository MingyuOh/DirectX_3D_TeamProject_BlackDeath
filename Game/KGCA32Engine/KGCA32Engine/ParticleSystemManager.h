#pragma once
#include "ParticleSystem.h"

class ParticleSystemManager : public BSingleton<ParticleSystemManager>
{
private:
	friend class BSingleton<ParticleSystemManager>;
public:
	map<INT, ParticleSystem*>					m_ParticleList;
	map<INT, ParticleSystem*>::iterator			m_Itor;
	vector<TCHAR*>								m_ParticleNameList;
	vector<TCHAR*>::iterator					m_ParticleNameItor;
	INT											m_iCurIndex;
public:
	INT											Add(ParticleSystem* newParticle);
	ParticleSystem* 							GetPtr(INT iIndex);
	ParticleSystem 								GetValue(INT iIndex);
	bool										Release();
public:
	ParticleSystemManager();
	virtual ~ParticleSystemManager();
};
#define I_ParticleSystem ParticleSystemManager::GetInstance()

