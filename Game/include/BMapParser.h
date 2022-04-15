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
	//파서들
	CharacterParser* m_CharParser;
	K_UIParser* m_UIParser;

	/*웨폰 파서용*/
	FILE* m_pStream;
	TCHAR m_pBuffer[256];
	TCHAR m_pString[256];
	
	vector<SpawnWeaponInfo*> m_WeaponInfo;
	SpawnWeaponInfo* NewWeapon;
	
	/*몬스터 파서용*/
	int fullStageCnt;
	map<TCHAR*, int> m_MonsterInfo;
	map<int, TCHAR[255]> m_TempMonsterName;
	vector<StageForMonsterInfo*> m_StageForMonster;
	MonsterInfo* NewMonsterInfo;

	vector<D3DXVECTOR3> MonsterRespawnable; // 몬스터 스폰가능지역

	/*맵 생성용 변수*/
	BMap SampleMap;

	/*MFC용 변수*/
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
	int m_VCtrlState; // 맵 높이조절 상태확인용 변수 
	int m_VSpreatState; // 맵 그리기 상태확인용 변수
	BSelect m_Select;
	float m_fRadius; // 마우스 피킹범위
	float m_fHeightValue;
	bool bParallel;
	float fParallel;
	// 스프레팅용 변수
	B_BOX SpreatingBox; // 스프레팅의 최대 최소범위uv값
	float m_fSpreatingRadius;
	int   m_iSpreatingColor;

	///////////////////////////*워터맵 전용 변수*/////////////////////////////////
	// 임시 라이트 ( 클래스화 예정 )
	D3DXVECTOR3	m_vInitLightPos;
	D3DXVECTOR3 m_LightPos;

	D3DXVECTOR3 m_vDirection;
	D3DXMATRIX	m_mLightWorld;

	/*Engine용*/
	BMapDesc* m_MFCWaterMapInfo;
	vector<BNMap*> m_vMFCWaterMap;
	vector<BWaterMap*> m_vWaterQuadTree;
	BNMap* m_MFCWaterMap;
	BWaterMap* m_WaterMapQuadTree;

	BMapTileRender* m_bSelectedMap; // 나중에 맵이 여러개 생성되었을 때 변경해주어야한다. 지금은 기본 생성된 0번맵을 넣어줄 것이다.
	int m_iSelectedMap;// 나중에 맵이 여러개 생성되었을 때 변경해주어야한다. 지금은 기본 생성된 0번맵을 넣어줄 것이다.
	
	//맵 세팅용 변수
	BMapSetting m_MapSetting;
	int m_EnemyPos;
	bool m_bItemInited;

	// 맵 오브젝트 배치용 임시변수 ( 일단 나무만 이걸로 생성하자 )
	int MapObjCount;
	vector<D3DXVECTOR3> m_MapObjPosition;
	
	// 맵 오브젝트
	vector<BAseObj*> m_MapObjMgr;
	BAseObj* m_MapObj;
	BAseObj m_RenderSettingASE;

	// 쉐도우
	ID3D11VertexShader*		m_pShadowVS; // 버텍스 쉐이더
	ID3D11PixelShader*		m_pShadowPS; // 픽셀 쉐이더
	ID3D11Buffer*			m_pShadowCB;
	SHADOW_CONSTANT_BUFFER  m_cbShadow;

	BRenderPlane m_ShadowPlane;
	D3DXVECTOR3 m_vLightPos;
	D3DXVECTOR3 m_vLightLookat;
	D3DXVECTOR3 m_vLightUp;
	D3DXMATRIX	m_matShadowView;
	D3DXMATRIX	m_matShadowProj;
	D3DXMATRIX	m_matTexture;
	D3DXMATRIX	m_matWVPT;

	// 임시용 변수
	vector<BLine> TestLine;
	bool bIsmapCreated;
	bool bIsMonsterAllDel;
	int iForRandom;

	//Particle 이펙트
	vector<ParticleSystem> m_ParticleSystemList;
	bool BStage1;
	bool BStage2;
	bool BStage3;
	bool BStage4;
	int HeroDem;
	BBoundingBox m_DrumBoundingBox[3];


	// 드럼통
	vector<InstancingInfo> m_StageDrumInfo;
	vector<D3DXMATRIX> m_StageDrumMat;
	
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	void MonsterRespawnPostionSetting(D3DXVECTOR3 Heropos);
	bool SettingParser(CharacterParser* CharParser, K_UIParser* UIParser);
	void WeaponParser(TCHAR* WeaponText);
	void MonsterParser(TCHAR* MonsterText);
	void RandomWeaponSpawn();
	void RandomMonsterSpawn(int StageNum);

	/*MFC용 함수*/
	bool CreateMap(int iCellCount, int iPatchCount, float fCellDistance, TCHAR** MapTexture, int TextureCount, TCHAR* HeightMapTex = NULL);
	bool CreateQuadTree(int iMaxDepthLimit, float fMinDivideSize, BMap* Map);
	bool MapHeightControl();
	D3DXVECTOR2 ChangeYPosition(D3DXVECTOR3 CPos, vector<PNCT_Vertex>* VertexList);
	D3DXVECTOR2 ChangeYParallel(D3DXVECTOR3 CPos, vector<PNCT_Vertex>* VertexList);
	D3DXVECTOR4 GetHeight(D3DXVECTOR3 HeroPos, int* HeroSound = NULL, bool* MoveAble = NULL);
	bool MapSpreating(); // 스프레팅 함수

	bool MapExport();
	bool MapImport(TCHAR* ImportFile);

	/*워터맵용 함수*/
	bool CreateWaterMap(BMapTileRender* SelectedMap, int iMaxDepthLimit, float fMinDivideSize, int iSelectedMap, TCHAR* WaterTex, TCHAR* WaterNormalTex, float fHeight, float fWave);
	
	// 맵 세팅용 함수
	vector<D3DXVECTOR3> GetEnemyPosition();

	// 맵 오브젝트용 함수
	BNode* GetMapObjNode(D3DXVECTOR3 HeroPos, BNode* pNode);
	bool CheckInNode(D3DXVECTOR3 HeroPos, BNode* pNode);

	//쉐도우용 함수
	HRESULT LoadShadowShader();
	HRESULT CreateShadowCB();
	void RenderShadow();
public:
	BMapParser();
	~BMapParser();
};

