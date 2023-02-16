#define FOGMODE_NONE    0
#define FOGMODE_LINEAR  1
#define FOGMODE_EXP     2
#define FOGMODE_EXP2    3
#define E 2.71828

Texture2D Texture0[4]	 : register (t0);
Texture2D NormalTexture  : register (t1);

SamplerState g_samLinear : register (s0);

// VS
struct VS_INPUT
{
	float3 Position		 : POSITION;
	float3 Normal		 : NORMAL;
	float4 Color		 : COLOR;
	float2 Texture		 : TEXTURE;
	float3 Tangent		 : TANGENT;
};
struct VS_OUTPUT
{
	float4 Position		 : SV_POSITION;
	float3 Normal		 : NORMAL;
	float4 Color		 : COLOR0;
	float2 Texture		 : TEXCOORD0;
	float3 vEye			 : TEXCOORD1;
	float3 vHalf		 : TEXCOORD2;
	float3 vLightDir	 : TEXCOORD3;
	float4 FogDist		 : TEXCOORD4;
	float  clip : SV_ClipDistance0;
};

// PS
struct PS_OUTPUT
{
	float4 Color :COLOR0;
};

// CB
cbuffer cb0 : register(b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	float4  g_MeshColor     : packoffset(c12);
};

cbuffer cbFog : register(b1)
{
	float4	g_ClipPlane:	packoffset(c0);
	float4	g_vCameraPos:	packoffset(c1);
	float4	g_FogColor:		packoffset(c2);
	float	g_FogMode : packoffset(c3.x);   // = FOGMODE_LINEAR;
	float	g_FogDensity : packoffset(c3.y);
	float	g_FogStart : packoffset(c3.z);
	float	g_FogEnd : packoffset(c3.w);
}

cbuffer cbTanLight : register(b2)
{
	matrix  g_matNormal;
	float3  cb_vLightVector;
	float3  cb_vEyePos;
	float3  cb_vEyeDir;
}

cbuffer cbObjectNeverChanges : register(b3)
{
	float4	cb_AmbientLightColor: packoffset(c0);
	float4	cb_DiffuseLightColor: packoffset(c1);
	float3	cb_SpecularLightColor: packoffset(c2);
	float	cb_SpecularPower : packoffset(c2.w);
}

// FogMode에 따른 계산함수
float CalcFogFactor(float4 vValue)
{
	float fogCoeff = 1.0f;

	if (FOGMODE_LINEAR == g_FogMode)
	{
		fogCoeff = (vValue.y - vValue.z) / (vValue.y - vValue.x);
	}
	else if (FOGMODE_EXP == g_FogMode)
	{
		fogCoeff = 1.0 / pow(E, vValue.z*g_FogDensity);
	}
	else if (FOGMODE_EXP2 == g_FogMode)
	{
		fogCoeff = 1.0 / pow(E, vValue.z*vValue.z*g_FogDensity*g_FogDensity);
	}

	return clamp(fogCoeff, 0, 1);
}

VS_OUTPUT VS(VS_INPUT vIn)
{
	VS_OUTPUT output = (VS_OUTPUT)0; // 초기화
	output.Position = float4(vIn.Position, 1.0f);
	float4 vWorldPos = mul(output.Position, g_matWorld);
	float4 vCameraPos = mul(vWorldPos, g_matView);
	output.Position = mul(vCameraPos, g_matProj);
	output.Texture = vIn.Texture;
	output.Color = vIn.Color;
	output.clip = dot(vWorldPos, g_ClipPlane);

	float3 vNormal = normalize(mul(vIn.Normal, (float3x3)g_matNormal));
	output.vEye = normalize(cb_vEyeDir);

	float3 T = normalize(mul(vIn.Tangent, (float3x3)g_matNormal));
	float3 B = normalize(cross(vNormal, T));
	// tanMat 접선벡터
	float3x3 tanMat = { T.x, B.x, vNormal.x,
		T.y, B.y, vNormal.y,
		T.z, B.z, vNormal.z };
	// 조명 미구현
	//float3 vLightDir = cb_vLightVector.xyz;
	//output.vHalf = normalize(mul(normalize(vLightDir + output.vEye), tanMat));
	//output.vLightDir = normalize(mul(-vLightDir, tanMat));
	//output.vEye = normalize(mul(output.vEye, tanMat));

	return output;
}

