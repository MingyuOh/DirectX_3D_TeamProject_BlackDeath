
//#include "stdafx.h"

#include "LATopObject.h"

#include "LAResourceManager.h"
#include "LALoadObjectManager.h"



bool LATopObject::CheckPositionTick(int iPercentage, bool bisReCheck)
{
	LAResource* pData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);

	if (pData->m_AnimationList.size())
	{
		SceneData& rData = pData->m_AnimationList[m_iCurrentMotion];

		int TotalTick = rData.iLastFrame - rData.iFirstFrame;
		int DiviceTick = TotalTick * iPercentage / 100;

		if (bisReCheck)
		{
			if (((int)m_fElapseFrame == DiviceTick))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (((int)m_fElapseFrame == DiviceTick) && (m_bisCheckTick == false))
			{
				m_bisCheckTick = true;

				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

bool LATopObject::CheckRifleIsShot()
{
	return m_bIsRifleShot;
}

void LATopObject::SetCBBoneData()
{
	//main
	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pResourceData)
	{
		for (int iBoneObj = 0; iBoneObj < (int)pResourceData->m_BoneObjectList.size(); iBoneObj++)
		{
			D3DXMatrixTranspose(&m_cbMatrixListData.g_mConstBoneWorld[iBoneObj], &pResourceData->m_BoneObjectList[iBoneObj]->m_matInversAni);
		}
	}

	//sub
}

void LATopObject::SetObjectName(std::wstring wsName)
{
	m_wsObjectName = wsName;
}

void LATopObject::SetMotionIdle(int isetMotion)
{
	m_iMotionIdleNumber = isetMotion;
	m_iCurrentMotion = isetMotion;
}

void LATopObject::SetCurrentMotion(int isetMotion)
{
	m_iCurrentMotion = isetMotion;
}

void LATopObject::SetType(E_LoadObjectType eSetType)
{
	m_ObjectType = eSetType;
}

bool LATopObject::SetMainResource(int iResourceindex)
{
	m_MainResourceNumber = iResourceindex;
	//if (pData == NULL)
	//	return false;

	//m_pMainResource = pData;

	////KeySet
	//for (int iCnt = 0; iCnt < (int)m_pMainResource->m_AnimationList.size(); iCnt++)
	//{
	//	MotionKeyInfo keyInfo;
	//	keyInfo.iMotionNumber = iCnt;

	//	m_AnimationKeyStateList.push_back(keyInfo);
	//}

	return true;
}

void LATopObject::SetPosition(float fx, float fy, float fz)
{
	//m_vObjectBeforePosition = m_vObjectPosition;

	m_vObjectPosition.x = fx;
	m_vObjectPosition.y = fy;
	m_vObjectPosition.z = fz;
}

void LATopObject::SetHeight(float fHeight)
{
	m_vObjectPosition.y = fHeight;
}

void LATopObject::SetCurrentTick(bool check)
{
	m_bSetParentTick = check;

	if (m_bSetParentTick)
	{
		LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
		if (pResourceData)
		{
			for (int iBoneObj = 0; iBoneObj < (int)pResourceData->m_BoneObjectList.size(); iBoneObj++)
			{
				pResourceData->m_BoneObjectList[iBoneObj]->SetCrrentTick(m_bSetParentTick);
			}

			for (int iSkinObj = 0; iSkinObj < (int)pResourceData->m_SkinObjectList.size(); iSkinObj++)
			{
				pResourceData->m_SkinObjectList[iSkinObj]->SetCrrentTick(m_bSetParentTick);
			}
		}
	}
}

void LATopObject::SetKeyState(DWORD dwKeyValue, int imotionIndex)
{
	if (((int)m_AnimationKeyStateList.size() - 1) < imotionIndex)
	{
		MotionKeyInfo addkey;
		addkey.dwkey = dwKeyValue;
		addkey.iMotionNumber = imotionIndex;
		m_MoveKeyStateList.push_back(addkey);

		return;
	}

	m_AnimationKeyStateList[imotionIndex].dwkey = dwKeyValue;
}

void LATopObject::SetAddAnimationSpeed(float fSpeed)
{
	m_fAddAnimationSpeed = fSpeed;
}

//void LATopObject::SetMoveSpeed(float idata)
//{
//	m_iMoveSpeed = idata;
//}

void LATopObject::SetScale(float fbaseHeightSize, float x, float y, float z)
{
	TMatrix baseScale;
	TMatrix secondScale;
	float maxHeight = 0.0f;

	//최대사이즈 선택
	if (maxHeight < m_vAllSkinMax.x)	maxHeight = m_vAllSkinMax.x;
	if (maxHeight < m_vAllSkinMax.y)	maxHeight = m_vAllSkinMax.y;
	if (maxHeight < m_vAllSkinMax.z)	maxHeight = m_vAllSkinMax.z;

	//1차 기본비율값 스케일
	float fScale = fbaseHeightSize / maxHeight;
	D3DXMatrixScaling(&baseScale, fScale, fScale, fScale);

	//2차 기본비율값 스케일
	D3DXMatrixScaling(&secondScale, x, y, z);

	//최종스케일값 설정
	m_matObjectScale = baseScale * secondScale;

	UpdateMinMax();
}

void LATopObject::SetObjectWorldMatrixToResource(TMatrix* pMatObjectWorld)
{
	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pResourceData)
	{
		//스키닝이면 
		if (pResourceData->m_bIsSkining)
		{
			//본이 있으면
			if ((int)pResourceData->m_BoneObjectList.size())
			{
				if (pMatObjectWorld == NULL)
				{
					pResourceData->m_BoneObjectList[0]->m_pmatCharacter = &m_matObjectWorld;
				}
				else
				{
					pResourceData->m_BoneObjectList[0]->m_pmatCharacter = pMatObjectWorld;
				}

				//카메라 포지션있으면 캐릭터 행렬 저장
				if ((int)pResourceData->m_BoneObjectList.size())
				{
					for (int iCnt = 0; iCnt < (int)pResourceData->m_BoneObjectList.size(); iCnt++)
					{
						if (pResourceData->m_BoneObjectList[iCnt]->m_MeshData->wsNodeName.compare(L"CameraPos01") == 0)
						{
							if (pMatObjectWorld == NULL)
							{
								pResourceData->m_BoneObjectList[iCnt]->m_pmatCharacter = &m_matObjectWorld;
							}
							else
							{
								pResourceData->m_BoneObjectList[iCnt]->m_pmatCharacter = pMatObjectWorld;
							}
							break;
						}
					}
				}

			}
			else
			{
				if ((int)pResourceData->m_SkinObjectList.size())
				{
					for (int iSkinObj = 0; iSkinObj < (int)pResourceData->m_SkinObjectList.size(); iSkinObj++)
					{
						if (pMatObjectWorld == NULL)
						{
							pResourceData->m_SkinObjectList[iSkinObj]->m_pmatCharacter = &m_matObjectWorld;
						}
						else
						{
							pResourceData->m_SkinObjectList[iSkinObj]->m_pmatCharacter = pMatObjectWorld;
						}
					}
				}
			}

			/*if ((int)pResourceData->m_SkinObjectList.size())
			{
				if (pMatObjectWorld == NULL)
				{
					pResourceData->m_SkinObjectList[0]->m_pmatCharacter = &m_matObjectWorld;
				}
				else
				{
					pResourceData->m_SkinObjectList[0]->m_pmatCharacter = pMatObjectWorld;
				}
			}*/

		}
		else
		{
			//본에 넣고 스킨에도 넣고
			if ((int)pResourceData->m_BoneObjectList.size())
			{
				for (int iCnt = 0; iCnt < (int)pResourceData->m_BoneObjectList.size(); iCnt++)
				{
					if (pMatObjectWorld == NULL)
					{
						pResourceData->m_BoneObjectList[iCnt]->m_pmatCharacter = &m_matObjectWorld;
					}
					else
					{
						pResourceData->m_BoneObjectList[iCnt]->m_pmatCharacter = pMatObjectWorld;
					}
				}
			}

			if ((int)pResourceData->m_SkinObjectList.size())
			{
				for (int iSkinObj = 0; iSkinObj < (int)pResourceData->m_SkinObjectList.size(); iSkinObj++)
				{
					if (pMatObjectWorld == NULL)
					{
						pResourceData->m_SkinObjectList[iSkinObj]->m_pmatCharacter = &m_matObjectWorld;
					}
					else
					{
						pResourceData->m_SkinObjectList[iSkinObj]->m_pmatCharacter = pMatObjectWorld;
					}
				}
			}
		}
		
	}
}

void LATopObject::SetMinMax()
{
	//스킨 리스트의 minmax 값 비교 오브젝트 minmax 설정

	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pResourceData)
	{
		if ((int)pResourceData->m_SkinObjectList.size())
		{
			m_vAllSkinMin = TVector3(999999.9f, 999999.9f, 999999.9f);
			m_vAllSkinMax = TVector3(-999999.9f, -999999.9f, -999999.9f);

			for (int iSkinObj = 0; iSkinObj < (int)pResourceData->m_SkinObjectList.size(); iSkinObj++)
			{
				TVector3 min = pResourceData->m_SkinObjectList[iSkinObj]->m_MeshData->vMin;
				TVector3 max = pResourceData->m_SkinObjectList[iSkinObj]->m_MeshData->vMax;
				/*min.x += m_pMainResource->m_SkinObjectList[iSkinObj]->m_MeshData->matLoadObjectWorld._41;
				min.y += m_pMainResource->m_SkinObjectList[iSkinObj]->m_MeshData->matLoadObjectWorld._42;
				min.z += m_pMainResource->m_SkinObjectList[iSkinObj]->m_MeshData->matLoadObjectWorld._43;

				max.x += m_pMainResource->m_SkinObjectList[iSkinObj]->m_MeshData->matLoadObjectWorld._41;
				max.y += m_pMainResource->m_SkinObjectList[iSkinObj]->m_MeshData->matLoadObjectWorld._42;
				max.z += m_pMainResource->m_SkinObjectList[iSkinObj]->m_MeshData->matLoadObjectWorld._43;*/

				if (m_vAllSkinMin.x > min.x) m_vAllSkinMin.x = min.x;
				if (m_vAllSkinMin.y > min.y) m_vAllSkinMin.y = min.y;
				if (m_vAllSkinMin.z > min.z) m_vAllSkinMin.z = min.z;

				if (m_vAllSkinMax.x < max.x) m_vAllSkinMax.x = max.x;
				if (m_vAllSkinMax.y < max.y) m_vAllSkinMax.y = max.y;
				if (m_vAllSkinMax.z < max.z) m_vAllSkinMax.z = max.z;
			}
		}
	}
}

