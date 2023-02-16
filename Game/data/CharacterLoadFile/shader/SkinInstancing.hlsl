
#define MAX_OBJECT_COUNT 3
#define MAX_BONE_MATRIX_COUNT 40
#define MAX_TOTAL 120

Texture2D g_txDiffuse : register (t0);
Texture2D g_txNormalMap : register (t1);
//Texture2D g_txBlendA : register (t1);
//Texture2D g_txBlendB : register (t2);
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
	float4 g_Data01				:	packoffset(c3);	//toolMode���� ��� x��: ���ÿ�����Ʈ
	float4 g_MeshColor			:	packoffset(c4);	//ShadowColor
	matrix g_matWorldInverse	:	packoffset(c5);
};

cbuffer cb2 : register(b2)
{
	float4x4 m_matConstBoneWorld[MAX_TOTAL];
};

cbuffer cb3 : register(b3)
{
	float4x4 m_matNormalInverse[MAX_OBJECT_COUNT];
};

//float4x4 FetchboneTransform(uint iBone)
//{
//	float4x4 matrixreturn;
//	matrixreturn = m_matConstBoneWorld[iBone];
//
//	return matrixreturn;
//}

struct VS_OUTPUT
{
	float4 Position		: SV_POSITION; // ���ؽ� ������
	float4 Normal		: NORMAL;
	float4 Diffuse		: COLOR0;      // ���ؽ� ����
	float2 Texture		: TEXCOORD0;	//Texture ������
	float3 vLightDir	: TEXCOORD1;
	float4 MeshColor	: TEXCOORD2;	//ShadowColor
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
	float4 tan : TANGENT;
	uint Instanceld	: SV_InstanceID;
};

VS_OUTPUT VS(VS_INPUT vln)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	float4x4 matMatrix = (float4x4)0;
	float4 vLocal = mul(vln.p, g_matWorld);
	float3 Nor = vln.n.xyz;
	Nor = mul(Nor, (float3x3)g_matWorld);
	float3 NorResult = float3(0,0,0);


	//Data01.y : CheckParentData
	if ((uint)vln.w2.w != 0)
	{
		uint BoneIndex;
		BoneIndex = vln.Instanceld * (uint)MAX_BONE_MATRIX_COUNT;

		for (int iBipedCnt = 0; iBipedCnt < (uint)vln.w2.w; iBipedCnt++)
		{
			uint iBoneIndex = (uint)vln.i1[iBipedCnt];
			float fWeight = vln.w1[iBipedCnt];
			uint Index;

			if (iBipedCnt < 4)
			{
				Index = BoneIndex + iBoneIndex;
				matMatrix = m_matConstBoneWorld[Index];// FetchboneTransform(iBoneIndex);
				Output.Position += fWeight * mul(vLocal, matMatrix);
				NorResult += fWeight * mul(Nor, (float3x3)matMatrix);				
			}
			else
			{
				iBoneIndex = (uint)vln.i2[iBipedCnt - 4];
				fWeight = vln.w2[iBipedCnt - 4];

				Index = BoneIndex + iBoneIndex;
				matMatrix = m_matConstBoneWorld[Index]; //FetchboneTransform(iBoneIndex);
				Output.Position += fWeight * mul(vLocal, matMatrix);
				NorResult += fWeight * mul(Nor, (float3x3)matMatrix);
			}
		}
	}
	else
	{
		Output.Position = vLocal;
		NorResult = Nor;
	}


	Output.Position = mul(Output.Position, g_matInverse);	//������ ��ķ� ��� (�̻��� �������)

	Output.Normal = float4(normalize(NorResult), 1.0f);
	Output.Position = mul(Output.Position, g_matView);
	Output.Position = mul(Output.Position, g_matProj);
	Output.vLightDir = normalize(g_vLIghtDirection.xyz);

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
	Output.MeshColor = g_MeshColor;

	return Output;
}

