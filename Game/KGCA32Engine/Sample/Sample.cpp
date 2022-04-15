#include "Sample.h"

Sample* pSample;
int g_LoadingNum = 0;

unsigned int WINAPI Thread_Function_FirstInit(void*avg)
{
	pSample->m_Effect.Init();
	g_LoadingNum = 30;
	if (pSample->m_Map.bIsmapCreated == false)
	{
		pSample->m_Map.Init();
	}
	g_LoadingNum = 60;

	pSample->m_Character.Init(&pSample->m_Map);
	if (pSample->m_Map.bIsmapCreated == false)
	{
		pSample->m_Map.SettingParser(&pSample->m_Character, &pSample->m_UI);
		pSample->m_Map.bIsmapCreated = true;
	}
	pSample->m_UI.CharacterUIDataLoad();
	pSample->m_UI.isGameStart = false;

	g_LoadingNum = 100;

	CloseHandle((HANDLE)pSample->Thread_FirstInit_ID);
	return 0;
}

bool Sample::Init()
{
	pSample = this;

	m_UI.Init();
	m_Map.bIsmapCreated = false;
	m_Map.m_bItemInited = false;
	return true;
}

bool Sample::Frame()
{
	if (m_UI.isGameStart && ThreadFunctionCall == false &&g_LoadingNum == 0)
	{
		//쓰레드 콜
		pSample->ThreadFunctionCall = true;
		pSample->Thread_FirstInit = (HANDLE)_beginthreadex(NULL, 0, Thread_Function_FirstInit, NULL, 0, (unsigned*)&pSample->Thread_FirstInit_ID);
		if (pSample->Thread_FirstInit == 0)
		{
			//쓰레드 생성 실패
			exit(1);
		}
	}

	m_UI.Frame();

	if (m_UI.m_pSelectScene == m_UI.m_SceneList[INGAME])
	{
		if (m_UI.m_pSelectScene->m_dwCurrentState <= INGAME_STATE_STAGE_4_BATTLE)
		{
			m_Map.Frame();
			m_Character.Frame();

			m_Effect.SetHeroPos(m_Character.pHero->GetPosition());
			m_Effect.SetHeroDir(m_Character.pHero->GetDirection());
			m_Effect.SetStageState(m_UI.m_SceneList[INGAME]->m_TextureCtl[INGAME_TEXTURE_STAGE]->m_dwCurrentState);
			m_Effect.Frame();
		}
	}

	return true;
}

bool Sample::Render()
{
	if (m_UI.m_pSelectScene == m_UI.m_SceneList[INGAME])
	{
		m_Map.Render();
		m_Character.Render();
		m_Effect.Render();
		if (m_UI.m_pSelectScene->m_dwCurrentState == INGAME_STATE_BACKTOMAIN)
		{
			g_LoadingNum = 0;
			ThreadFunctionCall = false;
			m_Map.m_bItemInited = false;
			m_Effect.m_bSnowDrop = true;
			m_Character.GameReset();
			m_Effect.Release();
			m_UI.GotoMainState();
		}
	}
	m_UI.Render();
	return true;
}

bool Sample::Release()
{
	m_UI.Release();
	m_Map.Release();
	m_Character.Release();
	m_Effect.Release();
	return true;
}


Sample::Sample()
{
	ThreadFunctionCall = false;
}


Sample::~Sample()
{
}

BBASIS_START;
BBASIS_WIN(L"CODEGAME");