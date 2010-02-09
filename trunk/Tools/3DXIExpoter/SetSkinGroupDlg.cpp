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

// SetSkinGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XEvolMaxExporter.h"
#include "SetSkinGroupDlg.h"
#include "MaxEnv.h"
#include "MaxMesh.h"
#include ".\setskingroupdlg.h"
#include "BoneGroupNameDlg.h"

// CSetSkinGroupDlg dialog


void CBuildSkinGroup::DestroyContex()
{
	m_Groups.clear();
}

void CBuildSkinGroup::BuildContex(sMaxNodes_t& Skins)
{
	for(size_t i = 0 ; i < Skins.size() ; ++i)
	{
		BuildContex(Skins[i],false);
	}
}
void CBuildSkinGroup::BuildContex(INode* pNode , bool bRecursive)
{
	TSTR groupName;
	wstring strGroupName;
	if( !pNode->UserPropExists(SKINGROUP_PRO) )
	{
		pNode->SetUserPropString(SKINGROUP_PRO,DEFAULT_SKIN);
		strGroupName = ToWCHAR(DEFAULT_SKIN);
	}
	else
	{
		pNode->GetUserPropString(SKINGROUP_PRO,groupName);
		strGroupName = ToWCHAR(groupName);
	}

	InsertSkinMesh(strGroupName,pNode);
	if(bRecursive == false)
		return ;

	for(int i = 0 ; i < pNode->NumberOfChildren() ; i++)
	{
		BuildContex(pNode->GetChildNode(i));
	}
}

void CBuildSkinGroup::BuildContex(bool bUseAllNode)
{
	sMaxNodes_t mesh_nodes;
	if(bUseAllNode == false)
	{
		sMaxNodes_t nodes;
		GetNodesFromSelectedNode(nodes);
		GetMeshNodesFromNodes(mesh_nodes,nodes);
		nodes.clear();
	}
	else
	{
		GetMeshNodesFromScene(mesh_nodes);
	}

	BuildContex(mesh_nodes);
}



int CBuildSkinGroup::CreateGroup(std::wstring GroupName)
{
	for(size_t i = 0 ; i < m_Groups.size() ; i++)
	{
		if(m_Groups[i].m_GroupName == GroupName)
			return (int)i;
	}
	sSkinGroup_t group;
	group.m_GroupName = GroupName;
	m_Groups.push_back(group);
	return (int)(m_Groups.size() ) - 1;
}

void CBuildSkinGroup::DeleteSkinMesh(INode* pNode)
{
	for(size_t iGroup = 0 ; iGroup < m_Groups.size() ; iGroup ++)
	{
		sSkinGroup_t& group = m_Groups[iGroup];
		sSkinMeshItems_t::iterator i = group.m_SkinMeshItems.begin();

		for(; i != group.m_SkinMeshItems.end() ; )
		{
			sSkinMeshItem_t& bitem = *i;
			if( bitem.m_SkinMesh == pNode)
			{				
				i = group.m_SkinMeshItems.erase(i);
			}
			else
			{
				i++;
			}
		}
	}
}

void CBuildSkinGroup::DeleteGroup(std::wstring name)
{
	sSkinGroups_t::iterator it = m_Groups.begin();
	for(size_t i = 0 ; i < m_Groups.size() ; i ++,it ++)
	{
		if(m_Groups[i].m_GroupName == name)
		{
			sSkinGroup_t& group = m_Groups[i];
			for(size_t j = 0 ; j < group.m_SkinMeshItems.size() ; ++j)
			{
				sSkinMeshItem_t& bItem = group.m_SkinMeshItems[j];
				bItem.m_SkinMesh->SetUserPropString(SKINGROUP_PRO,DEFAULT_SKIN);
			}
			m_Groups.erase(it);
			return ;
		}

	}
}

void CBuildSkinGroup::InsertSkinMesh(std::wstring GroupName, INode* pNode)
{
	DeleteSkinMesh(pNode);
	int iGroup = CreateGroup(GroupName);
	sSkinGroup_t& group = m_Groups[iGroup];
	sSkinMeshItem_t item ;
	item.m_SkinMesh = pNode;
	pNode->SetUserPropString(SKINGROUP_PRO,GroupName.c_str());
	group.m_SkinMeshItems.push_back(item);
}

