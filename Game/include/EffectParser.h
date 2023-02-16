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

	// 이펙트
	vector<ParticleSystem>		m_SystemParticle;
	int							m_iNumParticleSys;
	int							m_iIndexParticleSys; // 파티클시스템 인덱스(파티클시스템 매니저)

	// 히어로
	D3DXVECTOR3					m_vHeroPos;
	D3DXVECTOR3					m_vHeroDir;

	// 디바이스
	ComPtr<ID3D11Device>		m_pd3dDevice;
	ComPtr<ID3D11DeviceContext> m_pImmediateContext;
	BCamera*					m_pCamera;

	// 눈 이펙트
	bool						m_bSnowDrop;
	int							m_iStageState;
public:
	//--------------------------------------------------------------------------------------
	// 이펙트 로드 함수
	//--------------------------------------------------------------------------------------
	bool LoadEffect(TCHAR* szFileName, TCHAR* szParticleName);
public:
	//--------------------------------------------------------------------------------------
	// 캐릭터 포지션 저장 함수
	//--------------------------------------------------------------------------------------
	void					SetHeroPos(D3DXVECTOR3 vHeroPos);
	//--------------------------------------------------------------------------------------
	// 캐릭터 디렉션 저장 함수
	//--------------------------------------------------------------------------------------
	void					SetHeroDir(D3DXVECTOR3 vHeroDir);
	//--------------------------------------------------------------------------------------
	// 스테이지 상태 저장 함수
	//--------------------------------------------------------------------------------------
	void					SetStageState(int iStageState);
	//--------------------------------------------------------------------------------------
	// 재정의 함수
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

