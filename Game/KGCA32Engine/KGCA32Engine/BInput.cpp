#include "BInput.h"

#define KEYDOWN(key)		(((m_KeyState[key]) & 0x80)? true : false)
#define KEYUP(key)			(((m_KeyState[key]) & 0x80)? false : true)

namespace BBASIS 
{
	BBASIS_INPUT_MAP g_InputData;
}

HRESULT BInput::InitDirectInput()
{
	HRESULT hr = S_OK;
	if (FAILED(hr = DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL)))
	{
		return false;
	}
	if (FAILED(hr = m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyDevice, NULL)))
	{
		return false;
	}
	if (FAILED(hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL)))
	{
		return false;
	}
	// 장치별 반환 데이터 설정 (키보드)
	m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	// 장치별 반환 데이터 설정 (마우스)
	m_pMouseDevice->SetDataFormat(&c_dfDIMouse);

	// 장치별 독점 및 비독점 설정(협조레벨)
	if (FAILED(hr = m_pKeyDevice->SetCooperativeLevel(m_hWnd, // MFC 버전이라 g_hWnd 에서 m_hWnd로 일시변경
		DISCL_NONEXCLUSIVE | // 독점해서 사용하지 않겠다.
		DISCL_FOREGROUND |	 // 화면이 전면일 때만 받겠다.
		DISCL_NOWINKEY)))	 // 시작프로그램 Win키를 막는다.
	{
		return false;
	}
	while (m_pKeyDevice->Acquire() == DIERR_INPUTLOST); // 키를 받을때 까지 무한반복
	
	if (FAILED(hr = m_pMouseDevice->SetCooperativeLevel(m_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return false;
	}
	while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);

	return hr;
}

BYTE BInput::KeyCheck(BYTE dwKey)
{
	if (KEYDOWN(dwKey))
	{
		if (m_KeyStateOld[dwKey] == KEY_FREE)
			m_KeyStateOld[dwKey] = KEY_PUSH;
		else
			m_KeyStateOld[dwKey] = KEY_HOLD;
	}
	else
	{
		if (m_KeyStateOld[dwKey] == KEY_PUSH || m_KeyStateOld[dwKey] == KEY_HOLD)
			m_KeyStateOld[dwKey] = KEY_UP;
		else
			m_KeyStateOld[dwKey] = KEY_FREE;
	}
	return m_KeyStateOld[dwKey];
}

bool BInput::Init()
{
	ZeroMemory(m_KeyState, sizeof(BYTE) * 256);
	ZeroMemory(m_KeyStateOld, sizeof(BYTE) * 256);
	ZeroMemory(m_MouseState, sizeof(BYTE) * 3);
	ZeroMemory(m_BeforeMouseState, sizeof(BYTE) * 3);
	InitDirectInput();
	for (int iMouse = 0; iMouse < 3; iMouse++)
	{
		m_MFCDIMouseState[iMouse] = KEY_FREE;
		m_MFCDIMouseBeforeState[iMouse] = KEY_FREE;
	}
	return true;
}

bool BInput::frame()
{
	GetCursorPos(&m_MousePos);
	ScreenToClient(g_hWnd, &m_MousePos);

	HRESULT hr = S_FALSE;
	if (m_pMouseDevice == NULL || m_pKeyDevice == NULL) return false;

	if (FAILED(hr = m_pKeyDevice->GetDeviceState(256, m_KeyState)))
	{
		while (m_pKeyDevice->Acquire() == DIERR_INPUTLOST);
	}
	
	
	if (FAILED(hr = m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_DIMouseState)))
	{
		while (m_pMouseDevice->Acquire() == DIERR_INPUTLOST);
	}
	
