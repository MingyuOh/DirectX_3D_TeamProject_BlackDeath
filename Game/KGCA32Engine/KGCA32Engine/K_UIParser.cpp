#include "K_UIParser.h"
#include "LASound2.h"

void K_UIParser::SoundStopIngame()
{
	if (FarmingOrBattle == 1)
		m_Sound.stop(UI_SOUND_INGAME_FARMING);
	else if (FarmingOrBattle == 3)
	{
		m_Sound.stop(UI_SOUND_INGAME_FARMING);
		m_Sound.stop(UI_SOUND_INGAME_SNOW);
	}
	else if (FarmingOrBattle == 5)
		m_Sound.stop(UI_SOUND_INGAME_FARMING_2);
	else if (FarmingOrBattle == 7)
		m_Sound.stop(UI_SOUND_INGAME_FARMING_3);
	else if (FarmingOrBattle == 2)
		m_Sound.stop(UI_SOUND_INGAME_BATTLE);
	else if (FarmingOrBattle == 4)
	{
		m_Sound.stop(UI_SOUND_INGAME_BATTLE);
		m_Sound.stop(UI_SOUND_INGAME_SNOW);
	}
	else if (FarmingOrBattle == 6)
		m_Sound.stop(UI_SOUND_INGAME_BATTLE_2);
	else if (FarmingOrBattle == 8)
		m_Sound.stop(UI_SOUND_INGAME_BATTLE_3);
}

void K_UIParser::TextureSizeChange(int Texture_Num, K_SceneUI* pSelectScene)
{
	S_RELEASE(pSelectScene->m_TextureCtl[Texture_Num]->m_pPlane->m_pVertexBuffer);
	{
		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = sizeof(PNCT_VERTEX) * pSelectScene->m_TextureCtl[Texture_Num]->m_pPlane->m_VertexList.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = (void*)&pSelectScene->m_TextureCtl[Texture_Num]->m_pPlane->m_VertexList.at(0);
		m_pd3dDevice->CreateBuffer(&bd, &sd, &pSelectScene->m_TextureCtl[Texture_Num]->m_pPlane->m_pVertexBuffer);
	}
	pSelectScene->m_TextureCtl[Texture_Num]->Update();
}

void K_UIParser::GotoMainState()
{
	SetState();
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState =
		m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState;
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState =
		m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState;

	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_FULLMODE_CHECK]->isRenderOk =
		m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_FULLMODE_CHECK]->isRenderOk;
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_WINDOWMODE_CHECK]->isRenderOk =
		m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_WINDOWMODE_CHECK]->isRenderOk;
}

void K_UIParser::PickUpTextureRender()
{
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PICK_1]->isRenderOk = true;
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PICK_2]->isRenderOk = true;
}
void K_UIParser::PickUpTextureNotRender()
{
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PICK_1]->isRenderOk = false;
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PICK_2]->isRenderOk = false;
}

bool K_UIParser::BackgroundMusicSet(int iVolume)
{
	float fVolume = (float)iVolume / 10;
	m_Sound.VolumeSet(UI_SOUND_TITLE, fVolume);
	m_Sound.VolumeSet(UI_SOUND_LOADING, fVolume);
	m_Sound.VolumeSet(UI_SOUND_INGAME_FARMING, fVolume);
	m_Sound.VolumeSet(UI_SOUND_INGAME_BATTLE, fVolume);
	m_Sound.VolumeSet(UI_SOUND_INGAME_WIN, fVolume);
	m_Sound.VolumeSet(UI_SOUND_INGAME_LOSE, fVolume);
	return true;
}

bool K_UIParser::SoundEffectSet(int iVolume)
{
	float fVolume = (float)iVolume / 10;
	float fVol;
	m_Sound.VolumeSet(UI_SOUND_PHASE, fVolume);
	m_Sound.m_pChannel[UI_SOUND_PHASE]->getVolume(&fVol);
	m_Sound.VolumeSet(UI_SOUND_ICON_CLICK, fVolume);
	m_Sound.m_pChannel[UI_SOUND_ICON_CLICK]->getVolume(&fVol);
	m_Sound.VolumeSet(UI_SOUND_ICON_NEAR, fVolume);
	m_Sound.m_pChannel[UI_SOUND_ICON_NEAR]->getVolume(&fVol);
	for (int iSound = 0; iSound < LA_SOUND2_MGR.m_iTotalCount; iSound++)
	{
		LA_SOUND2_MGR.VolumeSet(iSound, fVolume);
	}
	return true;
}

bool K_UIParser::CharacterSceneDigitStateSet(int Dight, int Count, int TexTureNum)
{
	for (int iCnt = 0; iCnt < Count; iCnt++)
	{
		if (iCnt != 0 && Dight == 0)
			m_SceneList[INGAME]->m_TextureCtl[TexTureNum]->isRenderOk = false;
		else
		{
			if (!m_SceneList[INGAME]->m_TextureCtl[TexTureNum]->isRenderOk)
				m_SceneList[INGAME]->m_TextureCtl[TexTureNum]->isRenderOk = true;
			m_SceneList[INGAME]->m_TextureCtl[TexTureNum]->m_dwCurrentState = Dight % 10;	//나머지로
			Dight = Dight / 10;	//자릿수 -1
		}
		TexTureNum--;
	}
	return true;
}

bool K_UIParser::CharacterPanelDigitStateSet(int Dight, int Count, int TexTureNum)
{
	for (int iCnt = 0; iCnt < Count; iCnt++)
	{
		if (iCnt != 0 && Dight == 0)
			m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[TexTureNum]->isRenderOk = false;
		else
		{
			if (!m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[TexTureNum]->isRenderOk)
				m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[TexTureNum]->isRenderOk = true;
			m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[TexTureNum]->m_dwCurrentState = Dight % 10;	//나머지로
			Dight = Dight / 10;	//자릿수 -1
		}
		TexTureNum--;
	}
	return true;
}
bool K_UIParser::CharacterUIDataLoad()
{
	pHeroData = LOAD_OBJECT_MGR.GetHero();
	m_pCharacterUIData = pHeroData->GetUiCharacterData();
	m_pInventoryData = pHeroData->GetInventoryData();
	return true;
}

bool K_UIParser::TimeStateChange()
{
	if (m_SecondimeTick > m_SecnodTimeChangeTerm)	//시간의 첫번째 인자가 봐뀌는 텀
	{
		if (m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0)
		{
			m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_9;
			m_SecondimeTick -= m_SecondimeTick;
			m_SecondCount++;
			return true;
		}
		m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState -= 1;
		m_SecondimeTick -= m_SecondimeTick;
		m_SecondCount++;
	}
	if (m_SecondCount > 9)	//시간의 두번째 인자가 봐뀌는 텀
	{
		if (m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0)
		{
			m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_5;
			m_SecondCount = 0;
			m_TenSecondCount++;
			return true;
		}
		m_SecondCount = 0;
		m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState -= 1;
		m_TenSecondCount++;
	}
	if (m_TenSecondCount > 5)	//시간의 세번째 인자가 바뀌는 텀
	{
		if (m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0)
		{
			m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_9;
			m_TenSecondCount = 0;
			return true;
		}
		m_TenSecondCount = 0;
		m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState -= 1;
	}
}

