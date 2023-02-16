#pragma once
#include "Sprite.h"
#include "Particle.h"
#include "BCamera.h"

#define MAX_PARTICLE_COUNT 1000
#define EFFECT_EXTINCTION_DISTANCE 10.0f 

enum COORDINATE_TYPE
{
	RotationX = 0,
	RotationY = 1,
	RotationZ = 2
};

struct Instance
{
	TMatrix	 matWorld;
	TVector4  m_vColor;
};

class ParticleSystem
{
public:
	Sprite							m_Effect;						// Real Effect(this object have plane)
	vector<Particle>				m_Particle;						// Particle Elements Infomation
	BCamera*						m_pCamera;						// Camera
	TMatrix						m_matWorld;						// World Matrix
	ComPtr<ID3D11BlendState>		m_pAlphaBlend;					// Alpha Blend Device
	ComPtr<ID3D11BlendState>		m_pAlphaTesting;				// Alpha Testing Device
	BOOL							m_bAlphaBlending;				// Alpha Blend apply to Effect
	BOOL							m_bAlphaTesting;				// Alpha Testing apply to Effect

	Shape							m_BoundingBox;					// BoundingBox
	TVector3						m_BoundingBoxMax;				// BoundingBox Max Value
	TVector3						m_BoundingBoxMin;				// BoundingBox Min Value
	BPlane							m_Plane[6];						// BoundingBox Plane

	ComPtr<ID3D11Buffer>			m_pInstanceBuffer;				// Instancing Buffer
	ComPtr<ID3D11InputLayout>		m_pLayout;						// Instancing Layout
	vector<Instance>				m_InstDataArray;				// Instancing Data

	ComPtr<ID3D11Device>			m_pd3dDevice;					// Main Device
	ComPtr<ID3D11DeviceContext>	    m_pImmediateContext;			// Context Device

	int								m_iEffectCount;					// Effect Count
	EFFECT_TYPE						m_EffectType;					// Effect Type
	COORDINATE_TYPE					m_CoordinateType;				// If Effect type is circle exercise, this type determine how to rotate circle effect
	float							m_fLifeTime;					// LifeTime
	float							m_fEffectElapseTime;			// After Effect create, ElapledTime
	int								m_iRow;							// Matrix(Row) - Use Sprites Texture
	int								m_iCol;							// Matrix(Col) - Use Sprites Texture
	TCHAR*							m_szParticleName;				// ParticleName	(Using ParticleSystemManager)
	int								m_iIndex;						// Particle System Manager Index
	bool							m_bBillboard;					// Billboard Matrix apply to Effect
	bool							m_bRepeatEffect;				// Variables that distinguish repetitive effects
	bool							m_bPerspective;					// Perspective Projection apply to Effect
	bool							m_bOrthogonal;					// Orthogonal Projection Matrix apply to Effect
	bool							m_bRepetEffectSet;				// When repeat effect call SetFrameParticleSystemInfo Function, Particle first position set one
	bool							m_bRotationEffect;				// Checking rotation effect
public:
	//--------------------------------------------------------------------------------------
	// 인스턴스화 버퍼 생성
	//--------------------------------------------------------------------------------------
	bool			CreateInstanceBuffer();
	//--------------------------------------------------------------------------------------
	// 레이아웃 및 쉐이더 생성
	//--------------------------------------------------------------------------------------
	bool			LoadShaderAndInputLayout(TCHAR* vertexShaderName = NULL,
		TCHAR* PixelShaderName = NULL, TCHAR* geometryShaderName = NULL);
	//--------------------------------------------------------------------------------------
	// 디바이스 / 카메라 초기화 함수
	//--------------------------------------------------------------------------------------
	bool			Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext, BCamera* pMainCamera);
	//--------------------------------------------------------------------------------------
	// 파티클 업데이트 함수
	//--------------------------------------------------------------------------------------
	void			ParticleUpdate();
	//--------------------------------------------------------------------------------------
	// Create
	//--------------------------------------------------------------------------------------
	bool			Create();
	//--------------------------------------------------------------------------------------
	// 원운동 함수
	//--------------------------------------------------------------------------------------
	TVector3		Circle(float fRadius, float fRadian);
	//--------------------------------------------------------------------------------------
	// 바운딩박스 생성
	//--------------------------------------------------------------------------------------
	void			CreateBoundingBox(TVector3 ObjectPos = TVector3(0.0f, 0.0f, 0.0f), int iType = 0);
	//--------------------------------------------------------------------------------------
	// 파티클 시스템 프레임 셋 함수
	//--------------------------------------------------------------------------------------
	void			SetFrameParticleSystemInfo(TVector3* vPos = NULL, TVector3* vDir = NULL);
	//--------------------------------------------------------------------------------------
	// 이펙트 바운딩박스 범위 판정 함수
	//--------------------------------------------------------------------------------------
	bool			ClassifyPoint(TVector3* pv);
	//--------------------------------------------------------------------------------------
	// 이펙트 바운딩박스 범위 내에 있을 시 보간 함수
	//--------------------------------------------------------------------------------------
	bool			ClassifyPointInInterpolate(TVector3* pv);
	//--------------------------------------------------------------------------------------
	// 이펙트 시간 체크 함수
	//--------------------------------------------------------------------------------------
	void			CheckEffectRangeFromBox();
	//--------------------------------------------------------------------------------------
	// 이펙트 박스 범위 넘어가면 삭제 함수
	//--------------------------------------------------------------------------------------
	void			EffectLifeTimeCheck();
	//--------------------------------------------------------------------------------------
	// 시스템 함수
	//--------------------------------------------------------------------------------------
	bool			Init();
	bool			Frame();
	bool			Render();
	bool			Release();
public:
	ParticleSystem();
	~ParticleSystem();
};

TVector3 operator*(TVector3 v1, TVector3 v2);