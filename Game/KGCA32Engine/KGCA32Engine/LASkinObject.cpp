

//#include "stdafx.h"


#include "LASkinObject.h"

#include "CoreFuncLink.h"

#include "LAMaterialManager.h"
#include "LAMeshManager.h"
#include "LAAnimationManager.h"
#include "LALoadObjectManager.h"



struct HS_TESSELLATION_BUFFERType
{
	float		tessellationAmount;
	TVector3 padding;
};


void LASkinObject::SetCrrentTick(bool check)
{
	m_bSetParentTick = check;
}

void LASkinObject::SetBoneMatrices(CBConstBoneWorld* pCBboneWorldDataList)
{
	for (int iCnt = 0; iCnt < 255; iCnt++)
	{
		m_cbMatrixListData[iCnt] = pCBboneWorldDataList->g_mConstBoneWorld[iCnt];
	}

}

void LASkinObject::SetSelectObject(bool bcheck)
{
	m_SelectObject = bcheck;
}

void LASkinObject::SetDirectionLight(TVector4 vCameraPos, TVector4 vLigthPos, TVector4 vLightDirection)
{
	m_vCameraPos = vCameraPos;
	m_vLightPos = vLigthPos;
	m_vLightDirection = vLightDirection;
}

void  LASkinObject::CreateTangent(LAMeshData* pMeshData)
{
	if (pMeshData)
	{
		for (int iCnt = 0; iCnt < pMeshData->m_SubIndexList.size(); iCnt++)
		{
			int iSubSize = pMeshData->m_SubIndexList[iCnt].size();
			for (int jCnt = 0; jCnt < iSubSize; jCnt += 3)
			{
				TVector3 vTangent;
				TVector3 vBiNormal;
				TVector3 vNormal;

				int iIndexN0 = pMeshData->m_SubIndexList[iCnt][jCnt + 0];
				int iIndexN1 = pMeshData->m_SubIndexList[iCnt][jCnt + 1];
				int iIndexN2 = pMeshData->m_SubIndexList[iCnt][jCnt + 2];
				PNCT5_VERTEX& r0 = pMeshData->m_SubSkinUniqueVecterList[iCnt][iIndexN0];
				PNCT5_VERTEX& r1 = pMeshData->m_SubSkinUniqueVecterList[iCnt][iIndexN1];
				PNCT5_VERTEX& r2 = pMeshData->m_SubSkinUniqueVecterList[iCnt][iIndexN2];


				CreateTangentSpaceVectors(&TVector3(r0.Vertex.x, r0.Vertex.y, r0.Vertex.z),
					&TVector3(r1.Vertex.x, r1.Vertex.y, r1.Vertex.z),
					&TVector3(r2.Vertex.x, r2.Vertex.y, r2.Vertex.z),
					r0.Texture,
					r1.Texture,
					r2.Texture,
					&vTangent,
					&vBiNormal,
					&vNormal);
				//r0.Nomal = TVector4(vNormal, 1.0f);
				r0.vTangent = TVector4(vTangent.x, vTangent.y, vTangent.z, 1.0f);
				//r1.Nomal = TVector4(vNormal, 1.0f);
				r1.vTangent = TVector4(vTangent.x, vTangent.y, vTangent.z, 1.0f);
				//r2.Nomal = TVector4(vNormal, 1.0f);
				r2.vTangent = TVector4(vTangent.x, vTangent.y, vTangent.z, 1.0f);
			}
		}
	}
}

void  LASkinObject::CreateTangentSpaceVectors(TVector3 *v0,
	TVector3 *v1,
	TVector3 *v2,
	TVector2 uv0,
	TVector2 uv1,
	TVector2 uv2,
	TVector3 *vTangent,
	TVector3 *vBiNormal,
	TVector3 *vNormal)
{
	TVector3 vEdge1 = *v1 - *v0;
	TVector3 vEdge2 = *v2 - *v0;
	D3DXVec3Normalize(&vEdge1, &vEdge1);
	D3DXVec3Normalize(&vEdge2, &vEdge2);
	// UV delta
	TVector2 deltaUV1 = uv1 - uv0;
	TVector2 deltaUV2 = uv2 - uv0;
	D3DXVec2Normalize(&deltaUV1, &deltaUV1);
	D3DXVec2Normalize(&deltaUV2, &deltaUV2);


	TVector3 biNormal;
	float fDet = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	if (fabsf(fDet) < 1e-6f)
	{
		*vTangent = TVector3(1.0f, 0.0f, 0.0f);
		biNormal = TVector3(0.0f, 0.0f, 1.0f);
	}
	else
	{
		*vTangent = (vEdge1 * deltaUV2.y - vEdge2 * deltaUV1.y)*fDet;
		biNormal = (vEdge2 * deltaUV1.x - vEdge1 * deltaUV2.x)*fDet;
	}
	D3DXVec3Normalize(vTangent, vTangent);
	D3DXVec3Normalize(&biNormal, &biNormal);

	D3DXVec3Cross(vBiNormal, vNormal, vTangent);
	float crossinv = (D3DXVec3Dot(vBiNormal, &biNormal) < 0.0f) ? -1.0f : 1.0f;
	*vBiNormal *= crossinv;
}

void LASkinObject::SetBoneObjectCalculation(TMatrix matWorld)
{
	m_matBonObjectCalculation = matWorld;
}

bool LASkinObject::GetAnimationTrack(std::vector<AnimationTrack>& TrackList, int& NumstartTrack, int& NumEndTrack, float fCurTick)
{
	//이동
	int iCnt = 0;
	bool isSerchStart = false;
	bool isSerchEnd = false;


	//End 검색 입력
	for (iCnt = 0; iCnt < (int)TrackList.size(); iCnt++)
	{
		if (TrackList[iCnt].Tick >= fCurTick)
		{
			NumEndTrack = iCnt;

			if ((TrackList[iCnt].Tick > fCurTick) && (iCnt == 0))
			{
				NumEndTrack = -1;
				NumstartTrack = -1;

				return true;
			}

			isSerchEnd = true;
			break;
		}
	}
	if (isSerchEnd == false)
	{
		NumEndTrack = (int)TrackList.size() - 1;
	}

	//Start Serch
	for (iCnt = 0; iCnt < (int)TrackList.size(); iCnt++)
	{
		if (TrackList[iCnt].Tick <= fCurTick)
		{
			NumstartTrack = iCnt;

			isSerchStart = true;
		}
		else
		{
			break;
		}
	}
	if (isSerchStart == false)
	{
		NumstartTrack = 0;
	}



	return true;
}

void LASkinObject::UpDateNormals()
{
	std::vector<std::vector<TVector3>> m_FaceNormalList;
	std::vector<std::vector<TVector3>>& ref = m_FaceNormalList;
	std::vector<std::vector<int>> m_NormalLookUpTable;
	LAMeshData* pData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);

	if (pData)
	{
		int iSubSize = pData->m_SubSkinUniqueVecterList.size();

		m_FaceNormalList.resize(iSubSize);
		m_NormalLookUpTable.resize(iSubSize);
		for (int iCnt = 0; iCnt < iSubSize; iCnt++)
		{
			int iFaceSize = pData->m_SubIndexList[iCnt].size() / 3;
			//================================================================================
			//	** 페이스 노말 구축 **
			//================================================================================
			InitFaceNormals(ref[iCnt], iFaceSize, pData->m_SubSkinUniqueVecterList[iCnt], pData->m_SubIndexList[iCnt]);

			//================================================================================
			//	** 정점 룩업 테이블 생성 **
			//================================================================================
			GenNormalLookupTable(m_NormalLookUpTable[iCnt], iFaceSize, pData->m_SubIndexList[iCnt]);

			//================================================================================
			//	** 정점 노말 계산 **
			//================================================================================
			CalcPerVertexNormalsFastLookup(pData->m_SubSkinUniqueVecterList[iCnt], m_FaceNormalList[iCnt], m_NormalLookUpTable[iCnt]);
		}
	}
}

