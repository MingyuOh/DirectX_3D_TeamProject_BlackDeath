#pragma once
#include "K_Object.h"
class K_Plane : public K_Object
{
public:
	bool CreateVertexBuffer();
	bool CreateIndexBuffer();
public:
	bool Render(int Num);
public:
	K_Plane();
	~K_Plane();
};

