#include "BSkyObj.h"

//TCHAR* skyTextture[]
//{
//	L"../../data/st00_cm_front.bmp",
//	L"../../data/st00_cm_up.bmp",
//	L"../../data/st00_cm_right.bmp",
//	L"../../data/st00_cm_down.bmp",
//	L"../../data/st00_cm_back.bmp",
//	L"../../data/st00_cm_left.bmp",
//};


void BSkyObj::SetMatrix(TMatrix World, TMatrix View, TMatrix Proj)
{
	//if (World != NULL)
	{
		m_matWorld = World;
		m_tBox.vCenter.x = World._41;
		m_tBox.vCenter.y = World._42;
		m_tBox.vCenter.z = World._43;
	}
	m_matView = View;
	m_matView._41 = 0.0f;
	m_matView._42 = 0.0f;
	m_matView._43 = 0.0f;
	m_matView._44 = 1.0f;

	//if (Proj != NULL)
	{
		m_matProj = Proj;
	}
	D3DXMatrixTranspose(&m_matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_matView, &m_matView);
	D3DXMatrixTranspose(&m_matProj, &m_matProj);

	m_cbData.m_matWorld = m_matWorld;
	m_cbData.m_matView = m_matView;
	m_cbData.m_matProj = m_matProj;

	D3DXMatrixTranspose(&m_matWorld, &m_matWorld);
}

BSkyObj::BSkyObj()
{
}


BSkyObj::~BSkyObj()
{
}