void LASkinObject::InitFaceNormals(std::vector<TVector3>& rSaveFaceNormalList, int iFaceSize, std::vector<PNCT5_VERTEX>& rVertexList, std::vector<DWORD>& rIndexList)
{
	//================================================================================
	//	** 페이스 노말 구축 **						 트라이앵글 페이스 노말벡터 구하기
	//================================================================================
	int iFaceCount = 0;
	rSaveFaceNormalList.resize(iFaceSize);

	for (int iHeight = 0; iHeight < iFaceSize * 3; iHeight += 3)
	{
		DWORD index0 = rIndexList[iHeight + 0];
		DWORD index1 = rIndexList[iHeight + 1];
		DWORD index2 = rIndexList[iHeight + 2];

		TVector3 vector0 = TVector3(rVertexList[index0].Vertex.x, rVertexList[index0].Vertex.y, rVertexList[index0].Vertex.z);
		TVector3 vector1 = TVector3(rVertexList[index1].Vertex.x, rVertexList[index1].Vertex.y, rVertexList[index1].Vertex.z);
		TVector3 vector2 = TVector3(rVertexList[index2].Vertex.x, rVertexList[index2].Vertex.y, rVertexList[index2].Vertex.z);

		//내적
		TVector3 E0vector = vector1 - vector0;
		TVector3 E1vector = vector2 - vector0;

		//두 벡터의 외적
		D3DXVec3Cross(&rSaveFaceNormalList[iFaceCount], &E0vector, &E1vector);
		//정규화
		D3DXVec3Normalize(&rSaveFaceNormalList[iFaceCount], &rSaveFaceNormalList[iFaceCount]);

		iFaceCount++;
	}
}

void LASkinObject::GenNormalLookupTable(std::vector<int>& rNormalLookUpTable, int iFaceSize, std::vector<DWORD>& rIndexList)
{
	//================================================================================
	//	** 정점 룩업 테이블 생성 **						각 페이스 당 공유 정점 체크
	//================================================================================
	rNormalLookUpTable.resize(iFaceSize * 3 * 6);	//최대 중복 6개
	memset(&rNormalLookUpTable.at(0), -1, sizeof(int) * rNormalLookUpTable.size());

	for (int iCnt = 0; iCnt < iFaceSize; iCnt++)	//버텍스갯수만큼
	{
		for (int jCnt = 0; jCnt < 3; jCnt++)	//한 명의 정점
		{
			for (int kCnt = 0; kCnt < 6; kCnt++)	//공유 정점 체크
			{
				int LookIndex = rIndexList[(iCnt * 3) + jCnt];	//인덱스 리스트에서 버텍스 넘버 가져오기

				if (rNormalLookUpTable[(LookIndex * 6) + kCnt] == -1)
				{
					rNormalLookUpTable[(LookIndex * 6) + kCnt] = iCnt;		//공유 페이스 기록 
					break;	//1번만 기록
				}
			}
		}
	}
}

void LASkinObject::CalcPerVertexNormalsFastLookup(std::vector<PNCT5_VERTEX>& rSaveNormalVertexList, std::vector<TVector3>& rFaceNormal, std::vector<int>& rNormalLookUpTable)
{
	//================================================================================
	//	** 정점 노말 계산 **		룩업 테이블 노말값을 합산 하여 다시 공유 공점 노말 계산
	//================================================================================
	for (int iVertexCnt = 0; iVertexCnt < (int)rSaveNormalVertexList.size(); iVertexCnt++)
	{
		TVector3 avgNormal(0.0f, 0.0f, 0.0f);

		for (int iLookUpCnt = 0; iLookUpCnt < 6; iLookUpCnt++)
		{
			int LookUpIndex = (iVertexCnt * 6) + iLookUpCnt;
			if (rNormalLookUpTable[LookUpIndex] != -1)
			{
				avgNormal += rFaceNormal[rNormalLookUpTable[LookUpIndex]];
			}
			else
				break;
		}
		D3DXVec3Normalize(&avgNormal, &avgNormal);
		rSaveNormalVertexList[iVertexCnt].Nomal = TVector4(avgNormal.x, avgNormal.y, avgNormal.z, 1.0f);
	}

}


HRESULT LASkinObject::CreateConstantBuffer()
{
	HRESULT hr = S_OK;

	LAObject2::CreateConstantBuffer();

	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	//MatrixObjectList
	cbDesc.ByteWidth = sizeof(TMatrix) * 50;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	hr = m_pCoreDevice->CreateBuffer(&cbDesc, NULL, m_pCBConstBoneBuffer.GetAddressOf());
	if (!SUCCEEDED(hr))
	{
		return false;
	}


	/*============ 인스턴싱 애니메이션 상수 버퍼 생성==============*/
	//D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	//MatrixObjectList
	cbDesc.ByteWidth = sizeof(TMatrix) * g_iMaxRenderMonsterCount * g_iMaxBoneMatrixCount;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	hr = m_pCoreDevice->CreateBuffer(&cbDesc, NULL, m_pInstancingAnimationBuffer.GetAddressOf());
	if (!SUCCEEDED(hr))
	{
		return false;
	}

	/*============ 인스턴싱 노말에사용될 행렬 상수 버퍼 생성 ==============*/
	//D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	//MatrixObjectList
	cbDesc.ByteWidth = sizeof(TMatrix) * g_iMaxRenderMonsterCount;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.MiscFlags = 0;
	hr = m_pCoreDevice->CreateBuffer(&cbDesc, NULL, m_pInstancingNormalBuffer.GetAddressOf());
	if (!SUCCEEDED(hr))
	{
		return false;
	}


	return S_OK;
}


HRESULT LASkinObject::CreateLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA , 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA , 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D11_INPUT_PER_VERTEX_DATA , 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 88, D3D11_INPUT_PER_VERTEX_DATA , 0 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 104, D3D11_INPUT_PER_VERTEX_DATA , 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 120, D3D11_INPUT_PER_VERTEX_DATA , 0 }
	};
	UINT iSize = sizeof(layout) / sizeof(layout[0]);

	if (FAILED(m_pCoreDevice->CreateInputLayout(layout, iSize, pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), &m_pVertexLayout)))
	{
		TCHAR ErrorMsg[255] = L"Error CreateLayout";

		OutputDebugString(ErrorMsg);
	}

	return S_OK;
}

HRESULT LASkinObject::LoadShader()
{
	HRESULT hr;

	if (GetNormalMapping() == false)
	{
		hr = LAObject2::LoadShader();
		if (hr != S_OK)
		{
			return S_FALSE;
		}
	}



	return hr;
}


