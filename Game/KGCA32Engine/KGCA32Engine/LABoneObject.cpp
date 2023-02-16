
//#include "stdafx.h"

#include "LACoreLinkStd.h"
#include "LABoneObject.h"

#include "CoreFuncLink.h"
#include "LACharacterStruct.h"

#include "LAMaterialManager.h"
#include "LAMeshManager.h"
#include "LAAnimationManager.h"


//void LABoneObject::SetMatrix(TMatrix* pWorldMatrix, TMatrix* pViewMatrix, TMatrix* pProjectionMatrix)
//{
//	if (pWorldMatrix != NULL)
//	{
//		m_pmatCharacter = pWorldMatrix;
//
//		switch ((int)m_SpaceObject.m_CurrentSpaceType)
//		{
//		case (int)E_SpaceType::Sphere:
//		{
//			m_SpaceObject.m_pSpaceSpher->vCenter.x = pWorldMatrix->_41;
//			m_SpaceObject.m_pSpaceSpher->vCenter.y = pWorldMatrix->_42;
//			m_SpaceObject.m_pSpaceSpher->vCenter.z = pWorldMatrix->_43;
//			m_SpaceObject.m_pSpaceSpher->fRadius = pWorldMatrix->_11;
//		}break;
//
//		default:
//		{
//			break;
//		}
//		}
//
//	}
//
//	if (pViewMatrix != NULL)
//	{
//		m_ObjectView = *pViewMatrix;
//	}
//
//	if (pProjectionMatrix != NULL)
//	{
//		m_ObjectProjection = *pProjectionMatrix;
//	}
//}



void LABoneObject::SetCrrentTick(bool bCheck)
{
	m_bSetParentTick = bCheck;
}


