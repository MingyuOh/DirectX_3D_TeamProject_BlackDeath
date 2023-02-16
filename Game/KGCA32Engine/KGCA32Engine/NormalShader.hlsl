Texture2D Texture0[4] : register (t0);		
Texture2D AlphaTexture : register (t1);		
Texture2D AlphaImageTex1 : register (t2);
Texture2D AlphaImageTex2 : register (t3);
Texture2D AlphaImageTex3 : register (t4);
Texture2D AlphaImageTex4 : register (t5);
Texture2D g_txDepthMap : register (t6); // 쉐도우 텍스쳐

#define SMAP_SIZE 1024

SamplerState Sample0 : register (s0);	// 텍스쳐를 컨트롤 하기 위한 변수
SamplerState g_samShadowMap: register (s1);
SamplerComparisonState g_samComShadowMap: register (s2);

cbuffer cb0 : register(b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
};

cbuffer cb1: register(b1)
{
	float4x4		g_matShadow: packoffset(c0);
	float			g_iObjectID : packoffset(c4.x);
	float			g_iNumKernel : packoffset(c4.y);
}

struct VS_OUTPUT
{
	float4 Position : SV_POSITION; // 버텍스 포지션
	float3 Normal : NORMAL;		   // 버텍스 노멀
	float4 Diffuse : COLOR0;	   // 버텍스 색상값
	float2 Texture : TEXCOORD0;
	float4 TexShadow : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4 c : SV_Target;
};

VS_OUTPUT VS(in float3 p : POSITION, in float3 n : NORMAL, in float4 c : COLOR, in float2 t : TEXTURE)
{
	VS_OUTPUT Output;
	float3 vPos = p;
	Output.Position = float4(vPos, 1.0f);
	float4 ShadowPosition = float4(Output.Position);
	Output.Position = mul(Output.Position, g_matWorld);
	Output.Position = mul(Output.Position, g_matView);
	Output.Position = mul(Output.Position, g_matProj);
	
	Output.Normal = float3(n);
	Output.Diffuse = float4(c);
	Output.Texture = float2(t);

	ShadowPosition = mul(ShadowPosition, g_matWorld);
	Output.TexShadow = mul(ShadowPosition, g_matShadow);

	return Output;
}
//Texture0[0].Sample(Sample0, input.Texture);
float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 vLightDir = float3(240.0f, 160.0f, -130.0f);
	float fDot = dot(input.Normal, normalize(-vLightDir));

	PS_OUTPUT Alpha;
	Alpha.c = AlphaTexture.Sample(Sample0, input.Texture);
	PS_OUTPUT vOut;
	vOut.c = AlphaImageTex1.Sample(Sample0, input.Texture*20) * Alpha.c.x;
	vOut.c += AlphaImageTex2.Sample(Sample0, input.Texture*20) * Alpha.c.y;
	vOut.c += AlphaImageTex3.Sample(Sample0, input.Texture*20) * Alpha.c.z;
	vOut.c += AlphaImageTex4.Sample(Sample0, input.Texture) * Alpha.c.w;


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
	vOut.c = vOut.c*max(0.5f, fLightAmount);
	vOut.c.a = 1.0f;

	return vOut.c;// * fDot;
}

float4 AlphaMap_PS(VS_OUTPUT input) : SV_Target
{
	PS_OUTPUT Alpha;
	Alpha.c = AlphaTexture.Sample(Sample0, input.Texture);
	if (Alpha.c.x + Alpha.c.y + Alpha.c.z > 0)
		Alpha.c.w = 0.0f;
	//if (Alpha.c.w == 1.0f)
	//{
	//	Alpha.c.x = 1.0f;
	//	Alpha.c.y = 1.0f;
	//	Alpha.c.z = 1.0f;
	//}
	Alpha.c.w = 1.0f;
	return Alpha.c;
}