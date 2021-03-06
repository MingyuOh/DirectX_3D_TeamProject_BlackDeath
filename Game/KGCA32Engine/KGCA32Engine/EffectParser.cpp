#include "EffectParser.h"
#include "BEffectTime.h"
#include "FSMState.h"

float DXEffectTime::g_fDustCreateTime = 0.0f;

bool EffectParser::Init()
{
	m_pd3dDevice = g_pd3dDevice;
	m_pImmediateContext = g_pImmediateContext;
	m_pCamera = g_pMainCamera;

	LoadEffect(L"../../data/Object/WalkingDust.txt",		L"WalkingDust");
	LoadEffect(L"../../data/Object/EnemyBlood1.txt",		L"EnemyBlood1");
	LoadEffect(L"../../data/Object/EnemyBlood2.txt",		L"EnemyBlood2");
	LoadEffect(L"../../data/Object/HeroDamaged.txt",		L"HeroDamaged");
	LoadEffect(L"../../data/Object/GrenadeEffect.txt",		L"GrenadeEffect");
	LoadEffect(L"../../data/Object/RifleShot.txt",			L"RifleShotEffect");
	LoadEffect(L"../../data/Object/PistolShot.txt",			L"PistolShotEffect");
	LoadEffect(L"../../data/Object/SnowDrop1.txt",			L"SnowDrop1");
	LoadEffect(L"../../data/Object/SnowDrop2.txt",			L"SnowDrop2");
	LoadEffect(L"../../data/Object/SnowDrop3.txt",			L"SnowDrop3");
	LoadEffect(L"../../data/Object/SnowDrop4.txt",			L"SnowDrop4");
	LoadEffect(L"../../data/Object/CarFire.txt",			L"CarFire");
	LoadEffect(L"../../data/Object/StageBoom.txt",			L"StageBoom");
	LoadEffect(L"../../data/Object/ItemEffect.txt",			L"ItemEffect");
	LoadEffect(L"../../data/Object/HealingEffect.txt",		L"HealingEffect");
	LoadEffect(L"../../data/Object/HealingBackground.txt",	L"HealingBackground");
	LoadEffect(L"../../data/Object/BloodFloor.txt",			L"BloodFloor1");
	LoadEffect(L"../../data/Object/BloodFloor2.txt",		L"BloodFloor2");
	LoadEffect(L"../../data/Object/BloodFloor3.txt",		L"BloodFloor3");
	LoadEffect(L"../../data/Object/HeroElectroDamaged.txt",	L"HeroElectroDamaged");

	return true;
}

bool EffectParser::PreEffectFrame()
{
	if (m_iStageState == INGAME_TEXTURE_STAGE_2)
	{
		if (m_bSnowDrop == true)
		{
			// 传1 颇萍努 积己
			ParticleSystem addParticle = I_ParticleSystem.GetValue(SnowDrop1);
			addParticle.CreateBoundingBox(TVector3(0.0f, 0.0f, 0.0f), 1);
			m_SystemParticle.push_back(addParticle);

			// 传2 颇萍努 积己
			addParticle = I_ParticleSystem.GetValue(SnowDrop2);
			addParticle.CreateBoundingBox(TVector3(0.0f, 0.0f, 0.0f), 1);
			m_SystemParticle.push_back(addParticle);

			// 传3 颇萍努 积己
			addParticle = I_ParticleSystem.GetValue(SnowDrop3);
			addParticle.CreateBoundingBox(TVector3(0.0f, 0.0f, 0.0f), 1);
			m_SystemParticle.push_back(addParticle);

			// 传4 颇萍努 积己
			addParticle = I_ParticleSystem.GetValue(SnowDrop4);
			addParticle.CreateBoundingBox(TVector3(0.0f, 0.0f, 0.0f), 1);
			m_SystemParticle.push_back(addParticle);

			m_bSnowDrop = false;
		}
	}
	return true;
}

bool EffectParser::Frame()
{
	PreEffectFrame();

	for (int iObj = 0; iObj < m_SystemParticle.size(); iObj++)
	{
		// 颇萍努矫胶袍俊 颇萍努捞 粮犁窍瘤臼栏搁 昏力
		if (m_SystemParticle[iObj].m_Particle.size() <= 0)
		{
			m_SystemParticle.erase(m_SystemParticle.begin() + iObj);
			iObj--;
			continue;
		}
		m_SystemParticle[iObj].Frame();
	}

	PostEffectFrame();

	return true;
}

