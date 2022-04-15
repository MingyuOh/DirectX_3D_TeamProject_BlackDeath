#pragma once

/*라이브러리*/
#pragma warning (disable:4005)
#pragma comment (lib, "d3d11.lib")

#if defined(_DEBUG)
#pragma comment (lib, "d3dx10d.lib")
#pragma comment (lib, "d3dx11d.lib")
#else
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "d3dx11.lib")
#endif

#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (lib, "WindowsCodecs.lib")
#pragma comment(lib, "fmod_vc.lib")

/*헤더파일*/
#include <iostream>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <stdlib.h>
#include <queue>
#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include <crtdbg.h>
#include <math.h>
#include <string>
#include <d3dx11.h>
#include <d3d11.h>
#include <dinput.h>
#include <D3DCompiler.h>
#include <time.h>
#include <D3DX10math.h>
#include <vector>
#include <map>
#include <stack>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <memory>
#include <wrl.h>
#include <ctime>
#include <cmath>


extern UINT	 g_iWindowWidth;
extern UINT	 g_iWindowHeight;
extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext*	g_pImmediateContext;
extern D3D11_VIEWPORT* g_ViewPort;
extern ID3D11RenderTargetView* g_pRenderTargetView;
extern ID3D11DepthStencilView* g_pDepthStencilView;
extern IDXGISwapChain*			g_pSwapChain;

extern int						g_LoadingNum;

using namespace Microsoft::WRL;
using namespace std;
#define _CRT_SECURE_NO_WARNINGS

namespace BBASIS
{
	extern float g_fSecPerFrame;
	extern float g_fDurationTime;
	extern float g_fElapseTime;
	extern HWND g_hWnd;
	extern HINSTANCE g_hInstance;

	struct BBASIS_INPUT_MAP
	{
		bool bUpKey;
		bool bDownKey;
		bool bLeftKey;
		bool bRightKey;

		bool bWKey;
		bool bSKey;
		bool bAKey;
		bool bDKey;
		bool bQKey;
		bool bEKey;
		bool bZKey;
		bool bCKey;

		bool bBackViewKey;
		bool bDebugViewKey;
		bool bTreeViewKey;

		bool bLeftClick;
		bool bRightClick;
		bool bMiddleClick;

		bool bLeftHold;
		bool bRightHold;
		bool bMiddleHold;

		bool bExit;
		bool bSpace; // 카메라의 이동가속도를 증가시킨다.

		int  iMouseValue[3];

		bool bFullScreen;
		bool bChangeFillMode;
		bool bChangePrimitive;
		bool bChangeCullMode;
		bool bChangeCameraType;
		bool bDebugRender;
	};
	extern BBASIS_INPUT_MAP g_InputData;


	typedef basic_string<TCHAR> T_STR;
	typedef basic_string<wchar_t> W_STG;
	typedef basic_string<char>  C_STR;
	typedef vector<T_STR>		T_STR_VECTOR;
	typedef basic_string<TCHAR>::iterator	T_ITOR;
	typedef basic_string<wchar_t>::iterator	W_ITOR;
	typedef basic_string<char>::iterator	C_ITOR;
	typedef vector<T_STR>		T_ARRAY_ITOR;

	static std::wstring mtw(std::string str)
	{
		std::wstring ret = std::wstring(str.begin(), str.end());
		return  ret;
	}
	static std::string wtm(std::wstring str)
	{
		return  std::string(str.begin(), str.end());
	}
	static char* GetWtM(WCHAR* data)
	{
		char retData[4096] = { 0 };
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			data, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			data, -1,  //  소스
			retData, iLength, // 대상
			NULL, NULL);
		return retData;
	}
	static bool GetWtM(WCHAR* src, char* pDest)
	{
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			src, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			src, -1,  //  소스
			pDest, iLength, // 대상
			NULL, NULL);
		if (iRet == 0) return false;
		return true;
	}
	static WCHAR* GetMtW(char* data)
	{
		WCHAR retData[4096] = { 0 };
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
		int iRet = MultiByteToWideChar(CP_ACP, 0,
			data, -1,  //  소스
			retData, iLength); // 대상
		return retData;
	}
	static bool GetMtW(char* pSrc, WCHAR* pDest)
	{
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = MultiByteToWideChar(CP_ACP, 0,
			pSrc, -1, 0, 0);
		int iRet = MultiByteToWideChar(CP_ACP, 0,
			pSrc, -1,  //  소스
			pDest, iLength); // 대상		
		if (iRet == 0) return false;
		return true;
	}
	static void PRINT(char* fmt, ...) // 나열연산자
	{
		va_list arg;
		va_start(arg, fmt);
		char buf[256] = { 0, };
		vsprintf_s(buf, fmt, arg);
		printf("\n=====> %s", buf);
		va_end(arg);
	}

	static void ErrorQuit(TCHAR *msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(TCHAR*)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (TCHAR*)lpMsgBuf, msg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		exit(-1);
	}

	template<typename T>
	void stl_wipe_vector(vector<T>& rVector)
	{
		vector<T> emptyData;
		rVector.swap(emptyData);
	}
}