void LATopObject::SetObjectData(ObjectTableLoadData& objectdata)
{
	m_ObjectData = objectdata;
}

void LATopObject::SetSound(E_LoadTableType eTableType, int iMode)
{
	int SoundSize = m_ObjectData.wsSoundNameList.size();

	for (int iSound = 0; iSound < SoundSize; iSound++)
	{
		int SoundIndex = 0; 

		std::string copyPath = RESOURCE_MGR.m_LAFilePath.m_FileSoundPatch[(int)eTableType];
		std::wstring filename = m_ObjectData.wsSoundNameList[iSound];
		std::string convertTochar;
		convertTochar.assign(filename.begin(), filename.end());
		copyPath += convertTochar;

		std::vector<char> NoConstname(copyPath.begin(), copyPath.end());
		NoConstname.push_back('\0');

#ifdef TOOL_MODE

#else
		SoundIndex  = LA_SOUND2_MGR.Load(&NoConstname[0], iMode);
#endif
		m_SondIndexList.push_back(SoundIndex);
	}
}

void LATopObject::SetSkillData(SkillTableLoadData* rSkillData)
{
	if (rSkillData)
	{
		m_pSkillData = new LASkill;

		/*============================ 스킬 데이터 등록 ================================*/
		m_pSkillData->m_SkillTableLoadData = *rSkillData;

		/*============================ 스킬 사운드 등록 ================================*/
		int SoundSize = m_pSkillData->m_SkillTableLoadData.wsSoundNameList.size();
		for (int iSound = 0; iSound < SoundSize; iSound++)
		{
			int SoundIndex = 0;

			std::string copyPath = RESOURCE_MGR.m_LAFilePath.m_FileSoundPatch[(int)E_LoadTableType::SKILL_HITBOX];
			std::wstring filename = m_pSkillData->m_SkillTableLoadData.wsSoundNameList[iSound];
			std::string convertTochar;
			convertTochar.assign(filename.begin(), filename.end());
			copyPath += convertTochar;

			std::vector<char> NoConstname(copyPath.begin(), copyPath.end());
			NoConstname.push_back('\0');

#ifdef TOOL_MODE

#else
			SoundIndex = LA_SOUND2_MGR.Load(&NoConstname[0], 0);
#endif
			m_pSkillData->m_SkillSoundIndex.push_back(SoundIndex);
		}

	}
}

