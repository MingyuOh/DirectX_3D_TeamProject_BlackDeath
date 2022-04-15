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
	int TileSound;				// Ÿ�� ������ ���� �Ҹ�
	bool MonsterSpawnIndex[5];  // �ش� Ÿ�Ͽ��� ������ �� �ִ� ������ ����
	bool TileMoveUnable;			// ���ԺҰ��ɿ���
	bool TileiTemRespawn;		// ������ ���� ���� ����
	bool TileMonsterRespawn;	// ���� ���� ���� ����
	TVector3 TileCenter;		// ������ or ������ ��������
	TVector3 TileEdge[4];	// �ش� Ÿ���� 4���� ���ý� ��
	TVector3 TileNormal[2];     // �浹ó���� ���� �븻��
};

class BMapSetting
{
public:
	bool BMapSettingMode; // �ʼ��� ��带 �����ϸ� true
	bool BCreatePlane; // �巡�׸� ���� Plane�� ���� ������ �����ΰ�
	bool BLastCheck; // �巡�� ���� üũ

	bool BApply; // �����ư�� ����� �� �ӽ��ڵ�
	

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

	vector<TVector3> EnemyPosition1;
	vector<TVector3> EnemyPosition2;
	vector<TVector3> EnemyPosition3;
	vector<TVector3> EnemyPosition4;
	vector<TVector3> ItemPosition;

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

