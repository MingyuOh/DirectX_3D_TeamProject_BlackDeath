#include "BFPSCamera.h"
#include "BInput.h"
#include "BTimer.h"

bool BFPSCamera::Frame()
{
	// �ȱ�
	Walk();

	// �޸���
	Run();

	// ���콺 ȸ�� ��
	m_fYaw += I_Input.m_DIMouseState.lX * g_fSecPerFrame * 0.4f;
	m_fPitch += I_Input.m_DIMouseState.lY * g_fSecPerFrame * 0.4f;

	// ���콺 y�� ����
	if (m_fPitch > 0.25f)
		m_fPitch = 0.25f;
	if (m_fPitch < -1.0f)
		m_fPitch = -1.0f;

	UpdateQuaternion();
	return true;
}

void BFPSCamera::SetHeroPosition(TVector3* pHeroPos)
{
	m_pHeroPos = pHeroPos;
}

void BFPSCamera::SetCameraPosY(float fY)
{
	m_vPos.y = 1.8f + fY;
}

void BFPSCamera::SetMapNormal(TVector3 vMapNormal)
{
	m_vMapNormal = vMapNormal;
}

bool BFPSCamera::UpdateQuaternion()
{
	TQuaternion qRotation;
	D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fYaw, m_fPitch, m_fRoll);
	TMatrix matRotation;
	ZeroMemory(&matRotation, sizeof(matRotation));
	D3DXMatrixRotationQuaternion(&matRotation, &qRotation);
	D3DXMatrixInverse(&m_matView, NULL, &matRotation);
	UpdateVector();

	m_matView._41 = -(D3DXVec3Dot(&m_vPos, &m_vSide));
	m_matView._42 = -(D3DXVec3Dot(&m_vPos, &m_vUp));
	m_matView._43 = -(D3DXVec3Dot(&m_vPos, &m_vLook));
	return true;
}

bool BFPSCamera::SetViewMatrix(TVector3 vPos, TVector3 vTarget, TVector3 vUp)
{
	m_vPos = vPos;
	m_vTarget = vTarget;
	D3DXMatrixLookAtLH(&m_matView, &m_vPos, &m_vTarget, &vUp);

	TMatrix matInvView;
	D3DXMatrixInverse(&matInvView, NULL, &m_matView);
	
	// yaw/pitch ���� �˾Ƴ����� ī�޶��� z�� ���� �ʿ�
	TVector3* pZBasis = (TVector3*)&matInvView._31;
	m_fYaw = atan2f(pZBasis->x, pZBasis->z);
	float fLen = sqrtf(pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x);
	m_fPitch = -atan2f(pZBasis->y, fLen);
	
	UpdateVector();
	return true;
}

//--------------------------------------------------------------------------------------
// FPS ī�޶� �ȱ�
//--------------------------------------------------------------------------------------
void BFPSCamera::Walk()
{
	if(m_bCheckMap == false)
		m_vBeforeCameraPos = m_vPos;

	float fCameraSpeed = m_SpeedUp * g_fSecPerFrame;
	int iKeyCountCheck = 0;

	// ��
	if (g_InputData.bWKey)
	{
		MoveLook(fCameraSpeed);
		iKeyCountCheck++;
	}
	// ��
	if (g_InputData.bSKey)
	{
		MoveLook(fCameraSpeed * -1.0f);
		iKeyCountCheck++;
	}
	// ����
	if (g_InputData.bAKey)
	{
		MoveSide(fCameraSpeed * -1.0f);
		iKeyCountCheck++;
	}
	// ������
	if (g_InputData.bDKey)
	{
		MoveSide(fCameraSpeed);
		iKeyCountCheck++;
	}

	// ���� �̵� Ű ���� ���� �ӵ� ó��
	// (���� �� �ִ� �ִ� �̵� Ű�� �� = 2��)
	// �밢������ �̵� ��, ������ �������� �����Ͽ� �̵� ���̸� ���
	if (iKeyCountCheck >= 2)
	{
		fCameraSpeed = sqrt(pow(m_SpeedUp, 2) + pow(m_SpeedUp, 2));
		fCameraSpeed = fCameraSpeed * g_fSecPerFrame;
	}

	// ���� �浹ó��
	ProcessingSlip(fCameraSpeed);
}

