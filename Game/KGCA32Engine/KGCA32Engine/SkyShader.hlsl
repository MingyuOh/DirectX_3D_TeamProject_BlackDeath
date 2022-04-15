cbuffer cb0 /*상수버퍼*/
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
	uint ID : SV_PrimitiveID;
};

struct PS_OUTPUT
{
	float4 c : SV_Target;
};

Texture2D TextureSky[6] : register (t0);
SamplerState Sample0 : register (s0);	// 텍스쳐를 컨트롤 하기 위한 변수

VS_OUTPUT VS(in float3 p : POSITION, in float3 n : NORMAL, in float4 c : COLOR, in float2 t : TEXTURE)
{
	VS_OUTPUT Output;
	float3 vPos = p;
	float4 Proj = float4(vPos, 1.0f);
	Proj = mul(Proj, g_matWorld);
	Proj = mul(Proj, g_matView);
	Proj = mul(Proj, g_matProj);
	Output.Position = Proj.xyww;

	Output.Normal = float3(n);
	Output.Diffuse = float4(c);
	Output.Texture = float2(t);
	return Output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 vColor = float4(1.0f,1.0f,1.0f,1.0f);
	int TexID = (input.ID / 2);
	switch (TexID)
	{
	case 0:
		vColor = TextureSky[0].Sample(Sample0, input.Texture);
		break;
	case 1:
		vColor = TextureSky[1].Sample(Sample0, input.Texture);
		break;
	case 2:
		vColor = TextureSky[2].Sample(Sample0, input.Texture);
		break;
	case 3:
		vColor = TextureSky[3].Sample(Sample0, input.Texture);
		break;
	case 4:
		vColor = TextureSky[4].Sample(Sample0, input.Texture);
		break;
	case 5:
		vColor = TextureSky[5].Sample(Sample0, input.Texture);
		break;
	}
	PS_OUTPUT vOut;
	vOut.c = vColor;
	return vOut.c;
}