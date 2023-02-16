
////#include "stdafx.h"


#include "LALoadObject.h"

#include "CoreInputLink.h"
#include "CoreFuncLink.h"
#include "CoreStandardLink.h"

#include "LABoneObject.h"
#include "LASkinObject.h"
#include "LAResourceManager.h"




int LALoadObject::GetCurrentAnimationTick(SceneData* SceneData, int& UpdateTargetTick)
{
	m_fElapseFrame += g_fSecPerFrame * SceneData->iFrameSpeed;

	if ((int)m_fElapseFrame >= SceneData->iLastFrame)
	{
		m_fElapseFrame = SceneData->iFirstFrame;
	}
	m_iCurFrame = (int)m_fElapseFrame;
	UpdateTargetTick = m_iCurFrame * SceneData->iTickSecPerFrame;

	return UpdateTargetTick;
}

void LALoadObject::SetAnimationTickData(float fElapse, int iCurrentFrame, int iCurrentTickFrame)
{
	/*if (fElapse != -1)
		m_fElapseFrame = fElapse;

	if (iCurrentFrame != -1)
		m_iCurFrame = iCurrentFrame;

	if (iCurrentTickFrame != -1)
		m_fTickFrame = iCurrentTickFrame;*/
}

void LALoadObject::SetFrame(int iSetFrame)
{
	int iquotient = (int)m_fElapseFrame;
	float remainder = m_fElapseFrame - iquotient;
	m_fElapseFrame = (float)iSetFrame;
	m_fElapseFrame += remainder;
}

void LALoadObject::ResetTestSectionAnimationTick()
{
	ZeroMemory(&m_TestSectionAnimationTick, sizeof(SectionPlay));
}

void LALoadObject::PlaySectionAnimation(int iFirstFrame, int iLastFrame, bool bReplay)
{
	m_TestSectionAnimationTick.ifirst = iFirstFrame;
	m_TestSectionAnimationTick.ilast = iLastFrame;
	m_TestSectionAnimationTick.bReplay = bReplay;
	m_TestSectionAnimationTick.fElpaseTime = (float)iFirstFrame;

	m_bTestSectionAnimationPlay = true;
	m_bAnimationPlay = true;
}

LASkinObject* LALoadObject::GetSkinObject(std::wstring ObjectName)
{
	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(0);
	if (pResourceData)
	{

		for (int iCnt = 0; iCnt < (int)pResourceData->m_SkinObjectList.size(); iCnt++)
		{
			if (ObjectName.compare(pResourceData->m_SkinObjectList[iCnt]->m_MeshData->wsNodeName) == 0)
			{
				return pResourceData->m_SkinObjectList[iCnt];
			}
		}

	}

	return NULL;
}

LABoneObject* LALoadObject::GetMatrixObject(std::wstring ObjectName)
{
	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(0);
	if (pResourceData)
	{
		for (int iCnt = 0; iCnt < (int)pResourceData->m_BoneObjectList.size(); iCnt++)
		{
			if (ObjectName.compare(pResourceData->m_BoneObjectList[iCnt]->m_MeshData->wsNodeName) == 0)
			{
				return pResourceData->m_BoneObjectList[iCnt];
			}
		}
		
	}

	return NULL;
}



bool LALoadObject::DeleteSaveObjectData(std::wstring wsDataName)
{
	for (m_SaveObjecDatatItor = m_SaveAnimationDataList.begin(); m_SaveObjecDatatItor != m_SaveAnimationDataList.end(); m_SaveObjecDatatItor++)
	{
		if (wsDataName.compare(m_SaveObjecDatatItor->SaveFileName) == 0)
		{
			m_SaveAnimationDataList.erase(m_SaveObjecDatatItor);

			return true;
		}
	}

	return false;
}

SaveAnimationDataInfo* LALoadObject::FindSaveObjectData(std::wstring wsDataName)
{
	for (m_SaveObjecDatatItor = m_SaveAnimationDataList.begin(); m_SaveObjecDatatItor != m_SaveAnimationDataList.end(); m_SaveObjecDatatItor++)
	{
		if (wsDataName.compare(m_SaveObjecDatatItor->SaveFileName) == 0)
		{
			return &(*m_SaveObjecDatatItor);
		}
	}

	return NULL;
}

