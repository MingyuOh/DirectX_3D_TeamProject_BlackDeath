
//#include "stdafx.h"

#include "LAManagerOfManager.h"
#include "LACoreLinkStd.h"

#ifdef TOOL_MODE

#else
#include "LASound2.h"
#endif


void LAManagerOfManager::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BMapParser* pMap)
{
	LOAD_OBJECT_MGR.Init(pDevice, pContext, pMap);
	RESOURCE_MGR.Init(pDevice, pContext);

#ifdef TOOL_MODE

#else
	LA_SOUND2_MGR.Init();
#endif
}

bool LAManagerOfManager::Frame()
{
	return true;
}

bool LAManagerOfManager::Render()
{
	return true;
}

bool LAManagerOfManager::Release()
{
	LOAD_OBJECT_MGR.Release();
	RESOURCE_MGR.Release();

#ifdef TOOL_MODE

#else
	LA_SOUND2_MGR.Release();
#endif

	return true;
}

LAManagerOfManager::LAManagerOfManager()
{
}


LAManagerOfManager::~LAManagerOfManager()
{
}
