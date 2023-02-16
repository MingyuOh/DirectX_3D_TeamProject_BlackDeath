#pragma once
#include "BObject.h"

struct BMapDesc
{
	int				iNumCols;
	int				iNumRows;
	int				iCellCount;
	int				iPatchCount;
	float			fCellDistance;
	float			fScaleHeight;
	basic_string<TCHAR>	strTextureFile;
	basic_string<TCHAR>	strShaderFile;
	BMapDesc() { iNumCols = 0; iNumRows = 0; iCellCount = 0; iPatchCount = 0; fCellDistance = 0.0f; fScaleHeight = 1.0f; }
};

class BMap : public BObject
{
public:
	BMapDesc m_MapDesc;
	vector<TVector3> m_VertexArray;
	vector<float>		m_fHeightList;
	TCHAR* m_MapTexture;

	float m_iNumRows; // 가로 버텍스 (행)
	float m_iNumCols; // 세로 버텍스 (열)
	float m_iNumCellRows; //가로 셀의 갯수
	float m_iNumCellCols; //세로 셀의 갯수
	int m_fCellDistance;// 셀 하나의 길이 (정방형이므로 가로 세로 동일)

	ID3D11PixelShader* m_pAlphaPS; // 알파맵 픽셀 쉐이더

	bool m_bStaticLight;
	TVector3 m_vLightDir;

public:
	virtual bool Frame();
	bool CreateHeightMap(TCHAR* strHeightMapText);
	virtual HRESULT CreateVertexBuffer();		// 버텍스 버퍼 생성
	virtual HRESULT CreateIndexBuffer();		// 인덱스 버퍼 생성
	bool SetMapDesc(BMapDesc MapDesc);
	HRESULT Create(ID3D11Device* pDevice, ID3D11DeviceContext* m_pImmediateContext, TCHAR** pLoadTextureString, int TextureCnt, TCHAR* HeightMapString = NULL, const TCHAR* ShaderString = NULL);
	virtual float GetHeightMap(float row, float col);
	virtual float GetHeightMap(int row, int col);
	float Lerp(float fStart, float fEnd, float fTangent);
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);

	/*정점노말 관련 함수*/
	bool UpdateNormal();
	virtual TVector3 GetNormalOfVertex(UINT Index);
	void CalcVertexColor();
	bool ReLoadVBuffer();


public:
	BMap();
	~BMap();
};

