
#define MAX_BONE_MATRIX_COUNT 255

Texture2D g_txDiffuse : register (t0);
Texture2D g_txBlendA : register (t1);
Texture2D g_txBlendB : register (t2);
SamplerState sample0 : register (s0);

cbuffer cb0 : register(b0)
{
	matrix g_matWorld	:	packoffset(c0);
	matrix g_matView	:	packoffset(c4);
	matrix g_matProj	:	packoffset(c8);
	matrix g_matInverse :	packoffset(c12);
};

//cbuffer cb1 : register(b1)
//{
//	float4 g_DataA	:	packoffset(c0);
//	float4 g_DataB	:	packoffset(c1);
//	float4 cb_vEyePosition	:	packoffset(c2);
//};

cbuffer cb1 : register(b1)
{
	float4 g_vLightPos			:	packoffset(c0);
	float4 g_vLIghtDirection	:	packoffset(c1);
	float4 g_vEyePosition		:	packoffset(c2);
	float4 g_Data01				:	packoffset(c3);	//toolMode에서 사용 x값: 선택오브젝트
	matrix g_matWorldInverse	:	packoffset(c4);
};

cbuffer cb2 : register(b2)
{
	float4x4 m_matConstBoneWorld[MAX_BONE_MATRIX_COUNT];

};

float4x4 FetchboneTransform(uint iBone)
{
	float4x4 matrixreturn;
	matrixreturn = m_matConstBoneWorld[iBone];

	return matrixreturn;
}

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
	float2 t : TEXCOORD0;
	float4 i1 : TEXCOORD1;
	float4 w1 : TEXCOORD2;
	float4 i2 : TEXCOORD3;
	float4 w2 : TEXCOORD4;
};

VS_OUTPUT VS(VS_INPUT vln)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	float4x4 matMatrix;
	float4 vLocal = mul(vln.p, g_matWorld);
	vLocal.w = 1.0f;

	//float4 Nor = vln.n;
	float3 Nor = float3(vln.n.x, vln.n.y, vln.n.z);
	Nor = mul(Nor, (float3x3)g_matWorld);
	float3 NorResult = float3(0,0,0);//	mul(vln.n, g_matWorld);

	//Data01.y : CheckParentData
	if ((uint)vln.w2.w != 0)// && ((uint)g_Data01.y != 0))
	{
		for (int iBipedCnt = 0; iBipedCnt < (uint)vln.w2.w; iBipedCnt++)
		{
			uint iBoneIndex = (uint)vln.i1[iBipedCnt];
			float fWeight = vln.w1[iBipedCnt];

			if (iBipedCnt < 4)
			{
				matMatrix = FetchboneTransform(iBoneIndex);
				Output.Position += fWeight * mul(vLocal, matMatrix);
				NorResult += fWeight * mul(Nor, (float3x3)matMatrix);
			}
			else
			{
				iBoneIndex = (uint)vln.i2[iBipedCnt - 4];
				fWeight = vln.w2[iBipedCnt - 4];

				matMatrix = FetchboneTransform(iBoneIndex);
				Output.Position += fWeight * mul(vLocal, matMatrix);
				NorResult += fWeight * mul(Nor, (float3x3)matMatrix);
			}
		}
	}
	else
	{
		Output.Position = mul(vln.p, g_matWorld);
		Output.Position.w = 1.0f;
		//Output.Normal = vln.n;
		NorResult = normalize(mul(Nor, (float3x3)g_matWorld));
	}
	//Output.Position = mul(Output.Position, g_matWorld);
	//Output.Normal = normalize(mul(Output.Normal, g_matWorld));
	NorResult = normalize(NorResult);
	Output.Normal = float4(NorResult, 1.0f);

	Output.Position = mul(Output.Position, g_matView);
	Output.Position = mul(Output.Position, g_matProj);


	//x : CheckSelectObject
	if ((uint)g_Data01.x == 1)
	{
		Output.Diffuse = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		Output.Diffuse = vln.c;
	}

	Output.Texture = vln.t;

	return Output;
}

float4 TPS(VS_OUTPUT input) : SV_Target
{
	//float4 vdirection = input.Position - g_DataA;
	float4 vLight = float4(500, 200, 0, 1);
	float3 normal = float3(input.Normal.x, input.Normal.y, input.Normal.z);
	float3 lightpos = float3(g_vLightPos.x, g_vLightPos.y, g_vLightPos.z);

	float fdot = max(0.3f, dot(normalize(normal), normalize(lightpos)));

	float4 vTexColor = g_txDiffuse.Sample(sample0, input.Texture);
	//float4 vFinalColor = vTexColor * fdot;// input.Diffuse * fdot;// max(0.2, fdot);
	//input.Diffuse = input.Diffuse * fdot;
	//input.Diffuse.a = 1.0f;
	//float4 vFinalColor = vTexColor;// *g_DataA;// fdot;

	float4 vFinalColor;
	//normalize(g_vLightPos);
	vFinalColor = vTexColor * float4(fdot, fdot, fdot, 1.0f);
	vFinalColor.a = 1.0f;
	return vFinalColor;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 vLight = float4(500, 200, 0, 1);
	float fdot = dot(normalize(input.Normal), normalize(vLight));

	input.Diffuse = input.Diffuse * fdot;
	input.Diffuse.w = 1.0f;

	return  input.Diffuse;
}
