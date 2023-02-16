#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

bool Texture::LoadTexture(T_STR szFilePath, T_STR szExtension)
{
	if (szExtension == _T(".dds"))
	{
		return LoadDDS(szFilePath);
	}
	else if (szExtension == _T(".tga"))
	{
		return LoadTGA(szFilePath);
	}
	else
	{
		return LoadWIC(szFilePath);
	}
	return false;
}

bool Texture::Apply(ID3D11DeviceContext* pImmediateContext)
{
	for (auto& texture : m_pTextureSRV)
	{
		pImmediateContext->PSSetShaderResources(0, 1, texture.GetAddressOf());
	}
	return true;
}

bool Texture::Release()
{
	/*for (auto& textureSRV : m_pTextureSRV)
	{
		SAFE_RELEASE(textureSRV);
	}*/
	m_pTextureSRV.clear();
	return true;
}

bool Texture::LoadDDS(T_STR szFilePath)
{
	TexMetadata mdata;
	HRESULT hr = GetMetadataFromDDSFile(szFilePath.c_str(), DDS_FLAGS_NONE, mdata);

	if (FAILED(hr))
	{
		wchar_t buff[2048] = {};
		swprintf_s(buff, L"Failed to open texture file\n\nFilename = %ls\nHRESULT %08X", szFilePath.c_str(), static_cast<unsigned int>(hr));
		MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	if (mdata.dimension == TEX_DIMENSION_TEXTURE3D)
	{
		if (mdata.arraySize > 1)
		{
			wchar_t buff[2048] = {};
			swprintf_s(buff, L"Arrays of volume textures are not supported\n\nFilename = %ls\nArray size %zu", szFilePath.c_str(), mdata.arraySize);
			MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}

		m_iMaxIndex = static_cast<UINT>(mdata.depth);
	}
	else
	{
		m_iMaxIndex = static_cast<UINT>(mdata.arraySize);
	}

	switch (mdata.format)
	{
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		/*if (m_FeatureLevel < D3D_FEATURE_LEVEL_11_0)
		{
			wchar_t buff[2048] = {};
			swprintf_s(buff, L"BC6H/BC7 requires DirectX 11 hardware\n\nFilename = %ls\nDXGI Format %d\nFeature Level %d", name.c_str(), mdata.format, m_FeatureLevel);
			MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}*/
		break;

	default:
	{
		UINT flags = 0;
		hr = g_pd3dDevice->CheckFormatSupport(mdata.format, &flags);
		if (FAILED(hr) || !(flags & (D3D11_FORMAT_SUPPORT_TEXTURE1D | D3D11_FORMAT_SUPPORT_TEXTURE2D | D3D11_FORMAT_SUPPORT_TEXTURE3D)))
		{
			wchar_t buff[2048] = {};
			//swprintf_s(buff, L"Format not supported by DirectX hardware\n\nFilename = %ls\nDXGI Format %d\nFeature Level %d\nHRESULT = %08X", szFilePath.c_str(), mdata.format, m_FeatureLevel, static_cast<unsigned int>(hr));
			MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}
	}
	break;
	}

	ScratchImage image;
	ID3D11ShaderResourceView* pShaderResouceView;
	hr = LoadFromDDSFile(szFilePath.c_str(), DDS_FLAGS_NONE, &mdata, image);
	if (FAILED(hr))
	{
		wchar_t buff[2048] = {};
		swprintf_s(buff, L"Failed to load texture file\n\nFilename = %ls\nHRESULT %08X", szFilePath.c_str(), static_cast<unsigned int>(hr));
		MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// Special case to make sure Texture cubes remain arrays
	mdata.miscFlags &= ~TEX_MISC_TEXTURECUBE;

	hr = CreateShaderResourceView(g_pd3dDevice, image.GetImages(), image.GetImageCount(), mdata, &pShaderResouceView);
	if (FAILED(hr))
	{
		wchar_t buff[2048] = {};
		swprintf_s(buff, L"Failed creating texture from file\n\nFilename = %ls\nHRESULT = %08X", szFilePath.c_str(), static_cast<unsigned int>(hr));
		MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	m_pTextureSRV.push_back(pShaderResouceView);
	m_pTextureNameList.push_back(szFilePath);
	wchar_t p[30] = L"save.dds";
	SaveToDDSFile(*image.GetImages(), DDS_FLAGS_NONE, p);
	return true;
}

bool Texture::LoadTGA(T_STR szFilePath)
{
	TexMetadata mdata;
	HRESULT hr = GetMetadataFromTGAFile(szFilePath.c_str(), TGA_FLAGS_NONE, mdata);
	if (FAILED(hr))
	{
		wchar_t buff[2048] = {};
		swprintf_s(buff, L"Failed to open texture file\n\nFilename = %ls\nHRESULT %08X", szFilePath.c_str(), static_cast<unsigned int>(hr));
		MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	if (mdata.dimension == TEX_DIMENSION_TEXTURE3D)
	{
		if (mdata.arraySize > 1)
		{
			wchar_t buff[2048] = {};
			swprintf_s(buff, L"Arrays of volume textures are not supported\n\nFilename = %ls\nArray size %zu", szFilePath.c_str(), mdata.arraySize);
			MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}

		m_iMaxIndex = static_cast<UINT>(mdata.depth);
	}
	else
	{
		m_iMaxIndex = static_cast<UINT>(mdata.arraySize);
	}

	switch (mdata.format)
	{
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		/*if (m_FeatureLevel < D3D_FEATURE_LEVEL_11_0)
		{
			wchar_t buff[2048] = {};
			swprintf_s(buff, L"BC6H/BC7 requires DirectX 11 hardware\n\nFilename = %ls\nDXGI Format %d\nFeature Level %d", name.c_str(), mdata.format, m_FeatureLevel);
			MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}*/
		break;

	default:
	{
		UINT flags = 0;
		hr = g_pd3dDevice->CheckFormatSupport(mdata.format, &flags);
		if (FAILED(hr) || !(flags & (D3D11_FORMAT_SUPPORT_TEXTURE1D | D3D11_FORMAT_SUPPORT_TEXTURE2D | D3D11_FORMAT_SUPPORT_TEXTURE3D)))
		{
			wchar_t buff[2048] = {};
			//swprintf_s(buff, L"Format not supported by DirectX hardware\n\nFilename = %ls\nDXGI Format %d\nFeature Level %d\nHRESULT = %08X", szFilePath.c_str(), mdata.format, m_FeatureLevel, static_cast<unsigned int>(hr));
			MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}
	}
	break;
	}

	ScratchImage image;
	ID3D11ShaderResourceView* pShaderResouceView;
	hr = LoadFromTGAFile(szFilePath.c_str(), TGA_FLAGS_NONE, &mdata, image);
	if (FAILED(hr))
	{
		wchar_t buff[2048] = {};
		swprintf_s(buff, L"Failed to load texture file\n\nFilename = %ls\nHRESULT %08X", szFilePath.c_str(), static_cast<unsigned int>(hr));
		MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// Special case to make sure Texture cubes remain arrays
	mdata.miscFlags &= ~TEX_MISC_TEXTURECUBE;

	hr = CreateShaderResourceView(g_pd3dDevice, image.GetImages(), image.GetImageCount(), mdata, &pShaderResouceView);
	if (FAILED(hr))
	{
		wchar_t buff[2048] = {};
		swprintf_s(buff, L"Failed creating texture from file\n\nFilename = %ls\nHRESULT = %08X", szFilePath.c_str(), static_cast<unsigned int>(hr));
		MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	m_pTextureSRV.push_back(pShaderResouceView);
	m_pTextureNameList.push_back(szFilePath);

	return true;
}
bool Texture::LoadWIC(T_STR szFilePath)
{
	TexMetadata mdata;
	HRESULT hr = GetMetadataFromWICFile(szFilePath.c_str(), WIC_FLAGS_NONE, mdata);
	if (FAILED(hr))
	{
		wchar_t buff[2048] = {};
		swprintf_s(buff, L"Failed to open texture file\n\nFilename = %ls\nHRESULT %08X", szFilePath.c_str(), static_cast<unsigned int>(hr));
		MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	if (mdata.dimension == TEX_DIMENSION_TEXTURE3D)
	{
		if (mdata.arraySize > 1)
		{
			wchar_t buff[2048] = {};
			swprintf_s(buff, L"Arrays of volume textures are not supported\n\nFilename = %ls\nArray size %zu", szFilePath.c_str(), mdata.arraySize);
			MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}

		m_iMaxIndex = static_cast<UINT>(mdata.depth);
	}
	else
	{
		m_iMaxIndex = static_cast<UINT>(mdata.arraySize);
	}

	switch (mdata.format)
	{
	case DXGI_FORMAT_BC6H_TYPELESS:
	case DXGI_FORMAT_BC6H_UF16:
	case DXGI_FORMAT_BC6H_SF16:
	case DXGI_FORMAT_BC7_TYPELESS:
	case DXGI_FORMAT_BC7_UNORM:
	case DXGI_FORMAT_BC7_UNORM_SRGB:
		/*if (m_FeatureLevel < D3D_FEATURE_LEVEL_11_0)
		{
			wchar_t buff[2048] = {};
			swprintf_s(buff, L"BC6H/BC7 requires DirectX 11 hardware\n\nFilename = %ls\nDXGI Format %d\nFeature Level %d", name.c_str(), mdata.format, m_FeatureLevel);
			MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}*/
		break;

	default:
	{
		UINT flags = 0;
		hr = g_pd3dDevice->CheckFormatSupport(mdata.format, &flags);
		if (FAILED(hr) || !(flags & (D3D11_FORMAT_SUPPORT_TEXTURE1D | D3D11_FORMAT_SUPPORT_TEXTURE2D | D3D11_FORMAT_SUPPORT_TEXTURE3D)))
		{
			wchar_t buff[2048] = {};
			//swprintf_s(buff, L"Format not supported by DirectX hardware\n\nFilename = %ls\nDXGI Format %d\nFeature Level %d\nHRESULT = %08X", szFilePath.c_str(), mdata.format, m_FeatureLevel, static_cast<unsigned int>(hr));
			MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
			return 0;
		}
	}
	break;
	}

	ScratchImage image;
	ID3D11ShaderResourceView* pShaderResouceView;
	hr = LoadFromWICFile(szFilePath.c_str(), WIC_FLAGS_NONE, &mdata, image);
	if (FAILED(hr))
	{
		wchar_t buff[2048] = {};
		swprintf_s(buff, L"Failed to load texture file\n\nFilename = %ls\nHRESULT %08X", szFilePath.c_str(), static_cast<unsigned int>(hr));
		MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	// Special case to make sure Texture cubes remain arrays
	mdata.miscFlags &= ~TEX_MISC_TEXTURECUBE;

	hr = CreateShaderResourceView(g_pd3dDevice, image.GetImages(), image.GetImageCount(), mdata, &pShaderResouceView);
	if (FAILED(hr))
	{
		wchar_t buff[2048] = {};
		swprintf_s(buff, L"Failed creating texture from file\n\nFilename = %ls\nHRESULT = %08X", szFilePath.c_str(), static_cast<unsigned int>(hr));
		MessageBoxW(nullptr, buff, L"DDSView", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	m_pTextureSRV.push_back(pShaderResouceView);
	m_pTextureNameList.push_back(szFilePath);

	return true;
}