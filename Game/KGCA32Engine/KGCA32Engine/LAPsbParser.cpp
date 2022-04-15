
//#include "stdafx.h"


#include "LAPsbParser.h"

#include "LAMaterialManager.h"
#include "LAMeshManager.h"
#include "LAAnimationManager.h"

#include "LAResourceManager.h"


bool LAPsbParser::SetUniqueBuffer(std::vector<PNCT5_VERTEX>& UniqueDataList, std::vector<PNCT5_VERTEX>& VertexDataList, std::vector<DWORD>& IndexList)
{
	UniqueDataList.reserve(VertexDataList.size());
	IndexList.clear();
	IndexList.reserve(VertexDataList.size());

	for (int iCnt = 0; iCnt < (int)VertexDataList.size(); iCnt++)
	{
		int index = IsEqualVertexList(UniqueDataList, VertexDataList[iCnt]);

		if (index < 0)
		{
			UniqueDataList.push_back(VertexDataList[iCnt]);
			index = UniqueDataList.size() - 1;
		}

		IndexList.push_back((DWORD)index);
	}

	return true;
}

//bool LAPsbParser::SetUniqueBuffer(std::vector<PNCT5_VERTEX>& UniqueDataList, PNCT5_VERTEX*& VertexDataList, std::vector<DWORD>& IndexList, int iLoadCount)
//{
//	UniqueDataList.reserve(iLoadCount);
//	IndexList.clear();
//	IndexList.reserve(iLoadCount);
//
//	for (int iCnt = 0; iCnt < iLoadCount; iCnt++)
//	{
//		int index = IsEqualVertexList(UniqueDataList, VertexDataList[iCnt]);
//
//		if (index < 0)
//		{
//			UniqueDataList.push_back(VertexDataList[iCnt]);
//			index = UniqueDataList.size() - 1;
//		}
//
//		IndexList.push_back((DWORD)index);
//	}
//
//	return true;
//}

bool LAPsbParser::SetUniqueBuffer(std::vector<PNCT_VECTOR>& UniqueDataList, std::vector<PNCT_VECTOR>& VertexDataList, std::vector<DWORD>& IndexList)
{
	UniqueDataList.reserve(VertexDataList.size());
	IndexList.clear();
	IndexList.reserve(VertexDataList.size());

	for (int iCnt = 0; iCnt < (int)VertexDataList.size(); iCnt++)
	{
		int index = IsEqualVertexList(UniqueDataList, VertexDataList[iCnt]);

		if (index < 0)
		{
			UniqueDataList.push_back(VertexDataList[iCnt]);
			index = UniqueDataList.size() - 1;
		}

		IndexList.push_back((DWORD)index);
	}

	return true;
}


int LAPsbParser::IsEqualVertexList(std::vector<PNCT5_VERTEX>& UniqueDataArray, PNCT5_VERTEX& pnctData)
{
	for (int iCnt = 0; iCnt < (int)UniqueDataArray.size(); iCnt++)
	{
		if (pnctData == UniqueDataArray[iCnt])
		{
			return iCnt;
		}
	}

	return -1;

}

//int LAPsbParser::IsEqualVertexList(PNCT5_VERTEX*& UniqueDataArray, PNCT5_VERTEX& pnctData, int iCountSize)
//{
//	for (int iCnt = 0; iCnt < iCountSize; iCnt++)
//	{
//		if (pnctData == UniqueDataArray[iCnt])
//		{
//			return iCnt;
//		}
//	}
//
//	return -1;
//}

void LAPsbParser::SearchToObjectParent(LAResource& rResource)
{
	TMatrix matInverseParent;

	for (int iCnt = 0; iCnt < (int)rResource.m_BoneObjectList.size(); iCnt++)
	{
		if (!rResource.m_BoneObjectList[iCnt]->m_MeshData->wsParentName.empty())
		{
			for (int jCnt = 0; jCnt < (int)rResource.m_BoneObjectList.size(); jCnt++)
			{
				if (rResource.m_BoneObjectList[iCnt]->m_MeshData->wsParentName ==
					rResource.m_BoneObjectList[jCnt]->m_MeshData->wsNodeName)
				{
					//parent 주소 저장
					rResource.m_BoneObjectList[iCnt]->m_pmatParent = &rResource.m_BoneObjectList[jCnt]->m_matCalculation;

					//자식행렬 계산
					D3DXMatrixInverse(&matInverseParent, NULL, &rResource.m_BoneObjectList[jCnt]->m_MeshData->matLoadObjectWorld);
					rResource.m_BoneObjectList[iCnt]->m_matChildWorld =
						rResource.m_BoneObjectList[iCnt]->m_MeshData->matLoadObjectWorld * matInverseParent;

					break;
				}
			}
		}
	}

	for (int iCnt = 0; iCnt < (int)rResource.m_SkinObjectList.size(); iCnt++)
	{
		if (!rResource.m_SkinObjectList[iCnt]->m_MeshData->wsParentName.empty())
		{
			for (int jCnt = 0; jCnt < (int)rResource.m_BoneObjectList.size(); jCnt++)
			{
				if (rResource.m_SkinObjectList[iCnt]->m_MeshData->wsParentName ==
					rResource.m_BoneObjectList[jCnt]->m_MeshData->wsNodeName)
				{
					//parent 주소 저장
					rResource.m_SkinObjectList[iCnt]->m_pmatParent = &rResource.m_BoneObjectList[jCnt]->m_matCalculation;

					//자식행렬 계산
					D3DXMatrixInverse(&matInverseParent, NULL, &rResource.m_BoneObjectList[jCnt]->m_MeshData->matLoadObjectWorld);
					rResource.m_SkinObjectList[iCnt]->m_matChildWorld =
						rResource.m_SkinObjectList[iCnt]->m_MeshData->matLoadObjectWorld * matInverseParent;

					break;
				}
			}
		}
	}
}

int LAPsbParser::IsEqualVertexList(std::vector<PNCT_VECTOR>& UniqueDataArray, PNCT_VECTOR& pnctData)
{
	for (int iCnt = 0; iCnt < (int)UniqueDataArray.size(); iCnt++)
	{
		if (pnctData == UniqueDataArray[iCnt])
		{
			return iCnt;
		}
	}

	return -1;
}

bool LAPsbParser::Load(FILE* pStream, E_LoadObjectType eLoadObjectType, bool bBinary)
{
	if (pStream == NULL)
		return false;

	m_pPsbFile = pStream;

	//File Load
	LoadSceneData(eLoadObjectType, bBinary);
	LoadData(eLoadObjectType, bBinary);

	return true;
}

void LAPsbParser::LoadSceneData(E_LoadObjectType eLoadObjectType, bool bBinary)
{
	int ObjectCount = 0;

	if (bBinary)
	{
		StructSecneData data;
		fread(&data, sizeof(StructSecneData), 1, m_pPsbFile);

		m_SceneData.iFirstFrame = data.iFirstFrame;
		m_SceneData.iLastFrame = data.iLastFrame;
		m_SceneData.iFrameSpeed = data.iFrameSpeed;
		m_SceneData.iTickSecPerFrame = data.iTickSecPerFrame;
		ObjectCount = data.iObjectCount;
		m_bIsSkining = (int)data.bisSkining;

		m_pMeshList.resize(ObjectCount);
	}
	else
	{
		//Title
		_fgetts(m_pString, 256, m_pPsbFile);

		switch ((int)eLoadObjectType)
		{
		default:
		{
			//Scene 정보는 애니메이션에서만 사용
			_fgetts(m_pString, 256, m_pPsbFile);
			//SCENE_Data( startFrame, EndFrame, speed, Tick, objcetcount)
			_stscanf_s(m_pString, _T("%s %d %d %d %d %d %d"),
				m_pBuffer, _countof(m_pBuffer),
				&m_SceneData.iFirstFrame,
				&m_SceneData.iLastFrame,
				&m_SceneData.iFrameSpeed,
				&m_SceneData.iTickSecPerFrame,
				&ObjectCount,
				(int)&m_bIsSkining
			);

			m_pMeshList.resize(ObjectCount);
		}break;
		}
	}
}

void LAPsbParser::SaveSceneData(FILE* pFile, LALoadObject& rLoadObjectData, int iMeshCount, E_ObjectType objectType, bool bBinary)
{
	if (objectType == E_ObjectType::Skin_Object)
	{
		if (bBinary)
		{
		}
		else
		{
			_ftprintf(pFile, _T("%s"), _T("#PSB_CHARACTER_TOOL_SKIN_OBJECT_DATA"));
		}
	}
	else if (objectType == E_ObjectType::Animation)
	{
		int iFirstFrame = 0;
		int iLastFrame = rLoadObjectData.m_SaveObjecDatatItor->iLastFrame - rLoadObjectData.m_SaveObjecDatatItor->iFirstFrame;

		if (bBinary)
		{
			/*====================== 바이너리 씬 헤더 정보 출력 =======================*/
			StructSecneData data;
			data.iFirstFrame = iFirstFrame;
			data.iLastFrame = iLastFrame;
			data.iFrameSpeed = rLoadObjectData.m_SaveObjecDatatItor->iFrameSpeed;
			data.iTickSecPerFrame = rLoadObjectData.m_SaveObjecDatatItor->iTickSecPerFrame;
			data.iObjectCount = iMeshCount;
			data.bisSkining = 0;

			fwrite(&data, sizeof(StructSecneData), 1, pFile);
		}
		else
		{
			_ftprintf(pFile, _T("%s"), _T("#PSB_CHARACTER_TOOL_ANIMATION_DATA"));
			_ftprintf(pFile, _T("\n%s %d %d %d %d %d"), _T("#SCENE"),
				iFirstFrame,
				iLastFrame,
				rLoadObjectData.m_SaveObjecDatatItor->iFrameSpeed,
				rLoadObjectData.m_SaveObjecDatatItor->iTickSecPerFrame,
				iMeshCount
			);
		}

		return;
	}
	else
	{
		if (bBinary)
		{
		}
		else
		{
			_ftprintf(pFile, _T("%s"), _T("#PSB_CHARACTER_TOOL_MATRIX_OBJECT_DATA"));
		}
	}


	if (bBinary)
	{
		StructSecneData data;
		data.iFirstFrame = 0;
		data.iLastFrame = 0;
		data.iFrameSpeed = 0;
		data.iTickSecPerFrame = 0;
		data.iObjectCount = iMeshCount;
		data.bisSkining = (int)rLoadObjectData.m_NewLoadObject->m_pMainResource->m_bIsSkining;

		fwrite(&data, sizeof(StructSecneData), 1, pFile);
	}
	else
	{
		_ftprintf(pFile, _T("\n%s %d %d %d %d %d %d"), _T("#SCENE"),
			0, // rLoadObjectData.m_LoadObjectSceneDataList.iFirstFrame,
			0, //rLoadObjectData.m_LoadObjectSceneDataList.iLastFrame,
			0, //rLoadObjectData.m_LoadObjectSceneDataList.iFrameSpeed,
			0, //rLoadObjectData.m_LoadObjectSceneDataList.iTickSecPerFrame,
			iMeshCount,
			(int)rLoadObjectData.m_NewLoadObject->m_pMainResource->m_bIsSkining
		);
	}
}

