#include "MonsterAIManager.h"

//min ~ max ���� ������ ���ڸ� ����
int getRandomNumber(int min, int max)
{
	//�õ� ����
	random_device rn;
	mt19937_64 rnd(rn());

	//���� ���� (����)
	uniform_int_distribution<int> range(min, max);

	//����
	return range(rnd);
}

//��ǰ ����
void MonsterAIManager::bubble_sort(float arr[], TVector3 arr2[], int count)    // �Ű������� ������ �迭�� ����� ������ ����
{
	float temp;
	TVector3 temp2;
	for (int i = 0; i < count; i++)    // ����� ������ŭ �ݺ�
	{
		for (int j = 0; j < count - 1; j++)   // ����� ���� - 1��ŭ �ݺ�
		{
			if (arr[j] > arr[j + 1])          // ���� ����� ���� ���� ����� ���� ���Ͽ�
			{                                 // ū ����
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;            // ���� ��ҷ� ����

				temp2 = arr2[j];
				arr2[j] = arr2[j + 1];
				arr2[j + 1] = temp2;
			}
		}
	}
}
void MonsterAIManager::MakeGraph()
{
	m_Graph = new Graph();
	//���ؽ� �߰�f
	m_Graph->AddVertex(0);
	m_Graph->AddVertex(1);
	m_Graph->AddVertex(2);
	m_Graph->AddVertex(3);
	m_Graph->AddVertex(4);
	m_Graph->AddVertex(5);
	m_Graph->AddVertex(6);
	m_Graph->AddVertex(7);
	m_Graph->AddVertex(8);
	//�� ��и鿡 ���� �׷����� ����
	switch (m_pMap->m_EnemyPos)
	{
	case 1:
	{
		//0�� ��������
		m_Graph->addAdj(0, 1, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[0], m_pMap->m_MapSetting.EnemyPosition1[1]));
		m_Graph->addAdj(0, 5, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[0], m_pMap->m_MapSetting.EnemyPosition1[5]));
		//1�� ��������
		m_Graph->addAdj(1, 0, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[1], m_pMap->m_MapSetting.EnemyPosition1[0]));
		m_Graph->addAdj(1, 2, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[1], m_pMap->m_MapSetting.EnemyPosition1[2]));
		//2�� ��������
		m_Graph->addAdj(2, 1, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[2], m_pMap->m_MapSetting.EnemyPosition1[1]));
		m_Graph->addAdj(2, 3, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[2], m_pMap->m_MapSetting.EnemyPosition1[3]));
		//3�� ��������
		m_Graph->addAdj(3, 2, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[3], m_pMap->m_MapSetting.EnemyPosition1[2]));
		m_Graph->addAdj(3, 6, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[3], m_pMap->m_MapSetting.EnemyPosition1[6]));
		//4�� ��������
		m_Graph->addAdj(4, 5, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[4], m_pMap->m_MapSetting.EnemyPosition1[5]));
		//5�� ��������
		m_Graph->addAdj(5, 0, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[5], m_pMap->m_MapSetting.EnemyPosition1[0]));
		m_Graph->addAdj(5, 4, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[5], m_pMap->m_MapSetting.EnemyPosition1[4]));
		m_Graph->addAdj(5, 8, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[5], m_pMap->m_MapSetting.EnemyPosition1[8]));
		//6�� ��������
		m_Graph->addAdj(6, 3, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[6], m_pMap->m_MapSetting.EnemyPosition1[3]));
		m_Graph->addAdj(6, 7, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[6], m_pMap->m_MapSetting.EnemyPosition1[7]));
		//7�� ��������
		m_Graph->addAdj(7, 6, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[7], m_pMap->m_MapSetting.EnemyPosition1[6]));
		m_Graph->addAdj(7, 8, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[7], m_pMap->m_MapSetting.EnemyPosition1[8]));
		//8�� ��������
		m_Graph->addAdj(8, 5, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[8], m_pMap->m_MapSetting.EnemyPosition1[5]));
		m_Graph->addAdj(8, 7, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition1[8], m_pMap->m_MapSetting.EnemyPosition1[7]));
	}break;
	case 2:
	{
		//0�� ��������
		m_Graph->addAdj(0, 1, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[0], m_pMap->m_MapSetting.EnemyPosition2[1]));
		m_Graph->addAdj(0, 2, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[0], m_pMap->m_MapSetting.EnemyPosition2[2]));
		m_Graph->addAdj(0, 4, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[0], m_pMap->m_MapSetting.EnemyPosition2[4]));
		//1�� ��������
		m_Graph->addAdj(1, 0, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[1], m_pMap->m_MapSetting.EnemyPosition2[0]));
		m_Graph->addAdj(1, 3, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[1], m_pMap->m_MapSetting.EnemyPosition2[3]));
		//2�� ��������
		m_Graph->addAdj(2, 0, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[2], m_pMap->m_MapSetting.EnemyPosition2[0]));
		m_Graph->addAdj(2, 5, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[2], m_pMap->m_MapSetting.EnemyPosition2[5]));
		//3�� ��������
		m_Graph->addAdj(3, 1, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[3], m_pMap->m_MapSetting.EnemyPosition2[1]));
		m_Graph->addAdj(3, 8, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[3], m_pMap->m_MapSetting.EnemyPosition2[8]));
		//4�� ��������
		m_Graph->addAdj(4, 0, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[4], m_pMap->m_MapSetting.EnemyPosition2[0]));
		m_Graph->addAdj(4, 6, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[4], m_pMap->m_MapSetting.EnemyPosition2[6]));
		//5�� ��������
		m_Graph->addAdj(5, 2, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[5], m_pMap->m_MapSetting.EnemyPosition2[2]));
		m_Graph->addAdj(5, 7, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[5], m_pMap->m_MapSetting.EnemyPosition2[7]));
		//6�� ��������
		m_Graph->addAdj(6, 4, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[6], m_pMap->m_MapSetting.EnemyPosition2[4]));
		m_Graph->addAdj(6, 7, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[6], m_pMap->m_MapSetting.EnemyPosition2[7]));
		m_Graph->addAdj(6, 8, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[6], m_pMap->m_MapSetting.EnemyPosition2[8]));
		//7�� ��������
		m_Graph->addAdj(7, 5, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[7], m_pMap->m_MapSetting.EnemyPosition2[5]));
		m_Graph->addAdj(7, 6, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[7], m_pMap->m_MapSetting.EnemyPosition2[6]));
		//8�� ��������
		m_Graph->addAdj(8, 3, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[8], m_pMap->m_MapSetting.EnemyPosition2[3]));
		m_Graph->addAdj(8, 6, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition2[8], m_pMap->m_MapSetting.EnemyPosition2[6]));
	}break;
	case 3:
	{
		//0�� ��������
		m_Graph->addAdj(0, 2, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[0], m_pMap->m_MapSetting.EnemyPosition3[2]));
		m_Graph->addAdj(0, 5, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[0], m_pMap->m_MapSetting.EnemyPosition3[5]));
		//1�� ��������
		m_Graph->addAdj(1, 2, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[1], m_pMap->m_MapSetting.EnemyPosition3[2]));
		m_Graph->addAdj(1, 3, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[1], m_pMap->m_MapSetting.EnemyPosition3[3]));
		//2�� ��������
		m_Graph->addAdj(2, 0, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[2], m_pMap->m_MapSetting.EnemyPosition3[0]));
		m_Graph->addAdj(2, 1, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[2], m_pMap->m_MapSetting.EnemyPosition3[1]));
		m_Graph->addAdj(2, 4, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[2], m_pMap->m_MapSetting.EnemyPosition3[4]));
		//3�� ��������
		m_Graph->addAdj(3, 1, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[3], m_pMap->m_MapSetting.EnemyPosition3[1]));
		m_Graph->addAdj(3, 7, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[3], m_pMap->m_MapSetting.EnemyPosition3[7]));
		//4�� ��������
		m_Graph->addAdj(4, 2, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[4], m_pMap->m_MapSetting.EnemyPosition3[2]));
		m_Graph->addAdj(4, 8, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[4], m_pMap->m_MapSetting.EnemyPosition3[8]));
		//5�� ��������
		m_Graph->addAdj(5, 0, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[5], m_pMap->m_MapSetting.EnemyPosition3[0]));
		m_Graph->addAdj(5, 6, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[5], m_pMap->m_MapSetting.EnemyPosition3[6]));
		//6�� ��������
		m_Graph->addAdj(6, 5, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[6], m_pMap->m_MapSetting.EnemyPosition3[5]));
		m_Graph->addAdj(6, 8, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[6], m_pMap->m_MapSetting.EnemyPosition3[8]));
		//7�� ��������
		m_Graph->addAdj(7, 3, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[7], m_pMap->m_MapSetting.EnemyPosition3[3]));
		m_Graph->addAdj(7, 8, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[7], m_pMap->m_MapSetting.EnemyPosition3[8]));
		//8�� ��������
		m_Graph->addAdj(8, 4, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[8], m_pMap->m_MapSetting.EnemyPosition3[4]));
		m_Graph->addAdj(8, 6, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[8], m_pMap->m_MapSetting.EnemyPosition3[6]));
		m_Graph->addAdj(8, 7, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition3[8], m_pMap->m_MapSetting.EnemyPosition3[7]));
	}break;
	case 4:
	{
		//0�� ��������
		m_Graph->addAdj(0, 1, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[0], m_pMap->m_MapSetting.EnemyPosition4[1]));
		m_Graph->addAdj(0, 2, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[0], m_pMap->m_MapSetting.EnemyPosition4[2]));
		m_Graph->addAdj(0, 3, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[0], m_pMap->m_MapSetting.EnemyPosition4[3]));
		//1�� ��������
		m_Graph->addAdj(1, 0, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[1], m_pMap->m_MapSetting.EnemyPosition4[0]));
		m_Graph->addAdj(1, 2, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[1], m_pMap->m_MapSetting.EnemyPosition4[2]));
		m_Graph->addAdj(1, 4, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[1], m_pMap->m_MapSetting.EnemyPosition4[4]));
		m_Graph->addAdj(1, 5, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[1], m_pMap->m_MapSetting.EnemyPosition4[5]));
		//2�� ��������
		m_Graph->addAdj(2, 0, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[2], m_pMap->m_MapSetting.EnemyPosition4[0]));
		m_Graph->addAdj(2, 1, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[2], m_pMap->m_MapSetting.EnemyPosition4[1]));
		m_Graph->addAdj(2, 4, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[2], m_pMap->m_MapSetting.EnemyPosition4[4]));
		//3�� ��������
		m_Graph->addAdj(3, 0, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[3], m_pMap->m_MapSetting.EnemyPosition4[0]));
		m_Graph->addAdj(3, 6, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[6], m_pMap->m_MapSetting.EnemyPosition4[6]));
		//4�� ��������
		m_Graph->addAdj(4, 1, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[4], m_pMap->m_MapSetting.EnemyPosition4[1]));
		m_Graph->addAdj(4, 2, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[4], m_pMap->m_MapSetting.EnemyPosition4[2]));
		m_Graph->addAdj(4, 5, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[4], m_pMap->m_MapSetting.EnemyPosition4[5]));
		m_Graph->addAdj(4, 6, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[4], m_pMap->m_MapSetting.EnemyPosition4[6]));
		//5�� ��������
		m_Graph->addAdj(5, 1, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[5], m_pMap->m_MapSetting.EnemyPosition4[1]));
		m_Graph->addAdj(5, 4, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[5], m_pMap->m_MapSetting.EnemyPosition4[4]));
		m_Graph->addAdj(5, 7, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[5], m_pMap->m_MapSetting.EnemyPosition4[7]));
		//6�� ��������
		m_Graph->addAdj(6, 3, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[6], m_pMap->m_MapSetting.EnemyPosition4[3]));
		m_Graph->addAdj(6, 4, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[6], m_pMap->m_MapSetting.EnemyPosition4[4]));
		m_Graph->addAdj(6, 8, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[6], m_pMap->m_MapSetting.EnemyPosition4[8]));
		//7�� ��������
		m_Graph->addAdj(7, 5, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[7], m_pMap->m_MapSetting.EnemyPosition4[5]));
		m_Graph->addAdj(7, 8, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[7], m_pMap->m_MapSetting.EnemyPosition4[8]));
		//8�� ��������
		m_Graph->addAdj(8, 6, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[8], m_pMap->m_MapSetting.EnemyPosition4[6]));
		m_Graph->addAdj(8, 7, DistancePointtoPoint(m_pMap->m_MapSetting.EnemyPosition4[8], m_pMap->m_MapSetting.EnemyPosition4[7]));
	}break;
	}
}
TVector3 MonsterAIManager::SetMonsterLook(TVector3 HeroPos, TVector3 MonPos)
{
	//y���� 0���� �� �������ġ�� ���� ��ġ
	TVector3 HeroPosNoHeight(HeroPos.x, 0.0f, HeroPos.z);
	TVector3 MonsterPosNoHeight(MonPos.x, 0.0f, MonPos.z);
	//���ο� Look����
	TVector3 vLook = HeroPosNoHeight - MonsterPosNoHeight;
	D3DXVec3Normalize(&vLook, &vLook);

	return vLook;
}

