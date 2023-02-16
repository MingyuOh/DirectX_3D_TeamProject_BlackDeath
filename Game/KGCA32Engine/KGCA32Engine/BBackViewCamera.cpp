#include "BBackViewCamera.h"
#include "BInput.h"
#include "BTimer.h"

/*오브젝트의 위치벡터, 방향벡터를 기준으로 이동*/
bool BBackViewCamera::Init(TVector3 vTarget)
{
	m_fPrevRadius = D3DXVec3Length(&(m_vPos - vTarget));
	SetViewMatrix(m_vPos, vTarget, m_vUp);
	SetProjMatrix(g_iWindowWidth, g_iWindowHeight);
	return true;
}

void BBackViewCamera::SetHeroPosition(TVector3* pHeroPos)
{
	m_pHeroPos = pHeroPos;
}

bool BBackViewCamera::SetViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp)
{
	m_vDefaultEye = m_vPos = vPos;
	m_vDefaultLookAt = m_vTarget = vTarget;
	D3DXMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);

	TQuaternion qRotation;
	TVector3 vTrans, vScale, vAxis;
	TMatrix mInvView;
	TQuaternion q;
	float fAngle;

	/*행렬을 스케일, 회전, 이동으로 분해한다.*/
	if (SUCCEEDED(D3DXMatrixDecompose(&vScale, &qRotation, &vTrans, &m_matView)))
	{
		D3DXQuaternionNormalize(&qRotation, &qRotation);
		D3DXQuaternionToAxisAngle(&qRotation, &vAxis, &fAngle);
		// 반드시 정규화 해야 한다.
		D3DXVec3Normalize(&vAxis, &vAxis);

		q.x = sin(fAngle / 2) * vAxis.x;
		q.y = sin(fAngle / 2) * vAxis.y;
		q.z = sin(fAngle / 2) * vAxis.z;
		q.w = cos(fAngle / 2);
	}
	D3DXMatrixInverse(&mInvView, NULL, &m_matView);
	// The axis basis vectors and camera position are stored inside the 
	// position matrix in the 4 rows of the camera's world matrix.
	// To figure out the yaw/pitch of the camera, we just need the Z basis vector
	TVector3* pZBasis = (TVector3*)&mInvView._31;

	m_fYaw = atan2f(pZBasis->x, pZBasis->z);
	float fLen = sqrtf(pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x);
	m_fPitch = -atan2f(pZBasis->y, fLen);
	return 	UpdateVector();
}

bool BBackViewCamera::UpdateVector()
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

bool BBackViewCamera::Frame()
{
	SetProjMatrix(g_iWindowWidth, g_iWindowHeight);

	if (m_pHeroPos)
	{
		m_vTarget = *m_pHeroPos;
		m_vTarget.y += 2;
	}

	//// 다가감
	//if (g_InputData.bWKey)
	//	m_fPrevRadius -= (g_fSecPerFrame + m_SpeedUp);
	//// 멀어짐
	//if (g_InputData.bSKey)
	//	m_fPrevRadius += (g_fSecPerFrame + m_SpeedUp);
	m_fPrevRadius += g_fSecPerFrame * g_InputData.iMouseValue[2];

	// 회전
	if (I_Input.m_DIMouseState.rgbButtons[1] & 0x80)
	{
		m_fYaw += I_Input.m_DIMouseState.lX * g_fSecPerFrame;
		m_fPitch += I_Input.m_DIMouseState.lY * g_fSecPerFrame;
	}

	UpdateQuaternion();
	return true;
}

bool BBackViewCamera::UpdateQuaternion()
{
	TMatrix matRotation;
	// x축 행렬과 y축 행렬이 결합되어 새로운 행렬을 생성
	D3DXMatrixRotationYawPitchRoll(&matRotation, m_fYaw, m_fPitch, m_fRoll);
	TVector3 vWorldUp, vWorldLook;
	TVector3 vLocalUp = TVector3(0, 1, 0);
	TVector3 vLocalLook = TVector3(0, 0, 1);
	D3DXVec3TransformCoord(&vWorldUp, &vLocalUp, &matRotation);
	D3DXVec3TransformCoord(&vWorldLook, &vLocalLook, &matRotation);
	// 카메라 위치를 디렉션벡터* 반지름크기 만큼 이동시킨다.	
	m_vPos = m_vTarget - (vWorldLook * m_fPrevRadius);
	return SetViewMatrix(m_vPos, m_vTarget, vWorldUp);

	return true;
}

BBackViewCamera::BBackViewCamera()
{
	m_vUp = TVector3(0, 1, 0);
	m_vHeroLook = TVector3(0, 0, 1);
	m_vHeroUp = TVector3(0, 1, 0);
	m_vHeroSide = TVector3(1, 0, 0);
	m_vPos = TVector3(0.0f, 50.0f, -50.0f);
	m_fYaw = 0.0f;
	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
}


BBackViewCamera::~BBackViewCamera()
{
}
