#pragma once
#include "BCore.h"
#include "ParticleSystemManager.h"

enum WeaponCode
{
	SPAWN_WEAPON_1 = 1,
	SPAWN_WEAPON_2,
	SPAWN_WEAPON_3,
	SPAWN_WEAPON_4,
	SPAWN_WEAPON_5
};

struct SpawnWeaponInfo
{
	int WeaponIndex;
	TCHAR WeaponName[40];
	int MaxBullet[3];
	int MinBullet[3];
	int GetChance[3];
};

struct MonsterInfo
{
	int iStage;
	int iMonsterIndex[3];
	int iRespawnMaxCount;
	int iRespawnMinCount;
	int iRespawnChance[3];
};

struct StageForMonsterInfo
{
	MonsterInfo* MonsterChance[3];
};

class CharacterParser;
class K_UIParser;

class BMapParser
{
public:
	//�ļ���
	CharacterParser* m_CharParser;
	K_UIParser* m_UIParser;

	/*���� �ļ���*/
	FILE* m_pStream;
	TCHAR m_pBuffer[256];
	TCHAR m_pString[256];
	
	vector<SpawnWeaponInfo*> m_WeaponInfo;
	SpawnWeaponInfo* NewWeapon;
	
	/*���� �ļ���*/
	int fullStageCnt;
	map<TCHAR*, int> m_MonsterInfo;
	map<int, TCHAR[255]> m_TempMonsterName;
	vector<StageForMonsterInfo*> m_StageForMonster;
	MonsterInfo* NewMonsterInfo;

	vector<TVector3> MonsterRespawnable; // ���� ������������

	/*�� ������ ����*/
	BMap SampleMap;

	/*MFC�� ����*/
	vector<BMapDesc*> m_vMFCMapInfo;
	vector<BMap*> m_vMFCMap;
	vector<BMapTileRender*> m_vMapQuadTree;
	vector<int> m_viClosedIndex;
	MapExporter m_Exporter;
	MapImporter m_Importer;

	vector<TCHAR*> m_vMapTexture;
	map<int, TCHAR*> m_mMapSplattingTexture;

	BMapTileRender* m_MapQuadTree;
	BMapDesc* m_MFCMapInfo;
	BMap* m_MFCMap;
	int m_VCtrlState; // �� �������� ����Ȯ�ο� ���� 
	int m_VSpreatState; // �� �׸��� ����Ȯ�ο� ����
	BSelect m_Select;
	float m_fRadius; // ���콺 ��ŷ����
	float m_fHeightValue;
	bool bParallel;
	float fParallel;
	// �������ÿ� ����
	B_BOX SpreatingBox; // ���������� �ִ� �ּҹ���uv��
	float m_fSpreatingRadius;
	int   m_iSpreatingColor;

	///////////////////////////*���͸� ���� ����*/////////////////////////////////
	// �ӽ� ����Ʈ ( Ŭ����ȭ ���� )
	TVector3	m_vInitLightPos;
	TVector3 m_LightPos;

	TVector3 m_vDirection;
	TMatrix	m_mLightWorld;

	/*Engine��*/
	BMapDesc* m_MFCWaterMapInfo;
	vector<BNMap*> m_vMFCWaterMap;
	vector<BWaterMap*> m_vWaterQuadTree;
	BNMap* m_MFCWaterMap;
	BWaterMap* m_WaterMapQuadTree;

	BMapTileRender* m_bSelectedMap; // ���߿� ���� ������ �����Ǿ��� �� �������־���Ѵ�. ������ �⺻ ������ 0������ �־��� ���̴�.
	int m_iSelectedMap;// ���߿� ���� ������ �����Ǿ��� �� �������־���Ѵ�. ������ �⺻ ������ 0������ �־��� ���̴�.
	
	//�� ���ÿ� ����
	BMapSetting m_MapSetting;
	int m_EnemyPos;
	bool m_bItemInited;

	// �� ������Ʈ ��ġ�� �ӽú��� ( �ϴ� ������ �̰ɷ� �������� )
	int MapObjCount;
	vector<TVector3> m_MapObjPosition;
	
	// �� ������Ʈ
	vector<BAseObj*> m_MapObjMgr;
	BAseObj* m_MapObj;
	BAseObj m_RenderSettingASE;

	// ������
	ID3D11VertexShader*		m_pShadowVS; // ���ؽ� ���̴�
	ID3D11PixelShader*		m_pShadowPS; // �ȼ� ���̴�
	ID3D11Buffer*			m_pShadowCB;
	SHADOW_CONSTANT_BUFFER  m_cbShadow;

	BRenderPlane m_ShadowPlane;
	TVector3 m_vLightPos;
	TVector3 m_vLightLookat;
	TVector3 m_vLightUp;
	TMatrix	m_matShadowView;
	TMatrix	m_matShadowProj;
	TMatrix	m_matTexture;
	TMatrix	m_matWVPT;

	// �ӽÿ� ����
	vector<BLine> TestLine;
	bool bIsmapCreated;
	bool bIsMonsterAllDel;
	int iForRandom;

	//Particle ����Ʈ
	vector<ParticleSystem> m_ParticleSystemList;
	bool BStage1;
	bool BStage2;
	bool BStage3;
	bool BStage4;
	int HeroDem;
	BBoundingBox m_DrumBoundingBox[3];


	// �巳��
	vector<InstancingInfo> m_StageDrumInfo;
	vector<TMatrix> m_StageDrumMat;
	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	void MonsterRespawnPostionSetting(TVector3 Heropos);
	bool SettingParser(CharacterParser* CharParser, K_UIParser* UIParser);
	void WeaponParser(TCHAR* WeaponText);
	void MonsterParser(TCHAR* MonsterText);
	void RandomWeaponSpawn();
	void RandomMonsterSpawn(int StageNum);

	/*MFC�� �Լ�*/
	bool CreateMap(int iCellCount, int iPatchCount, float fCellDistance, TCHAR** MapTexture, int TextureCount, TCHAR* HeightMapTex = NULL);
	bool CreateQuadTree(int iMaxDepthLimit, float fMinDivideSize, BMap* Map);
	bool MapHeightControl();
	TVector2 ChangeYPosition(TVector3 CPos, vector<PNCT_Vertex>* VertexList);
	TVector2 ChangeYParallel(TVector3 CPos, vector<PNCT_Vertex>* VertexList);
	TVector4 GetHeight(TVector3 HeroPos, int* HeroSound = NULL, bool* MoveAble = NULL);
	bool MapSpreating(); // �������� �Լ�

	bool MapExport();
	bool MapImport(TCHAR* ImportFile);

	/*���͸ʿ� �Լ�*/
	bool CreateWaterMap(BMapTileRender* SelectedMap, int iMaxDepthLimit, float fMinDivideSize, int iSelectedMap, TCHAR* WaterTex, TCHAR* WaterNormalTex, float fHeight, float fWave);
	
	// �� ���ÿ� �Լ�
	vector<TVector3> GetEnemyPosition();

	// �� ������Ʈ�� �Լ�
	BNode* GetMapObjNode(TVector3 HeroPos, BNode* pNode);
	bool CheckInNode(TVector3 HeroPos, BNode* pNode);

	//������� �Լ�
	HRESULT LoadShadowShader();
	HRESULT CreateShadowCB();
	void RenderShadow();
public:
	BMapParser();
	~BMapParser();
};

