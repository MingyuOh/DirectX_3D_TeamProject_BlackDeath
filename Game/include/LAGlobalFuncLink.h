#pragma once

//#include "LAGlobalFunc.h"
//g_rtClient
//g_fSecPerFrame



//#include "LAObjectStruct.h"
//std::vector<SUBVECTOR> m_SubUniqueVectorList;
//std::vector<SUBVECTOR> m_SubVectorList;
//std::vector<SKIN_SUB_VERTEX> m_SubSkinVecterList;
//std::vector<SKIN_SUB_VERTEX> m_SubSkinUniqueVecterList;
//std::vector<SUBINDEX> m_SubIndexList;

#include "BStd.h"

#ifndef SAFE_ZERO_POINT
#define SAFE_ZERO_POINT(A)				{ A = NULL; }
#endif

struct SPACE_PLANE
{
	float fA;
	float fB;
	float fC;
	float fD;

	SPACE_PLANE()
	{
		fA = 0.0f;
		fB = 0.0f;
		fC = 0.0f;
		fD = 0.0f;
	}
};

struct SPACE_SPHERE
{
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;

	D3DXVECTOR3 vCenter;
	D3DXVECTOR3 vAxis[3];
	float fExtent[3];
	float fRadius;

	SPACE_SPHERE()
	{
		ZeroMemory(&vMin, sizeof(D3DXVECTOR3));
		ZeroMemory(&vMax, sizeof(D3DXVECTOR3));
		ZeroMemory(&vCenter, sizeof(D3DXVECTOR3));
		ZeroMemory(&vAxis, sizeof(D3DXVECTOR3) * 3);
		ZeroMemory(&fExtent, sizeof(float) * 3);
		fRadius = 0.0f;
	}
};

struct SPACE_BOX
{
	//aabb
	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;
	//obb
	D3DXVECTOR3 vCenter;
	D3DXVECTOR3 vPos[8];
	D3DXVECTOR3 vAxis[3];
	D3DXVECTOR3 vbaseAxis[3];
	float fExtent[3];
	float fRadius;

	SPACE_BOX()
	{
		ZeroMemory(&vMin, sizeof(D3DXVECTOR3));
		ZeroMemory(&vMax, sizeof(D3DXVECTOR3));
		ZeroMemory(&vCenter, sizeof(D3DXVECTOR3));
		ZeroMemory(&vPos, sizeof(D3DXVECTOR3) * 8);
		ZeroMemory(&vAxis, sizeof(D3DXVECTOR3) * 3);
		ZeroMemory(&vbaseAxis, sizeof(D3DXVECTOR3) * 3);
		ZeroMemory(&fExtent, sizeof(float) * 3);
		fRadius = 0.0f;
	}
};

struct PNCT_VECTOR_LINE
{
	D3DXVECTOR4 Vertex;
	D3DXVECTOR4 Color;
	PNCT_VECTOR_LINE() {}
	PNCT_VECTOR_LINE(D3DXVECTOR4 Vertex0,
		D3DXVECTOR4 Color0)
	{
		Vertex = Vertex0;
		Color = Color0;
	}
};

struct PNCT_VECTOR
{
	D3DXVECTOR4 Vertex;
	D3DXVECTOR4 Nomal;
	D3DXVECTOR4 Color;
	D3DXVECTOR2 Texture;

	PNCT_VECTOR()
	{
		ZeroMemory(&Vertex, sizeof(D3DXVECTOR4));
		ZeroMemory(&Nomal, sizeof(D3DXVECTOR4));
		ZeroMemory(&Color, sizeof(D3DXVECTOR4));
		ZeroMemory(&Texture, sizeof(D3DXVECTOR2));
	}
	PNCT_VECTOR(D3DXVECTOR4 dxVertex,
		D3DXVECTOR4 dxNomal,
		D3DXVECTOR4 dxColor,
		D3DXVECTOR2 dxTexture)
	{
		Vertex = dxVertex;
		Nomal = dxNomal;
		Color = dxColor;
		Texture = dxTexture;
	}
public:
	bool operator ==(PNCT_VECTOR& source)
	{
		if ((Vertex == source.Vertex) &
			(Nomal == source.Nomal) &
			(Color == source.Color) &
			(Texture == source.Texture))
		{
			return true;
		}

		return false;
	}

};

