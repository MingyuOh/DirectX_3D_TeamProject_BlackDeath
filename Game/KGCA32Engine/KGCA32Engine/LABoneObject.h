#pragma once

#include "LABox2.h"
#include "LAMesh.h"

class LABoneObject : public LAObject2
{
public:
	LAMesh* m_MeshData;
	LABox2* m_pDummyRenderBox;
	//Type 기본값 GeomObject
	TMatrix* m_pmatParent;
	TMatrix* m_pmatCharacter;
	TMatrix m_matChildWorld;
	TMatrix m_matCalculation;
	TMatrix m_matInversAni;
	TMatrix m_matObjectLocalPos;
	TMatrix m_matObjectLocalScale;
	TMatrix m_matObjectLocalRotation;
	
	bool m_bSetParentTick;
	int m_ParentObjectTick;
	int m_iCurrentMotion;
	int m_BoneTick;
	bool m_SelectObject;
	bool m_bIsCreate;
	bool m_bIsNewDummy;
public:
	//================================================================================
	//	** 클래스 함수 ** 
	//================================================================================
	void DrawMesh();
	void SetCrrentTick(bool bCheck);
	bool Interpolate(TMatrix& TargetMatrix, int iCurTick);
	bool GetAnimationTrack(std::vector<AnimationTrack>& TrackList, int& NumstartTrack, int& NumEndTrack, float fCurTick);
	bool SetAnimationTrack(float fCurTick);
	void SetSelectObject(bool bcheck);
	//void SetMatrix(TMatrix* pWorldMatrix, TMatrix* pViewMatrix, TMatrix* pProjectionMatrix);
	
public:
	//================================================================================
	//	** 재정의 함수 ** 
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

