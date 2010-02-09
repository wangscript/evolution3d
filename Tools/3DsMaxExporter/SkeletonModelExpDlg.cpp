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

// SkeletonModelExpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XEvolMaxExporter.h"
#include "SkeletonModelExpDlg.h"
#include ".\skeletonmodelexpdlg.h"
#include "SkeletonActionExpDlg.h"
#include "SkeletonSetDlg.h"

// CSkeletonModelExpDlg dialog

IMPLEMENT_DYNAMIC(CSkeletonModelExpDlg, CDialog)
CSkeletonModelExpDlg::CSkeletonModelExpDlg(sSkeletonModelExpCtx_t& expContex,CWnd* pParent /*=NULL*/)
	: CDialog(CSkeletonModelExpDlg::IDD, pParent)
    , m_fScale(1.0)
    ,m_ExpContex(expContex)
    , m_bTextMode(FALSE)
{
}

CSkeletonModelExpDlg::~CSkeletonModelExpDlg()
{
}

void CSkeletonModelExpDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDT_MODEL_NAME, m_ctrlModelName);
    DDX_Text(pDX, IDC_SCALE_FACTOR, m_fScale);
    DDX_Check(pDX, IDC_EXPTEXTMODE, m_bTextMode);
}


BEGIN_MESSAGE_MAP(CSkeletonModelExpDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDB_EXPORT, OnBnClickedExport)
    ON_BN_CLICKED(IDB_ACTION_SET, OnBnClickedActionSet)
	ON_BN_CLICKED(IDB_SET_SKELETON, OnBnClickedSetSkeleton)
END_MESSAGE_MAP()


// CSkeletonModelExpDlg message handlers

void CSkeletonModelExpDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    OnOK();
}

void CSkeletonModelExpDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}

void CSkeletonModelExpDlg::OnBnClickedExport()
{
    // TODO: Add your control notification handler code here
    UpdateData();
    m_ExpContex.m_fModelScale = 1.0/m_fScale;

    m_ExpContex.m_bTextMode = m_bTextMode != FALSE;
	WGetWndText(&m_ctrlModelName , m_ExpContex.m_ModelDesc.m_Name,32);
    m_ExpContex.m_ModelDesc.m_Name[31] = 0;
    OnOK();
}

void CSkeletonModelExpDlg::OnBnClickedActionSet()
{
    // TODO: Add your control notification handler code here
    CSkeletonActionExpDlg actionDlg(m_ExpContex.m_Actions);
    actionDlg.DoModal();
}

void CSkeletonModelExpDlg::OnBnClickedSetSkeleton()
{
	sMaxNodes_t bones;
	m_ExpContex.m_pSkeleton->get_bone_nodes(bones);

	UpdateData();

	CSkeletonSetDlg SSDlg(m_fScale);
	SSDlg.setSkeleton(m_ExpContex.m_pSkeleton);
	SSDlg.BuildNodeTree(bones);
	SSDlg.DoModal();
    
	//sMaxNodes_t newbones;
	//SSDlg.m_MaxNodeTree.buildFinalSelNodes(newbones);
	//m_ExpContex.m_pSkeleton->build_modifiers(newbones);
	//m_ExpContex.m_pSkeleton->push_bones(newbones);
	// TODO: Add your control notification handler code here
}