/*
#define KEYDOWN(key)		(((m_KeyState[key]) & 0x80)? true : false)
#define KEYUP(key)			(((m_KeyState[key]) & 0x80)? false : true)*/

	for (int iButton = 0; iButton < 3; iButton++)
	{
		m_BeforeMouseState[iButton] = m_DIMouseState.rgbButtons[iButton];
	}

	for (int iButton = 0; iButton < 3; iButton++)
	{
		if (m_BeforeMouseState[iButton] & 0x80)
		{
			if (m_MouseState[iButton] == KEY_FREE)
				m_MouseState[iButton] = KEY_PUSH;
			else
				m_MouseState[iButton] = KEY_HOLD;
		}
		else
		{
			if (m_MouseState[iButton] == KEY_PUSH || m_MouseState[iButton] == KEY_HOLD)
				m_MouseState[iButton] = KEY_UP;
			else
				m_MouseState[iButton] = KEY_FREE;
		}
	}
	
	ZeroMemory(&g_InputData, sizeof(BBASIS_INPUT_MAP));

	/*전역으로 확인 가능한 키값*/
	if (m_MouseState[0] == KEY_PUSH)    g_InputData.bLeftClick = true;
	if (m_MouseState[1] == KEY_PUSH) 	g_InputData.bRightClick = true;
	if (m_MouseState[2] == KEY_PUSH) 	g_InputData.bMiddleClick = true;

	if (m_MouseState[0] >= KEY_PUSH) 	g_InputData.bLeftHold = true;
	if (m_MouseState[1] >= KEY_PUSH) 	g_InputData.bRightHold = true;
	if (m_MouseState[2] >= KEY_PUSH) 	g_InputData.bMiddleHold = true;

	if (KeyCheck(DIK_A) == KEY_PUSH || KeyCheck(DIK_A) == KEY_HOLD)
		g_InputData.bAKey = true;
	if (KeyCheck(DIK_D) == KEY_PUSH || KeyCheck(DIK_D) == KEY_HOLD)
		g_InputData.bDKey = true;
	if (KeyCheck(DIK_W) == KEY_PUSH || KeyCheck(DIK_W) == KEY_HOLD)
		g_InputData.bWKey = true;
	if (KeyCheck(DIK_S) == KEY_PUSH || KeyCheck(DIK_S) == KEY_HOLD)
		g_InputData.bSKey = true;
	if (KeyCheck(DIK_Q) == KEY_PUSH || KeyCheck(DIK_Q) == KEY_HOLD)
		g_InputData.bQKey = true;
	if (KeyCheck(DIK_E) == KEY_PUSH || KeyCheck(DIK_E) == KEY_HOLD)
		g_InputData.bEKey = true;
	if (KeyCheck(DIK_SPACE) == KEY_PUSH || KeyCheck(DIK_SPACE) == KEY_HOLD)
		g_InputData.bSpace = true;
	if (KeyCheck(DIK_Z) == KEY_PUSH)
		g_InputData.bZKey = true;
	if (KeyCheck(DIK_C) == KEY_PUSH)
		g_InputData.bCKey = true;
	if (KeyCheck(DIK_3) == KEY_PUSH)
		g_InputData.bBackViewKey = true;
	if (KeyCheck(DIK_4) == KEY_PUSH)
		g_InputData.bDebugViewKey = true;
	if (KeyCheck(DIK_5) == KEY_PUSH)
		g_InputData.bTreeViewKey = true;

	g_InputData.iMouseValue[0] = m_DIMouseState.lX;
	g_InputData.iMouseValue[1] = m_DIMouseState.lY;
	g_InputData.iMouseValue[2] = m_DIMouseState.lZ;

	return true;
}
bool BInput::Render()
{
	return true;
}
bool BInput::Release()
{
	if (m_pKeyDevice) m_pKeyDevice->Unacquire();
	if (m_pMouseDevice) m_pMouseDevice->Unacquire();
	S_RELEASE(m_pKeyDevice);
	S_RELEASE(m_pMouseDevice);
	S_RELEASE(m_pDI);
	return true;
}

void BInput::DeviceAcquire()
{
	if (m_pKeyDevice)
		m_pKeyDevice->Acquire();
	if (m_pMouseDevice)
		m_pMouseDevice->Acquire();
}
void BInput::DeviceUnacquire()
{
	if (m_pKeyDevice)
		m_pKeyDevice->Unacquire();
	if (m_pMouseDevice)
		m_pMouseDevice->Unacquire();
}

void BInput::MsgEvent(MSG msg)
{
	switch (msg.message)
	{
	case WM_LBUTTONDOWN:
	{
		if (m_MouseState[0] == KEY_PUSH)
			m_MouseState[0] = KEY_HOLD;
		else
			m_MouseState[0] = KEY_PUSH;
	}break;
	case WM_LBUTTONUP:
	{
		m_MouseState[0] = KEY_UP;
	}break;
	case WM_RBUTTONDOWN:
	{
		if (m_MouseState[1] == KEY_PUSH)
			m_MouseState[1] = KEY_HOLD;
		else
			m_MouseState[1] = KEY_PUSH;
	}break;
	case WM_RBUTTONUP:
	{
		m_MouseState[1] = KEY_UP;
	}break;
	case WM_MBUTTONDOWN:
	{
		if (m_MouseState[2] == KEY_PUSH)
			m_MouseState[2] = KEY_HOLD;
		else
			m_MouseState[2] = KEY_PUSH;
	}break;
	case WM_MBUTTONUP:
	{
		m_MouseState[2] = KEY_UP;
	}break;
	}
}

BInput::BInput()
{
	m_pDI = NULL;
	m_dwImmediate = TRUE;
	m_pMouseDevice = NULL;
	m_pKeyDevice = NULL;
}


BInput::~BInput()
{
}