bool LAPsbParser::LoadData(E_LoadObjectType eLoadObjectType, bool bBinary)
{
	if (bBinary)
	{
		int iSizeObject = m_pMeshList.size();

		switch ((int)eLoadObjectType)
		{
		case (int)E_LoadObjectType::CHARACTER_MATRIX:
		case (int)E_LoadObjectType::CHARACTER_SKIN:
		case (int)E_LoadObjectType::CHARACTER_ITEM:
		{
			for (int iObjCnt = 0; iObjCnt < iSizeObject; iObjCnt++)
			{
				//이름 및 행렬 저장
				LoadNodeData(m_pMeshList[iObjCnt], bBinary);
				//Matrerial 데이터 저장
				LoadMaterialData(m_pMeshList[iObjCnt], bBinary);
				//Mesh 데이터 저장
				LoadMeshData(m_pMeshList[iObjCnt], bBinary);
			}
		}break;

		case (int)E_LoadObjectType::CHARACTER_ANIMATION:
		{
			for (int iObjCnt = 0; iObjCnt < iSizeObject; iObjCnt++)
			{
				//Animation 데이터 저장
				LoadAnimationData(m_pMeshList[iObjCnt], bBinary);
			}
		}break;

		default:
		{
			for (int iObjCnt = 0; iObjCnt < iSizeObject; iObjCnt++)
			{
				//이름 및 행렬 저장
				LoadNodeData(m_pMeshList[iObjCnt], bBinary);
				//Matrerial 데이터 저장
				LoadMaterialData(m_pMeshList[iObjCnt], bBinary);
				//Mesh 데이터 저장
				LoadMeshData(m_pMeshList[iObjCnt], bBinary);
			}
		}break;
		}
	}
	else
	{
		bool bReadMaterial = false;
		int iSection = 0;

		_fgetts(m_pString, 256, m_pPsbFile);

		//std::vector<LAMesh>& pMeshList = m_pMeshList[0]->m_SubMesh;
		int iSizeObject = m_pMeshList.size();

		switch ((int)eLoadObjectType)
		{
		case (int)E_LoadObjectType::CHARACTER_MATRIX:
		case (int)E_LoadObjectType::CHARACTER_SKIN:
		case (int)E_LoadObjectType::CHARACTER_ITEM:
		{
			for (int iObjCnt = 0; iObjCnt < iSizeObject; iObjCnt++)
			{
				//m_pMeshList[iObjCnt].m_SceneDataList = m_SceneDataList;
				//이름 및 행렬 저장
				LoadNodeData(m_pMeshList[iObjCnt], bBinary);
				//Matrerial 데이터 저장
				LoadMaterialData(m_pMeshList[iObjCnt], bBinary);
				//Mesh 데이터 저장
				LoadMeshData(m_pMeshList[iObjCnt], bBinary);

				_fgetts(m_pString, 256, m_pPsbFile);
			}
		}break;

		case (int)E_LoadObjectType::CHARACTER_ANIMATION:
		{
			for (int iObjCnt = 0; iObjCnt < iSizeObject; iObjCnt++)
			{
				//#OBJECT 0
				_fgetts(m_pString, 256, m_pPsbFile);

				//Animation 데이터 저장
				LoadAnimationData(m_pMeshList[iObjCnt], false);

				_fgetts(m_pString, 256, m_pPsbFile);
			}
		}break;

		default:
		{
			for (int iObjCnt = 0; iObjCnt < iSizeObject; iObjCnt++)
			{
				//m_pMeshList[iObjCnt].m_SceneDataList = m_SceneDataList;
				//이름 및 행렬 저장
				LoadNodeData(m_pMeshList[iObjCnt], bBinary);
				//Matrerial 데이터 저장
				LoadMaterialData(m_pMeshList[iObjCnt], bBinary);
				//Mesh 데이터 저장
				LoadMeshData(m_pMeshList[iObjCnt], bBinary);
				//Animation 데이터 저장
				//LoadAnimationData(m_pMeshList[iObjCnt]);

				_fgetts(m_pString, 256, m_pPsbFile);
			}
		}break;
		}
	}

	return true;
}

void LAPsbParser::LoadAnimationData(LAMesh& pMesh, bool bBinary)
{
	if (bBinary)
	{
		int AniScaleCount = 0;
		int AniRotationCount = 0;
		int AniTaransrationCount = 0;

		/*====================== 바이너리 Animation Head 정보 =======================*/
		StructAnimationHeadData data;
		ZeroMemory(&data, sizeof(StructAnimationHeadData));
		
		fread(&data, sizeof(StructAnimationHeadData), 1, m_pPsbFile);

		/*====================== 바이너리 Animation Nodename 정보 =======================*/
		//*NodeName Bip001
		ZeroMemory(m_pBuffer, 256);
		fread(m_pBuffer, sizeof(TCHAR) * data.iNodeNameCount, 1, m_pPsbFile);
		pMesh.wsNodeName = m_pBuffer;

		/*====================== 바이너리 Animation Data 정보 =======================*/
		//*ANIMATION 0 0 0
		AniScaleCount = data.iScaleCount;
		AniRotationCount = data.iRotationCount;
		AniTaransrationCount = data.iTransportCount;

		if ((AniScaleCount == 0) &&
			(AniRotationCount == 0) &&
			(AniTaransrationCount == 0))
			return;

		LAAnimaionData AniData;
		AnimationTrack pTrack;
		AnimationTrack pBaseTrack;

		/*====================== 바이너리 Animation Scale 정보 =======================*/
		if (AniScaleCount != 0)
		{
			//보류
		}

		/*====================== 바이너리 Animation Rotation 정보 =======================*/
		//회전
		if (AniRotationCount != 0)
		{
			TVector3 vAxis;
			float Angle;
			for (int iRotation = 0; iRotation < AniRotationCount; iRotation++)
			{
				StructAnimationData Anidata;
				ZeroMemory(&Anidata, sizeof(StructAnimationData));

				fread(&Anidata, sizeof(StructAnimationData), 1, m_pPsbFile);
			
				pBaseTrack.Tick = Anidata.Tick;
				pBaseTrack.Axis = TQuaternion(Anidata.Axis.x, Anidata.Axis.y, Anidata.Axis.z, Anidata.Axis.w);
				vAxis.x = Anidata.Axis.x;
				vAxis.y = Anidata.Axis.y;
				vAxis.z = Anidata.Axis.z;
				pTrack.Tick = Anidata.Tick;
				D3DXQuaternionRotationAxis(&pTrack.Axis, &vAxis, Anidata.Axis.w);

				AniData.BaseAniData[(int)E_AnimationType::AnimatinRotation].push_back(pBaseTrack);
				AniData.AnimationTrack[(int)E_AnimationType::AnimatinRotation].push_back(pTrack);
			}
		}

		/*====================== 바이너리 Animation Transpos 정보 =======================*/
		//이동
		if (AniTaransrationCount != 0)
		{
			for (int iRotation = 0; iRotation < AniTaransrationCount; iRotation++)
			{
				StructAnimationData Anidata;
				ZeroMemory(&Anidata, sizeof(StructAnimationData));

				fread(&Anidata, sizeof(StructAnimationData), 1, m_pPsbFile);

				pTrack.Tick = Anidata.Tick;
				pTrack.Pos = Anidata.Pos;

				AniData.AnimationTrack[(int)E_AnimationType::AnimatinTransformation].push_back(pTrack);
			}
		}

		//Manager 등록 아이디 저장
		pMesh.m_AnimationIDList.push_back(ANIMATION_MGR.AddAnimaion(AniData));

		return;
	}
	else
	{
		int AniScaleCount = 0;
		int AniRotationCount = 0;
		int AniTaransrationCount = 0;

		//*NodeName Bip001
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%s %s"),
			m_pBuffer, _countof(m_pBuffer),
			m_pBuffer, _countof(m_pBuffer)
		);
		pMesh.wsNodeName = m_pBuffer;

		//*ANIMATION 0 0 0
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%s %d %d %d"),
			m_pBuffer, _countof(m_pBuffer),
			&AniScaleCount,
			&AniRotationCount,
			&AniTaransrationCount
		);

		if ((AniScaleCount == 0) &&
			(AniRotationCount == 0) &&
			(AniTaransrationCount == 0))
			return;

		LAAnimaionData AniData;
		AnimationTrack pTrack;
		AnimationTrack pBaseTrack;

		if (AniScaleCount != 0)
		{
			//보류
		}

		//회전
		if (AniRotationCount != 0)
		{
			TVector3 vAxis;
			float Angle;
			for (int iRotation = 0; iRotation < AniRotationCount; iRotation++)
			{
				//tick Quaterion
				//0    -0.5774    -0.5774    -0.5774     4.1888
				_fgetts(m_pString, 256, m_pPsbFile);
				_stscanf_s(m_pString, _T("%d %f %f %f %f"),
					&pTrack.Tick,
					&vAxis.x,
					&vAxis.y,
					&vAxis.z,
					&Angle
				);
				pBaseTrack.Tick = pTrack.Tick;
				pBaseTrack.Axis = TQuaternion(vAxis.x, vAxis.y, vAxis.z, Angle);
				D3DXQuaternionRotationAxis(&pTrack.Axis, &vAxis, Angle);

				AniData.BaseAniData[(int)E_AnimationType::AnimatinRotation].push_back(pBaseTrack);
				AniData.AnimationTrack[(int)E_AnimationType::AnimatinRotation].push_back(pTrack);
			}
		}

		//이동
		if (AniTaransrationCount != 0)
		{
			for (int iRotation = 0; iRotation < AniTaransrationCount; iRotation++)
			{
				//tick vector
				//0     2.0272    -0.0000     6.9124
				_fgetts(m_pString, 256, m_pPsbFile);
				_stscanf_s(m_pString, _T("%d %f %f %f"),
					&pTrack.Tick,
					&pTrack.Pos.x,
					&pTrack.Pos.y,
					&pTrack.Pos.z
				);

				AniData.AnimationTrack[(int)E_AnimationType::AnimatinTransformation].push_back(pTrack);
			}
		}

		//Manager 등록 아이디 저장
		pMesh.m_AnimationIDList.push_back(ANIMATION_MGR.AddAnimaion(AniData));
	}
}


