Texture2D Texture0[4] : register (t0);		// 텍스쳐 레지스터 0번의 값을 가져오겠다.
Texture2D AlphaTexture : register (t1);		// 텍스쳐 레지스터 0번의 값을 가져오겠다.
Texture2D AlphaImageTex1 : register (t2);
Texture2D AlphaImageTex2 : register (t3);
Texture2D AlphaImageTex3 : register (t4);
Texture2D AlphaImageTex4 : register (t5);


SamplerState Sample0 : register (s0);	// 텍스쳐를 컨트롤 하기 위한 변수

cbuffer cb0
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION; // 버텍스 포지션
	float3 Normal : NORMAL;		   // 버텍스 노멀
	float4 Diffuse : COLOR0;	   // 버텍스 색상값
	float2 Texture : TEXCOORD0;
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
	Output.Position = mul(Output.Position, g_matWorld);
	Output.Position = mul(Output.Position, g_matView);
	Output.Position = mul(Output.Position, g_matProj);

	Output.Normal = float3(n);
	Output.Diffuse = float4(c);
	Output.Texture = float2(t);
	return Output;
}
//Texture0[0].Sample(Sample0, input.Texture);
float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.Diffuse;
}