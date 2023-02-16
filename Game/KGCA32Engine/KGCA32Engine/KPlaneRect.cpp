#include "KPlaneRect.h"

void K_ControlUI::SetTransition(DWORD dwEvent)
{
	m_dwCurrentState = m_FSM.GetOutputState(m_dwCurrentState, dwEvent);
}

void K_ControlUI::Set_SRT(
	float Sca_X,
	float Sca_Y,
	float Sca_Z)
{
	m_vScale.x = Sca_X;
	m_vScale.y = Sca_Y;
	m_vScale.z = Sca_Z;
}
bool K_ControlUI::Update_rtSize()
{
	int Weight = (m_rt.right - m_rt.left);
	int Height = (m_rt.bottom - m_rt.top);
	//이동을 위한 구간
	m_rtSize[MOVE].left = m_rt.left + (Weight*0.1);
	m_rtSize[MOVE].right = m_rt.right - (Weight*0.1);
	m_rtSize[MOVE].top = m_rt.top + (Height*0.1);
	m_rtSize[MOVE].bottom = m_rt.bottom - (Height*0.1);

	//왼쪽
	m_rtSize[LEFT].left = m_rt.left;
	m_rtSize[LEFT].right = m_rtSize[MOVE].left;
	m_rtSize[LEFT].top = m_rtSize[MOVE].top;
	m_rtSize[LEFT].bottom = m_rtSize[MOVE].bottom;

	//오른쪽
	m_rtSize[RIGHT].left = m_rtSize[MOVE].right;
	m_rtSize[RIGHT].right = m_rt.right;
	m_rtSize[RIGHT].top = m_rtSize[MOVE].top;
	m_rtSize[RIGHT].bottom = m_rtSize[MOVE].bottom;

	//위쪽
	m_rtSize[UP].left = m_rtSize[MOVE].left;
	m_rtSize[UP].right = m_rtSize[MOVE].right;
	m_rtSize[UP].top = m_rt.top;
	m_rtSize[UP].bottom = m_rtSize[MOVE].top;

	//아래쪽
	m_rtSize[DOWN].left = m_rtSize[MOVE].left;
	m_rtSize[DOWN].right = m_rtSize[MOVE].right;
	m_rtSize[DOWN].top = m_rtSize[MOVE].bottom;
	m_rtSize[DOWN].bottom = m_rt.bottom;

	//왼위
	m_rtSize[LEFT_UP].left = m_rt.left;
	m_rtSize[LEFT_UP].right = m_rtSize[MOVE].left;
	m_rtSize[LEFT_UP].top = m_rt.top;
	m_rtSize[LEFT_UP].bottom = m_rtSize[MOVE].top;

	//오위
	m_rtSize[RIGHT_UP].left = m_rtSize[MOVE].right;
	m_rtSize[RIGHT_UP].right = m_rt.right;
	m_rtSize[RIGHT_UP].top = m_rt.top;
	m_rtSize[RIGHT_UP].bottom = m_rtSize[MOVE].top;

	//왼아
	m_rtSize[LEFT_DOWN].left = m_rt.left;
	m_rtSize[LEFT_DOWN].right = m_rtSize[MOVE].left;
	m_rtSize[LEFT_DOWN].top = m_rtSize[MOVE].bottom;
	m_rtSize[LEFT_DOWN].bottom = m_rt.bottom;

	//오아
	m_rtSize[RIGHT_DOWN].left = m_rtSize[MOVE].right;
	m_rtSize[RIGHT_DOWN].right = m_rt.right;
	m_rtSize[RIGHT_DOWN].top = m_rtSize[MOVE].bottom;
	m_rtSize[RIGHT_DOWN].bottom = m_rt.bottom;

	return true;
}

//bool	K_ControlUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
//	TCHAR* vs,
//	TCHAR* ps,
//	TCHAR* pTexName)
//{
//	if (pTexName != NULL)
//	{
//		ZeroMemory(m_Text, sizeof(m_Text));
//		_tcscpy(m_Text, pTexName);
//	}
//	m_Button.Create(pDevice, pContext, vs, ps, pTexName);
//	m_pPlane = &m_Button;
//	return true;
//}