void CBuildSkinGroup::GetSkinMeshs(sMaxNodes_t& Skins)
{
	for(size_t i = 0 ; i < m_Groups.size() ; ++i)
	{
		sSkinGroup_t& group = m_Groups[i];
		for(size_t j = 0 ; j < group.m_SkinMeshItems.size() ; ++ j )
		{
			sSkinMeshItem_t& bItem = group.m_SkinMeshItems[j];
			Skins.push_back(bItem.m_SkinMesh);
		}
	}
}

sSkinGroups_t& CBuildSkinGroup::GetSkinGroups()
{
	return m_Groups;
}


IMPLEMENT_DYNAMIC(CSetSkinGroupDlg, CDialog)
CSetSkinGroupDlg::CSetSkinGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetSkinGroupDlg::IDD, pParent)
	, m_bRefAllNodes(FALSE)
{
}

CSetSkinGroupDlg::~CSetSkinGroupDlg()
{
}

void CSetSkinGroupDlg::GetSkinMeshs(sMaxNodes_t& Skins)
{
	for(size_t i = 0 ; i < m_Groups.size() ; ++i)
	{
		sSkinGroup_t& group = m_Groups[i];
		for(size_t j = 0 ; j < group.m_SkinMeshItems.size() ; ++ j )
		{
			sSkinMeshItem_t& bItem = group.m_SkinMeshItems[j];
			Skins.push_back(bItem.m_SkinMesh);
		}
	}
}

sSkinGroups_t& CSetSkinGroupDlg::GetSkinGroups()
{
	return m_Groups;
}

void CSetSkinGroupDlg::Confirm()
{
	if( ! IsWindow(G_MaxEnv().m_SkinGroupDlg.GetSafeHwnd()) )
	{
		G_MaxEnv().m_SkinGroupDlg.Create(CSetSkinGroupDlg::IDD);
		G_MaxEnv().m_SkinGroupDlg.hide();
	}
}

void CSetSkinGroupDlg::show()
{
	BuildContex();
	this->ShowWindow(SW_SHOW);
}
void CSetSkinGroupDlg::show(sMaxNodes_t& Skins)
{
	BuildContex(Skins);
	this->ShowWindow(SW_SHOW);
}
void CSetSkinGroupDlg::hide()
{   
	DestroyContex();
	ShowWindow(SW_HIDE);
}

void CSetSkinGroupDlg::DestroyContex()
{
	Confirm();
	m_Groups.clear();
	m_SkinGroupTree.DeleteAllItems();
}

void CSetSkinGroupDlg::BuildContex(sMaxNodes_t& Skins)
{
	Confirm();
	for(size_t i = 0 ; i < Skins.size() ; ++i)
	{
		BuildContex(Skins[i],false);
	}
}
void CSetSkinGroupDlg::BuildContex(INode* pNode , bool bRecursive)
{
	Confirm();
	TSTR groupName;
	wstring strGroupName;
	if( !pNode->UserPropExists(SKINGROUP_PRO) )
	{
		pNode->SetUserPropString(SKINGROUP_PRO,DEFAULT_SKIN);
		strGroupName = ToWCHAR(DEFAULT_SKIN);
	}
	else
	{
       pNode->GetUserPropString(SKINGROUP_PRO,groupName);
	   strGroupName = ToWCHAR(groupName);
	}

	InsertSkinMesh(strGroupName,pNode);
	if(bRecursive == false)
		return ;

	for(int i = 0 ; i < pNode->NumberOfChildren() ; i++)
	{
		BuildContex(pNode->GetChildNode(i));
	}
}

void CSetSkinGroupDlg::BuildContex(bool bUseAllNode)
{
	Confirm();

	sMaxNodes_t mesh_nodes;
	if(bUseAllNode == false)
	{
		sMaxNodes_t nodes;
		GetNodesFromSelectedNode(nodes);
		GetMeshNodesFromNodes(mesh_nodes,nodes);
		nodes.clear();
	}
	else
	{
		GetMeshNodesFromScene(mesh_nodes);
	}
	
	BuildContex(mesh_nodes);
}