void LAPsbParser::SaveAnimationData(FILE* pFile, LALoadObject& LoadObjectdata, LAMesh& pMesh, bool bBinary)
{
	if (bBinary)
	{
		if ((int)pMesh.m_AnimationIDList.size() != 0)
		{
			int AniNumber = LoadObjectdata.m_SaveObjecDatatItor->iAnimationNumber;
			//원본 애니메이션
			LAAnimaionData* pAniData = ANIMATION_MGR.GetAnimaion(pMesh.m_AnimationIDList[AniNumber]);
			int iSacleAniSize = 0;
			int iRotationAniSize = 0;
			int iTransrationAniSize = 0;

			int SaveStart = LoadObjectdata.m_SaveObjecDatatItor->iFirstFrame;
			int SaveEnd = LoadObjectdata.m_SaveObjecDatatItor->iLastFrame;
			int size = SaveEnd - SaveStart;
			int count = 0;

			if (pAniData)
			{
				if ((int)pAniData->AnimationTrack[(int)E_AnimationType::AnimatinScale].size() != 0)
				{
					iSacleAniSize = size;
				}
				if ((int)pAniData->BaseAniData[(int)E_AnimationType::AnimatinRotation].size() != 0)
				{
					iRotationAniSize = size;
				}
				if ((int)pAniData->AnimationTrack[(int)E_AnimationType::AnimatinTransformation].size() != 0)
				{
					iTransrationAniSize = size;
				}

				/*====================== 바이너리 Animation Head 정보 =======================*/
				StructAnimationHeadData data;
				ZeroMemory(&data, sizeof(StructAnimationHeadData));
				data.iNodeNameCount = pMesh.wsNodeName.length();
				data.iScaleCount = iSacleAniSize;
				data.iRotationCount = iRotationAniSize;
				data.iTransportCount = iTransrationAniSize;
				fwrite(&data, sizeof(StructAnimationHeadData), 1, pFile);

				/*====================== 바이너리 Animation Nodename 정보 =======================*/
				fwrite(pMesh.wsNodeName.c_str(), sizeof(TCHAR) * data.iNodeNameCount, 1, pFile);

				/*====================== 바이너리 Animation Data 정보 =======================*/
				if (iSacleAniSize > 0)
				{
					/*====================== 바이너리 Animation Scale 정보 =======================*/
				}
				if (iRotationAniSize > 0)
				{
					count = 0;
					std::vector<AnimationTrack>& rAniArray = pAniData->BaseAniData[(int)E_AnimationType::AnimatinRotation];

					for (int iCnt = SaveStart; iCnt < SaveEnd; iCnt++)
					{
						/*====================== 바이너리 Animation Rotation 정보 =======================*/
						StructAnimationData Anidata;
						ZeroMemory(&Anidata, sizeof(StructAnimationData));

						Anidata.Tick = count * LoadObjectdata.m_SaveObjecDatatItor->iTickSecPerFrame;
						Anidata.Axis = rAniArray[iCnt].Axis;

						fwrite(&Anidata, sizeof(StructAnimationData), 1, pFile);

						count++;
					}
				}
				if (iTransrationAniSize > 0)
				{
					count = 0;
					std::vector<AnimationTrack>& rAniArray = pAniData->AnimationTrack[(int)E_AnimationType::AnimatinTransformation];

					for (int iCnt = SaveStart; iCnt < SaveEnd; iCnt++)
					{
						/*====================== 바이너리 Animation Transpos 정보 =======================*/
						StructAnimationData Anidata;
						ZeroMemory(&Anidata, sizeof(StructAnimationData));

						Anidata.Tick = count * LoadObjectdata.m_SaveObjecDatatItor->iTickSecPerFrame;
						Anidata.Pos = rAniArray[iCnt].Pos;

						fwrite(&Anidata, sizeof(StructAnimationData), 1, pFile);

						count++;
					}
				}

				return;
			}
			else
			{
				/*====================== 바이너리 Animation Head 정보 =======================*/
				StructAnimationHeadData data;
				ZeroMemory(&data, sizeof(StructAnimationHeadData));
				int nameCount = pMesh.wsNodeName.length();
				data.iNodeNameCount = nameCount;
				fwrite(&data, sizeof(StructAnimationHeadData), 1, pFile);
				fwrite(pMesh.wsNodeName.c_str(), sizeof(TCHAR) * nameCount, 1, pFile);

				return;
			}
		}
		else
		{
			/*====================== 바이너리 Animation Head 정보 =======================*/
			StructAnimationHeadData data;
			ZeroMemory(&data, sizeof(StructAnimationHeadData));
			int nameCount = pMesh.wsNodeName.length();
			data.iNodeNameCount = nameCount;
			fwrite(&data, sizeof(StructAnimationHeadData), 1, pFile);
			fwrite(pMesh.wsNodeName.c_str(), sizeof(TCHAR) * nameCount, 1, pFile);

			return;
		}
	}
	else
	{
		if ((int)pMesh.m_AnimationIDList.size() != 0)
		{
			int AniNumber = LoadObjectdata.m_SaveObjecDatatItor->iAnimationNumber;
			//원본 애니메이션
			LAAnimaionData* pAniData = ANIMATION_MGR.GetAnimaion(pMesh.m_AnimationIDList[AniNumber]);
			int iSacleAniSize = 0;
			int iRotationAniSize = 0;
			int iTransrationAniSize = 0;

			int SaveStart = LoadObjectdata.m_SaveObjecDatatItor->iFirstFrame;
			int SaveEnd = LoadObjectdata.m_SaveObjecDatatItor->iLastFrame;
			int size = SaveEnd - SaveStart;
			int count = 0;

			if (pAniData)
			{
				if ((int)pAniData->AnimationTrack[(int)E_AnimationType::AnimatinScale].size() != 0)
				{
					iSacleAniSize = size;
				}
				if ((int)pAniData->BaseAniData[(int)E_AnimationType::AnimatinRotation].size() != 0)
				{
					iRotationAniSize = size;
				}
				if ((int)pAniData->AnimationTrack[(int)E_AnimationType::AnimatinTransformation].size() != 0)
				{
					iTransrationAniSize = size;
				}

				_ftprintf(pFile, _T("\n\t%s %s"), _T("*NodeName"), pMesh.wsNodeName.c_str());
				_ftprintf(pFile, _T("\n\t%s %d %d %d"), _T("*ANIMATION"), iSacleAniSize, iRotationAniSize, iTransrationAniSize);
				if (iSacleAniSize > 0)
				{
					_ftprintf(pFile, _T("\n\t\t%s"), _T("NOT SCALE PRINT"));
				}
				if (iRotationAniSize > 0)
				{
					count = 0;
					std::vector<AnimationTrack>& rAniArray = pAniData->BaseAniData[(int)E_AnimationType::AnimatinRotation];

					for (int iCnt = SaveStart; iCnt < SaveEnd; iCnt++)
					{
						//tick, x, y, z, w(쿼터니언)
						_ftprintf(pFile, _T("\n\t\t%d %10.4f %10.4f %10.4f %10.4f"),
							count * LoadObjectdata.m_SaveObjecDatatItor->iTickSecPerFrame,
							rAniArray[iCnt].Axis.x, rAniArray[iCnt].Axis.y, rAniArray[iCnt].Axis.z, rAniArray[iCnt].Axis.w);

						count++;
					}
				}
				if (iTransrationAniSize > 0)
				{
					count = 0;
					std::vector<AnimationTrack>& rAniArray = pAniData->AnimationTrack[(int)E_AnimationType::AnimatinTransformation];

					for (int iCnt = SaveStart; iCnt < SaveEnd; iCnt++)
					{
						_ftprintf(pFile, _T("\n\t\t%d %10.4f %10.4f %10.4f"),
							count * LoadObjectdata.m_SaveObjecDatatItor->iTickSecPerFrame,
							rAniArray[iCnt].Pos.x, rAniArray[iCnt].Pos.y, rAniArray[iCnt].Pos.z);

						count++;
					}
				}
			}
			else
			{
				_ftprintf(pFile, _T("\n\t%s %s"), _T("*NodeName"), pMesh.wsNodeName.c_str());
				_ftprintf(pFile, _T("\n\t%s %d %d %d"), _T("*ANIMATION"), 0, 0, 0);
			}
		}
		else
		{
			_ftprintf(pFile, _T("\n\t%s %s"), _T("*NodeName"), pMesh.wsNodeName.c_str());
			_ftprintf(pFile, _T("\n\t%s %d %d %d"), _T("*ANIMATION"), 0, 0, 0);

			return;
		}
	}
}

void LAPsbParser::LoadMaterialData(LAMesh& pMesh, bool bBinary)
{
	if (bBinary)
	{
		int MaterialCount;

		/*====================== 바이너리 매터리얼 카운드 =======================*/
		fread(&MaterialCount, sizeof(int), 1, m_pPsbFile);

		if (MaterialCount != -1)
		{
			//MaterialDataLoad
			for (int iMtlCnt = 0; iMtlCnt < MaterialCount; iMtlCnt++)
			{
				StructMaterialData data;

				/*====================== 바이너리 씬 헤더 정보 =======================*/
				fread(&data, sizeof(StructMaterialData), 1, m_pPsbFile);

				if (iMtlCnt == 0)
				{
					LAMaterialData pRootMaterialData;
					pMesh.m_RootMaterialDataID = MATERIAL_MGR.AddMaterial(pRootMaterialData);
					pRootMaterialData.iMaterialNumber = pMesh.m_RootMaterialDataID;
				}

				SubMaterialData* pSub = MATERIAL_MGR.GetSubMaterial(pMesh.m_RootMaterialDataID, data.iRootNumber);

				if (pSub == NULL)
				{
					pSub = new SubMaterialData;
					pSub->iSubMaterialNumber = data.iRootNumber;	//SubNumber

					TexMapData MapData;
					MapData.iTypeNumber = data.iSubNumber;	//TypeNumber

															/*====================== 바이너리 씬 네임 정보 =======================*/
					TCHAR bitmapName[256] = { 0 };
					fread(bitmapName, sizeof(TCHAR) * data.iCountMaterialName, 1, m_pPsbFile);

					MapData.pBitmapName = bitmapName;
					pSub->MapData.push_back(MapData);

					MATERIAL_MGR.AddSubMaterial(pMesh.m_RootMaterialDataID, *pSub);
				}
				else
				{

					TexMapData MapData;
					MapData.iTypeNumber = data.iSubNumber;

					/*====================== 바이너리 씬 네임 정보 =======================*/
					TCHAR bitmapName[256] = { 0 };
					fread(bitmapName, sizeof(TCHAR) * data.iCountMaterialName, 1, m_pPsbFile);

					MapData.pBitmapName = bitmapName;
					pSub->MapData.push_back(MapData);

				}
			}
		}
	}
	else
	{
		int iMaterialCount = 0;

		//*MATERIAL 1
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%s %d"),
			m_pBuffer, _countof(m_pBuffer),
			&iMaterialCount
		);

		if (iMaterialCount != -1)
		{
			int iSubMaterialNumber;
			int iSubAndSubNumber;
			int iRootMtl;

			//MaterialDataLoad
			for (int iMtlCnt = 0; iMtlCnt < iMaterialCount; iMtlCnt++)
			{
				//10 1 T_Base1.bmp
				_fgetts(m_pString, 256, m_pPsbFile);
				_stscanf_s(m_pString, _T("%d %d %s"),
					&iSubMaterialNumber,
					&iSubAndSubNumber,
					m_pBuffer, _countof(m_pBuffer)
				);

				if (iMtlCnt == 0)
				{
					LAMaterialData pRootMaterialData;
					//루트 아이디저장
					pMesh.m_RootMaterialDataID = MATERIAL_MGR.AddMaterial(pRootMaterialData);
					pRootMaterialData.iMaterialNumber = pMesh.m_RootMaterialDataID;
					iRootMtl = pMesh.m_RootMaterialDataID;

					SubMaterialData* pSub = MATERIAL_MGR.GetSubMaterial(iRootMtl, iSubMaterialNumber);

					if (pSub == NULL)
					{
						pSub = new SubMaterialData;
						pSub->iSubMaterialNumber = iSubMaterialNumber;

						TexMapData MapData;
						MapData.iTypeNumber = iSubAndSubNumber;
						MapData.pBitmapName = m_pBuffer;
						pSub->MapData.push_back(MapData);

						MATERIAL_MGR.AddSubMaterial(iRootMtl, *pSub);
					}
				}
				else
				{
					SubMaterialData* pSub = MATERIAL_MGR.GetSubMaterial(iRootMtl, iSubMaterialNumber);
					if (pSub != NULL)
					{
						TexMapData MapData;
						MapData.iTypeNumber = iSubAndSubNumber;
						MapData.pBitmapName = m_pBuffer;

						pSub->MapData.push_back(MapData);
					}
					else
					{
						pSub = new SubMaterialData;
						pSub->iSubMaterialNumber = iSubMaterialNumber;

						TexMapData MapData;
						MapData.iTypeNumber = iSubAndSubNumber;
						MapData.pBitmapName = m_pBuffer;
						pSub->MapData.push_back(MapData);

						MATERIAL_MGR.AddSubMaterial(iRootMtl, *pSub);
					}

					/*LAMaterialData* pRoot = MATERIAL_MGR.GetRootMaterial(iRootMtl);
					if (pRoot)
					{
					SubMaterialData* pSub = new SubMaterialData;
					pSub->iSubMaterialNumber = iSubMaterialNumber;

					TexMapData MapData;
					MapData.iTypeNumber = iSubAndSubNumber;
					MapData.pBitmapName = m_pBuffer;
					pSub->MapData.push_back(MapData);

					MATERIAL_MGR.AddSubMaterial(iRootMtl, *pSub);
					}*/
				}
			}
		}
	}
}

