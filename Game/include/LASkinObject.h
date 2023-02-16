#pragma once

#include "LAObject2.h"
#include "LAMesh.h"

class LAMeshData;
class LASkinObject : public LAObject2
{
public:
	LAMesh* m_MeshData;
	//Type 기본값 GeomObject
	D3DXMATRIX* m_pmatParent;
	D3DXMATRIX* m_pmatCharacter;
	D3DXMATRIX m_matChildWorld;
	D3DXMATRIX m_matCalculation;
	D3DXMATRIX m_matBonObjectCalculation;

	D3DXMATRIX m_matObjectLocalPos;
	D3DXMATRIX m_matObjectLocalScale;
	D3DXMATRIX m_matObjectLocalRotation;

	//CBConstBoneWorld* m_cbMatrixListData;
	D3DXMATRIX m_cbMatrixListData[MAX_BONE_MATRICES];
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBConstBoneBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pInstancingAnimationBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pInstancingNormalBuffer;
	//Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBHSTessellation;

	D3DXMATRIX m_matShadow;

	bool m_bSetParentTick;
	int m_iParentTick;
	int m_SkinTick;

	int m_iCurrentMotion;
	int m_ParentObjectTick;

	bool m_bisRander;
	bool m_SelectObject;
	bool m_bIsCreate;

public:
	//================================================================================
	//	** 클래스 함수 ** 
	//================================================================================
	void DrawMesh();
	bool Interpolate(D3DXMATRIX& TargetMatrix, int iCurTick);
	bool CreateNormalMap(std::wstring fileName);
	bool DeleteNormalMap();
	bool CreateLight();
	bool CreateNormalTexture(std::wstring fileName);
	bool DeleteNormalTexture();
	bool CreateNormalMapVertexBuffer();
	bool LoadNormalShader();
	bool LoadLightShader();
	bool LoadShadowShader();
	bool InstancingRender(InstancingDataInfo& rInstancingData);
	bool InstancingShadowRender(InstancingDataInfo& rInstancingData);
	void CreateShadowMatrix();
	//Set
	void SetCrrentTick(bool check);	
	bool SetAnimationTrack(float fCurTick);
	void SetBoneMatrices(CBConstBoneWorld* pCBboneWorldDataList);
	void SetSelectObject(bool bcheck);
	void SetDirectionLight(D3DXVECTOR4 vCameraPos, D3DXVECTOR4 vLigthPos, D3DXVECTOR4 vLightDirection);
	void CreateTangent(LAMeshData* pMeshData);
	void CreateTangentSpaceVectors(D3DXVECTOR3 *v0,	D3DXVECTOR3 *v1, D3DXVECTOR3 *v2, D3DXVECTOR2 uv0,
		D3DXVECTOR2 uv1, D3DXVECTOR2 uv2, D3DXVECTOR3 *vTangent, D3DXVECTOR3 *vBiNormal, D3DXVECTOR3 *vNormal);
	void SetBoneObjectCalculation(D3DXMATRIX matWorld);
	//Get
	bool GetAnimationTrack(std::vector<AnimationTrack>& TrackList, int& NumstartTrack, int& NumEndTrack, float fCurTick);
	

	//================================================================================
	//	** 정점 노말 연산 함수 **
	//================================================================================
	void UpDateNormals();
	void InitFaceNormals(std::vector<D3DXVECTOR3>& rSaveFaceNormalList, int iFaceSize, std::vector<PNCT5_VERTEX>& rVertexList, std::vector<DWORD>& rIndexList);
	void GenNormalLookupTable(std::vector<int>& rNormalLookUpTable, int iFaceSize, std::vector<DWORD>& rIndexList);
	void CalcPerVertexNormalsFastLookup(std::vector<PNCT5_VERTEX>& rSaveNormalVertexList, std::vector<D3DXVECTOR3>& rFaceNormal, std::vector<int>& rNormalLookUpTable);
public:
	//================================================================================
	//	** 재정의 함수 ** 
	//================================================================================
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();
	HRESULT LoadTextureResource();
	HRESULT CreateLayout();
	HRESULT LoadShader();
	bool Frame();
	bool Release();
	bool Render();
	bool RenderShadow();
public:
	LASkinObject()
	{
		m_pmatCharacter = NULL;
		m_bSetParentTick = false;
		m_MeshData = NULL;
		m_pmatParent = NULL;
		m_SelectObject = false;
		m_bisRander = true;
		m_iCurrentMotion = 0;
		m_ParentObjectTick = 0;
		m_iParentTick = 0;
		m_bIsCreate = false;

		D3DXMatrixIdentity(&m_matBonObjectCalculation);

	}
	virtual ~LASkinObject();
};

