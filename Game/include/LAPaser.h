#pragma once

#include "CoreStandardLink.h"

#include "LALoadObject.h"
#include "LAPsbParser.h"

const int ANIMATION_CHARACTER_COUNT = 27;
const int ANIMATION_MONSTER_COUNT = 6;



class LAPaser
{
public:
	FILE* m_pFile;
	FILE* m_pSubFile;
	FILE* m_pSubFile2;
	LAPsbParser m_PsbParser;
	ID3D11Device* m_pCoreDevice;
	ID3D11DeviceContext* m_pCoreContext;
	TCHAR m_pBuffer[500];
	TCHAR m_pString[1000];
public:
	bool OpenStream(FILE** pFile, const TCHAR* pFileName, TCHAR* mode);
	bool CloseStream(FILE** pFile);
	std::shared_ptr<LALoadObject> Load(const TCHAR* pFileName,  E_LoadParserType eLoadType);
	int LoadTool(std::wstring wsMatrixResourceName, std::wstring wsSkinResourceName, std::wstring wsAniResourceName, const TCHAR* pFileName, E_LoadObjectType eLoadObjectType, bool bBinary, int iLoadType, CrateObjectDest* pMatrixDest = NULL, CrateObjectDest* pSkinDest = NULL);
	bool LoadEngine(const TCHAR* pFileName, E_LoadObjectType LoadObjectType, bool bBinary, CrateObjectDest* pMatrixDest = NULL, CrateObjectDest* pSkinDest = NULL);
	bool LoadTable(E_LoadTableType loadtabletype);
	bool LoadCharacterTable();
	bool LoadMonsterTable();
	bool LoadWeaponTable();
	bool LoadArmorTable();
	bool LoadSkillHitbox();
	bool LoadSkillbullet();
	bool LoadSkillBuff();
	bool LoadSkillTrap();
	bool LoadResourceData(int iLoadTableIndex, std::wstring wsCodename,
		std::wstring wsMatrixName, std::wstring wsSkinName, std::wstring AnimationCodeName,
		E_LoadTableType eTableType, bool bBinary);
	bool LoadFile(int iResourceIndex, const TCHAR* pFileName, const TCHAR* pResourceName, const TCHAR* wcParentMatrixName, 
		E_LoadObjectType eLoadType, bool bBinary, CrateObjectDest* pMatrixDest = NULL, CrateObjectDest* pSkinDest = NULL);
	bool Save(const TCHAR* pFileName, E_LoadParserType ePaserType, LALoadObject& LoadObjectdata, E_ObjectType ObjectType, bool bSaveBinary);
	//void SearchToObjectParent(LALoadObject* rLoadObjectList);
public:
	bool Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
public:
	LAPaser();
	~LAPaser();
};

