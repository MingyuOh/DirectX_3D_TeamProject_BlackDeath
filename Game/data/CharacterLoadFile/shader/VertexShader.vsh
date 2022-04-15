
cbuffer cb0
{
	matrix g_matWorld	:	packoffset(c0);
	matrix g_matView	:	packoffset(c4);
	matrix g_matProj	:	packoffset(c8);
	matrix g_matInverse :	packoffset(c12);
};

cbuffer cb1 : register(b1)
{
	float4 g_vLightPos	:	packoffset(c0);
	float4 g_vLIghtDirection	:	packoffset(c1);
	float4 g_vEyePosition	:	packoffset(c2);
	float4 g_Data01 : packoffset(c3);
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION; // 버텍스 포지션
	float4 Normal  : NORMAL;
	float4 Diffuse : COLOR0;      // 버텍스 색상값
	float2 Texture : TEXCOORD;	//Texture 포지션
};

struct VS_INPUT
{
	float4 p : POSITION;
	float4 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT vln)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	Output.Position = mul(vln.p, g_matWorld);
	Output.Position = mul(Output.Position, g_matView);
	Output.Position = mul(Output.Position, g_matProj);
	//Output.Position = vln.p;
	Output.Normal = vln.n;
	//Output.Normal = normalize(mul(vln.n, g_matInverse));
	if ((uint)g_Data01.x == 1)
	{
		Output.Diffuse = float4(1,0,0,1);
	}
	else
	{
		Output.Diffuse = vln.c;
	}
	Output.Texture = vln.t;

	return Output;
}