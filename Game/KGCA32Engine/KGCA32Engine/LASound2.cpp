

#include "LASound2.h"

int LASound2::Load(char* pLoadName, int Mode)
{
	/*int CheckIndex;

	CheckIndex = CheckOverLoad(pLoadName);

	if (CheckIndex == -1)
	{*/
	switch (Mode)
	{
	case 0:
	{
		m_pSystem->createSound(pLoadName, FMOD_DEFAULT, 0, &m_pSound[m_iTotalCount]);
		m_pSound[m_iTotalCount]->setMode(FMOD_DEFAULT);
	}break;
	case 1:
	{
		m_pSystem->createSound(pLoadName, FMOD_DEFAULT, 0, &m_pSound[m_iTotalCount]);
		m_pSound[m_iTotalCount]->setMode(FMOD_LOOP_NORMAL);
	}break;
	}
	return m_iTotalCount++;
	/*}
	else
	{
	return CheckIndex;
	}*/
}

int LASound2::CheckOverLoad(std::string Name)
{
	char drive[256] = { 0 };
	char dir[256] = { 0 };
	char filename[256] = { 0 };
	char ext[256] = { 0 };
	_splitpath(Name.c_str(), drive, dir, filename, ext);

	for (int iCnt = 0; iCnt < (int)m_SoundNameList.size(); iCnt++)
	{
		if (m_SoundNameList[iCnt].compare(filename) == 0)
		{
			return iCnt;
		}
	}

	m_SoundNameList.push_back(filename);

	return -1;
}

bool LASound2::Init()
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
bool LASound2::Render()
{
	return true;
}
bool LASound2::Frame()
{
	return true;
}
bool LASound2::Release()
{
	/*for (int iSound = 0; iSound < m_iTotalCount; iSound++)
	{
		m_pSound[iSound]->release();
	}*/
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

bool LASound2::playeffect(int SoundNum)
{
	bool playing = false;
	if (m_pChannel[SoundNum]->isPlaying(&playing) == false)
	{
		m_pChannel[SoundNum]->stop();
	}
	HRESULT check;
	check = m_pSystem->playSound(m_pSound[SoundNum], 0, false, &m_pChannel[SoundNum]);

	if (check == S_OK)
	{
		return true;
	}
	else
	{
		return false;
	}

	return true;
}

bool LASound2::play(int MusicNum)
{
	bool playing = false;
	if (m_pChannel[MusicNum]->isPlaying(&playing) != false)
	{
		m_pSystem->playSound(m_pSound[MusicNum], 0, false, &m_pChannel[MusicNum]);
	}
	return true;
}
bool LASound2::playChannelSound(int MusicNum)
{
	for (int iSound = 0; iSound < m_iTotalCount; iSound++)
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
bool LASound2::Volume(int index, float fVoulume, bool bUP)
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
bool LASound2::VolumeSet(int index, float fVolume)
{
	m_pChannel[index]->setVolume(fVolume);
	return true;
}
bool LASound2::stop(int MusicNum)
{
	m_pChannel[MusicNum]->stop();
	return true;
}

bool LASound2::AllStop()
{
	for (int iCnt = 0; iCnt < m_iTotalCount; iCnt++)
	{
		m_pChannel[iCnt]->stop();
	}

	return true;
}

bool LASound2::paused(int MusicNum)
{
	bool paused;
	m_pChannel[MusicNum]->getPaused(&paused);
	m_pChannel[MusicNum]->setPaused(!paused);
	return true;
}



LASound2::~LASound2()
{
	Release();
}