HRESULT LABoneObject::CreateVertexBuffer()
{
	HRESULT hr = S_OK;
	if (m_MeshData->m_ObjectType == E_ObjectType::Helper_Bone)
	{
		return S_OK;
	}

	//Helper Object Return
	if (m_MeshData->m_ObjectType == E_ObjectType::Helper_Dummy)
	{
		if (m_MeshData->m_MeshDataID == -1)
		{
			m_pDummyRenderBox = new LABox2;
			m_pDummyRenderBox->Init(m_pCoreDevice, m_pCoreContext);

#ifdef TOOL_MODE

			CrateObjectDest ObjectDest;
			ObjectDest.vertexShaderName = L"../data/shader/VertexShader.vsh";
			ObjectDest.vertexFuntionNmae = NULL;
			ObjectDest.PixelShaderName = L"../data/shader/PixelShader.psh";
			ObjectDest.PixelFuntionNmae = NULL;
			ObjectDest.GiometryShaderName = NULL;
			ObjectDest.GiometryFuntionNmae = NULL;
			ObjectDest.pTextureName = NULL;
#else
			CrateObjectDest ObjectDest;
			ObjectDest.vertexShaderName = L"../../data/CharacterLoadFile/shader/VertexShader.vsh";
			ObjectDest.vertexFuntionNmae = NULL;
			ObjectDest.PixelShaderName = L"../../data/CharacterLoadFile/shader/PixelShader.psh";
			ObjectDest.PixelFuntionNmae = NULL;
			ObjectDest.GiometryShaderName = NULL;
			ObjectDest.GiometryFuntionNmae = NULL;
			ObjectDest.pTextureName = NULL;
#endif

			m_pDummyRenderBox->Create(ObjectDest);
			m_pDummyRenderBox->SetColor(0.0f, 0.0f, 1.0f, 1.0f);

			//Space Collision Box Create
			m_pDummyRenderBox->m_SpaceObject.CreateBox();

			return S_OK;
		}
	}

	//Biped SpaceBox Setting
	m_pDummyRenderBox = new LABox2;
	m_pDummyRenderBox->m_SpaceObject.CreateBox();

	//LAMaterialData* pMetarialData = MATERIAL_MGR.GetMaterial(m_MeshData->m_RootMaterialDataID);
	LAMeshData* pMeshData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);
	if (pMeshData != NULL)
	{
		m_pDummyRenderBox->m_SpaceObject.m_pSpaceBox->UpDateMinMax(pMeshData->m_SubUniqueVectorList[0]);

		//서브매터리얼 조건 검사후 생성
		pMeshData->m_SubVertexBufferList.resize(pMeshData->m_SubUniqueVectorList.size());
		for (int iCnt = 0; iCnt < (int)pMeshData->m_SubUniqueVectorList.size(); iCnt++)
		{
			if (pMeshData->m_SubUniqueVectorList[iCnt].size() <= 0)
				continue;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(PNCT_VECTOR) * pMeshData->m_SubUniqueVectorList[iCnt].size();
			int isize = sizeof(PNCT_VECTOR);
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = (void*)&pMeshData->m_SubUniqueVectorList[iCnt].at(0);

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

HRESULT LABoneObject::CreateIndexBuffer()
{
	HRESULT hr = S_OK;

	if (m_MeshData->m_ObjectType == E_ObjectType::Helper_Bone)
	{
		return S_OK;
	}

	//Helper Object Return
	if (m_MeshData->m_ObjectType == E_ObjectType::Helper_Dummy)
	{
		if (m_MeshData->m_MeshDataID == -1)
		{
			m_pDummyRenderBox->CreateIndexBuffer();

			return S_OK;
		}
	}

	//서브매터리얼 조건 검사후 생성
	//if (m_LoadObjectData.bisSubMaterial == false)
	//{
	//	m_SubIndexBufferList.resize(0);

	//	D3D11_BUFFER_DESC bd;
	//	ZeroMemory(&bd, sizeof(bd));
	//	bd.Usage = D3D11_USAGE_DEFAULT;
	//	bd.ByteWidth = sizeof(DWORD) * m_MeshData.m_SubIndexList[0].size();
	//	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	//	D3D11_SUBRESOURCE_DATA InitData;
	//	ZeroMemory(&InitData, sizeof(D3D11_SUBRESOURCE_DATA));
	//	InitData.pSysMem = (void*)&m_MeshData.m_SubIndexList[0].at(0);

	//	hr = m_pCoreDevice->CreateBuffer(&bd, &InitData, m_SubIndexBufferList[0].GetAddressOf());
	//	if (!SUCCEEDED(hr))
	//	{
	//		//ERROR
	//		return false;
	//	}
	//}
	//else
	//{


	LAMeshData* pMeshData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);
	if (pMeshData != NULL)
	{
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
	}

	return S_OK;
}

HRESULT LABoneObject::LoadTextureResource()
{
	HRESULT hr = S_OK;

	return S_OK;
}

bool LABoneObject::Frame()
{
	LAObject2::Frame();
	//int AniIndex = m_MeshData->m_AnimationIDList[m_iCurrentMotion];
	//LAAnimaionData* pAniData = ANIMATION_MGR.GetAnimaion(AniIndex);

	//Tick Check;
	
	
	if ((int)m_MeshData->m_AnimationIDList.size() != 0)
	{
		if (m_bSetParentTick == false)
		{
			//Interpolate(m_ObjectWorld, GetCurrentAnimationTick(&m_MeshData->m_SceneDataList, m_BoneTick));
		}
		else
		{
			Interpolate(m_ObjectWorld, m_ParentObjectTick);
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

	

	TMatrix inverse;
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

	//드레스포즈 애니메이션 
	D3DXMatrixInverse(&inverse, NULL, &m_MeshData->matLoadObjectWorld);
	m_matInversAni = inverse * m_matCalculation;

	if (m_MeshData->m_ObjectType == E_ObjectType::Helper_Dummy)
	{
		m_pDummyRenderBox->Frame();
		m_pDummyRenderBox->m_SpaceObject.Update(m_matCalculation);
		m_pDummyRenderBox->SetMatrix(&m_matCalculation, &m_ObjectView, &m_ObjectProjection);
	}
	else if (m_MeshData->m_ObjectType == E_ObjectType::Geom_Biped)
	{
		m_pDummyRenderBox->m_SpaceObject.Update(m_matCalculation);
	}

	return true;
}

bool LABoneObject::Interpolate(TMatrix& TargetMatrix, int iCurTick)
{
	bool bResult = SetAnimationTrack((float)iCurTick);
	if (bResult = false)
		return bResult;

	TargetMatrix = m_matObjectLocalScale * m_matObjectLocalRotation * m_matObjectLocalPos;

	return true;
}

bool LABoneObject::GetAnimationTrack(std::vector<AnimationTrack>& TrackList, int& NumstartTrack, int& NumEndTrack, float fCurTick)
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

bool LABoneObject::SetAnimationTrack(float fCurTick)
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
			//TMatrixIdentity(&m_matObjectLocalScale);
		}


		//================================================================================
		//	** AnimatinRotation ** 
		//================================================================================	
		if (pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation].size() != 0)
		{
			TQuaternion RotationQuternion; 

			//start,end Tick serch
			GetAnimationTrack(pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation], iNumstartTrack, iNumEndTrack, (float)fCurTick);

			//static int istartTick, iendTick;

			//if (savetick != m_iMainCurrentTick)
			//{
			//	//savetick = m_iMainCurrentTick;
			//	std::wstring debug;
			//	debug = std::to_wstring(iNumstartTrack);
			//	debug += L" ";
			//	debug += std::to_wstring(iNumEndTrack);
			//	OutputDebugString(debug.c_str());
			//	OutputDebugString(L"\n");
			//}
			if (iNumEndTrack  > 35)
			{
				int a = 10;
			}

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
				//if ((iNumstartTrack == iNumEndTrack))// && (iNumstartTrack == 0))
				//{
				//	//스타트 값은 초기값 으로 설정
				//	TQuaternionSlerp(&RotationQuternion, &qCWMrotation,
				//		&pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation][0].Axis, fLerpTime);
				//}
				//else
				//{
				D3DXQuaternionSlerp(&RotationQuternion, &pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation][iNumstartTrack].Axis,
						&pAniData->AnimationTrack[(int)E_AnimationType::AnimatinRotation][iNumEndTrack].Axis, fLerpTime);
			//	}

				//Geom회전행렬에 저장
				D3DXMatrixRotationQuaternion(&m_matObjectLocalRotation, &RotationQuternion);
			}
		}
		else
		{
			D3DXMatrixRotationQuaternion(&m_matObjectLocalRotation, &qCWMrotation);
			//TMatrixIdentity(&m_matObjectLocalRotation);
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
			//TMatrixIdentity(&m_matObjectLocalPos);
		}
	}
	else
	{
		D3DXMatrixScaling(&m_matObjectLocalScale, vCWMscale.x, vCWMscale.y, vCWMscale.z);
		D3DXMatrixRotationQuaternion(&m_matObjectLocalRotation, &qCWMrotation);
		D3DXMatrixTranslation(&m_matObjectLocalPos, vCWMtransration.x, vCWMtransration.y, vCWMtransration.z);

		/*TMatrixIdentity(&m_matObjectLocalScale);
		TMatrixIdentity(&m_matObjectLocalRotation);
		TMatrixIdentity(&m_matObjectLocalPos);*/
	}


	return true;
}