void	K_ControlUI::SetMatrix(TMatrix* pWorld, TMatrix* pView, TMatrix* pProj)
{
	if (pWorld != NULL)
	{
		m_matWorld = *pWorld;
		//오브젝트의 중점좌표 설정
		//m_pPlane->m_vCenter.x = 0.0f;	
		//m_pPlane->m_vCenter.y = 0.0f;
		//m_pPlane->m_vCenter.z = 0.0f;
	}
	if (pView != NULL)
	{
		m_pPlane->m_matView = *pView;
	}
	if (pProj != NULL)
	{
		m_pPlane->m_matProj = *pProj;
	}

	D3DXMatrixTranspose(&m_pPlane->m_cbData.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_pPlane->m_cbData.matView, &m_pPlane->m_matView);
	D3DXMatrixTranspose(&m_pPlane->m_cbData.matProj, &m_pPlane->m_matProj);
}

void	K_ControlUI::SetAmbientColor(float fR, float fG, float fB, float fA)
{
	m_pPlane->m_cbData.Color = TVector4(fR, fG, fB, fA);
}

bool K_ControlUI::Init(float ScaX, float ScaY, float ScaZ)
{
	//스케일, 회전, 이동 초기화
	//m_vScale.x = 100; m_vScale.y = 100; m_vScale.z = 100;

	Set_SRT(ScaX, ScaY, ScaZ);
	m_vRotate.x = 0; m_vRotate.y = 0; m_vRotate.z = 0;
	D3DXMatrixRotationYawPitchRoll(&m_matRotate, m_vRotate.x, m_vRotate.y, m_vRotate.z);
	m_vTrans.x = 0; m_vTrans.y = 0; m_vTrans.z = 0;

	D3DXMatrixScaling(&m_matWorld, m_vScale.x, m_vScale.y, m_vScale.z);
	m_matWorld._41 = 0;
	m_matWorld._42 = 0;
	m_matWorld._43 = 0;

	//충돌 평면 구성
	m_Vertices[0].v = TVector3(-1.0f, 1.0f, -1.0f);	m_Vertices[0].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[0].n = TVector3(0.0f, 0.0f, 1.0f);    m_Vertices[0].t = TVector2(0.0f, 0.0f);
	m_Vertices[1].v = TVector3(1.0f, 1.0f, -1.0f);	m_Vertices[1].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[1].n = TVector3(0.0f, 0.0f, 1.0f);    m_Vertices[1].t = TVector2(1.0f, 0.0f);
	m_Vertices[2].v = TVector3(1.0f, -1.0f, -1.0f);	m_Vertices[2].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[2].n = TVector3(0.0f, 0.0f, 1.0f);    m_Vertices[2].t = TVector2(1.0f, 1.0f);
	m_Vertices[3].v = TVector3(-1.0f, -1.0f, -1.0f);	m_Vertices[3].c = TVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Vertices[3].n = TVector3(0.0f, 0.0f, 1.0f);    m_Vertices[3].t = TVector2(0.0f, 1.0f);


	TVector3 vEye = TVector3(0.0f, 0.0f, -10.0f);
	TVector3 vAt = TVector3(0.0f, 0.0f, 0.0f);
	TVector3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_matView, &vEye, &vAt, &vUp);

	return true;
}

HRESULT	K_ControlUI::CreateResource(int iRectWidth, int iRectHeight)
{
	HRESULT hr = S_OK;
	iRectWidth = 1920 / 2;
	iRectHeight = 1080 / 2;
	//TMatrixOrthoLH(&m_matProj, iRectWidth * 2, iRectHeight * 2, 0.0f, 1000.0f);
	// 화면 중앙이 원점으로 계산되기 때문에 넓이 및 높이가 -1 ~ 1 범위로 직교투영된다. 
	D3DXMatrixOrthoOffCenterLH(&m_matProj, -iRectWidth, iRectWidth, -iRectHeight, iRectHeight, 0.0f, 1000.0f);
	return hr;
}

