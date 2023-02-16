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
	matrix	g_matNormal		: packoffset(c12);
	float4	g_vColor		: packoffset(c16);
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
};

struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float3 n : NORMAL;
	float4 c : COLOR0;
	float2 t : TEXCOORD0;
};

float4 GetPSOutput(uint itextCount, float2 t)
{
	float4 vColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	switch (itextCount)
	{
		case 1:
		{
			vColor = g_txDiffuse[0].Sample(sample0, t);
		}break;
		case 2:
		{
			vColor = g_txDiffuse[0].Sample(sample0, t) 
				+ g_txDiffuse[1].Sample(sample0, t);
		}break;
		case 3:
		{
			vColor = g_txDiffuse[0].Sample(sample0, t)
				+ g_txDiffuse[1].Sample(sample0, t)
				+ g_txDiffuse[2].Sample(sample0, t);
		}break;
		case 4:
		{
			vColor = g_txDiffuse[0].Sample(sample0, t)
				+ g_txDiffuse[1].Sample(sample0, t)
				+ g_txDiffuse[2].Sample(sample0, t)
				+ g_txDiffuse[3].Sample(sample0, t);
		}break;
	}
	return vColor;
}

VS_OUTPUT VS(VS_INPUT Input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = float4(Input.p, 1.0f);
	output.p = mul(output.p, g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	output.n = Input.n;
	output.c = Input.c;
	output.t = Input.t;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 vColor = GetPSOutput((uint)g_vColor.w, input.t) * input.c;
	return vColor;
}