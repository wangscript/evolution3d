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

// NodeViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "XEvolMaxExporter.h"
#include "NodeViewDlg.h"
#include "MaxNodeTree.h"
#include "IGame.h"
#include <IGameObject.h> 
#include ".\nodeviewdlg.h"

// CNodeViewDlg 对话框

IMPLEMENT_DYNAMIC(CNodeViewDlg, CDialog)
CNodeViewDlg::CNodeViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNodeViewDlg::IDD, pParent),
    m_MaxNodeTree(&m_TreeNodes)
{
	
}

CNodeViewDlg::~CNodeViewDlg()
{
}

void CNodeViewDlg::BuildRootNodeTree()
{
    m_MaxNodeTree.BuildRootScene();
}

void CNodeViewDlg::BuildSelNodeTree()
{
    m_MaxNodeTree.BuildSelectTree();
}

void CNodeViewDlg::InsertNode(HTREEITEM parentItem , CMaxNodeTreeItem& Node)
{
	INode* pNode = Node.m_pMaxNode;

    int type = IMAGE_UNKNOWN;
    if(CMaxEnv::IsMesh(pNode))
    {
        type = IMAGE_MESH;
    }

    if(CMaxEnv::IsBone(pNode) )
    {
        type = IMAGE_BONE;
    }
    if(CMaxEnv::IsPureDummy(pNode))
    {
        type = IMAGE_DUMMY;
    }
    Node.m_TreeItem = m_TreeNodes.InsertItem(INodeNameA(pNode),type,type,parentItem);
    m_TreeNodes.SetCheck(Node.m_TreeItem,TRUE);
    for(size_t i = 0 ; i < Node.m_ChildNodes.size(); i ++)
	{
       InsertNode(Node.m_TreeItem,Node.m_ChildNodes[i]);
	}
    m_TreeNodes.Expand(parentItem,TVE_EXPAND);
}

void CNodeViewDlg::ReleaseTree()
{ 
	m_MaxNodeTree.Clear();
}

void CNodeViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NODE_TREE, m_TreeNodes);
}


BEGIN_MESSAGE_MAP(CNodeViewDlg, CDialog)
    ON_NOTIFY(TVN_SELCHANGED, IDC_NODE_TREE, OnTvnSelchangedNodeTree)
    ON_NOTIFY(NM_CLICK, IDC_NODE_TREE, OnNMClickNodeTree)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CNodeViewDlg 消息处理程序

BOOL CNodeViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_TreeImages.Create(IDB_NODE_TYPE_BITMAP,16,8,0xffffffff);
	m_TreeNodes.SetImageList(&m_TreeImages,TVSIL_NORMAL);

    GetIGameInterface ()->InitialiseIGame();
	size_t nSize = m_MaxNodeTree.m_MaxNodeTreeRoot.size();
	for(size_t i =0 ; i < nSize; i++)
	{
		InsertNode(TVI_ROOT,m_MaxNodeTree.m_MaxNodeTreeRoot[i]);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNodeViewDlg::OnTvnSelchangedNodeTree(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}

void CNodeViewDlg::OnNMClickNodeTree(NMHDR *pNMHDR, LRESULT *pResult)
{
     LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
     //pNMTreeView->
    // TODO: Add your control notification handler code here
    *pResult = 0;
}

void CNodeViewDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    m_MaxNodeTree.confirmSel();
    OnOK();
}

void CNodeViewDlg::OnDestroy()
{

    m_MaxNodeTree.confirmSel();
    CDialog::OnDestroy();
   
    // TODO: Add your message handler code here
}
