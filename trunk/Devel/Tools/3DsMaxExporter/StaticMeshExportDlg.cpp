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

// StaticMeshExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XEvolMaxExporter.h"
#include "StaticMeshExportDlg.h"
#include "MeshPropertyDlg.h"
#include "StaticModelExport.h"
#include ".\staticmeshexportdlg.h"
#include "MeshPropertyDlg.h"
#include <math.h>
// CStaticMeshExportDlg dialog

IMPLEMENT_DYNAMIC(CStaticMeshExportDlg, CDialog)
CStaticMeshExportDlg::CStaticMeshExportDlg(MaxMeshs& meshs,CWnd* pParent /*=NULL*/)
	: CDialog(CStaticMeshExportDlg::IDD, pParent),m_MaxMeshs(meshs)
    , m_IsPackage(FALSE)
    , m_bIsTextMode(FALSE)
{
}

CStaticMeshExportDlg::~CStaticMeshExportDlg()
{
}

void CStaticMeshExportDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_IS_EXPORT_TO_PACKAGE, m_IsPackage);
    DDX_Control(pDX, IDT_MODEL_NAME, m_txtModelName);
    DDX_Check(pDX, IDC_IS_EXPORT_ASCII, m_bIsTextMode);
}


BEGIN_MESSAGE_MAP(CStaticMeshExportDlg, CDialog)
    ON_BN_CLICKED(IDC_MESH_PROP, OnBnClickedMeshProp)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CStaticMeshExportDlg message handlers

void CStaticMeshExportDlg::OnBnClickedMeshProp()
{
    // TODO: Add your control notification handler code here
   CMeshPropertyDlg mpdlg(m_MaxMeshs,this);
   mpdlg.DoModal();
}

BOOL CStaticMeshExportDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    char buf[256];
    sprintf(buf,"没名字的模型-%d",rand()%12000);
    // TODO:  Add extra initialization here
    m_txtModelName.SetWindowText(buf);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CStaticMeshExportDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
	WGetWndText(&m_txtModelName , m_model_name , 32);
    m_IsPackage   = IsDlgButtonChecked(IDC_IS_EXPORT_TO_PACKAGE);
    m_bIsTextMode = IsDlgButtonChecked(IDC_IS_EXPORT_ASCII);
    OnOK();
}