using namespace BBASIS;

/*싱글톤*/
template<class T> class BSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
	static T* GetInstancePtr()
	{
		static T theSingleInstance;
		return &theSingleInstance;
	}
};

/*객체 및 배열 할당과 삭제 및 소멸 매크로*/
#ifndef S_ZERO
#define S_ZERO(A)				{ A = 0 };
#endif

#ifndef S_NEW
#define S_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef S_DEL
#define S_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef S_NEW_ARRAY
#define S_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef S_DELETE_ARRAY
#define S_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef S_RELEASE
#define S_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef S_NEW_CLEAR
#define S_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef S_NEW_ARRAY_CLEAR
#define S_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif

#ifndef S_RETURN
#define S_RETURN(x){ hr = (x); if( FAILED(hr) ) { return hr; } }
#endif

#define BBASIS_START int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){ Sample gSample;
#define BBASIS_WIN(x) if( !gSample.InitWindow( hInstance, nCmdShow,  L#x ) )	{	return 0;	} /*ShowCursor(false);*/ gSample.Run();	return 1; }
#define BBASIS_RUN(x) TBASIS_START; TBASIS_WIN(x);


enum VertexControlerState
{
	vNone = 0,
	vUp,
	vDown,
	vParallel,
};

enum SpreatingState
{
	vSpreatNone = 0,
	vSpreatApply,
};

enum SpreatingColor
{
	vRed = 0,
	vGreen,
	vBlue,
	vAlpha,
};

enum EFFECT_TYPE 
{ 
  Straight = 0, 
  Circle 
}; // 이펙트 타입

static TCHAR* LoadSectionToken[] =
{
	L"*SCENESIZE",
	L"*SCENE",
	L"*PANEL",
	L"*BUTTON",
	L"*EDIT",
	L"*TEXTURE",
	L"*PEND"
};
enum LOADSECTION
{
	SECTION_SCENESIZE = 0,
	SECTION_SCENE,
	SECTION_PANEL,
	SECTION_BUTTON,
	SECTION_EDIT,
	SECTION_TEXTURE,
	SECTION_PEND
};
static TCHAR* ObejctSectionToken[] =
{
	L"*TEXTURENAME",
	L"*VERTEX",
	L"*MATWORLD",
	L"*END",
	L"*TEND"
};
enum LOADOBEJCT
{
	SECTION_TEXTURENAME = 0,
	SECTION_VERTEX,
	SECTION_MATWORLD,
	SECTION_END,
	SECTION_TEND
};

//--------------------------------------------------------------------------------------
// STL Delete 함수
//--------------------------------------------------------------------------------------
template<class T>
void DeleteVector(vector<T>& delData)
{
	vector<T> vClear;
	delData.swap(vClear);

	vClear.clear();
	delData.clear();
}

//--------------------------------------------------------------------------------------
// Define
//--------------------------------------------------------------------------------------
#define EPSILON						0.001f
#define PI							3.141592654f
#define DegreeToRadian(degree)		((degree) * ((PI) / (180.0f)))
#define RadianToDegree(radian)		((radian) * ((180.0f) / (PI)))
#define SAMPLE_BUFFER_SIZE			16 
#define KEYSTATECOUNT				256
#define randstep(min, max)			(min + ((float)max - (float)min) * (rand() / (float)RAND_MAX))
#define NEAR_DISTANCE				1.0f
#define FAR_DISTANCE				10000.0f


//--------------------------------------------------------------------------------------
// Character_UI_Struct
//--------------------------------------------------------------------------------------
struct LAObjectUIDataInfo
{
	int iCurrentCount;
	int iInventoryTotalCount;
	int iMaxCount;
	std::wstring wsiConFileName;


	//추가 
	//기본체력/현제최대체력
	float fHeroHp;
	float fHeroTotalHp;
	//기본방어력/현제최대방어력
	float fHeroDef;
	float fHeroTotalDef;
	//기본스피드/현제최대스피드
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

struct InstancingInfo
{
	int ObjName;
	int iInstanceNumber;
	D3DXVECTOR3 Scale;
	D3DXVECTOR3 RotationAxis[3];
	float Angle[3];
	D3DXVECTOR3 Translate;
	bool Selected;
	InstancingInfo()
	{
		ObjName = 9999;
		Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		RotationAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		RotationAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		RotationAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		Angle[0] = 0;
		Angle[1] = 0;
		Angle[2] = 0;
		Translate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Selected = false;
	}
};

struct SHADOW_CONSTANT_BUFFER
{
	D3DXMATRIX			g_matShadow;
	float				g_ShadowID;
	float				g_iNumKernel; // 커널을 통해 블러작업을 좀 더 주겠다.
	float				fZ, fW;
};

struct NORMAL_CONSTANT_BUFFER
{
	D3DXMATRIX g_matNormal[200];
	D3DXVECTOR3 cb_vLightVector;
	float Dumy1;
	D3DXVECTOR3 cb_vEyePos;
	float Dumy2;
	D3DXVECTOR3 cb_vEyeDir;
	float Dumy3;
};