void LAPsbParser::SaveMaterialData(FILE* pFile, LAMesh& pMesh, E_ObjectType objectType, bool bBinary)
{
	LAMaterialData* pMaterialData = MATERIAL_MGR.GetRootMaterial(pMesh.m_RootMaterialDataID);
	LAMeshData* pMeshData = MESH_MGR.GetMesh(pMesh.m_MeshDataID);

	if (pMaterialData != NULL)
	{
		if (bBinary)
		{
			int iTotalMtl = 0;
			int iUniqueSize = pMaterialData->m_SubMaterialArray.size();
			for (int iMtlCnt = 0; iMtlCnt < iUniqueSize; iMtlCnt++)
			{
				iTotalMtl += (int)pMaterialData->m_SubMaterialArray[iMtlCnt].MapData.size();
			}

			/*====================== 바이너리 매터리얼 카운드 =======================*/
			fwrite(&iTotalMtl, sizeof(int), 1, m_pPsbFile);

			for (int iSubCnt = 0; iSubCnt < iUniqueSize; iSubCnt++)
			{
				for (int iSubType = 0; iSubType < pMaterialData->m_SubMaterialArray[iSubCnt].MapData.size(); iSubType++)
				{
					/*====================== 바이너리 씬 헤더 정보 저장 =======================*/
					StructMaterialData data;
					data.iRootNumber = pMaterialData->m_SubMaterialArray[iSubCnt].iSubMaterialNumber;
					data.iSubNumber = pMaterialData->m_SubMaterialArray[iSubCnt].MapData[iSubType].iTypeNumber;
					data.iCountMaterialName = pMaterialData->m_SubMaterialArray[iSubCnt].MapData[iSubType].pBitmapName.length();

					/*====================== 바이너리 씬 헤더 정보 출력 =======================*/
					fwrite(&data, sizeof(StructMaterialData), 1, m_pPsbFile);

					/*====================== 바이너리 씬 네임 정보 출력 =======================*/
					fwrite(pMaterialData->m_SubMaterialArray[iSubCnt].MapData[iSubType].pBitmapName.c_str(), sizeof(TCHAR) * data.iCountMaterialName, 1, m_pPsbFile);
				}
			}
		}
		else
		{
			//오브젝트 매터리얼 서브사이즈 체크
			int iTotalMtl = 0;
			int iSubCount = pMaterialData->m_SubMaterialArray.size();
			for (int iMtlCnt = 0; iMtlCnt < iSubCount; iMtlCnt++)
			{
				iTotalMtl += (int)pMaterialData->m_SubMaterialArray[iMtlCnt].MapData.size();
			}

			_ftprintf(pFile, _T("\n\t%s %d"), _T("*MATERIAL"), iTotalMtl);

			//Sub Count
			for (int iSubCnt = 0; iSubCnt < iSubCount; iSubCnt++)
			{
				//적용 텍스쳐 검색
				int iTaxtureNumber = pMaterialData->m_SubMaterialArray[iSubCnt].iSubMaterialNumber;

				for (int iSubType = 0; iSubType < pMaterialData->m_SubMaterialArray[iSubCnt].MapData.size(); iSubType++)
				{
					if (iTaxtureNumber != -1)
					{
						_ftprintf(pFile, _T("\n\t\t%d %d %s"),
							iTaxtureNumber,
							pMaterialData->m_SubMaterialArray[iSubCnt].MapData[iSubType].iTypeNumber,
							pMaterialData->m_SubMaterialArray[iSubCnt].MapData[iSubType].pBitmapName.c_str());
					}
					else
					{
						_ftprintf(pFile, _T("\n\t\t%d %d %s"), -1, -1, L"NO_IMG");
					}
				}
			}
		}
	}
	else
	{
		if (bBinary)
		{
			int iUniqueSize = -1;

			/*====================== 바이너리 매터리얼 카운드 =======================*/
			fwrite(&iUniqueSize, sizeof(int), 1, m_pPsbFile);
		}
		else
		{
			//오브젝트 매터리얼 서브사이즈 체크
			_ftprintf(pFile, _T("\n\t%s %d"), _T("*MATERIAL"), -1);
		}
	}
}

void LAPsbParser::LoadMeshData(LAMesh& pMesh, bool bBinary)
{
	if (bBinary)
	{
		/*====================== 바이너리 Mesh Total Count 정보 =======================*/
		int iSubTotalCount;
		int CheckAni;
		fread(&iSubTotalCount, sizeof(int), 1, m_pPsbFile);

		if (iSubTotalCount != 0)
		{
			LAMeshData pMeshData;

			for (int iCnt = 0; iCnt < iSubTotalCount; iCnt++)
			{
				/*====================== 바이너리 Mesh 헤더 정보 =======================*/
				StructMeshHeadData data;

				fread(&data, sizeof(StructMeshHeadData), 1, m_pPsbFile);
				CheckAni = data.iSkining;

				if ((pMesh.m_ObjectType == E_ObjectType::Skin_Object) && (CheckAni == 1))
				{
					pMeshData.m_SubSkinVecterList.resize(iSubTotalCount);
					pMeshData.m_SubSkinUniqueVecterList.resize(iSubTotalCount);

					pMeshData.m_SubIndexList.resize(iSubTotalCount);
					pMeshData.m_SubTextureNumber.resize(iSubTotalCount);

					//TextureNumber
					pMeshData.m_SubTextureNumber[iCnt] = data.iTextureNumber;

					/*========================== New =============================*/
					int iVertexPnctListSize;
					int iIndexListSize;
					fread(&iVertexPnctListSize, sizeof(int), 1, m_pPsbFile);
					fread(&iIndexListSize, sizeof(int), 1, m_pPsbFile);

					pMeshData.m_NewUniqueVertexListCount.resize(iSubTotalCount);
					pMeshData.m_NewUniqueVertexListCount[0] = iVertexPnctListSize;
					pMeshData.m_NewUniqueVecterList.resize(iSubTotalCount);
					pMeshData.m_NewUniqueVecterList[0] = new PNCT5_VERTEX[iVertexPnctListSize];

					pMeshData.m_NewUniqueIndexListCount.resize(iSubTotalCount);
					pMeshData.m_NewUniqueIndexListCount[0] = iIndexListSize;
					pMeshData.m_NewUniqueIndexList.resize(iSubTotalCount);
					pMeshData.m_NewUniqueIndexList[0] = new DWORD[iIndexListSize];

					fread(pMeshData.m_NewUniqueVecterList[0], sizeof(PNCT5_VERTEX) * iVertexPnctListSize, 1, m_pPsbFile);
					fread(pMeshData.m_NewUniqueIndexList[0], sizeof(DWORD) * iIndexListSize, 1, m_pPsbFile);

					pMeshData.m_SubSkinUniqueVecterList[iCnt].reserve(iVertexPnctListSize);
					for (int iCntVertex = 0; iCntVertex < iVertexPnctListSize; iCntVertex++)
					{
						pMeshData.m_SubSkinUniqueVecterList[0].push_back(pMeshData.m_NewUniqueVecterList[0][iCntVertex]);
					}

					pMeshData.m_SubIndexList[iCnt].reserve(iIndexListSize);
					for (int iCntIndex = 0; iCntIndex < iIndexListSize; iCntIndex++)
					{
						pMeshData.m_SubIndexList[iCnt].push_back(pMeshData.m_NewUniqueIndexList[0][iCntIndex]);
					}

					//PNCT 데이터 얻어오기
					//LoadPNCT(pMeshData.m_SubLoadSkinVecterList[iCnt], data.iFaceCount * 3, bBinary);

					//PNCT 데이터 최적화 Mesh 저장
					//SetUniqueBuffer(pMeshData.m_SubSkinUniqueVecterList[iCnt], pMeshData.m_SubLoadSkinVecterList[iCnt], pMeshData.m_SubIndexList[iCnt], data.iFaceCount * 3);
				}
				else
				{
					//스키닝 아닌것 생략(현제 모두 PNCT5 로 사용중) 
				}
			}

			if ((pMesh.m_ObjectType == E_ObjectType::Geom_Default) && (CheckAni == 1))
			{
				pMesh.m_ObjectType = E_ObjectType::Skin_Object;
			}
			//Mesh Manager 에 저장
			pMesh.m_MeshDataID = MESH_MGR.AddMesh(pMeshData);
		}
	}
	else
	{
		int iSubCount = 0;

		//*MESH SUB_COUNT 1
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%s %s %d"),
			m_pBuffer, _countof(m_pBuffer),
			m_pBuffer, _countof(m_pBuffer),
			&iSubCount
		);

		int Numtexture;
		int CountFace;
		int CheckAni;

		if (iSubCount != 0)
		{
			LAMeshData pMeshData;

			//서브 메쉬 카운트 얻어오기
			//서브 만큼 반복
			for (int iCnt = 0; iCnt < iSubCount; iCnt++)
			{
				//*SUB 0 10 24 1(sub numver, textureNumber, faceCount, boolCheckAni(0: false , 1: true))
				_fgetts(m_pString, 256, m_pPsbFile);
				_stscanf_s(m_pString, _T("%s %d %d %d %d"),
					m_pBuffer, _countof(m_pBuffer),
					&Numtexture,
					&Numtexture,
					&CountFace,
					&CheckAni
				);

				if ((pMesh.m_ObjectType == E_ObjectType::Geom_Default) && (CheckAni == 1))
				{
					//pMesh.m_ObjectType = E_ObjectType::Skin_Object;

					pMeshData.m_SubSkinVecterList.resize(iSubCount);
					pMeshData.m_SubSkinUniqueVecterList.resize(iSubCount);

					pMeshData.m_SubIndexList.resize(iSubCount);
					pMeshData.m_SubTextureNumber.resize(iSubCount);

					//TextureNumber
					pMeshData.m_SubTextureNumber[iCnt] = Numtexture;

					//PNCT 데이터 얻어오기
					LoadPNCT(pMeshData.m_SubSkinVecterList[iCnt], CountFace * 3, bBinary);

					//PNCT 데이터 최적화 Mesh 저장
					SetUniqueBuffer(pMeshData.m_SubSkinUniqueVecterList[iCnt], pMeshData.m_SubSkinVecterList[iCnt], pMeshData.m_SubIndexList[iCnt]);
				}
				else
				{
					pMeshData.m_SubUniqueVectorList.resize(iSubCount);
					pMeshData.m_SubVectorList.resize(iSubCount);

					pMeshData.m_SubIndexList.resize(iSubCount);
					pMeshData.m_SubTextureNumber.resize(iSubCount);

					//TextureNumber
					pMeshData.m_SubTextureNumber[iCnt] = Numtexture;

					//PNCT 데이터 얻어오기
					LoadPNCT(pMeshData.m_SubVectorList[iCnt], CountFace * 3);

					//PNCT 데이터 최적화 Mesh 저장
					SetUniqueBuffer(pMeshData.m_SubUniqueVectorList[iCnt], pMeshData.m_SubVectorList[iCnt], pMeshData.m_SubIndexList[iCnt]);
				}
			}

			if ((pMesh.m_ObjectType == E_ObjectType::Geom_Default) && (CheckAni == 1))
			{
				pMesh.m_ObjectType = E_ObjectType::Skin_Object;
			}
			//Mesh Manager 에 저장
			pMesh.m_MeshDataID = MESH_MGR.AddMesh(pMeshData);
		}
	}
}

