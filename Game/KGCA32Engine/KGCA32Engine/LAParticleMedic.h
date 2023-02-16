#pragma once

#include "LAParticleRender.h"


class LAParticleMedic : public LAParticleRender
{
public:
	vector<ParticleSystem>		m_ParticleSystem;
public:
	void ParticleInit(ObjectTableLoadData& rTableLoadData);
	void ParticleFrame();
	void ParticleRender();
	void ParticleRelease();
	void DrawParticle();
	void Shot();
	void SetParticlePos(TVector3 vParticlePos);
	void SetParticleDir(TVector3 vParticleDir);
	void ReloadBullet(int iBulletCount);
public:
	LAParticleMedic();
	virtual ~LAParticleMedic();
};

