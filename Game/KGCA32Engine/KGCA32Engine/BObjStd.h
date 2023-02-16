#pragma once
#include "BStd.h"

struct B_BOX
{
	//공용
	TVector3 vCenter;
	TVector3 vPos[8];

	// AABB
	TVector3 vMax;
	TVector3 vMin;

	//OBB
	TVector3 vAxis[3];
	float fExtent[3];
};

struct PNCT_Vertex
{
	TVector3 Position;
	TVector3 Normal;
	TVector4 Color;
	TVector2 Texture;
	bool operator == (const PNCT_Vertex & Vertex)
	{
		if (Position == Vertex.Position  && Normal == Vertex.Normal && 	Color == Vertex.Color  &&	Texture == Vertex.Texture)
		{
			return true;
		}
		return  false;
	}
	PNCT_Vertex() {}
	PNCT_Vertex(TVector3 vp, TVector3	vn, TVector4 vc, TVector2 vt)
	{
		Position = vp, Normal = vn, Color = vc, Texture = vt;
	}
};

/*상수버퍼 구조체*/
struct VSConstantB
{
	TMatrix m_matWorld;
	TMatrix m_matView;
	TMatrix m_matProj;
	TMatrix m_matNormal;
	float f_Time;
	float f_Y;
	float f_Z;
	float f_W;
};