#pragma once


#include "LACoreLinkStd.h"
#include "CoreStandardLink.h"

#include "LACharacterStruct.h"
#include "LAResource.h"
#include "LASkill.h"

#ifdef TOOL_MODE

#else

#include "LASound2.h"
#include "BBoundingBox.h"
#include "ParticleSystem.h"

#endif

const int MAX_SUB_RESOURCE = 8;

class LATopObject
{
public:
	ObjectTableLoadData m_ObjectData;
	std::wstring m_wsObjectName;
	E_LoadObjectType m_ObjectType;
	int m_MainResourceNumber;
	LAResource* m_pMainResource;
	std::vector<MotionKeyInfo> m_AnimationKeyStateList;		//애니메이션 키값모션값 저장 리스트
	std::vector<MotionKeyInfo> m_MoveKeyStateList;			//캐릭터 이동관련 키값
	CBConstBoneWorld m_cbMatrixListData;
	int m_iNumTile;

	//SkillType
	LASkill* m_pSkillData;

	//CollitionData
	TVector3 m_vAllSkinMin;
	TVector3 m_vAllSkinMax;
	TVector3 m_vUpdateMin;
	TVector3 m_vUpdateMax;
	TVector3 m_vCollitionBox[8];

	//Direction Light
	TVector4 m_vLightPos;
	TVector4 m_vLightDirection;
	TVector4 m_vCameraPos;


	//KeyState
	int m_MovePushStack;
	bool m_bBipedRender;
	bool m_bSkinRender;
	int m_iMotionIdleNumber;
	int m_iBeforeMotion;
	int m_iCurrentMotion;

	//Animation
	float m_iMoveSpeed;
	float m_fAddAnimationSpeed;
	bool m_bSetParentTick;
	int m_iMainCurrentTick;
	float m_fElapseFrame;
	int m_iCurFrame;
	bool m_bisMove;
	bool m_bisMotion;
	bool m_bisJump;
	bool m_bisRun;
	bool m_bIsShot;
	bool m_bIsRifleShot;

	//check
	bool m_bisCheckTick;
	bool m_bIsObjectLife;

	//LASound2 m_Sound;
	std::vector<int> m_SondIndexList;

#ifdef TOOL_MODE

#else

	/*==================== 충돌처리 및 사운드 멤버 추가부분 ======================*/
	BBoundingBox m_BoundingBox;
	bool m_bDrawCollitionLine;
	BLine m_DrawLine;

	//Particle
	vector<ParticleSystem> m_ParticleSystemList;

#endif

public:
	TMatrix m_matObjectWorld;
	TVector3 m_vMoveUpdatePosition;
	TVector3 m_vObjectPosition;
	TVector3 m_vObjectBeforePosition;
	TMatrix m_matObjectScale;
	TMatrix m_matObjectrotation;
	TVector3 m_vObjectLook;
	TVector3 m_vObjectUp;
	TVector3 m_vObjectSide;
	
public:
	//================================================================================
	//	** Check  ** 
	//================================================================================
	bool CheckPositionTick(int iPercentage, bool bisRePlay);
	bool CheckRifleIsShot();
	//================================================================================
	//	** Set  ** 
	//================================================================================
	void SetCBBoneData();
	void SetObjectName(std::wstring wsName);
	void SetMotionIdle(int isetMotion);
	void SetCurrentMotion(int isetMotion);					//유저캐릭터 현제 미사용
	void SetType(E_LoadObjectType eSetType);
	bool SetMainResource(int iResourceindex);
	void SetPosition(float fx, float fy, float fz);
	void SetHeight(float fHeight);
	void SetCurrentTick(bool check);						//현제 미사용
	void SetKeyState(DWORD dwKeyValue, int imotionIndex);	//현제 미사용
	void SetAddAnimationSpeed(float fSpeed);
	//void SetMoveSpeed(float idata);							
	void SetScale(float fbaseHeightSize, float x, float y, float z);
	void SetObjectWorldMatrixToResource(TMatrix* pMatObjectWorld = NULL);
	void SetMinMax();
	void SetObjectData(ObjectTableLoadData& objectdata);
	virtual void SetSound(E_LoadTableType eTableType, int iMode);
	void SetSkillData(SkillTableLoadData* rSkillData);
	void SetTileNumber(int iTileNumber);
	void SetDirectionLight(TVector4 vCameraPos, TVector4 vLigthPos, TVector4 vLightDirection);
	//bool AddITemResource(LAItemResource* pData);
	//================================================================================
	//	** Get  ** 
	//================================================================================
	std::wstring GetObjectName();
	TVector3 GetPosition();
	TVector3 GetDirection();
	void GetInstancingData(InstancingDataInfo* pInstancingData, int iInstancingCount);
	//================================================================================
	//	** Update  ** 
	//================================================================================
	void MoveLook(float fValue);
	void MoveSide(float fValue);
	void UpdateWorldmatrix();
	void UpdateMinMax();
	void UpdatePosition();
	void UpdateMotion();
	void UpdateVector();
	void UpdateVector(TMatrix matRotation);
	void UpdateObjectDataInfo();
	//================================================================================
	//	** Func  ** 
	//================================================================================
	bool DeleteObject();
	virtual void ResetCheck();
public:
	virtual void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual bool Create(CrateObjectDest& pMatrixDest, CrateObjectDest& pSkinDest);
	virtual void SetMatrix(TMatrix* pWorldMatrix, TMatrix* pViewMatrix, TMatrix* pProjectionMatrix);
	virtual bool PrevFrame();
	virtual bool Frame();
	virtual bool PostFrame();
	virtual bool PrevRender();
	virtual bool Render();
	virtual bool PostRender();
	virtual bool RenderShadow();
	virtual bool Release();

public:
	LATopObject()
	{
		m_pMainResource = NULL;
		m_pSkillData = NULL;
		m_ObjectType = E_LoadObjectType::NONE;
		m_MainResourceNumber = -1;
		m_bBipedRender = false;
		m_bSkinRender = true;
		m_iMotionIdleNumber = 0;
		m_iBeforeMotion = 0;
		m_iCurrentMotion = 0;
		m_bSetParentTick = false;
		m_bIsShot = false;
		m_bisCheckTick = false;
		m_bIsObjectLife = true;
		m_bIsRifleShot = false;
		m_iMainCurrentTick = 0;
		m_fElapseFrame = 0.0f;
		m_fAddAnimationSpeed = 1.0f;
		m_iCurFrame = 0;
		m_iMoveSpeed = 1;
		D3DXMatrixIdentity(&m_matObjectWorld);
		D3DXMatrixIdentity(&m_matObjectScale);
		D3DXMatrixIdentity(&m_matObjectrotation);
		m_vObjectLook = TVector3(0.0f, 0.0f, -1.0f);
		m_vObjectUp = TVector3(0.0f, 1.0f, 0.0f);
		m_vObjectSide = TVector3(-1.0f, 0.0f, 0.0f);
		m_MovePushStack = 0;
		ZeroMemory(&m_vAllSkinMin, sizeof(TVector3));
		ZeroMemory(&m_vAllSkinMax, sizeof(TVector3));
		ZeroMemory(&m_vObjectPosition, sizeof(TVector3));
		ZeroMemory(&m_vObjectBeforePosition, sizeof(TVector3));
		ZeroMemory(&m_vMoveUpdatePosition, sizeof(TVector3));
		
	}
	virtual ~LATopObject();
};

