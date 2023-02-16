#pragma once

#include "LAParticleRender.h"

class LAParticleKnife : public LAParticleRender
{
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
	LAParticleKnife();
	virtual ~LAParticleKnife();
};

