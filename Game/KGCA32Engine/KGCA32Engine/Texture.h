#pragma once
#include "Bstd.h"

class Texture
{
public:
	//--------------------------------------------------------------------------------------
	// 텍스쳐 로딩 및 리소스버퍼 생성
	//--------------------------------------------------------------------------------------
	vector<ComPtr<ID3D11ShaderResourceView>>	m_pTextureSRV;
	//--------------------------------------------------------------------------------------
	// 텍스쳐 이름
	//--------------------------------------------------------------------------------------
	vector<T_STR>								m_pTextureNameList;
	ComPtr<ID3D11SamplerState>					m_pSampler;
public:
	UINT										m_iCurrentIndex = 0;
	UINT										m_iMaxIndex = 1;
	UINT										m_iIndices = 0;
public:
	bool										LoadTexture(T_STR szFilePath, T_STR szExtension);
	bool										LoadDDS(T_STR szFilePath);
	bool										LoadTGA(T_STR szFilePath);
	bool										LoadWIC(T_STR szFilePath);

	bool										Apply(ID3D11DeviceContext* pImmediateContext);
	bool										Release();
public:
	Texture();
	~Texture();
};