void LAPsbParser::SaveMeshData(FILE* pFile, LAMesh& pMesh, E_ObjectType objectType, bool bBinary)
{
	LAMeshData* pMeshData = MESH_MGR.GetMesh(pMesh.m_MeshDataID);
	if (pMeshData == NULL)
	{
		if (bBinary)
		{
			/*====================== 바이너리 Mesh 서브 토탈 카운트 저장 =======================*/
			int iSubTotalCount = 0;
			fwrite(&iSubTotalCount, sizeof(int), 1, m_pPsbFile);
		}
		else
		{
			_ftprintf(pFile, _T("\n\t%s %s %d"), _T("*MESH"), _T("SUB_COUNT"), 0);
		}
		return;
	}

	if (objectType == E_ObjectType::Skin_Object)
	{
		if (bBinary)
		{
			/*====================== 바이너리 Mesh 서브 토탈 카운트 저장 =======================*/
			int iSubTotalCount = pMeshData->m_SubSkinVecterList.size();
			fwrite(&iSubTotalCount, sizeof(int), 1, m_pPsbFile);

			for (int iSubCnt = 0; iSubCnt < iSubTotalCount; iSubCnt++)
			{
				/*====================== 바이너리 Mesh 헤더 정보 저장 =======================*/
				StructMeshHeadData data;
				data.iMeshSubCount = (int)pMeshData->m_SubSkinVecterList[iSubCnt].size();
				data.iSubNumber = iSubCnt;
				data.iTextureNumber = pMeshData->m_SubTextureNumber[iSubCnt];
				data.iFaceCount = (int)pMeshData->m_SubSkinVecterList[iSubCnt].size() / 3;
				data.iSkining = 1;

				/*====================== 바이너리 Mesh 헤더 정보 출력 =======================*/
				fwrite(&data, sizeof(StructMeshHeadData), 1, m_pPsbFile);

				/*====================== 바이너리 Mesh 네임 정보 출력 =======================*/
				//int iCount = 0; 
				int iSubUniqueSize = pMeshData->m_SubSkinUniqueVecterList[iSubCnt].size();
				int iSubUniqueIndexSize = pMeshData->m_SubIndexList[iSubCnt].size();
				fwrite(&iSubUniqueSize, sizeof(int), 1, m_pPsbFile);
				fwrite(&iSubUniqueIndexSize, sizeof(int), 1, m_pPsbFile);

				SKIN_SUB_VERTEX& rVertexData = pMeshData->m_SubSkinUniqueVecterList[iSubCnt];

				PNCT5_VERTEX* pData = new PNCT5_VERTEX[iSubUniqueSize];
				for (int iCnt = 0; iCnt < iSubUniqueSize; iCnt++)
				{
					pData[iCnt] = rVertexData[iCnt];
				}

				SUBINDEX& rIndexData = pMeshData->m_SubIndexList[iSubCnt];
				DWORD* pIndexData = new DWORD[iSubUniqueIndexSize];
				for (int iCnt = 0; iCnt < iSubUniqueIndexSize; iCnt++)
				{
					pIndexData[iCnt] = (DWORD)rIndexData[iCnt];
				}

				fwrite(pData, sizeof(PNCT5_VERTEX) * iSubUniqueSize, 1, m_pPsbFile);
				fwrite(pIndexData, sizeof(DWORD) * iSubUniqueIndexSize, 1, m_pPsbFile);

				delete[] pData;
				delete[] pIndexData;
			}
		}
		else
		{
			bool Checkskin = 1;	// 1 : skin
			int SubTotalCount = (int)pMeshData->m_SubSkinVecterList.size();
			_ftprintf(pFile, _T("\n\t%s %s %d"), _T("*MESH"), _T("SUB_COUNT"), SubTotalCount);

			for (int iSubCnt = 0; iSubCnt < SubTotalCount; iSubCnt++)
			{
				int iVertexCnt = (int)pMeshData->m_SubSkinVecterList[iSubCnt].size();
				int iFaceCnt = iVertexCnt / 3;
				_ftprintf(pFile, _T("\n\t\t%s %d %d %d %d"), _T("*SUB"),
					iSubCnt,
					pMeshData->m_SubTextureNumber[iSubCnt],
					iFaceCnt,
					(int)Checkskin
				);
				PNCT5_VERTEX PnctVertex;


				for (int iCnt = 0; iCnt < iVertexCnt; iCnt++)
				{
					PnctVertex = pMeshData->m_SubSkinVecterList[iSubCnt][iCnt];

					_ftprintf(pFile, _T("\n\t\t%10.4f %10.4f %10.4f %10.4f"), PnctVertex.Vertex.x, PnctVertex.Vertex.y, PnctVertex.Vertex.z, PnctVertex.Vertex.w);
					_ftprintf(pFile, _T(" %10.4f %10.4f %10.4f %10.4f"), PnctVertex.Nomal.x, PnctVertex.Nomal.y, PnctVertex.Nomal.z, PnctVertex.Nomal.w);
					_ftprintf(pFile, _T(" %10.4f %10.4f %10.4f %10.4f"), PnctVertex.Color.x, PnctVertex.Color.y, PnctVertex.Color.z, PnctVertex.Color.w);
					_ftprintf(pFile, _T(" %10.4f %10.4f %d"), PnctVertex.Texture.x, PnctVertex.Texture.y, pMeshData->m_SubTextureNumber[iSubCnt]);

					//WeightCount, objNum0 ~ 3, Weight0 ~ 3
					_ftprintf(pFile, _T("\n\t\t\t%d %d %d %d %d"), (int)PnctVertex.Weight2[3], (int)PnctVertex.ObjNum1[0], (int)PnctVertex.ObjNum1[1], (int)PnctVertex.ObjNum1[2], (int)PnctVertex.ObjNum1[3]);
					_ftprintf(pFile, _T(" %10.4f %10.4f %10.4f %10.4f"), PnctVertex.Weight1[0], PnctVertex.Weight1[1], PnctVertex.Weight1[2], PnctVertex.Weight1[3]);
					_ftprintf(pFile, _T(" %d %d %d %d"), (int)PnctVertex.ObjNum2[0], (int)PnctVertex.ObjNum2[1], (int)PnctVertex.ObjNum2[2], (int)PnctVertex.ObjNum2[3]);
					_ftprintf(pFile, _T(" %10.4f %10.4f %10.4f %10.4f"), PnctVertex.Weight2[0], PnctVertex.Weight2[1], PnctVertex.Weight2[2], 0.0f);
				}
			}
		}
	}
	else
	{
		if (bBinary)
		{
			/*====================== 바이너리 씬 헤더 정보 저장 =======================*/
			StructMeshHeadData data;
			data.iMeshSubCount = (int)pMeshData->m_SubVectorList.size();


			for (int iSubCnt = 0; iSubCnt < data.iMeshSubCount; iSubCnt++)
			{
				data.iSubNumber = iSubCnt;
				data.iTextureNumber = pMeshData->m_SubTextureNumber[iSubCnt];
				data.iFaceCount = (int)pMeshData->m_SubSkinVecterList[iSubCnt].size() / 3;
				data.iSkining = 0;

				/*====================== 바이너리 씬 헤더 정보 출력 =======================*/
				fwrite(&data, sizeof(StructMeshHeadData), 1, m_pPsbFile);

				/*====================== 바이너리 씬 네임 정보 출력 =======================*/
				SUBVECTOR& rVertexData = pMeshData->m_SubVectorList[iSubCnt];
				for (int iCnt = 0; iCnt < (data.iFaceCount * 3); iCnt++)
				{
					fwrite(&rVertexData[iCnt], sizeof(PNCT_VECTOR), 1, m_pPsbFile);
				}
			}
		}
		else
		{
			bool Checkskin = 0;	// 1 : skin
			int SubTotalCount = (int)pMeshData->m_SubVectorList.size();
			_ftprintf(pFile, _T("\n\t%s %s %d"), _T("*MESH"), _T("SUB_COUNT"), SubTotalCount);

			for (int iSubCnt = 0; iSubCnt < SubTotalCount; iSubCnt++)
			{
				int iVertexCnt = (int)pMeshData->m_SubVectorList[iSubCnt].size();
				int iFaceCnt = iVertexCnt / 3;
				_ftprintf(pFile, _T("\n\t\t%s %d %d %d %d"), _T("*SUB"),
					iSubCnt,
					pMeshData->m_SubTextureNumber[iSubCnt],
					iFaceCnt,
					(int)Checkskin
				);
				PNCT_VECTOR PnctVertex;


				for (int iCnt = 0; iCnt < iVertexCnt; iCnt++)
				{
					PnctVertex = pMeshData->m_SubVectorList[iSubCnt][iCnt];

					_ftprintf(pFile, _T("\n\t\t%10.4f %10.4f %10.4f %10.4f"), PnctVertex.Vertex.x, PnctVertex.Vertex.y, PnctVertex.Vertex.z, PnctVertex.Vertex.w);
					_ftprintf(pFile, _T(" %10.4f %10.4f %10.4f %10.4f"), PnctVertex.Nomal.x, PnctVertex.Nomal.y, PnctVertex.Nomal.z, PnctVertex.Nomal.w);
					_ftprintf(pFile, _T(" %10.4f %10.4f %10.4f %10.4f"), PnctVertex.Color.x, PnctVertex.Color.y, PnctVertex.Color.z, PnctVertex.Color.w);
					_ftprintf(pFile, _T(" %10.4f %10.4f %d"), PnctVertex.Texture.x, PnctVertex.Texture.y, pMeshData->m_SubTextureNumber[iSubCnt]);
				}
			}
		}
	}
}