bool K_ControlUI::Set(UINT iWidth, UINT iHeight)
{
	m_iWidthVP = 1920;
	m_iHeightVP = 1080;
	D3DXMatrixIdentity(&m_matViewPort);

	m_matViewPort._11 = 1;
	m_matViewPort._22 = -1;
	m_matViewPort._41 = (m_iWidthVP / 2.0f);
	m_matViewPort._42 = (m_iHeightVP / 2.0f);

	//CreateResource(iWidth, iHeight);
	return true;
}
void	K_ControlUI::Move(float x, float y, float z)
{
	m_vTrans.x = x; //-200, 150, 0
	m_vTrans.y = y;
	m_vTrans.z = z;
	Center(x, y, z);
}
void	K_ControlUI::Scale(float x, float y, float z)
{
	// 200, 150, 1
	m_vScale.x += x;
	m_vScale.y += y;
	m_vScale.z += z;
}
void	K_ControlUI::Rotate(float x, float y, float z)
{
}

void	K_ControlUI::Center(float x, float y, float z)
{
	m_pPlane->m_vCenter.x += x;
	m_pPlane->m_vCenter.x += y;
	m_pPlane->m_vCenter.x += z;
}
bool	K_ControlUI::Update()
{
	Set(g_iWindowWidth, g_iWindowHeight);
	D3DXMatrixScaling(&m_matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	//월드행렬
	m_matWorld = m_matScale * m_matRotate;
	//월드상의 위치값
	m_matWorld._41 = m_vTrans.x;
	m_matWorld._42 = m_vTrans.y;
	m_matWorld._43 = m_vTrans.z;
	D3DXMatrixDecompose(&m_vScale, &m_vQuat, &m_vTrans, &m_matWorld);
	//센터값 갱신

	TVector3 Center[4];

	TVector3 v[4];
	for (int iVer = 0; iVer < 4; iVer++)
	{
		D3DXVec3TransformCoord(&v[iVer], &m_Vertices[iVer].v, &m_matWorld);
		D3DXVec3TransformCoord(&v[iVer], &v[iVer], &m_matView);
		D3DXVec3TransformCoord(&Center[iVer], &m_Vertices[iVer].v, &m_matWorld);
		D3DXVec3TransformCoord(&v[iVer], &v[iVer], &m_matViewPort);
		//D3DXVec3TransformCoord(&Center[iVer], &Center[iVer], &m_matViewPort);
	}
	//중점 좌표의 계산  
	m_pPlane->m_vCenter.x = (Center[0].x + Center[1].x) / 2;
	m_pPlane->m_vCenter.y = (Center[0].y + Center[3].y) / 2;
	//
	m_rt.left = v[0].x;
	m_rt.right = v[1].x;
	m_rt.top = v[0].y;
	m_rt.bottom = v[2].y;

	Update_rtSize();
	return true;
}
bool K_ControlUI::Release()
{
	assert(m_pPlane);
	m_pPlane->Release();

	return true;
}
K_ControlUI::K_ControlUI()
{
	m_dwCurrentState = BUTTON_FREE;
	m_dwCurrentEvent = BUTTON_MOUSE_DOWN;
	Sound_Check = true;
	Sound_Click = true;

	isRenderOk = true;
	isActivateOK = true;
	//m_TextureNum = 0;
}
K_ControlUI::~K_ControlUI()
{

}
bool  K_ButtonCtl::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	TCHAR* vs,
	TCHAR* ps,
	TCHAR* pTexName)
{
	if (pTexName != NULL)
	{
		ZeroMemory(m_Text, sizeof(m_Text));
		_tcscpy(m_Text, pTexName);
	}
	m_Button.Create(pDevice, pContext, vs, ps, pTexName);
	m_pPlane = &m_Button;
	return true;
}
bool K_ButtonCtl::Render()
{
	assert(m_pPlane);
	m_pPlane->SetMatrix(&m_matWorld, &m_matView, &m_matProj);

	//플레인 렌더
	if (m_dwCurrentState == BUTTON_FREE)
		m_pPlane->Render(BUTTON_FREE);
	else if (m_dwCurrentState == BUTTON_UP)
		m_pPlane->Render(BUTTON_UP);
	else if (m_dwCurrentState == BUTTON_CLICKED)
		m_pPlane->Render(BUTTON_CLICKED);
	return true;
}
bool  K_EditCtl::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	TCHAR* vs,
	TCHAR* ps,
	TCHAR* pTexName)
{
	if (pTexName != NULL)
	{
		ZeroMemory(m_Text, sizeof(m_Text));
		_tcscpy(m_Text, pTexName);
	}
	m_Plane.Create(pDevice, pContext, vs, ps, pTexName);
	m_pPlane = &m_Plane;
	return true;
}
bool K_EditCtl::Render()
{
	assert(m_pPlane);
	m_pPlane->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
	m_pPlane->Render(0);
	return true;
}
bool  K_ImageCtl::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	TCHAR* vs,
	TCHAR* ps,
	TCHAR* pTexName)
{
	if (pTexName != NULL)
	{
		ZeroMemory(m_Text, sizeof(m_Text));
		_tcscpy(m_Text, pTexName);
	}
	m_Image.Create(pDevice, pContext, vs, ps, pTexName);
	m_pPlane = &m_Image;
	return true;
}
bool K_ImageCtl::Render()
{
	assert(m_pPlane);
	m_pPlane->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
	m_pPlane->Render(m_TexutreNum);
	return true;
}
bool  K_TextureCtl::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	TCHAR* vs,
	TCHAR* ps,
	TCHAR* pTexName)
{
	if (pTexName != NULL)
	{
		ZeroMemory(m_Text, sizeof(m_Text));
		_tcscpy(m_Text, pTexName);
	}
	m_Texture.Create(pDevice, pContext, vs, ps, pTexName);
	m_pPlane = &m_Texture;
	return true;
}
bool K_TextureCtl::Render()
{
	assert(m_pPlane);
	m_pPlane->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
	m_pPlane->Render(m_dwCurrentState);
	return true;
}
//패널 함수
void K_Panel::SetTransition(DWORD dwEvent)
{
	m_dwCurrentState = m_FSM.GetOutputState(m_dwCurrentState, dwEvent);
}

