
//#include "stdafx.h"



#include "LAPaser.h"

#include "LALoadObjectManager.h"
#include "LAResourceManager.h"

bool LAPaser::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pCoreDevice = pDevice;
	m_pCoreContext = pContext;

	return true;
}

bool LAPaser::OpenStream(FILE** pFile, const TCHAR* pFileName, TCHAR* mode)
{
	_tfopen_s(pFile, pFileName, mode);
	if (*pFile == NULL)
		return false;

	return true;
}

bool LAPaser::CloseStream(FILE** pFile)
{
	if (*pFile == NULL)
		return false;

	fclose(*pFile);

	pFile = NULL;

	return true;
}

//void LAPaser::SearchToObjectParent(LALoadObject* rLoadObjectList)
//{
//	TMatrix matInverseParent;
//
//	for (int iCnt = 0; iCnt < (int)rLoadObjectList->m_BoneObjectList.size(); iCnt++)
//	{
//		if (!rLoadObjectList->m_BoneObjectList[iCnt]->m_MeshData->wsParentName.empty())
//		{
//			for (int jCnt = 0; jCnt < (int)rLoadObjectList->m_BoneObjectList.size(); jCnt++)
//			{
//				if (rLoadObjectList->m_BoneObjectList[iCnt]->m_MeshData->wsParentName ==
//					rLoadObjectList->m_BoneObjectList[jCnt]->m_MeshData->wsNodeName)
//				{
//					//parent �ּ� ����
//					rLoadObjectList->m_BoneObjectList[iCnt]->m_pmatParent = &rLoadObjectList->m_BoneObjectList[jCnt]->m_matCalculation;
//
//					//�ڽ���� ���
//					TMatrixInverse(&matInverseParent, NULL, &rLoadObjectList->m_BoneObjectList[jCnt]->m_MeshData->matLoadObjectWorld);
//					rLoadObjectList->m_BoneObjectList[iCnt]->m_matChildWorld =
//						rLoadObjectList->m_BoneObjectList[iCnt]->m_MeshData->matLoadObjectWorld * matInverseParent;
//
//					break;
//				}
//
//			}
//		}
//	}
//
//	for (int iCnt = 0; iCnt < (int)rLoadObjectList->m_SkinObjectList.size(); iCnt++)
//	{
//		if (!rLoadObjectList->m_SkinObjectList[iCnt]->m_MeshData->wsParentName.empty())
//		{
//			for (int jCnt = 0; jCnt < (int)rLoadObjectList->m_BoneObjectList.size(); jCnt++)
//			{
//				if (rLoadObjectList->m_SkinObjectList[iCnt]->m_MeshData->wsParentName ==
//					rLoadObjectList->m_BoneObjectList[jCnt]->m_MeshData->wsNodeName)
//				{
//					//parent �ּ� ����
//					rLoadObjectList->m_SkinObjectList[iCnt]->m_pmatParent = &rLoadObjectList->m_BoneObjectList[jCnt]->m_matCalculation;
//
//					//�ڽ���� ���
//					TMatrixInverse(&matInverseParent, NULL, &rLoadObjectList->m_BoneObjectList[jCnt]->m_MeshData->matLoadObjectWorld);
//					rLoadObjectList->m_SkinObjectList[iCnt]->m_matChildWorld =
//						rLoadObjectList->m_SkinObjectList[iCnt]->m_MeshData->matLoadObjectWorld * matInverseParent;
//
//					break;
//				}
//
//			}
//
//			//�θ��尡 ��Ʈ���� �����Ϳ� ���ٸ� ��Ų�߿��� �ٽ� �˻�
//			for (int jCnt = 0; jCnt < (int)rLoadObjectList->m_SkinObjectList.size(); jCnt++)
//			{
//				if (rLoadObjectList->m_SkinObjectList[iCnt]->m_MeshData->wsParentName ==
//					rLoadObjectList->m_SkinObjectList[jCnt]->m_MeshData->wsNodeName)
//				{
//					//parent �ּ� ����
//					rLoadObjectList->m_SkinObjectList[iCnt]->m_pmatParent = &rLoadObjectList->m_SkinObjectList[jCnt]->m_matCalculation;
//
//					//�ڽ���� ���
//					TMatrixInverse(&matInverseParent, NULL, &rLoadObjectList->m_SkinObjectList[jCnt]->m_MeshData->matLoadObjectWorld);
//					rLoadObjectList->m_SkinObjectList[iCnt]->m_matChildWorld =
//						rLoadObjectList->m_SkinObjectList[iCnt]->m_MeshData->matLoadObjectWorld * matInverseParent;
//
//					break;
//				}
//
//			}
//
//		}
//	}
//}


std::shared_ptr<LALoadObject> LAPaser::Load(const TCHAR* pFileName, E_LoadParserType eLoadType)
{
	std::shared_ptr<LALoadObject> pAddObject;

	pAddObject = std::make_shared<LALoadObject>();
	//TypeNumber ����
	TCHAR drive[256] = { 0 };
	TCHAR dir[256] = { 0 };
	TCHAR filename[256] = { 0 };
	TCHAR ext[256] = { 0 };
	_tsplitpath_s(pFileName, drive, _countof(drive), dir, _countof(dir), filename, _countof(filename), ext, _countof(ext));

	if (OpenStream(&m_pFile, pFileName, L"rt"))
	{

		switch ((int)eLoadType)
		{
		case (int)E_LoadParserType::ASE_TYPE:
		{
			/*m_AseParser.Init(m_pCoreDevice, m_pCoreContext);
			if (m_AseParser.Load(m_pFile))
			{
				*pAddObject = *m_AseParser.Convert();
				m_AseParser.Release();
			}*/
		}break;

		case (int)E_LoadParserType::PSB_TYPE:
		{
			/*m_PsbParser.Init(m_pCoreDevice, m_pCoreContext);
			if (m_PsbParser.Load(m_pFile, E_LoadObjectType::NONE))
			{
				*pAddObject = *m_PsbParser.Convert(L"", E_LoadObjectType::NONE);
				m_PsbParser.Release();
				pAddObject->m_LoadMaxObjectName = filename;
				pAddObject->m_LoadMaxObjectName += ext;
			}*/
		}break;
		}

		CloseStream(&m_pFile);

		if (pAddObject != NULL)
		{
			//SearchToObjectParent(pAddObject.get());
		}
	}
	else
	{
		return NULL;
	}

	return pAddObject;
}

int LAPaser::LoadTool(std::wstring wsMatrixResourceName, std::wstring wsSkinResourceName, std::wstring wsAniResourceName, const TCHAR* pFileName, E_LoadObjectType eLoadObjectType, bool bBinary, int iLoadType, CrateObjectDest* pMatrixDest, CrateObjectDest* pSkinDest)
{
	int iResultNumber = 0;

	if (bBinary)
	{
		OpenStream(&m_pFile, pFileName, L"rb");
	}
	else
	{
		OpenStream(&m_pFile, pFileName, L"rt");
	}

	if (m_pFile != NULL)
	{
		//int iType = 0;

		//if (bBinary)
		//{

		//	//fread(&iType, sizeof(int), 1, m_pFile);
		//}
		//else
		//{
		//	_fgetts(m_pString, 256, m_pFile);
		//	_stscanf_s(m_pString, _T("%s %d"),
		//		m_pBuffer, _countof(m_pBuffer),
		//		&iType
		//	);

		//	/*std::wstring checkType = m_pString;
		//	if (checkType.compare(L"#PSB_CHARACTER_TOOL_MATRIX_OBJECT_DATA\n") == 0)
		//		iType = 1;
		//	if (checkType.compare(L"#PSB_CHARACTER_TOOL_SKIN_OBJECT_DATA\n") == 0)
		//		iType = 2;
		//	if (checkType.compare(L"#PSB_CHARACTER_TOOL_ANIMATION_DATA\n") == 0)
		//		iType = 3;*/


		//	//Ŀ�� ó������
		//	fseek(m_pFile, 0L, SEEK_SET);
		//}

		switch (iLoadType)
		{
		case 1:
		{
			m_PsbParser.Init(m_pCoreDevice, m_pCoreContext);
			if (m_PsbParser.Load(m_pFile, E_LoadObjectType::CHARACTER_MATRIX, bBinary))
			{
				//�ִϸ��̼� ������ �ε�
				m_PsbParser.Convert(0, wsMatrixResourceName, wsSkinResourceName, E_LoadObjectType::CHARACTER_MATRIX, NULL, NULL, false);
			}
			m_PsbParser.Release();

			iResultNumber = 1;
		}break;

		case 2:
		{
			m_PsbParser.Init(m_pCoreDevice, m_pCoreContext);
			if (m_PsbParser.Load(m_pFile, E_LoadObjectType::CHARACTER_SKIN, bBinary))
			{
				//�ִϸ��̼� ������ �ε�
				m_PsbParser.Convert(0, wsSkinResourceName, wsSkinResourceName, E_LoadObjectType::CHARACTER_SKIN, NULL, NULL, true);
			}
			m_PsbParser.Release();

			iResultNumber = 2;
		}break;

		case 3:
		{
			m_PsbParser.Init(m_pCoreDevice, m_pCoreContext);
			if (m_PsbParser.Load(m_pFile, E_LoadObjectType::CHARACTER_ANIMATION, bBinary))
			{
				//�ִϸ��̼� ������ �ε�
				m_PsbParser.Convert(0, wsAniResourceName, wsSkinResourceName, E_LoadObjectType::CHARACTER_ANIMATION, NULL, NULL, false);
			}
			m_PsbParser.Release();

			iResultNumber = 3;
		}break;
		}
	}
	else
	{
		//���Ͽ��� ������ üũ�ϴ� �κ�
		return -1;
	}
	CloseStream(&m_pSubFile);

	return iResultNumber;
}

