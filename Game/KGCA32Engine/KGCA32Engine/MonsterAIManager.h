#pragma once
#include "FiniteStateMachine.h"
#include "FSMState.h"
#include "BMapParser.h"
#include <random>


#define MAX_VALUE -987654321 
//정점 클래스
class Vertex
{
public:
	int index;	//정점 번호
	float dist;	//거리
	int post = -1;	//이전 정점	

	bool isVisited;
public:
	void SetDist(float D)	//거리셋팅
	{
		dist = D;
	}
	//오퍼레이터
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
	};	//생성자
	Vertex(int i, float d, int p)
	{
		index = i;
		dist = d;
		post = p;
	};
	~Vertex() {};
};

//그래프 클래스
class Graph
{
public:
	int n = 0;	//버텍스갯수
	vector<Vertex> m_Vertex;	//버텍스
	vector<vector<pair<int, float>>> m_adj;	//인접 출발->도착(거리)
	priority_queue<Vertex>	m_pq;	//우선순위큐
	map<int, pair<float, int>> m_map;
public:
	void AddVertex(int index)	//버텍스추가
	{
		m_Vertex.push_back(Vertex(index));
		n = m_Vertex.size();
		m_adj.resize(n);
	}
	void addAdj(int Start, int Goal, float Dis)
	{
		m_adj[Start].push_back(make_pair(Goal, Dis));	// Start버텍스의 Goal버텍스까지의 거리 Dis
	}
	void SetStart(int index)	//출발점 셋팅
	{
		for (int i = 0; i < m_Vertex.size(); i++)
		{
			if (i == index)
			{
				m_Vertex[i].SetDist(0);	//거리는 0
				m_Vertex[i].post = index;	//이전정점은 현재 정점
				m_Vertex[i].isVisited = true;	//방문함을 표시
			}
			else
			{
				m_Vertex[i].dist = MAX_VALUE;
				m_Vertex[i].post = -1;
				m_Vertex[i].isVisited = false;
			}
		}
	}
	void Dijkstra_AG(int s, int goal, int* Path)		//다익스트라 알고리즘
	{
		float dist[10];
		for (int i = 0; i < 10; i++)
		{
			dist[i] = -1 * MAX_VALUE;
		}
		dist[s] = 0;	//처음 거리는 0
		SetStart(s);	//시작정점 셋팅
		for (int i = 0; i < n; i++)
		{
			m_pq.push(m_Vertex[i]);	//우선순위큐에 모든 정점을 셋팅
		}
		m_map.insert(make_pair(s, make_pair(0, -1)));
		while (!m_pq.empty())	//우선순위가 비지 않았을때
		{
			int index = m_pq.top().index;
			float cost = -1 * m_pq.top().dist; m_pq.pop();
			if (dist[index] < cost) continue;
			for (int i = 0; i < m_adj[index].size(); i++)
			{
				int n = m_adj[index][i].first;
				float v = m_adj[index][i].second;
				if (!m_Vertex[n].isVisited)	//방문하지 않았을때 비교
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
		//맵삭제
		map<int, pair<float, int>>::iterator it1 = m_map.begin();
		while (it1 != m_map.end())
		{
			m_map.erase(it1++);
		}
		return true;
	}
	bool Release()
	{
		//버텍스 삭제
		m_Vertex.erase(m_Vertex.begin(), m_Vertex.begin() + n);
		m_adj.erase(m_adj.begin(), m_adj.end());
		return true;
	}
};

class MonsterAIManager
{
public:
	FiniteStateMachine	m_FSM;	//상태머신
	DWORD	m_dwCurrentState;	//현재 상태
	DWORD	m_dwCurrentEvent;	//Process함수로부터 얻을 이벤트
	Graph*	m_Graph;	//각 분면에서의 그래프

	int NumOfMonster;				//몬스터의 종류를 나타내는 int형 변수 0은 근거리 1은 원거리

	bool HavePath;
	bool NoSerachTarget;
	bool IsArraiveDest;
	int	m_Path[10];
private:
	BMapParser* m_pMap;
	//현재 구역의 모든 스폰지역
	//반복
	//스폰된 지역에서 가까운 스폰지역중 가까운곳 2군데중 랜덤하게 한군데를 리턴 선정후
	//그곳을 바라보며 이동
	//이동하다가 오브젝트를 만날경우 전 스폰지역으로 리턴
	//
public:
	void SetTransition(DWORD dwEvent);	//이벤트에 따른 상태변환함수
	void InsertMapMember(BMapParser* pMap);
	TVector3 SetMonsterLook(TVector3 HeroPos, TVector3 MonPos);
	float DistancePointtoPoint(TVector3 Point_1, TVector3 Point_2);
	void bubble_sort(float arr[], TVector3 arr2[], int count);		//거품정렬
	int ReturnMostCloseHerotoSpawnPoint(TVector3 HeroPos);	//히어로의 위치와 가장 가까운 스폰지점을 리턴
	int ReturnRandomSpawn(TVector3 MonPos);	//현재 위치를 제외한 0~6중 랜덤한 숫자를 리턴
	void MakeGraph();
	//TVector3 ReturnMostCloseSpawnPoint(TVector3 CurrentSpawnPoint);

	TVector3 MonsterPathing(int Path);

	bool Init();
	bool Release();
public:
	MonsterAIManager();
	~MonsterAIManager();
};

