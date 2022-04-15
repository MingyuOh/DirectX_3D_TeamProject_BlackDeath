#pragma once
#include"BTimer.h"
#include"BInput.h"
#include"BWrite.h"
#include"BBackViewCamera.h"
#include"BFPSCamera.h"
#include"BDirectionLine.h"
#include"BPlane.h"
#include"BSphere.h"
#include"BSkyObj.h"
#include"BSound.h"


/*맵 전용 include*/
#include "BMapLOD.h"
#include "BMapTileRender.h"
#include "BSelect.h"
#include "MapExporter.h"
#include "MapImporter.h"
#include "BWaterMap.h"
#include "BRenderPlane.h"
#include "BMapSetting.h"
#include "BAseObj.h"
#include "BParser.h"

extern BFPSCamera* g_pMainCamera;

class BCore : public BWindowLib
{
public:
	TMatrix m_matWorld;  // 로컬좌표 (백터값) * 월드 행렬 = 월드좌표
	TMatrix m_matView;	// 월드좌표 * 뷰행렬 = 뷰좌표 (카메라의 이동에 따른 좌표)
	TMatrix m_matProj;	// 뷰좌표 * 투영행렬 = 투영좌표 (카메라의 투영에 따른 좌표)

	//ID3D11RasterizerState* m_pRS; // 렌더링 형태 ( 와이어 & 필 )
	ComPtr<ID3D11RasterizerState> m_pRS[10];
	bool m_bWireFrameRender;	  // 렌더링 상태 온오프용 변수

	BTimer m_Timer;
	BWrite m_Writer;
	BSkyObj m_SkyBox;
	BFPSCamera* m_pMainCamera;
	BFPSCamera* m_pFPSCamera;
	BBackViewCamera* m_pDebugCamera;
	BCamera* m_pTreeCamera;
	BBackViewCamera* m_pBackViewCamera;
	
	BDirectionLine m_DirectionLine;
	bool m_DirectionLineOn;

	TVector4 vDirValue; /*카메라 회전, 이동 변수*/

	/*샘플러 변수*/
	ID3D11SamplerState* m_pSamplerState;
	D3D11_FILTER m_DXFilter;
	UINT m_iFilterCnt;
	D3D11_TEXTURE_ADDRESS_MODE m_TexAddressMode;
	UINT m_iTexAddressMode;

	D3D11_SHADER_RESOURCE_VIEW_DESC m_DescSRV;
	int m_iMinLod;

public:
	virtual bool		Init();
	virtual	bool		Frame();
	virtual	bool		Render();
	virtual bool		Release();
	virtual	bool		Run();

	virtual bool PreInit();
	virtual bool PreRender();
	virtual bool PreFrame();

	virtual bool PostInit();
	virtual bool PostRender();
	virtual bool PostFrame();

	bool BInit();
	bool BRender();
	bool BFrame();
	bool BRelease();

	bool CreateDxResource();
	bool DeleteDxResource();

public:
	BCore();
	virtual ~BCore();
};

