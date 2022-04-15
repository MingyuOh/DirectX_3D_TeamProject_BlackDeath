#pragma once

#include "CoreFuncLink.h"

#include "LASpacePlane2.h"
#include "LASpaceBox2.h"
#include "LASpaceSphere2.h"

class LASpaceObject2
{
public:
	E_SpaceType m_CurrentSpaceType;
	LASpacePlane2* m_pSpacePlane;
	LASpaceBox2* m_pSpaceBox;
	LASpaceSphere2* m_pSpaceSpher;
	TMatrix m_matWorld;
	TVector4 m_vColor;
	TVector3 m_vIntersection;
	bool m_bLiveFlag;
public:
	void Update(TMatrix& matDest);
	void CreatePlane();
	void CreateBox();
	void CreateSphere();
public:
	LASpaceObject2()
	{
		m_bLiveFlag = false;
		ZeroMemory(&m_vIntersection, sizeof(TVector3));
		m_CurrentSpaceType = E_SpaceType::None;
		m_pSpacePlane = NULL;
		m_pSpaceBox = NULL;
		m_pSpaceSpher = NULL;
		D3DXMatrixIdentity(&m_matWorld);
		m_vColor = TVector4((rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, 1.0f);
	}
	~LASpaceObject2();
};

