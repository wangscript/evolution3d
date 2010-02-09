/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// WaterMeshDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MaxMesh.h"
#include "XEvolMaxExporter.h"
#include "WaterMeshDlg.h"
#include ".\watermeshdlg.h"
struct sWMeshDesc_t
{
	int   m_nFace;
	int   m_nVertex;
	float m_fUTile;
	float m_fVTile;

	float m_fUScale;
	float m_fUFactor;

	float m_fVScale;
	float m_fVFactor;
};
struct sWMeshVertex_t
{
	sVector_t  m_Pos;
	sUVCoord_t m_UV;
	sColor_t   m_Diff;
};

// CWaterMeshDlg dialog

IMPLEMENT_DYNAMIC(CWaterMeshDlg, CDialog)
CWaterMeshDlg::CWaterMeshDlg(INode* pNode ,  CWnd* pParent /*=NULL*/)
	: CDialog(CWaterMeshDlg::IDD, pParent)
	, m_fUTile(1.0f)
	, m_fVTile(1.0f)
{
    m_pNode = pNode;
	m_Mesh.Init(m_pNode , NULL);
	
}

CWaterMeshDlg::~CWaterMeshDlg()
{
	m_Mesh.Free();
}

void CWaterMeshDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UTILE, m_fUTile);
	DDX_Text(pDX, IDC_VTILE, m_fVTile);
}


BEGIN_MESSAGE_MAP(CWaterMeshDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CWaterMeshDlg message handlers

BOOL CWaterMeshDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    m_Mesh.BuildVertexData(NULL);
	// TODO:  Add extra initialization here
    m_Box = m_Mesh.m_BoundBox;

    return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWaterMeshDlg::OnBnClickedOk()
{

	xMeshData& meshData = m_Mesh;
	sWMeshDesc_t desc;
	UpdateData();
	desc.m_fVTile = m_fVTile;
	desc.m_fUTile = m_fUTile;

	// TODO: Add your control notification handler code here
	wchar_t file_name[256]={0};
	OPENFILENAMEW ofn; 
	ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.hwndOwner = ::GetActiveWindow();
	ofn.lpstrFile = file_name;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"wmesh模型文件(*.wmesh)\0*.wmesh\0所有文件(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.lpstrDefExt = L"wmesh";
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if(GetSaveFileNameW(&ofn) == FALSE)
	{
		OnCancel();
		return;
	}
	xcomdoc doc;
	DeleteFileW(file_name);
	doc.open(file_name,xcdm_write,0,true,false);
	xcomdocstream* pFaceStream = doc.create_stream(L"wmeshib");
	int totalFace = 0;
	xcomdocstream* pDescStream = doc.create_stream(L"wmesh");

	xcomdocstream* pVBStream = doc.create_stream(L"wmeshvb");

	for(size_t i = 0 ; i < meshData.m_vSubMeshs.size() ; ++i )
	{
		CMaxSubMesh& subMesh = meshData.m_vSubMeshs[i];
		sFace_t* index_buffer  = subMesh.m_Faces;
		int nFace = subMesh.m_nFace;
		totalFace += nFace;
		pFaceStream->write(index_buffer,nFace);
	}
	doc.close_stream(pFaceStream);
	desc.m_nFace = totalFace;

	


	
    for(size_t i = 0 ; i < meshData.m_VertexData.m_Positons.size() ; i++)
	{
		sWMeshVertex_t v;
		v.m_Pos = meshData.m_VertexData.m_Positons[i];
		v.m_UV  = meshData.m_VertexData.m_UVChannels[0][i];
		v.m_Diff= meshData.m_VertexData.m_Diffuses[i];
		pVBStream->write(v);
	}


	desc.m_nVertex = (int)meshData.m_VertexData.m_Positons.size();

	float len_y = m_Box.m_maxy - m_Box.m_miny;
	float len_x = m_Box.m_maxx - m_Box.m_minx;

	desc.m_fVTile = m_fVTile;
	desc.m_fUTile = m_fUTile;

	desc.m_fUScale = 1.0f/len_x;
	desc.m_fVScale = 1.0f/len_y;
	desc.m_fUFactor = m_Box.m_minx;
	desc.m_fVFactor = m_Box.m_miny;

	pDescStream->write(desc);

	G_MaxEnv().SaveLogToXComdoc(doc);
	doc.close();
	OnOK();
}

bool On_ExportWaterMesh(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	if(G_MaxEnv().m_pInterace->GetSelNodeCount() != 1)
	{
		MessageBoxA(GetActiveWindow(),"水波Mesh只能导出一个Mesh","错误!",MB_OK);
		return false;
	}
	CWaterMeshDlg dlg(G_MaxEnv().m_pInterace->GetSelNode(0));
	dlg.DoModal();
	return true;
}