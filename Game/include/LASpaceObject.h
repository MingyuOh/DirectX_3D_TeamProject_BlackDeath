#pragma once

#include "LAGlobalFuncLink.h"

#include "LASpacePlane.h"
#include "LASpaceBox.h"
#include "LASpaceSphere.h"

class LASpaceObject
{
public:
	E_SpaceType m_CurrentSpaceType;
	LASpacePlane* m_pSpacePlane;
	LASpaceBox* m_pSpaceBox;
	LASpaceSphere* m_pSpaceSpher;
	D3DXMATRIX m_matWorld;
	D3DXVECTOR4 m_vColor;
	D3DXVECTOR3 m_vIntersection;
	bool m_bLiveFlag;
public:
	void Update(D3DXMATRIX& matDest);
	void CreatePlane();
	void CreateBox();
	void CreateSphere();
public:
	LASpaceObject()
	{
		m_bLiveFlag = false;
		ZeroMemory(&m_vIntersection, sizeof(D3DXVECTOR3));
		m_CurrentSpaceType = E_SpaceType::None;
		m_pSpacePlane = NULL;
		m_pSpaceBox = NULL;
		m_pSpaceSpher = NULL;
		D3DXMatrixIdentity(&m_matWorld);
		m_vColor = D3DXVECTOR4((rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1.0f);
	}
	~LASpaceObject();
};

