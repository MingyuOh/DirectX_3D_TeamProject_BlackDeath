#pragma once
#include "BStd.h"
#include "FSMState.h"

const int K_iMaxSound = 20;

class K_Sound
{
	//public:
	//	static BSound& GetInstance()
	//	{
	//		static BSound singleton;
	//		return singleton;
	//	}
public:
	FMOD::System*	m_pSystem;
	FMOD::Sound*	m_pSound[K_iMaxSound];
	FMOD::Channel*	m_pChannel[K_iMaxSound];
public:
	bool Load(char* pLoadName, int indexNum, int Mode);
	bool Init();
	bool Render();
	bool Frame();
	bool Release();

	bool playeffect(int SoundNum);
	bool play(int MusicNum);
	bool stop(int MusicNum);
	bool Volume(int index, float fVoulume, bool bUP);
	bool VolumeSet(int index, float fVolume);
	bool paused(int MusicNum);
	bool playChannelSound(int MusicNum);
public:
	K_Sound();
	~K_Sound();
};

//#define I_SoundMgr BSound::GetInstance()