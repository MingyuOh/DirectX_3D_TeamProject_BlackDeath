#define SHADOW_EPSILON 0.005f
#define SMAP_SIZE 1024

// 쉐이더에서도 전처리기가 돌아간다.

Texture2D		g_txDiffuse: register (t0);
Texture2D		g_txDepthMap: register (t1);

SamplerState g_samLinear: register (s0);
SamplerState g_samShadowMap: register (s1);
SamplerComparisonState g_samComShadowMap: register (s2);

cbuffer cb0: register (b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	matrix  g_matNormal		: packoffset(c12);
};
cbuffer cb1: register (b1)
{
	float4x4		g_matShadow: packoffset(c0);
	float			g_iObjectID : packoffset(c4.x);
	float			g_iNumKernel : packoffset(c4.y);
}

struct VS_INPUT_OBJ
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXTURE;
	float4x4 mTransform : mTransform;
	uint InstanceID : SV_InstanceID;
};

struct PCT_VS_OUTPUT
{
	float4 p		: SV_POSITION;
	float2 t		: TEXCOORD0;
};

PCT_VS_OUTPUT VS(VS_INPUT_OBJ input)
{
	PCT_VS_OUTPUT output = (PCT_VS_OUTPUT)0;
	float3 vPos = input.p;//* fTime;
	output.p = float4(vPos, 1.0f);
	output.p = mul(output.p, input.mTransform);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.t = output.p.zw;
	return output;
}

float4 PS(PCT_VS_OUTPUT input) : SV_Target
{
	float shadow = input.t.x / input.t.y;
	return  float4(shadow, shadow, shadow, 1.0f);
}

//struct PCT_VS_OUTPUT
//{
//	float4 p		: SV_POSITION;
//	float4 c		: COLOR0;
//	float2 t		: TEXCOORD0;
//	float4 TexShadow : TEXCOORD1;
//};
//
//PCT_VS_OUTPUT VS(VS_INPUT_OBJ input)
//{
//	PCT_VS_OUTPUT output = (PCT_VS_OUTPUT)0;
//	float3 vPos = input.p;//* fTime;
//	output.p = float4(vPos, 1.0f);
//	output.p = mul(output.p, input.mTransform);
//	output.p = mul(output.p, g_matView);
//	output.p = mul(output.p, g_matProj);
//	output.c = input.c;
//	output.t = input.t;
//	output.TexShadow = mul(float4(vPos, 1.0f), g_matShadow);
//	return output;
//}

//float4 PS(PCT_VS_OUTPUT input) : SV_Target
//{
//	float4 vDiffuseColor = g_txDiffuse.Sample(g_samLinear, input.t);
//	float fLightAmount = 0.0f;
//	float3 ShadowTexColor = input.TexShadow.xyz / input.TexShadow.w;
//	const float fdelta = 1.0f / SMAP_SIZE;
//	int iHalf = (g_iNumKernel - 1) / 2;
//
//	for (int v = -iHalf; v <= iHalf; v++)
//	{
//		for (int u = -iHalf; u <= iHalf; u++)
//		{
//			float2 vOffset = float2(u*fdelta, v*fdelta);
//			fLightAmount +=	g_txDepthMap.SampleCmpLevelZero(g_samComShadowMap, ShadowTexColor.xy + vOffset, ShadowTexColor.z);
//		}
//	}
//	fLightAmount /= g_iNumKernel*g_iNumKernel;
//	float4 fColor = float4(fLightAmount, fLightAmount, fLightAmount, 1.0f);
//	float4 vFinalColor = vDiffuseColor*max(0.5f, fLightAmount);
//	vFinalColor.a = 1.0f;
//	return vFinalColor;
//}