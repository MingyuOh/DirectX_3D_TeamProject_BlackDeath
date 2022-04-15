#pragma once

#include "CoreStandardLink.h"

#include "LACharacterStruct.h"

#ifdef TOOL_MODE

#else

#include "BBullet.h"
#include "BInput.h"
#include "ParticleSystemManager.h"
#include "ParticleList.h"

using namespace PARTICLE;

#endif


class BMapParser;
class LAParticleRender
{
public:
	ObjectTableLoadData			m_LoadTableData;
	TVector3					m_vParticlePos;
	TVector3					m_vParticleDir;
	BMapParser*					m_pMapParser;
	bool						m_bHeroShot;
public:
	// °øÅë
	virtual void ParticleInit(ObjectTableLoadData& rTableLoadData);
	virtual void ParticleFrame();
	virtual void ParticleRender(); 
	virtual void ParticleRelease(); 
	virtual void DrawParticle();
	virtual void SetParticlePos(TVector3 vParticlePos);
	virtual void SetParticleDir(TVector3 vParticleDir);

	// ¼ÒÃÑ
	virtual void RifleFrame();
	virtual void Shot();
	virtual void ReloadBullet(int iBulletCount);

#ifdef TOOL_MODE

#else
	virtual bool ProcessingCollision(BBoundingBox& bRelativeBox, TVector3& vPos);
#endif
	
	// ¼ö·ùÅº
	virtual void GrenadeFrame();
	virtual TVector3 GetGrenadePosition();
	virtual bool GetCheckBomb();
	virtual void ReloadGrenade(int iGrenadeCount, TVector3 vParticlePos, TVector3 vParticleDir);
	virtual bool CollisionObjectInBombRadian(TVector3 vObjPos);
public:
	LAParticleRender();
	virtual ~LAParticleRender();
};

