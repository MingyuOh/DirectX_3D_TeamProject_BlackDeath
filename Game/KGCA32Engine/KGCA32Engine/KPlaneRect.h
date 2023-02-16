#pragma once
#include "K_Plane.h"
#include "FSMState.h"
#include "FiniteStateMachine.h"

enum RT_TYPE {
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	LEFT_UP,
	RIGHT_UP,
	LEFT_DOWN,
	RIGHT_DOWN,
	MOVE,
};

class K_ControlUI
{
public:
	K_Plane*		m_pPlane;				//������
											//K_Plane			m_Button;				//������Ʈ


	TCHAR			m_Text[256];			//�ؽ���
	PNCT_VERTEX		m_Vertices[4];			//����

	RECT			m_rtSize[9];
	RECT			m_rt;

	TMatrix		m_matViewPort;
	TMatrix		m_matWorld;
	TMatrix		m_matView;
	TMatrix		m_matProj;
	//��Ʈ���� ���
	TMatrix		m_matScale;
	TMatrix		m_matRotate;

	TVector3		m_vScale;					//���������
	TVector3		m_vRotate;					//ȸ�����
	TVector3		m_vTrans;					//�̵����

	TQuaternion	m_vQuat;
	UINT			m_iWidthVP;
	UINT			m_iHeightVP;

	//int				m_TextureNum;				//���콺���¿����� �ؽ��ĸ� �����ϴ� int�� ����

	bool Sound_Check;								//��ư�� ���尡 �ߺ����� �ҷ����°� �����ϱ����� bool����
	bool Sound_Click;
	//���� ��ȯ
	FiniteStateMachine	m_FSM;
	DWORD	m_dwCurrentState;	//���� ����
	DWORD	m_dwCurrentEvent;	//Process�Լ��κ��� ���� �̺�Ʈ

	void SetTransition(DWORD dwEvent);	//���� ��ȯ �Լ�
										//

	bool isRenderOk;	//�����ص� �Ǵ���
	bool isActivateOK;	//�����ص� �Ǵ���
public:
	virtual void	SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj);
	virtual void	SetAmbientColor(float fR, float fG, float fB, float fA);

	virtual bool  Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		TCHAR* vs,
		TCHAR* ps,
		TCHAR* pTexName)
	{
		return true;
	};

	virtual HRESULT		CreateResource(int iRectWidth, int iRectHeight);
	virtual bool		Set(UINT iWidth, UINT iHeight);
	bool	Update_rtSize();
	void Set_SRT(
		float Sca_X,
		float Sca_Y,
		float Sca_Z);
	virtual void		Move(float x, float y, float z);
	virtual void		Scale(float x, float y, float z);
	virtual void		Rotate(float x, float y, float z);
	virtual void		Center(float x, float y, float z);
	virtual bool		Update();
	bool Init(float ScaX, float ScaY, float ScaZ);
	virtual bool Render() { return true; };
	bool Release();
public:
	K_ControlUI();
	virtual ~K_ControlUI();
};

//��ư Ŭ����
class K_ButtonCtl : public	K_ControlUI
{
public:
	K_Plane m_Button;
public:
	bool  Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		TCHAR* vs,
		TCHAR* ps,
		TCHAR* pTexName);
	bool Render();
public:
	K_ButtonCtl() {
		//���¸ӽſ� ����� �� ���
		//NONE���¿���
		m_FSM.AddStateTransition(BUTTON_FREE, BUTTON_MOUSE_UP, BUTTON_UP);
		m_FSM.AddStateTransition(BUTTON_FREE, BUTTON_MOUSE_DOWN, BUTTON_FREE);
		//UP���¿���
		m_FSM.AddStateTransition(BUTTON_UP, BUTTON_MOUSE_CLICKED, BUTTON_CLICKED);
		m_FSM.AddStateTransition(BUTTON_UP, BUTTON_MOUSE_UP, BUTTON_UP);
		m_FSM.AddStateTransition(BUTTON_UP, BUTTON_MOUSE_DOWN, BUTTON_FREE);
		//CLICKED���¿���
		//m_FSM.AddStateTransition(BUTTON_CLICKED, BUTTON_MOUSE_CLICKED, BUTTON_CLICKED);
		m_FSM.AddStateTransition(BUTTON_CLICKED, BUTTON_MOUSE_DOWN, BUTTON_FREE);
	};
	virtual ~K_ButtonCtl() {};
};

