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

// SkeletonSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "XEvolMaxExporter.h"
#include "SkeletonSetDlg.h"
#include ".\skeletonsetdlg.h"
#include "NodeViewDlg.h"

#include "SkeletonExp.h"
#include "MaxMesh.h"
// CSkeletonSetDlg dialog

IMPLEMENT_DYNAMIC(CSkeletonSetDlg, CDialog)
CSkeletonSetDlg::CSkeletonSetDlg(float fScale,CWnd* pParent /*=NULL*/)
	: CDialog(CSkeletonSetDlg::IDD, pParent)
    ,m_MaxNodeTree(&m_TreeNodes),
	m_fScale(fScale)
{
	m_pSkeleton = NULL;
}
void CSkeletonSetDlg::OnBnClickedCancel()
{
    OnCancel();
}
CSkeletonSetDlg::~CSkeletonSetDlg()
{
}

void CSkeletonSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeNodes);
	DDX_Text(pDX, IDC_FMODELSCALE, m_fScale);
}


BEGIN_MESSAGE_MAP(CSkeletonSetDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_VIEW_GROUP, OnBnClickedViewGroup)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedSaveSkeleton)
	ON_BN_CLICKED(IDC_ALL_BONES, OnBnClickedAllBones)
	ON_BN_CLICKED(IDC_ADD_BONES, &CSkeletonSetDlg::OnBnClickedAddBones)
END_MESSAGE_MAP()


// CSkeletonSetDlg message handlers

void CSkeletonSetDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    OnOK();
}



void CSkeletonSetDlg::BuildNodeTree(sMaxNodes_t& nodes)
{
    m_MaxNodeTree.BuildScene(nodes);
}
void CSkeletonSetDlg::BuildRootNodeTree()
{
    m_MaxNodeTree.BuildRootScene();
}

void CSkeletonSetDlg::BuildSelNodeTree()
{
    m_MaxNodeTree.BuildSelectTree();
}

void CSkeletonSetDlg::InsertNode(HTREEITEM parentItem , CMaxNodeTreeItem& Node)
{
    INode* pNode = Node.m_pMaxNode;

    int type = IMAGE_UNKNOWN;
    if(G_MaxEnv().IsBone(pNode) )
    {
        type = IMAGE_BONE;
    }
    if(G_MaxEnv().IsPureDummy(pNode))
    {
        type = IMAGE_DUMMY;
    }
    if(G_MaxEnv().IsMesh(pNode))
    {
        type = IMAGE_MESH;
    }

    Node.m_TreeItem = m_TreeNodes.InsertItem( INodeNameA(pNode),IMAGE_BONE,IMAGE_BONE,parentItem);
    m_TreeNodes.SetCheck(Node.m_TreeItem,TRUE);
    for(size_t i = 0 ; i < Node.m_ChildNodes.size(); i ++)
    {
        InsertNode(Node.m_TreeItem,Node.m_ChildNodes[i]);
    }
    m_TreeNodes.Expand(parentItem,TVE_EXPAND);
}

