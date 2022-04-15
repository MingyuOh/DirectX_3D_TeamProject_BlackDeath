

//#include "stdafx.h"

#include "LAResourceManager.h"

void LAResourceManager::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;


#ifdef TOOL_MODE

	//매트릭스 기본 쉐이더리소스값
	m_BoneShaderDest.vertexShaderName = L"../data/shader/VertexShader.vsh";
	m_BoneShaderDest.vertexFuntionNmae = NULL;
	m_BoneShaderDest.PixelShaderName = L"../data/shader/PixelShader.psh";
	m_BoneShaderDest.PixelFuntionNmae = NULL;
	m_BoneShaderDest.GiometryShaderName = NULL;
	m_BoneShaderDest.GiometryFuntionNmae = NULL;
	m_BoneShaderDest.pTextureName = NULL;

	//스킨 기본 쉐이더리소스값
	m_SkinShaderDest.vertexShaderName = L"../data/shader/Skin.hlsl";
	m_SkinShaderDest.vertexFuntionNmae = NULL;
	m_SkinShaderDest.PixelShaderName = L"../data/shader/Skin.hlsl";
	m_SkinShaderDest.PixelFuntionNmae = "TPS_LIGHT";
	m_SkinShaderDest.GiometryShaderName = NULL;
	m_SkinShaderDest.GiometryFuntionNmae = NULL;
	m_SkinShaderDest.pTextureName = NULL;

#else

	//매트릭스 기본 쉐이더리소스값
	m_BoneShaderDest.vertexShaderName = L"../../data/CharacterLoadFile/shader/VertexShader.vsh";
	m_BoneShaderDest.vertexFuntionNmae = "VS";
	m_BoneShaderDest.PixelShaderName = L"../../data/CharacterLoadFile/shader/PixelShader.psh";
	m_BoneShaderDest.PixelFuntionNmae = "PS";
	m_BoneShaderDest.GiometryShaderName = NULL;
	m_BoneShaderDest.GiometryFuntionNmae = NULL;
	m_BoneShaderDest.pTextureName = NULL;

	//스킨 기본 쉐이더리소스값
	m_SkinShaderDest.vertexShaderName = L"../../data/CharacterLoadFile/shader/Skin.hlsl";
	m_SkinShaderDest.vertexFuntionNmae = "VS";
	m_SkinShaderDest.PixelShaderName = L"../../data/CharacterLoadFile/shader/Skin.hlsl";
	m_SkinShaderDest.PixelFuntionNmae = "TPS_LIGHT";
	m_SkinShaderDest.GiometryShaderName = NULL;
	m_SkinShaderDest.GiometryFuntionNmae = NULL;
	m_SkinShaderDest.pTextureName = NULL;

	////노말맵 매트릭스 기본 쉐이더리소스값
	//m_BoneNormalMappingShaderDest.vertexShaderName = L"../../data/CharacterLoadFile/shader/VertexShader.vsh";
	//m_BoneNormalMappingShaderDest.vertexFuntionNmae = NULL;
	//m_BoneNormalMappingShaderDest.PixelShaderName = L"../../data/CharacterLoadFile/shader/PixelShader.psh";
	//m_BoneNormalMappingShaderDest.PixelFuntionNmae = NULL;
	//m_BoneNormalMappingShaderDest.GiometryShaderName = NULL;
	//m_BoneNormalMappingShaderDest.GiometryFuntionNmae = NULL;
	//m_BoneNormalMappingShaderDest.pTextureName = NULL;

	////노말맵 스킨 기본 쉐이더리소스값
	//m_SkinNormalMappingShaderDest.vertexShaderName = L"../../data/CharacterLoadFile/shader/Skin.hlsl";
	//m_SkinNormalMappingShaderDest.vertexFuntionNmae = "VS_NORMAL";
	//m_SkinNormalMappingShaderDest.PixelShaderName = L"../../data/CharacterLoadFile/shader/Skin.hlsl";
	//m_SkinNormalMappingShaderDest.PixelFuntionNmae = "TPS_NORMAL";
	//m_SkinNormalMappingShaderDest.GiometryShaderName = NULL;
	//m_SkinNormalMappingShaderDest.GiometryFuntionNmae = NULL;
	//m_SkinNormalMappingShaderDest.pTextureName = NULL;