void LABoneObject::SetSelectObject(bool bcheck)
{
	m_SelectObject = bcheck;
}



bool LABoneObject::Render()
{
	if (m_MeshData->m_ObjectType == E_ObjectType::Helper_Bone)
	{
		return false;
	}

	if (m_MeshData->m_ObjectType == E_ObjectType::Helper_Dummy)
	{
		if (m_SelectObject)
		{
			m_pDummyRenderBox->m_ObjectDataCB.vData01.x = 1.0f;
		}
		else
		{
			m_pDummyRenderBox->m_ObjectDataCB.vData01.x = 0.0f;
		}
		m_pCoreContext->UpdateSubresource(m_pDummyRenderBox->m_pConstantBufferData, 0, NULL, &m_pDummyRenderBox->m_ObjectDataCB, 0, 0);

		

		m_pDummyRenderBox->Render();

		return true;
	}

	m_pCoreContext->VSSetShader(m_pVS, NULL, 0);
	m_pCoreContext->HSSetShader(NULL, NULL, 0);
	m_pCoreContext->DSSetShader(NULL, NULL, 0);
	m_pCoreContext->PSSetShader(m_pPS, NULL, 0);
	m_pCoreContext->GSSetShader(NULL, NULL, 0);

	UINT stride = sizeof(PNCT_VECTOR);
	UINT offset = 0;

	m_pCoreContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pCoreContext->VSSetConstantBuffers(1, 1, &m_pConstantBufferData);
	m_pCoreContext->IASetInputLayout(m_pVertexLayout);

	//행렬 정보 셋팅
	D3DXMatrixTranspose(&m_ObjectCB.g_matWorld, &m_matCalculation);
	D3DXMatrixTranspose(&m_ObjectCB.g_matWorldInverse, &m_ObjectCB.g_matWorldInverse);
	D3DXMatrixTranspose(&m_ObjectCB.g_matView, &m_ObjectView);
	D3DXMatrixTranspose(&m_ObjectCB.g_matProj, &m_ObjectProjection);
	m_pCoreContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &m_ObjectCB, 0, 0);

	//그외 값 정보 셋팅
	if (m_SelectObject)
	{
		m_ObjectDataCB.vData01.x = 1.0f;
	}
	else
	{
		m_ObjectDataCB.vData01.x = 0.0f;
	}
	m_pCoreContext->UpdateSubresource(m_pConstantBufferData, 0, NULL, &m_ObjectDataCB, 0, 0);
	m_pCoreContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DrawMesh();

	


	

	return true;
}