HRESULT LASkinObject::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	//Helper Object Return
	if (m_MeshData->m_ObjectType == E_ObjectType::Helper_Dummy)
		return S_OK;

	if (m_MeshData->m_ObjectType == E_ObjectType::Helper_Bone)
		return S_OK;

	LAMeshData* pMeshData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);
	if (pMeshData)
	{
		//서브매터리얼 조건 검사후 생성
		pMeshData->m_SubVertexBufferList.resize(pMeshData->m_SubSkinUniqueVecterList.size());
		for (int iCnt = 0; iCnt < (int)pMeshData->m_SubSkinUniqueVecterList.size(); iCnt++)
		{
			if (pMeshData->m_SubSkinUniqueVecterList[iCnt].size() <= 0)
				continue;

			//if (GetNormalMapping() == true)
			//{
			//	/*======================== 정점 노말 생성 ========================*/
			//	UpDateNormals();
			//	/*======================== 노말맵핑 법선벡터 생성 ========================*/
			//	CreateTangent(pMeshData);
			//}

			/*======================== 버텍스 버퍼 생성 ========================*/
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(PNCT5_VERTEX) * pMeshData->m_SubSkinUniqueVecterList[iCnt].size();
			int isize = sizeof(PNCT5_VERTEX);
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = (void*)&pMeshData->m_SubSkinUniqueVecterList[iCnt].at(0);

			hr = m_pCoreDevice->CreateBuffer(&bd, &InitData, pMeshData->m_SubVertexBufferList[iCnt].GetAddressOf());
			if (!SUCCEEDED(hr))
			{
				//ERROR
				return false;
			}
		}
	}

	return S_OK;
}

HRESULT LASkinObject::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

	//Helper Object Return
	if (m_MeshData->m_ObjectType == E_ObjectType::Helper_Dummy)
		return S_OK;

	if (m_MeshData->m_ObjectType == E_ObjectType::Helper_Bone)
		return S_OK;


	LAMeshData* pMeshData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);

	pMeshData->m_SubIndexBufferList.resize(pMeshData->m_SubIndexList.size());
	for (int iCnt = 0; iCnt < (int)pMeshData->m_SubIndexList.size(); iCnt++)
	{
		if (pMeshData->m_SubIndexList[iCnt].size() <= 0)
			continue;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(DWORD) * pMeshData->m_SubIndexList[iCnt].size();
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
		InitData.pSysMem = (void*)&pMeshData->m_SubIndexList[iCnt].at(0);

		hr = m_pCoreDevice->CreateBuffer(&bd, &InitData, pMeshData->m_SubIndexBufferList[iCnt].GetAddressOf());
		if (!SUCCEEDED(hr))
		{
			//ERROR
			return false;
		}
	}

	return S_OK;
}

HRESULT LASkinObject::LoadTextureResource()
{
	HRESULT hr = S_OK;

	LAMeshData* pMeshData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);
	if (pMeshData)
	{
		LAMaterialData* pRootMtl = MATERIAL_MGR.GetRootMaterial(m_MeshData->m_RootMaterialDataID);
		if (pRootMtl != NULL)
		{
			for (int jCnt = 0; jCnt < pRootMtl->m_SubMaterialArray.size(); jCnt++)
			{
				SubMaterialData* pSub = MATERIAL_MGR.GetSubMaterial(m_MeshData->m_RootMaterialDataID, pMeshData->m_SubTextureNumber[jCnt]);
				if (pSub != NULL)
				{
					for (int iSearchNormla = 0; iSearchNormla < pSub->MapData.size(); iSearchNormla++)
					{

						if (pSub->MapData[iSearchNormla].iTypeNumber == 2)
						{
							//CreateNormalMapVertexBuffer();
							//LoadNormalShader();

							//SetNormalMapping(true);
						}

#ifdef TOOL_MODE
						std::wstring dir = L"../data/texture/";
#else
						std::wstring dir = L"../../data/CharacterLoadFile/texture/";
#endif
						dir += pSub->MapData[iSearchNormla].pBitmapName;

						string ext = GetFileExtension((TCHAR*)dir.c_str());
						pSub->MapData[iSearchNormla].texture.LoadTexture(dir, mtw(ext));


					}
					return S_OK;
				}
			}
		}
	}

	return S_OK;
}

bool LASkinObject::Frame()
{
	LAObject2::Frame();

	if ((int)m_MeshData->m_AnimationIDList.size() != 0)
	{
		if (m_bSetParentTick == false)
		{
			//Interpolate(m_ObjectWorld, GetCurrentAnimationTick(&m_MeshData->m_SceneDataList, m_SkinTick));
		}
		else
		{
			Interpolate(m_ObjectWorld, m_iParentTick);
		}

	}
	else
	{
		if (m_pmatParent != NULL)
		{
			m_ObjectWorld = m_matChildWorld;
		}
		else
		{
			m_ObjectWorld = m_MeshData->matLoadObjectWorld;
		}
	}

	if (m_pmatParent != NULL)
	{
		TMatrix temp = *m_pmatParent;

		m_matCalculation = m_ObjectWorld * temp;
	}
	else
	{
		TMatrix identity;

		D3DXMatrixIdentity(&identity);
		m_matCalculation = m_ObjectWorld * identity;
	}

	if (m_pmatCharacter != NULL)
	{
		D3DXMatrixMultiply(&m_matCalculation, &m_matCalculation, m_pmatCharacter);
	}

	return true;
}