struct PNCT5_VERTEX
{
	D3DXVECTOR4 Vertex;
	D3DXVECTOR4 Nomal;
	D3DXVECTOR4 Color;
	D3DXVECTOR2 Texture;
	float ObjNum1[4];
	float Weight1[4];
	float ObjNum2[4];
	float Weight2[4];

	PNCT5_VERTEX()
	{
		ZeroMemory(&Vertex, sizeof(D3DXVECTOR4));
		ZeroMemory(&Nomal, sizeof(D3DXVECTOR4));
		ZeroMemory(&Color, sizeof(D3DXVECTOR4));
		ZeroMemory(&Texture, sizeof(D3DXVECTOR2));
		ZeroMemory(&ObjNum1, sizeof(float) * 4);
		ZeroMemory(&Weight1, sizeof(float) * 4);
		ZeroMemory(&ObjNum2, sizeof(float) * 4);
		ZeroMemory(&Weight2, sizeof(float) * 4);
	}
	PNCT5_VERTEX(D3DXVECTOR4 dxVertex,
		D3DXVECTOR4 dxNomal,
		D3DXVECTOR4 dxColor,
		D3DXVECTOR2 dxTexture)
	{
		Vertex = dxVertex;
		Nomal = dxNomal;
		Color = dxColor;
		Texture = dxTexture;
	}
public:
	bool operator == (PNCT5_VERTEX& source)
	{
		if ((Vertex == source.Vertex) &
			(Nomal == source.Nomal) &
			(Color == source.Color) &
			(Texture == source.Texture)
			)
		{
			return true;
		}

		return false;
	}
};

struct VERTEX_CB
{
	D3DXMATRIX g_matWorld;		//c0
	D3DXMATRIX g_matView;		//c4
	D3DXMATRIX g_matProj;		//c8
	D3DXMATRIX g_matWorldInverse;		//c16
};

struct DataCB
{
	//D3DXMATRIX matNormal;
	D3DXVECTOR4 vLightPos;
	D3DXVECTOR4 vLIghtDirection;
	D3DXVECTOR4 vEyePosition;
	D3DXVECTOR4 vData01;
};

struct CrateObjectDest
{
	TCHAR* vertexShaderName;
	char* vertexFuntionNmae;
	TCHAR* PixelShaderName;
	char* PixelFuntionNmae;
	TCHAR* GiometryShaderName;
	char* GiometryFuntionNmae;
	TCHAR* pTextureName;

	CrateObjectDest()
	{
		vertexShaderName = NULL;
		vertexFuntionNmae = NULL;
		PixelShaderName = NULL;
		PixelFuntionNmae = NULL;
		GiometryShaderName = NULL;
		GiometryFuntionNmae = NULL;
		pTextureName = NULL;
	}
};

typedef std::vector<PNCT_VECTOR> SUBVECTOR;
typedef std::vector<PNCT5_VERTEX> SKIN_SUB_VERTEX;
typedef std::vector<DWORD> SUBINDEX;

enum class E_LoadParserType
{
	ASE_TYPE = 0,
	PSB_TYPE,
};

enum class E_LoadObjectType
{
	NONE,
	CHARACTER,
	CHARACTER_MATRIX,
	CHARACTER_SKIN,
	CHARACTER_ANIMATION,
	CHARACTER_ITEM,
	ITEM,

};

class LABoneObject;
class LASkinObject;
struct CheckObject
{
	bool bisRander;
	LABoneObject* pMatrixObject;
	LASkinObject* pSkinObject;
	CheckObject()
	{
		bisRander = true;
		pMatrixObject = NULL;
		pSkinObject = NULL;
	}
};

struct SectionInfo
{
	bool Result;
	TCHAR* pHeaderName;

	SectionInfo()
	{
		Result = false;
	}
};

const int MAX_BONE_MATRICES = 255;

struct CBConstBoneWorld
{
	D3DXMATRIX g_mConstBoneWorld[MAX_BONE_MATRICES];
};

enum class E_SpaceType
{
	None,
	Plane,
	Box,
	Sphere,
	BoxPoint,
	SpherePoint,
};