void LABoneObject::DrawMesh()
{
	//sub material check render
	if (m_ObjectDest.GiometryShaderName != NULL)
	{
		m_pCoreContext->DrawAuto();

		return;
	}

	UINT stride = sizeof(PNCT_VECTOR);
	UINT offset = 0;
	LAMeshData* pMeshData = NULL;

	pMeshData = MESH_MGR.GetMesh(m_MeshData->m_MeshDataID);
	if (pMeshData != NULL)
	{
		for (int iCnt = 0; iCnt < (int)pMeshData->m_SubIndexList.size(); iCnt++)
		{
			m_pCoreContext->IASetVertexBuffers(0, 1, pMeshData->m_SubVertexBufferList[iCnt].GetAddressOf(), &stride, &offset);
			m_pCoreContext->IASetIndexBuffer(pMeshData->m_SubIndexBufferList[iCnt].Get(), DXGI_FORMAT_R32_UINT, 0);
			//m_pCoreContext->PSSetShaderResources(0, 0, NULL);

			/*if (m_MeshData->m_MeshDataID != -1)
			{
				SubMaterialData* pData = MATERIAL_MGR.GetSubMaterial(m_MeshData->m_RootMaterialDataID, pMeshData->m_SubTextureNumber[iCnt]);
				if (pData != NULL)
				{
					if (pData->MapData[0].pTextureSRV != NULL)
					{
						m_pCoreContext->PSSetShaderResources(0, 1, pData->MapData[0].pTextureSRV.GetAddressOf());
					}
				}
			}*/
			m_pCoreContext->DrawIndexed(pMeshData->m_SubIndexList[iCnt].size(), 0, 0);
		}
	}
}

bool LABoneObject::Release()
{
	LAObject2::Release();

	if (m_MeshData != NULL)
	{
		delete m_MeshData;
	}
	m_pmatParent = NULL;

	return true;
}

LABoneObject::~LABoneObject()
{

}
