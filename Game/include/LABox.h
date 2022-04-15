#pragma once

#include "LAObject.h"

class LABox : public LAObject
{
public:
	HRESULT CreateVertexBuffer();		//���ؽ� ���� ����
	HRESULT CreateIndexBuffer();		//�ε��� ���� ����
	void SetColor(float r, float g, float b, float alpha);
public:
	LABox();
	virtual ~LABox();
};