bool LASkinObject::SetAnimationTrack(float fCurTick)
{
	float fLerpTime = 0.0f;
	int iNumstartTrack = 0;
	int iNumEndTrack = 0;

	TMatrix matCWM = m_MeshData->matLoadObjectWorld;
	TMatrix matLocalWorld;
	TVector3 vCWMscale;
	TQuaternion qCWMrotation;
	TVector3 vCWMtransration;

	//부모가 있을경우
	if (m_pmatParent != NULL)
	{
		matCWM = m_matChildWorld;
	}
	//자식행렬 분해
	D3DXMatrixDecompose(&vCWMscale, &qCWMrotation, &vCWMtransration, &matCWM);

	//================================================================================
	//	** AnimatinScale ** 
	//================================================================================
	TVector3 vScale;
	TQuaternion qScale;
	TMatrix matScale;
	TMatrix matScaleInverse;
	TVector3 vAxis;
	float fAngle = 0.0f;

	LAAnimaionData* pAniData = ANIMATION_MGR.GetAnimaion(m_MeshData->m_AnimationIDList[m_iCurrentMotion]);

	if (pAniData != NULL)
	{

		if (pAniData->AnimationTrack[(int)E_AnimationType::AnimatinScale].size() != 0)
		{
			//start,end Tick serch
			GetAnimationTrack(pAniData->AnimationTrack[(int)E_AnimationType::AnimatinScale], iNumstartTrack, iNumEndTrack, (float)fCurTick);

			if (iNumEndTrack == -1)
			{
				D3DXMatrixScaling(&m_matObjectLocalScale, vCWMscale.x, vCWMscale.y, vCWMscale.z);
			}
			else
			{
				//Tick값 계산
				fLerpTime = 0.0f;
				if (iNumstartTrack != iNumEndTrack)
				{
					fLerpTime = (float)(fCurTick - pAniData->AnimationTrack[(int)E_AnimationType::AnimatinScale][iNumstartTrack].Tick) /
						(pAniData->AnimationTrack[(int)E_AnimationType::AnimatinScale][iNumEndTrack].Tick - pAniData->AnimationTrack[(int)E_AnimationType::AnimatinScale][iNumstartTrack].Tick);
				}

				//스케일값 보간값
				D3DXVec3Lerp(&vScale, &pAniData->AnimationTrack[(int)E_AnimationType::AnimatinScale][iNumstartTrack].Pos,
					&pAniData->AnimationTrack[(int)E_AnimationType::AnimatinScale][iNumEndTrack].Pos, fLerpTime);
				//행렬에 스케일값 적용
				D3DXMatrixScaling(&m_matObjectLocalScale, vScale.x, vScale.y, vScale.z);

				//임의의값 보간값 얻어오기
				D3DXQuaternionSlerp(&qScale, &pAniData->AnimationTrack[(int)E_AnimationType::AnimatinScale][iNumstartTrack].Axis, &pAniData->AnimationTrack[(int)E_AnimationType::AnimatinScale][iNumEndTrack].Axis, fLerpTime);
				//임의의값 쿼터니언 벡터변경
				vAxis = TVector3(qScale.x, qScale.y, qScale.z);
				fAngle = qScale.w;
				//임의의값 행렬과 그 역행렬 구하기
				D3DXMatrixRotationAxis(&matScale, &vAxis, fAngle);
				D3DXMatrixInverse(&matScaleInverse, NULL, &matScale);

				//Local 스케일 행렬에 저장 : 임의의축 역행렬 * 기존스케일값 * 임의의축 행렬값
				m_matObjectLocalScale = matScaleInverse * m_matObjectLocalScale * matScale;
			}

		}
		else
		{
			D3DXMatrixScaling(&m_matObjectLocalScale, vCWMscale.x, vCWMscale.y, vCWMscale.z);
		}


		//================================================================================
		//	** AnimatinRotation ** 
		//================================================================================	
		if (pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation].size() != 0)
		{
			TQuaternion RotationQuternion;

			//start,end Tick serch
			GetAnimationTrack(pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation], iNumstartTrack, iNumEndTrack, (float)fCurTick);

			if (iNumEndTrack == -1)
			{
				//TMatrixRotationQuaternion(&m_matObjectLocalRotation, &qCWMrotation);
				D3DXMatrixIdentity(&m_matObjectLocalRotation);
			}
			else
			{

				fLerpTime = 0.0f;
				if (iNumstartTrack != iNumEndTrack)
				{
					fLerpTime = (float)(fCurTick - pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation][iNumstartTrack].Tick) /
						(pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation][iNumEndTrack].Tick - pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation][iNumstartTrack].Tick);
				}

				//스타트가 없을시
				if ((iNumstartTrack == iNumEndTrack) && (iNumstartTrack == 0))
				{
					//스타트 값은 초기값 으로 설정
					D3DXQuaternionSlerp(&RotationQuternion, &qCWMrotation,
						&pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation][0].Axis, fLerpTime);
				}
				else
				{
					D3DXQuaternionSlerp(&RotationQuternion, &pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation][iNumstartTrack].Axis,
						&pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation][iNumEndTrack].Axis, fLerpTime);
				}

				//Geom회전행렬에 저장
				D3DXMatrixRotationQuaternion(&m_matObjectLocalRotation, &RotationQuternion);
			}
		}
		else
		{
			D3DXMatrixRotationQuaternion(&m_matObjectLocalRotation, &qCWMrotation);
		}


		//================================================================================
		//	** AnimatinTransformation ** 
		//================================================================================
		TVector3 vpos;
		if (pAniData->AnimationTrack[(int)E_AnimationType::AnimatinTransformation].size() != 0)
		{
			//start,end Tick serch
			GetAnimationTrack(pAniData->AnimationTrack[(int)E_AnimationType::AnimatinTransformation], iNumstartTrack, iNumEndTrack, (float)fCurTick);

			if (iNumEndTrack == -1)
			{
				D3DXMatrixTranslation(&m_matObjectLocalPos, vCWMtransration.x, vCWMtransration.y, vCWMtransration.z);
			}
			else
			{
				//Tick값 계산
				fLerpTime = 0.0f;
				if (iNumstartTrack != iNumEndTrack)
				{
					fLerpTime = (float)(fCurTick - pAniData->AnimationTrack[(int)E_AnimationType::AnimatinTransformation][iNumstartTrack].Tick) /
						(pAniData->AnimationTrack[(int)E_AnimationType::AnimatinTransformation][iNumEndTrack].Tick - pAniData->AnimationTrack[(int)E_AnimationType::AnimatinTransformation][iNumstartTrack].Tick);
				}

				D3DXVec3Lerp(
					&vpos,
					&pAniData->AnimationTrack[(int)E_AnimationType::AnimatinTransformation][iNumstartTrack].Pos,
					&pAniData->AnimationTrack[(int)E_AnimationType::AnimatinTransformation][iNumEndTrack].Pos,
					fLerpTime);

				//포지션행렬에 이동행렬 저장
				D3DXMatrixTranslation(&m_matObjectLocalPos, vpos.x, vpos.y, vpos.z);
			}
		}
		else
		{
			D3DXMatrixTranslation(&m_matObjectLocalPos, vCWMtransration.x, vCWMtransration.y, vCWMtransration.z);
		}
	}
	else
	{
		D3DXMatrixScaling(&m_matObjectLocalScale, vCWMscale.x, vCWMscale.y, vCWMscale.z);
		D3DXMatrixRotationQuaternion(&m_matObjectLocalRotation, &qCWMrotation);
		D3DXMatrixTranslation(&m_matObjectLocalPos, vCWMtransration.x, vCWMtransration.y, vCWMtransration.z);
	}


	return true;
}

