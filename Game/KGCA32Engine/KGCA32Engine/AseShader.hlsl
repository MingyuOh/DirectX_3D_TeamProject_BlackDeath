#define SHADOW_EPSILON 0.0005f
#define SMAP_SIZE 1024

Texture2D g_txDiffuse   : register (t0); // 텍스쳐 레지스터 0번의 값을 가져오겠다.
Texture2D g_txDepthMap  : register (t1); // 쉐도우 텍스쳐
Texture2D g_txNormalMap : register (t2); // 노말맵 텍스쳐
SamplerState Sample0 : register (s0);	// 텍스쳐를 컨트롤 하기 위한 변수

SamplerState g_samLinear: register (s0);
SamplerState g_samShadowMap: register (s1);
SamplerComparisonState g_samComShadowMap: register (s2);

// 기본 상수버퍼
cbuffer cb0 : register (b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	matrix  g_matNormal		: packoffset(c12);
};

// 쉐도우 상수버퍼
cbuffer cb1: register (b1)
{
	float4x4		g_matShadow: packoffset(c0);
	float			g_iObjectID : packoffset(c4.x);
	float			g_iNumKernel : packoffset(c4.y);
}

// 노말맵핑 상수버퍼
cbuffer cb2: register (b2)
{
	float4x4 g_matMapNormal[200];
	float4 cb_vLightVector;
	float4 cb_vEyePos;
	float4 cb_vEyeDir;
}

struct VS_OUTPUT
{
	float4 Position		: SV_POSITION; // 버텍스 포지션
	float3 Normal		: NORMAL;	   // 버텍스 노멀
	float4 Diffuse		: COLOR0;	   // 버텍스 색상값
	float2 Texture		: TEXCOORD0;
	float4 TexShadow	: TEXCOORD1;
	float3 vEye			: TEXCOORD2;
	float3 vHalf		: TEXCOORD3;
	float3 vLightDir	: TEXCOORD4;
};

struct VS_INPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXTURE;
	float4x4 mTransform : mTransform;
	uint InstanceID : SV_InstanceID;
	float3 Tan : TANGENT;
};

struct PS_OUTPUT
{
	float4 c : SV_Target;
};

VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT Output;
	float3 vPos = vIn.p;//* fTime;
	Output.Position = float4(vPos, 1.0f);
	float4 ShadowPosition = float4(Output.Position);
	Output.Position = mul(Output.Position, vIn.mTransform); //Output.Position = mul(Output.Position, g_matWorld);
	float4 NormalPosition = float4(Output.Position);
	Output.Position = mul(Output.Position, g_matView);
	Output.Position = mul(Output.Position, g_matProj);

	Output.Normal = float3(vIn.n);
	Output.Normal = mul(Output.Normal, (float3x3)g_matMapNormal[vIn.InstanceID]);
	Output.Normal = normalize(Output.Normal);

	Output.Diffuse = float4(vIn.c);
	Output.Texture = float2(vIn.t);

	// 쉐도우
	ShadowPosition = mul(ShadowPosition, vIn.mTransform);
	Output.TexShadow = mul(ShadowPosition, g_matShadow);

	//노말맵핑
	float3 vNormal = normalize(mul(vIn.n, (float3x3)g_matMapNormal[vIn.InstanceID]));
	Output.vEye = normalize(cb_vEyePos.xyz - NormalPosition.xyz);

	float3 T = normalize(mul(vIn.Tan, (float3x3)g_matMapNormal[vIn.InstanceID]));
	float3 B = normalize(cross(vNormal, T));

	float3x3 tanMat = { T.x, B.x, vNormal.x,
		T.y, B.y, vNormal.y,
		T.z, B.z, vNormal.z };
	float3 vLightDir = normalize(cb_vLightVector.xyz - NormalPosition.xyz);
	Output.vHalf = normalize(mul(normalize(vLightDir + Output.vEye), tanMat));
	Output.vLightDir = normalize(mul(vLightDir, tanMat));
	Output.vEye = normalize(mul(Output.vEye, tanMat));

	return Output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	// 디퓨즈
	float4 normal = g_txNormalMap.Sample(g_samLinear, input.Texture);
	normal = normalize((normal - 0.5f) * 2.0f);
	float fDot = saturate(dot(normal.xyz, input.vLightDir));
	float3 LightColor = float3(1.0f, 1.0f, 1.0f) * fDot;
	// 스펙큘러
	float3 R = reflect(input.vLightDir, normal.xyz);
	float3 SpecColor = float3(1.0f, 1.0f, 1.0f) * pow(saturate(dot(R, input.vEye)), 10.0f);

	float4 TestLight = float4(SpecColor, 1.0f);

	float4 FinalLight = float4(LightColor + SpecColor, 1.0f);
	FinalLight.x = max(FinalLight.x, 0.5f);
	FinalLight.y = max(FinalLight.x, 0.5f);
	FinalLight.z = max(FinalLight.x, 0.5f);
	if (normal.x <= 0.0f && normal.y <= 0.0f && normal.z <= 0.0f && normal.w <= 0.0f)
	{
		FinalLight = float4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	float4 vDiffuseColor = g_txDiffuse.Sample(g_samLinear, input.Texture);
	vDiffuseColor = vDiffuseColor * FinalLight;
	float fLightAmount = 0.0f;
	float3 ShadowTexColor = input.TexShadow.xyz / input.TexShadow.w;
	const float fdelta = 1.0f / SMAP_SIZE;
	int iHalf = (g_iNumKernel - 1) / 2;

	for (int v = -iHalf; v <= iHalf; v++)
	{
		for (int u = -iHalf; u <= iHalf; u++)
		{
			float2 vOffset = float2(u*fdelta, v*fdelta);
			fLightAmount += g_txDepthMap.SampleCmpLevelZero(g_samComShadowMap, ShadowTexColor.xy + vOffset, ShadowTexColor.z).r;
		}
	}
	fLightAmount /= g_iNumKernel*g_iNumKernel;

	float4 fColor = float4(fLightAmount, fLightAmount, fLightAmount, 1.0f);
	float4 vFinalColor = vDiffuseColor*max(0.5f, fLightAmount);
	vFinalColor.a = 1.0f;

	return vFinalColor;
}