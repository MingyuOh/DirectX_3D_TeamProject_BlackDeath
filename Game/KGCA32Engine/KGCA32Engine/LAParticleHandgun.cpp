#include "LAParticleHandgun.h"

#include "LACoreLinkStd.h"

void LAParticleHandgun::ParticleInit(ObjectTableLoadData& rTableLoadData)
{
	m_LoadTableData = rTableLoadData;
}

void LAParticleHandgun::ParticleFrame()
{

}

void LAParticleHandgun::ParticleRender()
{

}

void LAParticleHandgun::ParticleRelease()
{

}

void LAParticleHandgun::DrawParticle()
{
	//파티클 호출
}

void LAParticleHandgun::Shot()
{

}

void LAParticleHandgun::SetParticlePos(TVector3 vParticlePos)
{
	m_vParticlePos = vParticlePos;
}

void LAParticleHandgun::SetParticleDir(TVector3 vParticleDir)
{
	m_vParticleDir = vParticleDir;
}

void LAParticleHandgun::ReloadBullet(int iBulletCount)
{

}

LAParticleHandgun::LAParticleHandgun()
{
}


LAParticleHandgun::~LAParticleHandgun()
{
}