void LATopObject::SetTileNumber(int iTileNumber)
{
	m_iNumTile = iTileNumber;
}

void LATopObject::SetDirectionLight(TVector4 vCameraPos, TVector4 vLigthPos, TVector4 vLightDirection)
{
	m_vCameraPos = vCameraPos;
	m_vLightPos = vLigthPos;
	m_vLightDirection = vLightDirection;

	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pResourceData)
	{
		if ((int)pResourceData->m_SkinObjectList.size())
		{
			for (int iBoneObj = 0; iBoneObj < (int)pResourceData->m_SkinObjectList.size(); iBoneObj++)
			{
				pResourceData->m_SkinObjectList[iBoneObj]->SetDirectionLight(vCameraPos, vLigthPos, vLightDirection);
				pResourceData->m_SkinObjectList[iBoneObj]->CreateShadowMatrix();
			}
		}
	}

}

std::wstring LATopObject::GetObjectName()
{
	return m_wsObjectName;
}

TVector3 LATopObject::GetPosition()
{
	return m_vObjectPosition;
}

TVector3 LATopObject::GetDirection()
{
	return m_vObjectLook;
}

void LATopObject::GetInstancingData(InstancingDataInfo* pInstancingData, int iInstancingCount)
{
	//main
	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pResourceData)
	{
		//월드행렬 변경
		SetObjectWorldMatrixToResource();

		//월드행렬연결
		if ((int)pResourceData->m_BoneObjectList.size())
		{
			for (int iBoneObj = 0; iBoneObj < (int)pResourceData->m_BoneObjectList.size(); iBoneObj++)
			{
				pResourceData->m_BoneObjectList[iBoneObj]->m_iCurrentMotion = m_iCurrentMotion;
				pResourceData->m_BoneObjectList[iBoneObj]->m_ParentObjectTick = m_iMainCurrentTick;
				pResourceData->m_BoneObjectList[iBoneObj]->Frame();
			}

			int boneIndex = g_iMaxBoneMatrixCount * iInstancingCount;
			//pInstancingData->matBoneAnimationWorld[iInstancingCount].resize(pResourceData->m_BoneObjectList.size());
			for (int iBoneObj = 0; iBoneObj < (int)pResourceData->m_BoneObjectList.size(); iBoneObj++)
			{
				/*========== 애니메이션 행렬 저장 =========*/
				D3DXMatrixTranspose(&pInstancingData->matBoneAnimationWorld[boneIndex + iBoneObj],
										&pResourceData->m_BoneObjectList[iBoneObj]->m_matInversAni);
			}

			if (m_bBipedRender == true)
			{
				for (int iBoneObj = 0; iBoneObj < (int)pResourceData->m_BoneObjectList.size(); iBoneObj++)
				{
					pResourceData->m_BoneObjectList[iBoneObj]->Render();
				}
			}
		}

		/*========== 노말에 사용할 월드 역행렬 저장 =========*/
		TMatrix WorldInverse;
		D3DXMatrixInverse(&WorldInverse, NULL, &m_matObjectWorld);
		pInstancingData->matNormalWorldInverse[iInstancingCount] = WorldInverse;	//노말 
	}

}

