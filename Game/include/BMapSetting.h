#include "BStd.h"
#include "BMapTileRender.h"
#include "BSelect.h"
#include "BCamera.h"
#include "BInput.h"

#pragma once
enum FootSound
{
	Ground = 0,
	Sand,
	Iron,
	Wood,
	Carfet,
	Water,
};

struct TileInfo
{
	int TileSound;				// 타일 위에서 나는 소리
	bool MonsterSpawnIndex[5];  // 해당 타일에서 생성될 수 있는 몬스터의 종류
	bool TileMoveUnable;			// 진입불가능여부
	bool TileiTemRespawn;		// 아이템 생성 가능 여부
	bool TileMonsterRespawn;	// 몬스터 생성 가능 여부
	D3DXVECTOR3 TileCenter;		// 아이템 or 몬스터의 생성지점
	D3DXVECTOR3 TileEdge[4];	// 해당 타일의 4개의 버택스 값
	D3DXVECTOR3 TileNormal[2];     // 충돌처리를 위한 노말값
};

class BMapSetting
{
public:
	bool BMapSettingMode; // 맵세팅 모드를 실행하면 true
	bool BCreatePlane; // 드래그를 통한 Plane을 생성 가능한 상태인가
	bool BLastCheck; // 드래그 여부 체크

	bool BApply; // 적용버튼을 만들기 전 임시코드
	

	BSelect m_InfoSelect;
	BCamera* m_pMainCamera;
	int m_iFirstPickedX;
	int m_iFirstPickedZ;
	int m_iLastPickedX;
	int m_iLastPickedZ;
	float m_fPickedY;
	ID3D11DeviceContext* m_pImmediateContext;
	
	vector<vector<int>> m_MapAddress;
	vector<TileInfo> m_TileInfo;
	BMapTileRender* m_SelectedMap;

	vector<D3DXVECTOR3> EnemyPosition1;
	vector<D3DXVECTOR3> EnemyPosition2;
	vector<D3DXVECTOR3> EnemyPosition3;
	vector<D3DXVECTOR3> EnemyPosition4;
	vector<D3DXVECTOR3> ItemPosition;

public:
	void Init(BCamera* MainCamera, ID3D11Device* m_pd3dDevice, ID3D11DeviceContext* pContext);
	//void FirstPointFind(BMapTileRender* vMapQuadTree);
	//void LastPointFind(BMapTileRender* vMapQuadTree);
	void MapInfoSetting();
	void Render();
	void SettingPositionInfo(BNode* pNode, int iTotalRow);
	void SettingSoundInfo();
	void SettingObjInfo();
	void SettingMap(BMapTileRender* vMapQuadTree);
public:
	BMapSetting();
	~BMapSetting();
};