//����Ʈ Ŭ����
class K_EditCtl : public K_ControlUI
{
public:
	K_Plane m_Plane;
public:
	bool  Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		TCHAR* vs,
		TCHAR* ps,
		TCHAR* pTexName);
	bool Render();
public:
	K_EditCtl() {
	};
	virtual ~K_EditCtl() {};
};
//�̹��� Ŭ����
class K_ImageCtl : public K_ControlUI
{
public:
	K_Plane m_Image;

	int m_TexutreNum;
public:
	bool  Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		TCHAR* vs,
		TCHAR* ps,
		TCHAR* pTexName);
	bool Render();
public:
	K_ImageCtl() {
		m_TexutreNum = 0;
	};
	virtual ~K_ImageCtl() {};
};
//�ؽ��� Ŭ����
class K_TextureCtl : public K_ControlUI
{
public:
	K_Plane m_Texture;
public:
	bool  Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		TCHAR* vs,
		TCHAR* ps,
		TCHAR* pTexName);
	bool Render();
public:
	K_TextureCtl() {
	};
	virtual ~K_TextureCtl() {};
};
//1���� ��濡�� �ߴ� ��ȭ���� ����
class K_Panel
{
public:
	std::vector<K_ImageCtl*>	m_ImageCtl;		//���ȭ��
	std::vector<K_ButtonCtl*>	m_ButtonCtl;	//��ư
	std::vector<K_EditCtl*>		m_EditCtl;		//����Ʈ
	std::vector<K_TextureCtl*>	m_TextureCtl;	//�ؽ���

	K_ControlUI* m_pSelectPlane;

	//���º�ȯ
	FiniteStateMachine	m_FSM;
	DWORD	m_dwCurrentState;	//���� ����
	DWORD	m_dwCurrentEvent;	//Process�Լ��κ��� ���� �̺�Ʈ
	void SetTransition(DWORD dwEvent);	//���� ��ȯ �Լ�
	HRESULT CreateResource(int iRectWidth, int iRectHeight);
public:
	bool Render();
	bool Release();

	K_Panel() {
		m_dwCurrentState = MAIN_PANEL_OFF;
		m_pSelectPlane = NULL;
	};
	virtual ~K_Panel() {};
};

//��� ȭ���� ��ü �Ǵ� ����
class K_SceneUI
{
public:
	std::vector<K_Panel*>		m_PanelList;	//�г�
	std::vector<K_ImageCtl*>	m_ImageCtl;		//���ȭ��
	std::vector<K_ButtonCtl*>	m_ButtonCtl;	//��ư
	std::vector<K_EditCtl*>		m_EditCtl;		//����Ʈ
	std::vector<K_TextureCtl*>	m_TextureCtl;	//�ؽ���

	K_ControlUI* m_pSelectPlane;	//������ ���õ� �÷���
	K_Panel* m_pSelectPanel;	//������ ���õ� �г�

	bool PlaneisOk;

	//���º�ȯ
	FiniteStateMachine	m_FSM;
	DWORD	m_dwCurrentState;	//���� ����
	DWORD	m_dwCurrentEvent;	//Process�Լ��κ��� ���� �̺�Ʈ
	void SetTransition(DWORD dwEvent);	//���� ��ȯ �Լ�

	HRESULT		CreateResource(int iRectWidth, int iRectHeight);
public:
	bool Render();
	bool Release();

	K_SceneUI() {
		m_pSelectPanel = NULL;
		m_pSelectPlane = NULL;
		PlaneisOk = false;
	};
	virtual ~K_SceneUI() {};
};