void LATopObject::MoveLook(float fValue)
{
	TVector3 vMove;
	vMove = m_vObjectLook * fValue;

	m_vMoveUpdatePosition.x += vMove.x;
	m_vMoveUpdatePosition.y += vMove.y;
	m_vMoveUpdatePosition.z += vMove.z;
}

void LATopObject::MoveSide(float fValue)
{
	TVector3 vMove;
	vMove = m_vObjectSide * fValue;

	m_vMoveUpdatePosition.x += vMove.x;
	m_vMoveUpdatePosition.y += vMove.y;
	m_vMoveUpdatePosition.z += vMove.z;
}

void LATopObject::UpdateWorldmatrix()
{
	m_matObjectWorld = m_matObjectScale * m_matObjectrotation;
	m_matObjectWorld._41 = m_vObjectPosition.x;
	m_matObjectWorld._42 = m_vObjectPosition.y;
	m_matObjectWorld._43 = m_vObjectPosition.z;
}

void LATopObject::UpdateMinMax()
{
	//스케일과 포지션값만 업데이트
	TMatrix matUpdate;
	D3DXMatrixIdentity(&matUpdate);
	matUpdate._11 = m_matObjectScale._11;
	matUpdate._22 = m_matObjectScale._22;
	matUpdate._33 = m_matObjectScale._33;

	ZeroMemory(&m_vUpdateMin, sizeof(TVector3));
	ZeroMemory(&m_vUpdateMax, sizeof(TVector3));

	D3DXVec3TransformCoord(&m_vUpdateMin, &m_vAllSkinMin, &matUpdate);
	D3DXVec3TransformCoord(&m_vUpdateMax, &m_vAllSkinMax, &matUpdate);

	m_vUpdateMin += m_vObjectPosition;
	m_vUpdateMax += m_vObjectPosition;

#ifdef TOOL_MODE

#else

	if (m_bDrawCollitionLine)
	{
		m_vCollitionBox[0] = TVector3(m_vUpdateMin.x, m_vUpdateMax.y, m_vUpdateMin.z);
		m_vCollitionBox[1] = TVector3(m_vUpdateMax.x, m_vUpdateMax.y, m_vUpdateMin.z);
		m_vCollitionBox[2] = TVector3(m_vUpdateMax.x, m_vUpdateMin.y, m_vUpdateMin.z);
		m_vCollitionBox[3] = TVector3(m_vUpdateMin.x, m_vUpdateMin.y, m_vUpdateMin.z);
		m_vCollitionBox[4] = TVector3(m_vUpdateMin.x, m_vUpdateMax.y, m_vUpdateMax.z);
		m_vCollitionBox[5] = TVector3(m_vUpdateMax.x, m_vUpdateMax.y, m_vUpdateMax.z);
		m_vCollitionBox[6] = TVector3(m_vUpdateMax.x, m_vUpdateMin.y, m_vUpdateMax.z);
		m_vCollitionBox[7] = TVector3(m_vUpdateMin.x, m_vUpdateMin.y, m_vUpdateMax.z);
	}

#endif

}

