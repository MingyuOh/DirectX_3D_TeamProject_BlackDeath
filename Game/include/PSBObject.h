#pragma once

#include "CharacterResource.h"
#include "ItemResource.h"

class PSBObject
{
private:
	E_LoadObjectType m_ObjectType;
	CharacterResource* m_pCharacterResource;
	CBConstBoneWorld m_cbMatrixListData;
	std::map<std::wstring, ItemResource*> m_pITemResourceList;
	D3DXMATRIX m_matCharacterWorld;
	//Motion
	int m_iMotionIdle;
	int m_iCurrentMotion;
	
	//Render
	bool m_bBipedRender;
	bool m_bSkinRender;
public:
	//Animation
	int m_iMainCurrentTick;
	float m_fElapseFrame;
	int m_iCurFrame;
public:
	void SetMotionIdle(int isetMotion);
	void SetCurrentMotion(int isetMotion);
	void SetType(E_LoadObjectType eSetType);
	bool SetCharacterResource(CharacterResource* pData);
	bool AddITemResource(ItemResource* pData);
	void SetCBBoneData();
	void SetCurrentTick();
	void SetPosition(float fx, float fy, float fz);
	void SetKeyState(DWORD dwKeyValue, int imotionIndex);

	void UpdateMotion(int iCurrentMotion, CharacterResource& rResourceMatrix, int& iOutputTick);
	void CkeckKeyState();
public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool Create(CrateObjectDest& pMatrixDest, CrateObjectDest& pSkinDest);
	void SetMatrix(D3DXMATRIX* pWorldMatrix, D3DXMATRIX* pViewMatrix, D3DXMATRIX* pProjectionMatrix);
	bool Frame();
	bool Render();
	bool Release();
	bool PostRender();
public:
	PSBObject();
	~PSBObject();
};

