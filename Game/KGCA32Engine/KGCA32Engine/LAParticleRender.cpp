
//#include "stdafx.h"

#include "LAParticleRender.h"

#include "LACoreLinkStd.h"

//--------------------------------------------------------------------------------------
// 공통
//--------------------------------------------------------------------------------------
void LAParticleRender::ParticleInit(ObjectTableLoadData& rTableLoadData) 
{

}

void LAParticleRender::ParticleFrame()
{

}

void LAParticleRender::ParticleRender()
{

}

void LAParticleRender::ParticleRelease()
{

}

void LAParticleRender::DrawParticle()
{

}

void LAParticleRender::SetParticlePos(TVector3 vParticlePos)
{

}

void LAParticleRender::SetParticleDir(TVector3 vParticleDir)
{

}

//--------------------------------------------------------------------------------------
// 라이플
//--------------------------------------------------------------------------------------
void LAParticleRender::ReloadBullet(int iBulletCount)
{

}

void LAParticleRender::RifleFrame()
{

}

void LAParticleRender::Shot()
{

}

#ifdef TOOL_MODE

#else

bool LAParticleRender::ProcessingCollision(BBoundingBox& bRelativeBox, TVector3& vPos)
{
	return true;
}

#endif



//--------------------------------------------------------------------------------------
// 수류탄
//--------------------------------------------------------------------------------------
void LAParticleRender::ReloadGrenade(int iGrenadeCount, TVector3 vParticlePos, TVector3 vParticleDir)
{

}

void LAParticleRender::GrenadeFrame()
{

}

TVector3 LAParticleRender::GetGrenadePosition()
{
	TVector3 vpos;
	ZeroMemory(&vpos, sizeof(TVector3));

	return vpos;
}

bool LAParticleRender::GetCheckBomb()
{
	return true;
}

bool LAParticleRender::CollisionObjectInBombRadian(TVector3 vObjPos)
{
	return true;
}

LAParticleRender::LAParticleRender()
{
	m_bHeroShot = false;
	m_pMapParser = NULL;
}


LAParticleRender::~LAParticleRender()
{
	m_pMapParser = NULL;
}
