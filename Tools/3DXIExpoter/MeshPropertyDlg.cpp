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

// MeshPropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XEvolMaxExporter.h"
#include "MeshPropertyDlg.h"


// CMeshPropertyDlg dialog

IMPLEMENT_DYNAMIC(CMeshPropertyDlg, CDialog)
CMeshPropertyDlg::CMeshPropertyDlg(MaxMeshs& meshs,CWnd* pParent /*=NULL*/)
	: CDialog(CMeshPropertyDlg::IDD, pParent)
    , m_bHasNormal(TRUE)
    , m_bHasDiffuse(TRUE)
    , m_bHasTangentSpace(FALSE)
    , m_nUVLayers(1)
    ,m_MaxMeshs(meshs)
{
}

CMeshPropertyDlg::~CMeshPropertyDlg()
{
}

void CMeshPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_HASNORMAL, m_bHasNormal);
    DDX_Check(pDX, IDC_HASDIFFUSE, m_bHasDiffuse);
    DDX_Check(pDX, IDC_HASTANGETSPACE, m_bHasTangentSpace);
    DDX_Text(pDX, IDT_UVLAYER, m_nUVLayers);
    DDV_MinMaxUInt(pDX, m_nUVLayers, 1, 8);
    DDX_Control(pDX, IDC_MESHLIST, m_MeshListCtrl);
}


BEGIN_MESSAGE_MAP(CMeshPropertyDlg, CDialog)
END_MESSAGE_MAP()


// CMeshPropertyDlg message handlers
