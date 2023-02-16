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
	//����̽� ��� ����
	HWND					m_hWnd;
	ID3D11Device*			m_pd3dDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
	//DXGI_SWAP_CHAIN_DESC	m_pSwapChainDesc;
	UINT					m_iWindowWidth;		// Ŭ���̾�Ʈ ���� ����ũ��
	UINT					m_iWindowHeight;	// �����̾�Ʈ ���� ����ũ��

	ComPtr<ID3D11BlendState>		m_pAlphaTesting;		// ���� ����̽�

	std::vector<K_SceneUI*> m_SceneList;	//Scene����Ʈ
	K_SceneUI*					m_pSelectScene;	//���õ� ��

												//ĳ���ͷκ��� �޾ƿ��� ������
	LAHero* pHeroData;
	const LACharacterUIData* m_pCharacterUIData;
	StructInventory*	 m_pInventoryData;

	//������ �ε��Ҷ� �ʿ�
	FILE* m_pStream;	//���� ��Ʈ��
	TCHAR m_pBuffer[256];	//���� ������ �о� �� ����
	TCHAR m_pString[256];

	K_Sound m_Sound;	//����
	//BMouseShape		m_Mouse;	//���콺
public:
	bool PlaneSelectIsOk[9];		//�÷����� �� ������ ���õǾ����� Ȯ���ϴ� ����
	bool MousePickisTrue;			//���콺�� ��ŷ�ƴ��� Ȯ���ϴ� ����

									//���콺 ����Ʈ
	Dot B_MousePoint;
	Dot A_MousePoint;
	//
	bool isModifiedOK;				//������ ������ ���� ǥ��
	bool isToolPanelOK;				//������ �г��� �����Ҷ� ����ϴ� ����

	bool isRealGame;				//true�ϰ�� ���� ����, false�ϰ�� �� ��
	bool isGamePanelOK;				//���� ���ӻ󿡼� �гΰ� ���� �����ϱ� ���� ����
	bool isGameStart;				//������ ���۵ƴ��� Ȯ��	//���۹�ư ������ �ѹ� true��

	K_ControlUI* SelectPlane;		//���õ� �÷���
	int			 PlaneNUM;			//�÷����� ��ȣ

									//=====�ð� ���� ����=====
	int	FarmingOrBattle;			//�Ĺ־�(=Ȧ��)���� ��Ʋ��(=¦��)���� �̿ܿ� 0
	float m_fFarmingTick;			//�Ĺֽð�
	float m_fBattleTick;			//��Ʋ�ð�

	float m_SecondimeTick;			//�ʴ��� ���� ƽ
	float m_SecnodTimeChangeTerm;	//�ʴ��� ���� ��

	int	  m_SecondCount;			//1�ʴ��� ī��Ʈ
	int	  m_TenSecondCount;			//10�ʴ��� ī��Ʈ
	int	  m_MinuteCount;			//1�д��� ī��Ʈ
									//========================
	float m_PhaseChangeTick;		//���̽����� ƽ
	float m_PhaseChangeTerm;		//���̽� ���� �� = 1��

	float m_HPBAR_ChangeTick;		//HPBAR���� ƽ

	//���� ���� ƽ
	float m_AIM_TextureChangePlusTick;	//���� ���� ƽ
	float m_AIM_TextureChangePlusTerm;	//���� ���� ��

	float m_AIM_TextureChangeMinusTick;	//���� ���� ƽ
	float m_AIM_TextureChangeMinusTerm;	//���� ���� ��

	//�κ�â���� �����г�
	float InvenInfoTextureRenderTick;
	int ButtonNum;
	bool InvenInfoisRender;
	bool isRenderButton;
	//
	bool MouseCursorOn;

public:
	//��� �Լ�
	bool AddRect(KUI_TYPE type, TCHAR* TextureStr,
		float Sca_X = 100.0f,
		float Sca_Y = 100.0f,
		float Sca_Z = 100.0f,
		KPS_TYPE ps_type = K_SCENE);		//UIPlane�� �߰��ϴ� �Լ�

	K_ControlUI* SelectRect();

	//���� �ε� �Լ�
	bool Load(TCHAR* szFileName);	//���� �ε��Լ�
	bool LoadScene();	//�� �ε��Լ�
	bool LoadPanel();
	//������Ʈ �ε� �Լ�
	bool LoadButton(KPS_TYPE type = K_SCENE);	//��ư
	bool LoadEdit(KPS_TYPE type = K_SCENE);		//����Ʈ
	bool LoadTexture(KPS_TYPE type = K_SCENE);	//�ؽ���
	bool Load_Texture(LOADSECTION ObjectNum, KPS_TYPE type = K_SCENE);	//�ؽ��� �ε� �Լ�

	bool SetState();
	bool SetUIFSM();
	void GotoMainState();

	bool SceneButtonSelect();
	bool PanelButtonSelect();

	bool SceneStateChange();
	bool PanelStateChange();

	bool TimeStateChange();	//�ð����º�ȭ �Լ�
	HRESULT CreateResource(int iWidth, int iHeight);

	//������ ��¿� �ʿ��� �Լ�
	bool CharacterUIDataLoad();	//UI������ �ε�
	bool CharacterSceneDigitStateSet(int Dight, int Count, int TexTureNum);	//�� ĳ���� ������ ����
	bool CharacterPanelDigitStateSet(int Dight, int Count, int TexTureNum); //�г� ĳ���� ������ ����
	void PickUpTextureRender();												//�ݱ� �ؽ��ĸ� �������ϴ� �Լ�
	void PickUpTextureNotRender();
	//���� �����Լ�
	bool BackgroundMusicSet(int iVolume);
	bool SoundEffectSet(int iVolume);

	void TextureSizeChange(int Texture_Num, K_SceneUI* pSelectScene);	//�ؽ���ũ�⺯�� �Լ�

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