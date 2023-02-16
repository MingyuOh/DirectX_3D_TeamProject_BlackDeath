#pragma once

#include "LAObject2.h"

class LABox2 : public LAObject2
{
public:
	HRESULT CreateVertexBuffer();		//버텍스 버퍼 생성
	HRESULT CreateIndexBuffer();		//인덱스 버퍼 생성
	HRESULT Create(CrateObjectDest& ObjectDest);
	bool Render();
	void SetColor(float r, float g, float b, float alpha);
public:
	LABox2();
	virtual ~LABox2();
};