bool LAPaser::LoadEngine(const TCHAR* pFileName, E_LoadObjectType LoadObjectType, bool bBinary, CrateObjectDest* pMatrixDest, CrateObjectDest* pSkinDest)
{
	//TypeNumber ����
	TCHAR drive[256] = { 0 };
	TCHAR dir[256] = { 0 };
	TCHAR filename[256] = { 0 };
	TCHAR ext[256] = { 0 };
	_tsplitpath_s(pFileName, drive, _countof(drive), dir, _countof(dir), filename, _countof(filename), ext, _countof(ext));

	if (OpenStream(&m_pFile, pFileName, L"rt"))
	{
		E_LoadObjectType type;
		//ī���� �ε�
		int Count = 0;
		_fgetts(m_pString, 256, m_pFile);
		_stscanf_s(m_pString, _T("%s %d"),
			m_pBuffer, _countof(m_pBuffer),
			&Count);
				
		for (int iRoadCnt = 0; iRoadCnt < Count; iRoadCnt++)
		{
			//ĳ���� �̸�
			TCHAR ResourceName[256] = { 0 };
			int ResourceIndex;

			_fgetts(m_pString, 256, m_pFile);
			_fgetts(m_pString, 256, m_pFile);
			_stscanf_s(m_pString, _T("%d %s"),
				&ResourceIndex,
				ResourceName, _countof(ResourceName)
				);

			//�⺻���
			std::wstring LoadCharacterFile;
			switch ((int)LoadObjectType)
			{
				case (int)E_LoadObjectType::CHARACTER:
				{
					LoadCharacterFile = L"../../data/CharacterLoadFile/Character/";
				}break;

				case (int)E_LoadObjectType::CHARACTER_ITEM:
				{
					LoadCharacterFile = L"../../data/CharacterLoadFile/CharacterItem/";
				}break;

				case (int)E_LoadObjectType::DECO:
				{
					LoadCharacterFile = L"../../data/CharacterLoadFile/Deco/";
				}break;
			}
			

			//3�� �ݺ� ��Ʈ����, ��Ų, �ִ�
			for (int iCnt = 0; iCnt < 3; iCnt++)
			{
				switch (iCnt)
				{
					case 0:
					{
						//SubCount
						int SubCount = 0;
						_fgetts(m_pString, 256, m_pFile);
						_fgetts(m_pString, 256, m_pFile);
						_stscanf_s(m_pString, _T("%s %d"),
							m_pBuffer, _countof(m_pBuffer),
							&SubCount);

						for (int iLoadCnt = 0; iLoadCnt < SubCount; iLoadCnt++)
						{
							std::wstring fullpatch = LoadCharacterFile;
							type = E_LoadObjectType::CHARACTER_MATRIX;

							_fgetts(m_pString, 256, m_pFile);
							_stscanf_s(m_pString, _T("%s"),
								m_pBuffer, _countof(m_pBuffer)
							);

							fullpatch += m_pBuffer;

							if (OpenStream(&m_pSubFile, fullpatch.c_str(), L"rt"))
							{
								m_PsbParser.Init(m_pCoreDevice, m_pCoreContext);
								if (m_PsbParser.Load(m_pSubFile, type, bBinary))
								{
									m_PsbParser.Convert(ResourceIndex, ResourceName, L"", type, pMatrixDest, pSkinDest, false);
								}
								m_PsbParser.Release();
							}
							else
							{
								//���Ͽ��� ������ üũ�ϴ� �κ�
								return false;
							}
							CloseStream(&m_pSubFile);
						}
					}break;

					case 1:
					{
						type = E_LoadObjectType::CHARACTER_SKIN;
						//std::wstring convertname;

						//SubCount
						int SubCount = 0;
						_fgetts(m_pString, 256, m_pFile);
						_fgetts(m_pString, 256, m_pFile);
						_stscanf_s(m_pString, _T("%s %d"),
							m_pBuffer, _countof(m_pBuffer),
							&SubCount);


						for (int iLoadCnt = 0; iLoadCnt < SubCount; iLoadCnt++)
						{
							std::wstring fullpatch = LoadCharacterFile;
							TCHAR ParentMatrixName[256] = { 0 };
							_fgetts(m_pString, 256, m_pFile);
							_stscanf_s(m_pString, _T("%s %s"),
								ParentMatrixName, _countof(ParentMatrixName),
								m_pBuffer, _countof(m_pBuffer)
							);
							//convertname = ParentMatrixName;

							fullpatch += m_pBuffer;

							if (OpenStream(&m_pSubFile, fullpatch.c_str(), L"rt"))
							{
								m_PsbParser.Init(m_pCoreDevice, m_pCoreContext);
								if (m_PsbParser.Load(m_pSubFile, type, bBinary))
								{
									m_PsbParser.Convert(ResourceIndex, ResourceName, ParentMatrixName, type, pMatrixDest, pSkinDest, false);
								}
								m_PsbParser.Release();
							}
							else
							{
								//���Ͽ��� ������ üũ�ϴ� �κ�
								return false;
							}
							CloseStream(&m_pSubFile);
						}
					}break;

					case 2:
					{
						type = E_LoadObjectType::CHARACTER_ANIMATION;
						std::wstring convertname;

						//SubCount
						int SubCount = 0;
						_fgetts(m_pString, 256, m_pFile);
						_fgetts(m_pString, 256, m_pFile);
						_stscanf_s(m_pString, _T("%s %d"),
							m_pBuffer, _countof(m_pBuffer),
							&SubCount);

						for (int iLoadCnt = 0; iLoadCnt < SubCount; iLoadCnt++)
						{
							std::wstring fullpatch = LoadCharacterFile;
							TCHAR ParentMatrixName[256] = { 0 };
							_fgetts(m_pString, 256, m_pFile);
							_stscanf_s(m_pString, _T("%s %s"),
								ParentMatrixName, _countof(ParentMatrixName),
								m_pBuffer, _countof(m_pBuffer)
							);
							convertname = ParentMatrixName;
							fullpatch += m_pBuffer;

							LoadFile(ResourceIndex, fullpatch.c_str(), convertname.c_str(), L"", type, pMatrixDest, pSkinDest);
						}
					}break;
				}	 //switch
			}	 //for
		}	//for
		CloseStream(&m_pFile);
	}	//if
	else
	{
		return NULL;
	}
}

