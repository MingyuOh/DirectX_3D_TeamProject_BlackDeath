#pragma once

#include "LABox2.h"
#include "LAMesh.h"

class LABoneObject : public LAObject2
{
public:
	LAMesh* m_MeshData;
	LABox2* m_pDummyRenderBox;
	//Type �⺻�� GeomObject
	D3DXMATRIX* m_pmatParent;
	D3DXMATRIX* m_pmatCharacter;
	D3DXMATRIX m_matChildWorld;
	D3DXMATRIX m_matCalculation;
	D3DXMATRIX m_matInversAni;
	D3DXMATRIX m_matObjectLocalPos;
	D3DXMATRIX m_matObjectLocalScale;
	D3DXMATRIX m_matObjectLocalRotation;
	
	bool m_bSetParentTick;
	int m_ParentObjectTick;
	int m_iCurrentMotion;
	int m_BoneTick;
	bool m_SelectObject;
	bool m_bIsCreate;
	bool m_bIsNewDummy;
public:
	//================================================================================
	//	** Ŭ���� �Լ� ** 
	//================================================================================
	void DrawMesh();
	void SetCrrentTick(bool bCheck);
	bool Interpolate(D3DXMATRIX& TargetMatrix, int iCurTick);
	bool GetAnimationTrack(std::vector<AnimationTrack>& TrackList, int& NumstartTrack, int& NumEndTrack, float fCurTick);
	bool SetAnimationTrack(float fCurTick);
	void SetSelectObject(bool bcheck);
	//void SetMatrix(D3DXMATRIX* pWorldMatrix, D3DXMATRIX* pViewMatrix, D3DXMATRIX* pProjectionMatrix);
	
public:
	//================================================================================
	//	** ������ �Լ� ** 
	//================================================================================
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT LoadTextureResource();
	bool Frame();
	bool Release();
	bool Render();
public:
	LABoneObject()
	{
		m_bIsNewDummy = false;
		m_bSetParentTick = false;
		m_MeshData = NULL;
		m_pmatParent = NULL;
		m_pDummyRenderBox = NULL;
		m_pmatCharacter = NULL;
		m_BoneTick = 0;
		m_SelectObject = false;
		m_ParentObjectTick = 0;
		m_iCurrentMotion = -1;
		m_bIsCreate = false;
	}
	virtual ~LABoneObject();
};

