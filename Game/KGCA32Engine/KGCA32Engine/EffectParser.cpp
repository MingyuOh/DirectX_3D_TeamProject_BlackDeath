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
			// ��1 ��ƼŬ ����
			ParticleSystem addParticle = I_ParticleSystem.GetValue(SnowDrop1);
			addParticle.CreateBoundingBox(TVector3(0.0f, 0.0f, 0.0f), 1);
			m_SystemParticle.push_back(addParticle);

			// ��2 ��ƼŬ ����
			addParticle = I_ParticleSystem.GetValue(SnowDrop2);
			addParticle.CreateBoundingBox(TVector3(0.0f, 0.0f, 0.0f), 1);
			m_SystemParticle.push_back(addParticle);

			// ��3 ��ƼŬ ����
			addParticle = I_ParticleSystem.GetValue(SnowDrop3);
			addParticle.CreateBoundingBox(TVector3(0.0f, 0.0f, 0.0f), 1);
			m_SystemParticle.push_back(addParticle);

			// ��4 ��ƼŬ ����
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
		// ��ƼŬ�ý��ۿ� ��ƼŬ�� �������������� ����
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
		// �ִ� ��ƼŬ ����
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf_s(m_pBuffer, _T("%d"), &m_iNumParticleSys);

		for (int iEffect = 0; iEffect < m_iNumParticleSys; iEffect++)
		{
			// ��ƼŬ ����
			ParticleSystem newParticle;
			
			// ��ƼŬ �̸� ����
			newParticle.m_szParticleName = szParticleName;

			// �ٿ�� �ڽ� ����
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

			// ����Ʈ ��
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &newParticle.m_iEffectCount);
			
			// ����Ʈ Ÿ��
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &newParticle.m_EffectType);

			// ����Ʈ Ÿ���� ����̸� �����̼� Ÿ�� �о�´�.
			if (newParticle.m_EffectType == EFFECT_TYPE::Circle)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%d"), &newParticle.m_CoordinateType);
			}
			
			// Life Time
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%f"), &newParticle.m_fLifeTime);

			// ��� ��
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d %d"), &newParticle.m_iRow, &newParticle.m_iCol);
			
			// ���� ����
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &iReadBoolValue);
			newParticle.m_bAlphaBlending = (bool)iReadBoolValue;

			// ���� �׽���
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &iReadBoolValue);
			newParticle.m_bAlphaTesting = (bool)iReadBoolValue;

			// ����ð�(�ʿ��� �� ����)
			_fgetts(m_pBuffer, 256, m_pStream);

			// ���������
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &iReadBoolValue);
			newParticle.m_bBillboard = (bool)iReadBoolValue;

			// ��������� ���� �ȵǾ��� �� ���ؽ� Ÿ��
			if (newParticle.m_bBillboard == false)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%d"), &newParticle.m_Effect.m_iVertexType);
			}
			
			// ����Ʈ �ݺ� ����
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &iReadBoolValue);
			newParticle.m_bRepeatEffect = (bool)iReadBoolValue;

			// ��������
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &iReadBoolValue);
			newParticle.m_bPerspective = (bool)iReadBoolValue;

			// ��������
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &iReadBoolValue);
			newParticle.m_bOrthogonal = (bool)iReadBoolValue;

			// �ؽ�ó ��
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf_s(m_pBuffer, _T("%d"), &newParticle.m_Effect.m_iTextureCount);
			
			// �ؽ�ó �̸� ����
			for (int iTex = 0; iTex < newParticle.m_Effect.m_iTextureCount; iTex++)
			{
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf(m_pBuffer, _T("%s"), m_pString);
				newParticle.m_Effect.m_szTextureName.push_back(m_pString);
			}
			
			// ���̴� ���� �� ����Ʈ ����
			_fgetts(m_pBuffer, 256, m_pStream);
			_stscanf(m_pBuffer, _T("%s"), m_pString);
			newParticle.m_Effect.Init();
			newParticle.m_Effect.Create(m_pd3dDevice.Get(), m_pString);

			// ���̴� ���ҽ��� ����
			newParticle.m_Effect.TextureLoad(m_pd3dDevice.Get(), newParticle.m_Effect.m_szTextureName);

			// ��ƼŬ�ý��� ����
			newParticle.Set(m_pd3dDevice.Get(), m_pImmediateContext.Get(), m_pCamera);
			newParticle.Create();
			newParticle.Init();

			// ��ƼŬ
			for (int iObj = 0; iObj < newParticle.m_iEffectCount; iObj++)
			{
				// ����Ʈ ��ġ
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%f %f %f"), 
					&newParticle.m_Particle[iObj].m_vPos.x,
					&newParticle.m_Particle[iObj].m_vPos.y,
					&newParticle.m_Particle[iObj].m_vPos.z);

				// �ݺ�����Ʈ�̸� ó����ġ ����
				if (newParticle.m_bRepeatEffect == true)
				{
					_fgetts(m_pBuffer, 256, m_pStream);
					_stscanf_s(m_pBuffer, _T("%f %f %f"),
						&newParticle.m_Particle[iObj].m_vFirstPos.x,
						&newParticle.m_Particle[iObj].m_vFirstPos.y,
						&newParticle.m_Particle[iObj].m_vFirstPos.z);
				}

				// ����Ʈ �÷�
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%f %f %f %f"),
					&newParticle.m_Particle[iObj].m_vColor.x,
					&newParticle.m_Particle[iObj].m_vColor.y,
					&newParticle.m_Particle[iObj].m_vColor.z,
					&newParticle.m_Particle[iObj].m_vColor.w);

				// ���⺤��
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%f %f %f"),
					&newParticle.m_Particle[iObj].m_vDirection.x,
					&newParticle.m_Particle[iObj].m_vDirection.y,
					&newParticle.m_Particle[iObj].m_vDirection.z);

				// ����Ʈ ũ��
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%f %f %f"),
					&newParticle.m_Particle[iObj].m_vScale.x,
					&newParticle.m_Particle[iObj].m_vScale.y,
					&newParticle.m_Particle[iObj].m_vScale.z);
				

				// ����Ʈ �ӵ�
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%f %f %f"),
					&newParticle.m_Particle[iObj].m_vSpeed.x,
					&newParticle.m_Particle[iObj].m_vSpeed.y,
					&newParticle.m_Particle[iObj].m_vSpeed.z);

				// ������
				_fgetts(m_pBuffer, 256, m_pStream);
				_stscanf_s(m_pBuffer, _T("%f"), &newParticle.m_Particle[iObj].m_fRadian);
			}
			// ��ƼŬ �ý��� �Ŵ����� �߰�
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
// ĳ���� ������ ���� �Լ�
//--------------------------------------------------------------------------------------
void EffectParser::SetHeroPos(TVector3 vHeroPos)
{
	m_vHeroPos = vHeroPos;
}

//--------------------------------------------------------------------------------------
// ĳ���� �𷺼� ���� �Լ�
//--------------------------------------------------------------------------------------
void EffectParser::SetHeroDir(TVector3 vHeroDir)
{
	m_vHeroDir = vHeroDir;
}

//--------------------------------------------------------------------------------------
// �������� ���� ���� �Լ�
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