void LAPsbParser::LoadPNCT(std::vector<PNCT5_VERTEX>& vVector, int iLoadCount, bool bBinary)
{
	vVector.resize(iLoadCount);
	
	if (bBinary)
	{
		for (int jCnt = 0; jCnt < iLoadCount; jCnt++)
		{
			/*====================== 바이너리 Mesh PNCT5 정보 =======================*/
			//StructPNCT5_VERTEX VertexData;
			//ZeroMemory(&VertexData, sizeof(StructPNCT5_VERTEX));
			fread(&vVector[jCnt], sizeof(StructPNCT5_VERTEX), 1, m_pPsbFile);
			//memcpy_s(&vVector[jCnt], sizeof(PNCT5_VERTEX), &VertexData, sizeof(StructPNCT5_VERTEX));
			//int a = 10;
			//컬러값 고정.
			/*vVector[jCnt].Color.x = 1.0f;
			vVector[jCnt].Color.y = 1.0f;
			vVector[jCnt].Color.z = 1.0f;
			vVector[jCnt].Color.w = 1.0f;*/

			/*fread(vVector[jCnt].Vertex, sizeof(TVector4), 1, m_pPsbFile);
			fread(vVector[jCnt].Nomal, sizeof(TVector4), 1, m_pPsbFile);
			fread(vVector[jCnt].Color, sizeof(TVector4), 1, m_pPsbFile);
			fread(vVector[jCnt].Texture, sizeof(TVector2), 1, m_pPsbFile);

			fread(&vVector[jCnt].ObjNum1[0], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].ObjNum1[1], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].ObjNum1[2], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].ObjNum1[3], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].ObjNum2[0], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].ObjNum2[1], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].ObjNum2[2], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].ObjNum2[3], sizeof(float), 1, m_pPsbFile);
				
			fread(&vVector[jCnt].Weight1[0], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].Weight1[1], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].Weight1[2], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].Weight1[3], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].Weight2[0], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].Weight2[1], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].Weight2[2], sizeof(float), 1, m_pPsbFile);
			fread(&vVector[jCnt].Weight2[3], sizeof(float), 1, m_pPsbFile);*/
		}
	}
	else
	{
		int textureNumber;
		// -10.5390     1.4553    18.2652     1.0000    -0.6986     0.5909     0.4036     1.0000     1.0000     1.0000     1.0000     1.0000     0.2895     0.0959 10
		for (int jCnt = 0; jCnt < iLoadCount; jCnt++)
		{
			TVector4 vloadcolor;

			_fgetts(m_pString, 256, m_pPsbFile);
			_stscanf_s(m_pString, _T("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %d"),
				&vVector[jCnt].Vertex.x, &vVector[jCnt].Vertex.y, &vVector[jCnt].Vertex.z, &vVector[jCnt].Vertex.w,
				&vVector[jCnt].Nomal.x, &vVector[jCnt].Nomal.y, &vVector[jCnt].Nomal.z, &vVector[jCnt].Nomal.w,
				&vloadcolor.x, &vloadcolor.y, &vloadcolor.z, &vloadcolor.w,
				&vVector[jCnt].Texture.x, &vVector[jCnt].Texture.y,
				&textureNumber
			);

			//임시 무조건 1로 컬러값 출력(추후 컬러값 필요시 수정하여 사용)
			if (vloadcolor.x != 1.0f)
				vloadcolor.x = 1.0f;// vloadcolor.x / 255;
			if (vloadcolor.y != 1.0f)
				vloadcolor.y = 1.0f;//vloadcolor.y / 255;
			if (vloadcolor.z != 1.0f)
				vloadcolor.z = 1.0f;//vloadcolor.z / 255;
			if (vloadcolor.w != 1.0f)
				vloadcolor.w = 1.0f;//vloadcolor.w / 255;
			vVector[jCnt].Color.x = fabs(vloadcolor.x);
			vVector[jCnt].Color.y = fabs(vloadcolor.y);
			vVector[jCnt].Color.z = fabs(vloadcolor.z);
			vVector[jCnt].Color.w = fabs(vloadcolor.w);

			float weight4;
			int iobjnum1[4];
			int iobjnum2[4];
			int weightCount;
			_fgetts(m_pString, 256, m_pPsbFile);
			_stscanf_s(m_pString, _T("%d %d %d %d %d %f %f %f %f %d %d %d %d %f %f %f %f"),
				&weightCount,
				&iobjnum1[0], &iobjnum1[1], &iobjnum1[2], &iobjnum1[3],
				&vVector[jCnt].Weight1[0], &vVector[jCnt].Weight1[1], &vVector[jCnt].Weight1[2], &vVector[jCnt].Weight1[3],
				&iobjnum2[0], &iobjnum2[1], &iobjnum2[2], &iobjnum2[3],
				&vVector[jCnt].Weight2[0], &vVector[jCnt].Weight2[1], &vVector[jCnt].Weight2[2], &weight4
			);

			for (int iCnt = 0; iCnt < weightCount; iCnt++)
			{
				if (iCnt < 4)
				{
					vVector[jCnt].ObjNum1[iCnt] = (float)iobjnum1[iCnt];
				}
				else
				{
					vVector[jCnt].ObjNum2[iCnt - 4] = (float)iobjnum2[iCnt - 4];
				}
			}
			//가중치4번째는 1~3  = 1- 합산해서 나온값과 같기 때문에 weight 카운트로 사용
			vVector[jCnt].Weight2[3] = (float)weightCount;
		}
	}
}

//void LAPsbParser::LoadPNCT(PNCT5_VERTEX*& vVector, int iLoadCount, bool bBinary)
//{
//	vVector = new PNCT5_VERTEX[iLoadCount];
//
//	if (bBinary)
//	{
//		fread(vVector, sizeof(PNCT5_VERTEX) * iLoadCount, 1, m_pPsbFile);
//	}
//	else
//	{
//		int textureNumber;
//		// -10.5390     1.4553    18.2652     1.0000    -0.6986     0.5909     0.4036     1.0000     1.0000     1.0000     1.0000     1.0000     0.2895     0.0959 10
//		for (int jCnt = 0; jCnt < iLoadCount; jCnt++)
//		{
//			TVector4 vloadcolor;
//
//			_fgetts(m_pString, 256, m_pPsbFile);
//			_stscanf_s(m_pString, _T("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %d"),
//				&vVector[jCnt].Vertex.x, &vVector[jCnt].Vertex.y, &vVector[jCnt].Vertex.z, &vVector[jCnt].Vertex.w,
//				&vVector[jCnt].Nomal.x, &vVector[jCnt].Nomal.y, &vVector[jCnt].Nomal.z, &vVector[jCnt].Nomal.w,
//				&vloadcolor.x, &vloadcolor.y, &vloadcolor.z, &vloadcolor.w,
//				&vVector[jCnt].Texture.x, &vVector[jCnt].Texture.y,
//				&textureNumber
//			);
//
//			//임시 무조건 1로 컬러값 출력(추후 컬러값 필요시 수정하여 사용)
//			if (vloadcolor.x != 1.0f)
//				vloadcolor.x = 1.0f;// vloadcolor.x / 255;
//			if (vloadcolor.y != 1.0f)
//				vloadcolor.y = 1.0f;//vloadcolor.y / 255;
//			if (vloadcolor.z != 1.0f)
//				vloadcolor.z = 1.0f;//vloadcolor.z / 255;
//			if (vloadcolor.w != 1.0f)
//				vloadcolor.w = 1.0f;//vloadcolor.w / 255;
//			vVector[jCnt].Color.x = fabs(vloadcolor.x);
//			vVector[jCnt].Color.y = fabs(vloadcolor.y);
//			vVector[jCnt].Color.z = fabs(vloadcolor.z);
//			vVector[jCnt].Color.w = fabs(vloadcolor.w);
//
//			float weight4;
//			int iobjnum1[4];
//			int iobjnum2[4];
//			int weightCount;
//			_fgetts(m_pString, 256, m_pPsbFile);
//			_stscanf_s(m_pString, _T("%d %d %d %d %d %f %f %f %f %d %d %d %d %f %f %f %f"),
//				&weightCount,
//				&iobjnum1[0], &iobjnum1[1], &iobjnum1[2], &iobjnum1[3],
//				&vVector[jCnt].Weight1[0], &vVector[jCnt].Weight1[1], &vVector[jCnt].Weight1[2], &vVector[jCnt].Weight1[3],
//				&iobjnum2[0], &iobjnum2[1], &iobjnum2[2], &iobjnum2[3],
//				&vVector[jCnt].Weight2[0], &vVector[jCnt].Weight2[1], &vVector[jCnt].Weight2[2], &weight4
//			);
//
//			for (int iCnt = 0; iCnt < weightCount; iCnt++)
//			{
//				if (iCnt < 4)
//				{
//					vVector[jCnt].ObjNum1[iCnt] = (float)iobjnum1[iCnt];
//				}
//				else
//				{
//					vVector[jCnt].ObjNum2[iCnt - 4] = (float)iobjnum2[iCnt - 4];
//				}
//			}
//			//가중치4번째는 1~3  = 1- 합산해서 나온값과 같기 때문에 weight 카운트로 사용
//			vVector[jCnt].Weight2[3] = (float)weightCount;
//		}
//	}
//}