bool LASkinObject::Render()
{
	m_pCoreContext->VSSetShader(m_pResourceVS, NULL, 0);
	m_pCoreContext->PSSetShader(m_pResourcePS, NULL, 0);

	m_pCoreContext->HSSetShader(NULL, NULL, 0);
	m_pCoreContext->DSSetShader(NULL, NULL, 0);
	m_pCoreContext->GSSetShader(NULL, NULL, 0);

	/*================= 레이아웃 셋팅 ================*/
	UINT stride = sizeof(PNCT_VECTOR);
	UINT offset = 0;

	m_pCoreContext->IASetInputLayout(m_pVertexLayout);

	D3DXMatrixTranspose(&m_ObjectCB.g_matWorld, &m_matCalculation);

	TMatrix matShadow;
	D3DXMatrixIdentity(&matShadow);
	D3DXMatrixInverse(&m_ObjectCB.g_matWorldInverse, NULL, &matShadow);
	D3DXMatrixTranspose(&m_ObjectCB.g_matWorldInverse, &m_ObjectCB.g_matWorldInverse);

	D3DXMatrixTranspose(&m_ObjectCB.g_matView, &m_ObjectView);
	D3DXMatrixTranspose(&m_ObjectCB.g_matProj, &m_ObjectProjection);

	/*================= 상수 버퍼 셋팅 ================*/
	m_pCoreContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pCoreContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferData);
	m_pCoreContext->VSSetConstantBuffers(2, 1, m_pCBConstBoneBuffer.GetAddressOf());
	//m_pCoreContext->HSSetConstantBuffers(1, 1, m_pCBHSTessellation.GetAddressOf());

	/*================= 툴 선택 여부 셋팅 ================*/
	if (m_SelectObject)
	{
		m_ObjectDataCB.vData01.x = 1.0f;
	}
	else
	{
		m_ObjectDataCB.vData01.x = 0.0f;
	}

	/*================== 노말맵핑이나 라이트적용시 상수버퍼 디렉션 라이트 셋팅 =====================*/
	//if ((GetNormalMapping() == true) || (GetLight() == true))
	{
		m_ObjectDataCB.vEyePosition = m_vCameraPos;								//카메라포지션
		m_ObjectDataCB.vLightPos = m_vLightPos;									//라이트포지션
		m_ObjectDataCB.vLIghtDirection = TVector4(0, 0, 0, 1) - m_vLightPos;	//라이트디렉션
	}

	/*================== 노말맵핑에 필요한 데이터 셋팅 =====================*/
	TMatrix matInvWorld;
	D3DXMatrixInverse(&matInvWorld, NULL, &m_matBonObjectCalculation);
	m_ObjectDataCB.matWorldInverse = matInvWorld;

	/*================= 상수 버퍼 쉐이더 전송 ================*/
	m_pCoreContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_ObjectCB, 0, 0);
	m_pCoreContext->UpdateSubresource(m_pConstantBufferData, 0, NULL, &m_ObjectDataCB, 0, 0);
	m_pCoreContext->UpdateSubresource(m_pCBConstBoneBuffer.Get(), 0, NULL, m_cbMatrixListData, 0, 0);

	/*================= 프리미티브 타잎 설정 ================*/
	m_pCoreContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/*================= 렌더 ================*/
	DrawMesh();

	return true;
}

bool LASkinObject::RenderShadow()
{
	/*================= 쉐도우최종행렬생성, 행렬셋팅, 쉐도우 픽쉘 쉐이더 셋팅 ================*/
	TMatrix matShadow;
	TPlane pPlan = TPlane(0, 1, 0, -0.01f);

	D3DXMatrixShadow(&matShadow, &m_vLightPos, &pPlan);

	m_ObjectDataCB.vMeshColor = TVector4(0, 0, 0, 0.5f);
	m_pCoreContext->UpdateSubresource(m_pConstantBufferData, 0, NULL, &m_ObjectDataCB, 0, 0);

	/*================= 쉐이더 셋팅 ================*/
	m_pCoreContext->VSSetShader(m_pResourceVS, NULL, 0);
	m_pCoreContext->HSSetShader(NULL, NULL, 0);
	m_pCoreContext->DSSetShader(NULL, NULL, 0);
	m_pCoreContext->GSSetShader(NULL, NULL, 0);

	m_pCoreContext->IASetInputLayout(m_pVertexLayout);

	D3DXMatrixTranspose(&m_ObjectCB.g_matWorld, &m_matCalculation);
	D3DXMatrixTranspose(&m_ObjectCB.g_matWorldInverse, &matShadow);
	D3DXMatrixTranspose(&m_ObjectCB.g_matView, &m_ObjectView);
	D3DXMatrixTranspose(&m_ObjectCB.g_matProj, &m_ObjectProjection);

	/*================= 상수 버퍼 셋팅 ================*/
	m_pCoreContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pCoreContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferData);
	m_pCoreContext->VSSetConstantBuffers(2, 1, m_pCBConstBoneBuffer.GetAddressOf());

	/*================= 툴 선택 여부 셋팅 ================*/
	if (m_SelectObject)
	{
		m_ObjectDataCB.vData01.x = 1.0f;
	}
	else
	{
		m_ObjectDataCB.vData01.x = 0.0f;
	}

	/*================== 상수버퍼 최종 월드행렬 셋팅 =====================*/
	TMatrix matInvWorld;
	D3DXMatrixInverse(&matInvWorld, NULL, &m_matBonObjectCalculation);
	m_ObjectDataCB.matWorldInverse = matInvWorld;

	/*================= 상수 버퍼 쉐이더 전송 ================*/
	m_pCoreContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_ObjectCB, 0, 0);
	m_pCoreContext->UpdateSubresource(m_pConstantBufferData, 0, NULL, &m_ObjectDataCB, 0, 0);
	m_pCoreContext->UpdateSubresource(m_pCBConstBoneBuffer.Get(), 0, NULL, m_cbMatrixListData, 0, 0);

	/*================= 프리미티브 타잎 설정 ================*/
	m_pCoreContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/*================= 쉐도우 픽셀 쉐이더 설정 ================*/
	m_pCoreContext->PSSetShader(m_pShadowPS, NULL, 0);

	/*================= 쉐도우 렌더 ================*/
	DrawMesh();

	return true;
}


void LASkinObject::DrawMesh()
{
	LAMeshData* pMeshData = NULL;

	//sub material check render
	if (m_ObjectDest.GiometryShaderName != NULL)
	{
		m_pCoreContext->DrawAuto();
	}
	else
	{
		UINT stride = sizeof(PNCT5_VERTEX);
		UINT offset = 0;

		pMeshData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);

		for (int iCnt = 0; iCnt < (int)pMeshData->m_SubIndexList.size(); iCnt++)
		{

#ifdef TOOL_MODE
			/*============ 노말맵 여부 판단 버텍스 버퍼 셋팅 =============*/
			if ((GetNormalMapping() == true) && ((int)pMeshData->m_SubNormalMapVertexBufferList.size() != 0))
			{
				m_pCoreContext->IASetVertexBuffers(0, 1, pMeshData->m_SubNormalMapVertexBufferList[iCnt].GetAddressOf(), &stride, &offset);
			}
			else
			{
				m_pCoreContext->IASetVertexBuffers(0, 1, pMeshData->m_SubVertexBufferList[iCnt].GetAddressOf(), &stride, &offset);
			}
#else
			m_pCoreContext->IASetVertexBuffers(0, 1, pMeshData->m_SubVertexBufferList[iCnt].GetAddressOf(), &stride, &offset);
#endif
			/*============ 인덱스 버퍼 셋팅 =============*/
			m_pCoreContext->IASetIndexBuffer(pMeshData->m_SubIndexBufferList[iCnt].Get(), DXGI_FORMAT_R32_UINT, 0);

			/*============ 픽셀 쉐이더 셋팅 =============*/
			if (m_MeshData->m_MeshDataID != -1)
			{
				LAMaterialData* pRootMtl = MATERIAL_MGR.GetRootMaterial(m_MeshData->m_RootMaterialDataID);
				if (pRootMtl != NULL)
				{
					for (int jCnt = 0; jCnt < pRootMtl->m_SubMaterialArray.size(); jCnt++)
					{
						for (int iSubType = 0; iSubType < pRootMtl->m_SubMaterialArray[jCnt].MapData.size(); iSubType++)
						{
							if (pRootMtl->m_SubMaterialArray[jCnt].MapData[iSubType].texture.m_pTextureSRV[0] != NULL)
							{
								m_pCoreContext->PSSetShaderResources(iSubType, 1, pRootMtl->m_SubMaterialArray[jCnt].MapData[iSubType].texture.m_pTextureSRV[0].GetAddressOf());
							}
						}
					}
				}
			}

			/*============ 렌더 =============*/
			m_pCoreContext->DrawIndexed(pMeshData->m_SubIndexList[iCnt].size(), 0, 0);
		}
	}

