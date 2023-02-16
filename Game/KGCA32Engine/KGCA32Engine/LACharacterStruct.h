#pragma once

#include "LACoreLinkStd.h"
#include "CoreStandardLink.h"

#ifdef TOOL_MODE

	#include "LAObjectStruct.h"

	struct B_BOX
	{
		//����
		TVector3 vCenter;
		TVector3 vPos[8];

		// AABB
		TVector3 vMax;
		TVector3 vMin;

		//OBB
		TVector3 vAxis[3];
		float fExtent[3];
	};

	//--------------------------------------------------------------------------------------
	// Character_UI_Struct
	//--------------------------------------------------------------------------------------
	struct LAObjectUIDataInfo
	{
		int iCurrentCount;
		int iInventoryTotalCount;
		int iMaxCount;
		std::wstring wsiConFileName;


		//�߰� 
		//�⺻ü��/�����ִ�ü��
		float fHeroHp;
		float fHeroTotalHp;
		//�⺻����/�����ִ����
		float fHeroDef;
		float fHeroTotalDef;
		//�⺻���ǵ�/�����ִ뽺�ǵ�
		float fHeroSpeed;
		float fHeroTotalSpeed;

		LAObjectUIDataInfo()
		{
			iCurrentCount = 0;
			iInventoryTotalCount = 0;
			iMaxCount = 0;

			fHeroHp = 0.0f;
			fHeroTotalHp = 0.0f;
			fHeroDef = 0.0f;
			fHeroTotalDef = 0.0f;
			fHeroSpeed = 0.0f;
			fHeroTotalSpeed = 0.0f;
		}
	};

	enum class E_LACharacterUiDataType
	{
		RIPLE_UI_DATA,
		HANDGUN_UI_DATA,
		KNIFE_UI_DATA,
		BOMB_UI_DATA,
		MEDIC_UI_DATA,
		CHARACTER_UI_DATA,
	};

	struct LACharacterUIData
	{
		LAObjectUIDataInfo* CharacterUIData[6];

		LACharacterUIData()
		{
			for (int iCnt = 0; iCnt < 6; iCnt++)
			{
				CharacterUIData[iCnt] = NULL;
			}
		}
	};

	//--------------------------------------------------------------------------------------
	// Character_INventory_Struct
	//--------------------------------------------------------------------------------------
	enum class E_InventoryWeaponType
	{
		WEAPON_AK47,
		WEAPON_K2,
		WEAPON_H416,
		WEAPON_HANDGUN,
		WEAPON_KNIFE,
		WEAPON_FRAG_GRENADE,
		WEAPON_PORTABLE_GRENADE,
		WEAPON_MEDIC,
	};

	enum class E_InventoryArmorType
	{
		ARMOR_HEAD,
		ARMOR_BODY,
		ARMOR_HAND,
		ARMOR_FOOT,
		ARMOR_ACE,
	};


	class LAItem;
	struct StructInventory
	{
		DWORD dwTotalCount_AK47;
		DWORD dwTotalCount_K2;
		DWORD dwTotalCount_HK416;
		DWORD dwTotalCount_FRAG_GRENADE;
		DWORD dwTotalCount_PORTABLE_GRENADE;
		DWORD dwTotalCount_HANDGUN;
		DWORD dwTotalCount_MEDICBOX;
		std::vector<std::shared_ptr<LAItem>> m_pInvenWeaponList;
		std::vector<std::shared_ptr<LAItem>> m_pInvenArmorList;

		StructInventory()
		{
			dwTotalCount_AK47 = 0;
			dwTotalCount_K2 = 0;
			dwTotalCount_HK416 = 0;
			dwTotalCount_FRAG_GRENADE = 0;
			dwTotalCount_PORTABLE_GRENADE = 0;
			dwTotalCount_HANDGUN = 0;
			dwTotalCount_MEDICBOX = 0;
		}
	};

#else


struct SPACE_PLANE
{
	float fA;
	float fB;
	float fC;
	float fD;

	SPACE_PLANE()
	{
		fA = 0.0f;
		fB = 0.0f;
		fC = 0.0f;
		fD = 0.0f;
	}
};

struct SPACE_SPHERE
{
	TVector3 vMin;
	TVector3 vMax;

	TVector3 vCenter;
	TVector3 vAxis[3];
	float fExtent[3];
	float fRadius;

