#include "MapExporter.h"



bool MapExporter::SetExporter(ID3D11DeviceContext* pContext, const TCHAR* m_szFileName)
{
	m_pContext = pContext;
	m_pStream = _tfopen(m_szFileName, _T("wt"));
	if (!m_pStream)
	{
		return false;
	}
	return true;
}

void MapExporter::ExpAlphaMapTex(ID3D11Texture2D* pTextureDest)
{
	D3DX11SaveTextureToFile(m_pContext, pTextureDest, D3DX11_IFF_BMP, L"../../data/MapTool_Alpha.bmp");
}

void MapExporter::ExpTexName(TCHAR* BaseTexName, map<int, TCHAR*> pSplattingTexName)
{
	_ftprintf(m_pStream, _T("%s  "), _T("*AlphaTexture"));
	_ftprintf(m_pStream, _T("%s\n"), _T("../../data/MapTool_Alpha.bmp"));

	_ftprintf(m_pStream, _T("%s  "), _T("*BaseTexture"));
	_ftprintf(m_pStream, _T("%s\n"), BaseTexName);
	
	_ftprintf(m_pStream, _T("%s\n"), _T("*SplattingTexture"));
	_ftprintf(m_pStream, _T("%s\t%s\n"), _T("*Red"), pSplattingTexName[0]);
	_ftprintf(m_pStream, _T("%s\t%s\n"), _T("*Green"), pSplattingTexName[1]);
	_ftprintf(m_pStream, _T("%s\t%s\n"), _T("*Blue"), pSplattingTexName[2]);
	_ftprintf(m_pStream, _T("%s\t%s\n"), _T("*Alpha"), pSplattingTexName[3]);
}

void MapExporter::ExpMapInfo(BMapDesc* pMapInfo)
{
	_ftprintf(m_pStream, _T("%s\t%d\n"), _T("*CellCount"), pMapInfo->iCellCount);
	_ftprintf(m_pStream, _T("%s\t%d\n"), _T("*PatchCount"), pMapInfo->iPatchCount);
	_ftprintf(m_pStream, _T("%s\t%10.4f\n"), _T("*CellDistance"), pMapInfo->fCellDistance);
}

void MapExporter::ExpHeightTotal(BNode* RootNode)
{
	_ftprintf(m_pStream, _T("%s\t\n"), _T("*Height"));
	ExpHeight(RootNode);
}
void MapExporter::ExpHeight(BNode* Node)
{
	if (Node->m_pChild[0] != NULL)
	{
		for (int iNode = 0; iNode < 4; iNode++)
		{
			ExpHeight(Node->m_pChild[iNode]);
		}
	}
	else
	{
		if (Node->m_VertexList.size())
		{
			// m_tBox 출력순서 = Min xyz, Max xyz, Center xyz, fExtent 0, 1, 2
			_ftprintf(m_pStream, _T("%f %f %f\n"), Node->m_tBox.vMin.x, Node->m_tBox.vMin.y, Node->m_tBox.vMin.z);
			_ftprintf(m_pStream, _T("%f %f %f\n"), Node->m_tBox.vMax.x, Node->m_tBox.vMax.y, Node->m_tBox.vMax.z);
			_ftprintf(m_pStream, _T("%f %f %f\n"), Node->m_tBox.vCenter.x, Node->m_tBox.vCenter.y, Node->m_tBox.vCenter.z);
			_ftprintf(m_pStream, _T("%f %f %f\n"), Node->m_tBox.fExtent[0], Node->m_tBox.fExtent[1], Node->m_tBox.fExtent[2]);
			for (int iVer = 0; iVer < Node->m_VertexList.size(); iVer++)
			{
				_ftprintf(m_pStream, _T("%f\n"), Node->m_VertexList[iVer].Position.y);
			}
			_ftprintf(m_pStream, _T("\n"));
		}
	}
}



MapExporter::MapExporter()
{
}


MapExporter::~MapExporter()
{
}