void LAPsbParser::LoadPNCT(std::vector<PNCT_VECTOR>& vVector, int iLoadCount)
{
	vVector.resize(iLoadCount);
	int textureNumber;

	// -10.5390     1.4553    18.2652     1.0000    -0.6986     0.5909     0.4036     1.0000     1.0000     1.0000     1.0000     1.0000     0.2895     0.0959 10
	for (int jCnt = 0; jCnt < iLoadCount; jCnt++)
	{
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%f %f %f %f %f %f %f %f %f %f %f %f %f %f %d"),
			&vVector[jCnt].Vertex.x, &vVector[jCnt].Vertex.y, &vVector[jCnt].Vertex.z, &vVector[jCnt].Vertex.w,
			&vVector[jCnt].Nomal.x, &vVector[jCnt].Nomal.y, &vVector[jCnt].Nomal.z, &vVector[jCnt].Nomal.w,
			&vVector[jCnt].Color.x, &vVector[jCnt].Color.y, &vVector[jCnt].Color.z, &vVector[jCnt].Color.w,
			&vVector[jCnt].Texture.x, &vVector[jCnt].Texture.y,
			&textureNumber
		);

	}
}


void LAPsbParser::LoadNodeData(LAMesh& pMesh, bool bBinary)
{
	if (bBinary)
	{
		StructObjectHeadData data;

		fread(&data, sizeof(StructObjectHeadData), 1, m_pPsbFile);
		int a = 10; 

		//Mesh Data Save
		pMesh.m_ObjectType = (E_ObjectType)data.ObjectType;
		pMesh.m_RootMaterialDataID = data.iMaterialNumber;

		/*====================== 바이너리 씬 matrix vecter 정보 출력 =======================*/
		_D3D_MATRIX matrixdata;
		fread(&matrixdata, sizeof(_D3D_MATRIX), 1, m_pPsbFile);
		fread(&pMesh.vMin, sizeof(TVector3), 1, m_pPsbFile);
		fread(&pMesh.vMax, sizeof(TVector3), 1, m_pPsbFile);
		pMesh.matLoadObjectWorld = matrixdata.d3dxmatrix();

		TCHAR NodeName[256] = {0};
		TCHAR ParentName[256] = { 0 };
		fread(NodeName, sizeof(TCHAR) * data.iCountNodeName, 1, m_pPsbFile);
		fread(ParentName, sizeof(TCHAR) * data.iCountParantName, 1, m_pPsbFile);
		pMesh.wsNodeName = NodeName;
		pMesh.wsParentName = ParentName;
	}
	else
	{
		int NumObject = 0;
		int TypeObject = 0;
		int NumRootMaterial = 0;

		_fgetts(m_pString, 256, m_pPsbFile);
		//#OBJECT 0 2 ROOT_MATERIAL_NUM -1
		_stscanf_s(m_pString, _T("%s %d %d %s %d"),
			m_pBuffer, _countof(m_pBuffer),
			&NumObject,
			&TypeObject,
			m_pBuffer, _countof(m_pBuffer),
			&NumRootMaterial
		);

		//Mesh Data Save
		pMesh.m_ObjectType = (E_ObjectType)TypeObject;
		pMesh.m_RootMaterialDataID = NumRootMaterial;

		//*NODE_NAME Dummy01
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%s %s"),
			m_pBuffer, _countof(m_pBuffer),
			m_pBuffer, _countof(m_pBuffer)
		);
		pMesh.wsNodeName = m_pBuffer;

		//*PARENT_NAME NULL
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%s %s"),
			m_pBuffer, _countof(m_pBuffer),
			m_pBuffer, _countof(m_pBuffer)
		);
		if (_tcsicmp(m_pBuffer, L"NULL") != 0)
		{
			pMesh.wsParentName = m_pBuffer;
		}

		//행렬저장
		TMatrix matworld;

		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%f %f %f %f"),
			&matworld._11, &matworld._12, &matworld._13, &matworld._14
		);
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%f %f %f %f"),
			&matworld._21, &matworld._22, &matworld._23, &matworld._24
		);
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%f %f %f %f"),
			&matworld._31, &matworld._32, &matworld._33, &matworld._34
		);
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%f %f %f %f"),
			&matworld._41, &matworld._42, &matworld._43, &matworld._44
		);

		pMesh.matLoadObjectWorld = matworld;

		//BoundingBox
		TVector3 min, max;
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%s"),
			m_pBuffer, _countof(m_pBuffer)
		);
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%f %f %f"),
			&min.x, &min.y, &min.z
		);
		_fgetts(m_pString, 256, m_pPsbFile);
		_stscanf_s(m_pString, _T("%f %f %f"),
			&max.x, &max.y, &max.z
		);
		pMesh.vMin = min;
		pMesh.vMax = max;
	}
}

void LAPsbParser::SaveNodeData(FILE* pFile, LAMesh& pMesh, int iMeshCount, bool bBinary)
{
	if (bBinary)
	{
		/*====================== 바이너리 씬 헤더 정보 저장 =======================*/
		StructObjectHeadData data;
		data.ObjectType = (int)pMesh.m_ObjectType;
		data.iCountNodeName = pMesh.wsNodeName.length();
		data.iCountParantName = pMesh.wsParentName.length();
		/*data.matNodeTM = pMesh.matLoadObjectWorld;
		data.vBoundingMin = pMesh.vMin;
		data.vBoundingMax = pMesh.vMax;*/
		LAMaterialData* MTlData = MATERIAL_MGR.GetRootMaterial(pMesh.m_RootMaterialDataID);
		if (MTlData)
		{
			data.iMaterialNumber = MTlData->iMaterialNumber;
		}
		else
		{
			data.iMaterialNumber = -1;
		}
		/*====================== 바이너리 씬 헤더 정보 출력 =======================*/
		fwrite(&data, sizeof(StructObjectHeadData), 1, m_pPsbFile);

		/*====================== 바이너리 씬 matrix vecter 정보 출력 =======================*/
		_D3D_MATRIX savematrix;
		savematrix = pMesh.matLoadObjectWorld;
		int isize = sizeof(savematrix);
		int imatrixSize = sizeof(_D3D_MATRIX);
		fwrite(&savematrix, sizeof(_D3D_MATRIX), 1, m_pPsbFile);
		fwrite(&pMesh.vMin, sizeof(TVector3), 1, m_pPsbFile);
		fwrite(&pMesh.vMax, sizeof(TVector3), 1, m_pPsbFile);

		/*====================== 바이너리 씬 네임 정보 출력 =======================*/
		fwrite(pMesh.wsNodeName.c_str(), sizeof(TCHAR) * data.iCountNodeName, 1, m_pPsbFile);
		fwrite(pMesh.wsParentName.c_str(), sizeof(TCHAR) * data.iCountParantName, 1, m_pPsbFile);
	}
	else
	{
		if (pMesh.m_ObjectType == E_ObjectType::Skin_Object)
		{
			_ftprintf(pFile, _T("\n\n%s %d %d %s %d"), _T("#OBJECT"), iMeshCount, (int)E_ObjectType::Geom_Default,
				_T("ROOT_MATERIAL_NUM"), pMesh.m_RootMaterialDataID);
		}
		else
		{
			_ftprintf(pFile, _T("\n\n%s %d %d %s %d"), _T("#OBJECT"), iMeshCount, (int)pMesh.m_ObjectType,
				_T("ROOT_MATERIAL_NUM"), pMesh.m_RootMaterialDataID);
		}

		_ftprintf(pFile, _T("\n\t%s %s"), _T("*NODE_NAME"), pMesh.wsNodeName.c_str());
		if (pMesh.wsParentName.empty())
		{
			_ftprintf(pFile, _T("\n\t%s %s"), _T("*PARENT_NAME"), L"NULL");
		}
		else
		{
			_ftprintf(pFile, _T("\n\t%s %s"), _T("*PARENT_NAME"), pMesh.wsParentName.c_str());
		}

		_ftprintf(pFile, _T("\n\t\t%10.4f %10.4f %10.4f %10.4f"), pMesh.matLoadObjectWorld._11, pMesh.matLoadObjectWorld._12,
			pMesh.matLoadObjectWorld._13, pMesh.matLoadObjectWorld._14);

		_ftprintf(pFile, _T("\n\t\t%10.4f %10.4f %10.4f %10.4f"), pMesh.matLoadObjectWorld._21, pMesh.matLoadObjectWorld._22,
			pMesh.matLoadObjectWorld._23, pMesh.matLoadObjectWorld._24);

		_ftprintf(pFile, _T("\n\t\t%10.4f %10.4f %10.4f %10.4f"), pMesh.matLoadObjectWorld._31, pMesh.matLoadObjectWorld._32,
			pMesh.matLoadObjectWorld._33, pMesh.matLoadObjectWorld._34);

		_ftprintf(pFile, _T("\n\t\t%10.4f %10.4f %10.4f %10.4f"), pMesh.matLoadObjectWorld._41, pMesh.matLoadObjectWorld._42,
			pMesh.matLoadObjectWorld._43, pMesh.matLoadObjectWorld._44);

		//BoundingBox
		_ftprintf(pFile, _T("\n\t%s"), _T("*BOUNDING_BOX"));
		_ftprintf(pFile, _T("\n\t\t%f %f %f"),
			pMesh.vMin.x,
			pMesh.vMin.y,
			pMesh.vMin.z
		);
		_ftprintf(pFile, _T("\n\t\t%f %f %f"),
			pMesh.vMax.x,
			pMesh.vMax.y,
			pMesh.vMax.z
		);
	}
}