#ifdef TOOL_MODE
	/*======================== Test NormalVertex DrawLine =========================*/
	/*CreateDrawLine();

	if (pMeshData)
	{
	DrawLine(pMeshData->m_SubSkinUniqueVecterList[0], pMeshData->m_SubIndexList[0], 10, m_matCalculation);
	}*/
#else
#endif

}

bool LASkinObject::Interpolate(TMatrix& TargetMatrix, int iCurTick)
{
	bool bResult = SetAnimationTrack((float)iCurTick);
	if (bResult = false)
		return bResult;

	TargetMatrix = m_matObjectLocalScale * m_matObjectLocalRotation * m_matObjectLocalPos;

	return true;
}

bool LASkinObject::CreateNormalMap(std::wstring fileName)
{
	if (CreateNormalTexture(fileName))
	{
		if (LoadNormalShader())
		{
			if (CreateNormalMapVertexBuffer())
			{
				SetNormalMapping(true);
				SetResourceVS(m_pNormalVS);
				SetResourcePS(m_pNormalPS);

				return true;
			}
		}
	}

	return false;
}

bool LASkinObject::DeleteNormalMap()
{
	if (DeleteNormalTexture())
	{
		SetNormalMapping(false);

		return true;
	}

	return false;
}

bool LASkinObject::CreateLight()
{
	if (LoadLightShader() == true)
	{
		SetLight(true);

		return true;
	}

	return false;
}

bool LASkinObject::CreateNormalTexture(std::wstring fileName)
{
	LAMeshData* pMeshData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);
	if (pMeshData)
	{
		for (int iCnt = 0; iCnt < (int)pMeshData->m_SubIndexList.size(); iCnt++)
		{
			LAMaterialData* pRootMtl = MATERIAL_MGR.GetRootMaterial(m_MeshData->m_RootMaterialDataID);
			if (pRootMtl != NULL)
			{
				for (int jCnt = 0; jCnt < pRootMtl->m_SubMaterialArray.size(); jCnt++)
				{
					//Diffuse Texture
					if (pRootMtl->m_SubMaterialArray[jCnt].MapData.size() == 1)
					{
						SubMaterialData* pSub = MATERIAL_MGR.GetSubMaterial(m_MeshData->m_RootMaterialDataID, pMeshData->m_SubTextureNumber[jCnt]);
						if (pSub != NULL)
						{
							TexMapData MapData;

							//NormalMap Txture type
							MapData.iTypeNumber = 2;
							MapData.pBitmapName = fileName;

							pSub->MapData.push_back(MapData);

							//MATERIAL_MGR.AddSubMaterial(m_MeshData->m_RootMaterialDataID, *pSub);
							MATERIAL_MGR.ConvertTextureResource(m_pCoreDevice);
						}
					}
				}
			}
		}

		return true;
	}

	return false;
}

bool LASkinObject::DeleteNormalTexture()
{
	LAMeshData* pMeshData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);
	if (pMeshData)
	{
		for (int iCnt = 0; iCnt < (int)pMeshData->m_SubIndexList.size(); iCnt++)
		{
			LAMaterialData* pRootMtl = MATERIAL_MGR.GetRootMaterial(m_MeshData->m_RootMaterialDataID);
			if (pRootMtl != NULL)
			{
				for (int jCnt = 0; jCnt < pRootMtl->m_SubMaterialArray.size(); jCnt++)
				{
					//Diffuse Texture
					if (pRootMtl->m_SubMaterialArray[jCnt].MapData.size() >= 2)
					{
						SubMaterialData* pSub = MATERIAL_MGR.GetSubMaterial(m_MeshData->m_RootMaterialDataID, pMeshData->m_SubTextureNumber[jCnt]);
						if (pSub != NULL)
						{
							for (int iSearchNormla = 0; iSearchNormla < pSub->MapData.size(); iSearchNormla++)
							{
								if (pSub->MapData[iSearchNormla].iTypeNumber == 2)
								{
									pSub->MapData[iSearchNormla].texture.m_pTextureSRV[0].ReleaseAndGetAddressOf();
									pSub->MapData.erase(pSub->MapData.begin() + iSearchNormla);
								}
							}
						}
					}
				}
			}
		}
	}

	return true;
}

bool LASkinObject::CreateNormalMapVertexBuffer()
{
	HRESULT hr;

	LAMeshData* pMeshData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);
	if (pMeshData)
	{
		//서브매터리얼 조건 검사후 생성
		pMeshData->m_SubNormalMapVertexBufferList.resize(pMeshData->m_SubSkinUniqueVecterList.size());
		for (int iCnt = 0; iCnt < (int)pMeshData->m_SubSkinUniqueVecterList.size(); iCnt++)
		{
			if (pMeshData->m_SubSkinUniqueVecterList[iCnt].size() <= 0)
				continue;

			/*======================== 정점 노말 생성(변경) ========================*/
			UpDateNormals();
			/*======================== 노말맵핑 법선벡터 생성(라이트에 사용) ========================*/
			CreateTangent(pMeshData);

			/*======================== 버텍스 버퍼 생성 ========================*/
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(PNCT5_VERTEX) * pMeshData->m_SubSkinUniqueVecterList[iCnt].size();
			int isize = sizeof(PNCT5_VERTEX);
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = (void*)&pMeshData->m_SubSkinUniqueVecterList[iCnt].at(0);

			hr = m_pCoreDevice->CreateBuffer(&bd, &InitData, pMeshData->m_SubNormalMapVertexBufferList[iCnt].GetAddressOf());
			if (!SUCCEEDED(hr))
			{
				//ERROR
				return false;
			}
		}
	}

	return true;
}

bool LASkinObject::LoadNormalShader()
{
	if (m_pNormalVS != NULL)
		return false;

	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef TOOL_MODE
	CrateObjectDest NormalDest;
	NormalDest.vertexShaderName = L"../data/shader/Skin.hlsl";
	NormalDest.vertexFuntionNmae = "VS_NORMAL";
	NormalDest.PixelShaderName = L"../data/shader/Skin.hlsl";
	NormalDest.PixelFuntionNmae = "TPS_NORMAL";
	NormalDest.GiometryShaderName = NULL;
	NormalDest.GiometryFuntionNmae = NULL;
	NormalDest.pTextureName = NULL;
#else
	CrateObjectDest NormalDest;
	NormalDest.vertexShaderName = L"../../data/CharacterLoadFile/shader/Skin.hlsl";
	NormalDest.vertexFuntionNmae = "VS";
	NormalDest.PixelShaderName = L"../../data/CharacterLoadFile/shader/Skin.hlsl";
	NormalDest.PixelFuntionNmae = "TPS_NORMAL";
	NormalDest.GiometryShaderName = NULL;
	NormalDest.GiometryFuntionNmae = NULL;
	NormalDest.pTextureName = NULL;
#endif

	/*================= 버텍스 쉐이더 생성 =================*/
	//CreateVertexShader
	hr = D3DCompileFromFile(NormalDest.vertexShaderName, NULL, NULL, NormalDest.vertexFuntionNmae, "vs_5_0", NULL, NULL, &pVSBuf, NULL);
	if (hr != S_OK)
	{
		return false;
	}
	m_pCoreDevice->CreateVertexShader((DWORD*)pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), NULL, &m_pNormalVS);

	/*================= 픽셀 쉐이더 생성 =================*/
	//CreatePixelShader
	hr = D3DCompileFromFile(NormalDest.PixelShaderName, NULL, NULL, NormalDest.PixelFuntionNmae, "ps_5_0", NULL, NULL, &pPSBuf, NULL);
	if (hr != S_OK)
	{
		return false;
	}
	m_pCoreDevice->CreatePixelShader((DWORD*)pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pNormalPS);

	return true;
}