	SPACE_SPHERE()
	{
		ZeroMemory(&vMin, sizeof(TVector3));
		ZeroMemory(&vMax, sizeof(TVector3));
		ZeroMemory(&vCenter, sizeof(TVector3));
		ZeroMemory(&vAxis, sizeof(TVector3) * 3);
		ZeroMemory(&fExtent, sizeof(float) * 3);
		fRadius = 0.0f;
	}
};

struct SPACE_BOX
{
	//aabb
	TVector3 vMin;
	TVector3 vMax;
	//obb
	TVector3 vCenter;
	TVector3 vPos[8];
	TVector3 vAxis[3];
	TVector3 vbaseAxis[3];
	float fExtent[3];
	float fRadius;

	SPACE_BOX()
	{
		ZeroMemory(&vMin, sizeof(TVector3));
		ZeroMemory(&vMax, sizeof(TVector3));
		ZeroMemory(&vCenter, sizeof(TVector3));
		ZeroMemory(&vPos, sizeof(TVector3) * 8);
		ZeroMemory(&vAxis, sizeof(TVector3) * 3);
		ZeroMemory(&vbaseAxis, sizeof(TVector3) * 3);
		ZeroMemory(&fExtent, sizeof(float) * 3);
		fRadius = 0.0f;
	}
};

struct PNCT_VECTOR_LINE
{
	TVector4 Vertex;
	TVector4 Color;
	PNCT_VECTOR_LINE() {}
	PNCT_VECTOR_LINE(TVector4 Vertex0,
		TVector4 Color0)
	{
		Vertex = Vertex0;
		Color = Color0;
	}
};

struct PNCT_VECTOR
{
	TVector4 Vertex;
	TVector4 Nomal;
	TVector4 Color;
	TVector2 Texture;

	PNCT_VECTOR()
	{
		ZeroMemory(&Vertex, sizeof(TVector4));
		ZeroMemory(&Nomal, sizeof(TVector4));
		ZeroMemory(&Color, sizeof(TVector4));
		ZeroMemory(&Texture, sizeof(TVector2));
	}
	PNCT_VECTOR(TVector4 dxVertex,
		TVector4 dxNomal,
		TVector4 dxColor,
		TVector2 dxTexture)
	{
		Vertex = dxVertex;
		Nomal = dxNomal;
		Color = dxColor;
		Texture = dxTexture;
	}
public:
	bool operator ==(PNCT_VECTOR& source)
	{
		if ((Vertex == source.Vertex) &
			(Nomal == source.Nomal) &
			(Color == source.Color) &
			(Texture == source.Texture))
		{
			return true;
		}

		return false;
	}

};

struct PNCT5_VERTEX
{
	TVector4 Vertex;
	TVector4 Nomal;
	TVector4 Color;
	TVector2 Texture;
	float ObjNum1[4];
	float Weight1[4];
	float ObjNum2[4];
	float Weight2[4];
	TVector4 vTangent;

	PNCT5_VERTEX()
	{
		ZeroMemory(&Vertex, sizeof(TVector4));
		ZeroMemory(&Nomal, sizeof(TVector4));
		ZeroMemory(&Color, sizeof(TVector4));
		ZeroMemory(&Texture, sizeof(TVector2));
		ZeroMemory(&ObjNum1, sizeof(float) * 4);
		ZeroMemory(&Weight1, sizeof(float) * 4);
		ZeroMemory(&ObjNum2, sizeof(float) * 4);
		ZeroMemory(&Weight2, sizeof(float) * 4);
		ZeroMemory(&vTangent, sizeof(TVector4));
	}
	PNCT5_VERTEX(TVector4 dxVertex,
		TVector4 dxNomal,
		TVector4 dxColor,
		TVector2 dxTexture,
		TVector4 dxTangent)
	{
		Vertex = dxVertex;
		Nomal = dxNomal;
		Color = dxColor;
		Texture = dxTexture;
		vTangent = dxTangent;
	}
public:
	bool operator == (PNCT5_VERTEX& source)
	{
		/*if ((Vertex == source.Vertex) &
		(Nomal == source.Nomal) &
		(Color == source.Color) &
		(Texture == source.Texture) &
		(vTangent == source.vTangent)
		)*/
		if ((Vertex == source.Vertex) &
			(Texture == source.Texture))
		{
			return true;
		}

		return false;
	}
};

