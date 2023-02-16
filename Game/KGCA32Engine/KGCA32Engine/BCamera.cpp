#include "BCamera.h"
#include "BInput.h"
#include "BTimer.h"

/*카메라에는 프러스텀이 맴버로 포함되어 있습니다.*/
// 사용은 함수 CreateFrustum / 함수 FrustumRender를 통해 사용하실 수 있습니다.

bool BCamera::Init(TVector3 Pos, TVector3 Target)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_vPos = Pos;
	m_vTarget = Target;
	m_vUp = TVector3(0.0f, 1.0f, 0.0f);

	SetViewMatrix(m_vPos, m_vTarget, m_vUp);
	SetProjMatrix(g_iWindowWidth, g_iWindowHeight);
	return true;
}

bool BCamera::Frame()
{
	bCameraChange = false;
	// 위
	if (g_InputData.bWKey)
	{
		MoveLook((g_fSecPerFrame * m_SpeedUp));
		bCameraChange = true;
	}
	// 아래
	if (g_InputData.bSKey)
	{
		MoveLook((g_fSecPerFrame * m_SpeedUp) * -1.0f);
		bCameraChange = true;
	}
	// 왼쪽
	if (g_InputData.bAKey)
	{
		MoveSide((g_fSecPerFrame * m_SpeedUp) * -1.0f);
		bCameraChange = true;
	}
	// 오른쪽
	if (g_InputData.bDKey)
	{
		MoveSide(g_fSecPerFrame * m_SpeedUp);
		bCameraChange = true;
	}
	// 위
	if (g_InputData.bQKey)
	{
		MoveUp(10.0f * (g_fSecPerFrame)* m_SpeedUp);
		bCameraChange = true;
	}
	// 아래
	if (g_InputData.bEKey)
	{
		MoveUp(10.0f * (g_fSecPerFrame) * -1.0f * m_SpeedUp);
		bCameraChange = true;
	}
	/*카메라 이동가속도*/
	if (g_InputData.bSpace)	m_SpeedUp += g_fSecPerFrame * 10.0f;
	else					m_SpeedUp -= g_fSecPerFrame * 10.0f;
	// 최소값으로 고정
	if (m_SpeedUp < 1.0f) m_SpeedUp = 1.0f;

	if (I_Input.m_DIMouseState.rgbButtons[1] & 0x80)
	{
		m_fPitch += XMConvertToRadians(I_Input.m_DIMouseState.lY) * 0.1f;
		m_fYaw += XMConvertToRadians(I_Input.m_DIMouseState.lX) * 0.1f;
		bCameraChange = true;
	}
	UpdateQuaternion();
	return true;
}

bool BCamera::Render()
{
	return true;
}
bool BCamera::Release()
{
	return true;
}

bool BCamera::SetProjMatrix(int iWidth, int iHeight, float fFov, float fNear, float fFar)
{
	float fAspect = iWidth / (float)iHeight;
	D3DXMatrixPerspectiveFovLH(&m_matProj, fFov, fAspect, fNear, fFar);
	return true;
}
bool BCamera::SetViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp)
{
	m_vPos = vPos;
	m_vTarget = vTarget;
	m_vUp = vUp;

	D3DXMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &m_vUp);
	TMatrix InvmatView;
	D3DXMatrixInverse(&InvmatView, NULL, &m_matView);
	TVector3* pZBasis = (TVector3*)&InvmatView._31;
	m_fYaw = atan2f(pZBasis->x, pZBasis->z);
	float fLen = sqrtf(pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x);
	m_fPitch = -atan2f(pZBasis->y, fLen);

	UpdateVector();
	return true;
}
bool BCamera::UpdateVector()
{
	m_vSide.x = m_matView._11;
	m_vSide.y = m_matView._21;
	m_vSide.z = m_matView._31;

	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;

	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;

	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vSide, &m_vSide);
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	return true;
}
bool BCamera::UpdateQuaternion()
{
	TQuaternion qRotation;
	// x축 행렬과 y축 행렬이 결합되어 새로운 행렬을 생성
	D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fYaw, m_fPitch, m_fRoll);
	TMatrix matRotation;
	ZeroMemory(&matRotation, sizeof(matRotation));
	D3DXMatrixRotationQuaternion(&matRotation, &qRotation);
	// 사원수를 행렬로 변환해주는 함수 ( 상호 변환 가능 )
	// 이는 오브젝트 행렬이므로 카메라 행렬로의 변환이 필요 ( 역행렬 )
	D3DXMatrixInverse(&m_matView, NULL, &matRotation);
	// 이것을 뷰행렬로 이용
	// 회전행렬을 만들었으므로 이동행렬을 추가
	UpdateVector();

	m_matView._41 = -(D3DXVec3Dot(&m_vPos, &m_vSide));
	m_matView._42 = -(D3DXVec3Dot(&m_vPos, &m_vUp));
	m_matView._43 = -(D3DXVec3Dot(&m_vPos, &m_vLook));
	return true;
}

void BCamera::MoveLook(float fValue)
{
	m_vPos += m_vLook * fValue;
}
void BCamera::MoveSide(float fValue)
{
	m_vPos += m_vSide * fValue;
}
void BCamera::MoveUp(float fValue)
{
	m_vPos += m_vUp * fValue;
}

BCamera::BCamera()
{
	m_fPitch = 0.0f;
	m_fYaw = 0.0f;
	m_fRoll = 0.0f;
	m_SpeedUp = 0.0f;
	m_pHeroPos = NULL;
}


BCamera::~BCamera()
{
}
   