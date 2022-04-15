#pragma once
#include "BPlane.h"

class BNPlane : public BPlane
{
public:
	HRESULT LoadShaderAndInputLayout(const TCHAR* pShaderFile);
public:
	BNPlane();
	~BNPlane();
};