//--------------------------------------------------------------------------------------
// FPS ī�޶� �޸���
//--------------------------------------------------------------------------------------
void BFPSCamera::Run()
{
	if (m_bCheckMap == false)
		m_vBeforeCameraPos = m_vPos;

	float fCameraSpeed = CAMERA_RUN_SPEED * m_SpeedUp * g_fSecPerFrame;
	int iKeyCountCheck = 0;

	// ���δ޸���
	if (I_Input.KeyCheck(DIK_LSHIFT) && g_InputData.bWKey)
	{
		MoveLook(fCameraSpeed);
		iKeyCountCheck++;
	}

	// �ڷ� �޸���
	if (I_Input.KeyCheck(DIK_LSHIFT) && g_InputData.bSKey)
	{
		MoveLook(fCameraSpeed * -1.0f);
		iKeyCountCheck++;
	}

	// �������� �޸���
	if (I_Input.KeyCheck(DIK_LSHIFT) && g_InputData.bAKey)
	{
		MoveSide(fCameraSpeed * -1.0f);
		iKeyCountCheck++;
	}

	// ���������� �޸���
	if (I_Input.KeyCheck(DIK_LSHIFT) && g_InputData.bDKey)
	{
		MoveSide(fCameraSpeed);
		iKeyCountCheck++;
	}

	// ���� �̵� Ű ���� ���� �ӵ� ó��
	// (���� �� �ִ� �ִ� �̵� Ű�� �� = 2��)
	// �밢������ �̵� ��, ������ �������� �����Ͽ� �̵� ���̸� ���
	if (iKeyCountCheck >= 2)
	{
		fCameraSpeed = sqrt(pow(CAMERA_RUN_SPEED * m_SpeedUp, 2) + pow(CAMERA_RUN_SPEED * m_SpeedUp, 2));
		fCameraSpeed = fCameraSpeed * g_fSecPerFrame;
	}

	// ���� �浹ó��
	ProcessingSlip(fCameraSpeed);
}

//--------------------------------------------------------------------------------------
// ����� �ٿ���ڽ� ���� �������� �Լ�
//--------------------------------------------------------------------------------------
void BFPSCamera::SetHeroBoundingBoxInfo(BBoundingBox heroBoundingBox)
{
	m_CameraBoundingBox = heroBoundingBox;
}

//--------------------------------------------------------------------------------------
// FPS ī�޶� �̲��� ó��
//--------------------------------------------------------------------------------------
void BFPSCamera::ProcessingSlip(float fSpeed)
{
	if (m_bCheckMap == true)
	{
		// FPS ī�޶� ���� �̲���
		m_CameraBoundingBox.m_Collision.SlipDetection(m_vPos, m_vBeforeCameraPos, fSpeed);
	}
}

//--------------------------------------------------------------------------------------
// FPS ī�޶�� ������Ʈ �浹ó��
//--------------------------------------------------------------------------------------
void BFPSCamera::CameraToObjectCollision(BBoundingBox& objectBox)
{
	// �浹üũ
	if (m_CameraBoundingBox.m_Collision.CheckSphereInSphere(
		m_CameraBoundingBox.m_vCenter,
		objectBox.m_vCenter,
		m_CameraBoundingBox.m_fRadius,
		objectBox.m_fRadius) == true)
	{
		// ī�޶�� ������Ʈ ������ ���� �浹 ���� ���
		TVector3 vBack = m_CameraBoundingBox.m_Collision.SphereInSphereOppositeDir(
			m_CameraBoundingBox.m_vCenter,
			objectBox.m_vCenter,
			m_CameraBoundingBox.m_fRadius,
			objectBox.m_fRadius);

		// ī�޶� �ӵ���ŭ �ڷ� �̵�
		m_vPos = m_vPos + (vBack * m_SpeedUp * g_fSecPerFrame);

		UpdateQuaternion();
	}
}

//--------------------------------------------------------------------------------------
// FPS ī�޶� ������ �ߵ��� �ݵ�ó�� �Լ�
//--------------------------------------------------------------------------------------
void BFPSCamera::CameraRebound()
{
	m_fPitch -= CAMERA_REBOUND_VALUE * g_fSecPerFrame;
}

BFPSCamera::BFPSCamera()
{
	m_vUp = TVector3(0, 1, 0);
	m_vHeroLook = TVector3(0, 0, 1);
	m_vHeroUp = TVector3(0, 1, 0);
	m_vHeroSide = TVector3(1, 0, 0);
	m_vPos = TVector3(0.0f, 50.0f, -50.0f);
	m_SpeedUp = 20.0f;
	m_fYaw = 0.0f;
	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_bCheckMap = true;
}


BFPSCamera::~BFPSCamera()
{
}