bool LAPaser::LoadTable(E_LoadTableType loadtabletype)
{
	switch ((int)loadtabletype)
	{
		//�ε�Ϸ�
		case (int)E_LoadTableType::CHARACTER:
		{
			if (OpenStream(&m_pFile, RESOURCE_MGR.m_LAFilePath.m_FirstFilepatch[(int)E_LoadTableType::CHARACTER].c_str(), L"rt"))
			{
				LoadCharacterTable();

				CloseStream(&m_pFile);
			}
			else
			{
				return false;
			}

		}break;

		//�ε�Ϸ�
		case (int)E_LoadTableType::MONSTER:
		{
			if (OpenStream(&m_pFile, RESOURCE_MGR.m_LAFilePath.m_FirstFilepatch[(int)E_LoadTableType::MONSTER].c_str(), L"rt"))
			{
				LoadMonsterTable();

				CloseStream(&m_pFile);
			}
			else
			{
				return false;
			}
		}break;

		//�ε�Ϸ�
		case (int)E_LoadTableType::WEAPON:
		{
			if (OpenStream(&m_pFile, RESOURCE_MGR.m_LAFilePath.m_FirstFilepatch[(int)E_LoadTableType::WEAPON].c_str(), L"rt"))
			{
				LoadWeaponTable();

				CloseStream(&m_pFile);
			}
			else
			{
				return false;
			}
		}break;

		case (int)E_LoadTableType::ARMOR:
		{
			if (OpenStream(&m_pFile, RESOURCE_MGR.m_LAFilePath.m_FirstFilepatch[(int)E_LoadTableType::ARMOR].c_str(), L"rt"))
			{
				LoadArmorTable();

				CloseStream(&m_pFile);
			}
			else
			{
				return false;
			}
		}break;

		case (int)E_LoadTableType::SKILL_HITBOX:
		{
			if (OpenStream(&m_pFile, RESOURCE_MGR.m_LAFilePath.m_SkillFilePatch[((int)E_LoadTableType::SKILL_HITBOX) - 6].c_str(), L"rt"))
			{
				LoadSkillHitbox();

				CloseStream(&m_pFile);
			}
			else
			{
				return false;
			}
		}break;

		case (int)E_LoadTableType::SKILL_BULLET:
		{
			if (OpenStream(&m_pFile, RESOURCE_MGR.m_LAFilePath.m_SkillFilePatch[((int)E_LoadTableType::SKILL_BULLET) - 6].c_str(), L"rt"))
			{
				LoadSkillbullet();

				CloseStream(&m_pFile);
			}
			else
			{
				return false;
			}
		}break;

		case (int)E_LoadTableType::SKILL_BUFF:
		{
			if (OpenStream(&m_pFile, RESOURCE_MGR.m_LAFilePath.m_SkillFilePatch[((int)E_LoadTableType::SKILL_BUFF) - 6].c_str(), L"rt"))
			{
				LoadSkillBuff();

				CloseStream(&m_pFile);
			}
			else
			{
				return false;
			}
		}break;

		case (int)E_LoadTableType::SKILL_TRAP:
		{
			if (OpenStream(&m_pFile, RESOURCE_MGR.m_LAFilePath.m_SkillFilePatch[((int)E_LoadTableType::SKILL_TRAP) - 6].c_str(), L"rt"))
			{
				LoadSkillTrap();

				CloseStream(&m_pFile);
			}
			else
			{
				return false;
			}
		}break;

		default:
		{
			return false;
		}
	}

	return true;
}

bool LAPaser::LoadCharacterTable()
{
	int Tableindex = -1; 
	float Scale = 0.0f;
	int Hp = 0; 
	float Def = 0.0f;
	float MoveSpeed = 0.0f;
	TCHAR CodeName[256] = { 0 };
	TCHAR Desc[256] = { 0 };
	TCHAR MatrixPath[256] = { 0 };
	TCHAR SkinPath[256] = { 0 };
	TCHAR AnimationPath[256] = { 0 };
	TCHAR BaseWeapon[5][256] = { 0 };
	TCHAR BaseArmor[5][256] = { 0 };
	TCHAR WorkSound[10][256] = { 0 };

	//_fgetts(m_pString, 256, m_pFile);
	//_fgetts(m_pString, 256, m_pFile);
	//_fgetts(m_pString, 256, m_pFile);
	
	/* File Read */
	int iCount = 0;

	ZeroMemory(m_pString, sizeof(TCHAR) * 500);
	_fgetts(m_pString, 500, m_pFile);
	_stscanf_s(m_pString, _T("%d"), //�ڴ� ���� ���� �ʿ�� �ε�
		&iCount);

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		ZeroMemory(m_pString, sizeof(TCHAR) * 500);
		
		//Index, codeName,  mash, skin, scale, animation, hp, def, movespeed, �⺻����5��, �⺻���5��, �̵��� ����10��
		_fgetts(m_pString, 500, m_pFile);
		_stscanf_s(m_pString, _T("%d %s %s %s %f %s %d %f %f %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s"),
			&Tableindex,
			CodeName, _countof(CodeName),
			MatrixPath, _countof(MatrixPath),
			SkinPath, _countof(SkinPath),
			&Scale,
			AnimationPath, _countof(AnimationPath),
			&Hp,
			&Def,
			&MoveSpeed,
			BaseWeapon[0], _countof(BaseWeapon[0]),
			BaseWeapon[1], _countof(BaseWeapon[1]),
			BaseWeapon[2], _countof(BaseWeapon[2]),
			BaseWeapon[3], _countof(BaseWeapon[3]),
			BaseWeapon[4], _countof(BaseWeapon[4]),
			BaseArmor[0], _countof(BaseArmor[0]),
			BaseArmor[1], _countof(BaseArmor[1]),
			BaseArmor[2], _countof(BaseArmor[2]),
			BaseArmor[3], _countof(BaseArmor[3]),
			BaseArmor[4], _countof(BaseArmor[4]),
			WorkSound[0], _countof(WorkSound[0]),
			WorkSound[1], _countof(WorkSound[1]),
			WorkSound[2], _countof(WorkSound[2]),
			WorkSound[3], _countof(WorkSound[3]),
			WorkSound[4], _countof(WorkSound[4]),
			WorkSound[5], _countof(WorkSound[5]),
			WorkSound[6], _countof(WorkSound[6]),
			WorkSound[7], _countof(WorkSound[7]),
			WorkSound[8], _countof(WorkSound[8]),
			WorkSound[9], _countof(WorkSound[9])
		);

		/*=========================== ���ҽ� �ߺ� �˻� ================================*/
		//���̺��ε��� �ߺ�üũ 
		bool CheckOverlapTableIndex = LOAD_OBJECT_MGR.CheckOverlapTableIndex(E_LoadTableType::CHARACTER, Tableindex);
		if (CheckOverlapTableIndex == true)
			return true;

		int iIndex = -1;
		bool CheckResouce = false;
		iIndex = RESOURCE_MGR.GetResourceSkinNameIndex(SkinPath);

		if (iIndex == -1)
		{
			CheckResouce = LoadResourceData(Tableindex, CodeName, MatrixPath, SkinPath, AnimationPath, E_LoadTableType::CHARACTER, true);
			LAResource* pData = RESOURCE_MGR.GetResourceAdress(CodeName);
			pData->m_ResourceMatrixName = MatrixPath;
			pData->m_ResourceSkinName = SkinPath;
		}
		else
		{
			CheckResouce = true;
		}

		if(CheckResouce)
		{
			ObjectTableLoadData loadData;

			loadData.eObjectType = E_LoadTableType::CHARACTER;
			loadData.iTableIndex = Tableindex;
			loadData.wsCodeName = CodeName;
			loadData.wsMatrixName = MatrixPath;
			loadData.wsSkinName = SkinPath;
			loadData.fScale = Scale;
			loadData.iHp = Hp;
			loadData.fDef = Def;
			loadData.fRunSpeed = MoveSpeed;

			//�⺻����, ���, ���� ���
			for (int iCnt = 0; iCnt < 5; iCnt++)
			{
				loadData.wsCharacterWeaponList.push_back(BaseWeapon[iCnt]);
			}
			for (int iCnt = 0; iCnt < 5; iCnt++)
			{
				loadData.wscharacterArmorList.push_back(BaseArmor[iCnt]);
			}
			for (int iCnt = 0; iCnt < 10; iCnt++)
			{
				loadData.wsSoundNameList.push_back(WorkSound[iCnt]);
			}

			LOAD_OBJECT_MGR.AddObjectDataInfo(loadData);
		}
		else
		{
			//�ε����
			std::wstring ErrorMGG = L"Not Load Character Table Data : Index ";
			ErrorMGG += std::to_wstring(Tableindex);
			ErrorMGG += L"CodeName ";
			ErrorMGG += CodeName;
			OutputDebugString(ErrorMGG.c_str());

		}
	}

	return true;
}