HRESULT K_UIParser::CreateResource(int iWidth, int iHeight)
{
	HRESULT hr;
	for (int iCnt = 0; iCnt < m_SceneList.size(); iCnt++)
	{
		m_SceneList[iCnt]->CreateResource(iWidth, iHeight);
	}
	return S_OK;
}
bool K_UIParser::SetState()
{
	//씬들의 초기상태 등록
	m_pSelectScene = m_SceneList[MAIN];

	m_SceneList[MAIN]->m_dwCurrentState = MAIN_STATE_NONE;
	m_SceneList[MAIN]->m_pSelectPanel = NULL;
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState = 4;
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState = 4;
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_FULLMODE_CHECK]->isRenderOk = false;
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_WINDOWMODE_CHECK]->isRenderOk = true;

	m_SceneList[LOADING]->m_dwCurrentState = LOADING_STATE_LOAD;
	m_SceneList[LOADING]->m_TextureCtl[LOADING_TEXTURE_WAITING]->isRenderOk = true;
	m_SceneList[LOADING]->m_TextureCtl[LOADING_TEXUTRE_COMPLETE]->isRenderOk = false;
	m_SceneList[LOADING]->m_pSelectPanel = NULL;
	//인게임의 초기상태는 스테이지1의 파밍
	m_SceneList[INGAME]->m_dwCurrentState = INGAME_STATE_STAGE_1_FARMING;
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_STAGE]->m_dwCurrentState = INGAME_TEXTURE_STAGE_1;
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PHASE]->m_dwCurrentState = INGAME_TEXTURE_PHASE_FARMING;
	//스테이지1의 파밍 1분
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_1]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_1;
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_2;
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
	m_SecondCount = 9;
	m_TenSecondCount = 3;

	//인게임 텍스쳐 렌더 여부
	for (int iTexture = INGAME_TEXTURE_RIPLETEXT; iTexture <= INGAME_TEXTURE_MEDICNUMBER_2; iTexture++)
	{
		m_SceneList[INGAME]->m_TextureCtl[iTexture]->isRenderOk = false;
	}
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_RIPLE_RELOAD]->isRenderOk = false;
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_HANDGUN_RELOAD]->isRenderOk = false;
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_BOMB_RELOAD]->isRenderOk = false;
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_MEDIC_RELOAD]->isRenderOk = false;
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PICK_1]->isRenderOk = false;
	m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PICK_2]->isRenderOk = false;

	//인벤창의 버튼 및 텍스쳐 비활성화
	for (int iTexture = INGAME_PANEL_INVEN_TEXTURE_HP_PLUS; iTexture <= INGAME_PANEL_INVEN_TEXTURE_MOVE_PLUS_3; iTexture++)
	{
		m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[iTexture]->isRenderOk = false;
	}
	for (int iTexture = 0; iTexture < 5; iTexture++)
	{
		m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINRIPLE_T + iTexture]->isRenderOk = false;
	}
	for (int iTexture = 0; iTexture < 5; iTexture++)
	{
		m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINBOMB_T + iTexture]->isRenderOk = false;
	}
	for (int iTexture = 0; iTexture < 5; iTexture++)
	{
		m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINMEDIC_T + iTexture]->isRenderOk = false;
	}
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINHAT]->isRenderOk = false;
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINBODY]->isRenderOk = false;
	for (int iButton = INGAME_PANEL_INVEN_BUTTON_WEAPON_1; iButton <= INGAME_PANEL_INVEN_BUTTON_WEAPON_10; iButton++)
	{
		m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_ButtonCtl[iButton]->isActivateOK = false;
		m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_ButtonCtl[10 + iButton]->isActivateOK = false;
		for (int iTexture = INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1; iTexture <= INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_SHOOT; iTexture++)
		{
			m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[iTexture + (iButton - 1) * 7]->isRenderOk = false;
		}
		m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_ARMOR + (iButton - 1)]->isRenderOk = false;
	}
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->isRenderOk = false;
	//
	m_SceneList[INGAME]->m_pSelectPanel = NULL;

	MouseCursorOn = true;	//마우스커서 온 트루
	return true;
}
bool K_UIParser::SetUIFSM()
{
	//파서에서 Init과정을 마치고 기본 틀이 만들어졌을때 호출
	//메인씬에서의 경우의수 등록
	m_SceneList[MAIN]->m_FSM.AddStateTransition(MAIN_STATE_NONE, MAIN_BUTTON_GAMEPLAY_CLICKED, MAIN_STATE_GAMEPLAY_SCENE_ON);
	m_SceneList[MAIN]->m_FSM.AddStateTransition(MAIN_STATE_NONE, MAIN_BUTTON_HOWTOPLAY_CLICKED, MAIN_STATE_HOWTOPLAY_PANEL_ON);
	m_SceneList[MAIN]->m_FSM.AddStateTransition(MAIN_STATE_NONE, MAIN_BUTTON_OPTION_CLICKED, MAIN_STATE_OPTION_PANEL_ON);
	m_SceneList[MAIN]->m_FSM.AddStateTransition(MAIN_STATE_NONE, MAIN_BUTTON_CREADIT_CLICKED, MAIN_STATE_CREADIT_PANEL_ON);
	m_SceneList[MAIN]->m_FSM.AddStateTransition(MAIN_STATE_NONE, MAIN_BUTTON_EXIT_CLICKED, MAIN_STATE_EXIT_PANEL_ON);

	m_SceneList[MAIN]->m_FSM.AddStateTransition(MAIN_STATE_HOWTOPLAY_PANEL_ON, MAIN_PANEL_HOWTOPLAY_OFF, MAIN_STATE_NONE);
	m_SceneList[MAIN]->m_FSM.AddStateTransition(MAIN_STATE_OPTION_PANEL_ON, MAIN_PANEL_OPTION_OFF, MAIN_STATE_NONE);
	m_SceneList[MAIN]->m_FSM.AddStateTransition(MAIN_STATE_CREADIT_PANEL_ON, MAIN_PANEL_CREADIT_OFF, MAIN_STATE_NONE);
	m_SceneList[MAIN]->m_FSM.AddStateTransition(MAIN_STATE_EXIT_PANEL_ON, MAIN_PANEL_EXIT_OFF, MAIN_STATE_NONE);
	//메인씬의 패널의 경우의수 등록
	//HOWTOPLAY 패널
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_HOWTOPLAY]->m_FSM.AddStateTransition(MAIN_PANEL_OFF, MAIN_BUTTON_HOWTOPLAY_CLICKED, MAIN_PANEL_ON);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_HOWTOPLAY]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_HOWTOPLAY_X_CLICKED, MAIN_PANEL_OFF);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_HOWTOPLAY]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_HOWTOPLAY_NEXT_CLICKED, MAIN_HOWTOPLAY_PANEL_NEXT);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_HOWTOPLAY]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_HOWTOPLAY_BACK_CLICKED, MAIN_HOWTOPLAY_PANEL_BACK);

	//OPTION 패널
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_FSM.AddStateTransition(MAIN_PANEL_OFF, MAIN_BUTTON_OPTION_CLICKED, MAIN_PANEL_ON);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_OPTION_BACK_CLICKED, MAIN_PANEL_OFF);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_OPTION_BM_LEFT_CLICKED, MAIN_OPTION_PANEL_BM_LEFT);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_OPTION_BM_RIGHT_CLICKED, MAIN_OPTION_PANEL_BM_RIGHT);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_OPTION_SE_LEFT_CLICKED, MAIN_OPTION_PANEL_SE_LEFT);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_OPTION_SE_RIGHT_CLICKED, MAIN_OPTION_PANEL_SE_RIGHT);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_OPTION_FULLMODE_CLICKED, MAIN_OPTION_PANEL_FULLMODE);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_OPTION_WINDOWMODE_CLICKED, MAIN_OPTION_PANEL_WINDOWMODE);

	//CREDIT패널
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_CREADIT]->m_FSM.AddStateTransition(MAIN_PANEL_OFF, MAIN_BUTTON_CREADIT_CLICKED, MAIN_PANEL_ON);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_CREADIT]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_CREADIT_X_CLICKED, MAIN_PANEL_OFF);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_CREADIT]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_CREADIT_NEXT_CLICKED, MAIN_CREADIT_PANEL_NEXT);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_CREADIT]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_CREADIT_BACK_CLICKED, MAIN_CREADIT_PANEL_BACK);

	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_EXIT]->m_FSM.AddStateTransition(MAIN_PANEL_OFF, MAIN_BUTTON_EXIT_CLICKED, MAIN_PANEL_ON);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_EXIT]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_EXIT_YES_CLICKED, MAIN_PANEL_OFF);
	m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_EXIT]->m_FSM.AddStateTransition(MAIN_PANEL_ON, MAIN_PANEL_EXIT_NO_CLICKED, MAIN_PANEL_OFF);

	//로딩씬의 경우의수 등록
	m_SceneList[LOADING]->m_FSM.AddStateTransition(LOADING_STATE_LOAD, LOADING_EVENT_LOAD, LOADING_STATE_LOAD);
	m_SceneList[LOADING]->m_FSM.AddStateTransition(LOADING_STATE_LOAD, LOADING_EVENT_COMPLETE, LOADING_EVENT_COMPLETE);

	//로딩씬의 INFORMATION 패널
	m_SceneList[LOADING]->m_PanelList[LOADING_PANEL_INFORMATION]->m_FSM.AddStateTransition(LOADING_PANEL_INFORMATION_ON, LOADING_PANEL_INFORMATION_BACK_CLICKED, LOADING_PANEL_INFORMATION_BACK);
	m_SceneList[LOADING]->m_PanelList[LOADING_PANEL_INFORMATION]->m_FSM.AddStateTransition(LOADING_PANEL_INFORMATION_ON, LOADING_PANEL_INFORMATION_NEXT_CLICKED, LOADING_PANEL_INFORMATION_NEXT);
	//인게임씬의 경우의수 등록
	//스테이지1
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_1_FARMING, INGAME_EVENT_ESC_CLICKED, INGAME_STATE_NEEDTOHELP_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_1_FARMING, INGAME_EVENT_INVEN_CLICKED, INGAME_STATE_INVEN_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_1_FARMING, INGAME_EVENT_FARMING_END, INGAME_STATE_STAGE_1_BATTLE);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_1_BATTLE, INGAME_EVENT_ESC_CLICKED, INGAME_STATE_NEEDTOHELP_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_1_BATTLE, INGAME_EVENT_INVEN_CLICKED, INGAME_STATE_INVEN_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_1_BATTLE, INGAME_EVENT_BATTLE_END, INGAME_STATE_STAGE_2_FARMING);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_1_BATTLE, INGAME_EVENT_LOSE, INGAME_STATE_LOSE_PANEL_ON);	//스테이지1의 배틀씬에서의 패배

																																//스테이지2
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_2_FARMING, INGAME_EVENT_ESC_CLICKED, INGAME_STATE_NEEDTOHELP_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_2_FARMING, INGAME_EVENT_INVEN_CLICKED, INGAME_STATE_INVEN_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_2_FARMING, INGAME_EVENT_FARMING_END, INGAME_STATE_STAGE_2_BATTLE);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_2_BATTLE, INGAME_EVENT_ESC_CLICKED, INGAME_STATE_NEEDTOHELP_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_2_BATTLE, INGAME_EVENT_INVEN_CLICKED, INGAME_STATE_INVEN_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_2_BATTLE, INGAME_EVENT_BATTLE_END, INGAME_STATE_STAGE_3_FARMING);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_2_BATTLE, INGAME_EVENT_LOSE, INGAME_STATE_LOSE_PANEL_ON);	//스테이지2의 배틀씬에서의 패배

																																//스테이지3
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_3_FARMING, INGAME_EVENT_ESC_CLICKED, INGAME_STATE_NEEDTOHELP_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_3_FARMING, INGAME_EVENT_INVEN_CLICKED, INGAME_STATE_INVEN_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_3_FARMING, INGAME_EVENT_FARMING_END, INGMAE_STATE_STAGE_3_BATTLE);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGMAE_STATE_STAGE_3_BATTLE, INGAME_EVENT_ESC_CLICKED, INGAME_STATE_NEEDTOHELP_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGMAE_STATE_STAGE_3_BATTLE, INGAME_EVENT_INVEN_CLICKED, INGAME_STATE_INVEN_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGMAE_STATE_STAGE_3_BATTLE, INGAME_EVENT_BATTLE_END, INGAME_STATE_STAGE_4_FARMING);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGMAE_STATE_STAGE_3_BATTLE, INGAME_EVENT_LOSE, INGAME_STATE_LOSE_PANEL_ON);	//스테이지3의 배틀씬에서의 패배

																																//스테이지4
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_4_FARMING, INGAME_EVENT_ESC_CLICKED, INGAME_STATE_NEEDTOHELP_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_4_FARMING, INGAME_EVENT_INVEN_CLICKED, INGAME_STATE_INVEN_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_4_FARMING, INGAME_EVENT_FARMING_END, INGAME_STATE_STAGE_4_BATTLE);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_4_BATTLE, INGAME_EVENT_ESC_CLICKED, INGAME_STATE_NEEDTOHELP_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_4_BATTLE, INGAME_EVENT_INVEN_CLICKED, INGAME_STATE_INVEN_PANEL_ON);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_4_BATTLE, INGAME_EVENT_LOSE, INGAME_STATE_LOSE_PANEL_ON);	//스테이지4의 배틀씬에서의 패배
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_STAGE_4_BATTLE, INGAME_EVENT_WIN, INGAME_STATE_WIN_PANEL_ON);	//스테이지4의 배틀씨네서의 승리

	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_NEEDTOHELP_PANEL_ON, INGAME_EVENT_BACKTOMAIN, INGAME_STATE_BACKTOMAIN);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_WIN_PANEL_ON, INGAME_EVENT_BACKTOMAIN, INGAME_STATE_BACKTOMAIN);
	m_SceneList[INGAME]->m_FSM.AddStateTransition(INGAME_STATE_LOSE_PANEL_ON, INGAME_EVENT_BACKTOMAIN, INGAME_STATE_BACKTOMAIN);
	//인게임씬의 패널의 경우의수 등록
	//인게임씬의 NEEDTOHELP 패널
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_NEEDTOHELP]->m_FSM.AddStateTransition(INGAME_PANEL_OFF, INGAME_EVENT_ESC_CLICKED, INGAME_PANEL_ON);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_NEEDTOHELP]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_HOWTOPLAY_CLICKED, INGAME_PANEL_NEEDTOHELP_HOWTOPLAY_ON);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_NEEDTOHELP]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_OPTION_CLICKED, INGAME_PANEL_NEEDTOHELP_OPTION_ON);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_NEEDTOHELP]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_NEEDTOHELP_GOBACKGAME_CLICKED, INGAME_PANEL_OFF);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_NEEDTOHELP]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_NEEDTOHELP_GOTOMAIN_CLICKED, INGAME_PANEL_GOTOMAIN);

	//인게임씬의 HOWTOPLAY 패널
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_HOWTOPLAY]->m_FSM.AddStateTransition(INGAME_PANEL_OFF, INGAME_PANEL_HOWTOPLAY_CLICKED, INGAME_PANEL_ON);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_HOWTOPLAY]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_HOWTOPLAY_NEXT_CLICKED, INGAME_HOWTOPLAY_PANEL_NEXT);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_HOWTOPLAY]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_HOWTOPLAY_BACK_CLICKED, INGAME_HOWTOPLAY_PANEL_BACK);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_HOWTOPLAY]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_HOWTOPLAY_X_CLICKED, INGAME_PANEL_OFF);

	//인게임씬의 OPTION 패널
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_FSM.AddStateTransition(INGAME_PANEL_OFF, INGAME_PANEL_OPTION_CLICKED, INGAME_PANEL_ON);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_OPTION_BACK_CLICKED, INGAME_PANEL_OFF);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_OPTION_BM_LEFT_CLICKED, INGAME_OPTION_PANEL_BM_LEFT);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_OPTION_BM_RIGHT_CLICKED, INGAME_OPTION_PANEL_BM_RIGHT);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_OPTION_SE_LEFT_CLICKED, INGAME_OPTION_PANEL_SE_LEFT);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_OPTION_SE_RIGHT_CLICKED, INGAME_OPTION_PANEL_SE_RIGHT);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_OPTION_FULLMODE_CLICKED, INGAME_OPTION_PANEL_FULLMODE);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_OPTION_WINDOWMODE_CLICKED, INGAME_OPTION_PANEL_WINDOWMODE);

	//인게임씬의 INVEN 패널
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_FSM.AddStateTransition(INGAME_PANEL_OFF, INGAME_EVENT_INVEN_CLICKED, INGAME_PANEL_ON);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_INVEN]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_INVEN_X_CLICKED, INGAME_PANEL_OFF);

	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_WIN]->m_FSM.AddStateTransition(INGAME_PANEL_OFF, INGAME_EVENT_WIN, INGAME_PANEL_ON);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_WIN]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_WIN_GOTOMAIN_CLICKED, INGAME_PANEL_GOTOMAIN);

	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_LOSE]->m_FSM.AddStateTransition(INGAME_PANEL_OFF, INGAME_EVENT_LOSE, INGAME_PANEL_ON);
	m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_LOSE]->m_FSM.AddStateTransition(INGAME_PANEL_ON, INGAME_PANEL_LOSE_GOTOMAIN_CLICKED, INGAME_PANEL_GOTOMAIN);
	return true;
}
bool K_UIParser::AddRect(KUI_TYPE type, TCHAR* TextureStr,
	float Sca_X,
	float Sca_Y,
	float Sca_Z,
	KPS_TYPE ps_type)
{
	switch (type)
	{
	case K_BUTTON:
	{
		K_ButtonCtl*	pUIControl = new K_ButtonCtl();
		pUIControl->Set(g_iWindowWidth, g_iWindowHeight);
		pUIControl->CreateResource(g_iWindowWidth, g_iWindowHeight);
		pUIControl->Init(Sca_X, Sca_Y, Sca_Z);
		pUIControl->Create(m_pd3dDevice, m_pImmediateContext,
			L"../../Data/UIShader/VertexShader.vsh",
			L"../../Data/UIShader/PixelShader.psh",
			TextureStr);
		//선택된 씬의 버튼컨트롤에 추가
		if (ps_type == K_SCENE)
			m_pSelectScene->m_ButtonCtl.push_back(pUIControl);
		else if (ps_type == K_PANEL)
			m_pSelectScene->m_pSelectPanel->m_ButtonCtl.push_back(pUIControl);
	}break;
	case K_IMAGE:
	{
		K_ImageCtl*		pUIControl = new K_ImageCtl();
		pUIControl->Set(g_iWindowWidth, g_iWindowHeight);
		pUIControl->CreateResource(g_iWindowWidth, g_iWindowHeight);
		pUIControl->Init(Sca_X, Sca_Y, Sca_Z);
		pUIControl->Create(m_pd3dDevice, m_pImmediateContext,
			L"../../Data/UIShader/VertexShader.vsh",
			L"../../Data/UIShader/PixelShader.psh",
			TextureStr);
		//선택된 씬의 이미지컨트롤에 추가
		if (ps_type == K_SCENE)
			m_pSelectScene->m_ImageCtl.push_back(pUIControl);
		else if (ps_type == K_PANEL)
			m_pSelectScene->m_pSelectPanel->m_ImageCtl.push_back(pUIControl);
	}break;
	case K_EDIT:
	{
		K_EditCtl*		pUIControl = new K_EditCtl();
		pUIControl->Set(g_iWindowWidth, g_iWindowHeight);
		pUIControl->CreateResource(g_iWindowWidth, g_iWindowHeight);
		pUIControl->Init(Sca_X, Sca_Y, Sca_Z);
		pUIControl->Create(m_pd3dDevice, m_pImmediateContext,
			L"../../Data/UIShader/VertexShader.vsh",
			L"../../Data/UIShader/PixelShader.psh",
			TextureStr);
		//선택된 씬의 이미지컨트롤에 추가
		if (ps_type == K_SCENE)
			m_pSelectScene->m_EditCtl.push_back(pUIControl);
		else if (ps_type == K_PANEL)
			m_pSelectScene->m_pSelectPanel->m_EditCtl.push_back(pUIControl);
	}break;
	case K_TEXTURE:
	{
		K_TextureCtl*		pUIControl = new K_TextureCtl();
		pUIControl->Set(g_iWindowWidth, g_iWindowHeight);
		pUIControl->CreateResource(g_iWindowWidth, g_iWindowHeight);
		pUIControl->Init(Sca_X, Sca_Y, Sca_Z);
		pUIControl->Create(m_pd3dDevice, m_pImmediateContext,
			L"../../Data/UIShader/VertexShader.vsh",
			L"../../Data/UIShader/PixelShader.psh",
			TextureStr);
		//선택된 씬의 이미지컨트롤에 추가
		if (ps_type == K_SCENE)
			m_pSelectScene->m_TextureCtl.push_back(pUIControl);
		else if (ps_type == K_PANEL)
			m_pSelectScene->m_pSelectPanel->m_TextureCtl.push_back(pUIControl);
	}break;
	}
	return true;
}
bool K_UIParser::SceneButtonSelect()
{
	POINT mouse;
	GetCursorPos(&mouse);	//마우스 포인터 위치
	ScreenToClient(m_hWnd, &mouse);

	RECT AAA;
	GetClientRect(m_hWnd, &AAA);
	float ClientX = 1920 * ((float)mouse.x / AAA.right);
	float ClientY = 1080 * ((float)mouse.y / AAA.bottom);

	if (m_pSelectScene == m_SceneList[MAIN])	//씬이 메인 씬일때
	{
		for (int iRect = m_pSelectScene->m_ButtonCtl.size() - 1; iRect >= 0; iRect--)
		{
			K_ControlUI* pRect = m_pSelectScene->m_ButtonCtl[iRect];
			RECT rt = pRect->m_rt;
			if (rt.left <= ClientX && rt.right >= ClientX)
			{
				if (rt.top <= ClientY && rt.bottom >= ClientY)
				{
					//버튼위에 마우스가 올라갔을때
					if (pRect->Sound_Check)
					{
						m_Sound.playeffect(UI_SOUND_ICON_NEAR);
					}
					pRect->Sound_Check = false;
					//I_SoundMgr.m_pSystem->playSound(I_SoundMgr.m_pSound[UI_SOUND_ICON_NEAR], 0, false, &I_SoundMgr.m_pChannel[UI_SOUND_ICON_NEAR]);
					pRect->SetTransition(BUTTON_MOUSE_UP);
				}
				else
				{
					pRect->SetTransition(BUTTON_MOUSE_DOWN);
					pRect->Sound_Check = true;
				}
			}
			else
			{
				pRect->SetTransition(BUTTON_MOUSE_DOWN);
				pRect->Sound_Check = true;
			}
			if (I_Input.m_MouseState[0] == KEY_PUSH)
			{
				if (rt.left <= ClientX && rt.right >= ClientX)
				{
					if (rt.top <= ClientY && rt.bottom >= ClientY)
					{
						m_Sound.playeffect(UI_SOUND_ICON_CLICK);
						pRect->SetTransition(BUTTON_MOUSE_CLICKED);
						if (iRect == MAIN_BUTTON_GAMEPLAY)	//게임 플레이 버튼
						{
							m_pSelectScene->SetTransition(MAIN_BUTTON_GAMEPLAY_CLICKED);
							//m_Mouse.m_bRenderMouseShape = false;
						}
						else if (iRect == MAIN_BUTTON_HOWTOPLAY)
						{
							m_pSelectScene->SetTransition(MAIN_BUTTON_HOWTOPLAY_CLICKED);
							m_pSelectScene->m_PanelList[MAIN_PANEL_HOWTOPLAY]->SetTransition(MAIN_BUTTON_HOWTOPLAY_CLICKED);
						}
						else if (iRect == MAIN_BUTTON_OPTION)
						{
							m_pSelectScene->SetTransition(MAIN_BUTTON_OPTION_CLICKED);
							m_pSelectScene->m_PanelList[MAIN_PANEL_OPTION]->SetTransition(MAIN_BUTTON_OPTION_CLICKED);
						}
						else if (iRect == MAIN_BUTTON_CREADIT)
						{
							m_pSelectScene->SetTransition(MAIN_BUTTON_CREADIT_CLICKED);
							m_pSelectScene->m_PanelList[MAIN_PANEL_CREADIT]->SetTransition(MAIN_BUTTON_CREADIT_CLICKED);
						}
						else if (iRect == MAIN_BUTTON_EXIT)
						{
							m_pSelectScene->SetTransition(MAIN_BUTTON_EXIT_CLICKED);
							m_pSelectScene->m_PanelList[MAIN_PANEL_EXIT]->SetTransition(MAIN_BUTTON_EXIT_CLICKED);
						}
					}
				}
			}
		}
	}
	else if (m_pSelectScene == m_SceneList[INGAME])	//인게임씬일때
	{
		if (I_Input.KeyCheck(DIK_ESCAPE) == KEY_PUSH)	//ESC키로 수정 필요..
		{
			ShowCursor(true);
			ShowCursor(true);
			//m_Mouse.m_bRenderMouseShape = true;
			
			SoundStopIngame();

			for (int iSound = 0; iSound < LA_SOUND2_MGR.m_iTotalCount; iSound++)
			{
				LA_SOUND2_MGR.paused(iSound);
			}

			m_pSelectScene->SetTransition(INGAME_EVENT_ESC_CLICKED);
			m_pSelectScene->m_PanelList[INGAME_PANEL_NEEDTOHELP]->SetTransition(INGAME_EVENT_ESC_CLICKED);
		}
		else if (I_Input.KeyCheck(DIK_I) == KEY_PUSH)
		{
			ShowCursor(true);
			ShowCursor(true);
			//m_Mouse.m_bRenderMouseShape = true;

			SoundStopIngame();

			for (int iSound = 0; iSound < LA_SOUND2_MGR.m_iTotalCount; iSound++)
			{
				LA_SOUND2_MGR.paused(iSound);
			}
			m_pSelectScene->SetTransition(INGAME_EVENT_INVEN_CLICKED);
			m_pSelectScene->m_PanelList[INGAME_PANEL_INVEN]->SetTransition(INGAME_EVENT_INVEN_CLICKED);
		}
	}
	return true;
}

