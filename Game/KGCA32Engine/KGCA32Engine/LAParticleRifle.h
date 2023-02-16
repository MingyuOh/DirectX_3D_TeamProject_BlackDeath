#pragma once

#include "LAParticleRender.h"

class LAParticleRifle : public LAParticleRender
{
public:
	vector<BBullet>				m_Bullet;
	int							m_iShotIndex;
	vector<ParticleSystem>		m_ParticleSystem;
public:
	//--------------------------------------------------------------------------------------
	// ������ �Ѿ� / ��ƼŬ ����
	//--------------------------------------------------------------------------------------
	void						ParticleInit(ObjectTableLoadData& rTableLoadData);
	//--------------------------------------------------------------------------------------
	// ������ ��ƼŬ ������
	//--------------------------------------------------------------------------------------
	void						ParticleFrame();
	//--------------------------------------------------------------------------------------
	// ������ �Ѿ� / ��ƼŬ ������
	//--------------------------------------------------------------------------------------
	void						ParticleRender();
	//--------------------------------------------------------------------------------------
	// ������ �Ѿ� / ��ƼŬ ������
	//--------------------------------------------------------------------------------------
	void						ParticleRelease();
	//--------------------------------------------------------------------------------------
	// ������ ��ƼŬ ������
	//--------------------------------------------------------------------------------------
	void						DrawParticle();
	//--------------------------------------------------------------------------------------
	// ������ �Ѿ� ������
	//--------------------------------------------------------------------------------------
	void						RifleFrame();
	//--------------------------------------------------------------------------------------
	// ������ �߻� �Լ�
	//--------------------------------------------------------------------------------------
	void						Shot();
	//--------------------------------------------------------------------------------------
	// ������ �Ѿ� / ��ƼŬ ��ġ ���� �Լ�
	//--------------------------------------------------------------------------------------
	void						SetParticlePos(TVector3 vParticlePos);
	//--------------------------------------------------------------------------------------
	// ������ �Ѿ� / ��ƼŬ �𷺼� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void						SetParticleDir(TVector3 vParticleDir);
	//--------------------------------------------------------------------------------------
	// ������ �Ѿ� �浹ó�� �Լ�
	//--------------------------------------------------------------------------------------
	bool						ProcessingCollision(BBoundingBox& bRelativeBox, TVector3& vPos);
	//--------------------------------------------------------------------------------------
	// ������ �Ѿ� ������ �Լ�
	//--------------------------------------------------------------------------------------
	void						ReloadBullet(int iBulletCount);
public:
	LAParticleRifle();
	virtual ~LAParticleRifle();
};