void LALoadObject::SetDirectionLight(TVector4 vCameraPos, TVector4 vLigthPos, TVector4 vLightDirection)
{
	if (m_NewLoadObject)
	{
		m_NewLoadObject->SetDirectionLight(vCameraPos, vLigthPos, vLightDirection);
	}
}

void LALoadObject::AddAnimationData(SaveAnimationDataInfo& data)
{
	data.iFrameSpeed = m_SelectSceneData.iFrameSpeed;
	data.iTickSecPerFrame = m_SelectSceneData.iTickSecPerFrame;

	m_SaveAnimationDataList.push_back(data);
	data.iAnimationNumber = m_SaveAnimationDataList.size() - 1;
}

//controlBox Animation buttom
void LALoadObject::SetAnimationPlay(bool bPlayStop)
{
	if (m_bTestSectionAnimationPlay == true)
	{
		SetFrame(m_TestSectionAnimationTick.iCurrentframe);
		ResetTestSectionAnimationTick();
		m_bTestSectionAnimationPlay = false;
	}

	m_bAnimationPlay = bPlayStop;
}

int LALoadObject::GetCrrentTickFrame()
{
	return 0;// m_iCurrentTick / m_LoadObjectSceneDataList.iTickSecPerFrame;
}

void LALoadObject::ControlBoxUpdateAniTick(int& iTargetTick)
{
	m_bControlTickSet = true;
	//m_iCurrentTick = iTargetTick * m_LoadObjectSceneDataList.iTickSecPerFrame;
}

void LALoadObject::SetWriteSwapChain(IDXGISwapChain* pSawpChain)
{
	//m_CurrentTickWrite.Init(pSawpChain);
	//m_CurrentTickWrite.SetRightPosition(DWRITE_TEXT_ALIGNMENT_TRAILING);
}

bool LALoadObject::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return true;
}

//void LALoadObject::SetCBBoneData()
//{
//	for (int iBoneObj = 0; iBoneObj < (int)m_BoneObjectList.size(); iBoneObj++)
//	{
//		TMatrixTranspose(&m_cbMatrixListData.g_mConstBoneWorld[iBoneObj], &m_BoneObjectList[iBoneObj]->m_matInversAni);
//	}
//}

bool LALoadObject::PostRender()
{
	/*TCHAR CrrentTick[255] = { 0 };
	int ConvertTick;

	if (m_bTestSectionAnimationPlay)
	{
		ConvertTick = m_TestSectionAnimationTick.iCurrentTick / m_LoadObjectSceneDataList.iTickSecPerFrame;
	}
	else
	{
		ConvertTick = m_iCurrentTick / m_LoadObjectSceneDataList.iTickSecPerFrame;
	}

	_itow_s(ConvertTick, CrrentTick, 10);*/
	//m_CurrentTickWrite.DrawTextW(g_rtClient, CrrentTick, D2D1::ColorF(1, 0, 0, 1));

	return true;
}

bool LALoadObject::RenderShadow()
{
	if (m_NewLoadObject != NULL)
	{
		m_NewLoadObject->RenderShadow();
	}

	return true;
}

