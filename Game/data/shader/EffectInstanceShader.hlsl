Texture2D g_txDiffuse[4]	: register(t0);
SamplerState sample0		: register(s0);

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer cb0
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4	g_vColor		: packoffset(c12);
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 p : POSITION;
	float3 n : NORMAL;
	float4 c : COLOR;
	float2 t : TEXTURE;

	float4x4	matInstance   : mTransform;
	float4		instanceColor : PARTICLECOLOR;
	uint		InstanceId	  : SV_InstanceID;
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT Input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = float4(Input.p, 1.0f);
	output.p = mul(output.p, Input.matInstance);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.n = Input.n;
	output.c = Input.instanceColor;
	output.t = Input.t;
	return output;
}