bool LAPaser::LoadMonsterTable()
{
	int Tableindex = -1;
	TCHAR CodeName[256] = { 0 };
	TCHAR MatrixPath[256] = { 0 };
	TCHAR SkinPath[256] = { 0 };
	float Scale = 0.0f;
	TCHAR AnimationPath[256] = { 0 };
	int Hp = 0;
	float Def = 0.0f;
	float MoveSpeed = 0.0f;
	TCHAR SkillType[256] = { 0 };
	TCHAR SkillName[256] = { 0 };
	float AbilityReaction = 0.0f;
	float AbilityAimIncrease = 0.0f;
	TCHAR Bullet_need_Reload[256] = { 0 };
	int AbilityReloadMax = 0;
	float AbilityAttackSpeed = 0.0f;
	TCHAR SoundName[10][255] = { 0 };

	/* File Read */
	int iCount = 0;

	ZeroMemory(m_pString, sizeof(TCHAR) * 500);
	_fgetts(m_pString, 500, m_pFile);
	_stscanf_s(m_pString, _T("%d"), //�ڴ� ���� ���� �ʿ�� �ε�
		&iCount);


	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		ZeroMemory(m_pString, sizeof(TCHAR) * 500);
		
		//Index, codeName,  mash, skin, scale, animation, hp, def, movespeed skilltype skillname �ɷ�ġ5��, ����3��
		_fgetts(m_pString, 500, m_pFile);
		_stscanf_s(m_pString, _T("%d %s %s %s %f %s %d %f %f %s %s %f %f %s %d %f %s %s %s"), //�ڴ� ���� ���� �ʿ�� �ε�
			&Tableindex,
			CodeName, _countof(CodeName),
			MatrixPath, _countof(MatrixPath),
			SkinPath, _countof(SkinPath),
			&Scale,
			AnimationPath, _countof(AnimationPath),
			&Hp,
			&Def,
			&MoveSpeed,
			SkillType, _countof(SkillType),
			SkillName, _countof(SkillName),
			&AbilityReaction,
			&AbilityAimIncrease,
			Bullet_need_Reload, _countof(Bullet_need_Reload),
			&AbilityReloadMax,
			&AbilityAttackSpeed,
			SoundName[0], _countof(SoundName[0]),
			SoundName[1], _countof(SoundName[1]),
			SoundName[2], _countof(SoundName[2])
		);

		/*=========================== ���ҽ� �ߺ� �˻� ================================*/
		//���̺��ε��� �ߺ�üũ 
		bool CheckOverlapTableIndex = LOAD_OBJECT_MGR.CheckOverlapTableIndex(E_LoadTableType::MONSTER, Tableindex);
		if (CheckOverlapTableIndex == true)
			return true;


		int iIndex = -1;
		bool CheckResouce = false;
		iIndex = RESOURCE_MGR.GetResourceSkinNameIndex(SkinPath);

		if (iIndex == -1)
		{
			CheckResouce = LoadResourceData(Tableindex, CodeName, MatrixPath, SkinPath, AnimationPath, E_LoadTableType::MONSTER, true);
			LAResource* pData = RESOURCE_MGR.GetResourceAdress(CodeName);
			pData->m_ResourceMatrixName = MatrixPath;
			pData->m_ResourceSkinName = SkinPath;
		}
		else
		{
			CheckResouce = true;
		}

		if (CheckResouce)
		{
			ObjectTableLoadData loadData;

			loadData.eObjectType = E_LoadTableType::MONSTER;
			loadData.iTableIndex = Tableindex;
			loadData.wsCodeName = CodeName;
			loadData.wsMatrixName = MatrixPath;
			loadData.wsSkinName = SkinPath;
			loadData.fScale = Scale;
			loadData.iHp = Hp;
			loadData.fDef = Def;
			loadData.fRunSpeed = MoveSpeed;

			std::wstring type = SkillType;
			if (type.compare(L"SKILL_HITBOX") == 0) loadData.eSkillType = E_SkillType::SKILL_HITBOX;
			if (type.compare(L"SKILL_BULLET") == 0) loadData.eSkillType = E_SkillType::SKILL_BULLET;
			loadData.wsSkillName = SkillName;
			loadData.fAbilityReaction = AbilityReaction;
			loadData.fAbilityAimIncrease = AbilityAimIncrease;
			type = Bullet_need_Reload;
			if (type.compare(L"TRUE") == 0) loadData.bBullet_need_Reload = true;
			if (type.compare(L"FALSE") == 0) loadData.bBullet_need_Reload = false;
			loadData.iAbilityReloadMax = AbilityReloadMax;
			loadData.fAbilityAttackSpeed = AbilityAttackSpeed;
			loadData.wsSoundNameList.push_back(SoundName[0]);
			loadData.wsSoundNameList.push_back(SoundName[1]);
			loadData.wsSoundNameList.push_back(SoundName[2]);

			LOAD_OBJECT_MGR.AddObjectDataInfo(loadData);
		}
		else
		{
			//�ε����
			std::wstring ErrorMGG = L"Not Load Monster Table Data : Index ";
			ErrorMGG += std::to_wstring(Tableindex);
			ErrorMGG += L"CodeName ";
			ErrorMGG += CodeName;
			OutputDebugString(ErrorMGG.c_str());
		}
	}

	return true;
}

bool LAPaser::LoadWeaponTable()
{
	int Tableindex = -1;
	TCHAR CodeName[256] = { 0 };
	TCHAR WeaponType[256] = { 0 };
	TCHAR IconName[256] = { 0 };
	TCHAR MatrixPath[256] = { 0 };
	TCHAR SkinPath[256] = { 0 };
	float Scale = 0.0f;
	TCHAR Sound[10][256] = { 0 };
	ZeroMemory(Sound, sizeof(TCHAR) * 10 * 256);
	TCHAR SkillType[256] = { 0 };
	TCHAR SkillName[256] = { 0 };
	float AbilityReaction = 0.0f;
	float AbilityAimIncrease = 0.0f;
	TCHAR Bullet_need_Reload[256] = { 0 };
	int AbilityReloadMax = 0;
	float AbilityAttackSpeed = 0.0f;
	TCHAR InvenIconName[256] = { 0 };
	TCHAR InvenItemName[256] = { 0 };
	TCHAR InvenDesc[256] = { 0 };

	/* File Read */
	int iCount = 0;

	ZeroMemory(m_pString, sizeof(TCHAR) * 500);
	_fgetts(m_pString, 500, m_pFile);
	_stscanf_s(m_pString, _T("%d"), //�ڴ� ���� ���� �ʿ�� �ε�
		&iCount);

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		ZeroMemory(m_pString, sizeof(TCHAR) * 500);
	
		//Index, codeName,  mash, skin, scale  sound3�� skilltype  skillname  �ɷ�ġ5��  �κ������ܰ��,�̸�  �κ���ǥ����DESC 
		_fgetts(m_pString, 500, m_pFile);
		_stscanf_s(m_pString, _T("%d %s %s %s %s %s %f %s %s %s %s %s %f %f %s %d %f %s %s %s"), //�ڴ� ���� ���� �ʿ�� �ε�
			&Tableindex,
			CodeName, _countof(CodeName),
			WeaponType, _countof(WeaponType),
			IconName, _countof(IconName),
			MatrixPath, _countof(MatrixPath),
			SkinPath, _countof(SkinPath),
			&Scale,
			Sound[0], _countof(Sound[0]),
			Sound[1], _countof(Sound[1]),
			Sound[2], _countof(Sound[2]),
			SkillType, _countof(SkillType),
			SkillName, _countof(SkillName),
			&AbilityReaction,
			&AbilityAimIncrease,
			Bullet_need_Reload, _countof(Bullet_need_Reload),
			&AbilityReloadMax,
			&AbilityAttackSpeed,
			InvenIconName, _countof(InvenIconName),
			InvenItemName, _countof(InvenItemName),
			InvenDesc, _countof(InvenDesc)
		);
		/*=========================== �κ�ǥ�� �̸� �پ�� ���� ================================*/
		std::wstring ConvertInvenItemName = InvenItemName;
		const TCHAR frome[2] = L"_";
		TCHAR to[2] = L" ";
		for (int ilen = 0; ilen < ConvertInvenItemName.size(); ilen++)
		{
			if (ConvertInvenItemName[ilen] == frome[0])
			{
				ConvertInvenItemName[ilen] = to[0];
			}
		}

		//std::replace(ConvertName.begin(), ConvertName.end(), L"_", L" ");

		/*=========================== ���ҽ� �ߺ� �˻� ================================*/
		//���̺��ε��� �ߺ�üũ 
		bool CheckOverlapTableIndex = LOAD_OBJECT_MGR.CheckOverlapTableIndex(E_LoadTableType::WEAPON, Tableindex);
		if (CheckOverlapTableIndex == true)
			return true;

		int iIndex = -1;
		bool CheckResouce = false;
		iIndex = RESOURCE_MGR.GetResourceSkinNameIndex(SkinPath);

		if (iIndex == -1)
		{
			CheckResouce = LoadResourceData(Tableindex, CodeName, MatrixPath, SkinPath, L"NONE", E_LoadTableType::WEAPON, true);
			LAResource* pData = RESOURCE_MGR.GetResourceAdress(CodeName);
			pData->m_ResourceMatrixName = MatrixPath;
			pData->m_ResourceSkinName = SkinPath;
		}
		else
		{
			CheckResouce = true;
		}

		if (CheckResouce)
		{
			std::wstring type;
			ObjectTableLoadData loadData;

			loadData.eObjectType = E_LoadTableType::WEAPON;
			loadData.iTableIndex = Tableindex;
			loadData.wsCodeName = CodeName;
			loadData.wsMatrixName = MatrixPath;
			loadData.wsSkinName = SkinPath;
			type = WeaponType;

			if (type.compare(L"RIPLE") == 0) loadData.eWeaponType = E_WeaponType::RIFLE;
			if (type.compare(L"HANDGUN") == 0) loadData.eWeaponType = E_WeaponType::HANDGUN;
			if (type.compare(L"KNIFE") == 0) loadData.eWeaponType = E_WeaponType::KNIFE;
			if (type.compare(L"BOMB") == 0) loadData.eWeaponType = E_WeaponType::BOMB;
			if (type.compare(L"MEDIC") == 0) loadData.eWeaponType = E_WeaponType::MEDIC;

			loadData.wsIconNameList.push_back(IconName);

			loadData.fScale = Scale;
			
			loadData.wsSoundNameList.push_back(Sound[0]);
			loadData.wsSoundNameList.push_back(Sound[1]);
			loadData.wsSoundNameList.push_back(Sound[2]);

			type = SkillType;
			if (type.compare(L"SKILL_HITBOX") == 0) loadData.eSkillType = E_SkillType::SKILL_HITBOX;
			if (type.compare(L"SKILL_BULLET") == 0) loadData.eSkillType = E_SkillType::SKILL_BULLET;
			if (type.compare(L"SKILL_BUFF") == 0) loadData.eSkillType = E_SkillType::SKILL_BUFF;
			if (type.compare(L"SKILL_TRAP") == 0) loadData.eSkillType = E_SkillType::SKILL_TRAP;
			loadData.wsSkillName = SkillName;
			loadData.fAbilityReaction = AbilityReaction;
			loadData.fAbilityAimIncrease = AbilityAimIncrease;
			type = Bullet_need_Reload;
			if (type.compare(L"TRUE") == 0) loadData.bBullet_need_Reload = true;
			if (type.compare(L"FALSE") == 0) loadData.bBullet_need_Reload = false;
			loadData.iAbilityReloadMax = AbilityReloadMax;
			loadData.fAbilityAttackSpeed = AbilityAttackSpeed;
			loadData.wsIconNameList.push_back(InvenIconName);
			loadData.wsIconNameList.push_back(ConvertInvenItemName);
			loadData.wsIconNameList.push_back(InvenDesc);

			LOAD_OBJECT_MGR.AddObjectDataInfo(loadData);
		}
		else
		{
			//�ε����
			std::wstring ErrorMGG = L"Not Load Weapon Table Data : Index ";
			ErrorMGG += std::to_wstring(Tableindex);
			ErrorMGG += L"CodeName ";
			ErrorMGG += CodeName;
			OutputDebugString(ErrorMGG.c_str());
		}
	}

	return true;
}