bool LALoadObject::Frame()
{

#ifdef TOOL_MODE

	if (I_Input.KeyCheck(DIK_O) == (DWORD)keyState::KEY_UP)
	{
		m_bBipedRender = !m_bBipedRender;

		if (m_NewLoadObject)
		{
			m_NewLoadObject->m_bBipedRender = m_bBipedRender;
		}

	}

	if (I_Input.KeyCheck(DIK_P) == (DWORD)keyState::KEY_UP)
	{
		m_bSkinRender = !m_bSkinRender;

		if (m_NewLoadObject)
		{
			m_NewLoadObject->m_bSkinRender = m_bSkinRender;
		}
	}

#else

#endif
	//애니메이션 플레이 여부
	if (m_bAnimationPlay == true)
	{
		//애니메이션 테스트 구간 여부
		if (m_bTestSectionAnimationPlay == true)
		{
			m_TestSectionAnimationTick.fElpaseTime += g_fSecPerFrame * m_SelectSceneData.iFrameSpeed;

			if ((int)m_TestSectionAnimationTick.fElpaseTime >= m_TestSectionAnimationTick.ilast)
			{
				if (m_TestSectionAnimationTick.bReplay == true)
				{
					m_TestSectionAnimationTick.fElpaseTime = m_TestSectionAnimationTick.ifirst;
				}
				else
				{
					m_TestSectionAnimationTick.fElpaseTime = m_TestSectionAnimationTick.ifirst;
					m_bAnimationPlay = false;
				}
			}

			m_TestSectionAnimationTick.iCurrentframe = (int)m_TestSectionAnimationTick.fElpaseTime;
			m_TestSectionAnimationTick.iCurrentTick = m_TestSectionAnimationTick.iCurrentframe * m_SelectSceneData.iTickSecPerFrame;

			//본 오브젝트 프레임
			if (m_NewLoadObject != NULL)
			{
				m_NewLoadObject->m_iMainCurrentTick = m_TestSectionAnimationTick.iCurrentTick;
				m_NewLoadObject->Frame();
			}

			return true;
		}
		else
		{
			GetCurrentAnimationTick(&m_SelectSceneData, m_iCurrentTick);

			//본 오브젝트 프레임
			if (m_NewLoadObject != NULL)
			{
				m_NewLoadObject->m_iMainCurrentTick = m_iCurrentTick;
				m_NewLoadObject->Frame();
			}
			return true;
		}
	}

	//본 오브젝트 프레임
	if (m_NewLoadObject != NULL)
	{
		m_NewLoadObject->m_iMainCurrentTick = m_iCurrentTick;
		m_NewLoadObject->Frame();
	}

	
	
	return true;
}

void LALoadObject::SetMatrix(TMatrix* pWorldMatrix, TMatrix* pViewMatrix, TMatrix* pProjectionMatrix)
{
	//본 오브젝트 프레임
	if (m_NewLoadObject != NULL)
	{
		m_NewLoadObject->SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);
	}
}

void LALoadObject::SetCurrentMotion(int iAniIndex)
{
	m_NewLoadObject->m_iCurrentMotion = iAniIndex;
}

//bool LALoadObject::BoneObjCreate(CrateObjectDest* pObjectDest)
//{
//	if (pObjectDest == NULL)
//	{
//		pObjectDest = &m_BoneShaderDest;
//	}
//
//	for (int iBoneObj = 0; iBoneObj < (int)m_BoneObjectList.size(); iBoneObj++)
//	{
//		m_BoneObjectList[iBoneObj]->Create(*pObjectDest);
//	}
//
//	return true;
//}
//
//bool LALoadObject::SkinObjCreate(CrateObjectDest* pObjectDest)
//{
//	if (pObjectDest == NULL)
//	{
//		pObjectDest = &m_SkinShaderDest;
//	}
//
//	for (int iSkinObj = 0; iSkinObj < (int)m_SkinObjectList.size(); iSkinObj++)
//	{
//		m_SkinObjectList[iSkinObj]->Create(*pObjectDest);
//	}
//
//	return true;
//}

bool LALoadObject::Render()
{
	if (m_NewLoadObject != NULL)
	{
		m_NewLoadObject->Render();
	}

	return true;
}

HRESULT LALoadObject::DeleteResource()
{
	//m_CurrentTickWrite.DeleteResource();

	return S_OK;
}

HRESULT LALoadObject::CreateResource()
{
	//m_CurrentTickWrite.CreateResource();

	return S_OK;
}


bool LALoadObject::Release()
{
	if (m_NewLoadObject != NULL)
	{
		m_NewLoadObject->Release();
	}

	return true;
}


LALoadObject::~LALoadObject()
{
}


