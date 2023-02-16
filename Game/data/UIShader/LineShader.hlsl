struct VS_OUTPUT
{
	float4 p : SV_POSITION;
	float4 c : COLOR0;
};
struct VS_INPUT
{
	float3 p : POSITION;
	float4 c : COLOR;
};
struct PS_OUTPUT
{
	float4 c : SV_Target;   
};

cbuffer CB  
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
}

//cbuffer CB
//{
//	matrix  matWorld : packoffset(c0); // c0,~ c3
//	float a : packoffset(c4.x);
//	float fTime : packoffset(c4.y); // 4¹è¼ö	
//	float c : packoffset(c4.z);
//	float d : packoffset(c4.w);
//}

VS_OUTPUT VS(VS_INPUT input)
{

	VS_OUTPUT output = (VS_OUTPUT)0;
	output.p = mul(float4(input.p, 1.0f), g_matWorld);
	output.p = mul(output.p, g_matView);
	output.p = mul(output.p, g_matProj);
	//output.p = mul( float4(input.p, 1.0f), matWorld);
	output.c = input.c;// *fTime;
	return output;

	//VS_OUTPUT output = (VS_OUTPUT)0;
	//output.p = float4(input.p, 1.0f);
	//output.p = mul(input.p, g_matWorld);
	//output.p = mul(output.p, g_matView);
	//output.p = mul(output.p, g_matProj);
	//output.c = input.c;// *fTime;
	//return output;
}

PS_OUTPUT PS(VS_OUTPUT input)
{
	PS_OUTPUT vOut = (PS_OUTPUT)0;  
	vOut.c = input.c;
	return vOut;
}