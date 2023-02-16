#pragma once
#include "BStd.h"

using namespace BBASIS;

enum keyState {
	KEY_FREE = 0,
	KEY_UP = 1,
	KEY_PUSH = 2,
	KEY_HOLD = 3,
};

class BInput : public BSingleton <BInput>
{
private:
	friend class BSingleton<BInput>;

public:
	LPDIRECTINPUT8 m_pDI;					// 다이렉트 인풋 객체
	LPDIRECTINPUTDEVICE8 m_pKeyDevice;		// 키보드
	LPDIRECTINPUTDEVICE8 m_pMouseDevice;	// 마우스
	
	DIDEVICEOBJECTDATA m_didod[16];


	BYTE m_KeyState[256];					// 키보드에서 받아오는 입력형태의 수
	BYTE m_KeyStateOld[256];				// hold, up 상태를 확인하기 위한 이전 상태값
	DIMOUSESTATE m_DIMouseState;			// 마우스 상태값
	DIMOUSESTATE m_DIMouseBeforeState;		// 마우스 이전상태값
  
	TCHAR m_csBuffer[260];
	BYTE m_MouseState[3];
	BYTE m_BeforeMouseState[3];
	POINT m_MousePos;
	DWORD m_dwImmediate;

	/*MFC용*/
	BYTE m_MFCDIMouseState[3];
	BYTE m_MFCDIMouseBeforeState[3];
	HWND m_hWnd;
public:
	bool Init();
	bool frame();
	bool Render();
	bool Release();

	BYTE KeyCheck(BYTE dwKey);
	HRESULT InitDirectInput();
	void DeviceAcquire();
	void DeviceUnacquire();
	void MsgEvent(MSG msg);

public:
	BInput();
	~BInput();
};

#define I_Input BInput::GetInstance()