struct VERTEX_CB
{
	TMatrix g_matWorld;		//c0
	TMatrix g_matView;		//c4
	TMatrix g_matProj;		//c8
	TMatrix g_matWorldInverse;		//c16
};

struct DataCB
{
	//TMatrix matNormal;
	TVector4 vLightPos;
	TVector4 vLIghtDirection;
	TVector4 vEyePosition;
	TVector4 vData01;
	TVector4 vMeshColor;
	TMatrix matWorldInverse;
};

struct CrateObjectDest
{
	TCHAR* vertexShaderName;
	char* vertexFuntionNmae;
	TCHAR* PixelShaderName;
	char* PixelFuntionNmae;
	TCHAR* GiometryShaderName;
	char* GiometryFuntionNmae;
	TCHAR* pTextureName;

	CrateObjectDest()
	{
		vertexShaderName = NULL;
		vertexFuntionNmae = NULL;
		PixelShaderName = NULL;
		PixelFuntionNmae = NULL;
		GiometryShaderName = NULL;
		GiometryFuntionNmae = NULL;
		pTextureName = NULL;
	}
};

typedef std::vector<PNCT_VECTOR> SUBVECTOR;
typedef std::vector<PNCT5_VERTEX> SKIN_SUB_VERTEX;
typedef std::vector<DWORD> SUBINDEX;

enum class E_LoadParserType
{
	ASE_TYPE = 0,
	PSB_TYPE,
};

struct SectionInfo
{
	bool Result;
	TCHAR* pHeaderName;

	SectionInfo()
	{
		Result = false;
	}
};

const int MAX_BONE_MATRICES = 255;


struct CBConstBoneWorld
{
	TMatrix g_mConstBoneWorld[MAX_BONE_MATRICES];
	CBConstBoneWorld()
	{
		ZeroMemory(&g_mConstBoneWorld, sizeof(TMatrix) * MAX_BONE_MATRICES);
	}
};

enum class E_SpaceType
{
	None,
	Plane,
	Box,
	Sphere,
	BoxPoint,
	SpherePoint,
};

enum class E_LoadObjectType
{
	NONE,
	HERO,
	CHARACTER,
	CHARACTER_MATRIX,
	CHARACTER_SKIN,
	CHARACTER_ANIMATION,
	CHARACTER_ITEM,
	DECO,

};

class LABoneObject;
class LASkinObject;
struct CheckObject
{
	bool bisRander;
	LABoneObject* pMatrixObject;
	LASkinObject* pSkinObject;
	CheckObject()
	{
		bisRander = true;
		pMatrixObject = NULL;
		pSkinObject = NULL;
	}
};


#endif


//LAResource

struct SectionPlay
{
	int ifirst;
	int ilast;
	int iCurrentframe;
	int iCurrentTick;
	float fElpaseTime;
	bool bReplay;
	SectionPlay()
	{
		ifirst = 0;
		ilast = 0;
		iCurrentTick = 0;
		iCurrentframe = 0;
		fElpaseTime = 0.0f;
		bReplay = true;
	}
};


//enum List

//�̵����ø���Ʈ��  ��� ��� �Է��� �������� ������ (������Ʈ ������ �ִϸ��̼� ������ ����Ʈ��ŭ�� �߰���)
//Anipatch[0], _countof(Anipatch[0]),		//������
//Anipatch[1], _countof(Anipatch[1]),		//����
//Anipatch[2], _countof(Anipatch[2]),		//����
//Anipatch[3], _countof(Anipatch[3]),		//������
//Anipatch[4], _countof(Anipatch[4]),		//����
//Anipatch[5], _countof(Anipatch[5]),		//�޸���
//Anipatch[6], _countof(Anipatch[6]),		//����
//Anipatch[7], _countof(Anipatch[7]),		//���
//Anipatch[8], _countof(Anipatch[8]),		//���� ����
//Anipatch[9], _countof(Anipatch[9]),		//���� ���� ��
//Anipatch[10], _countof(Anipatch[10]),		//���� ������
//Anipatch[11], _countof(Anipatch[11]),		//���� ����
//Anipatch[12], _countof(Anipatch[12]),		//���� ����
//Anipatch[13], _countof(Anipatch[13]),		//���� ���� ��
//Anipatch[14], _countof(Anipatch[14]),		//���� ������
//Anipatch[15], _countof(Anipatch[15]),		//���� ����
//Anipatch[16], _countof(Anipatch[16]),	//Į ����
//Anipatch[17], _countof(Anipatch[17]),	//Į ���� �� 
//Anipatch[18], _countof(Anipatch[18]),	//Į ���
//Anipatch[19], _countof(Anipatch[19]),	//����ź ����
//Anipatch[20], _countof(Anipatch[20]),	//����ź ���� ��
//Anipatch[21], _countof(Anipatch[21]),	//����ź ������
//Anipatch[22], _countof(Anipatch[22]),	//����ź ���
//Anipatch[23], _countof(Anipatch[23]),	//��ǰ ����
//Anipatch[24], _countof(Anipatch[24]),	//��ǰ ���� �� 
//Anipatch[25], _countof(Anipatch[25]),	//��ǰ ������
//Anipatch[26], _countof(Anipatch[26])	//��ǰ ���
enum class E_CharacterMotion	//18���� �׼�
{
	//���
	IDLE = 0,
	WORK_UP,
	WORK_DOWN,
	WORK_LEFT,
	WORK_RIGHT,
	RUN,
	JUMP,
	//DEATH,