bool LAPaser::LoadArmorTable()
{
	int Tableindex = -1;
	TCHAR CodeName[256] = { 0 };
	TCHAR ArmorType[256] = { 0 };
	TCHAR IconName[256] = { 0 };
	TCHAR MatrixPath[256] = { 0 };
	TCHAR SkinPath[256] = { 0 };
	float Scale = 0.0f;
	TCHAR Sound[10][256] = { 0 };
	ZeroMemory(Sound, sizeof(TCHAR) * 10 * 256);
	TCHAR AbilityType[256] = { 0 };
	float AbilityNumber = 0.0f;
	TCHAR InvenIconName[256] = { 0 };
	TCHAR InvenItemName[256] = { 0 };
	TCHAR InvenDesc[256] = { 0 };

	/* File Read */
	int iCount = 0;

	ZeroMemory(m_pString, sizeof(TCHAR) * 500);
	_fgetts(m_pString, 500, m_pFile);
	_stscanf_s(m_pString, _T("%d"), //�ڴ� ���� ���� �ʿ�� �ε�
		&iCount);

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		ZeroMemory(m_pString, sizeof(TCHAR) * 500);

		_fgetts(m_pString, 500, m_pFile);
		_stscanf_s(m_pString, _T("%d %s %s %s %s %s %f %s %s %s %f %s %s %s"), //�ڴ� ���� ���� �ʿ�� �ε�
			&Tableindex,
			CodeName, _countof(CodeName),
			ArmorType, _countof(ArmorType),
			IconName, _countof(IconName),
			MatrixPath, _countof(MatrixPath),
			SkinPath, _countof(SkinPath),
			&Scale,
			Sound[0], _countof(Sound[0]),
			Sound[1], _countof(Sound[1]),
			AbilityType, _countof(AbilityType),
			&AbilityNumber,
			InvenIconName, _countof(InvenIconName),
			InvenItemName, _countof(InvenItemName),
			InvenDesc, _countof(InvenDesc)
			
		);
		/*=========================== �κ�ǥ�� �̸� �پ�� ���� ================================*/
		std::wstring ConvertInvenItemName = InvenItemName;
		const TCHAR frome[2] = L"_";
		TCHAR to[2] = L" ";
		for (int ilen = 0; ilen < ConvertInvenItemName.size(); ilen++)
		{
			if (ConvertInvenItemName[ilen] == frome[0])
			{
				ConvertInvenItemName[ilen] = to[0];
			}
		}

		//std::replace(ConvertName.begin(), ConvertName.end(), L"_", L" ");

		/*=========================== ���ҽ� �ߺ� �˻� ================================*/
		//���̺��ε��� �ߺ�üũ 
		bool CheckOverlapTableIndex = LOAD_OBJECT_MGR.CheckOverlapTableIndex(E_LoadTableType::ARMOR, Tableindex);
		if (CheckOverlapTableIndex == true)
			return true;

		int iIndex = -1;
		bool CheckResouce = false;
		iIndex = RESOURCE_MGR.GetResourceSkinNameIndex(SkinPath);

		if (iIndex == -1)
		{
			CheckResouce = LoadResourceData(Tableindex, CodeName, MatrixPath, SkinPath, L"NONE", E_LoadTableType::ARMOR, true);
			LAResource* pData = RESOURCE_MGR.GetResourceAdress(CodeName);
			pData->m_ResourceMatrixName = MatrixPath;
			pData->m_ResourceSkinName = SkinPath;
		}
		else
		{
			CheckResouce = true;
		}

		if (CheckResouce)
		{
			std::wstring type;
			ObjectTableLoadData loadData;

			loadData.eObjectType = E_LoadTableType::ARMOR;
			loadData.iTableIndex = Tableindex;
			loadData.wsCodeName = CodeName;

			type = ArmorType;
			/*1. ATTACK_POWER
			2. DEFENCE
			3. HP
			4. ATTACK_SPEED
			5. RUN_SPEED*/
			if (type.compare(L"HEAD") == 0) loadData.eArmorType = E_ArmorType::HEAD;
			if (type.compare(L"BODY") == 0) loadData.eArmorType = E_ArmorType::BODY;
			if (type.compare(L"HAND") == 0) loadData.eArmorType = E_ArmorType::HAND;
			if (type.compare(L"FOOT") == 0) loadData.eArmorType = E_ArmorType::FOOT;

			loadData.wsIconNameList.push_back(IconName);
			loadData.wsMatrixName = MatrixPath;
			loadData.wsSkinName = SkinPath;
			loadData.fScale = Scale;
			loadData.wsSoundNameList.push_back(Sound[0]);
			loadData.wsSoundNameList.push_back(Sound[1]);

			/*1. ATTACK_POWER
				2. DEFENCE
				3. HP
				4. ATTACK_SPEED
				5. RUN_SPEED*/
			type = AbilityType;
			if (type.compare(L"ATTACK_POWER") == 0) loadData.e_ArmorAbilityType = E_ArmorAbilityType::ATTACK_POWER;
			if (type.compare(L"DEFENCE") == 0)		loadData.e_ArmorAbilityType = E_ArmorAbilityType::DEFENCE;
			if (type.compare(L"HP") == 0)			loadData.e_ArmorAbilityType = E_ArmorAbilityType::HP;
			if (type.compare(L"ATTACK_SPEED") == 0) loadData.e_ArmorAbilityType = E_ArmorAbilityType::ATTACK_SPEED;
			if (type.compare(L"RUN_SPEED") == 0)	loadData.e_ArmorAbilityType = E_ArmorAbilityType::RUN_SPEED;

			loadData.fObjectWeight = AbilityNumber;
			loadData.wsIconNameList.push_back(InvenIconName);
			loadData.wsIconNameList.push_back(ConvertInvenItemName); 
			loadData.wsIconNameList.push_back(InvenDesc);

			LOAD_OBJECT_MGR.AddObjectDataInfo(loadData);
		}
		else
		{
			//�ε����
			std::wstring ErrorMGG = L"Not Load Weapon Table Data : Index ";
			ErrorMGG += std::to_wstring(Tableindex);
			ErrorMGG += L"CodeName ";
			ErrorMGG += CodeName;
			OutputDebugString(ErrorMGG.c_str());
		}
	}

	return true;
}