#endif

	//테이블 경로
	m_LAFilePath.m_FirstFilepatch[(int)E_LoadTableType::CHARACTER] = L"../../data/CharacterLoadFile/CharacterTable.txt";
	m_LAFilePath.m_FirstFilepatch[(int)E_LoadTableType::MONSTER] = L"../../data/CharacterLoadFile/MonsterTable.txt";
	m_LAFilePath.m_FirstFilepatch[(int)E_LoadTableType::WEAPON] = L"../../data/CharacterLoadFile/WeaponTable.txt";
	m_LAFilePath.m_FirstFilepatch[(int)E_LoadTableType::ARMOR] = L"../../data/CharacterLoadFile/ArmorTable.txt";
	m_LAFilePath.m_FirstFilepatch[(int)E_LoadTableType::OBJECT] = L"../../data/CharacterLoadFile/ObjectTable.txt";
	m_LAFilePath.m_FirstFilepatch[(int)E_LoadTableType::MAP_OBJECT] = L"../../data/CharacterLoadFile/MapObjectTable.txt";

	//character
	m_LAFilePath.m_FilePatch[0] = L"../../data/CharacterLoadFile/Character/matrix/";
	m_LAFilePath.m_FilePatch[1] = L"../../data/CharacterLoadFile/Character/skin/";
	m_LAFilePath.m_FilePatch[2] = L"../../data/CharacterLoadFile/Character/animation/";
	//monster
	m_LAFilePath.m_FilePatch[3] = L"../../data/CharacterLoadFile/Monster/matrix/";
	m_LAFilePath.m_FilePatch[4] = L"../../data/CharacterLoadFile/Monster/skin/";
	m_LAFilePath.m_FilePatch[5] = L"../../data/CharacterLoadFile/Monster/animation/";
	//weapon
	m_LAFilePath.m_FilePatch[6] = L"../../data/CharacterLoadFile/Weapon/matrix/";
	m_LAFilePath.m_FilePatch[7] = L"../../data/CharacterLoadFile/Weapon/skin/";
	m_LAFilePath.m_FilePatch[8] = L"../../data/CharacterLoadFile/Weapon/animation/";
	//armor
	m_LAFilePath.m_FilePatch[9] = L"../../data/CharacterLoadFile/Armor/matrix/";
	m_LAFilePath.m_FilePatch[10] = L"../../data/CharacterLoadFile/Armor/skin/";
	m_LAFilePath.m_FilePatch[11] = L"../../data/CharacterLoadFile/Armor/animation/";
	//object
	m_LAFilePath.m_FilePatch[12] = L"../../data/CharacterLoadFile/Object/matrix/";
	m_LAFilePath.m_FilePatch[13] = L"../../data/CharacterLoadFile/Object/skin/";
	m_LAFilePath.m_FilePatch[14] = L"../../data/CharacterLoadFile/Object/animation/";
	//mapObject
	m_LAFilePath.m_FilePatch[15] = L"../../data/CharacterLoadFile/MapObject/matrix/";
	m_LAFilePath.m_FilePatch[16] = L"../../data/CharacterLoadFile/MapObject/skin/";
	m_LAFilePath.m_FilePatch[17] = L"../../data/CharacterLoadFile/MapObject/animation/";

	//애니메이션 테이블 경로
	m_LAFilePath.m_FileAnimationPatch[0] = L"../../data/CharacterLoadFile/AnimationCharacterTable.txt";
	m_LAFilePath.m_FileAnimationPatch[1] = L"../../data/CharacterLoadFile/AnimationMonsterTable.txt";

	//사운드 경로 저장
	m_LAFilePath.m_FileSoundPatch[(int)E_LoadTableType::CHARACTER] = "../../data/CharacterLoadFile/Character/sound/";
	m_LAFilePath.m_FileSoundPatch[(int)E_LoadTableType::MONSTER] = "../../data/CharacterLoadFile/Monster/sound/";
	m_LAFilePath.m_FileSoundPatch[(int)E_LoadTableType::WEAPON] = "../../data/CharacterLoadFile/Weapon/sound/";
	m_LAFilePath.m_FileSoundPatch[(int)E_LoadTableType::ARMOR] = "../../data/CharacterLoadFile/Armor/sound/";
	m_LAFilePath.m_FileSoundPatch[(int)E_LoadTableType::OBJECT] = "../../data/CharacterLoadFile/Object/sound/";
	m_LAFilePath.m_FileSoundPatch[(int)E_LoadTableType::MAP_OBJECT] = "../../data/CharacterLoadFile/MapObject/sound/";
	m_LAFilePath.m_FileSoundPatch[(int)E_LoadTableType::SKILL_HITBOX] = "../../data/CharacterLoadFile/SkillSound/";

	//스킬 테이블 경로
	m_LAFilePath.m_SkillFilePatch[((int)E_LoadTableType::SKILL_HITBOX) - 6] = L"../../data/CharacterLoadFile/SkillHitboxTable.txt";
	m_LAFilePath.m_SkillFilePatch[((int)E_LoadTableType::SKILL_BULLET) - 6] = L"../../data/CharacterLoadFile/SkillBulletTable.txt";
	m_LAFilePath.m_SkillFilePatch[((int)E_LoadTableType::SKILL_BUFF) - 6] = L"../../data/CharacterLoadFile/SkillBuffTable.txt";
	m_LAFilePath.m_SkillFilePatch[((int)E_LoadTableType::SKILL_TRAP) - 6] = L"../../data/CharacterLoadFile/SkillTrapTable.txt";
}

