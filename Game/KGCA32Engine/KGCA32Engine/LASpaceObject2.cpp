
//#include "stdafx.h"


#include "LASpaceObject2.h"

void LASpaceObject2::Update(TMatrix& matDest)
{
	m_matWorld = matDest;

	switch ((int)m_CurrentSpaceType)
	{
		case (int)E_SpaceType::Box:
		{
			m_pSpaceBox->UpDate(matDest);
		}break;

		case (int)E_SpaceType::Sphere:
		{
			m_pSpaceSpher->UpDate(matDest);
		}break;
	}
}

void LASpaceObject2::CreatePlane()
{
	m_pSpacePlane = new LASpacePlane2;
	m_CurrentSpaceType = E_SpaceType::Plane;
	m_bLiveFlag = true;
}

void LASpaceObject2::CreateBox()
{
	m_pSpaceBox = new LASpaceBox2;
	m_CurrentSpaceType = E_SpaceType::Box;
	m_bLiveFlag = true;
}

void LASpaceObject2::CreateSphere()
{
	m_pSpaceSpher = new LASpaceSphere2;
	m_CurrentSpaceType = E_SpaceType::Sphere;
	m_bLiveFlag = true;
}


LASpaceObject2::~LASpaceObject2()
{

#ifdef TOOL_MODE

	SAFE_DEL(m_pSpacePlane);
	SAFE_DEL(m_pSpaceBox);
	SAFE_DEL(m_pSpaceSpher);

#else

	S_DEL(m_pSpacePlane);
	S_DEL(m_pSpaceBox);
	S_DEL(m_pSpaceSpher);

#endif
	
}