bool K_UIParser::PanelButtonSelect()
{
	POINT mouse;
	GetCursorPos(&mouse);	//마우스 포인터 위치
	ScreenToClient(m_hWnd, &mouse);

	RECT AAA;
	GetClientRect(m_hWnd, &AAA);
	float ClientX = 1920 * ((float)mouse.x / AAA.right);
	float ClientY = 1080 * ((float)mouse.y / AAA.bottom);

	if (m_pSelectScene == m_SceneList[MAIN])//메인 씬
	{
		for (int iRect = m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1; iRect >= 0; iRect--)
		{
			K_ControlUI* pRect = m_pSelectScene->m_pSelectPanel->m_ButtonCtl[iRect];
			RECT rt = pRect->m_rt;
			if (pRect->isRenderOk)
			{
				if (rt.left <= ClientX && rt.right >= ClientX)
				{
					if (rt.top <= ClientY && rt.bottom >= ClientY)
					{
						if (pRect->Sound_Check)
						{
							m_Sound.playeffect(UI_SOUND_ICON_NEAR);
						}
						pRect->Sound_Check = false;
						//버튼위에 마우스가 올라갔을때
						pRect->SetTransition(BUTTON_MOUSE_UP);
					}
					else
					{
						pRect->Sound_Check = true;
						pRect->SetTransition(BUTTON_MOUSE_DOWN);
					}
				}
				else
				{
					pRect->Sound_Check = true;
					pRect->SetTransition(BUTTON_MOUSE_DOWN);
				}
				//버튼 푸쉬
				if (I_Input.m_MouseState[0] == KEY_PUSH)
				{
					if (rt.left <= ClientX && rt.right >= ClientX)
					{
						if (rt.top <= ClientY&& rt.bottom >= ClientY)
						{
							m_Sound.playeffect(UI_SOUND_ICON_CLICK);
							if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[MAIN_PANEL_HOWTOPLAY])
							{
								//HOWTOPLAY버튼일때 필요한 작업 삽입
								if (iRect == MAIN_PANEL_HOWTOPLAY_BUTTON_X)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_HOWTOPLAY]->SetTransition(MAIN_PANEL_HOWTOPLAY_X_CLICKED);
								}
								else if (iRect == MAIN_PANEL_HOWTOPLAY_BUTTON_NEXT)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_HOWTOPLAY]->SetTransition(MAIN_PANEL_HOWTOPLAY_NEXT_CLICKED);
								}
								else if (iRect == MAIN_PANEL_HOWTOPLAY_BUTTON_BACK)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_HOWTOPLAY]->SetTransition(MAIN_PANEL_HOWTOPLAY_BACK_CLICKED);
								}
								return pRect;
							}
							else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[MAIN_PANEL_OPTION])
							{
								//옵션버튼일때 필요한 작업 삽입
								if (iRect == MAIN_PANEL_OPTION_BUTTON_BACK)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_OPTION]->SetTransition(MAIN_PANEL_OPTION_BACK_CLICKED);
								}
								else if (iRect == MAIN_PANEL_OPTION_BUTTON_BM_LEFT)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_OPTION]->SetTransition(MAIN_PANEL_OPTION_BM_LEFT_CLICKED);
								}
								else if (iRect == MAIN_PANEL_OPTION_BUTTON_BM_RIGHT)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_OPTION]->SetTransition(MAIN_PANEL_OPTION_BM_RIGHT_CLICKED);
								}
								else if (iRect == MAIN_PANEL_OPTION_BUTTON_SE_LEFT)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_OPTION]->SetTransition(MAIN_PANEL_OPTION_SE_LEFT_CLICKED);
								}
								else if (iRect == MAIN_PANEL_OPTION_BUTTON_SE_RIGHT)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_OPTION]->SetTransition(MAIN_PANEL_OPTION_SE_RIGHT_CLICKED);
								}
								else if (iRect == MAIN_PANEL_OPTION_BUTTON_FULLMODE)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_OPTION]->SetTransition(MAIN_PANEL_OPTION_FULLMODE_CLICKED);
								}
								else if (iRect == MAIN_PANEL_OPTION_BUTTON_WINDOWMODE)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_OPTION]->SetTransition(MAIN_PANEL_OPTION_WINDOWMODE_CLICKED);
								}
								return pRect;
							}
							else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[MAIN_PANEL_CREADIT])
							{
								//크레딧버튼일때 필요한 작업 삽입]
								if (iRect == MAIN_PANEL_CREADIT_BUTTON_X)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_CREADIT]->SetTransition(MAIN_PANEL_CREADIT_X_CLICKED);
								}
								else if (iRect == MAIN_PANEL_CREADIT_BUTTON_NEXT)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_CREADIT]->SetTransition(MAIN_PANEL_CREADIT_NEXT_CLICKED);
								}
								else if (iRect == MAIN_PANEL_CREADIT_BUTTON_BACK)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_CREADIT]->SetTransition(MAIN_PANEL_CREADIT_BACK_CLICKED);
								}
								return pRect;
							}
							else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[MAIN_PANEL_EXIT])
							{
								//EXIT버튼일때 필요한 작업 삽입
								if (iRect == MAIN_PANEL_EXIT_BUTTON_YES)
								{
									//m_pSelectScene->m_PanelList[MAIN_PANEL_EXIT]->SetTransition(MAIN_PANEL_EXIT_YES_CLICKED);
									PostQuitMessage(0);	//종료메세지
								}
								else if (iRect == MAIN_PANEL_EXIT_BUTTON_NO)
								{
									m_pSelectScene->m_PanelList[MAIN_PANEL_EXIT]->SetTransition(MAIN_PANEL_EXIT_NO_CLICKED);
								}
								return pRect;
							}
						}
					}
				}
				else
				{
					pRect->Sound_Click = true;
				}
			}
		}
	}
	else if (m_pSelectScene == m_SceneList[LOADING])	//로딩씬
	{
		for (int iRect = m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1; iRect >= 0; iRect--)
		{
			K_ControlUI* pRect = m_pSelectScene->m_pSelectPanel->m_ButtonCtl[iRect];
			RECT rt = pRect->m_rt;
			if (pRect->isRenderOk)
			{
				if (rt.left <= ClientX && rt.right >= ClientX)
				{
					if (rt.top <= ClientY && rt.bottom >= ClientY)
					{
						if (pRect->Sound_Check)
						{
							m_Sound.playeffect(UI_SOUND_ICON_NEAR);
						}
						pRect->Sound_Check = false;
						//버튼위에 마우스가 올라갔을때
						pRect->SetTransition(BUTTON_MOUSE_UP);
					}
					else
					{
						pRect->Sound_Check = true;
						pRect->SetTransition(BUTTON_MOUSE_DOWN);
					}
				}
				else
				{
					pRect->Sound_Check = true;
					pRect->SetTransition(BUTTON_MOUSE_DOWN);
				}
				//버튼 푸쉬
				if (I_Input.m_MouseState[0] == KEY_PUSH)
				{
					if (rt.left <= ClientX && rt.right >= ClientX)
					{
						if (rt.top <= ClientY&& rt.bottom >= ClientY)
						{
							m_Sound.playeffect(UI_SOUND_ICON_CLICK);
							if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[LOADING_PANEL_INFORMATION])
							{
								//HOWTOPLAY버튼일때 필요한 작업 삽입
								if (iRect == LOADING_PANEL_INFORMATION_BUTTON_BACK)
								{
									m_pSelectScene->m_PanelList[LOADING_PANEL_INFORMATION]->SetTransition(LOADING_PANEL_INFORMATION_BACK_CLICKED);
								}
								else if (iRect == LOADING_PANEL_INFORMATION_BUTTON_NEXT)
								{
									m_pSelectScene->m_PanelList[LOADING_PANEL_INFORMATION]->SetTransition(LOADING_PANEL_INFORMATION_NEXT_CLICKED);
								}
								return pRect;
							}
						}
					}
				}
				else
				{
					pRect->Sound_Click = true;
				}
			}
		}
	}
	else if (m_pSelectScene == m_SceneList[INGAME])	//인게임 씬
	{
		for (int iRect = m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1; iRect >= 0; iRect--)
		{
			K_ControlUI* pRect = m_pSelectScene->m_pSelectPanel->m_ButtonCtl[iRect];
			RECT rt = pRect->m_rt;
			if (pRect->isRenderOk && pRect->isActivateOK)
			{
				if (rt.left <= ClientX && rt.right >= ClientX)
				{
					if (rt.top <= ClientY && rt.bottom >= ClientY)
					{
						if (pRect->Sound_Check)
						{
							m_Sound.playeffect(UI_SOUND_ICON_NEAR);
						}
						pRect->Sound_Check = false;
						//버튼위에 마우스가 올라갔을때
						pRect->SetTransition(BUTTON_MOUSE_UP);
						if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_INVEN] && iRect != 0)
						{
							ButtonNum = iRect;
							//무기
							if (iRect <= INGAME_PANEL_INVEN_BUTTON_WEAPON_10)
							{
								if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * (iRect - 1)]->isRenderOk)
								{
									if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * (iRect - 1)]->m_dwCurrentState == WEAPON1_AK47)
									{
										m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->m_dwCurrentState = IMFO_AK47;
									}
									else if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * (iRect - 1)]->m_dwCurrentState == WEAPON1_HK416)
									{
										m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->m_dwCurrentState = IMFO_HK416;
									}
									else if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * (iRect - 1)]->m_dwCurrentState == WEAPON1_K2)
									{
										m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->m_dwCurrentState = IMFO_K2;
									}
								}
								else if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T2 + 7 * (iRect - 1)]->isRenderOk)
								{
									if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T2 + 7 * (iRect - 1)]->m_dwCurrentState == WEAPON4_FRAG)
									{
										m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->m_dwCurrentState = IMFO_FLAG;
									}
									else if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T2 + 7 * (iRect - 1)]->m_dwCurrentState == WEAPON4_PORTABLE)
									{
										m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->m_dwCurrentState = IMFO_PORTABLE;
									}
								}
								else if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T3 + 7 * (iRect - 1)]->isRenderOk)
								{
									if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T3 + 7 * (iRect - 1)]->m_dwCurrentState == WEAPON5_DEDONGGANG)
									{
										m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->m_dwCurrentState = IMFO_DEDONGGANG;
									}
								}
							}
							else {
								//방어구
								if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_ARMOR + (iRect - 11)]->isRenderOk)
								{
									if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_ARMOR + (iRect - 11)]->m_dwCurrentState == ARMOR_HAT)
									{
										m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->m_dwCurrentState = IMFO_HAT;
									}
									else if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_ARMOR + (iRect - 11)]->m_dwCurrentState == ARMOR_BODY)
									{
										m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->m_dwCurrentState = IMFO_BODY;
									}
								}
							}
							InvenInfoTextureRenderTick += g_fSecPerFrame;
							if (InvenInfoTextureRenderTick > 1.5f)
							{
								m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->isRenderOk = true;
								if (!InvenInfoisRender)
								{
									POINT	MousePoint;
									GetCursorPos(&MousePoint);
									ScreenToClient(m_hWnd, &MousePoint);

									RECT BBB;
									GetClientRect(m_hWnd, &BBB);
									float MOUSEX = 1920 * ((float)MousePoint.x / BBB.right);
									float MOUSEY = 1080 * ((float)MousePoint.y / BBB.bottom);

									MOUSEX = MOUSEX - (1920 / 2);
									MOUSEY = MOUSEY - (1080 / 2);
									float TextureX = 463 * ((float)BBB.right / 1920);
									float TextureY = 251 * ((float)BBB.bottom / 1080);
									m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->m_vTrans.x
										= MOUSEX - TextureX;
									m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->m_vTrans.y
										= -(MOUSEY + TextureY);
									InvenInfoisRender = true;
								}
							}
						}
					}
					else
					{
						pRect->Sound_Check = true;
						pRect->SetTransition(BUTTON_MOUSE_DOWN);
						if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_INVEN] && iRect == ButtonNum)
						{
							m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->isRenderOk = false;
							InvenInfoTextureRenderTick = 0.0f;
							InvenInfoisRender = false;
						}
					}
				}
				else
				{
					pRect->Sound_Check = true;
					pRect->SetTransition(BUTTON_MOUSE_DOWN);
					if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_INVEN] && iRect == ButtonNum)
					{
						m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_INFORMATION]->isRenderOk = false;
						InvenInfoTextureRenderTick = 0.0f;
						InvenInfoisRender = false;
					}
				}
				//버튼 푸쉬
				if (I_Input.m_MouseState[0] == KEY_PUSH)
				{
					if (rt.left <= ClientX && rt.right >= ClientX)
					{
						if (rt.top <= ClientY&& rt.bottom >= ClientY)
						{
							m_Sound.playeffect(UI_SOUND_ICON_CLICK);
							if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_NEEDTOHELP])	//NEEDTOHELP패널일때
							{
								if (iRect == INGAME_PANEL_NEEDTOHELP_BUTTON_HOWTOPLAY)
								{
									m_pSelectScene->m_pSelectPanel->SetTransition(INGAME_PANEL_HOWTOPLAY_CLICKED);
								}
								else if (iRect == INGAME_PANEL_NEEDTOHELP_BUTTON_OPTION)
								{
									m_pSelectScene->m_pSelectPanel->SetTransition(INGAME_PANEL_OPTION_CLICKED);
								}
								else if (iRect == INGAME_PANEL_NEEDTOHELP_GOTOMAIN)
								{

									SoundStopIngame();
									m_pSelectScene->m_PanelList[INGAME_PANEL_NEEDTOHELP]->SetTransition(INGAME_PANEL_NEEDTOHELP_GOTOMAIN_CLICKED);
								}
								else if (iRect == INGAME_PANEL_NEEDTOHELP_GOBACKGAME)
								{
									ShowCursor(false);
									ShowCursor(false);
									//m_Mouse.m_bRenderMouseShape = false;
									//ShowCursor(true);
									
									SoundStopIngame();
									//UI를 제외한 모든 사운드를 pause
									for (int iSound = 0; iSound < LA_SOUND2_MGR.m_iTotalCount; iSound++)
									{
										LA_SOUND2_MGR.paused(iSound);
									}
									m_pSelectScene->m_PanelList[INGAME_PANEL_NEEDTOHELP]->SetTransition(INGAME_PANEL_NEEDTOHELP_GOBACKGAME_CLICKED);
								}
								return pRect;
							}
							else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_INVEN])	//인벤 패널일때
							{
								if (iRect == INGAME_PANEL_INVEN_BUTTON_X)
								{
									ShowCursor(false);
									ShowCursor(false);
									//m_Mouse.m_bRenderMouseShape = false;
									//ShowCursor(true);
									SoundStopIngame();
									//UI를 제외한 모든 사운드를 pause
									for (int iSound = 0; iSound < LA_SOUND2_MGR.m_iTotalCount; iSound++)
									{
										LA_SOUND2_MGR.paused(iSound);
									}
									m_pSelectScene->m_PanelList[INGAME_PANEL_INVEN]->SetTransition(INGAME_PANEL_INVEN_X_CLICKED);
								}
								else if (iRect >= INGAME_PANEL_INVEN_BUTTON_WEAPON_1 &&
									iRect <= INGAME_PANEL_INVEN_BUTTON_WEAPON_10)
								{
									//무기의 장착 버튼이 눌려졌을때
									if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * (iRect - 1)]->isRenderOk)
									{
										if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * (iRect - 1)]->m_dwCurrentState == WEAPON1_AK47)
										{
											pHeroData->SetWeapon(E_InventoryWeaponType::WEAPON_AK47);
										}
										else if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * (iRect - 1)]->m_dwCurrentState == WEAPON1_HK416)
										{
											pHeroData->SetWeapon(E_InventoryWeaponType::WEAPON_H416);
										}
										else if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * (iRect - 1)]->m_dwCurrentState == WEAPON1_K2)
										{
											pHeroData->SetWeapon(E_InventoryWeaponType::WEAPON_K2);
										}
									}
									else if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T2 + 7 * (iRect - 1)]->isRenderOk)
									{
										if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T2 + 7 * (iRect - 1)]->m_dwCurrentState == WEAPON4_FRAG)
										{
											pHeroData->SetWeapon(E_InventoryWeaponType::WEAPON_FRAG_GRENADE);
										}
										else if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T2 + 7 * (iRect - 1)]->m_dwCurrentState == WEAPON4_PORTABLE)
										{
											pHeroData->SetWeapon(E_InventoryWeaponType::WEAPON_PORTABLE_GRENADE);
										}
									}
									else if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T3 + 7 * (iRect - 1)]->isRenderOk)
									{
										if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T3 + 7 * (iRect - 1)]->m_dwCurrentState == WEAPON5_DEDONGGANG)
										{
											pHeroData->SetWeapon(E_InventoryWeaponType::WEAPON_MEDIC);
										}
									}
								}
								else
								{
									if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_ARMOR + (iRect - 11)]->m_dwCurrentState == ARMOR_HAT)
									{
										//일단은 공백
									}
									else if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_ARMOR + (iRect - 11)]->m_dwCurrentState == ARMOR_BODY)
									{
										//일단은 공백
									}
								}
							}
							else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_HOWTOPLAY])
							{
								//HOWTOPLAY버튼일때 필요한 작업 삽입
								if (iRect == INGAME_PANEL_HOWTOPLAY_BUTTON_X)
								{
									m_pSelectScene->m_PanelList[INGAME_PANEL_HOWTOPLAY]->SetTransition(INGAME_PANEL_HOWTOPLAY_X_CLICKED);
								}
								else if (iRect == INGAME_PANEL_HOWTOPLAY_BUTTON_NEXT)
								{
									m_pSelectScene->m_PanelList[INGAME_PANEL_HOWTOPLAY]->SetTransition(INGAME_PANEL_HOWTOPLAY_NEXT_CLICKED);
								}
								else if (iRect == INGAME_PANEL_HOWTOPLAY_BUTTON_BACK)
								{
									m_pSelectScene->m_PanelList[INGAME_PANEL_HOWTOPLAY]->SetTransition(INGAME_PANEL_HOWTOPLAY_BACK_CLICKED);
								}
								return pRect;
							}
							else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_OPTION])
							{
								//옵션버튼일때 필요한 작업 삽입
								if (iRect == INGAME_PANEL_OPTION_BUTTON_BACK)
								{
									m_pSelectScene->m_PanelList[INGAME_PANEL_OPTION]->SetTransition(INGAME_PANEL_OPTION_BACK_CLICKED);
								}
								else if (iRect == INGAME_PANEL_OPTION_BUTTON_BM_LEFT)
								{
									m_pSelectScene->m_PanelList[INGAME_PANEL_OPTION]->SetTransition(INGAME_PANEL_OPTION_BM_LEFT_CLICKED);
								}
								else if (iRect == INGAME_PANEL_OPTION_BUTTON_BM_RIGHT)
								{
									m_pSelectScene->m_PanelList[INGAME_PANEL_OPTION]->SetTransition(INGAME_PANEL_OPTION_BM_RIGHT_CLICKED);
								}
								else if (iRect == INGAME_PANEL_OPTION_BUTTON_SE_LEFT)
								{
									m_pSelectScene->m_PanelList[INGAME_PANEL_OPTION]->SetTransition(INGAME_PANEL_OPTION_SE_LEFT_CLICKED);
								}
								else if (iRect == INGAME_PANEL_OPTION_BUTTON_SE_RIGHT)
								{
									m_pSelectScene->m_PanelList[INGAME_PANEL_OPTION]->SetTransition(INGAME_PANEL_OPTION_SE_RIGHT_CLICKED);
								}
								else if (iRect == INGAME_PANEL_OPTION_BUTTON_FULLMODE)
								{
									m_pSelectScene->m_PanelList[INGAME_PANEL_OPTION]->SetTransition(INGAME_PANEL_OPTION_FULLMODE_CLICKED);
								}
								else if (iRect == INGAME_PANEL_OPTION_BUTTON_WINDOWMODE)
								{
									m_pSelectScene->m_PanelList[INGAME_PANEL_OPTION]->SetTransition(INGAME_PANEL_OPTION_WINDOWMODE_CLICKED);
								}
								return pRect;
							}
							else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_LOSE])	//LOSE패널일때
							{
								if (iRect == INGAME_PANEL_LOSE_BUTTON_GOTOMAIN)
								{
									m_Sound.stop(UI_SOUND_INGAME_LOSE);
									m_pSelectScene->m_PanelList[INGAME_PANEL_LOSE]->SetTransition(INGAME_PANEL_LOSE_GOTOMAIN_CLICKED);
								}
							}
							else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_WIN])	//WIN패널일때
							{
								if (iRect == INGAME_PANEL_WIN_BUTTON_GOTOMAIN)
								{
									m_Sound.stop(UI_SOUND_INGAME_WIN);
									m_pSelectScene->m_PanelList[INGAME_PANEL_WIN]->SetTransition(INGAME_PANEL_WIN_GOTOMAIN_CLICKED);
								}
							}
						}
					}
				}
			}
		}
	}
	return true;
}