bool EffectParser::PostEffectFrame()
{
	return true;
}

bool EffectParser::Render()
{
	for (int iObj = 0; iObj < m_SystemParticle.size(); iObj++)
		m_SystemParticle[iObj].Render();
	return true;
}

bool EffectParser::Release()
{
	if (m_SystemParticle.size() != 0)
		DeleteVector(m_SystemParticle);

	return true;
}

bool EffectParser::LoadEffect(TCHAR* szFileName, TCHAR* szParticleName)
{
	_tfopen_s(&m_pStream, szFileName, _T("rt"));
	if (m_pStream == NULL)
	{
		return false;
	}
	while (!feof(m_pStream))
	{
		TCHAR* szString[256];
		int iReadBoolValue;
		// 弥措 颇萍努 俺荐
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf_s(m_pBuffer, _T("%d"), &m_iNumParticleSys);

		for (int iEffect = 0; iEffect < m_iNumParticleSys; iEffect++)
		{
			// 颇萍努 积己
			ParticleSystem newParticle;
			
			// 颇萍努 捞抚 历厘
			newParticle.m_szParticleName = szParticleName;

			// 官款爹 冠胶 沥焊
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%f %f %f"), 
				&newParticle.m_BoundingBoxMin.x, 
				&newParticle.m_BoundingBoxMin.y, 
				&newParticle.m_BoundingBoxMin.z);
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%f %f %f"), 
				&newParticle.m_BoundingBoxMax.x, 
				&newParticle.m_BoundingBoxMax.y, 
				&newParticle.m_BoundingBoxMax.z);

			// 捞棋飘 荐
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &newParticle.m_iEffectCount);
			
			// 捞棋飘 鸥涝
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &newParticle.m_EffectType);

			// 捞棋飘 鸥涝捞 盔款悼捞搁 肺抛捞记 鸥涝 佬绢柯促.
			if (newParticle.m_EffectType == EFFECT_TYPE::Circle)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%d"), &newParticle.m_CoordinateType);
			}
			
			// Life Time
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%f"), &newParticle.m_fLifeTime);

			// 青纺 蔼
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d %d"), &newParticle.m_iRow, &newParticle.m_iCol);
			
			// 舅颇 喉坊爹
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &iReadBoolValue);
			newParticle.m_bAlphaBlending = (bool)iReadBoolValue;

			// 舅颇 抛胶泼
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &iReadBoolValue);
			newParticle.m_bAlphaTesting = (bool)iReadBoolValue;

			// 版苞矫埃(鞘夸且 矫 佬澜)
			_fgetts(m_pBuffer, 256, m_pStream);

			// 呼焊靛青纺
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &iReadBoolValue);
			newParticle.m_bBillboard = (bool)iReadBoolValue;

			// 呼焊靛青纺 利侩 救登菌阑 锭 滚咆胶 鸥涝
			if (newParticle.m_bBillboard == false)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%d"), &newParticle.m_Effect.m_iVertexType);
			}
			
			// 捞棋飘 馆汗 咯何
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &iReadBoolValue);
			newParticle.m_bRepeatEffect = (bool)iReadBoolValue;

			// 盔辟捧康
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &iReadBoolValue);
			newParticle.m_bPerspective = (bool)iReadBoolValue;

			// 流背捧康
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &iReadBoolValue);
			newParticle.m_bOrthogonal = (bool)iReadBoolValue;

			// 咆胶贸 荐
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &newParticle.m_Effect.m_iTextureCount);
			
			// 咆胶贸 捞抚 历厘
			for (int iTex = 0; iTex < newParticle.m_Effect.m_iTextureCount; iTex++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s"), m_pString);
				newParticle.m_Effect.m_szTextureName.push_back(m_pString);
			}
			
			// 嘉捞歹 历厘 棺 捞棋飘 积己
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s"), m_pString);
			newParticle.m_Effect.Init();
			newParticle.m_Effect.Create(m_pd3dDevice.Get(), m_pString);

			// 嘉捞歹 府家胶轰 积己
			newParticle.m_Effect.TextureLoad(m_pd3dDevice.Get(), newParticle.m_Effect.m_szTextureName);

			// 颇萍努矫胶袍 积己
			newParticle.Set(m_pd3dDevice.Get(), m_pImmediateContext.Get(), m_pCamera);
			newParticle.Create();
			newParticle.Init();

			// 颇萍努
			for (int iObj = 0; iObj < newParticle.m_iEffectCount; iObj++)
			{
				// 捞棋飘 困摹
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%f %f %f"), 
					&newParticle.m_Particle[iObj].m_vPos.x,
					&newParticle.m_Particle[iObj].m_vPos.y,
					&newParticle.m_Particle[iObj].m_vPos.z);

				// 馆汗捞棋飘捞搁 贸澜困摹 历厘
				if (newParticle.m_bRepeatEffect == true)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf_s(m_pBuffer, _T("%f %f %f"),
						&newParticle.m_Particle[iObj].m_vFirstPos.x,
						&newParticle.m_Particle[iObj].m_vFirstPos.y,
						&newParticle.m_Particle[iObj].m_vFirstPos.z);
				}

				// 捞棋飘 拿矾
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%f %f %f %f"),
					&newParticle.m_Particle[iObj].m_vColor.x,
					&newParticle.m_Particle[iObj].m_vColor.y,
					&newParticle.m_Particle[iObj].m_vColor.z,
					&newParticle.m_Particle[iObj].m_vColor.w);

				// 规氢氦磐
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%f %f %f"),
					&newParticle.m_Particle[iObj].m_vDirection.x,
					&newParticle.m_Particle[iObj].m_vDirection.y,
					&newParticle.m_Particle[iObj].m_vDirection.z);

				// 捞棋飘 农扁
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%f %f %f"),
					&newParticle.m_Particle[iObj].m_vScale.x,
					&newParticle.m_Particle[iObj].m_vScale.y,
					&newParticle.m_Particle[iObj].m_vScale.z);
				

				// 捞棋飘 加档
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%f %f %f"),
					&newParticle.m_Particle[iObj].m_vSpeed.x,
					&newParticle.m_Particle[iObj].m_vSpeed.y,
					&newParticle.m_Particle[iObj].m_vSpeed.z);

				// 馆瘤抚
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%f"), &newParticle.m_Particle[iObj].m_fRadian);
			}
			// 颇萍努 矫胶袍 概聪历俊 眠啊
			INT iIndex = I_ParticleSystem.Add(&newParticle);
			if (iIndex > m_iIndexParticleSys)
				m_iIndexParticleSys++;
		}
		_fgetts(m_pBuffer, 256, m_pStream);
	}
	fclose(m_pStream);

	return true;
}