void LATopObject::UpdatePosition()
{
	if (m_MovePushStack > 1)
	{
		//사선길이 체크
		float root = std::sqrt(2) / 2;

		m_vMoveUpdatePosition.x = m_vMoveUpdatePosition.x * root;
		m_vMoveUpdatePosition.z = m_vMoveUpdatePosition.z * root;
	}
	//프레임 전 위치값 저장
	m_vObjectBeforePosition = m_vObjectPosition;

	//포지션 업데이트
	m_vObjectPosition.x += m_vMoveUpdatePosition.x;
	m_vObjectPosition.y = m_vMoveUpdatePosition.y;
	m_vObjectPosition.z += m_vMoveUpdatePosition.z;

	//이동값 저장 변수 초기화
	m_MovePushStack = 0;
	ZeroMemory(&m_vMoveUpdatePosition, sizeof(TVector3));
}

void LATopObject::UpdateMotion()
{
	if (m_iBeforeMotion != m_iCurrentMotion)
	{
		m_fElapseFrame = 0.0f;
	}

	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pResourceData)
	{
		SceneData& rData = pResourceData->m_AnimationList[m_iCurrentMotion];
		
		float addspeed = m_fAddAnimationSpeed * g_fSecPerFrame;
		m_fElapseFrame += g_fSecPerFrame * rData.iFrameSpeed;
		m_fElapseFrame += addspeed;
	
		if ((int)m_fElapseFrame >= rData.iLastFrame - 1)
		{
			m_fElapseFrame = rData.iFirstFrame + 1;

			ResetCheck();
			//죽었을경우 삭제리스트에 추가
			if (m_bIsObjectLife == false)
			{
				DeleteObject();
			}

			return;
		}
	
		m_iMainCurrentTick = (int)(m_fElapseFrame * rData.iTickSecPerFrame);
		//기존모션 체크하기위해 저자
		m_iBeforeMotion = m_iCurrentMotion;
	}
}