bool K_UIParser::SceneStateChange()
{
	if (m_pSelectScene == m_SceneList[MAIN])	//메인씬일때
	{
		m_Sound.play(UI_SOUND_TITLE);
		if (m_pSelectScene->m_dwCurrentState == MAIN_STATE_NONE)
			m_pSelectScene->m_pSelectPanel = NULL;
		else if (m_pSelectScene->m_dwCurrentState == MAIN_STATE_GAMEPLAY_SCENE_ON)
		{
			m_pSelectScene = m_SceneList[LOADING];
			m_Sound.stop(UI_SOUND_TITLE);
			m_Sound.play(UI_SOUND_LOADING);
			isGameStart = true;
		}
		else if (m_pSelectScene->m_PanelList[MAIN_PANEL_HOWTOPLAY]->m_dwCurrentState == MAIN_PANEL_ON)
		{
			isGamePanelOK = true;
			m_pSelectScene->m_pSelectPanel = m_pSelectScene->m_PanelList[MAIN_PANEL_HOWTOPLAY];
		}
		else if (m_pSelectScene->m_PanelList[MAIN_PANEL_OPTION]->m_dwCurrentState == MAIN_PANEL_ON)
		{
			isGamePanelOK = true;
			m_pSelectScene->m_pSelectPanel = m_pSelectScene->m_PanelList[MAIN_PANEL_OPTION];
		}
		else if (m_pSelectScene->m_PanelList[MAIN_PANEL_CREADIT]->m_dwCurrentState == MAIN_PANEL_ON)
		{
			isGamePanelOK = true;
			m_pSelectScene->m_pSelectPanel = m_pSelectScene->m_PanelList[MAIN_PANEL_CREADIT];
		}
		else if (m_pSelectScene->m_PanelList[MAIN_PANEL_EXIT]->m_dwCurrentState == MAIN_PANEL_ON)
		{
			isGamePanelOK = true;
			m_pSelectScene->m_pSelectPanel = m_pSelectScene->m_PanelList[MAIN_PANEL_EXIT];
		}
	}
	else if (m_pSelectScene == m_SceneList[LOADING])//로딩씬일때
	{
		isGamePanelOK = true;
		m_pSelectScene->m_pSelectPanel = m_pSelectScene->m_PanelList[LOADING_PANEL_INFORMATION];
		m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_matWorld._42 = 70.0f;
		m_Sound.play(UI_SOUND_LOADING);
		if (m_pSelectScene->m_dwCurrentState == LOADING_STATE_LOAD)
		{
			if (m_pSelectScene->m_TextureCtl[LOADING_TEXTURE_BAR]->m_rt.right > 1853)
			{
				m_pSelectScene->SetTransition(LOADING_EVENT_COMPLETE);
				return true;
			}
			float Loding = 17.0f* (g_LoadingNum / (float)100);
			if (m_pSelectScene->m_TextureCtl[LOADING_TEXTURE_BAR]->m_pPlane->m_VertexList[1].v.x < Loding)
			{
				m_pSelectScene->m_TextureCtl[LOADING_TEXTURE_BAR]->m_pPlane->m_VertexList[1].v.x += g_fSecPerFrame * 5; //10.0f;
				m_pSelectScene->m_TextureCtl[LOADING_TEXTURE_BAR]->m_Vertices[1].v.x += g_fSecPerFrame * 5;	// 10.0f;//5 * g_fSecPerFrame;
				m_pSelectScene->m_TextureCtl[LOADING_TEXTURE_BAR]->m_pPlane->m_VertexList[2].v.x += g_fSecPerFrame * 5; //= 10.0f;//5 * g_fSecPerFrame;
				m_pSelectScene->m_TextureCtl[LOADING_TEXTURE_BAR]->m_Vertices[2].v.x += g_fSecPerFrame * 5; //= 10.0f; //5 * g_fSecPerFrame;
				
				TextureSizeChange(LOADING_TEXTURE_BAR, m_pSelectScene);
			}
		}
		else if (m_pSelectScene->m_dwCurrentState == LOADING_STATE_COMPLETE)
		{
			m_pSelectScene->m_TextureCtl[LOADING_TEXTURE_WAITING]->isRenderOk = false;
			m_pSelectScene->m_TextureCtl[LOADING_TEXUTRE_COMPLETE]->isRenderOk = true;
			if (I_Input.m_MouseState[0] == KEY_PUSH)
			{
				//패널창의 이미지를 0 번으로
				m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum = 0;
				isGamePanelOK = false;

				//로딩바를 원상태로
				m_Sound.playeffect(UI_SOUND_ICON_CLICK);
				m_pSelectScene->m_TextureCtl[LOADING_TEXTURE_BAR]->m_pPlane->m_VertexList[1].v.x = -1.0f;//+= g_fSecPerFrame; //10.0f;
				m_pSelectScene->m_TextureCtl[LOADING_TEXTURE_BAR]->m_Vertices[1].v.x = -1.0f;	//+= g_fSecPerFrame;	// 10.0f;//5 * g_fSecPerFrame;
				m_pSelectScene->m_TextureCtl[LOADING_TEXTURE_BAR]->m_pPlane->m_VertexList[2].v.x = -1.0f;	// g_fSecPerFrame; //= 10.0f;//5 * g_fSecPerFrame;
				m_pSelectScene->m_TextureCtl[LOADING_TEXTURE_BAR]->m_Vertices[2].v.x = -1.0f; //+= g_fSecPerFrame; //= 10.0f; //5 * g_fSecPerFrame;

				TextureSizeChange(LOADING_TEXTURE_BAR, m_pSelectScene);

				m_pSelectScene = m_SceneList[INGAME];
				m_Sound.stop(UI_SOUND_LOADING);

				//옵션 상태값
				m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState =
					m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState;
				m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState =
					m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState;

				m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_FULLMODE_CHECK]->isRenderOk =
					m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_FULLMODE_CHECK]->isRenderOk;
				m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_WINDOWMODE_CHECK]->isRenderOk =
					m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_WINDOWMODE_CHECK]->isRenderOk;

				//마우스 커서 없어지게
				ShowCursor(false);
				ShowCursor(false);
			}
		}
	}
	else if (m_pSelectScene == m_SceneList[INGAME])	//인게임씬일때
	{
		//치트키 :: 남은 시간을 1초로
		if (I_Input.KeyCheck(DIK_F7) == KEY_PUSH)
		{
			m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_1]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
			m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
			m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
			m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_1;
			m_SecondCount = 0;
			m_TenSecondCount = 0;
		}
		//
		//에임 변경

			if (I_Input.m_MouseState[0] == KEY_HOLD)
			{
				if (pHeroData->CheckRifleIsShot())
				{
					m_AIM_TextureChangePlusTick += g_fSecPerFrame;
					if (m_AIM_TextureChangePlusTerm < m_AIM_TextureChangePlusTick)
					{
						if (m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_AIM_CROSS]->m_dwCurrentState < AIM_TEXTURE_7)
						{
							m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_AIM_CROSS]->m_dwCurrentState++;
						}
						m_AIM_TextureChangePlusTick -= m_AIM_TextureChangePlusTerm;
					}
				}
			}
			else
			{
				m_AIM_TextureChangeMinusTick += g_fSecPerFrame;
				if (m_AIM_TextureChangeMinusTerm < m_AIM_TextureChangeMinusTick)
				{
					if (m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_AIM_CROSS]->m_dwCurrentState > AIM_TEXTURE_1)
					{
						m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_AIM_CROSS]->m_dwCurrentState--;
					}
					m_AIM_TextureChangeMinusTick -= m_AIM_TextureChangeMinusTerm;
				}
			}
		//
		{
			if (pHeroData->CheckInItem())
			{
				PickUpTextureRender();
			}
			else if (!pHeroData->CheckInItem())
			{
				PickUpTextureNotRender();
			}
			//체력표시
			int CURRENT_HP = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->iCurrentCount;	//현재 수치
			if (CURRENT_HP < 0)
				CURRENT_HP = 0;
			CharacterSceneDigitStateSet(CURRENT_HP, 4, INGAME_TEXTURE_HPNUMBER_4);
			int MAX_HP = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->iMaxCount;	//맥스 수치
			CharacterSceneDigitStateSet(MAX_HP, 4, INGAME_TEXTURE_HPNUMBER_8);

			//체력바 변경
			float HPBAR_X = -1.0f + 2.0f * ((float)CURRENT_HP / (float)MAX_HP);	//남은 체력의 비율
			//앞면 체력바 변경
			{
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_HPBAR_FRONT]->m_pPlane->m_VertexList[1].v.x = HPBAR_X;
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_HPBAR_FRONT]->m_Vertices[1].v.x = HPBAR_X;
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_HPBAR_FRONT]->m_pPlane->m_VertexList[2].v.x = HPBAR_X;
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_HPBAR_FRONT]->m_Vertices[2].v.x = HPBAR_X;

				TextureSizeChange(INGAME_TEXTURE_HPBAR_FRONT, m_pSelectScene);
			}

			m_HPBAR_ChangeTick += g_fSecPerFrame;

			//0.5초후 뒷면 체력바 변경
			if (m_HPBAR_ChangeTick > 0.5f)
			{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_HPBAR_BACK]->m_pPlane->m_VertexList[1].v.x = HPBAR_X;
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_HPBAR_BACK]->m_Vertices[1].v.x = HPBAR_X;
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_HPBAR_BACK]->m_pPlane->m_VertexList[2].v.x = HPBAR_X;
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_HPBAR_BACK]->m_Vertices[2].v.x = HPBAR_X;

					TextureSizeChange(INGAME_TEXTURE_HPBAR_BACK, m_pSelectScene);
					m_HPBAR_ChangeTick = 0.0f;
			}

			//무기1표시
			if (m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::RIPLE_UI_DATA])
			{
				for (int iTexture = INGAME_TEXTURE_RIPLETEXT; iTexture <= INGAME_TEXTURE_RIPLENUMBER_6; iTexture++)
				{
					m_pSelectScene->m_TextureCtl[iTexture]->isRenderOk = true;
				}
				if (pHeroData->GetCurrenWeaponType(0) == E_InventoryWeaponType::WEAPON_AK47)
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_RIPLETEXT]->m_dwCurrentState = WEAPON1_AK47;
				}
				else if (pHeroData->GetCurrenWeaponType(0) == E_InventoryWeaponType::WEAPON_H416)
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_RIPLETEXT]->m_dwCurrentState = WEAPON1_HK416;
				}
				else if (pHeroData->GetCurrenWeaponType(0) == E_InventoryWeaponType::WEAPON_K2)
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_RIPLETEXT]->m_dwCurrentState = WEAPON1_K2;
				}
				int CURRENT_RIPLE = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::RIPLE_UI_DATA]->iCurrentCount;	//현재 수치
				CharacterSceneDigitStateSet(CURRENT_RIPLE, 3, INGAME_TEXTURE_RIPLENUMBER_3);
				int MAX_RIPLE = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::RIPLE_UI_DATA]->iMaxCount;	//맥스 수치
				CharacterSceneDigitStateSet(MAX_RIPLE, 3, INGAME_TEXTURE_RIPLENUMBER_6);

				if (CURRENT_RIPLE == 0)
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_RIPLE_RELOAD]->isRenderOk = true;
					D3DXMatrixRotationZ(&m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_RIPLE_RELOAD]->m_matRotate, 0.15f);
				}
				else
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_RIPLE_RELOAD]->isRenderOk = false;
				}
			}

			//무기2표시    
			if (m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::HANDGUN_UI_DATA])
			{
				for (int iTexture = INGAME_TEXTURE_HANDGUNTEXT; iTexture <= INGAME_TEXTURE_HANDGUNNUMBER_6; iTexture++)
				{
					m_pSelectScene->m_TextureCtl[iTexture]->isRenderOk = true;
				}
				int CURRENT_HANDGUN = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::HANDGUN_UI_DATA]->iCurrentCount;	//현재 수치
				CharacterSceneDigitStateSet(CURRENT_HANDGUN, 3, INGAME_TEXTURE_HANDGUNNUMBER_3);
				int MAX_HANDGUN = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::HANDGUN_UI_DATA]->iMaxCount;	//맥스 수치
				CharacterSceneDigitStateSet(MAX_HANDGUN, 3, INGAME_TEXTURE_HANDGUNNUMBER_6);

				if (CURRENT_HANDGUN == 0)
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_HANDGUN_RELOAD]->isRenderOk = true;
					D3DXMatrixRotationZ(&m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_HANDGUN_RELOAD]->m_matRotate, 0.15f);
				}
				else
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_HANDGUN_RELOAD]->isRenderOk = false;
				}
			}
			if (m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::KNIFE_UI_DATA])
			{
				for (int iTexture = INGAME_TEXTURE_KNIFETEXT; iTexture <= INGAME_TEXTURE_KNIFENUMBER_2; iTexture++)
				{
					m_pSelectScene->m_TextureCtl[iTexture]->isRenderOk = true;
				}
				//나이프표시
				int CURRENT_KNIFE = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::KNIFE_UI_DATA]->iCurrentCount;	//현재 수치
				CharacterSceneDigitStateSet(CURRENT_KNIFE, 1, INGAME_TEXTURE_KNIFENUMBER_1);
				int MAX_KNIFE = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::KNIFE_UI_DATA]->iMaxCount;	//맥스 수치
				CharacterSceneDigitStateSet(MAX_KNIFE, 1, INGAME_TEXTURE_KNIFENUMBER_2);
			}
			if (m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::BOMB_UI_DATA])
			{
				for (int iTexture = INGAME_TEXTURE_BOMBTEXT; iTexture <= INGAME_TEXTURE_BOMBNUMBER_2; iTexture++)
				{
					m_pSelectScene->m_TextureCtl[iTexture]->isRenderOk = true;
				}
				//폭탄표시
				if (pHeroData->GetCurrenWeaponType(3) == E_InventoryWeaponType::WEAPON_FRAG_GRENADE)
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_BOMBTEXT]->m_dwCurrentState = WEAPON4_FRAG;
				}
				else if (pHeroData->GetCurrenWeaponType(3) == E_InventoryWeaponType::WEAPON_PORTABLE_GRENADE)
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_BOMBTEXT]->m_dwCurrentState = WEAPON4_PORTABLE;
				}
				int CURRENT_BOMB = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::BOMB_UI_DATA]->iCurrentCount;	//현재 수치
				CharacterSceneDigitStateSet(CURRENT_BOMB, 1, INGAME_TEXTURE_BOMBNUMBER_1);
				int MAX_BOMB = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::BOMB_UI_DATA]->iMaxCount;	//맥스 수치
				CharacterSceneDigitStateSet(MAX_BOMB, 1, INGAME_TEXTURE_BOMBNUMBER_2);

				if (CURRENT_BOMB == 0)
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_BOMB_RELOAD]->isRenderOk = true;
					D3DXMatrixRotationZ(&m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_BOMB_RELOAD]->m_matRotate, 0.15f);
				}
				else
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_BOMB_RELOAD]->isRenderOk = false;
				}
			}
			if (m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::MEDIC_UI_DATA])
			{
				for (int iTexture = INGAME_TEXTURE_MEDICTEXT; iTexture <= INGAME_TEXTURE_MEDICNUMBER_2; iTexture++)
				{
					m_pSelectScene->m_TextureCtl[iTexture]->isRenderOk = true;
				}
				//메딕표시
				int CURRENT_MEDIC = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::MEDIC_UI_DATA]->iCurrentCount;	//현재 수치
				CharacterSceneDigitStateSet(CURRENT_MEDIC, 1, INGAME_TEXTURE_MEDICNUMBER_1);
				int MAX_MEDIC = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::MEDIC_UI_DATA]->iMaxCount;	//맥스 수치
				CharacterSceneDigitStateSet(MAX_MEDIC, 1, INGAME_TEXTURE_MEDICNUMBER_2);

				if (CURRENT_MEDIC == 0)
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_MEDIC_RELOAD]->isRenderOk = true;
					D3DXMatrixRotationZ(&m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_MEDIC_RELOAD]->m_matRotate, 0.15f);
				}
				else
				{
					m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_MEDIC_RELOAD]->isRenderOk = false;
				}
			}
		}
		if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_STAGE_1_FARMING)	//인게임의 상태가 NONE일때
		{
			FarmingOrBattle = 1;
			m_Sound.play(UI_SOUND_INGAME_FARMING);
			m_pSelectScene->m_pSelectPanel = NULL;
			m_SecondimeTick += g_fSecPerFrame;
			//부여된 시간이 전부 흘렀을때
			if (m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0)
			{
				m_PhaseChangeTick += g_fSecPerFrame;
				if (m_PhaseChangeTick > m_PhaseChangeTerm)
				{
					m_Sound.stop(UI_SOUND_INGAME_FARMING);
					m_Sound.playeffect(UI_SOUND_PHASE);
					m_pSelectScene->SetTransition(INGAME_EVENT_FARMING_END);	//상태변환
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PHASE]->m_dwCurrentState = INGAME_TEXTURE_PHASE_BATTLE;
					//시간셋팅
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_1]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_1;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SecondCount = 9;
					m_TenSecondCount = 5;
					m_PhaseChangeTick = 0;
				}
				return true;
			}
			//시간변화
			TimeStateChange();
		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_STAGE_1_BATTLE)
		{
			//캐릭터의 체력이 0과 같거나 작을때 패배
			if (m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->iCurrentCount <= 0)
			{
				m_Sound.stop(UI_SOUND_INGAME_BATTLE);
				m_pSelectScene->SetTransition(INGAME_EVENT_LOSE);
				m_pSelectScene->m_PanelList[INGAME_PANEL_LOSE]->SetTransition(INGAME_EVENT_LOSE);
				return true;
			}

			FarmingOrBattle = 2;
			m_Sound.play(UI_SOUND_INGAME_BATTLE);
			m_pSelectScene->m_pSelectPanel = NULL;
			m_SecondimeTick += g_fSecPerFrame;
			//부여된 시간이 전부 흘렀을때
			if (m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0
				)
			{
				m_PhaseChangeTick += g_fSecPerFrame;
				if (m_PhaseChangeTick > m_PhaseChangeTerm)
				{
					m_Sound.playeffect(UI_SOUND_PHASE);
					m_Sound.playeffect(UI_SOUND_INGAME_BOMB);
					m_Sound.stop(UI_SOUND_INGAME_BATTLE);
					m_pSelectScene->SetTransition(INGAME_EVENT_BATTLE_END);	//상태변환
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PHASE]->m_dwCurrentState = INGAME_TEXTURE_PHASE_FARMING;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_STAGE]->m_dwCurrentState = INGAME_TEXTURE_STAGE_2;
					//시간셋팅
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_1]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_4;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SecondCount = 9;
					m_TenSecondCount = 0;
					m_PhaseChangeTick = 0;
				}
				return true;
			}
			//시간변화
			TimeStateChange();
		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_STAGE_2_FARMING)
		{
			FarmingOrBattle = 3;
			m_Sound.play(UI_SOUND_INGAME_FARMING);
			m_Sound.play(UI_SOUND_INGAME_SNOW);
			m_pSelectScene->m_pSelectPanel = NULL;
			m_SecondimeTick += g_fSecPerFrame;
			if (m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0
				)
			{
				m_PhaseChangeTick += g_fSecPerFrame;
				if (m_PhaseChangeTick > m_PhaseChangeTerm)
				{
					m_Sound.playeffect(UI_SOUND_PHASE);
					m_Sound.stop(UI_SOUND_INGAME_FARMING);
					m_pSelectScene->SetTransition(INGAME_EVENT_FARMING_END);	//상태변환
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PHASE]->m_dwCurrentState = INGAME_TEXTURE_PHASE_BATTLE;
					//시간셋팅
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_1]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_1;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_2;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SecondCount = 9;
					m_TenSecondCount = 3;
					m_PhaseChangeTick = 0;
				}
				return true;
			}
			//시간변화
			TimeStateChange();
		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_STAGE_2_BATTLE)
		{
			//캐릭터의 체력이 0과 같거나 작을때 패배
			if (m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->iCurrentCount <= 0)
			{
				m_Sound.stop(UI_SOUND_INGAME_BATTLE);
				m_Sound.stop(UI_SOUND_INGAME_SNOW);
				m_pSelectScene->SetTransition(INGAME_EVENT_LOSE);
				m_pSelectScene->m_PanelList[INGAME_PANEL_LOSE]->SetTransition(INGAME_EVENT_LOSE);
				return true;
			}

			FarmingOrBattle = 4;
			m_Sound.play(UI_SOUND_INGAME_BATTLE);
			m_pSelectScene->m_pSelectPanel = NULL;
			m_SecondimeTick += g_fSecPerFrame;
			if (m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0
				)
			{
				m_PhaseChangeTick += g_fSecPerFrame;
				if (m_PhaseChangeTick > m_PhaseChangeTerm)
				{
					m_Sound.playeffect(UI_SOUND_PHASE);
					m_Sound.playeffect(UI_SOUND_INGAME_BOMB);
					m_Sound.stop(UI_SOUND_INGAME_BATTLE);
					m_Sound.stop(UI_SOUND_INGAME_SNOW);
					m_pSelectScene->SetTransition(INGAME_EVENT_BATTLE_END);	//상태변환
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PHASE]->m_dwCurrentState = INGAME_TEXTURE_PHASE_FARMING;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_STAGE]->m_dwCurrentState = INGAME_TEXTURE_STAGE_3;
					//시간셋팅
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_1]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_5;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SecondCount = 9;
					m_TenSecondCount = 0;
					m_PhaseChangeTick = 0;
				}
				return true;
			}
			TimeStateChange();
		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_STAGE_3_FARMING)
		{
			FarmingOrBattle = 5;
			m_Sound.play(UI_SOUND_INGAME_FARMING_2);
			m_pSelectScene->m_pSelectPanel = NULL;
			m_SecondimeTick += g_fSecPerFrame;
			if (m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0
				)
			{
				m_PhaseChangeTick += g_fSecPerFrame;
				if (m_PhaseChangeTick > m_PhaseChangeTerm)
				{
					m_Sound.playeffect(UI_SOUND_PHASE);
					m_Sound.stop(UI_SOUND_INGAME_FARMING_2);
					m_pSelectScene->SetTransition(INGAME_EVENT_FARMING_END);	//상태변환
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PHASE]->m_dwCurrentState = INGAME_TEXTURE_PHASE_BATTLE;
					//시간셋팅
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_1]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_1;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_4;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SecondCount = 9;
					m_TenSecondCount = 1;
					m_PhaseChangeTick = 0;
				}
				return true;
			}
			//시간변화
			TimeStateChange();
		}
		else if (m_pSelectScene->m_dwCurrentState == INGMAE_STATE_STAGE_3_BATTLE)
		{
			//캐릭터의 체력이 0과 같거나 작을때 패배
			if (m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->iCurrentCount <= 0)
			{
				m_Sound.stop(UI_SOUND_INGAME_BATTLE_2);
				m_pSelectScene->SetTransition(INGAME_EVENT_LOSE);
				m_pSelectScene->m_PanelList[INGAME_PANEL_LOSE]->SetTransition(INGAME_EVENT_LOSE);
				return true;
			}
			FarmingOrBattle = 6;
			m_Sound.play(UI_SOUND_INGAME_BATTLE_2);
			m_pSelectScene->m_pSelectPanel = NULL;
			m_SecondimeTick += g_fSecPerFrame;
			if (m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0
				)
			{
				m_PhaseChangeTick += g_fSecPerFrame;
				if (m_PhaseChangeTick > m_PhaseChangeTerm)
				{
					m_Sound.stop(UI_SOUND_INGAME_BATTLE_2);
					m_Sound.playeffect(UI_SOUND_PHASE);
					m_Sound.playeffect(UI_SOUND_INGAME_BOMB);
					m_pSelectScene->SetTransition(INGAME_EVENT_BATTLE_END);	//상태변환
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PHASE]->m_dwCurrentState = INGAME_TEXTURE_PHASE_FARMING;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_STAGE]->m_dwCurrentState = INGAME_TEXTURE_STAGE_4;
					//시간셋팅
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_1]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_1;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SecondCount = 9;
					m_TenSecondCount = 5;
					m_PhaseChangeTick = 0;
				}
				return true;
			}
			TimeStateChange();
		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_STAGE_4_FARMING)
		{
			FarmingOrBattle = 7;
			m_Sound.play(UI_SOUND_INGAME_FARMING_3);
			m_pSelectScene->m_pSelectPanel = NULL;
			m_SecondimeTick += g_fSecPerFrame;
			if (m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0
				)
			{
				m_PhaseChangeTick += g_fSecPerFrame;
				if (m_PhaseChangeTick > m_PhaseChangeTerm)
				{
					m_Sound.stop(UI_SOUND_INGAME_FARMING_3);
					m_Sound.playeffect(UI_SOUND_PHASE);
					m_pSelectScene->SetTransition(INGAME_EVENT_FARMING_END);	//상태변환
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_PHASE]->m_dwCurrentState = INGAME_TEXTURE_PHASE_BATTLE;
					//시간셋팅
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_1]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_2;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState = INGAME_TEXTURE_TIME_1_STATE_0;
					m_SecondCount = 9;
					m_TenSecondCount = 5;
					m_PhaseChangeTick = 0;
				}
				return true;
			}
			//시간변화
			TimeStateChange();
		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_STAGE_4_BATTLE)
		{
			//캐릭터의 체력이 0과 같거나 작을때 패배
			if (m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->iCurrentCount <= 0)
			{
				m_Sound.stop(UI_SOUND_INGAME_BATTLE_3);
				m_pSelectScene->SetTransition(INGAME_EVENT_LOSE);
				m_pSelectScene->m_PanelList[INGAME_PANEL_LOSE]->SetTransition(INGAME_EVENT_LOSE);
				return true;
			}

			FarmingOrBattle = 8;
			m_Sound.play(UI_SOUND_INGAME_BATTLE_3);
			m_pSelectScene->m_pSelectPanel = NULL;
			m_SecondimeTick += g_fSecPerFrame;
			if (m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_4]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_3]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0 &&
				m_pSelectScene->m_TextureCtl[INGAME_TEXTURE_TIME_2]->m_dwCurrentState == INGAME_TEXTURE_TIME_1_STATE_0
				)
			{
				m_Sound.stop(UI_SOUND_INGAME_BATTLE_3);
				m_pSelectScene->SetTransition(INGAME_EVENT_WIN);	//상태변환
				m_pSelectScene->m_PanelList[INGAME_PANEL_WIN]->SetTransition(INGAME_EVENT_WIN);
				return true;
			}
			TimeStateChange();
		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_NEEDTOHELP_PANEL_ON)
		{
			isGamePanelOK = true;
			m_pSelectScene->m_pSelectPanel = m_pSelectScene->m_PanelList[INGAME_PANEL_NEEDTOHELP];
		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_INVEN_PANEL_ON)
		{
			isGamePanelOK = true;
			m_pSelectScene->m_pSelectPanel = m_pSelectScene->m_PanelList[INGAME_PANEL_INVEN];
		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_HOWTOPLAY_PANEL_ON)
		{
			isGamePanelOK = true;
			m_pSelectScene->m_pSelectPanel = m_pSelectScene->m_PanelList[INGAME_PANEL_HOWTOPLAY];
		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_OPTION_PANEL_ON)
		{
			isGamePanelOK = true;
			m_pSelectScene->m_pSelectPanel = m_pSelectScene->m_PanelList[INGAME_PANEL_OPTION];
		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_WIN_PANEL_ON)
		{
			if (MouseCursorOn)
			{
				ShowCursor(true);
				ShowCursor(true);
				//m_Mouse.m_bRenderMouseShape = true;
				MouseCursorOn = false;
			}
			//모든 소리를 스탑
			for (int iSound = 0; iSound < LA_SOUND2_MGR.m_iTotalCount; iSound++)
				LA_SOUND2_MGR.stop(iSound);
			isGamePanelOK = true;
			m_pSelectScene->m_pSelectPanel = m_pSelectScene->m_PanelList[INGAME_PANEL_WIN];
			m_Sound.play(UI_SOUND_INGAME_WIN);
		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_LOSE_PANEL_ON)
		{
			if (MouseCursorOn)
			{
				ShowCursor(true);
				ShowCursor(true);
				//m_Mouse.m_bRenderMouseShape = true;
				MouseCursorOn = false;
			}
			//모든 소리를 스탑
			for (int iSound = 0; iSound < LA_SOUND2_MGR.m_iTotalCount; iSound++)
				LA_SOUND2_MGR.stop(iSound);
			isGamePanelOK = true;
			m_pSelectScene->m_pSelectPanel = m_pSelectScene->m_PanelList[INGAME_PANEL_LOSE];
			m_Sound.play(UI_SOUND_INGAME_LOSE);

		}
		else if (m_pSelectScene->m_dwCurrentState == INGAME_STATE_BACKTOMAIN)
		{

			m_pSelectScene = m_SceneList[MAIN];
		}
		//상태 더 추가
	}
	return true;
}
bool K_UIParser::PanelStateChange()
{
	if (m_pSelectScene == m_SceneList[MAIN])	//메인씬일때
	{
		if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[MAIN_PANEL_HOWTOPLAY])	//메인 씬의 HOWTOPLAY패널
		{
			//6번 일때 Next버튼 안보이게
			if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum == 6)
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[MAIN_PANEL_HOWTOPLAY_BUTTON_NEXT]->isRenderOk = false;
			else
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[MAIN_PANEL_HOWTOPLAY_BUTTON_NEXT]->isRenderOk = true;
			//0번 일때 Back버튼 안보이게
			if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum == 0)
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[MAIN_PANEL_HOWTOPLAY_BUTTON_BACK]->isRenderOk = false;
			else
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[MAIN_PANEL_HOWTOPLAY_BUTTON_BACK]->isRenderOk = true;
			if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_PANEL_OFF)
			{
				m_pSelectScene->SetTransition(MAIN_PANEL_HOWTOPLAY_OFF);
				m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum = 0;
				isGamePanelOK = false;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_HOWTOPLAY_PANEL_NEXT)
			{
				if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum != 6)
					m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum++;
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_HOWTOPLAY_PANEL_BACK)
			{
				if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum != 0)
					m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum--;
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
		}
		else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[MAIN_PANEL_OPTION])	//메인 씬의 옵션패널
		{
			if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_PANEL_OFF)
			{
				m_pSelectScene->SetTransition(MAIN_PANEL_OPTION_OFF);
				isGamePanelOK = false;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_OPTION_PANEL_BM_LEFT)
			{
				if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState != 0)
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState--;
				BackgroundMusicSet(m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState);
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_OPTION_PANEL_BM_RIGHT)
			{
				if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState != 9)
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState++;
				BackgroundMusicSet(m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState);
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_OPTION_PANEL_SE_LEFT)
			{
				if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState != 0)
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState--;
				SoundEffectSet(m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState);
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_OPTION_PANEL_SE_RIGHT)
			{
				if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState != 9)
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState++;
				SoundEffectSet(m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState);
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_OPTION_PANEL_FULLMODE)
			{
				SoundEffectSet(m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState);
				g_pSwapChain->SetFullscreenState(TRUE, NULL);
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_FULLMODE_CHECK]->isRenderOk = true;
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_WINDOWMODE_CHECK]->isRenderOk = false;
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_OPTION_PANEL_WINDOWMODE)
			{
				SoundEffectSet(m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState);
				g_pSwapChain->SetFullscreenState(FALSE, NULL);
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_FULLMODE_CHECK]->isRenderOk = false;
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_WINDOWMODE_CHECK]->isRenderOk = true;
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
		}
		else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[MAIN_PANEL_CREADIT])	//메인 씬의 크레딧패널
		{
			//6번 일때 Next버튼 안보이게
			if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum == 5)
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[MAIN_PANEL_CREADIT_BUTTON_NEXT]->isRenderOk = false;
			else
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[MAIN_PANEL_CREADIT_BUTTON_NEXT]->isRenderOk = true;
			//0번 일때 Back버튼 안보이게
			if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum == 0)
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[MAIN_PANEL_CREADIT_BUTTON_BACK]->isRenderOk = false;
			else
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[MAIN_PANEL_CREADIT_BUTTON_BACK]->isRenderOk = true;
			if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_PANEL_OFF)
			{
				m_pSelectScene->SetTransition(MAIN_PANEL_CREADIT_OFF);
				m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum = 0;
				isGamePanelOK = false;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_CREADIT_PANEL_NEXT)
			{
				if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum != 5)
					m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum++;
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_CREADIT_PANEL_BACK)
			{
				if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum != 0)
					m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum--;
				//else if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum == 0)
				//	m_pSelectScene->m_pSelectPanel->m_ButtonCtl[MAIN_PANEL_CREADIT_BUTTON_BACK]->isRenderOk = false;
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
		}
		else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[MAIN_PANEL_EXIT])	//메인 씬의 EXIT패널
		{
			if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == MAIN_PANEL_OFF)
			{
				m_pSelectScene->SetTransition(MAIN_PANEL_EXIT_OFF);
				isGamePanelOK = false;
			}
		}
	}
	else if (m_pSelectScene == m_SceneList[LOADING])	//로딩씬일때
	{
		if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[LOADING_PANEL_INFORMATION])	//로딩 씬의 INFORMATION패널
		{
			//6번 일때 Next버튼 안보이게
			if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum == 12)
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[LOADING_PANEL_INFORMATION_BUTTON_NEXT]->isRenderOk = false;
			else
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[LOADING_PANEL_INFORMATION_BUTTON_NEXT]->isRenderOk = true;
			//0번 일때 Back버튼 안보이게
			if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum == 0)
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[LOADING_PANEL_INFORMATION_BUTTON_BACK]->isRenderOk = false;
			else
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[LOADING_PANEL_INFORMATION_BUTTON_BACK]->isRenderOk = true;

			if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == LOADING_PANEL_INFORMATION_NEXT)
			{
				if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum != 12)
					m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum++;
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = LOADING_PANEL_INFORMATION_ON;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == LOADING_PANEL_INFORMATION_BACK)
			{
				if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum != 0)
					m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum--;
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = LOADING_PANEL_INFORMATION_ON;
			}
		}
	}
	else if (m_pSelectScene == m_SceneList[INGAME])	//인게임씬일때
	{
		if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_NEEDTOHELP])	//인게임 씬의 NEEDTOHELP패널
		{
			if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_PANEL_OFF)
			{
				if (FarmingOrBattle == 1)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_1_FARMING;
				else if (FarmingOrBattle == 2)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_1_BATTLE;
				else if (FarmingOrBattle == 3)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_2_FARMING;
				else if (FarmingOrBattle == 4)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_2_BATTLE;
				else if (FarmingOrBattle == 5)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_3_FARMING;
				else if (FarmingOrBattle == 6)
					m_pSelectScene->m_dwCurrentState = INGMAE_STATE_STAGE_3_BATTLE;
				else if (FarmingOrBattle == 7)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_4_FARMING;
				else if (FarmingOrBattle == 8)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_4_BATTLE;
				isGamePanelOK = false;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_PANEL_NEEDTOHELP_HOWTOPLAY_ON)
			{
				m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_HOWTOPLAY]->SetTransition(INGAME_PANEL_HOWTOPLAY_CLICKED);
				m_pSelectScene->m_dwCurrentState = INGAME_STATE_HOWTOPLAY_PANEL_ON;
				isGamePanelOK = false;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_PANEL_NEEDTOHELP_OPTION_ON)
			{
				m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->SetTransition(INGAME_PANEL_OPTION_CLICKED);
				m_pSelectScene->m_dwCurrentState = INGAME_STATE_OPTION_PANEL_ON;
				isGamePanelOK = false;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_PANEL_GOTOMAIN)
			{
				m_pSelectScene->SetTransition(INGAME_EVENT_BACKTOMAIN);
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = INGAME_PANEL_OFF;
				isGamePanelOK = false;
			}
		}
		else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_INVEN])
		{
			//캐릭터 능력치 표시
			int CURRENT_HP = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->fHeroHp;
			CharacterPanelDigitStateSet(CURRENT_HP, 4, INGAME_PANEL_INVEN_TEXTURE_HP_4);
			int CURRENT_DEF = (int)m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->fHeroDef;
			CharacterPanelDigitStateSet(CURRENT_DEF, 3, INGAME_PANEL_INVEN_TEXTURE_ARMOR_3);
			int CURRENT_SPEED = (int)m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->fHeroSpeed;
			CharacterPanelDigitStateSet(CURRENT_SPEED, 3, INGAME_PANEL_INVEN_TEXTURE_MOVE_3);

			//착용하고있는 무기 표시
			//무기1
			if (m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::RIPLE_UI_DATA])
			{
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINRIPLE_T]->isRenderOk = true;
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINRIPLE_SHOOT]->isRenderOk = true;
				if (pHeroData->GetCurrenWeaponType(0) == E_InventoryWeaponType::WEAPON_AK47)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINRIPLE_T]->m_dwCurrentState = WEAPON1_AK47;
				}
				else if (pHeroData->GetCurrenWeaponType(0) == E_InventoryWeaponType::WEAPON_H416)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINRIPLE_T]->m_dwCurrentState = WEAPON1_HK416;
				}
				else if (pHeroData->GetCurrenWeaponType(0) == E_InventoryWeaponType::WEAPON_K2)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINRIPLE_T]->m_dwCurrentState = WEAPON1_K2;
				}
				int CURRENT_RIPLE = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::RIPLE_UI_DATA]->iMaxCount;	//맥스 수치
				CharacterPanelDigitStateSet(CURRENT_RIPLE, 3, INGAME_PANEL_INVEN_TEXTURE_MAINRIPLE_3);
			}
			//무기4
			if (m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::BOMB_UI_DATA])
			{
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINBOMB_T]->isRenderOk = true;
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINBOMB_SHOOT]->isRenderOk = true;
				if (pHeroData->GetCurrenWeaponType(3) == E_InventoryWeaponType::WEAPON_FRAG_GRENADE)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINBOMB_T]->m_dwCurrentState = WEAPON4_FRAG;
				}
				else if (pHeroData->GetCurrenWeaponType(3) == E_InventoryWeaponType::WEAPON_PORTABLE_GRENADE)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINBOMB_T]->m_dwCurrentState = WEAPON4_PORTABLE;
				}
				int CURRENT_BOMB = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::BOMB_UI_DATA]->iMaxCount;	//맥스 수치
				CharacterPanelDigitStateSet(CURRENT_BOMB, 3, INGAME_PANEL_INVEN_TEXTURE_MAINBOMB_3);
			}
			//무기5
			if (m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::MEDIC_UI_DATA])
			{
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINMEDIC_T]->isRenderOk = true;
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINMEDIC_SHOOT]->isRenderOk = true;
				if (pHeroData->GetCurrenWeaponType(3) == E_InventoryWeaponType::WEAPON_MEDIC)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINMEDIC_T]->m_dwCurrentState = WEAPON5_DEDONGGANG;
				}
				int CURRENT_MEDIC = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::MEDIC_UI_DATA]->iMaxCount;	//맥스 수치
				CharacterPanelDigitStateSet(CURRENT_MEDIC, 3, INGAME_PANEL_INVEN_TEXTURE_MAINMEDIC_3);
			}
			//인벤창에 아이템 표시
			for (int iWeapon = 0; iWeapon < m_pInventoryData->m_pInvenWeaponList.size(); iWeapon++)
			{
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[INGAME_PANEL_INVEN_BUTTON_WEAPON_1 + iWeapon]->isActivateOK = true;
				if (m_pInventoryData->m_pInvenWeaponList[iWeapon]->GetWeaponType() == E_InventoryWeaponType::WEAPON_AK47)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * iWeapon]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_SHOOT + 7 * iWeapon]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * iWeapon]->m_dwCurrentState = WEAPON1_AK47;
					int NumOfBullet = m_pInventoryData->dwTotalCount_AK47;
					CharacterPanelDigitStateSet(NumOfBullet, 3, INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_3 + 7 * iWeapon);
				}
				else if (m_pInventoryData->m_pInvenWeaponList[iWeapon]->GetWeaponType() == E_InventoryWeaponType::WEAPON_K2)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * iWeapon]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_SHOOT + 7 * iWeapon]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * iWeapon]->m_dwCurrentState = WEAPON1_K2;
					int NumOfBullet = m_pInventoryData->dwTotalCount_K2;
					CharacterPanelDigitStateSet(NumOfBullet, 3, INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_3 + 7 * iWeapon);
				}
				else if (m_pInventoryData->m_pInvenWeaponList[iWeapon]->GetWeaponType() == E_InventoryWeaponType::WEAPON_H416)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * iWeapon]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_SHOOT + 7 * iWeapon]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T1 + 7 * iWeapon]->m_dwCurrentState = WEAPON1_HK416;
					int NumOfBullet = m_pInventoryData->dwTotalCount_HK416;
					CharacterPanelDigitStateSet(NumOfBullet, 3, INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_3 + 7 * iWeapon);
				}
				else if (m_pInventoryData->m_pInvenWeaponList[iWeapon]->GetWeaponType() == E_InventoryWeaponType::WEAPON_FRAG_GRENADE)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T2 + 7 * iWeapon]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_SHOOT + 7 * iWeapon]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T2 + 7 * iWeapon]->m_dwCurrentState = WEAPON4_FRAG;
					int NumOfBullet = m_pInventoryData->dwTotalCount_FRAG_GRENADE;
					CharacterPanelDigitStateSet(NumOfBullet, 3, INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_3 + 7 * iWeapon);
				}
				else if (m_pInventoryData->m_pInvenWeaponList[iWeapon]->GetWeaponType() == E_InventoryWeaponType::WEAPON_PORTABLE_GRENADE)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T2 + 7 * iWeapon]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_SHOOT + 7 * iWeapon]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T2 + 7 * iWeapon]->m_dwCurrentState = WEAPON4_PORTABLE;
					int NumOfBullet = m_pInventoryData->dwTotalCount_PORTABLE_GRENADE;
					CharacterPanelDigitStateSet(NumOfBullet, 3, INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_3 + 7 * iWeapon);
				}
				else if (m_pInventoryData->m_pInvenWeaponList[iWeapon]->GetWeaponType() == E_InventoryWeaponType::WEAPON_MEDIC)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T3 + 7 * iWeapon]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_SHOOT + 7 * iWeapon]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_T3 + 7 * iWeapon]->m_dwCurrentState = WEAPON5_DEDONGGANG;
					int NumOfBullet = m_pInventoryData->dwTotalCount_MEDICBOX;
					CharacterPanelDigitStateSet(NumOfBullet, 3, INGAME_PANEL_INVEN_TEXTURE_CHOOSE_WEAPON_3 + 7 * iWeapon);
				}

			}
			for (int iArmor = 0; iArmor < m_pInventoryData->m_pInvenArmorList.size(); iArmor++)
			{
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[INGAME_PANEL_INVEN_BUTTON_ARMOR_1 + iArmor]->isActivateOK = true;
				if (m_pInventoryData->m_pInvenArmorList[iArmor]->GetArmorType() == E_InventoryArmorType::ARMOR_HEAD)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_ARMOR + iArmor]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_ARMOR + iArmor]->m_dwCurrentState = ARMOR_HAT;
					//착용 활성화
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINHAT]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_ARMOR_PLUS]->isRenderOk = true;
					int PlusDef = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->fHeroTotalDef -
						m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->fHeroDef;
					CharacterPanelDigitStateSet(PlusDef, 3, INGAME_PANEL_INVEN_TEXTURE_ARMOR_PLUS_3);
				}
				else if (m_pInventoryData->m_pInvenArmorList[iArmor]->GetArmorType() == E_InventoryArmorType::ARMOR_BODY)
				{
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_ARMOR + iArmor]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_CHOOSE_ARMOR + iArmor]->m_dwCurrentState = ARMOR_BODY;
					//착용 활성화
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_MAINBODY]->isRenderOk = true;
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_INVEN_TEXTURE_HP_PLUS]->isRenderOk = true;
					int PlusHP = m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->fHeroTotalHp
						- m_pCharacterUIData->CharacterUIData[(int)E_LACharacterUiDataType::CHARACTER_UI_DATA]->fHeroHp;
					CharacterPanelDigitStateSet(PlusHP, 3, INGAME_PANEL_INVEN_TEXTURE_HP_PLUS_3);
				}
			}
			if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_PANEL_OFF)
			{
				if (FarmingOrBattle == 1)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_1_FARMING;
				else if (FarmingOrBattle == 2)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_1_BATTLE;
				else if (FarmingOrBattle == 3)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_2_FARMING;
				else if (FarmingOrBattle == 4)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_2_BATTLE;
				else if (FarmingOrBattle == 5)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_3_FARMING;
				else if (FarmingOrBattle == 6)
					m_pSelectScene->m_dwCurrentState = INGMAE_STATE_STAGE_3_BATTLE;
				else if (FarmingOrBattle == 7)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_4_FARMING;
				else if (FarmingOrBattle == 8)
					m_pSelectScene->m_dwCurrentState = INGAME_STATE_STAGE_4_BATTLE;
				isGamePanelOK = false;
			}
		}
		else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_HOWTOPLAY])
		{
			//6번 일때 Next버튼 안보이게
			if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum == 6)
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[INGAME_PANEL_HOWTOPLAY_BUTTON_NEXT]->isRenderOk = false;
			else
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[INGAME_PANEL_HOWTOPLAY_BUTTON_NEXT]->isRenderOk = true;
			//0번 일때 Back버튼 안보이게
			if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum == 0)
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[INGAME_PANEL_HOWTOPLAY_BUTTON_BACK]->isRenderOk = false;
			else
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[INGAME_PANEL_HOWTOPLAY_BUTTON_BACK]->isRenderOk = true;
			if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_PANEL_OFF)
			{
				m_pSelectScene->m_dwCurrentState = INGAME_STATE_NEEDTOHELP_PANEL_ON;
				m_pSelectScene->m_PanelList[INGAME_PANEL_NEEDTOHELP]->m_dwCurrentState = INGAME_PANEL_ON;
				m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum = 0;
				isGamePanelOK = false;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_HOWTOPLAY_PANEL_NEXT)
			{
				if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum != 6)
					m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum++;
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_HOWTOPLAY_PANEL_BACK)
			{
				if (m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum != 0)
					m_pSelectScene->m_pSelectPanel->m_ImageCtl[0]->m_TexutreNum--;
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
		}
		else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_OPTION])
		{
			if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_PANEL_OFF)
			{
				m_pSelectScene->m_dwCurrentState = INGAME_STATE_NEEDTOHELP_PANEL_ON;
				m_pSelectScene->m_PanelList[INGAME_PANEL_NEEDTOHELP]->m_dwCurrentState = INGAME_PANEL_ON;
				isGamePanelOK = false;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_OPTION_PANEL_BM_LEFT)
			{
				if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState != 0)
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState--;
				BackgroundMusicSet(m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState);
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_OPTION_PANEL_BM_RIGHT)
			{
				if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState != 9)
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState++;
				BackgroundMusicSet(m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState);
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_OPTION_PANEL_SE_LEFT)
			{
				if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState != 0)
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState--;
				SoundEffectSet(m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState);
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_OPTION_PANEL_SE_RIGHT)
			{
				if (m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState != 9)
					m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState++;
				SoundEffectSet(m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState);
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_OPTION_PANEL_FULLMODE)
			{
				SoundEffectSet(m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState);
				g_pSwapChain->SetFullscreenState(TRUE, NULL);
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_FULLMODE_CHECK]->isRenderOk = true;
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_WINDOWMODE_CHECK]->isRenderOk = false;
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
			else if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_OPTION_PANEL_WINDOWMODE)
			{
				SoundEffectSet(m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState);
				g_pSwapChain->SetFullscreenState(FALSE, NULL);
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_FULLMODE_CHECK]->isRenderOk = false;
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_WINDOWMODE_CHECK]->isRenderOk = true;
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = 0;
			}
		}
		else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_LOSE])
		{
			if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_PANEL_GOTOMAIN)
			{
				m_pSelectScene->SetTransition(INGAME_EVENT_BACKTOMAIN);
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = INGAME_PANEL_OFF;
				isGamePanelOK = false;
			}
		}
		else if (m_pSelectScene->m_pSelectPanel == m_pSelectScene->m_PanelList[INGAME_PANEL_WIN])
		{
			if (m_pSelectScene->m_pSelectPanel->m_dwCurrentState == INGAME_PANEL_GOTOMAIN)
			{
				m_pSelectScene->SetTransition(INGAME_EVENT_BACKTOMAIN);
				m_pSelectScene->m_pSelectPanel->m_dwCurrentState = INGAME_PANEL_OFF;
				isGamePanelOK = false;
			}
		}
	}
	return true;
}
K_ControlUI* K_UIParser::SelectRect()
{
	POINT mouse;
	GetCursorPos(&mouse);	//마우스 포인터 위치
	ScreenToClient(m_hWnd, &mouse);

	RECT AAA;
	GetClientRect(m_hWnd, &AAA);
	float ClientX = 1920 * ((float)mouse.x / AAA.right);
	float ClientY = 1080 * ((float)mouse.y / AAA.bottom);
	//mouse.x = 1920 * (mouse.x / AAA.right);
	if (m_pSelectScene)
	{
		if (isToolPanelOK == true && m_pSelectScene->m_pSelectPanel != NULL)
		{
			//버튼일경우
			for (int iRect = m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1; iRect >= 0; iRect--)
			{
				K_ControlUI* pRect = m_pSelectScene->m_pSelectPanel->m_ButtonCtl[iRect];
				RECT rt = pRect->m_rt;
				if (rt.left <= ClientX && rt.right >= ClientX)
				{
					if (rt.top <= ClientY && rt.bottom >= ClientY)
					{
						pRect->SetTransition(BUTTON_MOUSE_UP);
					}
					else
					{
						pRect->SetTransition(BUTTON_MOUSE_DOWN);
					}
				}
				else
				{
					pRect->SetTransition(BUTTON_MOUSE_DOWN);
				}

				if (I_Input.m_MouseState[0] == KEY_PUSH)
				{
					if (rt.left <= ClientX && rt.right >= ClientX)
					{
						if (rt.top <= ClientY && rt.bottom >= ClientY)
						{
							return pRect;
						}
					}
				}
			}
			//에디트일경우
			for (int iRect = m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1; iRect >= 0; iRect--)
			{
				K_ControlUI* pRect = m_pSelectScene->m_pSelectPanel->m_EditCtl[iRect];
				RECT rt = pRect->m_rt;
				if (I_Input.m_MouseState[0] == KEY_PUSH)
				{
					if (rt.left <= mouse.x && rt.right >= mouse.x)
					{
						if (rt.top <= ClientY && rt.bottom >= ClientY)
						{
							return pRect;
						}
					}
				}
			}
			//텍스쳐일 경우
			for (int iRect = m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1; iRect >= 0; iRect--)
			{
				K_ControlUI* pRect = m_pSelectScene->m_pSelectPanel->m_TextureCtl[iRect];
				RECT rt = pRect->m_rt;
				if (I_Input.m_MouseState[0] == KEY_PUSH)
				{
					if (rt.left <= ClientX && rt.right >= ClientX)
					{
						if (rt.top <= ClientY && rt.bottom >= ClientY)
						{
							return pRect;
						}
					}
				}
			}
		}
		else
		{//버튼일경우
			for (int iRect = m_pSelectScene->m_ButtonCtl.size() - 1; iRect >= 0; iRect--)
			{
				K_ControlUI* pRect = m_pSelectScene->m_ButtonCtl[iRect];
				RECT rt = pRect->m_rt;
				if (rt.left <= ClientX && rt.right >= ClientX)
				{
					if (rt.top <= ClientY && rt.bottom >= ClientY)
					{
						//버튼위에 마우스가 올라갔을때
						pRect->SetTransition(BUTTON_MOUSE_UP);
					}
					else
					{
						pRect->SetTransition(BUTTON_MOUSE_DOWN);
					}
				}
				else
				{
					pRect->SetTransition(BUTTON_MOUSE_DOWN);
				}

				if (I_Input.m_MouseState[0] == KEY_PUSH)
				{
					if (rt.left <= ClientX && rt.right >= ClientX)
					{
						if (rt.top <= ClientY && rt.bottom >= ClientY)
						{
							return pRect;
						}
					}
				}
			}
			//에디트일경우
			for (int iRect = m_pSelectScene->m_EditCtl.size() - 1; iRect >= 0; iRect--)
			{
				K_ControlUI* pRect = m_pSelectScene->m_EditCtl[iRect];
				RECT rt = pRect->m_rt;
				if (I_Input.m_MouseState[0] == KEY_PUSH)
				{
					if (rt.left <= ClientX && rt.right >= ClientX)
					{
						if (rt.top <= ClientY && rt.bottom >= ClientY)
						{
							return pRect;
						}
					}
				}
			}
			//텍스쳐일경우
			for (int iRect = m_pSelectScene->m_TextureCtl.size() - 1; iRect >= 0; iRect--)
			{
				K_ControlUI* pRect = m_pSelectScene->m_TextureCtl[iRect];
				RECT rt = pRect->m_rt;
				if (I_Input.m_MouseState[0] == KEY_PUSH)
				{
					if (rt.left <= ClientX && rt.right >= ClientX)
					{
						if (rt.top <= ClientY && rt.bottom >= ClientY)
						{
							return pRect;
						}
					}
				}
			}
		}
	}
	return NULL;
}

