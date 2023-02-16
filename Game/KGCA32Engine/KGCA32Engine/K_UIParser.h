#pragma once
#include "BCore.h"
#include "KPlaneRect.h"
#include "K_Sound.h"
//#include "BMouseShape.h"
#include "LALoadObjectManager.h"

struct Dot
{
	float x;
	float y;
};

class K_UIParser
{
public:
	//디바이스 멤버 변수
	HWND					m_hWnd;
	ID3D11Device*			m_pd3dDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
	//DXGI_SWAP_CHAIN_DESC	m_pSwapChainDesc;
	UINT					m_iWindowWidth;		// 클라이언트 영역 가로크기
	UINT					m_iWindowHeight;	// 을라이언트 영역 세로크기

	ComPtr<ID3D11BlendState>		m_pAlphaTesting;		// 블렌드 디바이스

	std::vector<K_SceneUI*> m_SceneList;	//Scene리스트
	K_SceneUI*					m_pSelectScene;	//선택된 씬

												//캐릭터로부터 받아오는 데이터
	LAHero* pHeroData;
	const LACharacterUIData* m_pCharacterUIData;
	StructInventory*	 m_pInventoryData;

	//파일을 로드할때 필요
	FILE* m_pStream;	//파일 스트림
	TCHAR m_pBuffer[256];	//파일 내용을 읽어 올 버퍼
	TCHAR m_pString[256];

	K_Sound m_Sound;	//사운드
	//BMouseShape		m_Mouse;	//마우스
public:
	bool PlaneSelectIsOk[9];		//플레인의 각 구간이 선택되었는지 확인하는 변수
	bool MousePickisTrue;			//마우스가 피킹됐는지 확인하는 변수

									//마우스 포인트
	Dot B_MousePoint;
	Dot A_MousePoint;
	//
	bool isModifiedOK;				//편집이 가능한 상태 표시
	bool isToolPanelOK;				//툴에서 패널을 편집할때 사용하는 변수

	bool isRealGame;				//true일경우 실제 게임, false일경우 툴 상
	bool isGamePanelOK;				//실제 게임상에서 패널과 씬을 구별하기 위한 변수
	bool isGameStart;				//게임이 시작됐는지 확인	//시작버튼 누를시 한번 true로

	K_ControlUI* SelectPlane;		//선택된 플레인
	int			 PlaneNUM;			//플레인의 번호

									//=====시간 관련 변수=====
	int	FarmingOrBattle;			//파밍씬(=홀수)인지 배틀씬(=짝수)인지 이외엔 0
	float m_fFarmingTick;			//파밍시간
	float m_fBattleTick;			//배틀시간

	float m_SecondimeTick;			//초단위 증가 틱
	float m_SecnodTimeChangeTerm;	//초단위 변경 텀

	int	  m_SecondCount;			//1초단위 카운트
	int	  m_TenSecondCount;			//10초단위 카운트
	int	  m_MinuteCount;			//1분단위 카운트
									//========================
	float m_PhaseChangeTick;		//페이스변경 틱
	float m_PhaseChangeTerm;		//페이스 변경 텀 = 1초

	float m_HPBAR_ChangeTick;		//HPBAR변경 틱

	//에임 변경 틱
	float m_AIM_TextureChangePlusTick;	//변경 증가 틱
	float m_AIM_TextureChangePlusTerm;	//변경 증가 텀

	float m_AIM_TextureChangeMinusTick;	//변경 감소 틱
	float m_AIM_TextureChangeMinusTerm;	//변경 감소 텀

	//인벤창에서 정보패널
	float InvenInfoTextureRenderTick;
	int ButtonNum;
	bool InvenInfoisRender;
	bool isRenderButton;
	//
	bool MouseCursorOn;

public:
	//멤버 함수
	bool AddRect(KUI_TYPE type, TCHAR* TextureStr,
		float Sca_X = 100.0f,
		float Sca_Y = 100.0f,
		float Sca_Z = 100.0f,
		KPS_TYPE ps_type = K_SCENE);		//UIPlane을 추가하는 함수

	K_ControlUI* SelectRect();

	//파일 로드 함수
	bool Load(TCHAR* szFileName);	//파일 로드함수
	bool LoadScene();	//씬 로드함수
	bool LoadPanel();
	//오브젝트 로드 함수
	bool LoadButton(KPS_TYPE type = K_SCENE);	//버튼
	bool LoadEdit(KPS_TYPE type = K_SCENE);		//에디트
	bool LoadTexture(KPS_TYPE type = K_SCENE);	//텍스쳐
	bool Load_Texture(LOADSECTION ObjectNum, KPS_TYPE type = K_SCENE);	//텍스쳐 로드 함수

	bool SetState();
	bool SetUIFSM();
	void GotoMainState();

	bool SceneButtonSelect();
	bool PanelButtonSelect();

	bool SceneStateChange();
	bool PanelStateChange();

	bool TimeStateChange();	//시간상태변화 함수
	HRESULT CreateResource(int iWidth, int iHeight);

	//데이터 출력에 필요한 함수
	bool CharacterUIDataLoad();	//UI데이터 로드
	bool CharacterSceneDigitStateSet(int Dight, int Count, int TexTureNum);	//씬 캐릭터 데이터 셋팅
	bool CharacterPanelDigitStateSet(int Dight, int Count, int TexTureNum); //패널 캐릭터 데이터 셋팅
	void PickUpTextureRender();												//줍기 텍스쳐를 렌더링하는 함수
	void PickUpTextureNotRender();
	//사운드 셋팅함수
	bool BackgroundMusicSet(int iVolume);
	bool SoundEffectSet(int iVolume);

	void TextureSizeChange(int Texture_Num, K_SceneUI* pSelectScene);	//텍스쳐크기변경 함수

	void SoundStopIngame();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	K_UIParser();
	~K_UIParser();
};