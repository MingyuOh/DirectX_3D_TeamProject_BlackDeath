#include "LAParticleKnife.h"

void LAParticleKnife::ParticleInit(ObjectTableLoadData& rTableLoadData)
{
	m_LoadTableData = rTableLoadData;
}

void LAParticleKnife::ParticleFrame()
{

}

void LAParticleKnife::ParticleRender()
{

}

void LAParticleKnife::ParticleRelease()
{

}

void LAParticleKnife::DrawParticle()
{
	//��ƼŬ ȣ��
}

void LAParticleKnife::Shot()
{

}

void LAParticleKnife::SetParticlePos(TVector3 vParticlePos)
{
	m_vParticlePos = vParticlePos;
}

void LAParticleKnife::SetParticleDir(TVector3 vParticleDir)
{
	m_vParticleDir = vParticleDir;
}

void LAParticleKnife::ReloadBullet(int iBulletCount)
{

}

LAParticleKnife::LAParticleKnife()
{
}


LAParticleKnife::~LAParticleKnife()
{
}