	//RIFLE_BEFORE_SET,
	RIFLE_AFTER_SET,
	RIFLE_RELOAD,
	RIFLE_ACTION,

	//HANDGUN_BEFORE_SET,
	HANDGUN_AFTER_SET,
	//HANDGUN_RELOAD,
	//HANDGUN_ACTION,

	//KNIFE_BEFORE_SET,
	KNIFE_AFTER_SET,
	//KNIFE_ACTION,

	//BOMB_BEFORE_SET,
	BOMB_AFTER_SET,
	BOMB_RELOAD,
	BOMB_ACTION,

	//MEDIC_BEFORE_SET,
	MEDIC_AFTER_SET,
	MEDIC_RELOAD,
	MEDIC_ACTION,
};

/*================= Character Sound ===================*/
enum class E_CharacterSound
{
	CHAR_GROUND_R,
	CHAR_GROUND_L,
	CHAR_IRON_R, 
	CHAR_IRON_L,
	CHAR_WOOD_R,
	CHAR_WOOD_L,
	CHAR_SAND_R,
	CHAR_SAND_L,
	CHAR_CARFET_R,
	CHAR_CARFET_L
};

/*================= Monster Sound ===================*/
enum class E_MonsterSound
{
	SNDEFFECT_MOB1_MOV,
	SNDEFFECT_MOB1_RUN,
	SNDEFFECT_MOB1_DEAD
};

/*================= Weapon Sound ===================*/
enum class E_WeaponSound
{
	SNDEFFECT_SWAP,
	SNDEFFECT_TAKE, 
	SNDEFFECT_RELOAD,
};

/*================= Armor Sound ===================*/
enum class E_ArmorSound
{
	SNDEFFECT_ARMOR_TAKE,
	SNDEFFECT_ARMOR_WEAR,
};

/*================= Skill Trap Sound ===================*/
enum class E_SkillTrapSound
{
	SNDEFFECT_BOMB_ATTACK1,
	SNDEFFECT_BOMB_ATTACK2,
};

/*================= Skill Trap Sound ===================*/
enum class E_SkillBuffSound
{
	SNDEFFECT_ATTACK,
};

//Anipatch[0], _countof(Anipatch[0]),		//�����ɶ�
//Anipatch[1], _countof(Anipatch[1]),		//��������
//Anipatch[2], _countof(Anipatch[2]),		//�ɾ�ٴҶ�
//Anipatch[3], _countof(Anipatch[3]),		//�ٸ���
//Anipatch[4], _countof(Anipatch[4]),		//����
//Anipatch[5], _countof(Anipatch[5]),		//�׾�����
enum class E_MonsterMotion
{
	//SPAWN,
	//IDLE,
	WORK,
	//RUN,
	ATTACK,
	DEATH,
};

struct MotionKeyInfo
{
	DWORD dwkey;
	BYTE mKeyState;
	int iMotionNumber;

	MotionKeyInfo()
	{
		mKeyState = 0;
		dwkey = 0;
		iMotionNumber = 0;
	}
};

