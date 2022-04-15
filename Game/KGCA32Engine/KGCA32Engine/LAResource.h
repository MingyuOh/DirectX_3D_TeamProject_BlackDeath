#pragma once

#include "CoreStandardLink.h"

#include "LABoneObject.h"
#include "LASkinObject.h"

class LAResource
{
public:
	std::wstring m_ResourceName;
	std::wstring m_ResourceCodeName;
	std::wstring m_ResourceMatrixName;
	std::wstring m_ResourceSkinName;
	std::wstring m_ResourceParentName;
	ID3D11VertexShader* m_pBoneVS;				//버텍스 쉐이더
	ID3D11PixelShader*  m_pBonePS;				//픽셀 쉐이더
	ID3D11VertexShader* m_pSkinVS;				//버텍스 쉐이더
	ID3D11PixelShader*  m_pSkinPS;				//픽셀 쉐이더
	ID3DBlob* m_pBoneVSBlob;
	ID3DBlob* m_pSkinVSBlob;
	SceneData m_CharacterSceneData;
	std::vector<LABoneObject*> m_BoneObjectList;
	std::vector<LASkinObject*> m_SkinObjectList;
	std::vector<CheckObject> m_SkinRenderList;
	std::vector<SceneData> m_AnimationList;	//모션넘버는 벡터순서그대로사용, 모션을 돌리는데 필요한 씬정보
	bool m_bIsSkining;
public:
	LAResource()
	{
		m_bIsSkining = false;
		m_pBoneVS = NULL;
		m_pBonePS = NULL;
		m_pSkinVS = NULL;
		m_pSkinPS = NULL;
		m_pBoneVSBlob = NULL;
		m_pSkinVSBlob = NULL;
	}
	~LAResource();
};