void LATopObject::UpdateVector()
{
	//방향벡터 업데이트
	m_vObjectSide.x = m_matObjectWorld._11;
	m_vObjectSide.y = m_matObjectWorld._21;
	m_vObjectSide.z = m_matObjectWorld._31;

	m_vObjectUp.x = m_matObjectWorld._12;
	m_vObjectUp.y = m_matObjectWorld._22;
	m_vObjectUp.z = m_matObjectWorld._32;

	m_vObjectLook.x = m_matObjectWorld._13;
	m_vObjectLook.y = m_matObjectWorld._23;
	m_vObjectLook.z = m_matObjectWorld._33;

	D3DXVec3Normalize(&m_vObjectUp, &m_vObjectUp);
	D3DXVec3Normalize(&m_vObjectSide, &m_vObjectSide);
	D3DXVec3Normalize(&m_vObjectLook, &m_vObjectLook);
}

void LATopObject::UpdateVector(TMatrix matRotation)
{
	//방향벡터 업데이트
	m_vObjectSide.x = matRotation._11;
	m_vObjectSide.y = matRotation._21;
	m_vObjectSide.z = matRotation._31;

	m_vObjectUp.x = matRotation._12;
	m_vObjectUp.y = matRotation._22;
	m_vObjectUp.z = matRotation._32;

	m_vObjectLook.x = matRotation._13;
	m_vObjectLook.y = matRotation._23;
	m_vObjectLook.z = matRotation._33;

	D3DXVec3Normalize(&m_vObjectUp, &m_vObjectUp);
	D3DXVec3Normalize(&m_vObjectSide, &m_vObjectSide);
	D3DXVec3Normalize(&m_vObjectLook, &m_vObjectLook);
}

void LATopObject::UpdateObjectDataInfo()
{
	//오브젝트 이름 등록
	SetObjectName(m_ObjectData.wsCodeName);

	//minmax셋팅
	SetMinMax();
	//스케일값 셋팅
	SetScale(SCALE_BASE_SIZE, m_ObjectData.fScale, m_ObjectData.fScale, m_ObjectData.fScale);

	//충돌체크 바운딩박스 셋팅
	TVector3 vbaseSaclingMin;
	TVector3 vbaseSaclingMax;
	D3DXVec3TransformCoord(&vbaseSaclingMin, &m_vAllSkinMin, &m_matObjectScale);
	D3DXVec3TransformCoord(&vbaseSaclingMax, &m_vAllSkinMax, &m_matObjectScale);

#ifdef TOOL_MODE

#else

	m_BoundingBox.CreateBoundingBox(vbaseSaclingMax, vbaseSaclingMin, m_vObjectPosition);

#endif
}

bool LATopObject::DeleteObject()
{
	if (m_ObjectData.eObjectType == E_LoadTableType::MONSTER)
	{
		LOAD_OBJECT_MGR.AddDeleteList(m_ObjectData.eObjectType, this);
	}

	return true;
}

void LATopObject::ResetCheck()
{
	m_bisMotion = false;
	m_bisJump = false;
	m_bisRun = false;
	m_bIsShot = false;
	m_bIsRifleShot = false;
	m_bisCheckTick = false;
}

void LATopObject::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

#ifdef TOOL_MODE

#else

	//test
	m_DrawLine.Init();
	m_DrawLine.Create(g_pd3dDevice, L"../../data/shader/LineShader.hlsl");
	m_bDrawCollitionLine = false;

#endif
}

bool LATopObject::Create(CrateObjectDest& pMatrixDest, CrateObjectDest& pSkinDest)
{
	//main
	/*if (m_pMainResource != NULL)
	{
		for (int iBoneObj = 0; iBoneObj < (int)m_pMainResource->m_BoneObjectList.size(); iBoneObj++)
		{
			m_pMainResource->m_BoneObjectList[iBoneObj]->Create(pMatrixDest);
			m_pMainResource->m_BoneObjectList[iBoneObj]->SetCrrentTick(true);
		}

		for (int iSkinObj = 0; iSkinObj < (int)m_pMainResource->m_SkinObjectList.size(); iSkinObj++)
		{
			m_pMainResource->m_SkinObjectList[iSkinObj]->Create(pSkinDest);
			m_pMainResource->m_SkinObjectList[iSkinObj]->SetCrrentTick(true);
		}
	}*/
	
	//sub

	return true;
}