LALoadObject* LAPsbParser::Convert(int iCodeIndex, std::wstring CodeName, std::wstring wsParentMatrixName, E_LoadObjectType eLoadObjectType, CrateObjectDest* pMatrixDest, CrateObjectDest* pSkinDest, bool bLoadShadow)
{
	switch ((int)eLoadObjectType)
	{
		case (int)E_LoadObjectType::CHARACTER_MATRIX:
		{
			LAResource CharacterData;
			CharacterData.m_ResourceCodeName = CodeName;
			CharacterData.m_ResourceParentName = wsParentMatrixName;
			//CharacterData.m_CharacterSceneData = m_pMeshList[0].m_SceneDataList;

			for (int iObjCnt = 0; iObjCnt < (int)m_pMeshList.size(); iObjCnt++)
			{
				LABoneObject* pBoneObject = new LABoneObject;
				pBoneObject->m_MeshData = new LAMesh;

				*pBoneObject->m_MeshData = m_pMeshList[iObjCnt];
				CharacterData.m_BoneObjectList.push_back(pBoneObject);
			}
			//부모노드찾아서 등록
			SearchToObjectParent(CharacterData);
			//캐릭터 매니저에 등록
			RESOURCE_MGR.AddResource(CharacterData, pMatrixDest, pSkinDest);
			//Texture Update
			MATERIAL_MGR.ConvertTextureResource(m_pCoreDevice);

			return NULL;
		}break;

		case (int)E_LoadObjectType::CHARACTER_SKIN:
		{
			bool bNewResource = false;

			LAResource* pResource = RESOURCE_MGR.GetResourceAdress(CodeName);

			if (pResource == NULL)
			{
				pResource = new LAResource;
				pResource->m_ResourceCodeName = CodeName;
				pResource->m_ResourceParentName = wsParentMatrixName;
				
				bNewResource = true;
			}

			for (int iObjCnt = 0; iObjCnt < (int)m_pMeshList.size(); iObjCnt++)
			{

				LASkinObject* pSkinObject = new LASkinObject;
				pSkinObject->m_MeshData = new LAMesh;

				*pSkinObject->m_MeshData = m_pMeshList[iObjCnt];
				pResource->m_SkinObjectList.push_back(pSkinObject);

				CheckObject Renderobj;
				Renderobj.pSkinObject = pSkinObject;
				pResource->m_SkinRenderList.push_back(Renderobj);
			}
			if (bNewResource == false)
			{
				pResource->m_bIsSkining = m_bIsSkining;
				//부모노드찾아서 등록
				SearchToObjectParent(*pResource);
				int iMatrixIndex = RESOURCE_MGR.GetResourceCodeNameIndex(CodeName);
				RESOURCE_MGR.CreateResource(iMatrixIndex, pMatrixDest, pSkinDest, bLoadShadow);
			}
			else
			{
				pResource->m_bIsSkining = m_bIsSkining;
				if (!RESOURCE_MGR.AddResource(*pResource, pMatrixDest, pSkinDest))
				{
					return NULL;
				}
			}


			MATERIAL_MGR.ConvertTextureResource(m_pCoreDevice);

			return NULL;
		}break;

		case (int)E_LoadObjectType::CHARACTER_ANIMATION:
		{
			LAResource* pResource = RESOURCE_MGR.GetResourceAdress(CodeName);
			if (pResource == NULL)
				return NULL;

			//캐릭터리소스에 애니메이션 아이디등록
			int bonecount = pResource->m_BoneObjectList.size();
			int meshcount = m_pMeshList.size();
			if ((bonecount != 0) && (m_pMeshList.size() != 0))
			{
				for (int iResourceCnt = 0; iResourceCnt < bonecount; iResourceCnt++)
				{
					for (int iMeshCnt = 0; iMeshCnt < meshcount; iMeshCnt++)
					{
						if (pResource->m_BoneObjectList[iResourceCnt]->m_MeshData->wsNodeName.compare(m_pMeshList[iMeshCnt].wsNodeName) == 0)
						{
							if ((int)m_pMeshList[iMeshCnt].m_AnimationIDList.size() != 0)
							{
								pResource->m_BoneObjectList[iResourceCnt]->m_MeshData->m_AnimationIDList.push_back(m_pMeshList[iMeshCnt].m_AnimationIDList[0]);
								pResource->m_BoneObjectList[iResourceCnt]->m_MeshData->m_SceneDataList.push_back(m_SceneData);
								break;
							}
							else
							{
								pResource->m_BoneObjectList[iResourceCnt]->m_MeshData->m_AnimationIDList.push_back(-1);
								pResource->m_BoneObjectList[iResourceCnt]->m_MeshData->m_SceneDataList.push_back(m_SceneData);
								break;
							}
						}
					}
				}
			}

			//캐릭터리소스 모션리스트에 씬정보등록
			pResource->m_CharacterSceneData = m_SceneData;
			pResource->m_AnimationList.push_back(m_SceneData);

			int a = 10;
		}break;
	}	//switch

	//로드된 리소스 이미지 업데이트
	MATERIAL_MGR.ConvertTextureResource(m_pCoreDevice);

	return NULL;
}

bool LAPsbParser::Save(const TCHAR* pFileName, LALoadObject& LoadObjectdata, E_ObjectType ObjectType, bool bBinary)
{
	std::vector<LAMesh*> SaveMeshDataList;



	switch ((int)ObjectType)
	{
	case (int)E_ObjectType::Geom_Biped:
	case (int)E_ObjectType::Helper_Dummy:
	case (int)E_ObjectType::Helper_Bone:
	{
		//세이브파일이름 곧 매트릭스이름 등록
		TCHAR drive[256] = { 0 };
		TCHAR dir[256] = { 0 };
		TCHAR filename[256] = { 0 };
		TCHAR ext[256] = { 0 };
		_tsplitpath_s(pFileName, drive, _countof(drive), dir, _countof(dir), filename, _countof(filename), ext, _countof(ext));
		//LoadObjectdata.m_MatrixName = filename;

		LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(0);
		int iMeshSize = 0; 
		if (pResourceData)
		{
			iMeshSize = pResourceData->m_BoneObjectList.size();
		}

		if (iMeshSize == 0)
		{
			return true;
		}
		//씬 정보 기록
		SaveSceneData(m_pPsbFile, LoadObjectdata, iMeshSize, E_ObjectType::Geom_Biped, bBinary);
		for (int iCnt = 0; iCnt < iMeshSize; iCnt++)
		{
			SaveMeshDataList.push_back(pResourceData->m_BoneObjectList[iCnt]->m_MeshData);
		}
		//나머지 정보 기록
		SaveData(m_pPsbFile, SaveMeshDataList, E_ObjectType::Geom_Biped, bBinary);
	}break;

	case (int)E_ObjectType::Skin_Object:
	{
		/*if (LoadObjectdata.m_NewLoadObject->m_pMainResource->m_MatrixName.empty())
		{
		LoadObjectdata.m_MatrixName = L"NULL";
		}*/

		LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(0);
		int iMeshSize = 0;
		if (pResourceData)
		{
			iMeshSize = pResourceData->m_SkinRenderList.size();
		}

		if (iMeshSize == 0)
		{
			return true;
		}

		for (int iCnt = 0; iCnt < iMeshSize; iCnt++)
		{
			//SKin Form 에서 선택된 스킨만 저장
			if (pResourceData->m_SkinRenderList[iCnt].bisRander)
			{
				SaveMeshDataList.push_back(pResourceData->m_SkinRenderList[iCnt].pSkinObject->m_MeshData);
			}
			//SaveMeshDataList.push_back(LoadObjectdata.m_SkinObjectList[iCnt]->m_MeshData);
		}
		//Save Mesh Data List Output
		int NumSelectSkin = SaveMeshDataList.size();
		SaveSceneData(m_pPsbFile, LoadObjectdata, NumSelectSkin, E_ObjectType::Skin_Object, bBinary);
		SaveData(m_pPsbFile, SaveMeshDataList, E_ObjectType::Skin_Object, bBinary);
	}break;

	case (int)E_ObjectType::Animation:
	{

		int AniCount = LoadObjectdata.m_SaveAnimationDataList.size();
		if (AniCount <= 0)
			break;

		//경로분해						
		TCHAR drive[256] = { 0 };
		TCHAR dir[256] = { 0 };
		TCHAR filename[256] = { 0 };
		TCHAR ext[256] = { 0 };
		_tsplitpath_s(pFileName, drive, _countof(drive), dir, _countof(dir), filename, _countof(filename), ext, _countof(ext));

		//세이브파일이름 곧 매트릭스이름 등록
		std::wstring SaveFileName;
		SaveFileName += drive;
		SaveFileName += dir;

		//저장할 데이터 체크
		LAResource* pResourceData = RESOURCE_MGR.GetResourceAdress(0);
		int iMeshSize = 0;
		if (pResourceData)
		{
			iMeshSize = pResourceData->m_BoneObjectList.size();
		}
		if (iMeshSize == 0)
			return true;

		for (int iCnt = 0; iCnt < iMeshSize; iCnt++)
		{
			SaveMeshDataList.push_back(pResourceData->m_BoneObjectList[iCnt]->m_MeshData);
		}

		//애니메이션 카운트만큼 파일 저장(애니메이션 이름으로 저장)
		;
		for (LoadObjectdata.m_SaveObjecDatatItor = LoadObjectdata.m_SaveAnimationDataList.begin();
			LoadObjectdata.m_SaveObjecDatatItor != LoadObjectdata.m_SaveAnimationDataList.end();
			LoadObjectdata.m_SaveObjecDatatItor++)
		{
			std::wstring fullpath = SaveFileName;
			fullpath += LoadObjectdata.m_SaveObjecDatatItor->SaveFileName;
			fullpath += ext;

			FILE* m_pAniFile;

			if (bBinary)
			{
				//File Open
				_tfopen_s(&m_pAniFile, fullpath.c_str(), _T("wb"));
				if (m_pAniFile == NULL)
					return false;
			}
			else
			{
				//File Open
				_tfopen_s(&m_pAniFile, fullpath.c_str(), _T("wt"));
				if (m_pAniFile == NULL)
					return false;
			}

			//씬 정보 기록
			SaveSceneData(m_pAniFile, LoadObjectdata, iMeshSize, E_ObjectType::Animation, bBinary);
			//나머지 정보 기록
			for (int iCnt = 0; iCnt < iMeshSize; iCnt++)
			{
				if (bBinary)
				{

				}
				else
				{
					_ftprintf(m_pAniFile, _T("\n"));
					_ftprintf(m_pAniFile, _T("\n%s %d"), _T("#OBJECT"), iCnt);
				}
				SaveAnimationData(m_pAniFile, LoadObjectdata, *SaveMeshDataList[iCnt], bBinary);
			}

			fclose(m_pAniFile);
		}
	}break;
	}


	return true;
}

bool LAPsbParser::SaveData(FILE* pFile, std::vector<LAMesh*>& SaveMeshList, E_ObjectType objectType, bool bBinary)
{
	switch ((int)objectType)
	{
	case (int)E_ObjectType::Animation:
	{

	}break;

	default:
	{
		for (int iCnt = 0; iCnt < (int)SaveMeshList.size(); iCnt++)
		{
			SaveNodeData(pFile, *SaveMeshList[iCnt], iCnt, bBinary);
			SaveMaterialData(pFile, *SaveMeshList[iCnt], objectType, bBinary);
			SaveMeshData(pFile, *SaveMeshList[iCnt], objectType, bBinary);
			//SaveAnimationData(*SaveMeshList[iCnt]);
		}

	}break;
	}

	return true;
}

bool LAPsbParser::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pCoreDevice = pDevice;
	m_pCoreContext = pContext;

	return true;
}

bool LAPsbParser::Release()
{
	m_pPsbFile = NULL;

	m_pMeshList.clear();
	ZeroMemory(&m_SceneData, sizeof(SceneData));

	return true;
}

LAPsbParser::~LAPsbParser()
{
}