void MonsterAIManager::SetTransition(DWORD dwEvent)
{
	m_dwCurrentState = m_FSM.GetOutputState(m_dwCurrentState, dwEvent);
}
void MonsterAIManager::InsertMapMember(BMapParser* pMap)
{
	m_pMap = pMap;
	MakeGraph();
}

float MonsterAIManager::DistancePointtoPoint(TVector3 Point_1, TVector3 Point_2)
{
	float Dis_X = Point_1.x - Point_2.x;
	float Dis_Y = Point_1.y - Point_2.y;
	float Dis_Z = Point_1.z - Point_2.z;

	float Distance = sqrt(pow(Dis_X, 2) + pow(Dis_Y, 2) + pow(Dis_Z, 2));

	return Distance;
}
int MonsterAIManager::ReturnRandomSpawn(TVector3 MonPos)
{
	int Random;
	int ClosePoint = ReturnMostCloseHerotoSpawnPoint(MonPos);
	while (1)
	{
		Random = getRandomNumber(0, 8);
		if (ClosePoint != Random)
			break;
	}
	return Random;
}
int MonsterAIManager::ReturnMostCloseHerotoSpawnPoint(TVector3 HeroPos)
{
	float DistanceHerotoSpawnPoint[9];
	TVector3 HerotoSpawnPoint[9];

	switch (m_pMap->m_EnemyPos)
	{
	case 1:
	{
		int SpawnNumber;
		for (int iSpawn = 0; iSpawn < 9; iSpawn++)
		{
			DistanceHerotoSpawnPoint[iSpawn] = DistancePointtoPoint(HeroPos, m_pMap->m_MapSetting.EnemyPosition1[iSpawn]);
			HerotoSpawnPoint[iSpawn] = m_pMap->m_MapSetting.EnemyPosition1[iSpawn];
		}
		bubble_sort(DistanceHerotoSpawnPoint, HerotoSpawnPoint, 9);

		for (int iSpawn = 0; iSpawn < 9; iSpawn++)
		{
			if (HerotoSpawnPoint[0] == m_pMap->m_MapSetting.EnemyPosition1[iSpawn])
			{
				return iSpawn;
			}
		}
	}break;
	case 2:
	{
		int SpawnNumber;
		for (int iSpawn = 0; iSpawn < 9; iSpawn++)
		{
			DistanceHerotoSpawnPoint[iSpawn] = DistancePointtoPoint(HeroPos, m_pMap->m_MapSetting.EnemyPosition2[iSpawn]);
			HerotoSpawnPoint[iSpawn] = m_pMap->m_MapSetting.EnemyPosition2[iSpawn];
		}
		bubble_sort(DistanceHerotoSpawnPoint, HerotoSpawnPoint, 9);

		for (int iSpawn = 0; iSpawn < 9; iSpawn++)
		{
			if (HerotoSpawnPoint[0] == m_pMap->m_MapSetting.EnemyPosition2[iSpawn])
			{
				return iSpawn;
			}
		}
	}break;
	case 3:
	{
		int SpawnNumber;
		for (int iSpawn = 0; iSpawn < 9; iSpawn++)
		{
			DistanceHerotoSpawnPoint[iSpawn] = DistancePointtoPoint(HeroPos, m_pMap->m_MapSetting.EnemyPosition3[iSpawn]);
			HerotoSpawnPoint[iSpawn] = m_pMap->m_MapSetting.EnemyPosition3[iSpawn];
		}
		bubble_sort(DistanceHerotoSpawnPoint, HerotoSpawnPoint, 9);

		for (int iSpawn = 0; iSpawn < 9; iSpawn++)
		{
			if (HerotoSpawnPoint[0] == m_pMap->m_MapSetting.EnemyPosition3[iSpawn])
			{
				return iSpawn;
			}
		}
	}break;
	case 4:
	{
		int SpawnNumber;
		for (int iSpawn = 0; iSpawn < 9; iSpawn++)
		{
			DistanceHerotoSpawnPoint[iSpawn] = DistancePointtoPoint(HeroPos, m_pMap->m_MapSetting.EnemyPosition4[iSpawn]);
			HerotoSpawnPoint[iSpawn] = m_pMap->m_MapSetting.EnemyPosition4[iSpawn];
		}
		bubble_sort(DistanceHerotoSpawnPoint, HerotoSpawnPoint, 9);

		for (int iSpawn = 0; iSpawn < 9; iSpawn++)
		{
			if (HerotoSpawnPoint[0] == m_pMap->m_MapSetting.EnemyPosition4[iSpawn])
			{
				return iSpawn;
			}
		}
	}break;
	}
}
TVector3 MonsterAIManager::MonsterPathing(int Path)
{
	if (m_pMap)
	{
		switch (m_pMap->m_EnemyPos)
		{
		case 1:
		{
			return m_pMap->m_MapSetting.EnemyPosition1[Path];
		}break;
		case 2:
		{
			return m_pMap->m_MapSetting.EnemyPosition2[Path];
		}break;
		case 3:
		{
			return m_pMap->m_MapSetting.EnemyPosition3[Path];
		}break;
		case 4:
		{
			return m_pMap->m_MapSetting.EnemyPosition4[Path];
		}break;
		}
	}
}

