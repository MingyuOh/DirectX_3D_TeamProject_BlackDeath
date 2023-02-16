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
	ID3D11VertexShader* m_pBoneVS;				//���ؽ� ���̴�
	ID3D11PixelShader*  m_pBonePS;				//�ȼ� ���̴�
	ID3D11VertexShader* m_pSkinVS;				//���ؽ� ���̴�
	ID3D11PixelShader*  m_pSkinPS;				//�ȼ� ���̴�
	ID3DBlob* m_pBoneVSBlob;
	ID3DBlob* m_pSkinVSBlob;
	SceneData m_CharacterSceneData;
	std::vector<LABoneObject*> m_BoneObjectList;
	std::vector<LASkinObject*> m_SkinObjectList;
	std::vector<CheckObject> m_SkinRenderList;
	std::vector<SceneData> m_AnimationList;	//��ǳѹ��� ���ͼ����״�λ��, ����� �����µ� �ʿ��� ������
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