void LATopObject::SetMatrix(TMatrix* pWorldMatrix, TMatrix* pViewMatrix, TMatrix* pProjectionMatrix)
{
	//main
	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pResourceData)
	{
		for (int iBoneObj = 0; iBoneObj < (int)pResourceData->m_BoneObjectList.size(); iBoneObj++)
		{
			pResourceData->m_BoneObjectList[iBoneObj]->SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);
		}

		for (int iSkinObj = 0; iSkinObj < (int)pResourceData->m_SkinObjectList.size(); iSkinObj++)
		{
			pResourceData->m_SkinObjectList[iSkinObj]->SetMatrix(pWorldMatrix, pViewMatrix, pProjectionMatrix);
		}
	}

	

	//sub


	//test
#ifdef TOOL_MODE

#else

	if (m_bDrawCollitionLine)
	{	
		TMatrix world;
		D3DXMatrixIdentity(&world);
		m_DrawLine.SetMatrix(world, *pViewMatrix, *pProjectionMatrix);
	}

#endif

}

bool LATopObject::PrevFrame()
{
	//공통적으로 체크할것들

	//포지션업데이트
	UpdatePosition();

	return true;
}


bool LATopObject::Frame()
{
	//PrevFrame();

	PostFrame();



	return true;
}

bool LATopObject::PostFrame()
{
	//최종 행렬 계산
	UpdateWorldmatrix();

	//업데이트 minmax
	UpdateMinMax();



#ifdef TOOL_MODE

#else
	//충돌체크
	m_BoundingBox.UpdateBoundingBox(m_vObjectPosition);

	// 파티클 리스트 프레임
	for (int iParticle = 0; iParticle < m_ParticleSystemList.size(); iParticle++)
	{
		// 파티클시스템에 파티클이 존재하지않으면 삭제
		if (m_ParticleSystemList[iParticle].m_Particle.size() <= 0)
		{
			m_ParticleSystemList.erase(m_ParticleSystemList.begin() + iParticle);
			continue;
		}
		m_ParticleSystemList[iParticle].Frame();
	}

#endif

	

	return true;
}

bool LATopObject::PrevRender()
{
	//main
	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pResourceData)
	{
		//월드행렬 변경
		SetObjectWorldMatrixToResource();

		//월드행렬연결
		if ((int)pResourceData->m_BoneObjectList.size())
		{
			for (int iBoneObj = 0; iBoneObj < (int)pResourceData->m_BoneObjectList.size(); iBoneObj++)
			{
				pResourceData->m_BoneObjectList[iBoneObj]->m_iCurrentMotion = m_iCurrentMotion;
				pResourceData->m_BoneObjectList[iBoneObj]->m_ParentObjectTick = m_iMainCurrentTick;
				pResourceData->m_BoneObjectList[iBoneObj]->Frame();
			}

			SetCBBoneData();
		}

		if ((int)pResourceData->m_SkinObjectList.size())
		{
			for (int iSkinObj = 0; iSkinObj < (int)pResourceData->m_SkinObjectList.size(); iSkinObj++)
			{
				pResourceData->m_SkinObjectList[iSkinObj]->Frame();
				pResourceData->m_SkinObjectList[iSkinObj]->SetBoneMatrices(&m_cbMatrixListData);

				/*========================= 라이트 셋팅 ==========================*/
				pResourceData->m_SkinObjectList[iSkinObj]->SetDirectionLight(m_vCameraPos, m_vLightPos, m_vLightDirection);

				/*========================= 오브젝트 월드행렬 셋팅 ==========================*/
				pResourceData->m_SkinObjectList[iSkinObj]->SetBoneObjectCalculation(m_matObjectWorld);
			}
		}
	}

	//sub
	return true;
}

bool LATopObject::Render()
{
	PrevRender();
	//main
	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pResourceData)
	{
		if (m_bBipedRender == true)
		{
			for (int iBoneObj = 0; iBoneObj < (int)pResourceData->m_BoneObjectList.size(); iBoneObj++)
			{
				pResourceData->m_BoneObjectList[iBoneObj]->Render();
			}
		}

		if (m_bSkinRender == true)
		{
			for (int iSkinObj = 0; iSkinObj < (int)pResourceData->m_SkinObjectList.size(); iSkinObj++)
			{
				//기본값 true 툴에서사용
				if (pResourceData->m_SkinObjectList[iSkinObj]->m_bisRander)
				{
					pResourceData->m_SkinObjectList[iSkinObj]->Render();
				}
			}
		}

		PostRender();
	}

	//sub

	

	return true;
}