bool LAResourceManager::Frame()
{
	return true;
}

bool LAResourceManager::Render()
{
	return true;
}

bool LAResourceManager::Release()
{
	for (m_ResourceListItor = m_ResourceList.begin(); m_ResourceListItor != m_ResourceList.end(); m_ResourceListItor++)
	{
		(m_ResourceListItor->second).reset();
	}
	m_ResourceList.clear();

	return true;
}

bool LAResourceManager::AddResource(LAResource& rResource, CrateObjectDest* pMatrixDest, CrateObjectDest* pSkinDest)
{
	m_ResourceListItor = m_ResourceList.find(GetResourceCodeNameIndex(rResource.m_ResourceCodeName));

	if (m_ResourceListItor == m_ResourceList.end())
	{
		m_ResourceList.insert(std::make_pair(m_iResourceCount, std::make_shared<LAResource>(rResource)));

		CreateResource(m_iResourceCount++, pMatrixDest, pSkinDest, false);

		return true;
	}

	return false;
}

void LAResourceManager::CreateResource(int iResourceNumber, CrateObjectDest* pMatrixDest, CrateObjectDest* pSkinDest, bool bLoadShadow)
{
	//리소스 Create() 확인 안되어있을경우 Create
	m_ResourceListItor = m_ResourceList.find(iResourceNumber);
	LAResource* pData = m_ResourceListItor->second.get();
	if (pData)
	{
		/*====== 쉐이더 생성 =====*/
		if (CreateResourceShader(pData, pMatrixDest, pSkinDest))
		{
			if ((int)pData->m_BoneObjectList.size())
			{
				for (int iBoneObj = 0; iBoneObj < (int)pData->m_BoneObjectList.size(); iBoneObj++)
				{
					if (!pData->m_BoneObjectList[iBoneObj]->m_bIsCreate)
					{
						pData->m_BoneObjectList[iBoneObj]->Init(m_pDevice, m_pContext);
						pData->m_BoneObjectList[iBoneObj]->SetBlob(pData->m_pBoneVSBlob);
						if (pMatrixDest)
						{
							pData->m_BoneObjectList[iBoneObj]->Create(*pMatrixDest);
						}
						else
						{
							pData->m_BoneObjectList[iBoneObj]->Create(m_BoneShaderDest);
						}
						pData->m_BoneObjectList[iBoneObj]->SetCrrentTick(true);
						pData->m_BoneObjectList[iBoneObj]->m_bIsCreate = true;
						pData->m_BoneObjectList[iBoneObj]->SetResourceVS(pData->m_pBoneVS);
						pData->m_BoneObjectList[iBoneObj]->SetResourcePS(pData->m_pBonePS);
						/*if (pData->m_BoneObjectList[iBoneObj]->m_pDummyRenderBox)
						{
						pData->m_BoneObjectList[iBoneObj]->m_pDummyRenderBox->SetBlob(pData->m_pBoneVSBlob);
						pData->m_BoneObjectList[iBoneObj]->m_pDummyRenderBox->SetResourceVS(pData->m_pBoneVS);
						pData->m_BoneObjectList[iBoneObj]->m_pDummyRenderBox->SetResourcePS(pData->m_pBonePS);
						}*/
					}
				}
			}

			if ((int)pData->m_SkinObjectList.size())
			{
				for (int iSkinObj = 0; iSkinObj < (int)pData->m_SkinObjectList.size(); iSkinObj++)
				{
					if (!pData->m_SkinObjectList[iSkinObj]->m_bIsCreate)
					{
						pData->m_SkinObjectList[iSkinObj]->Init(m_pDevice, m_pContext);
						pData->m_SkinObjectList[iSkinObj]->SetBlob(pData->m_pSkinVSBlob);
						if (pSkinDest)
						{
							pData->m_SkinObjectList[iSkinObj]->Create(*pSkinDest);
						}
						else
						{
							pData->m_SkinObjectList[iSkinObj]->Create(m_SkinShaderDest);
						}
						pData->m_SkinObjectList[iSkinObj]->SetCrrentTick(true);
						pData->m_SkinObjectList[iSkinObj]->m_bIsCreate = true;

#ifdef TOOL_MODE
						if (bLoadShadow)
						{
							pData->m_SkinObjectList[iSkinObj]->LoadShadowShader();
						}
#else

						if (bLoadShadow)
						{
							pData->m_SkinObjectList[iSkinObj]->LoadShadowShader();
						}
						/*if (pData->m_SkinObjectList[iSkinObj]->GetNormalMapping() == false)
						{
						pData->m_SkinObjectList[iSkinObj]->CreateLight();
						}*/
#endif

						pData->m_SkinObjectList[iSkinObj]->SetResourceVS(pData->m_pSkinVS);
						pData->m_SkinObjectList[iSkinObj]->SetResourcePS(pData->m_pSkinPS);

					}
				}
			}
		}	//CreateResource
	}
}