//--------------------------------------------------------------------------------------
// 某腐磐 器瘤记 历厘 窃荐
//--------------------------------------------------------------------------------------
void EffectParser::SetHeroPos(TVector3 vHeroPos)
{
	m_vHeroPos = vHeroPos;
}

//--------------------------------------------------------------------------------------
// 某腐磐 叼泛记 历厘 窃荐
//--------------------------------------------------------------------------------------
void EffectParser::SetHeroDir(TVector3 vHeroDir)
{
	m_vHeroDir = vHeroDir;
}

//--------------------------------------------------------------------------------------
// 胶抛捞瘤 惑怕 历厘 窃荐
//--------------------------------------------------------------------------------------
void EffectParser::SetStageState(int iStageState)
{
	m_iStageState = iStageState;
}

EffectParser::EffectParser()
{
	m_pStream			= NULL;
	m_iIndexParticleSys = -1;
	m_iNumParticleSys	= 0;
	m_vHeroPos			= TVector3(0.0f, 0.0f, 0.0f);
	m_vHeroDir			= TVector3(0.0f, 0.0f, 0.0f);
	m_bSnowDrop			= true;
	m_iStageState		= 0;
}


EffectParser::~EffectParser()
{
	if (m_SystemParticle.size() != 0)
	{
		for (int iDel = 0; iDel < m_SystemParticle.size(); iDel++)
		{
			m_SystemParticle[iDel].Release();
		}
		DeleteVector(m_SystemParticle);
	}
}