struct SaveAnimationDataInfo
{
	std::wstring LoadObjectName;
	std::wstring SaveFileName;
	int iAnimationNumber;
	int iFirstFrame;
	int iLastFrame;
	int iFrameSpeed;
	int iTickSecPerFrame;

	SaveAnimationDataInfo()
	{
		iAnimationNumber = 0;
		iFirstFrame = 0;
		iLastFrame = 0;
		iFrameSpeed = 0;
		iTickSecPerFrame = 0;
	}
};

struct ObjectRenderData
{
	int iCurrentScene;
	int iCurrentTick;
	TMatrix matObjectWorld;

	ObjectRenderData()
	{
		iCurrentScene = 0;
		iCurrentTick = 0;
		ZeroMemory(&matObjectWorld, sizeof(TMatrix));
	}
};

class LAResource;
struct RenderResourceInfo
{
	LAResource* pResource;
	std::vector<ObjectRenderData> ObjectRenderList;

	RenderResourceInfo()
	{
		pResource = NULL;
	}
};

enum class E_LoadTableType
{
	CHARACTER,
	MONSTER,
	WEAPON,
	ARMOR,
	OBJECT,
	MAP_OBJECT,
	SKILL_HITBOX,
	SKILL_BULLET,
	SKILL_BUFF,
	SKILL_TRAP,
};

enum class E_SkillType
{
	SKILL_HITBOX,
	SKILL_BULLET,
	SKILL_BUFF,
	SKILL_TRAP,
};
//1. ATTACK_POWER
//2. DEFENCE
//3. HP
//4. ATTACK_SPEED
//5. RUN_SPEED
enum class E_ArmorAbilityType
{
	ATTACK_POWER,
	DEFENCE,
	HP,
	ATTACK_SPEED,
	RUN_SPEED
};

enum class E_WeaponType
{
	RIFLE,
	HANDGUN,
	KNIFE,
	BOMB,
	MEDIC,
};

enum class E_ArmorType
{
	HEAD,
	BODY,
	HAND,
	FOOT,
	ACE,
};

//���̺��� �ε��� ������Ʈ �⺻������ 
struct ObjectTableLoadData
{
	E_LoadTableType eObjectType;
	int iTableIndex;
	std::wstring wsCodeName;
	std::wstring wsMatrixName;
	std::wstring wsSkinName;
	int iHp;
	float fDef;
	float fScale;
	float fRunSpeed;

	//������
	std::vector<std::wstring> wsIconNameList;
	//ȿ����
	std::vector<std::wstring> wsSoundNameList;

	//ĳ����
	std::vector<std::wstring> wsCharacterWeaponList;
	std::vector<std::wstring> wscharacterArmorList;

	//����, ������ ���� ���
	E_SkillType eSkillType;
	E_ArmorAbilityType e_ArmorAbilityType;
	E_WeaponType eWeaponType;
	E_ArmorType eArmorType;
	std::wstring wsSkillName;
	float fAbilityReaction;
	float fAbilityAimIncrease;
	bool bBullet_need_Reload;		//�ɷ�ġ ���� �پ������� ���� ����
	int iAbilityReloadMax;			//�ɷ�ġ ������ �������ִ� �ִ���� (�Ѿ��̳� ����ź���� ġ����)
	float fAbilityAttackSpeed;		//�ɷ�ġ ���ݼӵ�

	//������Ʈ
	float fObjectWeight;
	int iBulletCount;

	ObjectTableLoadData()
	{
		iTableIndex = -1;
		iHp = 0;
		fDef = 0.0f;
		fRunSpeed = 0.0f;

		//Monster
		fAbilityReaction = 0.0f;
		fAbilityAimIncrease = 0.0f;
		bBullet_need_Reload = false;
		iAbilityReloadMax = 0;
		fAbilityAttackSpeed = 1.0f;

		//StructureObject
		fObjectWeight = 0.0f;

		//���� Ÿ�Ժ�����(�Ѿ�, ����ź)
		eWeaponType = (E_WeaponType) - 1;
		eSkillType = (E_SkillType)-1;
		iBulletCount = 0;

		//Armor
		eArmorType = (E_ArmorType) - 1;
		e_ArmorAbilityType = (E_ArmorAbilityType)-1;
	}
};

struct ObjectTotalDataInfo
{
	int iTotalMaxHp;
	int iCurrentHp;
	float fTotalDef;
	float fTotalMoveSpeed;