static enum { Matirx = 0, Skin = 1 };
bool LAResourceManager::CreateResourceShader(LAResource* pResource, CrateObjectDest* pMatrixDest, CrateObjectDest* pSkinDest)
{
	CrateObjectDest* pDest = NULL;

	for (int iCnt = 0; iCnt < 2; iCnt++)
	{
		if (iCnt == Matirx)
		{
			if (pMatrixDest == NULL)
			{
				pDest = &m_BoneShaderDest;
			}
			else
			{
				pDest = pMatrixDest;
			}
		}

		if (iCnt == Skin)
		{
			if (pSkinDest == NULL)
			{
				pDest = &m_SkinShaderDest;
			}
			else
			{
				pDest = pSkinDest;
			}
		}


		HRESULT hr = S_OK;
		ID3DBlob* pPSBuf = NULL;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
		//DWORD dwShaderFlags = D3DCOMPILE_DEBUG;

		/*============ CreateVertexShader =============*/
		if (pDest->vertexFuntionNmae == NULL)
		{
			if (iCnt == Matirx)
			{
				hr = D3DCompileFromFile(pDest->vertexShaderName, NULL, NULL, "VS", "vs_5_0", NULL, NULL, &pResource->m_pBoneVSBlob, NULL);
			}
			else
			{
				hr = D3DCompileFromFile(pDest->vertexShaderName, NULL, NULL, "VS", "vs_5_0", NULL, NULL, &pResource->m_pSkinVSBlob, NULL);
			}
		}
		else
		{
			if (iCnt == Matirx)
			{
				hr = D3DCompileFromFile(pDest->vertexShaderName, NULL, NULL, pDest->vertexFuntionNmae, "vs_5_0", NULL, NULL, &pResource->m_pBoneVSBlob, NULL);
			}
			else
			{
				hr = D3DCompileFromFile(pDest->vertexShaderName, NULL, NULL, pDest->vertexFuntionNmae, "vs_5_0", NULL, NULL, &pResource->m_pSkinVSBlob, NULL);
			}

		}
		if (hr == E_FAIL)
		{
			OutputDebugString(L"\n Not Creat VertexShader \n");
			return false;
		}
		if (iCnt == Matirx)
		{
			m_pDevice->CreateVertexShader((DWORD*)pResource->m_pBoneVSBlob->GetBufferPointer(), pResource->m_pBoneVSBlob->GetBufferSize(), NULL, &pResource->m_pBoneVS);
		}
		else
		{
			m_pDevice->CreateVertexShader((DWORD*)pResource->m_pSkinVSBlob->GetBufferPointer(), pResource->m_pSkinVSBlob->GetBufferSize(), NULL, &pResource->m_pSkinVS);
		}


		/*============ CreatePixelShader =============*/
		if (pDest->PixelFuntionNmae == NULL)
		{
			hr = D3DCompileFromFile(pDest->PixelShaderName, NULL, NULL, "PS", "ps_5_0", NULL, NULL, &pPSBuf, NULL);
		}
		else
		{
			hr = D3DCompileFromFile(pDest->PixelShaderName, NULL, NULL, pDest->PixelFuntionNmae, "ps_5_0", NULL, NULL, &pPSBuf, NULL);
		}
		if (hr == E_FAIL)
		{
			OutputDebugString(L"\n Not Creat VertexShader \n");
			return false;
		}
		if (iCnt == Matirx)
		{
			m_pDevice->CreatePixelShader((DWORD*)pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &pResource->m_pBonePS);
		}
		else
		{
			m_pDevice->CreatePixelShader((DWORD*)pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), NULL, &pResource->m_pSkinPS);
		}
	}
}

