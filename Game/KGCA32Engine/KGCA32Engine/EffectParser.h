#pragma once
#include "BCore.h"
#include "ParticleSystemManager.h"
#include "ParticleList.h"

using namespace PARTICLE;

class EffectParser
{
public:
	FILE*	m_pStream;
	TCHAR	m_pBuffer[256];
	TCHAR	m_pString[256];

	// ����Ʈ
	vector<ParticleSystem>		m_SystemParticle;
	int							m_iNumParticleSys;
	int							m_iIndexParticleSys; // ��ƼŬ�ý��� �ε���(��ƼŬ�ý��� �Ŵ���)

	// �����
	TVector3					m_vHeroPos;
	TVector3					m_vHeroDir;

	// ����̽�
	ComPtr<ID3D11Device>		m_pd3dDevice;
	ComPtr<ID3D11DeviceContext> m_pImmediateContext;
	BCamera*					m_pCamera;

	// �� ����Ʈ
	bool						m_bSnowDrop;
	int							m_iStageState;
public:
	//--------------------------------------------------------------------------------------
	// ����Ʈ �ε� �Լ�
	//--------------------------------------------------------------------------------------
	bool LoadEffect(TCHAR* szFileName, TCHAR* szParticleName);
public:
	//--------------------------------------------------------------------------------------
	// ĳ���� ������ ���� �Լ�
	//--------------------------------------------------------------------------------------
	void					SetHeroPos(TVector3 vHeroPos);
	//--------------------------------------------------------------------------------------
	// ĳ���� �𷺼� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void					SetHeroDir(TVector3 vHeroDir);
	//--------------------------------------------------------------------------------------
	// �������� ���� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void					SetStageState(int iStageState);
	//--------------------------------------------------------------------------------------
	// ������ �Լ�
	//--------------------------------------------------------------------------------------
	bool					Init();
	bool					PreEffectFrame();
	bool					Frame();
	bool					PostEffectFrame();
	bool					Render();
	bool					Release();
public:
	EffectParser();
	virtual ~EffectParser();
};

