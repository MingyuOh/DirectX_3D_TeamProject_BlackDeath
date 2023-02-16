#include "BSound.h"

bool BSound::Init()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
	m_pSystem->createSound("../../data/UISound/BGM_TITLE.mp3", FMOD_DEFAULT, 0, &m_pSound[UI_SOUND_TITLE]);
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
	m_pSound[UI_SOUND_ICON_NEAR]->setMode(FMOD_DEFAULT);

	//m_pSystem->createSound("../../data/MenuSound.ogg", FMOD_DEFAULT, 0, &m_pSound[MenuUp]);
	//m_pSystem->createSound("../../data/MenuSelectSound.mp3", FMOD_DEFAULT, 0, &m_pSound[MenuSelect]);
	//m_pSystem->createSound("../../data/NoteMusic_umikaze.mp3", FMOD_DEFAULT, 0, &m_pSound[GameBG]);
	//m_pSystem->createSound("../../data/CloseMusic.mp3", FMOD_DEFAULT, 0, &m_pSound[CloseBG]);
	//m_pSystem->createSound("../../data/ResultMusic.mp3", FMOD_DEFAULT, 0, &m_pSound[ResultBG]);
	//m_pSound[UI_SOUND_TITLE]->setMode(FMOD_LOOP_NORMAL);
	//m_pSound[ResultBG]->setMode(FMOD_LOOP_NORMAL);
	//m_pSound[GameBG]->setMode(FMOD_LOOP_OFF);
	return true;
}
bool BSound::Render()
{
	return true;
}
bool BSound::Frame()
{
	return true;
}
bool BSound::Release()
{
	m_pSound[UI_SOUND_TITLE]->release();
	m_pSound[UI_SOUND_LOADING]->release();
	m_pSound[UI_SOUND_PHASE]->release();
	m_pSound[UI_SOUND_INGAME_FARMING]->release();
	m_pSound[UI_SOUND_INGAME_BATTLE]->release();
	m_pSound[UI_SOUND_INGAME_WIN]->release();
	m_pSound[UI_SOUND_INGAME_LOSE]->release();
	m_pSound[UI_SOUND_ICON_CLICK]->release();
	m_pSound[UI_SOUND_ICON_NEAR]->release();
	//m_pSound[GameBG]->release();
	m_pSystem->close();
	m_pSystem->release();
	return true;
}

bool BSound::playeffect(int SoundNum)
{
	bool playing = false;
	if (m_pChannel[SoundNum]->isPlaying(&playing) == false)
	{
		m_pChannel[SoundNum]->stop();
	}
	m_pSystem->playSound(m_pSound[SoundNum], 0, false, &m_pChannel[SoundNum]);
	return true;
}

bool BSound::play(int MusicNum)
{
	bool playing = false;
	if (m_pChannel[MusicNum]->isPlaying(&playing) != false)
	{
		m_pSystem->playSound(m_pSound[MusicNum], 0, false, &m_pChannel[MusicNum]);
	}
	return true;
}

bool BSound::stop(int MusicNum)
{
	m_pChannel[MusicNum]->stop();
	return true;
}

bool BSound::paused(int MusicNum)
{
	bool paused;
	m_pChannel[MusicNum]->getPaused(&paused);
	m_pChannel[MusicNum]->setPaused(!paused);
	return true;
}

BSound::BSound()
{
}


BSound::~BSound()
{
}