int LAResourceManager::GetResourceIndex(std::wstring codeName, std::wstring matrixname, std::wstring skinName)
{
	int CheckIndex = -1;

	if (codeName != L"NONE")
	{
		CheckIndex = GetResourceCodeNameIndex(codeName);
	}

	if (CheckIndex == -1)
	{
		if (matrixname != L"NONE")
		{
			CheckIndex = GetResourceMatirxNameIndex(matrixname);
		}

		if (CheckIndex == -1)
		{
			if (skinName != L"NONE")
			{
				CheckIndex = GetResourceSkinNameIndex(skinName);
			}

			if (CheckIndex == -1)
			{
				return CheckIndex;
			}
		}
	}


	return CheckIndex;
}

int LAResourceManager::GetResourceCodeNameIndex(std::wstring codeName)
{
	if (m_ResourceList.size() == 0)
		return -1;

	for (m_ResourceListItor = m_ResourceList.begin();
		m_ResourceListItor != m_ResourceList.end();
		m_ResourceListItor++)
	{
		if (codeName.compare(m_ResourceListItor->second->m_ResourceCodeName) == 0)
		{
			return m_ResourceListItor->first;
		}
	}

	return -1;
}

int LAResourceManager::GetResourceMatirxNameIndex(std::wstring matrixname)
{
	if (m_ResourceList.size() == 0)
		return -1;

	for (m_ResourceListItor = m_ResourceList.begin();
		m_ResourceListItor != m_ResourceList.end();
		m_ResourceListItor++)
	{
		if (matrixname.compare(m_ResourceListItor->second->m_ResourceMatrixName) == 0)
		{
			return m_ResourceListItor->first;
		}
	}

	return -1;
}

int LAResourceManager::GetResourceSkinNameIndex(std::wstring skinName)
{
	if (m_ResourceList.size() == 0)
		return -1;

	for (m_ResourceListItor = m_ResourceList.begin();
		m_ResourceListItor != m_ResourceList.end();
		m_ResourceListItor++)
	{
		if (skinName.compare(m_ResourceListItor->second->m_ResourceSkinName) == 0)
		{
			return m_ResourceListItor->first;
		}
	}

	return -1;
}

int LAResourceManager::GetResourceCodeNameIndex(int iResourceIndex)
{
	if (m_ResourceList.size() == 0)
		return -1;

	m_ResourceListItor = m_ResourceList.find(iResourceIndex);
	if (m_ResourceListItor == m_ResourceList.end())
	{
		return -1;
	}

	return m_ResourceListItor->first;
}


LAResource* LAResourceManager::GetResourceAdress(std::wstring wsCharacterName)
{
	if (m_ResourceList.size() == 0)
		return NULL;

	for (m_ResourceListItor = m_ResourceList.begin();
		m_ResourceListItor != m_ResourceList.end();
		m_ResourceListItor++)
	{
		if (wsCharacterName.compare(m_ResourceListItor->second->m_ResourceCodeName) == 0)
		{
			return m_ResourceListItor->second.get();
		}
	}

	return NULL;
}

LAResource* LAResourceManager::GetResourceAdressSkinName(std::wstring wsCharacterName)
{
	if (m_ResourceList.size() == 0)
		return NULL;

	for (m_ResourceListItor = m_ResourceList.begin();
		m_ResourceListItor != m_ResourceList.end();
		m_ResourceListItor++)
	{
		if (wsCharacterName.compare(m_ResourceListItor->second->m_ResourceSkinName) == 0)
		{
			return m_ResourceListItor->second.get();
		}
	}
}

LAResource* LAResourceManager::GetResourceAdress(int iResourceNumber)
{
	if (m_ResourceList.size() == 0)
		return NULL;

	m_ResourceListItor = m_ResourceList.find(iResourceNumber);
	if (m_ResourceListItor != m_ResourceList.end())
	{
		return m_ResourceListItor->second.get();
	}

	return NULL;
}

CrateObjectDest LAResourceManager::GetCreateObjectDest(E_LoadObjectType e_Type)
{
	switch ((int)e_Type)
	{
	case (int)E_LoadObjectType::CHARACTER_MATRIX:
	{
		return m_BoneShaderDest;
	}break;
	case (int)E_LoadObjectType::CHARACTER_SKIN:
	{
		return m_SkinShaderDest;
	}break;
	}
}

LAResourceManager::LAResourceManager()
{
	m_iResourceCount = 0;
	m_pDevice = NULL;
	m_pContext = NULL;
}


LAResourceManager::~LAResourceManager()
{
}
