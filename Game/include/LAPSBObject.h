#pragma once

#include "LACharacterResource.h"
#include "LAItemResource.h"

class LAPSBObject
{
private:
	//공유 리소스
	LACharacterResource* m_pCharacterResource;
	std::map<std::wstring, LAItemResource*> m_pITemResourceList;

	//공용데이터
	int m_iMoveSpeed;
	E_LoadObjectType m_ObjectType;

	//D3DXVECTOR3 m_vPosition;
	std::vector<MotionKeyInfo> m_AnimationKeyStateList;		//애니메이션 키값모션값 저장 리스트
	std::vector<MotionKeyInfo> m_MoveKeyStateList;			//캐릭터 이동관련 키값


															//캐릭터관련데이터
	CBConstBoneWorld m_cbMatrixListData;

	//Motion
	int m_iMotionIdle;
	int m_iCurrentMotion;

	//Render
	bool m_bBipedRender;
	bool m_bSkinRender;
	bool m_MotionPlay;
public:
	D3DXMATRIX m_matCharacterWorld;
	D3DXVECTOR3 m_vObjectLook;
	D3DXVECTOR3 m_vObjectUp;
	D3DXVECTOR3 m_vObjectSide;
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;
	//Animation
	int m_iMainCurrentTick;
	float m_fElapseFrame;
	int m_iCurFrame;
	float Theta;
public:
	void SetMotionIdle(int isetMotion);
	void SetCurrentMotion(int isetMotion);
	void SetType(E_LoadObjectType eSetType);
	bool SetCharacterResource(LACharacterResource* pData);
	bool AddITemResource(LAItemResource* pData);
	void SetCBBoneData();
	void SetCurrentTick();
	void SetPosition(float fx, float fy, float fz);
	void SetHeight(float fHeight);
	void SetKeyState(DWORD dwKeyValue, int imotionIndex);
	void SetMoveSpeed(int idata);
	void SetDirection(D3DXVECTOR3* vData);
	void SetRotation(float x, float y, float z);
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetDirection();
	void MoveLook(float fValue);
	void MoveSide(float fValue);
	void UpdateMotion(int& iCurrentMotion, LACharacterResource& rResourceMatrix, int& iOutputTick);
	void CkeckKeyState();
	void UpdateVector();
public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool Create(CrateObjectDest& pMatrixDest, CrateObjectDest& pSkinDest);
	void SetMatrix(D3DXMATRIX* pWorldMatrix, D3DXMATRIX* pViewMatrix, D3DXMATRIX* pProjectionMatrix);
	bool Frame();
	bool Render();
	bool Release();
	bool PostRender();
public:
	LAPSBObject();
	~LAPSBObject();
};