//파일 로드 함수
bool K_UIParser::Load(TCHAR* szFileName)
{
	isRealGame = true;
	//파일 오픈
	m_pStream = _tfopen(szFileName, _T("rt"));
	if (m_pStream == NULL)
	{
		return false;
	}
	//TEXT파일을 이용해서 그 안의 정보를 통해 UI를 생성
	while (!feof(m_pStream))
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, (TCHAR*)(LPCTSTR)LoadSectionToken[SECTION_SCENE]))
		{
			LoadScene();
		}
	}
	fclose(m_pStream);
	return true;
}

bool K_UIParser::LoadScene()
{
	bool bLoad = true;
	_fgetts(m_pBuffer, 256, m_pStream);
	_stscanf(m_pBuffer, _T("%s"), m_pString);

	K_SceneUI* Scene = new K_SceneUI();	//새로운 씬을 생성
	m_pSelectScene = Scene;
	AddRect(K_IMAGE, NULL, 1920 / 2 - 1.0f, 1080 / 2 - 1.0f, 0);
	m_pSelectScene->m_ImageCtl[m_pSelectScene->m_ImageCtl.size() - 1]->m_pPlane->MultiTextureLoad(m_pString);
	m_pSelectScene->m_ImageCtl[m_pSelectScene->m_ImageCtl.size() - 1]->m_pPlane->LoadTexture();
	//리스트에 추가
	m_SceneList.push_back(Scene);
	while (bLoad)
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, LoadSectionToken[SECTION_PANEL]))
		{
			LoadPanel();
		}
		else if (!_tcsicmp(m_pString, LoadSectionToken[SECTION_BUTTON]))
		{
			LoadButton();
		}
		else if (!_tcsicmp(m_pString, LoadSectionToken[SECTION_EDIT]))
		{
			LoadEdit();
		}
		else if (!_tcsicmp(m_pString, LoadSectionToken[SECTION_TEXTURE]))
		{
			LoadTexture();
		}
		else if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_END]))
		{
			break;
		}
	}
	return true;
}
bool K_UIParser::LoadPanel()
{
	bool bLoad = true;
	_fgetts(m_pBuffer, 256, m_pStream);
	TVector3	scale;
	_stscanf(m_pBuffer, _T("%f	%f	%f"), &scale.x, &scale.y, &scale.z);

	K_Panel* Panel = new K_Panel();
	m_pSelectScene->m_pSelectPanel = Panel;
	AddRect(K_IMAGE, NULL, scale.x, scale.y, scale.z, K_PANEL);

	while (bLoad)
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_TEND]))
		{
			break;
		}
		m_pSelectScene->m_pSelectPanel->m_ImageCtl[m_pSelectScene->m_pSelectPanel->m_ImageCtl.size() - 1]->m_pPlane->MultiTextureLoad(m_pString);
		m_pSelectScene->m_pSelectPanel->m_ImageCtl[m_pSelectScene->m_pSelectPanel->m_ImageCtl.size() - 1]->m_pPlane->LoadTexture();
	}
	m_pSelectScene->m_PanelList.push_back(Panel);

	while (bLoad)
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, LoadSectionToken[SECTION_PEND]))
		{
			break;
		}
		if (!_tcsicmp(m_pString, LoadSectionToken[SECTION_BUTTON]))
		{
			LoadButton(K_PANEL);
		}
		if (!_tcsicmp(m_pString, LoadSectionToken[SECTION_EDIT]))
		{
			LoadEdit(K_PANEL);
		}
		if (!_tcsicmp(m_pString, LoadSectionToken[SECTION_TEXTURE]))
		{
			LoadTexture(K_PANEL);
		}
	}
	return true;
}