int CSetSkinGroupDlg::CreateGroup(std::wstring GroupName)
{

	for(size_t i = 0 ; i < m_Groups.size() ; i++)
	{
		if(m_Groups[i].m_GroupName == GroupName)
			return (int)i;
	}

	HTREEITEM hTreeItem =  m_SkinGroupTree.InsertItem( ToAnsi(GroupName).c_str(),2,2);
	sSkinGroup_t group;
	group.m_GroupName = GroupName;
	group.m_TreeItem  = hTreeItem;
	m_Groups.push_back(group);
	return (int)(m_Groups.size() ) - 1;
}

void CSetSkinGroupDlg::DeleteSkinMesh(INode* pNode)
{
	for(size_t iGroup = 0 ; iGroup < m_Groups.size() ; iGroup ++)
	{
		sSkinGroup_t& group = m_Groups[iGroup];
		sSkinMeshItems_t::iterator i = group.m_SkinMeshItems.begin();

		for(; i != group.m_SkinMeshItems.end() ; )
		{
			sSkinMeshItem_t& bitem = *i;
			if( bitem.m_SkinMesh == pNode)
			{				
				m_SkinGroupTree.DeleteItem(bitem.m_TreeItem);
				i = group.m_SkinMeshItems.erase(i);
			}
			else
			{
				i++;
			}
		}
	}
}
void CSetSkinGroupDlg::DeleteSkinMesh(HTREEITEM hItem)
{
	for(size_t iGroup = 0 ; iGroup < m_Groups.size() ; iGroup ++)
	{
		sSkinGroup_t& group = m_Groups[iGroup];
		sSkinMeshItems_t::iterator i = group.m_SkinMeshItems.begin();

		for(; i != group.m_SkinMeshItems.end() ;)
		{
			sSkinMeshItem_t& bitem = *i;
			if( bitem.m_TreeItem == hItem)
			{
				m_SkinGroupTree.DeleteItem(bitem.m_TreeItem);
				i = group.m_SkinMeshItems.erase(i);
				break;
			}
			else
			{
				i++;
			}
		}
	}
}

void CSetSkinGroupDlg::DeleteGroup(HTREEITEM hItem)
{
	sSkinGroups_t::iterator it = m_Groups.begin();
	for(size_t i = 0 ; i < m_Groups.size() ; i ++,it ++)
	{
		if(m_Groups[i].m_TreeItem == hItem)
		{
			sSkinGroup_t& group = m_Groups[i];
			for(size_t j = 0 ; j < group.m_SkinMeshItems.size() ; ++j)
			{
				sSkinMeshItem_t& bItem = group.m_SkinMeshItems[j];
				m_SkinGroupTree.DeleteItem(bItem.m_TreeItem);
				bItem.m_SkinMesh->SetUserPropString(SKINGROUP_PRO,DEFAULT_SKIN);
			}
			m_Groups.erase(it);
			return ;
		}
	}
}

void CSetSkinGroupDlg::DeleteGroup(std::wstring name)
{
	sSkinGroups_t::iterator it = m_Groups.begin();
	for(size_t i = 0 ; i < m_Groups.size() ; i ++,it ++)
	{
		if(m_Groups[i].m_GroupName == name)
		{
			sSkinGroup_t& group = m_Groups[i];
			for(size_t j = 0 ; j < group.m_SkinMeshItems.size() ; ++j)
			{
				sSkinMeshItem_t& bItem = group.m_SkinMeshItems[j];
				m_SkinGroupTree.DeleteItem(bItem.m_TreeItem);
				bItem.m_SkinMesh->SetUserPropString(SKINGROUP_PRO,DEFAULT_SKIN);
			}
			m_Groups.erase(it);
			m_SkinGroupTree.DeleteItem(m_Groups[i].m_TreeItem);
			return ;
		}

	}
}

void CSetSkinGroupDlg::InsertSkinMesh(std::wstring GroupName, INode* pNode)
{
	DeleteSkinMesh(pNode);
	int iGroup = CreateGroup(GroupName);
	sSkinGroup_t& group = m_Groups[iGroup];
	HTREEITEM hItem = m_SkinGroupTree.InsertItem(INodeNameA(pNode),1 ,1,group.m_TreeItem);
	sSkinMeshItem_t item ;
	item.m_SkinMesh = pNode;
	pNode->SetUserPropString(SKINGROUP_PRO, ToAnsi(GroupName).c_str());
	item.m_TreeItem = hItem;
	group.m_SkinMeshItems.push_back(item);
}

void CSetSkinGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_SkinGroupTree);
	DDX_Check(pDX, IDC_ALL_NODES, m_bRefAllNodes);
}


BEGIN_MESSAGE_MAP(CSetSkinGroupDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ASSIGN_GROUP, OnBnClickedAssignGroup)
	ON_BN_CLICKED(IDC_DEL_SKINGROUP, OnBnClickedDelSkingroup)
	ON_BN_CLICKED(IDC_CREATE_SKINGROUP, OnBnClickedCreateSkingroup)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DUMP_TO_FILE, OnBnClickedDumpToFile)
	ON_BN_CLICKED(IDC_REFLESH, OnBnClickedReflesh)
END_MESSAGE_MAP()


// CSetSkinGroupDlg message handlers

void CSetSkinGroupDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CSetSkinGroupDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CSetSkinGroupDlg::OnBnClickedAssignGroup()
{
	// TODO: Add your control notification handler code here
	HTREEITEM hTreeItem = m_SkinGroupTree.GetSelectedItem();
	wstring name;
	int iFind = -1;
	for(size_t i = 0 ; i < m_Groups.size() ; i++)
	{
		if(hTreeItem == m_Groups[i].m_TreeItem)
		{
			name = m_Groups[i].m_GroupName;
			iFind  = (int)i;
			break;
		}
	}

	if(iFind == -1)
		return ;


	sMaxNodes_t nodes;
	sMaxNodes_t mesh_nodes;
	GetNodesFromSelectedNode(nodes);
	GetMeshNodesFromNodes(mesh_nodes,nodes);

	for(size_t i = 0 ;  i < mesh_nodes.size() ; i++)
	{
		INode* pNode = mesh_nodes[i];
		pNode->SetUserPropString(SKINGROUP_PRO,name.c_str());
		InsertSkinMesh(name.c_str(),pNode);
	}
}

void CSetSkinGroupDlg::OnBnClickedDelSkingroup()
{
	HTREEITEM hTreeItem = m_SkinGroupTree.GetSelectedItem();
	wstring name;
	int iFind = -1;
	for(size_t i = 0 ; i < m_Groups.size() ; i++)
	{
		if(hTreeItem == m_Groups[i].m_TreeItem)
		{
			name = m_Groups[i].m_GroupName;
			iFind  = (int)i;
			break;
		}
	}

	if(iFind == -1)
		return ;

	DeleteGroup(name);
}

void CSetSkinGroupDlg::OnBnClickedCreateSkingroup()
{
	CBoneGroupNameDlg BGND;
	if(BGND.DoModal() == IDCANCEL)
		return;
	wstring GroupName = BGND.GetGroupName();
	CreateGroup(GroupName);
}

BOOL CSetSkinGroupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	// TODO:  Add extra initialization here
	m_TreeImages.Create(IDB_NODE_TYPE_BITMAP,16,8,0xffffffff);
	m_SkinGroupTree.SetImageList(&m_TreeImages,TVSIL_NORMAL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetSkinGroupDlg::OnBnClickedReflesh()
{
	DestroyContex();
    UpdateData();
	BuildContex(m_bRefAllNodes != FALSE);
}

void CSetSkinGroupDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
    hide();
}
#include <fstream>
using namespace std;
void CSetSkinGroupDlg::OnBnClickedDumpToFile()
{
	ofstream out;
	G_MaxEnv().BeginLog(L"xrm_skin_group.dump.txt");
	for(size_t iG = 0 ; iG < m_Groups.size() ; ++iG)
	{
		sSkinGroup_t& g = m_Groups[iG];
		XEVOL_LOG(eXL_DEBUG_HIGH , "<Begin Group\n");
		XEVOL_LOG(eXL_DEBUG_HIGH , L"  name =  %s\n" , g.m_GroupName.c_str() );  
		for(size_t i = 0 ; i < g.m_SkinMeshItems.size() ; ++i)
		{
			sSkinMeshItem_t& item = g.m_SkinMeshItems[i];
            XEVOL_LOG(eXL_DEBUG_HIGH , "     item< %d > name= %s\n",i,item.m_SkinMesh->GetName() ); 
		}
		XEVOL_LOG(eXL_DEBUG_HIGH , "End Group>\n");
	}
	G_MaxEnv().FinishLog();
	// TODO: Add your control notification handler code here
}
