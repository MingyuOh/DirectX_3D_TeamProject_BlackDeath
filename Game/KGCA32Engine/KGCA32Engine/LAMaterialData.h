#pragma once

#include "CoreStandardLink.h"
#include "Texture.h"

struct TexMapData
{
	int iTypeNumber;
	std::wstring pBitmapName;
	Texture texture;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureSRV;

	TexMapData()
	{
		iTypeNumber = -1;
	}
};

struct SubMaterialData
{
	int iSubMaterialNumber;
	std::vector<TexMapData> MapData;

	SubMaterialData()
	{
		iSubMaterialNumber = 0;
	}
};

class LAMaterialData
{
public:
	int iMaterialNumber;
	std::vector<SubMaterialData> m_SubMaterialArray;
public:
	LAMaterialData()
	{
		iMaterialNumber = 0;
	}
	~LAMaterialData();
};