HRESULT	K_Panel::CreateResource(int iRectWidth, int iRectHeight)
{
	HRESULT hr;
	for (int iCnt = 0; iCnt < m_ImageCtl.size(); iCnt++)
	{
		m_ImageCtl[iCnt]->CreateResource(iRectWidth, iRectHeight);
	}
	for (int iCnt = 0; iCnt < m_ButtonCtl.size(); iCnt++)
	{
		m_ButtonCtl[iCnt]->CreateResource(iRectWidth, iRectHeight);
	}
	for (int iCnt = 0; iCnt < m_EditCtl.size(); iCnt++)
	{
		m_EditCtl[iCnt]->CreateResource(iRectWidth, iRectHeight);
	}
	for (int iCnt = 0; iCnt < m_TextureCtl.size(); iCnt++)
	{
		m_TextureCtl[iCnt]->CreateResource(iRectWidth, iRectHeight);
	}
	return S_OK;
}
bool K_Panel::Render()
{
	//패널의 배경 출력
	for (int iCnt = 0; iCnt < m_ImageCtl.size(); iCnt++)
	{
		if (m_ImageCtl[iCnt]->isRenderOk == true)
			m_ImageCtl[iCnt]->Render();
	}
	//패널의 버튼 출력
	for (int iCnt = 0; iCnt < m_ButtonCtl.size(); iCnt++)
	{
		if (m_ButtonCtl[iCnt]->isRenderOk == true)
			m_ButtonCtl[iCnt]->Render();
	}
	//패널의 에디터 출력
	for (int iCnt = 0; iCnt < m_EditCtl.size(); iCnt++)
	{
		if (m_EditCtl[iCnt]->isRenderOk == true)
			m_EditCtl[iCnt]->Render();
	}
	//패널의 텍스쳐 출력
	for (int iCnt = 0; iCnt < m_TextureCtl.size(); iCnt++)
	{
		if (m_TextureCtl[iCnt]->isRenderOk == true)
			m_TextureCtl[iCnt]->Render();
	}
	return true;
}
bool K_Panel::Release()
{
	//배경화면 삭제
	for (int iCnt = 0; iCnt < m_ImageCtl.size(); iCnt++)
	{
		m_ImageCtl[iCnt]->Release();
	}
	//버튼 삭제
	for (int iCnt = 0; iCnt < m_ButtonCtl.size(); iCnt++)
	{
		m_ButtonCtl[iCnt]->Release();
	}
	//에디터 삭제
	for (int iCnt = 0; iCnt < m_EditCtl.size(); iCnt++)
	{
		m_EditCtl[iCnt]->Release();
	}
	//텍스쳐 삭제
	for (int iCnt = 0; iCnt < m_TextureCtl.size(); iCnt++)
	{
		m_TextureCtl[iCnt]->Release();
	}
	return true;
}

