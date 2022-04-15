#pragma once

#include "LAObject.h"

class LABox : public LAObject
{
public:
	HRESULT CreateVertexBuffer();		//버텍스 버퍼 생성
	HRESULT CreateIndexBuffer();		//인덱스 버퍼 생성
	void SetColor(float r, float g, float b, float alpha);
public:
	LABox();
	virtual ~LABox();
};

