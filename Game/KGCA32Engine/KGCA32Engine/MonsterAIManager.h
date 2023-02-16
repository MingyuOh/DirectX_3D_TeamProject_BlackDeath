#pragma once
#include "FiniteStateMachine.h"
#include "FSMState.h"
#include "BMapParser.h"
#include <random>


#define MAX_VALUE -987654321 
//���� Ŭ����
class Vertex
{
public:
	int index;	//���� ��ȣ
	float dist;	//�Ÿ�
	int post = -1;	//���� ����	

	bool isVisited;
public:
	void SetDist(float D)	//�Ÿ�����
	{
		dist = D;
	}
	//���۷�����
	bool operator >(const Vertex& v) const {
		return dist > v.dist;
	}
	bool operator <(const Vertex& v) const {
		return dist < v.dist;
	}

public:
	Vertex(int i) {
		index = i;
		dist = MAX_VALUE;
		isVisited = false;
	};	//������
	Vertex(int i, float d, int p)
	{
		index = i;
		dist = d;
		post = p;
	};
	~Vertex() {};
};

//�׷��� Ŭ����
class Graph
{
public:
	int n = 0;	//���ؽ�����
	vector<Vertex> m_Vertex;	//���ؽ�
	vector<vector<pair<int, float>>> m_adj;	//���� ���->����(�Ÿ�)
	priority_queue<Vertex>	m_pq;	//�켱����ť
	map<int, pair<float, int>> m_map;
public:
	void AddVertex(int index)	//���ؽ��߰�
	{
		m_Vertex.push_back(Vertex(index));
		n = m_Vertex.size();
		m_adj.resize(n);
	}
	void addAdj(int Start, int Goal, float Dis)
	{
		m_adj[Start].push_back(make_pair(Goal, Dis));	// Start���ؽ��� Goal���ؽ������� �Ÿ� Dis
	}
	void SetStart(int index)	//����� ����
	{
		for (int i = 0; i < m_Vertex.size(); i++)
		{
			if (i == index)
			{
				m_Vertex[i].SetDist(0);	//�Ÿ��� 0
				m_Vertex[i].post = index;	//���������� ���� ����
				m_Vertex[i].isVisited = true;	//�湮���� ǥ��
			}
			else
			{
				m_Vertex[i].dist = MAX_VALUE;
				m_Vertex[i].post = -1;
				m_Vertex[i].isVisited = false;
			}
		}
	}
	void Dijkstra_AG(int s, int goal, int* Path)		//���ͽ�Ʈ�� �˰���
	{
		float dist[10];
		for (int i = 0; i < 10; i++)
		{
			dist[i] = -1 * MAX_VALUE;
		}
		dist[s] = 0;	//ó�� �Ÿ��� 0
		SetStart(s);	//�������� ����
		for (int i = 0; i < n; i++)
		{
			m_pq.push(m_Vertex[i]);	//�켱����ť�� ��� ������ ����
		}
		m_map.insert(make_pair(s, make_pair(0, -1)));
		while (!m_pq.empty())	//�켱������ ���� �ʾ�����
		{
			int index = m_pq.top().index;
			float cost = -1 * m_pq.top().dist; m_pq.pop();
			if (dist[index] < cost) continue;
			for (int i = 0; i < m_adj[index].size(); i++)
			{
				int n = m_adj[index][i].first;
				float v = m_adj[index][i].second;
				if (!m_Vertex[n].isVisited)	//�湮���� �ʾ����� ��
				{
					m_Vertex[n].isVisited = true;
					if (dist[n] > dist[index] + v) {
						dist[n] = dist[index] + v;
						Vertex newVt = Vertex(n, -dist[n], index);
						m_pq.push(newVt);
						m_map[n] = make_pair(dist[n], index);
					}
				}
			}
		}

		if (dist[goal] != -MAX_VALUE) {
			stack<int> st;
			int next = m_map[goal].second;
			int path_num = 0;
			while (next != -1)
			{
				st.push(next);
				next = m_map[next].second;
			}
			while (!st.empty())
			{
				Path[path_num] = st.top();
				st.pop();
				path_num++;
			}
			Path[path_num] = goal;
		}

		for (int i = 0; i < 10; i++)
		{
			dist[i] = -1;
		}
		DeleteMap();
	}
	bool DeleteMap()
	{
		//�ʻ���
		map<int, pair<float, int>>::iterator it1 = m_map.begin();
		while (it1 != m_map.end())
		{
			m_map.erase(it1++);
		}
		return true;
	}
	bool Release()
	{
		//���ؽ� ����
		m_Vertex.erase(m_Vertex.begin(), m_Vertex.begin() + n);
		m_adj.erase(m_adj.begin(), m_adj.end());
		return true;
	}
};

class MonsterAIManager
{
public:
	FiniteStateMachine	m_FSM;	//���¸ӽ�
	DWORD	m_dwCurrentState;	//���� ����
	DWORD	m_dwCurrentEvent;	//Process�Լ��κ��� ���� �̺�Ʈ
	Graph*	m_Graph;	//�� �и鿡���� �׷���

	int NumOfMonster;				//������ ������ ��Ÿ���� int�� ���� 0�� �ٰŸ� 1�� ���Ÿ�

	bool HavePath;
	bool NoSerachTarget;
	bool IsArraiveDest;
	int	m_Path[10];
private:
	BMapParser* m_pMap;
	//���� ������ ��� ��������
	//�ݺ�
	//������ �������� ����� ���������� ������ 2������ �����ϰ� �ѱ����� ���� ������
	//�װ��� �ٶ󺸸� �̵�
	//�̵��ϴٰ� ������Ʈ�� ������� �� ������������ ����
	//
public:
	void SetTransition(DWORD dwEvent);	//�̺�Ʈ�� ���� ���º�ȯ�Լ�
	void InsertMapMember(BMapParser* pMap);
	TVector3 SetMonsterLook(TVector3 HeroPos, TVector3 MonPos);
	float DistancePointtoPoint(TVector3 Point_1, TVector3 Point_2);
	void bubble_sort(float arr[], TVector3 arr2[], int count);		//��ǰ����
	int ReturnMostCloseHerotoSpawnPoint(TVector3 HeroPos);	//������� ��ġ�� ���� ����� ���������� ����
	int ReturnRandomSpawn(TVector3 MonPos);	//���� ��ġ�� ������ 0~6�� ������ ���ڸ� ����
	void MakeGraph();
	//TVector3 ReturnMostCloseSpawnPoint(TVector3 CurrentSpawnPoint);

	TVector3 MonsterPathing(int Path);

	bool Init();
	bool Release();
public:
	MonsterAIManager();
	~MonsterAIManager();
};