float4 PS(VS_OUTPUT vIn) : SV_Target
{
	PS_OUTPUT vOut;
vOut.Color = Texture0[0].Sample(g_samLinear, vIn.Texture);

// 조명 미구현
//// 디퓨즈 조명 
//float4 normal = g_txNormalMap.Sample(g_samLinear, input.t);
//normal = normalize((normal - 0.5f) * 2.0f);
//float  fDot = saturate(dot(normal.xyz, input.vLightDir));
//float3 LightColor = cb_DiffuseLightColor.rgb * fDot;

//// 스펙큘러 조명 		
//float3 R = reflect(-input.vLightDir, normal.xyz);
//float3 SpecColor = cb_SpecularLightColor.rgb * pow(saturate(dot(R, input.vEye)), cb_SpecularPower);

//// 전체 컬러 조합  	
//float4 vFinalColor = DiffuseColor * float4(LightColor + SpecColor, 1.0f);
//vFinalColor.a = 1.0f;

// 거리 포그 및 높이 포그 적용
float fFogDistance = 1.0f - CalcFogFactor(vIn.FogDist); // 거리포그
float fValue = (-g_ClipPlane.w - vIn.FogDist.w) / (-g_ClipPlane.w - vIn.FogDist.w - 2.0f);
float fFogHeight = 1.0f - fValue;						// 높이포그
float4 vFogColor = g_FogColor * (fFogDistance + fFogHeight);
vOut.Color = vOut.Color + vFogColor;
return vOut.Color;
}

float4 Clip_PS(VS_OUTPUT vIn) : SV_Target
{
	PS_OUTPUT vOut;
vOut.Color = Texture0[0].Sample(g_samLinear, vIn.Texture);

// 조명 미구현
//// 디퓨즈 조명 
//float4 normal = g_txNormalMap.Sample(g_samLinear, input.t);
//normal = normalize((normal - 0.5f) * 2.0f);
//float  fDot = saturate(dot(normal.xyz, input.vLightDir));
//float3 LightColor = cb_DiffuseLightColor.rgb * fDot;

//// 스펙큘러 조명 		
//float3 R = reflect(-input.vLightDir, normal.xyz);
//float3 SpecColor = cb_SpecularLightColor.rgb * pow(saturate(dot(R, input.vEye)), cb_SpecularPower);

//// 전체 컬러 조합  	
//float4 vFinalColor = DiffuseColor * float4(LightColor + SpecColor, 1.0f);
//vFinalColor.a = 1.0f;
return vOut.Color;
}


VS_OUTPUT NoneClip_VS(VS_INPUT vIn)
{
	VS_OUTPUT output = (VS_OUTPUT)0; // 초기화
	output.Position = float4(vIn.Position, 1.0f);
	float4 vWorldPos = mul(output.Position, g_matWorld);
	float4 vCameraPos = mul(vWorldPos, g_matView);
	output.Position = mul(vCameraPos, g_matProj);
	output.Texture = vIn.Texture;
	output.Color = vIn.Color;

	float3 vNormal = normalize(mul(vIn.Normal, (float3x3)g_matNormal));
	output.vEye = normalize(cb_vEyeDir);

	float3 T = normalize(mul(vIn.Tangent, (float3x3)g_matNormal));
	float3 B = normalize(cross(vNormal, T));
	// tanMat 접선벡터
	float3x3 tanMat = { T.x, B.x, vNormal.x,
		T.y, B.y, vNormal.y,
		T.z, B.z, vNormal.z };
	// 조명 미구현
	//float3 vLightDir = cb_vLightVector.xyz;
	//output.vHalf = normalize(mul(normalize(vLightDir + output.vEye), tanMat));
	//output.vLightDir = normalize(mul(-vLightDir, tanMat));
	//output.vEye = normalize(mul(output.vEye, tanMat));

	return output;
}