bool K_UIParser::LoadButton(KPS_TYPE type)
{
	bool bLoad = true;
	while (bLoad)
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_TEXTURENAME]))
		{
			Load_Texture(SECTION_BUTTON, type);
			if (type == K_SCENE)
				m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->LoadTexture();
			else
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->LoadTexture();
		}
		else if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_VERTEX]))
		{
			if (type == K_SCENE)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[0].v.x,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[0].v.y,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[0].v.z
				);
				m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_Vertices[0].v
					= m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[0].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[1].v.x,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[1].v.y,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[1].v.z
				);
				m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_Vertices[1].v
					= m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[1].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[2].v.x,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[2].v.y,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[2].v.z
				);
				m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_Vertices[2].v
					= m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[2].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[3].v.x,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[3].v.y,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[3].v.z
				);
				m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_Vertices[3].v
					= m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[3].v;

				S_RELEASE(m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_pVertexBuffer);

				{
					D3D11_BUFFER_DESC bd;
					bd.ByteWidth = sizeof(PNCT_VERTEX) * m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList.size();
					bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					bd.Usage = D3D11_USAGE_DEFAULT;
					bd.CPUAccessFlags = 0;
					bd.MiscFlags = 0;
					bd.StructureByteStride = 0;
					D3D11_SUBRESOURCE_DATA sd;
					sd.pSysMem = (void*)&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList.at(0);
					m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->m_pVertexBuffer);
				}
			}
			else
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[0].v.x,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[0].v.y,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[0].v.z
				);
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_Vertices[0].v
					= m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[0].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[1].v.x,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[1].v.y,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[1].v.z
				);
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_Vertices[1].v
					= m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[1].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[2].v.x,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[2].v.y,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[2].v.z
				);
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_Vertices[2].v
					= m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[2].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[3].v.x,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[3].v.y,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[3].v.z
				);
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_Vertices[3].v
					= m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList[3].v;

				S_RELEASE(m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_pVertexBuffer);

				{
					D3D11_BUFFER_DESC bd;
					bd.ByteWidth = sizeof(PNCT_VERTEX) * m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList.size();
					bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					bd.Usage = D3D11_USAGE_DEFAULT;
					bd.CPUAccessFlags = 0;
					bd.MiscFlags = 0;
					bd.StructureByteStride = 0;
					D3D11_SUBRESOURCE_DATA sd;
					sd.pSysMem = (void*)&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_VertexList.at(0);
					m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->m_pVertexBuffer);
				}
			}
		}
		else if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_MATWORLD]))
		{
			if (type == K_SCENE)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_vScale.x,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_matWorld._12,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_matWorld._13,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_matWorld._14);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_matWorld._21,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_vScale.y,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_matWorld._23,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_matWorld._24);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_matWorld._31,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_matWorld._32,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_vScale.z,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_matWorld._34);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_vTrans.x,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_vTrans.y,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_vTrans.z,
					&m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_matWorld._44);
			}
			else
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_vScale.x,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_matWorld._12,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_matWorld._13,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_matWorld._14);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_matWorld._21,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_vScale.y,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_matWorld._23,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_matWorld._24);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_matWorld._31,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_matWorld._32,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_vScale.z,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_matWorld._34);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_vTrans.x,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_vTrans.y,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_vTrans.z,
					&m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_matWorld._44);

			}
		}
		else if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_END]))
		{
			break;
		}
	}
	return true;
}
bool K_UIParser::LoadEdit(KPS_TYPE type)
{
	bool bLoad = true;
	while (bLoad)
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_TEXTURENAME]))
		{
			Load_Texture(SECTION_EDIT, type);
			if (type == K_SCENE)
				m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->LoadTexture();
			else
				m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->LoadTexture();
		}
		else if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_VERTEX]))
		{
			if (type == K_SCENE)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[0].v.x,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[0].v.y,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[0].v.z
				);
				m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_Vertices[0].v
					= m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[0].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[1].v.x,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[1].v.y,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[1].v.z
				);
				m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_Vertices[1].v
					= m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[1].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[2].v.x,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[2].v.y,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[2].v.z
				);
				m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_Vertices[2].v
					= m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[2].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[3].v.x,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[3].v.y,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[3].v.z
				);
				m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_Vertices[3].v
					= m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[3].v;

				S_RELEASE(m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_pVertexBuffer);

				{
					D3D11_BUFFER_DESC bd;
					bd.ByteWidth = sizeof(PNCT_VERTEX) * m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList.size();
					bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					bd.Usage = D3D11_USAGE_DEFAULT;
					bd.CPUAccessFlags = 0;
					bd.MiscFlags = 0;
					bd.StructureByteStride = 0;
					D3D11_SUBRESOURCE_DATA sd;
					sd.pSysMem = (void*)&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_VertexList.at(0);
					m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->m_pVertexBuffer);
				}
			}
			else
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[0].v.x,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[0].v.y,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[0].v.z
				);
				m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_Vertices[0].v
					= m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[0].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[1].v.x,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[1].v.y,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[1].v.z
				);
				m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_Vertices[1].v
					= m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[1].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[2].v.x,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[2].v.y,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[2].v.z
				);
				m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_Vertices[2].v
					= m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[2].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[3].v.x,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[3].v.y,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[3].v.z
				);
				m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_Vertices[3].v
					= m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList[3].v;

				S_RELEASE(m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_pVertexBuffer);

				{
					D3D11_BUFFER_DESC bd;
					bd.ByteWidth = sizeof(PNCT_VERTEX) * m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList.size();
					bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					bd.Usage = D3D11_USAGE_DEFAULT;
					bd.CPUAccessFlags = 0;
					bd.MiscFlags = 0;
					bd.StructureByteStride = 0;
					D3D11_SUBRESOURCE_DATA sd;
					sd.pSysMem = (void*)&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_VertexList.at(0);
					m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->m_pVertexBuffer);
				}
			}
		}
		else if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_MATWORLD]))
		{
			if (type == K_SCENE)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_vScale.x,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_matWorld._12,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_matWorld._13,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_matWorld._14);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_matWorld._21,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_vScale.y,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_matWorld._23,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_matWorld._24);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_matWorld._31,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_matWorld._32,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_vScale.z,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_matWorld._34);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_vTrans.x,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_vTrans.y,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_vTrans.z,
					&m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_matWorld._44);
			}
			else
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_vScale.x,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_matWorld._12,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_matWorld._13,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_matWorld._14);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_matWorld._21,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_vScale.y,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_matWorld._23,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_matWorld._24);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_matWorld._31,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_matWorld._32,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_vScale.z,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_matWorld._34);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_vTrans.x,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_vTrans.y,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_vTrans.z,
					&m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_matWorld._44);

			}
		}
		else if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_END]))
		{
			break;
		}
	}
	return true;
}
//0.81408191
bool K_UIParser::LoadTexture(KPS_TYPE type)
{
	bool bLoad = true;
	while (bLoad)
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_TEXTURENAME]))
		{
			Load_Texture(SECTION_TEXTURE, type);
			if (type == K_SCENE)
				m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->LoadTexture();
			else
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->LoadTexture();
		}
		else if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_VERTEX]))
		{
			if (type == K_SCENE)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[0].v.x,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[0].v.y,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[0].v.z
				);
				m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_Vertices[0].v
					= m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[0].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[1].v.x,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[1].v.y,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[1].v.z
				);
				m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_Vertices[1].v
					= m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[1].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[2].v.x,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[2].v.y,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[2].v.z
				);
				m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_Vertices[2].v
					= m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[2].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[3].v.x,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[3].v.y,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[3].v.z
				);
				m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_Vertices[3].v
					= m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[3].v;

				S_RELEASE(m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_pVertexBuffer);

				{
					D3D11_BUFFER_DESC bd;
					bd.ByteWidth = sizeof(PNCT_VERTEX) * m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList.size();
					bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					bd.Usage = D3D11_USAGE_DEFAULT;
					bd.CPUAccessFlags = 0;
					bd.MiscFlags = 0;
					bd.StructureByteStride = 0;
					D3D11_SUBRESOURCE_DATA sd;
					sd.pSysMem = (void*)&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList.at(0);
					m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->m_pVertexBuffer);
				}
			}
			else
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[0].v.x,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[0].v.y,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[0].v.z
				);
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_Vertices[0].v
					= m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[0].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[1].v.x,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[1].v.y,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[1].v.z
				);
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_Vertices[1].v
					= m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[1].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[2].v.x,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[2].v.y,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[2].v.z
				);
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_Vertices[2].v
					= m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[2].v;

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f"), &m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[3].v.x,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[3].v.y,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[3].v.z
				);
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_Vertices[3].v
					= m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList[3].v;

				S_RELEASE(m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_pVertexBuffer);

				{
					D3D11_BUFFER_DESC bd;
					bd.ByteWidth = sizeof(PNCT_VERTEX) * m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList.size();
					bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					bd.Usage = D3D11_USAGE_DEFAULT;
					bd.CPUAccessFlags = 0;
					bd.MiscFlags = 0;
					bd.StructureByteStride = 0;
					D3D11_SUBRESOURCE_DATA sd;
					sd.pSysMem = (void*)&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_VertexList.at(0);
					m_pd3dDevice->CreateBuffer(&bd, &sd, &m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->m_pVertexBuffer);
				}
			}
		}
		else if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_MATWORLD]))
		{
			if (type == K_SCENE)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_vScale.x,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_matWorld._12,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_matWorld._13,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_matWorld._14);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_matWorld._21,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_vScale.y,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_matWorld._23,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_matWorld._24);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_matWorld._31,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_matWorld._32,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_vScale.z,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_matWorld._34);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_vTrans.x,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_vTrans.y,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_vTrans.z,
					&m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_matWorld._44);
			}
			else
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_vScale.x,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_matWorld._12,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_matWorld._13,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_matWorld._14);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_matWorld._21,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_vScale.y,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_matWorld._23,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_matWorld._24);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_matWorld._31,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_matWorld._32,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_vScale.z,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_matWorld._34);

				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%f	%f	%f	%f"),
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_vTrans.x,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_vTrans.y,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_vTrans.z,
					&m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_matWorld._44);

			}
		}
		else if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_END]))
		{
			break;
		}
	}
	return true;
}