void CSkeletonSetDlg::ReleaseTree()
{ 
    m_MaxNodeTree.Clear();
}
void CSkeletonSetDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}
BOOL CSkeletonSetDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_TreeImages.Create(IDB_NODE_TYPE_BITMAP,16,8,0xffffffff);
    m_TreeNodes.SetImageList(&m_TreeImages,TVSIL_NORMAL);
    size_t nSize = m_MaxNodeTree.m_MaxNodeTreeRoot.size();
    for(size_t i =0 ; i < nSize; i++)
    {
        InsertNode(TVI_ROOT,m_MaxNodeTree.m_MaxNodeTreeRoot[i]);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CSkeletonSetDlg::OnBnClickedViewGroup()
{

	sMaxNodes_t bones;
	m_MaxNodeTree.buildFinalSelNodes(bones);
	CSetBoneGroupDlg SBCD(this);
	SBCD.SetContexs(&bones);
	SBCD.DoModal();
	// TODO: Add your control notification handler code here
}

void CSkeletonSetDlg::OnBnClickedSaveSkeleton()
{
	UpdateData();
	wchar_t skeleton_file[128];
	skeleton_file[0] = 0;

	if(G_MaxEnv().GetSaveFileName(skeleton_file,
		L"XEvol骨骼文件(*.skeleton)\0*.skeleton\0XEvol模型文件(*.xrm)\0*.xrm\0所有文件(*.*)\0*.*\0",
		L"skeleton",
		L"请选择一个骨架文件") == false)
	{
		return ;
	}

	CSkeletonExporter skel_exporter;
	sMaxNodes_t bones;
	m_MaxNodeTree.buildFinalSelNodes(bones);

    CSkeletonExporter* pSkeleton = &skel_exporter;
	if(m_pSkeleton)
	{
		pSkeleton = m_pSkeleton;
	}
	else
	{
		skel_exporter.AddSkinModifiers(bones);
		skel_exporter.push_bones(bones);
		skel_exporter.m_fScale = 1.0f/m_fScale;
	}


	xcomdoc skeleton_doc;
	skeleton_doc.open(skeleton_file,xcdm_rw,0,true,false);
	if(skeleton_doc.failed())
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"打开骨架文件失败\r\n");
		skeleton_doc.close();
		return ;
	}

	xcomdocstream* pstream = skeleton_doc.open_stream(L"skeleton/skeleton",false);
	if(pstream == NULL)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"打开骨架文件失败[skeleton/skeleton流不存在]\r\n");
		skeleton_doc.close();
		return ;
	}
	pSkeleton->buildHiberarchys();
	pSkeleton->save_skeleton(skeleton_doc);
	G_MaxEnv().SaveLogToXComdoc(skeleton_doc);
	skeleton_doc.close();
	return ;
}

void CSkeletonSetDlg::OnBnClickedAllBones()
{
	// TODO: Add your control notification handler code here
	m_MaxNodeTree.Clear();
	sMaxNodes_t nodes;
	GetNodesFromScene(nodes);
	sMaxNodes_t bones;
	GetBonesFromNodes(bones,nodes);
	m_MaxNodeTree.BuildScene(bones);

	m_TreeNodes.DeleteAllItems();
	size_t nSize = m_MaxNodeTree.m_MaxNodeTreeRoot.size();
	for(size_t i =0 ; i < nSize; i++)
	{
		InsertNode(TVI_ROOT,m_MaxNodeTree.m_MaxNodeTreeRoot[i]);
	}

	//把新的骨头加到这个骨架中
	if(m_pSkeleton)
	{
		m_pSkeleton->push_bones(bones);
	}

}

void CSkeletonSetDlg::OnBnClickedAddBones()
{
	// TODO: Add your control notification handler code here
	CNodeViewDlg dlg;
	dlg.BuildRootNodeTree();
	if(IDOK == dlg.DoModal() )
	{  
		sMaxNodes_t bones;
		dlg.getBuiledNodeTree().buildFinalSelNodes(bones);
		for(sMaxNodes_t::iterator i = bones.begin() ; i != bones.end() ; i++)
		{
			if(*i == G_MaxEnv().m_pInterace->GetRootNode())
			{
				bones.erase(i);
				break;
			}
		}

		for(size_t i =0 ; i < bones.size() ; i++)
		{
			if(G_MaxEnv().m_pInterace->GetRootNode() != bones[i] )
			   G_MaxEnv().SetXEvol3DBone(bones[i],TRUE);
		}

		m_MaxNodeTree.BuildScene(bones);
		m_TreeNodes.DeleteAllItems();
		size_t nSize = m_MaxNodeTree.m_MaxNodeTreeRoot.size();
		for(size_t i =0 ; i < nSize; i++)
		{
			InsertNode(TVI_ROOT,m_MaxNodeTree.m_MaxNodeTreeRoot[i]);
		}

		//把新的骨头加到这个骨架中
		if(m_pSkeleton)
		{
			m_pSkeleton->push_bones(bones);
		}
	}
}
