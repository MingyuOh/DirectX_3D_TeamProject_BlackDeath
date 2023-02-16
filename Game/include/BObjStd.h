#pragma once
#include "BStd.h"

struct B_BOX
{
	//공용
	D3DXVECTOR3 vCenter;
	D3DXVECTOR3 vPos[8];

	// AABB
	D3DXVECTOR3 vMax;
	D3DXVECTOR3 vMin;

	//OBB
	D3DXVECTOR3 vAxis[3];
	float fExtent[3];
};

struct PNCT_Vertex
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR4 Color;
	D3DXVECTOR2 Texture;
	bool operator == (const PNCT_Vertex & Vertex)
	{
		if (Position == Vertex.Position  && Normal == Vertex.Normal && 	Color == Vertex.Color  &&	Texture == Vertex.Texture)
		{
			return true;
		}
		return  false;
	}
	PNCT_Vertex() {}
	PNCT_Vertex(D3DXVECTOR3 vp, D3DXVECTOR3	vn, D3DXVECTOR4 vc, D3DXVECTOR2 vt)
	{
		Position = vp, Normal = vn, Color = vc, Texture = vt;
	}
};

/*상수버퍼 구조체*/
struct VSConstantB
{
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	D3DXMATRIX m_matNormal;
	float f_Time;
	float f_Y;
	float f_Z;
	float f_W;
};