bool LATopObject::PostRender()
{

#ifdef TOOL_MODE

#else

	if (m_bDrawCollitionLine)
	{
		m_DrawLine.m_iPrimitiveType = 2;
		//임시 minmax 라인그리기
		m_DrawLine.SetLineStartToEnd(g_pImmediateContext, m_vCollitionBox[0], m_vCollitionBox[1], TVector4(1, 0, 0, 1));
		m_DrawLine.Render(g_pImmediateContext);
		m_DrawLine.SetLineStartToEnd(g_pImmediateContext, m_vCollitionBox[1], m_vCollitionBox[2], TVector4(1, 0, 0, 1));
		m_DrawLine.Render(g_pImmediateContext);
		m_DrawLine.SetLineStartToEnd(g_pImmediateContext, m_vCollitionBox[2], m_vCollitionBox[3], TVector4(1, 0, 0, 1));
		m_DrawLine.Render(g_pImmediateContext);
		m_DrawLine.SetLineStartToEnd(g_pImmediateContext, m_vCollitionBox[3], m_vCollitionBox[0], TVector4(1, 0, 0, 1));
		m_DrawLine.Render(g_pImmediateContext);

		m_DrawLine.SetLineStartToEnd(g_pImmediateContext, m_vCollitionBox[4], m_vCollitionBox[5], TVector4(1, 0, 0, 1));
		m_DrawLine.Render(g_pImmediateContext);
		m_DrawLine.SetLineStartToEnd(g_pImmediateContext, m_vCollitionBox[5], m_vCollitionBox[6], TVector4(1, 0, 0, 1));
		m_DrawLine.Render(g_pImmediateContext);
		m_DrawLine.SetLineStartToEnd(g_pImmediateContext, m_vCollitionBox[6], m_vCollitionBox[7], TVector4(1, 0, 0, 1));
		m_DrawLine.Render(g_pImmediateContext);
		m_DrawLine.SetLineStartToEnd(g_pImmediateContext, m_vCollitionBox[7], m_vCollitionBox[4], TVector4(1, 0, 0, 1));
		m_DrawLine.Render(g_pImmediateContext);

		m_DrawLine.SetLineStartToEnd(g_pImmediateContext, m_vCollitionBox[0], m_vCollitionBox[4], TVector4(1, 0, 0, 1));
		m_DrawLine.Render(g_pImmediateContext);
		m_DrawLine.SetLineStartToEnd(g_pImmediateContext, m_vCollitionBox[1], m_vCollitionBox[5], TVector4(1, 0, 0, 1));
		m_DrawLine.Render(g_pImmediateContext);
		m_DrawLine.SetLineStartToEnd(g_pImmediateContext, m_vCollitionBox[2], m_vCollitionBox[6], TVector4(1, 0, 0, 1));
		m_DrawLine.Render(g_pImmediateContext);
		m_DrawLine.SetLineStartToEnd(g_pImmediateContext, m_vCollitionBox[3], m_vCollitionBox[7], TVector4(1, 0, 0, 1));
		m_DrawLine.Render(g_pImmediateContext);
	}


	// 파티클 리스트 렌더링
	for (int iParticle = 0; iParticle < m_ParticleSystemList.size(); iParticle++)
		m_ParticleSystemList[iParticle].Render();

#endif


	return true;
}

bool LATopObject::RenderShadow()
{
	PrevRender();

	LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(m_MainResourceNumber);
	if (pResourceData)
	{
		if (m_bSkinRender == true)
		{
			for (int iSkinObj = 0; iSkinObj < (int)pResourceData->m_SkinObjectList.size(); iSkinObj++)
			{
				//기본값 true 툴에서사용
				if (pResourceData->m_SkinObjectList[iSkinObj]->m_bisRander)
				{
					pResourceData->m_SkinObjectList[iSkinObj]->RenderShadow();
				}
			}
		}
	}

	return true;
}


bool LATopObject::Release()
{
	if (m_pSkillData)
	{
		delete m_pSkillData;
		m_pSkillData = NULL;
	}

#ifdef TOOL_MODE

#else
	m_ParticleSystemList.clear();
#endif

	m_pMainResource = NULL;

	return true;
}

LATopObject::~LATopObject()
{
}