bool LASkinObject::LoadLightShader()
{
	if (m_pLightPS == NULL)
	{
		/*================= 정점 쉐이더 동일 =================*/
		/*================= 픽셀 쉐이더만 생성 =================*/
		HRESULT hr = S_OK;
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef TOOL_MODE
		CrateObjectDest NormalDest;
		NormalDest.vertexShaderName = L"../data/shader/Skin.hlsl";
		NormalDest.vertexFuntionNmae = NULL;
		NormalDest.PixelShaderName = L"../data/shader/Skin.hlsl";
		NormalDest.PixelFuntionNmae = "TPS_LIGHT";
		NormalDest.GiometryShaderName = NULL;
		NormalDest.GiometryFuntionNmae = NULL;
		NormalDest.pTextureName = NULL;
#else
		CrateObjectDest NormalDest;
		NormalDest.vertexShaderName = L"../../data/CharacterLoadFile/shader/Skin.hlsl";
		NormalDest.vertexFuntionNmae = NULL;
		NormalDest.PixelShaderName = L"../../data/CharacterLoadFile/shader/Skin.hlsl";
		NormalDest.PixelFuntionNmae = "TPS_LIGHT";
		NormalDest.GiometryShaderName = NULL;
		NormalDest.GiometryFuntionNmae = NULL;
		NormalDest.pTextureName = NULL;
#endif

		//CreatePixelShader
		hr = D3DCompileFromFile(NormalDest.PixelShaderName, NULL, NULL, NormalDest.PixelFuntionNmae, "ps_5_0", NULL, NULL, &pPSBuf, NULL);
		if (hr != S_OK)
		{
			return true;
		}
		m_pCoreDevice->CreatePixelShader((DWORD*)pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pLightPS);

	}

	return true;
}

bool LASkinObject::LoadShadowShader()
{
	if (m_pShadowPS == NULL)
	{
		/*================= 정점 쉐이더 동일 =================*/
		/*================= 픽셀 쉐이더만 생성 =================*/
		HRESULT hr = S_OK;
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef TOOL_MODE
		CrateObjectDest ShadowDest;
		ShadowDest.vertexShaderName = L"../data/shader/Skin.hlsl";
		ShadowDest.vertexFuntionNmae = NULL;
		ShadowDest.PixelShaderName = L"../data/shader/Skin.hlsl";
		ShadowDest.PixelFuntionNmae = "PS_COLOR";
		ShadowDest.GiometryShaderName = NULL;
		ShadowDest.GiometryFuntionNmae = NULL;
		ShadowDest.pTextureName = NULL;
#else
		CrateObjectDest ShadowDest;
		ShadowDest.vertexShaderName = L"../../data/CharacterLoadFile/shader/Skin.hlsl";
		ShadowDest.vertexFuntionNmae = NULL;
		ShadowDest.PixelShaderName = L"../../data/CharacterLoadFile/shader/Skin.hlsl";
		ShadowDest.PixelFuntionNmae = "PS_COLOR";
		ShadowDest.GiometryShaderName = NULL;
		ShadowDest.GiometryFuntionNmae = NULL;
		ShadowDest.pTextureName = NULL;
#endif

		//CreatePixelShader
		hr = D3DCompileFromFile(ShadowDest.PixelShaderName, NULL, NULL, ShadowDest.PixelFuntionNmae, "ps_5_0", NULL, NULL, &pPSBuf, NULL);
		if (hr != S_OK)
		{
			return false;
		}
		m_pCoreDevice->CreatePixelShader((DWORD*)pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &m_pShadowPS);

	}

	return true;
}

bool LASkinObject::InstancingRender(InstancingDataInfo& rInstancingData)
{
	m_pCoreContext->VSSetShader(m_pResourceVS, NULL, 0);
	m_pCoreContext->PSSetShader(m_pResourcePS, NULL, 0);

	m_pCoreContext->HSSetShader(NULL, NULL, 0);
	m_pCoreContext->DSSetShader(NULL, NULL, 0);
	m_pCoreContext->GSSetShader(NULL, NULL, 0);

	/*================= 레이아웃 셋팅 ================*/
	UINT stride = sizeof(PNCT_VECTOR);
	UINT offset = 0;

	m_pCoreContext->IASetInputLayout(m_pVertexLayout);

	D3DXMatrixTranspose(&m_ObjectCB.g_matWorld, &m_matCalculation);

	TMatrix matShadow;
	D3DXMatrixIdentity(&matShadow);
	D3DXMatrixInverse(&m_ObjectCB.g_matWorldInverse, NULL, &matShadow);
	D3DXMatrixTranspose(&m_ObjectCB.g_matWorldInverse, &m_ObjectCB.g_matWorldInverse);
	
	D3DXMatrixTranspose(&m_ObjectCB.g_matView, &m_ObjectView);
	D3DXMatrixTranspose(&m_ObjectCB.g_matProj, &m_ObjectProjection);

	/*================= 상수 버퍼 셋팅 ================*/
	m_pCoreContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pCoreContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferData);
	m_pCoreContext->VSSetConstantBuffers(2, 1, m_pInstancingAnimationBuffer.GetAddressOf());
	m_pCoreContext->VSSetConstantBuffers(3, 1, m_pInstancingNormalBuffer.GetAddressOf());

	/*================= 툴 선택 여부 셋팅 ================*/
	if (m_SelectObject)
	{
		m_ObjectDataCB.vData01.x = 1.0f;
	}
	else
	{
		m_ObjectDataCB.vData01.x = 0.0f;
	}

	/*================== 노말맵핑이나 라이트적용시 상수버퍼 디렉션 라이트 셋팅 =====================*/
	//if ((GetNormalMapping() == true) || (GetLight() == true))
	{
		m_ObjectDataCB.vEyePosition = m_vCameraPos;								//카메라포지션
		m_ObjectDataCB.vLightPos = m_vLightPos;									//라이트포지션
		m_ObjectDataCB.vLIghtDirection = TVector4(0, 0, 0, 1) - m_vLightPos;	//라이트디렉션
		m_ObjectDataCB.vMeshColor = TVector4(0, 0, 0, 0.5f);
	}

	/*================== 노말맵핑에 필요한 데이터 셋팅 =====================*/
	TMatrix matInvWorld;
	D3DXMatrixInverse(&matInvWorld, NULL, &m_matBonObjectCalculation);
	m_ObjectDataCB.matWorldInverse = matInvWorld;

	/*================= 상수 버퍼 쉐이더 전송 ================*/
	m_pCoreContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_ObjectCB, 0, 0);
	m_pCoreContext->UpdateSubresource(m_pConstantBufferData, 0, NULL, &m_ObjectDataCB, 0, 0);
	m_pCoreContext->UpdateSubresource(m_pInstancingAnimationBuffer.Get(), 0, NULL, rInstancingData.matBoneAnimationWorld, 0, 0);
	m_pCoreContext->UpdateSubresource(m_pInstancingNormalBuffer.Get(), 0, NULL, rInstancingData.matNormalWorldInverse, 0, 0);

	/*================= 프리미티브 타잎 설정 ================*/
	m_pCoreContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	/*================= 인스턴싱 렌더 ================*/
	LAMeshData* pMeshData = NULL;

	//sub material check render
	if (m_ObjectDest.GiometryShaderName != NULL)
	{
		m_pCoreContext->DrawAuto();
	}
	else
	{
		UINT stride = sizeof(PNCT5_VERTEX);
		UINT offset = 0;

		pMeshData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);

		for (int iCnt = 0; iCnt < (int)pMeshData->m_SubIndexList.size(); iCnt++)
		{

			/*============ 버텍스 버퍼 셋팅 =============*/
#ifdef TOOL_MODE
			if ((GetNormalMapping() == true) && ((int)pMeshData->m_SubNormalMapVertexBufferList.size() != 0))
			{
				m_pCoreContext->IASetVertexBuffers(0, 1, pMeshData->m_SubNormalMapVertexBufferList[iCnt].GetAddressOf(), &stride, &offset);
			}
			else
			{
				m_pCoreContext->IASetVertexBuffers(0, 1, pMeshData->m_SubVertexBufferList[iCnt].GetAddressOf(), &stride, &offset);
			}
#else
			m_pCoreContext->IASetVertexBuffers(0, 1, pMeshData->m_SubVertexBufferList[iCnt].GetAddressOf(), &stride, &offset);
#endif
			/*============ 인덱스 버퍼 셋팅 =============*/
			m_pCoreContext->IASetIndexBuffer(pMeshData->m_SubIndexBufferList[iCnt].Get(), DXGI_FORMAT_R32_UINT, 0);

			/*============ 픽셀 쉐이더 리소스 셋팅 =============*/
			if (m_MeshData->m_MeshDataID != -1)
			{
				LAMaterialData* pRootMtl = MATERIAL_MGR.GetRootMaterial(m_MeshData->m_RootMaterialDataID);
				if (pRootMtl != NULL)
				{
					for (int jCnt = 0; jCnt < pRootMtl->m_SubMaterialArray.size(); jCnt++)
					{
						for (int iSubType = 0; iSubType < pRootMtl->m_SubMaterialArray[jCnt].MapData.size(); iSubType++)
						{
							if (pRootMtl->m_SubMaterialArray[jCnt].MapData[iSubType].texture.m_pTextureSRV[0] != NULL)
							{
								m_pCoreContext->PSSetShaderResources(iSubType, 1, pRootMtl->m_SubMaterialArray[jCnt].MapData[iSubType].texture.m_pTextureSRV[0].GetAddressOf());
							}
						}
					}
				}
			}

			/*============ 렌더 =============*/
			m_pCoreContext->DrawIndexedInstanced(pMeshData->m_SubIndexList[iCnt].size(), rInstancingData.InstancingCount, 0, 0, 0);
			//m_pCoreContext->DrawIndexedInstanced(pMeshData->m_SubIndexList[iCnt].size(), rInstancingData.InstancingCount, 0, 0, 0);





			


		}
	}

	return true;
}

