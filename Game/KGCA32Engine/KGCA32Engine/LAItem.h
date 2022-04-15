#pragma once

#include "CoreStandardLink.h"

#include "LATopObject.h"
#include "LAParticleRender.h"

#ifdef TOOL_MODE

#else

#include "ParticleSystemManager.h"

#endif


class BMapParser;
class LAItem : public LATopObject , public std::auto_ptr<LAItem>
{
public:
	LAObjectUIDataInfo* m_ProjectileData;
	LAParticleRender* m_ParticleRender;
public:
	//set
	void SetProjectileData();
	//get
	LAObjectUIDataInfo* GetProjectileData();
	E_InventoryWeaponType GetWeaponType();
	E_InventoryArmorType GetArmorType();
	TVector3 GetParticlePositon();
	float GetAttackSpeed();
	//Func
	bool Delete();
	void CreateParticle(BMapParser* pMapData);

public:
	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	bool Frame();
	bool Render();
	bool Release();
public:
	LAItem();
	virtual~LAItem();
};

