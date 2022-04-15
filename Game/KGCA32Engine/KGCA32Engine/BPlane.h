#pragma once
#include "BObject.h"

class BPlane : public BObject
{
public:
	float fA, fB, fC, fD; // ����� �����Ŀ� ����

public:
	virtual HRESULT CreateVertexBuffer();		// ���ؽ� ���� ����
	virtual HRESULT CreateIndexBuffer();		// �ε��� ���� ����

	/*���ν��ҿ� �Լ�*/
	bool CreatePlane(TVector3 v0, TVector3 v1, TVector3 v2);	// ����� �������� ��� �Լ�
	bool CreatePlane(TVector3 vNormal, TVector3 v0);				// ����� �������� ��� �Լ�
	void UpdateVertexMinMax();
public:
	BPlane();
	~BPlane();
};