	//������ �Ӽ��� �߰�
	float fTotalAddAttackPower;
	float fTotalAddAttackSpeed;
	float fTotalAddRunSpeed;
	

	ObjectTotalDataInfo()
	{
		iTotalMaxHp = 0;
		iCurrentHp = 0;
		fTotalDef = 0.0;
		fTotalMoveSpeed = 0.0;
		
		fTotalAddAttackPower = 0.0;
		fTotalAddAttackSpeed = 0.0;
		fTotalAddRunSpeed = 0.0;
	}
};

struct SkillTableLoadData
{
	/*============== ���� ==============*/
	E_LoadTableType eObjectType;
	int iTableIndex;			//�ε���
	std::wstring wsCodeName;	//�ڵ����
	//����Ʈ
	std::vector<std::wstring> wsEffectNameList;	//�߻�ü ����Ʈ �̸�
	//ȿ����
	std::vector<std::wstring> wsSoundNameList;	//ȿ����
	float fAttackPower;	//������(�Ѿ��ϰ�� 1�ߴ� ������)

	/*============== HIt Box ==============*/
	float fHitboxOffset;		//��Ʈ�ڽ� ������
	float fHitboxRadius;		//��Ʈ�ڽ� ������
	float fHitboxTime;			//��Ʈ�ڽ� ������Ÿ��
	
	/*============== Bullet ==============*/
	std::wstring wsBulletMesh3DName;	//������ 3D ���ҽ�
	float fBulletSpeed;					//�߻�ü ���ǵ�
	float fBulletRange;					//��Ÿ�

	/*============== Buff ==============*/
	float fHealPower;		//�� �Ŀ�
	float fHealTime;		//�� �ð�

	/*============== Trap ==============*/
	float fTrapDistance;
	float fTrapDiameter;
	float fTrapAttackDelay;

	SkillTableLoadData()
	{
		/*============== ���� ==============*/
		iTableIndex = -1;
		fAttackPower = 0.0f;
		/*============== HIt Box ==============*/
		fHitboxOffset = 0.0f;
		fHitboxRadius = 0.0f;
		fHitboxTime = 0.0f;
		/*============== Bullet ==============*/
		fBulletSpeed = 0.0f;
		fBulletRange = 0.0f;
		/*============== Buff ==============*/
		fHealPower = 0.0f;
		fHealTime = 0.0f;
		/*============== Trap ==============*/
		fTrapDistance = 0.0f;
		fTrapDiameter = 0.0f;
		fTrapAttackDelay = 0.0f;
	}
};

struct StructSecneData
{
	int iFirstFrame;
	int iLastFrame;
	int iFrameSpeed;
	int iTickSecPerFrame;
	int iObjectCount;
	int bisSkining;
};


#pragma pack(push, 1)

typedef struct _D3D_MATRIX {
	union {
		struct {
			int        _11, _12, _13, _14;
			int        _21, _22, _23, _24;
			int        _31, _32, _33, _34;
			int        _41, _42, _43, _44;
		};
		int m[4][4];
	};

	void operator = (TMatrix& rmatrix)
	{
		_11 = (int)(rmatrix._11 * 1000), _12 = (int)(rmatrix._12 * 1000), _13 = (int)(rmatrix._13 * 1000), _14 = (int)(rmatrix._14 * 1000);
		_21 = (int)(rmatrix._21 * 1000), _22 = (int)(rmatrix._22 * 1000), _23 = (int)(rmatrix._23 * 1000), _24 = (int)(rmatrix._24 * 1000);
		_31 = (int)(rmatrix._31 * 1000), _32 = (int)(rmatrix._32 * 1000), _33 = (int)(rmatrix._33 * 1000), _34 = (int)(rmatrix._34 * 1000);
		_41 = (int)(rmatrix._41 * 1000), _42 = (int)(rmatrix._42 * 1000), _43 = (int)(rmatrix._43 * 1000), _44 = (int)(rmatrix._44 * 1000);
	}

	TMatrix d3dxmatrix()
	{
		TMatrix redata;

		redata._11 = (float)(_11 / 1000.0f), redata._12 = (float)(_12 / 1000.0f), redata._13 = (float)(_13 / 1000.0f), redata._14 = (float)(_14 / 1000.0f);
		redata._21 = (float)(_21 / 1000.0f), redata._22 = (float)(_22 / 1000.0f), redata._23 = (float)(_23 / 1000.0f), redata._24 = (float)(_24 / 1000.0f);
		redata._31 = (float)(_31 / 1000.0f), redata._32 = (float)(_32 / 1000.0f), redata._33 = (float)(_33 / 1000.0f), redata._34 = (float)(_34 / 1000.0f);
		redata._41 = (float)(_41 / 1000.0f), redata._42 = (float)(_42 / 1000.0f), redata._43 = (float)(_43 / 1000.0f), redata._44 = (float)(_44 / 1000.0f);

		return  redata;
	}

}*LPD3D_MATRIX;