bool K_UIParser::Load_Texture(LOADSECTION ObjectNum, KPS_TYPE type)
{
	bool Load = true;
	if (type == K_SCENE)
	{
		if (ObjectNum == SECTION_BUTTON)
			AddRect(K_BUTTON, NULL);
		if (ObjectNum == SECTION_EDIT)
			AddRect(K_EDIT, NULL);
		if (ObjectNum == SECTION_TEXTURE)
			AddRect(K_TEXTURE, NULL);
	}
	else
	{
		if (ObjectNum == SECTION_BUTTON)
			AddRect(K_BUTTON, NULL, 100.0f, 100.0f, 100.0f, K_PANEL);
		if (ObjectNum == SECTION_EDIT)
			AddRect(K_EDIT, NULL, 100.0f, 100.0f, 100.0f, K_PANEL);
		if (ObjectNum == SECTION_TEXTURE)
			AddRect(K_TEXTURE, NULL, 100.0f, 100.0f, 100.0f, K_PANEL);
	}
	while (Load)
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, _T("%s"), m_pString);
		if (!_tcsicmp(m_pString, ObejctSectionToken[SECTION_TEND]))
		{
			break;
		}
		if (type == K_SCENE)
		{
			if (ObjectNum == SECTION_BUTTON)
				m_pSelectScene->m_ButtonCtl[m_pSelectScene->m_ButtonCtl.size() - 1]->m_pPlane->MultiTextureLoad(m_pString);
			if (ObjectNum == SECTION_EDIT)
				m_pSelectScene->m_EditCtl[m_pSelectScene->m_EditCtl.size() - 1]->m_pPlane->MultiTextureLoad(m_pString);
			if (ObjectNum == SECTION_TEXTURE)
				m_pSelectScene->m_TextureCtl[m_pSelectScene->m_TextureCtl.size() - 1]->m_pPlane->MultiTextureLoad(m_pString);
		}
		else
		{
			if (ObjectNum == SECTION_BUTTON)
				m_pSelectScene->m_pSelectPanel->m_ButtonCtl[m_pSelectScene->m_pSelectPanel->m_ButtonCtl.size() - 1]->m_pPlane->MultiTextureLoad(m_pString);
			if (ObjectNum == SECTION_EDIT)
				m_pSelectScene->m_pSelectPanel->m_EditCtl[m_pSelectScene->m_pSelectPanel->m_EditCtl.size() - 1]->m_pPlane->MultiTextureLoad(m_pString);
			if (ObjectNum == SECTION_TEXTURE)
				m_pSelectScene->m_pSelectPanel->m_TextureCtl[m_pSelectScene->m_pSelectPanel->m_TextureCtl.size() - 1]->m_pPlane->MultiTextureLoad(m_pString);
		}
	}
	return true;
}

bool K_UIParser::Init()
{
	m_hWnd = g_hWnd;
	m_pd3dDevice = g_pd3dDevice;
	m_pImmediateContext = g_pImmediateContext;

	Load(L"../../data/UIObject/74PER.txt");
	m_pSelectScene = m_SceneList[MAIN];
	SetState();
	SetUIFSM();
	m_Sound.Init();
	m_Sound.Load("../../data/UISound/BGM_TITLE.mp3", UI_SOUND_TITLE, 1);
	m_Sound.Load("../../data/UISound/BGM_LOADING.mp3", UI_SOUND_LOADING, 1);
	m_Sound.Load("../../data/UISound/SNDEFFECT_BATTLE_PHASE.mp3", UI_SOUND_PHASE, 0);
	m_Sound.Load("../../data/UISound/BGM_FARMINGPHASE_1.mp3", UI_SOUND_INGAME_FARMING, 1);
	m_Sound.Load("../../data/UISound/BGM_FARMINGPHASE_3.mp3", UI_SOUND_INGAME_FARMING_2, 1);
	m_Sound.Load("../../data/UISound/BGM_FARMINGPHASE_5.mp3", UI_SOUND_INGAME_FARMING_3, 1);
	m_Sound.Load("../../data/UISound/BGM_BATTLEPHASE_1.mp3", UI_SOUND_INGAME_BATTLE, 1);
	m_Sound.Load("../../data/UISound/BGM_BATTLEPHASE_3.mp3", UI_SOUND_INGAME_BATTLE_2, 1);
	m_Sound.Load("../../data/UISound/BGM_BATTLEPHASE_5.mp3", UI_SOUND_INGAME_BATTLE_3, 1);
	m_Sound.Load("../../data/UISound/BGM_SNOW.mp3", UI_SOUND_INGAME_SNOW, 1);
	m_Sound.Load("../../data/UISound/BGM_BATTLE_WIN.mp3", UI_SOUND_INGAME_WIN, 1);
	m_Sound.Load("../../data/UISound/BGM_BATTLE_LOSE.mp3", UI_SOUND_INGAME_LOSE, 1);
	m_Sound.Load("../../data/UISound/SNDEFFECT_BTN_CLICK.mp3", UI_SOUND_ICON_CLICK, 0);
	m_Sound.Load("../../data/UISound/SNDEFFECT_BTN_NEAR.mp3", UI_SOUND_ICON_NEAR, 0);
	m_Sound.Load("../../data/UISound/SNDEFFECT_BOMB_ATTACK2.mp3", UI_SOUND_INGAME_BOMB, 0);

	//m_Mouse.Init();

	return true;
}