bool LAPaser::LoadSkillHitbox()
{
	int Tableindex = -1;
	TCHAR CodeName[256] = { 0 };
	TCHAR EffectName[256] = { 0 };
	TCHAR Sound[256] = { 0 };
	float fHitboxOffset;
	float fHitboxRadius;
	float fHitboxTime;
	float AttckPower;

	/* File Read */
	int iCount = 0;

	ZeroMemory(m_pString, sizeof(TCHAR) * 500);
	_fgetts(m_pString, 500, m_pFile);
	_stscanf_s(m_pString, _T("%d"), //�ڴ� ���� ���� �ʿ�� �ε�
		&iCount);

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		ZeroMemory(m_pString, sizeof(TCHAR) * 500);

		_fgetts(m_pString, 500, m_pFile);
		_stscanf_s(m_pString, _T("%d %s %s %s %f %f %f %f"),
			&Tableindex,
			CodeName, _countof(CodeName),
			EffectName, _countof(EffectName),
			Sound, _countof(Sound),
			&fHitboxOffset,
			&fHitboxRadius,
			&fHitboxTime,
			&AttckPower
		);

		SkillTableLoadData loadData;

		loadData.eObjectType = E_LoadTableType::SKILL_HITBOX;
		loadData.iTableIndex = Tableindex;
		loadData.wsCodeName = CodeName;
		loadData.wsEffectNameList.push_back(EffectName);
		loadData.wsSoundNameList.push_back(Sound);
		loadData.fHitboxOffset = fHitboxOffset;
		loadData.fHitboxRadius = fHitboxRadius;
		loadData.fHealTime = fHitboxTime;
		loadData.fAttackPower = AttckPower;

		LOAD_OBJECT_MGR.AddSkillDataInfo(loadData);

	}
	return true;
}

bool LAPaser::LoadSkillbullet()
{
	int Tableindex = -1;
	TCHAR CodeName[256] = { 0 };
	TCHAR EffectName[256] = { 0 };
	TCHAR BulletMesh[256] = { 0 };
	TCHAR Sound[256] = { 0 };
	float fSpeed;
	float fRange;
	float AttckPower;

	/* File Read */
	int iCount = 0;

	ZeroMemory(m_pString, sizeof(TCHAR) * 500);
	_fgetts(m_pString, 500, m_pFile);
	_stscanf_s(m_pString, _T("%d"), //�ڴ� ���� ���� �ʿ�� �ε�
		&iCount);

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		ZeroMemory(m_pString, sizeof(TCHAR) * 500);

		_fgetts(m_pString, 500, m_pFile);
		_stscanf_s(m_pString, _T("%d %s %s %s %s %f %f %f"),
			&Tableindex,
			CodeName, _countof(CodeName),
			EffectName, _countof(EffectName),
			BulletMesh, _countof(BulletMesh),
			Sound, _countof(Sound),
			&fSpeed,
			&fRange,
			&AttckPower
		);

		SkillTableLoadData loadData;

		loadData.eObjectType = E_LoadTableType::SKILL_BULLET;
		loadData.iTableIndex = Tableindex;
		loadData.wsCodeName = CodeName;
		loadData.wsEffectNameList.push_back(EffectName);
		loadData.wsBulletMesh3DName = BulletMesh;
		loadData.wsSoundNameList.push_back(Sound);
		loadData.fBulletSpeed = fSpeed;
		loadData.fBulletRange = fRange;
		loadData.fAttackPower = AttckPower;

		LOAD_OBJECT_MGR.AddSkillDataInfo(loadData);

	}
	return true;
}

bool LAPaser::LoadSkillBuff()
{
	int Tableindex = -1;
	TCHAR CodeName[256] = { 0 };
	TCHAR EffectName[256] = { 0 };
	TCHAR Sound[256] = { 0 };
	float fHealPower;
	float fHealTime;

	/* File Read */
	int iCount = 0;

	ZeroMemory(m_pString, sizeof(TCHAR) * 500);
	_fgetts(m_pString, 500, m_pFile);
	_stscanf_s(m_pString, _T("%d"), //�ڴ� ���� ���� �ʿ�� �ε�
		&iCount);

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		ZeroMemory(m_pString, sizeof(TCHAR) * 500);

		_fgetts(m_pString, 500, m_pFile);
		_stscanf_s(m_pString, _T("%d %s %s %s %f %f"),
			&Tableindex,
			CodeName, _countof(CodeName),
			EffectName, _countof(EffectName),
			Sound, _countof(Sound),
			&fHealPower,
			&fHealTime
		);

		SkillTableLoadData loadData;

		loadData.eObjectType = E_LoadTableType::SKILL_BUFF;
		loadData.iTableIndex = Tableindex;
		loadData.wsCodeName = CodeName;
		loadData.wsEffectNameList.push_back(EffectName);
		loadData.wsSoundNameList.push_back(Sound);
		loadData.fHealPower = fHealPower;
		loadData.fHealTime = fHealTime;

		LOAD_OBJECT_MGR.AddSkillDataInfo(loadData);

	}
	return true;
}

bool LAPaser::LoadSkillTrap()
{
	int Tableindex = -1;
	TCHAR CodeName[256] = { 0 };
	TCHAR EffectName[256] = { 0 };
	TCHAR EffectName2[256] = { 0 };
	TCHAR BulletMesh[256] = { 0 };
	TCHAR Sound[256] = { 0 };
	TCHAR Sound2[256] = { 0 };
	float fDistance;
	float fDeameter;
	float fAttackDelay;
	float fAttackPower;

	/* File Read */
	int iCount = 0;

	ZeroMemory(m_pString, sizeof(TCHAR) * 500);
	_fgetts(m_pString, 500, m_pFile);
	_stscanf_s(m_pString, _T("%d"), //�ڴ� ���� ���� �ʿ�� �ε�
		&iCount);

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		ZeroMemory(m_pString, sizeof(TCHAR) * 500);

		_fgetts(m_pString, 500, m_pFile);
		_stscanf_s(m_pString, _T("%d %s %s %s %s %s %s %f %f %f %f"),
			&Tableindex,
			CodeName, _countof(CodeName),
			EffectName, _countof(EffectName),
			EffectName2, _countof(EffectName2),
			BulletMesh, _countof(BulletMesh),
			Sound, _countof(Sound),
			Sound2, _countof(Sound2),
			&fDistance,
			&fDeameter,
			&fAttackDelay,
			&fAttackPower
		);

		SkillTableLoadData loadData;

		loadData.eObjectType = E_LoadTableType::SKILL_TRAP;
		loadData.iTableIndex = Tableindex;
		loadData.wsCodeName = CodeName;
		loadData.wsEffectNameList.push_back(EffectName);
		loadData.wsEffectNameList.push_back(EffectName2);
		loadData.wsBulletMesh3DName = BulletMesh;
		loadData.wsSoundNameList.push_back(Sound);
		loadData.wsSoundNameList.push_back(Sound2);
		loadData.fTrapDistance = fDistance;
		loadData.fTrapDiameter = fDeameter;
		loadData.fTrapAttackDelay = fAttackDelay;
		loadData.fAttackPower = fAttackPower;

		LOAD_OBJECT_MGR.AddSkillDataInfo(loadData);
		
	}
	return true;
}

