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
	K_Plane*		m_pPlane;				//포인터
											//K_Plane			m_Button;				//오브젝트


	TCHAR			m_Text[256];			//텍스쳐
	PNCT_VERTEX		m_Vertices[4];			//정점

	RECT			m_rtSize[9];
	RECT			m_rt;

	TMatrix		m_matViewPort;
	TMatrix		m_matWorld;
	TMatrix		m_matView;
	TMatrix		m_matProj;
	//매트릭스 멤버
	TMatrix		m_matScale;
	TMatrix		m_matRotate;

	TVector3		m_vScale;					//스케일행렬
	TVector3		m_vRotate;					//회전행렬
	TVector3		m_vTrans;					//이동행렬

	TQuaternion	m_vQuat;
	UINT			m_iWidthVP;
	UINT			m_iHeightVP;

	//int				m_TextureNum;				//마우스상태에따라 텍스쳐를 결정하는 int형 변수

	bool Sound_Check;								//버튼의 사운드가 중복으로 불려지는걸 방지하기위한 bool변수
	bool Sound_Click;
	//상태 변환
	FiniteStateMachine	m_FSM;
	DWORD	m_dwCurrentState;	//현재 상태
	DWORD	m_dwCurrentEvent;	//Process함수로부터 얻을 이벤트

	void SetTransition(DWORD dwEvent);	//상태 변환 함수
										//

	bool isRenderOk;	//렌더해도 되는지
	bool isActivateOK;	//동작해도 되는지
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

//버튼 클래스
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
		//상태머신에 경우의 수 등록
		//NONE상태에서
		m_FSM.AddStateTransition(BUTTON_FREE, BUTTON_MOUSE_UP, BUTTON_UP);
		m_FSM.AddStateTransition(BUTTON_FREE, BUTTON_MOUSE_DOWN, BUTTON_FREE);
		//UP상태에서
		m_FSM.AddStateTransition(BUTTON_UP, BUTTON_MOUSE_CLICKED, BUTTON_CLICKED);
		m_FSM.AddStateTransition(BUTTON_UP, BUTTON_MOUSE_UP, BUTTON_UP);
		m_FSM.AddStateTransition(BUTTON_UP, BUTTON_MOUSE_DOWN, BUTTON_FREE);
		//CLICKED상태에서
		//m_FSM.AddStateTransition(BUTTON_CLICKED, BUTTON_MOUSE_CLICKED, BUTTON_CLICKED);
		m_FSM.AddStateTransition(BUTTON_CLICKED, BUTTON_MOUSE_DOWN, BUTTON_FREE);
	};
	virtual ~K_ButtonCtl() {};
};

//에디트 클래스
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
//이미지 클래스
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
//텍스쳐 클래스
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
//1개의 배경에서 뜨는 대화상자 개념
class K_Panel
{
public:
	std::vector<K_ImageCtl*>	m_ImageCtl;		//배경화면
	std::vector<K_ButtonCtl*>	m_ButtonCtl;	//버튼
	std::vector<K_EditCtl*>		m_EditCtl;		//에디트
	std::vector<K_TextureCtl*>	m_TextureCtl;	//텍스쳐

	K_ControlUI* m_pSelectPlane;

	//상태변환
	FiniteStateMachine	m_FSM;
	DWORD	m_dwCurrentState;	//현재 상태
	DWORD	m_dwCurrentEvent;	//Process함수로부터 얻을 이벤트
	void SetTransition(DWORD dwEvent);	//상태 변환 함수
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

//배경 화면이 교체 되는 단위
class K_SceneUI
{
public:
	std::vector<K_Panel*>		m_PanelList;	//패널
	std::vector<K_ImageCtl*>	m_ImageCtl;		//배경화면
	std::vector<K_ButtonCtl*>	m_ButtonCtl;	//버튼
	std::vector<K_EditCtl*>		m_EditCtl;		//에디트
	std::vector<K_TextureCtl*>	m_TextureCtl;	//텍스쳐

	K_ControlUI* m_pSelectPlane;	//씬에서 선택된 플레인
	K_Panel* m_pSelectPanel;	//씬에서 선택된 패널

	bool PlaneisOk;

	//상태변환
	FiniteStateMachine	m_FSM;
	DWORD	m_dwCurrentState;	//현재 상태
	DWORD	m_dwCurrentEvent;	//Process함수로부터 얻을 이벤트
	void SetTransition(DWORD dwEvent);	//상태 변환 함수

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