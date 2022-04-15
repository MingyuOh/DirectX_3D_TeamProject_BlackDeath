#include "K_Sound.h"

bool K_Sound::Load(char* pLoadName, int indexNum, int Mode)
{
	switch (Mode)
	{
	case 0:
	{
		m_pSystem->createSound(pLoadName, FMOD_DEFAULT, 0, &m_pSound[indexNum]);
		m_pSound[indexNum]->setMode(FMOD_DEFAULT);
	}break;
	case 1:
	{
		m_pSystem->createSound(pLoadName, FMOD_DEFAULT, 0, &m_pSound[indexNum]);
		m_pSound[indexNum]->setMode(FMOD_LOOP_NORMAL);
	}break;
	}
	return true;
}
bool K_Sound::Init()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
	/*m_pSystem->createSound("../../data/UISound/BGM_TITLE.mp3", FMOD_DEFAULT, 0, &m_pSound[UI_SOUND_TITLE]);
	m_pSound[UI_SOUND_TITLE]->setMode(FMOD_LOOP_NORMAL);
	m_pSystem->createSound("../../data/UISound/BGM_LOADING.mp3", FMOD_DEFAULT, 0, &m_pSound[UI_SOUND_LOADING]);
	m_pSound[UI_SOUND_LOADING]->setMode(FMOD_LOOP_NORMAL);
	m_pSystem->createSound("../../data/UISound/SNDEFFECT_BATTLE_PHASE.mp3", FMOD_DEFAULT, 0, &m_pSound[UI_SOUND_PHASE]);
	m_pSound[UI_SOUND_PHASE]->setMode(FMOD_DEFAULT);
	m_pSystem->createSound("../../data/UISound/BGM_FARMINGPHASE_1.mp3", FMOD_DEFAULT, 0, &m_pSound[UI_SOUND_INGAME_FARMING]);
	m_pSound[UI_SOUND_INGAME_FARMING]->setMode(FMOD_LOOP_NORMAL);
	m_pSystem->createSound("../../data/UISound/BGM_BATTLEPHASE_1.mp3", FMOD_DEFAULT, 0, &m_pSound[UI_SOUND_INGAME_BATTLE]);
	m_pSound[UI_SOUND_INGAME_BATTLE]->setMode(FMOD_LOOP_NORMAL);
	m_pSystem->createSound("../../data/UISound/BGM_BATTLE_WIN.mp3", FMOD_DEFAULT, 0, &m_pSound[UI_SOUND_INGAME_WIN]);
	m_pSound[UI_SOUND_INGAME_WIN]->setMode(FMOD_LOOP_NORMAL);
	m_pSystem->createSound("../../data/UISound/BGM_BATTLE_LOSE.mp3", FMOD_DEFAULT, 0, &m_pSound[UI_SOUND_INGAME_LOSE]);
	m_pSound[UI_SOUND_INGAME_LOSE]->setMode(FMOD_LOOP_NORMAL);
	m_pSystem->createSound("../../data/UISound/SNDEFFECT_BTN_CLICK.mp3", FMOD_DEFAULT, 0, &m_pSound[UI_SOUND_ICON_CLICK]);
	m_pSound[UI_SOUND_ICON_CLICK]->setMode(FMOD_DEFAULT);
	m_pSystem->createSound("../../data/UISound/SNDEFFECT_BTN_NEAR.mp3", FMOD_DEFAULT, 0, &m_pSound[UI_SOUND_ICON_NEAR]);
	m_pSound[UI_SOUND_ICON_NEAR]->setMode(FMOD_DEFAULT);*/
	return true;
}
bool K_Sound::Render()
{
	return true;
}
bool K_Sound::Frame()
{
	return true;
}
bool K_Sound::Release()
{
	for (int iSound = 0; iSound < K_iMaxSound; iSound++)
	{
		m_pSound[iSound]->release();
	}
	/*m_pSound[UI_SOUND_TITLE]->release();
	m_pSound[UI_SOUND_LOADING]->release();
	m_pSound[UI_SOUND_PHASE]->release();
	m_pSound[UI_SOUND_INGAME_FARMING]->release();
	m_pSound[UI_SOUND_INGAME_BATTLE]->release();
	m_pSound[UI_SOUND_INGAME_WIN]->release();
	m_pSound[UI_SOUND_INGAME_LOSE]->release();
	m_pSound[UI_SOUND_ICON_CLICK]->release();
	m_pSound[UI_SOUND_ICON_NEAR]->release();*/
	//m_pSound[GameBG]->release();
	m_pSystem->close();
	m_pSystem->release();
	return true;
}

bool K_Sound::playeffect(int SoundNum)
{
	bool playing = false;
	if (m_pChannel[SoundNum]->isPlaying(&playing) == false)
	{
		m_pChannel[SoundNum]->stop();
	}
	m_pSystem->playSound(m_pSound[SoundNum], 0, false, &m_pChannel[SoundNum]);
	return true;
}

bool K_Sound::play(int MusicNum)
{
	bool playing = false;
	if (m_pChannel[MusicNum]->isPlaying(&playing) != false)
	{
		m_pSystem->playSound(m_pSound[MusicNum], 0, false, &m_pChannel[MusicNum]);
	}
	return true;
}
bool K_Sound::playChannelSound(int MusicNum)
{
	for (int iSound = 0; iSound < K_iMaxSound; iSound++)
	{
		if (iSound == MusicNum)
		{
			bool playing = false;
			if (m_pChannel[MusicNum]->isPlaying(&playing) != false)
			{
				m_pSystem->playSound(m_pSound[MusicNum], 0, false, &m_pChannel[MusicNum]);
			}
		}
		else
		{
			m_pChannel[iSound]->stop();
		}
	}
	return true;
}
bool K_Sound::Volume(int index, float fVoulume, bool bUP)
{
	float CurrentVoulume;
	m_pChannel[index]->getVolume(&CurrentVoulume);
	if (bUP == true)	//º¼·ý Áõ°¡
	{
		CurrentVoulume += fVoulume;
		if (CurrentVoulume > 1.0f)
			CurrentVoulume = 1.0f;
	}
	else                //º¼·ý °¨¼Ò
	{
		CurrentVoulume -= fVoulume;
		if (CurrentVoulume < 0.0f)
			CurrentVoulume = 0.0f;
	}
	m_pChannel[index]->setVolume(CurrentVoulume);
	return true;
}
bool K_Sound::VolumeSet(int index, float fVolume)
{
	m_pChannel[index]->setVolume(fVolume);
	return true;
}
bool K_Sound::stop(int MusicNum)
{
	m_pChannel[MusicNum]->stop();
	return true;
}

bool K_Sound::paused(int MusicNum)
{
	bool paused;
	m_pChannel[MusicNum]->getPaused(&paused);
	m_pChannel[MusicNum]->setPaused(!paused);
	return true;
}

K_Sound::K_Sound()
{
}


K_Sound::~K_Sound()
{
}