bool LAPaser::LoadResourceData(int iLoadTableIndex, std::wstring wsCodename, std::wstring wsMatrixName, std::wstring wsSkinName, std::wstring AnimationCodeName, E_LoadTableType eTableType, bool bBinary)
{
	//3�� �ݺ� ��Ʈ����, ��Ų, �ִ�
	int ipatchIndex = (int)eTableType * 3;

	CrateObjectDest skinNormal;
	skinNormal.vertexShaderName = L"../../data/CharacterLoadFile/shader/Skin.hlsl";
	skinNormal.vertexFuntionNmae = "VS_NORMAL";
	skinNormal.PixelShaderName = L"../../data/CharacterLoadFile/shader/Skin.hlsl";
	skinNormal.PixelFuntionNmae = "TPS_NORMAL";
	skinNormal.GiometryShaderName = NULL;
	skinNormal.GiometryFuntionNmae = NULL;
	skinNormal.pTextureName = NULL;

	CrateObjectDest skinNormalInstancing;
	skinNormalInstancing.vertexShaderName = L"../../data/CharacterLoadFile/shader/SkinInstancing.hlsl";
	skinNormalInstancing.vertexFuntionNmae = "VS_NORMAL";
	skinNormalInstancing.PixelShaderName = L"../../data/CharacterLoadFile/shader/SkinInstancing.hlsl";
	skinNormalInstancing.PixelFuntionNmae = "TPS_NORMAL";
	skinNormalInstancing.GiometryShaderName = NULL;
	skinNormalInstancing.GiometryFuntionNmae = NULL;
	skinNormalInstancing.pTextureName = NULL;

	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		E_LoadObjectType type;
		bool bFileOpen = false;

		switch (iCnt)
		{
		case 0:
		{
			if (wsMatrixName.compare(L"NONE") == 0)
			{
				break;
			}

			std::wstring fullpatch = RESOURCE_MGR.m_LAFilePath.m_FilePatch[ipatchIndex];
			type = E_LoadObjectType::CHARACTER_MATRIX;
			fullpatch += wsMatrixName;
						
			if (bBinary)
			{
				bFileOpen = OpenStream(&m_pSubFile, fullpatch.c_str(), L"rb");
			}
			else
			{
				bFileOpen = OpenStream(&m_pSubFile, fullpatch.c_str(), L"rt");
			}
			if (bFileOpen)
			{
				m_PsbParser.Init(m_pCoreDevice, m_pCoreContext);
				if (m_PsbParser.Load(m_pSubFile, type, bBinary))
				{
					/*switch (eTableType)
					{
					case E_LoadTableType::CHARACTER:	m_PsbParser.Convert(iLoadTableIndex, wsCodename, L"", type, NULL, &skinLight);	break;
					case E_LoadTableType::MONSTER:		m_PsbParser.Convert(iLoadTableIndex, wsCodename, L"", type);	break;
					case E_LoadTableType::WEAPON:		m_PsbParser.Convert(iLoadTableIndex, wsCodename, L"", type, NULL, &skinLight);	break;
					case E_LoadTableType::ARMOR:		m_PsbParser.Convert(iLoadTableIndex, wsCodename, L"", type, NULL, &skinLight);	break;
					}*/
					m_PsbParser.Convert(iLoadTableIndex, wsCodename, L"", type, NULL, NULL, false);
				}
				m_PsbParser.Release();
			}
			else
			{
				//���Ͽ��� ������ üũ�ϴ� �κ�
				return false;
			}
			CloseStream(&m_pSubFile);
			

		}break;

		case 1:
		{
			if (wsSkinName.compare(L"NONE") == 0)
			{
				break;
			}

			type = E_LoadObjectType::CHARACTER_SKIN;
			std::wstring fullpatch = RESOURCE_MGR.m_LAFilePath.m_FilePatch[ipatchIndex + 1];
			TCHAR ParentMatrixName[256] = { 0 };
			fullpatch += wsSkinName;

			if (bBinary)
			{
				bFileOpen = OpenStream(&m_pSubFile, fullpatch.c_str(), L"rb");
			}
			else
			{
				bFileOpen = OpenStream(&m_pSubFile, fullpatch.c_str(), L"rt");
			}
			if (bFileOpen)
			{
				m_PsbParser.Init(m_pCoreDevice, m_pCoreContext);
				if (m_PsbParser.Load(m_pSubFile, type, bBinary))
				{
					switch (eTableType)
					{
					case E_LoadTableType::CHARACTER:	m_PsbParser.Convert(iLoadTableIndex, wsCodename, L"", type, NULL, NULL, false);	break;
					case E_LoadTableType::MONSTER:		m_PsbParser.Convert(iLoadTableIndex, wsCodename, L"", type, NULL, &skinNormalInstancing, true);	break;
					case E_LoadTableType::WEAPON:		m_PsbParser.Convert(iLoadTableIndex, wsCodename, L"", type, NULL, &skinNormal, true);	break;
					case E_LoadTableType::ARMOR:		m_PsbParser.Convert(iLoadTableIndex, wsCodename, L"", type, NULL, NULL, false);	break;
					default:
					{
						m_PsbParser.Convert(iLoadTableIndex, wsCodename, L"", type, NULL, NULL, false);
					}break;
					}
					
				}
				m_PsbParser.Release();
			}
			else
			{
				//���Ͽ��� ������ üũ�ϴ� �κ�
				return false;
			}
			CloseStream(&m_pSubFile);

		}break;

		case 2:
		{
			if (AnimationCodeName.compare(L"NONE") == 0)
			{
				break;
			}

			type = E_LoadObjectType::CHARACTER_ANIMATION;
			FILE* pAnimationFile = NULL;
			bool IsLoad = false;

			int AnipatchIndex;
			if ((int)eTableType == (int)E_LoadTableType::CHARACTER)	AnipatchIndex = (int)E_LoadTableType::CHARACTER;
			if ((int)eTableType == (int)E_LoadTableType::MONSTER)	AnipatchIndex = (int)E_LoadTableType::MONSTER;

			if (OpenStream(&m_pSubFile, RESOURCE_MGR.m_LAFilePath.m_FileAnimationPatch[AnipatchIndex].c_str(), L"rt"))
			{
				int Tableindex = -1;
				TCHAR CodeName[256] = { 0 };
				TCHAR Desc[256] = { 0 };
				TCHAR Anipatch[27][256] = { 0 };	//���� �ִϸ��̼� ���� 27�� 
				std::wstring check;
				

				//_fgetts(m_pString, 256, m_pFile);
				//_fgetts(m_pString, 256, m_pFile);
				//_fgetts(m_pString, 256, m_pFile);

				switch (AnipatchIndex)
				{
				case (int)E_LoadTableType::CHARACTER:
				{
					/* File Read */
					int iCount = 0;

					ZeroMemory(m_pString, sizeof(TCHAR) * 1000);
					_fgetts(m_pString, 1000, m_pSubFile);
					_stscanf_s(m_pString, _T("%d"), 
						&iCount);

					for (int iCnt = 0; iCnt < iCount; iCnt++)
					{
						//1	TEST_PLAYER_ANIMATION	�÷��̾� �ִϸ��̼� �׷�	ani_player_stand.PSB	ani_player_run_front.PSB	ani_player_run_back.PSB	ani_player_run_right.PSB	ani_player_run_left.PSB	ani_player_death.PSB	ani_player_weapon1_equip.PSB	ani_player_weapon1_use.PSB	ani_player_weapon2_equip.PSB	ani_player_weapon2_use.PSB	ani_player_weapon3_equip.PSB	ani_player_weapon3_use.PSB	ani_player_weapon4_equip.PSB	ani_player_weapon4_use.PSB	ani_player_weapon5_equip.PSB	ani_player_weapon5_use.PSB	
						_fgetts(m_pString, 1000, m_pSubFile);
						_stscanf_s(m_pString, _T("%d %s"),
							&Tableindex,
							CodeName, _countof(CodeName)
						);

						//�ִϸ��̼� ����Ʈ���� �ڵ���� ã��
						check = CodeName;
						if (check.compare(AnimationCodeName) == 0)
						{
							//ã�Ҵٸ� 
							_stscanf_s(m_pString, _T("%d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s"),
								&Tableindex,
								CodeName, _countof(CodeName),
								Anipatch[0], _countof(Anipatch[0]),		//������
								Anipatch[1], _countof(Anipatch[1]),		//����
								Anipatch[2], _countof(Anipatch[2]),		//����
								Anipatch[3], _countof(Anipatch[3]),		//������
								Anipatch[4], _countof(Anipatch[4]),		//����
								Anipatch[5], _countof(Anipatch[5]),		//�޸���
								Anipatch[6], _countof(Anipatch[6]),		//����
								Anipatch[7], _countof(Anipatch[7]),		//���
								Anipatch[8], _countof(Anipatch[8]),		//���� ����
								Anipatch[9], _countof(Anipatch[9]),		//���� ���� ��
								Anipatch[10], _countof(Anipatch[10]),		//���� ������
								Anipatch[11], _countof(Anipatch[11]),		//���� ����
								Anipatch[12], _countof(Anipatch[12]),		//���� ����
								Anipatch[13], _countof(Anipatch[13]),		//���� ���� ��
								Anipatch[14], _countof(Anipatch[14]),		//���� ������
								Anipatch[15], _countof(Anipatch[15]),		//���� ����
								Anipatch[16], _countof(Anipatch[16]),	//Į ����
								Anipatch[17], _countof(Anipatch[17]),	//Į ���� �� 
								Anipatch[18], _countof(Anipatch[18]),	//Į ���
								Anipatch[19], _countof(Anipatch[19]),	//����ź ����
								Anipatch[20], _countof(Anipatch[20]),	//����ź ���� ��
								Anipatch[21], _countof(Anipatch[21]),	//����ź ������
								Anipatch[22], _countof(Anipatch[22]),	//����ź ���
								Anipatch[23], _countof(Anipatch[23]),	//��ǰ ����
								Anipatch[24], _countof(Anipatch[24]),	//��ǰ ���� �� 
								Anipatch[25], _countof(Anipatch[25]),	//��ǰ ������
								Anipatch[26], _countof(Anipatch[26])	//��ǰ ���
							);

							for (int iAniCnt = 0; iAniCnt < ANIMATION_CHARACTER_COUNT; iAniCnt++)
							{
								std::wstring check = Anipatch[iAniCnt];

								if (check.compare(L"NONE") == 0)
								{
									continue;
								}

								std::wstring fullpatch = RESOURCE_MGR.m_LAFilePath.m_FilePatch[ipatchIndex + 2];
								fullpatch += Anipatch[iAniCnt];

								LoadFile(iLoadTableIndex, fullpatch.c_str(), wsCodename.c_str(), L"", type, bBinary);
						
							}
							break;
						}	//if
					}	//for

					IsLoad = true;
				}break;

				case (int)E_LoadTableType::MONSTER:
				{
					/* File Read */
					int iCount = 0;

					ZeroMemory(m_pString, sizeof(TCHAR) * 500);
					_fgetts(m_pString, 500, m_pSubFile);
					_stscanf_s(m_pString, _T("%d"),
						&iCount);

					for (int iCnt = 0; iCnt < iCount; iCnt++)
					{
						//1	TEST_PLAYER_ANIMATION	�÷��̾� �ִϸ��̼� �׷�	ani_player_stand.PSB	ani_player_run_front.PSB	ani_player_run_back.PSB	ani_player_run_right.PSB	ani_player_run_left.PSB	ani_player_death.PSB	ani_player_weapon1_equip.PSB	ani_player_weapon1_use.PSB	ani_player_weapon2_equip.PSB	ani_player_weapon2_use.PSB	ani_player_weapon3_equip.PSB	ani_player_weapon3_use.PSB	ani_player_weapon4_equip.PSB	ani_player_weapon4_use.PSB	ani_player_weapon5_equip.PSB	ani_player_weapon5_use.PSB	
						_fgetts(m_pString, 256, m_pSubFile);
						_stscanf_s(m_pString, _T("%d %s"),
							&Tableindex,
							CodeName, _countof(CodeName)
						);

						//�ִϸ��̼� ����Ʈ���� �ڵ���� ã��
						check = CodeName;
						if (check.compare(AnimationCodeName) == 0)
						{
							//ã�Ҵٸ� 
							_stscanf_s(m_pString, _T("%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s"),
								&Tableindex,
								CodeName, _countof(CodeName),
								Anipatch[0], _countof(Anipatch[0]),		//������
								Anipatch[1], _countof(Anipatch[1]),		//����
								Anipatch[2], _countof(Anipatch[2]),		//����
								Anipatch[3], _countof(Anipatch[3]),		//������
								Anipatch[4], _countof(Anipatch[4]),		//����
								Anipatch[5], _countof(Anipatch[5]),		//���
								Anipatch[6], _countof(Anipatch[6]),		//���� ����
								Anipatch[7], _countof(Anipatch[7]),		//���� ���
								Anipatch[8], _countof(Anipatch[8]),		//���� ����
								Anipatch[9], _countof(Anipatch[9]),		//���� ���
								Anipatch[10], _countof(Anipatch[10]),	//Į ����
								Anipatch[11], _countof(Anipatch[11]),	//Į ���
								Anipatch[12], _countof(Anipatch[12]),	//����ź ����
								Anipatch[13], _countof(Anipatch[13]),	//����ź ���
								Anipatch[14], _countof(Anipatch[14]),	//��ǰ ����
								Anipatch[15], _countof(Anipatch[15])	//��ǰ ���
							);

							for (int iAniCnt = 0; iAniCnt < ANIMATION_MONSTER_COUNT; iAniCnt++)
							{
								std::wstring check = Anipatch[iAniCnt];

								if (check.compare(L"NONE") == 0)
								{
									continue;
								}

								std::wstring fullpatch = RESOURCE_MGR.m_LAFilePath.m_FilePatch[ipatchIndex + 2];
								fullpatch += Anipatch[iAniCnt];

								LoadFile(iLoadTableIndex, fullpatch.c_str(), wsCodename.c_str(), L"", type, bBinary);
							}
							break;
						}	//if
					}	//for
				}break;
				}	//2�� switch

				CloseStream(&m_pSubFile);
			}	//if
			else
			{
				return false;
			}

		}break;
		}	 //switch
	}	 //for

	return true;
}

