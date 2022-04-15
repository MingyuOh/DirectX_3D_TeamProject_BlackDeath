#pragma once
#include "BObject.h"

class BBox : public BObject
{
public:
	virtual HRESULT CreateVertexBuffer();		// 버텍스 버퍼 생성
	virtual HRESULT CreateIndexBuffer();		// 인덱스 버퍼 생성
	virtual void UpdateVertexMinMax();
public:
	BBox();
	~BBox();
};