bool K_UIParser::Frame()
{
	if (isRealGame)
	{
		if (m_pSelectScene == m_SceneList[LOADING])
		{
			SceneButtonSelect();
			SceneStateChange();
			if (isGamePanelOK)
			{
				PanelButtonSelect();
				PanelStateChange();
			}
		}
		else
		{
			if (!isGamePanelOK)
			{
				SceneButtonSelect();
				SceneStateChange();
			}
			else
			{
				PanelButtonSelect();
				PanelStateChange();
			}
		}
	}
	else
	{
		if (SelectRect())
		{
			if (isToolPanelOK && m_pSelectScene->m_pSelectPanel != NULL)
			{
				m_pSelectScene->m_pSelectPanel->m_pSelectPlane = SelectRect();
				SelectPlane = m_pSelectScene->m_pSelectPanel->m_pSelectPlane;
			}
			else
			{
				m_pSelectScene->m_pSelectPlane = SelectRect();
				SelectPlane = m_pSelectScene->m_pSelectPlane;
			}

			POINT mouse;
			GetCursorPos(&mouse);	//마우스 포인터 위치
			ScreenToClient(m_hWnd, &mouse);

			RECT AAA;
			GetClientRect(m_hWnd, &AAA);
			float ClientX = 1920 * ((float)mouse.x / AAA.right);
			float ClientY = 1080 * ((float)mouse.y / AAA.bottom);

			B_MousePoint.x = ClientX;
			B_MousePoint.y = ClientY;

			if (isModifiedOK == true)	//수정이 가능할때만
			{
				if (SelectPlane->m_rtSize[MOVE].left <= ClientX && SelectPlane->m_rtSize[MOVE].right >= ClientX)
				{
					if (SelectPlane->m_rtSize[MOVE].top <= ClientY && SelectPlane->m_rtSize[MOVE].bottom >= ClientY)
					{
						PlaneSelectIsOk[MOVE] = true;
					}
				}
				if (SelectPlane->m_rtSize[LEFT].left <= ClientX && SelectPlane->m_rtSize[LEFT].right >= ClientX)
				{
					if (SelectPlane->m_rtSize[LEFT].top <= ClientY && SelectPlane->m_rtSize[LEFT].bottom >= ClientY)
					{
						PlaneSelectIsOk[LEFT] = true;
					}
				}
				if (SelectPlane->m_rtSize[RIGHT].left <= ClientX && SelectPlane->m_rtSize[RIGHT].right >= ClientX)
				{
					if (SelectPlane->m_rtSize[RIGHT].top <= ClientY && SelectPlane->m_rtSize[RIGHT].bottom >= ClientY)
					{
						PlaneSelectIsOk[RIGHT] = true;
					}
				}
				if (SelectPlane->m_rtSize[UP].left <= ClientX && SelectPlane->m_rtSize[UP].right >= ClientX)
				{
					if (SelectPlane->m_rtSize[UP].top <= ClientY && SelectPlane->m_rtSize[UP].bottom >= ClientY)
					{
						PlaneSelectIsOk[UP] = true;
					}
				}
				if (SelectPlane->m_rtSize[DOWN].left <= ClientX && SelectPlane->m_rtSize[DOWN].right >= ClientX)
				{
					if (SelectPlane->m_rtSize[DOWN].top <= ClientY && SelectPlane->m_rtSize[DOWN].bottom >= ClientY)
					{
						PlaneSelectIsOk[DOWN] = true;
					}
				}
				if (SelectPlane->m_rtSize[LEFT_UP].left <= ClientX && SelectPlane->m_rtSize[LEFT_UP].right >= ClientX)
				{
					if (SelectPlane->m_rtSize[LEFT_UP].top <= ClientY && SelectPlane->m_rtSize[LEFT_UP].bottom >= ClientY)
					{
						PlaneSelectIsOk[LEFT_UP] = true;
					}
				}
				if (SelectPlane->m_rtSize[RIGHT_UP].left <= ClientX && SelectPlane->m_rtSize[RIGHT_UP].right >= ClientX)
				{
					if (SelectPlane->m_rtSize[RIGHT_UP].top <= ClientY && SelectPlane->m_rtSize[RIGHT_UP].bottom >= ClientY)
					{
						PlaneSelectIsOk[RIGHT_UP] = true;
					}
				}
				if (SelectPlane->m_rtSize[LEFT_DOWN].left <= ClientX && SelectPlane->m_rtSize[LEFT_DOWN].right >= ClientX)
				{
					if (SelectPlane->m_rtSize[LEFT_DOWN].top <= ClientY && SelectPlane->m_rtSize[LEFT_DOWN].bottom >= ClientY)
					{
						PlaneSelectIsOk[LEFT_DOWN] = true;
					}
				}
				if (SelectPlane->m_rtSize[RIGHT_DOWN].left <= ClientX && SelectPlane->m_rtSize[RIGHT_DOWN].right >= ClientX)
				{
					if (SelectPlane->m_rtSize[RIGHT_DOWN].top <= ClientY && SelectPlane->m_rtSize[RIGHT_DOWN].bottom >= ClientY)
					{
						PlaneSelectIsOk[RIGHT_DOWN] = true;
					}
				}
			}
		}
		if (SelectPlane != NULL)
		{
			if (PlaneSelectIsOk[MOVE] == true)	//이동
			{
				if (I_Input.m_MouseState[0] == KEY_HOLD)
				{
					POINT mouse;
					GetCursorPos(&mouse);	//마우스 포인터 위치
					ScreenToClient(m_hWnd, &mouse);

					RECT AAA;
					GetClientRect(m_hWnd, &AAA);
					float ClientX = 1920 * ((float)mouse.x / AAA.right);
					float ClientY = 1080 * ((float)mouse.y / AAA.bottom);

					//마우스위치 - 클라이언트상의 위치(센터값을 0,0으로 봤을때의 위치값을 계산)
					int Width = ClientX - (1920 / 2);
					int Height = ClientY - (1080 / 2);
					TVector3 vCenter(Width, Height, 0);
					//무브함수 호출
					SelectPlane->Move(vCenter.x, -vCenter.y, vCenter.z);

				}
				if (I_Input.m_MouseState[0] == KEY_UP)
				{
					SelectPlane = NULL;
					PlaneSelectIsOk[MOVE] = false;
				}
			}
			else if (PlaneSelectIsOk[LEFT] == true)
			{
				if (I_Input.m_MouseState[0] == KEY_HOLD)
				{
					POINT mouse;
					GetCursorPos(&mouse);	//마우스 포인터 위치
					ScreenToClient(m_hWnd, &mouse);

					RECT AAA;
					GetClientRect(m_hWnd, &AAA);
					float ClientX = 1920 * ((float)mouse.x / AAA.right);
					float ClientY = 1080 * ((float)mouse.y / AAA.bottom);

					A_MousePoint.x = ClientX;
					A_MousePoint.y = ClientY;


					float MovingX = (B_MousePoint.x - A_MousePoint.x)*0.01*g_fSecPerFrame;
					float MovingY = (B_MousePoint.y - A_MousePoint.y)*0.01*g_fSecPerFrame;

					SelectPlane->m_pPlane->m_VertexList[0].v.x -= MovingX;
					SelectPlane->m_Vertices[0].v.x -= MovingX;
					SelectPlane->m_pPlane->m_VertexList[3].v.x -= MovingX;
					SelectPlane->m_Vertices[3].v.x -= MovingX;

					//센터값 갱신
					SelectPlane->m_pPlane->m_vCenter.x -= MovingX;

					S_RELEASE(SelectPlane->m_pPlane->m_pVertexBuffer);

					{
						D3D11_BUFFER_DESC bd;
						bd.ByteWidth = sizeof(PNCT_VERTEX) * SelectPlane->m_pPlane->m_VertexList.size();
						bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
						bd.Usage = D3D11_USAGE_DEFAULT;
						bd.CPUAccessFlags = 0;
						bd.MiscFlags = 0;
						bd.StructureByteStride = 0;
						D3D11_SUBRESOURCE_DATA sd;
						sd.pSysMem = (void*)&SelectPlane->m_pPlane->m_VertexList.at(0);
						m_pd3dDevice->CreateBuffer(&bd, &sd, &SelectPlane->m_pPlane->m_pVertexBuffer);
					}
				}
				if (I_Input.m_MouseState[0] == KEY_UP)
				{
					SelectPlane = NULL;
					PlaneSelectIsOk[LEFT] = false;
				}
			}
			else if (PlaneSelectIsOk[RIGHT] == true)
			{
				if (I_Input.m_MouseState[0] == KEY_HOLD)
				{
					POINT mouse;
					GetCursorPos(&mouse);	//마우스 포인터 위치
					ScreenToClient(m_hWnd, &mouse);

					RECT AAA;
					GetClientRect(m_hWnd, &AAA);
					float ClientX = 1920 * ((float)mouse.x / AAA.right);
					float ClientY = 1080 * ((float)mouse.y / AAA.bottom);

					A_MousePoint.x = ClientX;
					A_MousePoint.y = ClientY;


					float MovingX = (A_MousePoint.x - B_MousePoint.x)*0.01*g_fSecPerFrame;
					float MovingY = (A_MousePoint.y - B_MousePoint.y)*0.01*g_fSecPerFrame;

					SelectPlane->m_pPlane->m_VertexList[1].v.x += MovingX;
					SelectPlane->m_Vertices[1].v.x += MovingX;
					SelectPlane->m_pPlane->m_VertexList[2].v.x += MovingX;
					SelectPlane->m_Vertices[2].v.x += MovingX;
					S_RELEASE(SelectPlane->m_pPlane->m_pVertexBuffer);

					{
						D3D11_BUFFER_DESC bd;
						bd.ByteWidth = sizeof(PNCT_VERTEX) * SelectPlane->m_pPlane->m_VertexList.size();
						bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
						bd.Usage = D3D11_USAGE_DEFAULT;
						bd.CPUAccessFlags = 0;
						bd.MiscFlags = 0;
						bd.StructureByteStride = 0;
						D3D11_SUBRESOURCE_DATA sd;
						sd.pSysMem = (void*)&SelectPlane->m_pPlane->m_VertexList.at(0);
						m_pd3dDevice->CreateBuffer(&bd, &sd, &SelectPlane->m_pPlane->m_pVertexBuffer);
					}
				}
				if (I_Input.m_MouseState[0] == KEY_UP)
				{
					SelectPlane = NULL;
					PlaneSelectIsOk[RIGHT] = false;
				}
			}
			else if (PlaneSelectIsOk[UP] == true)
			{
				if (I_Input.m_MouseState[0] == KEY_HOLD)
				{
					POINT mouse;
					GetCursorPos(&mouse);	//마우스 포인터 위치
					ScreenToClient(m_hWnd, &mouse);

					RECT AAA;
					GetClientRect(m_hWnd, &AAA);
					float ClientX = 1920 * ((float)mouse.x / AAA.right);
					float ClientY = 1080 * ((float)mouse.y / AAA.bottom);

					A_MousePoint.x = ClientX;
					A_MousePoint.y = ClientY;

					float MovingX = (B_MousePoint.x - A_MousePoint.x)*0.01*g_fSecPerFrame;
					float MovingY = (B_MousePoint.y - A_MousePoint.y)*0.01*g_fSecPerFrame;

					SelectPlane->m_pPlane->m_VertexList[1].v.y += MovingY;
					SelectPlane->m_Vertices[1].v.y += MovingY;
					SelectPlane->m_pPlane->m_VertexList[0].v.y += MovingY;
					SelectPlane->m_Vertices[0].v.y += MovingY;
					S_RELEASE(SelectPlane->m_pPlane->m_pVertexBuffer);
					{
						D3D11_BUFFER_DESC bd;
						bd.ByteWidth = sizeof(PNCT_VERTEX) * SelectPlane->m_pPlane->m_VertexList.size();
						bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
						bd.Usage = D3D11_USAGE_DEFAULT;
						bd.CPUAccessFlags = 0;
						bd.MiscFlags = 0;
						bd.StructureByteStride = 0;
						D3D11_SUBRESOURCE_DATA sd;
						sd.pSysMem = (void*)&SelectPlane->m_pPlane->m_VertexList.at(0);
						m_pd3dDevice->CreateBuffer(&bd, &sd, &SelectPlane->m_pPlane->m_pVertexBuffer);
					}
				}
				if (I_Input.m_MouseState[0] == KEY_UP)
				{
					SelectPlane = NULL;
					PlaneSelectIsOk[UP] = false;
				}
			}
			else if (PlaneSelectIsOk[DOWN] == true)
			{
				if (I_Input.m_MouseState[0] == KEY_HOLD)
				{
					POINT mouse;
					GetCursorPos(&mouse);	//마우스 포인터 위치
					ScreenToClient(m_hWnd, &mouse);

					RECT AAA;
					GetClientRect(m_hWnd, &AAA);
					float ClientX = 1920 * ((float)mouse.x / AAA.right);
					float ClientY = 1080 * ((float)mouse.y / AAA.bottom);

					A_MousePoint.x = ClientX;
					A_MousePoint.y = ClientY;


					float MovingX = (A_MousePoint.x - B_MousePoint.x)*0.01*g_fSecPerFrame;
					float MovingY = (A_MousePoint.y - B_MousePoint.y)*0.01*g_fSecPerFrame;

					SelectPlane->m_pPlane->m_VertexList[3].v.y -= MovingY;
					SelectPlane->m_Vertices[3].v.y -= MovingY;
					SelectPlane->m_pPlane->m_VertexList[2].v.y -= MovingY;
					SelectPlane->m_Vertices[2].v.y -= MovingY;
					S_RELEASE(SelectPlane->m_pPlane->m_pVertexBuffer);

					{
						D3D11_BUFFER_DESC bd;
						bd.ByteWidth = sizeof(PNCT_VERTEX) * SelectPlane->m_pPlane->m_VertexList.size();
						bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
						bd.Usage = D3D11_USAGE_DEFAULT;
						bd.CPUAccessFlags = 0;
						bd.MiscFlags = 0;
						bd.StructureByteStride = 0;
						D3D11_SUBRESOURCE_DATA sd;
						sd.pSysMem = (void*)&SelectPlane->m_pPlane->m_VertexList.at(0);
						m_pd3dDevice->CreateBuffer(&bd, &sd, &SelectPlane->m_pPlane->m_pVertexBuffer);
					}
				}
				if (I_Input.m_MouseState[0] == KEY_UP)
				{
					SelectPlane = NULL;
					PlaneSelectIsOk[DOWN] = false;
				}
			}
			else if (PlaneSelectIsOk[LEFT_UP] == true)
			{
				if (I_Input.m_MouseState[0] == KEY_HOLD)
				{
					POINT mouse;
					GetCursorPos(&mouse);	//마우스 포인터 위치
					ScreenToClient(m_hWnd, &mouse);

					RECT AAA;
					GetClientRect(m_hWnd, &AAA);
					float ClientX = 1920 * ((float)mouse.x / AAA.right);
					float ClientY = 1080 * ((float)mouse.y / AAA.bottom);

					A_MousePoint.x = ClientX;
					A_MousePoint.y = ClientY;

					float MovingX = (A_MousePoint.x - B_MousePoint.x)*0.01*g_fSecPerFrame;
					float MovingY = (A_MousePoint.y - B_MousePoint.y)*0.01*g_fSecPerFrame;

					SelectPlane->m_pPlane->m_VertexList[0].v.x += MovingX;
					SelectPlane->m_Vertices[0].v.x += MovingX;
					SelectPlane->m_pPlane->m_VertexList[0].v.y -= MovingY;
					SelectPlane->m_Vertices[0].v.y -= MovingY;
					SelectPlane->m_pPlane->m_VertexList[3].v.x += MovingX;
					SelectPlane->m_Vertices[3].v.x += MovingX;
					SelectPlane->m_pPlane->m_VertexList[1].v.y -= MovingY;
					SelectPlane->m_Vertices[1].v.y -= MovingY;

					S_RELEASE(SelectPlane->m_pPlane->m_pVertexBuffer);

					{
						D3D11_BUFFER_DESC bd;
						bd.ByteWidth = sizeof(PNCT_VERTEX) * SelectPlane->m_pPlane->m_VertexList.size();
						bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
						bd.Usage = D3D11_USAGE_DEFAULT;
						bd.CPUAccessFlags = 0;
						bd.MiscFlags = 0;
						bd.StructureByteStride = 0;
						D3D11_SUBRESOURCE_DATA sd;
						sd.pSysMem = (void*)&SelectPlane->m_pPlane->m_VertexList.at(0);
						m_pd3dDevice->CreateBuffer(&bd, &sd, &SelectPlane->m_pPlane->m_pVertexBuffer);
					}
				}
				if (I_Input.m_MouseState[0] == KEY_UP)
				{
					SelectPlane = NULL;
					PlaneSelectIsOk[LEFT_UP] = false;
				}
			}
			else if (PlaneSelectIsOk[RIGHT_UP] == true)
			{
				if (I_Input.m_MouseState[0] == KEY_HOLD)
				{
					POINT mouse;
					GetCursorPos(&mouse);	//마우스 포인터 위치
					ScreenToClient(m_hWnd, &mouse);

					RECT AAA;
					GetClientRect(m_hWnd, &AAA);
					float ClientX = 1920 * ((float)mouse.x / AAA.right);
					float ClientY = 1080 * ((float)mouse.y / AAA.bottom);

					A_MousePoint.x = ClientX;
					A_MousePoint.y = ClientY;

					float MovingX = (A_MousePoint.x - B_MousePoint.x)*0.01*g_fSecPerFrame;
					float MovingY = (A_MousePoint.y - B_MousePoint.y)*0.01*g_fSecPerFrame;

					SelectPlane->m_pPlane->m_VertexList[1].v.x += MovingX;
					SelectPlane->m_Vertices[1].v.x += MovingX;
					SelectPlane->m_pPlane->m_VertexList[1].v.y -= MovingY;
					SelectPlane->m_Vertices[1].v.y -= MovingY;
					SelectPlane->m_pPlane->m_VertexList[2].v.x += MovingX;
					SelectPlane->m_Vertices[2].v.x += MovingX;
					SelectPlane->m_pPlane->m_VertexList[0].v.y -= MovingY;
					SelectPlane->m_Vertices[0].v.y -= MovingY;

					S_RELEASE(SelectPlane->m_pPlane->m_pVertexBuffer);

					{
						D3D11_BUFFER_DESC bd;
						bd.ByteWidth = sizeof(PNCT_VERTEX) * SelectPlane->m_pPlane->m_VertexList.size();
						bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
						bd.Usage = D3D11_USAGE_DEFAULT;
						bd.CPUAccessFlags = 0;
						bd.MiscFlags = 0;
						bd.StructureByteStride = 0;
						D3D11_SUBRESOURCE_DATA sd;
						sd.pSysMem = (void*)&SelectPlane->m_pPlane->m_VertexList.at(0);
						m_pd3dDevice->CreateBuffer(&bd, &sd, &SelectPlane->m_pPlane->m_pVertexBuffer);
					}
				}
				if (I_Input.m_MouseState[0] == KEY_UP)
				{
					SelectPlane = NULL;
					PlaneSelectIsOk[RIGHT_UP] = false;
				}

			}
			else if (PlaneSelectIsOk[LEFT_DOWN] == true)
			{
				if (I_Input.m_MouseState[0] == KEY_HOLD)
				{
					POINT mouse;
					GetCursorPos(&mouse);	//마우스 포인터 위치
					ScreenToClient(m_hWnd, &mouse);

					RECT AAA;
					GetClientRect(m_hWnd, &AAA);
					float ClientX = 1920 * ((float)mouse.x / AAA.right);
					float ClientY = 1080 * ((float)mouse.y / AAA.bottom);

					A_MousePoint.x = ClientX;
					A_MousePoint.y = ClientY;

					float MovingX = (A_MousePoint.x - B_MousePoint.x)*0.01*g_fSecPerFrame;
					float MovingY = (A_MousePoint.y - B_MousePoint.y)*0.01*g_fSecPerFrame;

					SelectPlane->m_pPlane->m_VertexList[3].v.x += MovingX;
					SelectPlane->m_Vertices[3].v.x += MovingX;
					SelectPlane->m_pPlane->m_VertexList[3].v.y -= MovingY;
					SelectPlane->m_Vertices[3].v.y -= MovingY;
					SelectPlane->m_pPlane->m_VertexList[0].v.x += MovingX;
					SelectPlane->m_Vertices[0].v.x += MovingX;
					SelectPlane->m_pPlane->m_VertexList[2].v.y -= MovingY;
					SelectPlane->m_Vertices[2].v.y -= MovingY;

					S_RELEASE(SelectPlane->m_pPlane->m_pVertexBuffer);

					{
						D3D11_BUFFER_DESC bd;
						bd.ByteWidth = sizeof(PNCT_VERTEX) * SelectPlane->m_pPlane->m_VertexList.size();
						bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
						bd.Usage = D3D11_USAGE_DEFAULT;
						bd.CPUAccessFlags = 0;
						bd.MiscFlags = 0;
						bd.StructureByteStride = 0;
						D3D11_SUBRESOURCE_DATA sd;
						sd.pSysMem = (void*)&SelectPlane->m_pPlane->m_VertexList.at(0);
						m_pd3dDevice->CreateBuffer(&bd, &sd, &SelectPlane->m_pPlane->m_pVertexBuffer);
					}
				}
				if (I_Input.m_MouseState[0] == KEY_UP)
				{
					SelectPlane = NULL;
					PlaneSelectIsOk[LEFT_DOWN] = false;
				}

			}
			else if (PlaneSelectIsOk[RIGHT_DOWN] == true)
			{
				if (I_Input.m_MouseState[0] == KEY_HOLD)
				{
					POINT mouse;
					GetCursorPos(&mouse);	//마우스 포인터 위치
					ScreenToClient(m_hWnd, &mouse);

					RECT AAA;
					GetClientRect(m_hWnd, &AAA);
					float ClientX = 1920 * ((float)mouse.x / AAA.right);
					float ClientY = 1080 * ((float)mouse.y / AAA.bottom);

					A_MousePoint.x = ClientX;
					A_MousePoint.y = ClientY;

					float MovingX = (A_MousePoint.x - B_MousePoint.x)*0.01*g_fSecPerFrame;
					float MovingY = (A_MousePoint.y - B_MousePoint.y)*0.01*g_fSecPerFrame;

					SelectPlane->m_pPlane->m_VertexList[2].v.x += MovingX;
					SelectPlane->m_Vertices[2].v.x += MovingX;
					SelectPlane->m_pPlane->m_VertexList[2].v.y -= MovingY;
					SelectPlane->m_Vertices[2].v.y -= MovingY;
					SelectPlane->m_pPlane->m_VertexList[1].v.x += MovingX;
					SelectPlane->m_Vertices[1].v.x += MovingX;
					SelectPlane->m_pPlane->m_VertexList[3].v.y -= MovingY;
					SelectPlane->m_Vertices[3].v.y -= MovingY;

					S_RELEASE(SelectPlane->m_pPlane->m_pVertexBuffer);

					{
						D3D11_BUFFER_DESC bd;
						bd.ByteWidth = sizeof(PNCT_VERTEX) * SelectPlane->m_pPlane->m_VertexList.size();
						bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
						bd.Usage = D3D11_USAGE_DEFAULT;
						bd.CPUAccessFlags = 0;
						bd.MiscFlags = 0;
						bd.StructureByteStride = 0;
						D3D11_SUBRESOURCE_DATA sd;
						sd.pSysMem = (void*)&SelectPlane->m_pPlane->m_VertexList.at(0);
						m_pd3dDevice->CreateBuffer(&bd, &sd, &SelectPlane->m_pPlane->m_pVertexBuffer);
					}
				}
				if (I_Input.m_MouseState[0] == KEY_UP)
				{
					SelectPlane = NULL;
					PlaneSelectIsOk[RIGHT_DOWN] = false;
				}
			}
		}
	}
	if (m_pSelectScene)
	{
		for (int iPanel = 0; iPanel < m_pSelectScene->m_PanelList.size(); iPanel++)
		{
			for (int iPlane = 0; iPlane < m_pSelectScene->m_PanelList[iPanel]->m_ButtonCtl.size(); iPlane++)
			{
				TVector3 vPos(0, 0, 0);
				K_ControlUI* pRect = m_pSelectScene->m_PanelList[iPanel]->m_ButtonCtl[iPlane];
				pRect->Update();
			}
			//에디트
			for (int iPlane = 0; iPlane < m_pSelectScene->m_PanelList[iPanel]->m_EditCtl.size(); iPlane++)
			{
				TVector3 vPos(0, 0, 0);
				K_ControlUI* pRect = m_pSelectScene->m_PanelList[iPanel]->m_EditCtl[iPlane];
				pRect->Update();
			}
			//텍스쳐
			for (int iPlane = 0; iPlane < m_pSelectScene->m_PanelList[iPanel]->m_TextureCtl.size(); iPlane++)
			{
				TVector3 vPos(0, 0, 0);
				K_ControlUI* pRect = m_pSelectScene->m_PanelList[iPanel]->m_TextureCtl[iPlane];
				pRect->Update();
			}
		}
		//버튼
		for (int iPlane = 0; iPlane < m_pSelectScene->m_ButtonCtl.size(); iPlane++)
		{
			TVector3 vPos(0, 0, 0);
			K_ControlUI* pRect = m_pSelectScene->m_ButtonCtl[iPlane];
			pRect->Update();
		}
		//에디트
		for (int iPlane = 0; iPlane < m_pSelectScene->m_EditCtl.size(); iPlane++)
		{
			TVector3 vPos(0, 0, 0);
			K_ControlUI* pRect = m_pSelectScene->m_EditCtl[iPlane];
			pRect->Update();
		}
		//텍스쳐
		for (int iPlane = 0; iPlane < m_pSelectScene->m_TextureCtl.size(); iPlane++)
		{
			TVector3 vPos(0, 0, 0);
			K_ControlUI* pRect = m_pSelectScene->m_TextureCtl[iPlane];
			pRect->Update();
		}
	}
	if (m_pSelectScene == m_SceneList[MAIN] || m_pSelectScene == m_SceneList[LOADING])
	{
		BackgroundMusicSet(m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState);
		SoundEffectSet(m_SceneList[MAIN]->m_PanelList[MAIN_PANEL_OPTION]->m_TextureCtl[MAIN_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState);
	}
	else
	{
		BackgroundMusicSet(m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_BM_NUM]->m_dwCurrentState);
		SoundEffectSet(m_SceneList[INGAME]->m_PanelList[INGAME_PANEL_OPTION]->m_TextureCtl[INGAME_PANEL_OPTION_TEXTURE_SE_NUM]->m_dwCurrentState);
	}
	//m_Mouse.Frame();
	return true;
}

bool K_UIParser::Render()
{
	if (m_pSelectScene)
	{
		m_pSelectScene->Render();
	}
	//m_Mouse.Render();
	return true;
}

bool K_UIParser::Release()
{
	for (int iScene = 0; iScene < m_SceneList.size(); iScene++)
	{
		m_SceneList[iScene]->Release();
	}
	m_Sound.Release();
	//m_Mouse.Release();
	return true;
}

K_UIParser::K_UIParser()
{
	//멤버 변수 초기화
	for (int iCnt = 0; iCnt < 9; iCnt++)
	{
		PlaneSelectIsOk[iCnt] != false;
	}
	MousePickisTrue = false;
	isModifiedOK = false;
	isToolPanelOK = false;
	SelectPlane = NULL;
	isRealGame = false;
	isGamePanelOK = false;

	m_fFarmingTick = 0.0f;			//파밍시간 초기화
	m_fBattleTick = 0.0f;				//배틀시간 초기화

	m_SecnodTimeChangeTerm = 1.0f;
	m_SecondimeTick = 0.0f;
	m_SecondCount = 0;
	m_TenSecondCount = 0;

	m_PhaseChangeTerm = 0.5f;
	FarmingOrBattle = 0;
	MouseCursorOn = true;
	
	m_HPBAR_ChangeTick = 0.0f;

	InvenInfoTextureRenderTick = 0.0f;
	InvenInfoisRender = false;
	isRenderButton = true;

	m_AIM_TextureChangePlusTick = 0.0f;
	m_AIM_TextureChangePlusTerm = 0.3f;

	m_AIM_TextureChangeMinusTick = 0.0f;
	m_AIM_TextureChangeMinusTerm = 0.01f;
}


K_UIParser::~K_UIParser()
{
}
