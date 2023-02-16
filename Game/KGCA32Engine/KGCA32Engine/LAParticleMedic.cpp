#include "LAParticleMedic.h"

#include "LACoreLinkStd.h"

void LAParticleMedic::ParticleInit(ObjectTableLoadData& rTableLoadData)
{
	m_LoadTableData = rTableLoadData;
}

void LAParticleMedic::ParticleFrame()
{
	for (int iParticle = 0; iParticle < m_ParticleSystem.size(); iParticle++)
	{
		m_ParticleSystem[iParticle].SetFrameParticleSystemInfo();
		m_ParticleSystem[iParticle].Frame();
	}
}

void LAParticleMedic::ParticleRender()
{
	for (int iParticle = 0; iParticle < m_ParticleSystem.size(); iParticle++)
	{
		m_ParticleSystem[iParticle].Render();
	}
}

void LAParticleMedic::ParticleRelease()
{
	// 이펙트 제거
	for (int iShot = 0; iShot < m_ParticleSystem.size(); iShot++)
	{
		m_ParticleSystem[iShot].Release();
	}
}

void LAParticleMedic::DrawParticle()
{
	//파티클 호출
	ParticleFrame();
	ParticleRender();
}

void LAParticleMedic::Shot()
{
	ParticleSystem addHealEffect, addHealBackgroundEffect;
	addHealEffect = I_ParticleSystem.GetValue(HealingEffect);
	addHealBackgroundEffect = I_ParticleSystem.GetValue(HealingBackground);
	m_ParticleSystem.push_back(addHealEffect);
	m_ParticleSystem.push_back(addHealBackgroundEffect);
}

void LAParticleMedic::SetParticlePos(TVector3 vParticlePos)
{
	m_vParticlePos = vParticlePos;
}

void LAParticleMedic::SetParticleDir(TVector3 vParticleDir)
{
	m_vParticleDir = vParticleDir;
}

void LAParticleMedic::ReloadBullet(int iBulletCount)
{

}

LAParticleMedic::LAParticleMedic()
{
}


LAParticleMedic::~LAParticleMedic()
{
}
