#pragma once

#include "BStd.h"
#include "LAPSBSingleton.h"

const int IMAX_SOUND_SIZE = 20000;

class LASound2 : public LAPSBSingleton<LASound2>
{
private:
	friend class LAPSBSingleton<LASound2>;
public:
	FMOD::System*	m_pSystem;
	FMOD::Sound*	m_pSound[IMAX_SOUND_SIZE];
	std::vector<std::string> m_SoundNameList;
	FMOD::Channel*	m_pChannel[IMAX_SOUND_SIZE];
	int m_iTotalCount;
public:
	int Load(char* pLoadName, int Mode);
	int CheckOverLoad(std::string Name);
	bool Init();
	bool Render();
	bool Frame();
	bool Release();

	bool playeffect(int SoundNum);
	bool play(int MusicNum);
	bool stop(int MusicNum);
	bool AllStop();
	bool Volume(int index, float fVoulume, bool bUP);
	bool VolumeSet(int index, float fVolume);
	bool paused(int MusicNum);
	bool playChannelSound(int MusicNum);
public:
	LASound2()
	{
		m_iTotalCount = 0;
	}
	~LASound2();
};

#define LA_SOUND2_MGR LASound2::GetInstance()