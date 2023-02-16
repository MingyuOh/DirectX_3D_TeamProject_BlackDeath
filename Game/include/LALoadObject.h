#pragma once



#include "LAObject2.h"
#include "LAAnimaionData.h"

#include "LAHero.h"
#include "CoreStandardLink.h"
//#include "LACharacterStruct.h"

class LALoadObject
{
public:
	//LAWrite m_CurrentTickWrite;
	//New
	LAHero* m_NewLoadObject;

	//std::wstring m_LoadMaxObjectName;
	//std::wstring m_MatrixName;
	std::vector<SceneData> m_LoadObjectSceneDataList;
	SceneData m_SelectSceneData;
	//CrateObjectDest m_BoneShaderDest;
	//CrateObjectDest m_SkinShaderDest;
	//CBConstBoneWorld m_cbMatrixListData;
	//int m_iCurrentTick;

	//Matrix Render
	bool m_bBipedRender;
	//std::vector<LABoneObject*> m_BoneObjectList;

	//Skin Rander
	bool m_bSkinRender;
	//std::vector<LASkinObject*> m_SkinObjectList;
	//std::vector<CheckObject> m_SkinRenderList;

	//애니메이션 구간 플레이 
	bool m_bTestSectionAnimationPlay;
	SectionPlay m_TestSectionAnimationTick;
	//애니메이션 분해한 리스트
	std::list<SaveAnimationDataInfo> m_SaveAnimationDataList;
	std::list<SaveAnimationDataInfo>::iterator m_SaveObjecDatatItor;

	//check
	bool m_bAnimationPlay;
	bool m_bControlTickSet;

	//================================================================================
	//	** Animation Tick ** 
	//================================================================================
	//int m_NumAnimation;
	int m_iCurrentTick;
	float m_fElapseFrame;
	int m_iCurFrame;
	float m_fTickFrame;
public:
	//================================================================================
	//	** 클래스 함수 ** 
	//================================================================================
	void SetWriteSwapChain(IDXGISwapChain* pSawpChain);
	//bool BoneObjCreate(CrateObjectDest* pObjectDest = NULL);
	//bool SkinObjCreate(CrateObjectDest* pObjectDest = NULL);
	//void SetCBBoneData();
	void ControlBoxUpdateAniTick(int& iTargetTick);
	void SetAnimationPlay(bool bPlayStop);
	void ResetTestSectionAnimationTick();
	int GetCrrentTickFrame();
	void PlaySectionAnimation(int iFirstFrame, int iLastFrame, bool bReplay);
	LASkinObject* GetSkinObject(std::wstring ObjectName);
	LABoneObject* GetMatrixObject(std::wstring ObjectName);
	//애니메이션 리스트 관련 함수
	void AddAnimationData(SaveAnimationDataInfo& data);
	bool DeleteSaveObjectData(std::wstring wsDataName);
	SaveAnimationDataInfo* FindSaveObjectData(std::wstring wsDataName);
	void SetDirectionLight(D3DXVECTOR4 vCameraPos, D3DXVECTOR4 vLigthPos, D3DXVECTOR4 vLightDirection);
	//================================================================================
	//	** My SDK Fuction ** 
	//================================================================================
	void SetFrame(int iSetFrame);
	void SetAnimationTickData(float fElapse = -1.0f, int iCurrentFrame = -1, int iCurrentTickFrame = -1);
	int GetCurrentAnimationTick(SceneData* SceneData, int& UpdateTargetTick);
	//================================================================================
	//	** 기본 함수 ** 
	//================================================================================
	bool Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void SetMatrix(D3DXMATRIX* pWorldMatrix, D3DXMATRIX* pViewMatrix, D3DXMATRIX* pProjectionMatrix);
	void SetCurrentMotion(int iAniIndex);
	HRESULT DeleteResource();
	HRESULT CreateResource();
	bool Frame();
	bool Render();
	bool Release();
	bool PostRender();
	bool RenderShadow();
public:
	LALoadObject()
	{
		//m_iCurrentTick = 0;
		m_bBipedRender = false;
		m_bSkinRender = true;
		m_bAnimationPlay = false;
		m_bTestSectionAnimationPlay = false;
		m_bControlTickSet = false;
		//m_NumAnimation = 0.0;
		//m_fElapseFrame = 0.0f;
		m_iCurFrame = 0;
		m_iCurrentTick = 0;
		m_NewLoadObject = NULL;
		
	}
	virtual ~LALoadObject();
};

