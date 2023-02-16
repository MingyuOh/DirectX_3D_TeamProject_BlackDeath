#pragma once
#include "BStd.h"
#include "FSMState.h"

const int g_iMaxSound = 20;

class BSound
{
public:
	static BSound& GetInstance()
	{
		static BSound singleton;
		return singleton;
	}
public:
	FMOD::System*	m_pSystem;
	FMOD::Sound*	m_pSound[g_iMaxSound];
	FMOD::Channel*	m_pChannel[g_iMaxSound];
public:

	bool Init();
	bool Render();
	bool Frame();
	bool Release();

	bool playeffect(int SoundNum);
	bool play(int MusicNum);
	bool stop(int MusicNum);
	bool paused(int MusicNum);
public:
	BSound();
	~BSound();
};

#define I_SoundMgr BSound::GetInstance()