bool MonsterAIManager::Init()
{
	if (NumOfMonster == 0)
	{
		//���� �߰�
		//=====================IDLE���¿����� ��ȭ======================
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_IDLE, MONSTER_EVENT_PATROL, MONSTER_ZOMBIE_STATE_MOVE);
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_IDLE, MONSTER_EVENT_FINDTARGET, MONSTER_ZOMBIE_STATE_DASH);
		//m_FSM.AddStateTransition(MONSTER_STATE_IDLE, MONSTER_EVENT_ATTACK, MONSTER_STATE_ATTACK);
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_IDLE, MONSTER_EVENT_DEAD, MONSTER_ZOMBIE_STATE_DEAD);
		//=====================MOVE���¿����� ��ȭ======================
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_MOVE, MONSTER_EVENT_PATROL, MONSTER_ZOMBIE_STATE_MOVE);
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_MOVE, MONSTER_EVENT_COLLISOIONTOOBJECT, MONSTER_ZOMBIE_STATE_SLIP);
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_MOVE, MONSTER_EVENT_FINDTARGET, MONSTER_ZOMBIE_STATE_DASH);
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_MOVE, MONSTER_EVENT_DEAD, MONSTER_ZOMBIE_STATE_DEAD);
		//=======-===============SLIP������ ��ȭ========================
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_SLIP, MONSTER_EVENT_PATROL, MONSTER_ZOMBIE_STATE_MOVE);
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_SLIP, MONSTER_EVENT_COLLISOIONTOOBJECT, MONSTER_ZOMBIE_STATE_SLIP);
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_SLIP, MONSTER_EVENT_DEAD, MONSTER_ZOMBIE_STATE_DEAD);
		//============-==========DASH������ ��ȭ========================
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_DASH, MONSTER_EVENT_FINDTARGET, MONSTER_ZOMBIE_STATE_DASH);
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_DASH, MONSTER_EVENT_ATTACK, MONSTER_ZOMBIE_STATE_ATTACK);
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_DASH, MONSTER_EVENT_LOSTTARGET, MONSTER_ZOMBIE_STATE_MOVE);
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_DASH, MONSTER_EVENT_DEAD, MONSTER_ZOMBIE_STATE_DEAD);
		//======================ATTACK������ ��ȭ=======================
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_ATTACK, MONSTER_EVENT_ATTACK, MONSTER_ZOMBIE_STATE_ATTACK);
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_ATTACK, MONSTER_EVENT_LOSTTARGET, MONSTER_ZOMBIE_STATE_MOVE);
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_ATTACK, MONSTER_EVENT_DEAD, MONSTER_ZOMBIE_STATE_DEAD);
		//=======================DEAD������ ��ȭ========================
		m_FSM.AddStateTransition(MONSTER_ZOMBIE_STATE_DEAD, MONSTER_EVENT_DEAD, MONSTER_ZOMBIE_STATE_DEAD);

		m_dwCurrentState = MONSTER_ZOMBIE_STATE_MOVE;
	}
	else if (NumOfMonster == 1)
	{
		//���� �߰�
		//=====================IDLE���¿����� ��ȭ======================
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_IDLE, MONSTER_EVENT_PATROL, MONSTER_GUNNER_STATE_MOVE);
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_IDLE, MONSTER_EVENT_FINDTARGET, MONSTER_GUNNER_STATE_ATTACK);
		//m_FSM.AddStateTransition(MONSTER_STATE_IDLE, MONSTER_EVENT_ATTACK, MONSTER_STATE_ATTACK);
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_IDLE, MONSTER_EVENT_DEAD, MONSTER_GUNNER_STATE_DEAD);
		//=====================MOVE���¿����� ��ȭ======================
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_MOVE, MONSTER_EVENT_PATROL, MONSTER_GUNNER_STATE_MOVE);
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_MOVE, MONSTER_EVENT_COLLISOIONTOOBJECT, MONSTER_GUNNER_STATE_SLIP);
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_MOVE, MONSTER_EVENT_FINDTARGET, MONSTER_GUNNER_STATE_ATTACK);
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_MOVE, MONSTER_EVENT_DEAD, MONSTER_GUNNER_STATE_DEAD);
		//=======-===============SLIP������ ��ȭ========================
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_SLIP, MONSTER_EVENT_PATROL, MONSTER_GUNNER_STATE_MOVE);
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_SLIP, MONSTER_EVENT_COLLISOIONTOOBJECT, MONSTER_GUNNER_STATE_SLIP);
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_SLIP, MONSTER_EVENT_DEAD, MONSTER_GUNNER_STATE_DEAD);
		//============-==========DASH������ ��ȭ========================
		//m_FSM.AddStateTransition(MONSTER_STATE_DASH, MONSTER_EVENT_FINDTARGET, MONSTER_STATE_DASH);
		//m_FSM.AddStateTransition(MONSTER_STATE_DASH, MONSTER_EVENT_ATTACK, MONSTER_STATE_ATTACK);
		//m_FSM.AddStateTransition(MONSTER_STATE_DASH, MONSTER_EVENT_LOSTTARGET, MONSTER_STATE_MOVE);
		//m_FSM.AddStateTransition(MONSTER_STATE_DASH, MONSTER_EVENT_DEAD, MONSTER_STATE_DEAD);
		//======================ATTACK������ ��ȭ=======================
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_ATTACK, MONSTER_EVENT_ATTACK, MONSTER_GUNNER_STATE_ATTACK);
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_ATTACK, MONSTER_EVENT_LOSTTARGET, MONSTER_GUNNER_STATE_MOVE);
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_ATTACK, MONSTER_EVENT_DEAD, MONSTER_GUNNER_STATE_DEAD);
		//=======================DEAD������ ��ȭ========================
		m_FSM.AddStateTransition(MONSTER_GUNNER_STATE_DEAD, MONSTER_EVENT_DEAD, MONSTER_GUNNER_STATE_DEAD);

		m_dwCurrentState = MONSTER_GUNNER_STATE_MOVE;
	}
	return true;
}
bool MonsterAIManager::Release()
{
	m_pMap = NULL;
	delete m_pMap;
	m_Graph->Release();
	m_FSM.Release();
	return true;
}
MonsterAIManager::MonsterAIManager()
{
	for (int i = 0; i < 10; i++)
		m_Path[i] = -1;
	HavePath = false;
	NoSerachTarget = true;
	IsArraiveDest = false;
}


MonsterAIManager::~MonsterAIManager()
{
}

