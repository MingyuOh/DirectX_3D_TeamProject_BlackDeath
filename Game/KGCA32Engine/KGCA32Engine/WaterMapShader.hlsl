#define FOGMODE_NONE    0
#define FOGMODE_LINEAR  1
#define FOGMODE_EXP     2
#define FOGMODE_EXP2    3
#define E 2.71828

Texture2D g_txDiffuse	: register (t0);
Texture2D g_txBottomMap	: register (t1);      // m_pRefractRV // ��������
Texture2D g_txUpMap		: register (t2);      // m_pReflectRV // �ݻ纤��
Texture2D g_txNormalMap : register (t3);
Texture2D g_txHeightMap : register (t4); 
Texture2D g_txEnvMap    : register (t5); // �̻��

SamplerState g_samLinear : register (s0);


// CB
cbuffer cb0 : register(b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	matrix	g_matNormal		: packoffset(c12);
	float4  g_MeshColor     : packoffset(c16);
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
	matrix  g_matLightNormal;
	float4  cb_vLightVector;
	float4  cb_vEyePos;
	float4  cb_vEyeDir;
}

cbuffer cbObjectNeverChanges : register(b3)
{
	float4	cb_AmbientLightColor: packoffset(c0);
	float4	cb_DiffuseLightColor: packoffset(c1);
	float3	cb_SpecularLightColor: packoffset(c2);
	float	cb_SpecularPower : packoffset(c2.w);
}

// VS
struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 Color : COLOR;
	float2 Texture : TEXTURE;
	float3 Tangent : TANGENT;
};
struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Normal	: NORMAL;
	float4 Color	: COLOR;
	float2 Texture  : TEXCOORD0;
	float2 TextureW : TEXCOORD1;
	float3 vEye		: TEXCOORD2;
	float3 vHalf	: TEXCOORD3;
	float3 vLightDir: TEXCOORD4;
	float3 r		: TEXCOORD5; // �� �ݻ� �Ʒ��� 
	float3 f		: TEXCOORD6; // �� �ݻ� ����
	float3 e		: TEXCOORD7; // �������� �Ÿ�
};

// FogMode�� ���� ����Լ�
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
	VS_OUTPUT output = (VS_OUTPUT)0; // �ʱ�ȭ
	float4 worldPos = mul(float4(vIn.Position, 1), g_matWorld);
	float fTime = cos(g_MeshColor.x)* 0.1f;
	float fAngle = fTime * length(vIn.Position / 256.0f);
	float fCos, fSin;
	sincos(fAngle, fSin, fCos);
	output.Color = vIn.Color;
	output.Texture = vIn.Texture;
	//worldPos.y = min(worldPos.y, worldPos.y - 2.0f - cos(fTime * vIn.Position.x) + sin(fTime * vIn.Position.z));
	output.Color.w = worldPos.y;

	float4 cameraPos = mul(worldPos, g_matView);
	output.Position = mul(cameraPos, g_matProj);

	float3 vNormal = normalize(mul(vIn.Normal, (float3x3)g_matLightNormal));
	output.Normal = vNormal;

	float3 cb_EyeD = cb_vEyeDir.xyz;
	output.vEye = normalize(cb_EyeD);

	float3 T = normalize(mul(vIn.Tangent, (float3x3)g_matLightNormal));
	float3 B = normalize(cross(vNormal, T));

	float3x3 tanMat = { T.x, B.x, vNormal.x,
						T.y, B.y, vNormal.y,
						T.z, B.z, vNormal.z };

	float3 vLightDir = cb_vLightVector.xyz;
	output.vHalf = normalize(mul(normalize(vLightDir + output.vEye), tanMat));
	output.vLightDir = normalize(mul(-vLightDir, tanMat));
	output.vEye = normalize(mul(output.vEye, tanMat));

	// �������� ���� �� �ݻ� �ؽ��� ��ǥ
	float2 tex;
	tex.x = 0.5f * output.Position.x / output.Position.w + 0.5f;
	tex.y = -0.5f * output.Position.y / output.Position.w + 0.5f;
	output.TextureW = tex;

	const float ref_at_norm_incidence = 1.33f;
	float3 Incident = normalize(worldPos.xyz - cb_vEyePos.xyz);
	output.e = Incident;

	output.r = normalize(reflect(Incident, output.Normal));
	output.f = normalize(refract(Incident, output.Normal, 1 / ref_at_norm_incidence));

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	// ��ǻ�� ����
	float4 vWaterColor = g_txDiffuse.Sample(g_samLinear, input.Texture);
	float4 normal = g_txNormalMap.Sample(g_samLinear, input.Texture * 30.0f);
	normal = normalize((normal - 0.5f) * 2.0f);
	float fDot = saturate(dot(normal.xyz, input.vLightDir)); // saturate = 0~1�� ������ ���� ���ѽ�Ű�� �Լ�
	float3 LightColor = cb_DiffuseLightColor.rgb * fDot;
	
	// ����ŧ�� ����
	float3 R = reflect(-input.vLightDir, normal.xyz);
	float3 SpecColor = cb_SpecularLightColor.rgb * pow(saturate(dot(R, input.vEye)), cb_SpecularPower);

	// ��ü �÷� ����
	float4 vLightColor = float4(LightColor + SpecColor, 1.0f);

	float4 vRefractColor = g_txBottomMap.Sample(g_samLinear, input.TextureW + normal.xy * 0.005f);
	float4 vReflectColor = g_txUpMap.Sample(g_samLinear, input.TextureW + normal.xy * 0.01f);
	float4 vColor = vLightColor * vWaterColor * (vRefractColor + vReflectColor);
	vColor.a = 1.0f;

	// �ٴ� ���̰��� ���� ���İ� ���
	// ���̰��� ���� ����ó�� �� ��������
	float fHeightColor = (g_txHeightMap.Sample(g_samLinear, input.Texture).x * g_MeshColor.y) + g_MeshColor.z;
	float fDistance = 0.0f;
	if (input.Color.w >= fHeightColor) //���� ������ ���̰� ���� ���̺��� ���ٸ�
	{
		fDistance = (input.Color.w - fHeightColor) / (input.Color.w);// - g_MeshColor.z);
		if (fDistance < 0.0f)
		{
			fDistance = fDistance * -1.0f;
		}
		fDistance = saturate(fDistance);
	}
	float4 vFinalColor = lerp(float4(1.0f, 1.0f, 1.0f, 0.01f), vColor, fDistance);
	// ȯ��� ���� �̱���
	//const float ref_at_norm_incidence = 1.33f;
	//float4 ReflectedColor = g_txEnvMap.Sample(g_samLinear, input.r);
	//float4 RefractedColor = g_txEnvMap.Sample(g_samLinear, input.f);

	//float R0 = pow(1.0 - ref_at_norm_incidence, 2.0) / pow(1.0 + ref_at_norm_incidence, 2.0);
	//float fresnel = ComputeFresnel(input.r, input.n, R0);
	//// ȯ��ʰ� ��ǻ����� ����
	//float4 vEnvColor = lerp(vFinalColor, ReflectedColor, fresnel + 0.3f);
	//vFinalColor = lerp(vFinalColor, ReflectedColor, fresnel + 0.3f);
	return vFinalColor;
}