bool LASkinObject::InstancingShadowRender(InstancingDataInfo& rInstancingData)
{
	/*================= 기존 렌더 동일 업데이트 ================*/
	m_pCoreContext->VSSetShader(m_pResourceVS, NULL, 0);
	m_pCoreContext->PSSetShader(m_pResourcePS, NULL, 0);
	m_pCoreContext->HSSetShader(NULL, NULL, 0);
	m_pCoreContext->DSSetShader(NULL, NULL, 0);
	m_pCoreContext->GSSetShader(NULL, NULL, 0);
	m_pCoreContext->IASetInputLayout(m_pVertexLayout);
	m_pCoreContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pCoreContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferData);
	m_pCoreContext->VSSetConstantBuffers(2, 1, m_pInstancingAnimationBuffer.GetAddressOf());
	m_pCoreContext->VSSetConstantBuffers(3, 1, m_pInstancingNormalBuffer.GetAddressOf());
	m_pCoreContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_ObjectCB, 0, 0);
	m_pCoreContext->UpdateSubresource(m_pConstantBufferData, 0, NULL, &m_ObjectDataCB, 0, 0);
	m_pCoreContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	LAMeshData* pMeshData = NULL;

	/*================= 애니메이션 및 월드 상수 데이터 업데이트 ================*/
	m_pCoreContext->UpdateSubresource(m_pInstancingAnimationBuffer.Get(), 0, NULL, rInstancingData.matBoneAnimationWorld, 0, 0);
	m_pCoreContext->UpdateSubresource(m_pInstancingNormalBuffer.Get(), 0, NULL, rInstancingData.matNormalWorldInverse, 0, 0);


	pMeshData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);
	if (pMeshData == NULL)
		return false;

	for (int iCnt = 0; iCnt < (int)pMeshData->m_SubIndexList.size(); iCnt++)
	{
		UINT stride = sizeof(PNCT5_VERTEX);
		UINT offset = 0;
#ifdef TOOL_MODE
		if ((GetNormalMapping() == true) && ((int)pMeshData->m_SubNormalMapVertexBufferList.size() != 0))
		{
			m_pCoreContext->IASetVertexBuffers(0, 1, pMeshData->m_SubNormalMapVertexBufferList[iCnt].GetAddressOf(), &stride, &offset);
		}
		else
		{
			m_pCoreContext->IASetVertexBuffers(0, 1, pMeshData->m_SubVertexBufferList[iCnt].GetAddressOf(), &stride, &offset);
		}
#else
		m_pCoreContext->IASetVertexBuffers(0, 1, pMeshData->m_SubVertexBufferList[iCnt].GetAddressOf(), &stride, &offset);
#endif
		/*============ 인덱스 버퍼 셋팅 =============*/
		m_pCoreContext->IASetIndexBuffer(pMeshData->m_SubIndexBufferList[iCnt].Get(), DXGI_FORMAT_R32_UINT, 0);

		/*================= 쉐도우 상수 버퍼 데이터 업데이트 ================*/
		D3DXMatrixTranspose(&m_ObjectCB.g_matWorldInverse, &m_matShadow);
		m_pCoreContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_ObjectCB, 0, 0);

		/*================= 쉐도우 픽셀 쉐이더 설정 ================*/
		m_pCoreContext->PSSetShader(m_pShadowPS, NULL, 0);

		/*============ 렌더 =============*/
		m_pCoreContext->DrawIndexedInstanced(pMeshData->m_SubIndexList[iCnt].size(), rInstancingData.InstancingCount, 0, 0, 0);
	}

	return true;
}

void LASkinObject::CreateShadowMatrix()
{
	/*================= 쉐도우최종행렬생성 ================*/
	TPlane pPlan = TPlane(0, 1, 0, -0.01f);
	D3DXMatrixShadow(&m_matShadow, &m_vLightPos, &pPlan);
}

bool LASkinObject::Release()
{
	LAObject2::Release();

#ifdef TOOL_MODE

	SAFE_DEL(m_MeshData);

#else

	S_DEL(m_MeshData);

#endif

	m_pmatParent = NULL;

	return true;
}

LASkinObject::~LASkinObject()
{
}