VS_OUTPUT VS_NORMAL(VS_INPUT vln)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	float4 vLocal = mul(vln.p, g_matWorld);
	float3 Nor = vln.n.xyz;
	float3 NorResult = float3(0, 0, 0);
	float3 Tangent = vln.tan.xyz;
	float3 TanResult = float3(0, 0, 0);

	//Data01.y : CheckParentData
	if ((uint)vln.w2.w != 0)
	{
		float4x4 matMatrix;
		uint BoneIndex;
		BoneIndex = vln.Instanceld * (uint)MAX_BONE_MATRIX_COUNT;

		for (int iBipedCnt = 0; iBipedCnt < (uint)vln.w2.w; iBipedCnt++)
		{
			uint iBoneIndex = (uint)vln.i1[iBipedCnt];
			float fWeight = vln.w1[iBipedCnt];
			uint Index;

			if (iBipedCnt < 4)
			{
				Index = BoneIndex + iBoneIndex;
				matMatrix = m_matConstBoneWorld[Index]; //FetchboneTransform(iBoneIndex);
				Output.Position += fWeight * mul(vLocal, matMatrix);
				NorResult += fWeight * mul(Nor, (float3x3)matMatrix);
				TanResult += fWeight * mul(Tangent, (float3x3)matMatrix);
			}
			else
			{
				iBoneIndex = (uint)vln.i2[iBipedCnt - 4];
				fWeight = vln.w2[iBipedCnt - 4];

				Index = BoneIndex + iBoneIndex;
				matMatrix = m_matConstBoneWorld[Index]; //FetchboneTransform(iBoneIndex);
				Output.Position += fWeight * mul(vLocal, matMatrix);
				NorResult += fWeight * mul(Nor, (float3x3)matMatrix);
				TanResult += fWeight * mul(Tangent, (float3x3)matMatrix);
			}
		}
	}
	else
	{
		Output.Position = vLocal;
		NorResult = Nor;
		TanResult = Tangent;
	}

	Output.Position = mul(Output.Position, g_matInverse);
	Output.Position = mul(Output.Position, g_matView);
	Output.Position = mul(Output.Position, g_matProj);

	NorResult = mul(NorResult, (float3x3)g_matView);
	NorResult = mul(NorResult, (float3x3)g_matProj);

	TanResult = mul(TanResult, (float3x3)g_matView);
	TanResult = mul(TanResult, (float3x3)g_matProj);

	/*===================== �������� ��ȯ(�븻��) ======================*/
	float3 vNormal = normalize(mul(NorResult, (float3x3)m_matNormalInverse[vln.Instanceld]));
	float3 T = normalize(mul(TanResult, (float3x3)m_matNormalInverse[vln.Instanceld]));
	float3 B = normalize(cross(vNormal, T));

	float3x3 tanMat = { T.x, B.x, vNormal.x,
		T.y, B.y, vNormal.y,
		T.z, B.z, vNormal.z };

	Output.Normal = float4(NorResult, 1.0f);
	Output.vLightDir = normalize(mul(g_vLIghtDirection.xyz, tanMat));		//����Ʈ���� �������ͷ� ��ȯ

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
	Output.MeshColor = g_MeshColor;

	return Output;
}

float4 TPS(VS_OUTPUT input) : SV_Target
{
	//�⺻ �ؽ��� ��������
	float4 vTexColor;
	vTexColor = g_txDiffuse.Sample(sample0, input.Texture);

	//���� �÷�
	float4 vFinalColor;
	vFinalColor = vTexColor * input.Diffuse;
	vFinalColor.a = 1.0f;

	return vFinalColor;
}

float4 TPS_LIGHT(VS_OUTPUT input) : SV_Target
{
	float3 normal = float3(input.Normal.xyz);

	//�⺻ �ؽ��� ��������
	float4 vTexColor = g_txDiffuse.Sample(sample0, input.Texture);

	//���� ���
	float fdot;
	fdot = max(0.3f, dot(normal, -input.vLightDir));

	//���� �÷�
	float4 vFinalColor;
	vFinalColor = vTexColor * float4(fdot, fdot, fdot, 1.0f);
	vFinalColor.a = 1.0f;

	return vFinalColor;
}

float4 TPS_NORMAL(VS_OUTPUT input) : SV_Target
{
	//�⺻ �ؽ��� ��������
	float4 vTexColor = g_txDiffuse.Sample(sample0, input.Texture);

	//�븻�� �븻 ��������
	float4 DiffuseNormal = g_txNormalMap.Sample(sample0, input.Texture);
	DiffuseNormal = (DiffuseNormal - 0.5f) * 2.0f;

	//���� ���
	float fdot;
	fdot = max(0.5f, dot(DiffuseNormal.xyz, -input.vLightDir.xyz));

	//���� �÷�
	float4 vFinalColor;
	vFinalColor = vTexColor * float4(fdot, fdot, fdot, 1.0f);
	vFinalColor.a = 1.0f;

	return vFinalColor;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	//float4 vLight = float4(500, 200, 0, 1);
	//float fdot = dot(normalize(input.Normal), normalize(vLight));

	//input.Diffuse = input.Diffuse * fdot;
	input.Diffuse.w = 1.0f;

	return  input.Diffuse;
}

float4 PS_COLOR(VS_OUTPUT input) : SV_Target
{
	return input.MeshColor;
}
