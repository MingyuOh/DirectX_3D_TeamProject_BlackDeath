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
	// �ν��Ͻ�ȭ ���� ����
	//--------------------------------------------------------------------------------------
	bool			CreateInstanceBuffer();
	//--------------------------------------------------------------------------------------
	// ���̾ƿ� �� ���̴� ����
	//--------------------------------------------------------------------------------------
	bool			LoadShaderAndInputLayout(TCHAR* vertexShaderName = NULL,
		TCHAR* PixelShaderName = NULL, TCHAR* geometryShaderName = NULL);
	//--------------------------------------------------------------------------------------
	// ����̽� / ī�޶� �ʱ�ȭ �Լ�
	//--------------------------------------------------------------------------------------
	bool			Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext, BCamera* pMainCamera);
	//--------------------------------------------------------------------------------------
	// ��ƼŬ ������Ʈ �Լ�
	//--------------------------------------------------------------------------------------
	void			ParticleUpdate();
	//--------------------------------------------------------------------------------------
	// Create
	//--------------------------------------------------------------------------------------
	bool			Create();
	//--------------------------------------------------------------------------------------
	// ��� �Լ�
	//--------------------------------------------------------------------------------------
	TVector3		Circle(float fRadius, float fRadian);
	//--------------------------------------------------------------------------------------
	// �ٿ���ڽ� ����
	//--------------------------------------------------------------------------------------
	void			CreateBoundingBox(TVector3 ObjectPos = TVector3(0.0f, 0.0f, 0.0f), int iType = 0);
	//--------------------------------------------------------------------------------------
	// ��ƼŬ �ý��� ������ �� �Լ�
	//--------------------------------------------------------------------------------------
	void			SetFrameParticleSystemInfo(TVector3* vPos = NULL, TVector3* vDir = NULL);
	//--------------------------------------------------------------------------------------
	// ����Ʈ �ٿ���ڽ� ���� ���� �Լ�
	//--------------------------------------------------------------------------------------
	bool			ClassifyPoint(TVector3* pv);
	//--------------------------------------------------------------------------------------
	// ����Ʈ �ٿ���ڽ� ���� ���� ���� �� ���� �Լ�
	//--------------------------------------------------------------------------------------
	bool			ClassifyPointInInterpolate(TVector3* pv);
	//--------------------------------------------------------------------------------------
	// ����Ʈ �ð� üũ �Լ�
	//--------------------------------------------------------------------------------------
	void			CheckEffectRangeFromBox();
	//--------------------------------------------------------------------------------------
	// ����Ʈ �ڽ� ���� �Ѿ�� ���� �Լ�
	//--------------------------------------------------------------------------------------
	void			EffectLifeTimeCheck();
	//--------------------------------------------------------------------------------------
	// �ý��� �Լ�
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