struct StructV4
{
	int x;
	int y;
	int z;
	int w;

	void operator = (TVector4& v4)
	{
		x = (int)(v4.x * 1000.0f);
		y = (int)(v4.y * 1000.0f);
		z = (int)(v4.z * 1000.0f);
		w = (int)(v4.w * 1000.0f);
	}
	TVector4 D3DXVEC4()
	{
		TVector4 data;

		data.x = (float)(x / 1000.0f);
		data.y = (float)(y / 1000.0f);
		data.z = (float)(z / 1000.0f);
		data.w = (float)(w / 1000.0f);

		return data;
	}
};

struct StructV2
{
	int x;
	int y;

	void operator = (TVector2& v2)
	{
		x = (int)(v2.x * 1000.0f);
		y = (int)(v2.y * 1000.0f);
	}

	TVector2 D3DXVEC2()
	{
		TVector2 data;

		data.x = (float)(x / 1000.0f);
		data.y = (float)(y / 1000.0f);

		return data;
	}
};

struct StructPNCT5_VERTEX
{
	TVector4 Vertex;
	TVector4 Normal;
	TVector4 Color;
	TVector2 Texture;
	float ObjNum1[4];
	float Weight1[4];
	float ObjNum2[4];
	float Weight2[4];
};
#pragma pack(pop)




struct StructObjectHeadData
{
	int ObjectType;
	int iMaterialNumber;
	int iCountNodeName;
	int iCountParantName;
};

struct StructMaterialData
{
	int iRootNumber;
	int iSubNumber;
	int iCountMaterialName;
};

struct StructMeshHeadData
{
	int iMeshSubCount;
	int iSubNumber;
	int iTextureNumber;
	int iFaceCount;
	int iSkining;
};

struct StructAnimationHeadData
{
	int iNodeNameCount;
	int iScaleCount;
	int iRotationCount;
	int iTransportCount;
};

struct StructAnimationData
{
	int Tick;
	TVector3 Pos;
	TQuaternion Axis;
};

static const float SCALE_BASE_SIZE = 2.0;
const int MAX_WEAPON_SIZE = 5;
const int MAX_ARMOR_SIZE = 5;
const int MAX_MOVE_KEY_COUNT = 4;
const int MAX_ACTION_KEY_COUNT = 9;
const float WORK_MOVE_SPEED = 1.5;
const float RUN_MOVE_SPEED = 2.0;


struct FIlePath
{
	std::wstring m_FirstFilepatch[6]; //table ���(ĳ����, ����, ����, ��, ������Ʈ, �ʿ�����Ʈ)
	std::wstring m_FilePatch[18]; //������ matrix, skin, animation ���
	std::wstring m_FileAnimationPatch[2]; //ĳ����, ���� �ִϸ��̼����̺� ���
	std::string m_FileSoundPatch[7];
	std::wstring m_SkillFilePatch[4];	//Hitbox, bullet, Buff, Trap
};


const int g_iMaxRenderMonsterCount = 3;
const int g_iMaxBoneMatrixCount = 40;
struct InstancingDataInfo
{
	int InstancingCount;
	TMatrix matNormalWorldInverse[g_iMaxRenderMonsterCount];	//�븻 �������� ��ȯ�� ���
	TMatrix matBoneAnimationWorld[g_iMaxRenderMonsterCount * g_iMaxBoneMatrixCount];	//�ִϸ��̼�

	InstancingDataInfo()
	{
		ZeroMemory(&matNormalWorldInverse, sizeof(TMatrix) * g_iMaxRenderMonsterCount);
		ZeroMemory(&matBoneAnimationWorld, sizeof(TMatrix) * g_iMaxRenderMonsterCount * g_iMaxBoneMatrixCount);
		InstancingCount = 0; 
	}
};