void K_SceneUI::SetTransition(DWORD dwEvent)
{
	m_dwCurrentState = m_FSM.GetOutputState(m_dwCurrentState, dwEvent);
}

HRESULT	K_SceneUI::CreateResource(int iRectWidth, int iRectHeight)
{
	HRESULT hr;
	for (int iCnt = 0; iCnt < m_PanelList.size(); iCnt++)
	{
		m_PanelList[iCnt]->CreateResource(iRectWidth, iRectHeight);
	}
	for (int iCnt = 0; iCnt < m_ImageCtl.size(); iCnt++)
	{
		m_ImageCtl[iCnt]->CreateResource(iRectWidth, iRectHeight);
	}
	for (int iCnt = 0; iCnt < m_ButtonCtl.size(); iCnt++)
	{
		m_ButtonCtl[iCnt]->CreateResource(iRectWidth, iRectHeight);
	}
	for (int iCnt = 0; iCnt < m_EditCtl.size(); iCnt++)
	{
		m_EditCtl[iCnt]->CreateResource(iRectWidth, iRectHeight);
	}
	for (int iCnt = 0; iCnt < m_TextureCtl.size(); iCnt++)
	{
		m_TextureCtl[iCnt]->CreateResource(iRectWidth, iRectHeight);
	}

	return S_OK;
}

bool K_SceneUI::Render()
{
	//배경화면 출력
	for (int iCnt = 0; iCnt < m_ImageCtl.size(); iCnt++)
	{
		if (m_ImageCtl[iCnt]->isRenderOk == true)
			m_ImageCtl[iCnt]->Render();
	}
	//버튼 출력
	for (int iCnt = 0; iCnt < m_ButtonCtl.size(); iCnt++)
	{
		if (m_ButtonCtl[iCnt]->isRenderOk == true)
			m_ButtonCtl[iCnt]->Render();
	}
	//에디터 출력
	for (int iCnt = 0; iCnt < m_EditCtl.size(); iCnt++)
	{
		if (m_EditCtl[iCnt]->isRenderOk == true)
			m_EditCtl[iCnt]->Render();
	}
	//텍스쳐 출력
	for (int iCnt = 0; iCnt < m_TextureCtl.size(); iCnt++)
	{
		if (m_TextureCtl[iCnt]->isRenderOk == true)
			m_TextureCtl[iCnt]->Render();
	}
	//패널이 선택됐을때 렌더
	if (m_pSelectPanel != NULL)
		m_pSelectPanel->Render();
	return true;
}

bool K_SceneUI::Release()
{
	//패널 삭제
	for (int iCnt = 0; iCnt < m_PanelList.size(); iCnt++)
	{
		m_PanelList[iCnt]->Release();
	}
	//배경화면 삭제
	for (int iCnt = 0; iCnt < m_ImageCtl.size(); iCnt++)
	{
		m_ImageCtl[iCnt]->Release();
	}
	//버튼 삭제
	for (int iCnt = 0; iCnt < m_ButtonCtl.size(); iCnt++)
	{
		m_ButtonCtl[iCnt]->Release();
	}
	//에디터 삭제
	for (int iCnt = 0; iCnt < m_EditCtl.size(); iCnt++)
	{
		m_EditCtl[iCnt]->Release();
	}
	//텍스쳐 삭제
	for (int iCnt = 0; iCnt < m_TextureCtl.size(); iCnt++)
	{
		m_TextureCtl[iCnt]->Release();
	}
	return true;
}