bool LAPaser::LoadFile(int iResourceIndex, const TCHAR* pFileName, const TCHAR* pResourceName, const TCHAR* wcParentMatrixName, 
	E_LoadObjectType eLoadType, bool bBinary, CrateObjectDest* pMatrixDest, CrateObjectDest* pSkinDest)
{
	TCHAR drive[256] = { 0 };
	TCHAR dir[256] = { 0 };
	TCHAR filename[256] = { 0 };
	TCHAR ext[256] = { 0 };
	_tsplitpath_s(pFileName, drive, _countof(drive), dir, _countof(dir), filename, _countof(filename), ext, _countof(ext));

	bool fileopen = false;
	if (bBinary)
	{
		fileopen = OpenStream(&m_pSubFile2, pFileName, L"rb");
	}
	else
	{
		fileopen = OpenStream(&m_pSubFile2, pFileName, L"rt");
	}

	if (fileopen)
	{
		m_PsbParser.Init(m_pCoreDevice, m_pCoreContext);
		if (m_PsbParser.Load(m_pSubFile2, eLoadType, bBinary))
		{
			//�ִϸ��̼� ������ �ε�
			m_PsbParser.Convert(iResourceIndex, pResourceName, wcParentMatrixName, eLoadType, pMatrixDest, pSkinDest, false);
		}
		m_PsbParser.Release();

		CloseStream(&m_pSubFile2);
		return true;
	}

	return false;
}

bool LAPaser::Save(const TCHAR* pFileName, E_LoadParserType ePaserType, LALoadObject& LoadObjectdata, E_ObjectType ObjectType, bool bSaveBinary)
{
	bool bFileOpen = false;
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

	if (bSaveBinary)
	{
		bFileOpen = OpenStream(&m_pFile, pFileName, L"wb");
	}
	else
	{
		bFileOpen = OpenStream(&m_pFile, pFileName, L"wt");
	}

	if (bFileOpen)
	{

		switch ((int)ePaserType)
		{
			case (int)E_LoadParserType::ASE_TYPE:
			{
				/*m_AseParser.Init(m_pCoreDevice, m_pCoreContext);
				if (m_AseParser.Load(m_pFile))
				{
				*pAddObject = *m_AseParser.Convert();
				m_AseParser.Release();
				}*/
			}break;

			case (int)E_LoadParserType::PSB_TYPE:
			{
				m_PsbParser.m_pPsbFile = m_pFile;
				m_PsbParser.Init(m_pCoreDevice, m_pCoreContext);
				m_PsbParser.Save(pFileName, LoadObjectdata, ObjectType, bSaveBinary);
				m_PsbParser.Release();
			}break;

		}

		CloseStream(&m_pFile);
	}
	else
	{
		return false;
	}

	return true;
}

LAPaser::LAPaser()
{
	m_pFile = NULL;
	m_pSubFile = NULL;
	m_pSubFile2 = NULL;
	ZeroMemory(m_pBuffer, sizeof(TCHAR) * 256);
	ZeroMemory(m_pString, sizeof(TCHAR) * 256);
}


LAPaser::~LAPaser()
{
}
