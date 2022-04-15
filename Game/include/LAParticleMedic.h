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
	void SetParticlePos(D3DXVECTOR3 vParticlePos);
	void SetParticleDir(D3DXVECTOR3 vParticleDir);
	void ReloadBullet(int iBulletCount);
public:
	LAParticleMedic();
	virtual ~LAParticleMedic();
};

