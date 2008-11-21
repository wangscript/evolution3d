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

// SetBoneGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MaxEnv.h"
#include "XEvolMaxExporter.h"
#include "SetBoneGroupDlg.h"
#include ".\setbonegroupdlg.h"
#include "BoneGroupNameDlg.h"

// CSetBoneGroupDlg dialog

IMPLEMENT_DYNAMIC(CSetBoneGroupDlg, CDialog)
CSetBoneGroupDlg::CSetBoneGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetBoneGroupDlg::IDD, pParent)
{
	m_pBones = NULL;
}

CSetBoneGroupDlg::~CSetBoneGroupDlg()
{
}

void CSetBoneGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_BoneGroupTree);
	DDX_Control(pDX, IDC_BUTTON5, m_BtnRefresh);
	DDX_Control(pDX, IDC_BUTTON3, m_BtnDelGroup);
	DDX_Control(pDX, IDC_CREATE_GROUP, m_BtnCreateGroup);
	DDX_Control(pDX, IDC_ASSIGN_GROUP, m_BtnAssignBoneGroup);
}

void CSetBoneGroupDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(m_pBones)
	{
        EndDialog(IDOK);
	}
	else
	{
		hide();
	}

}

void CSetBoneGroupDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	DestroyContex();
	OnOK();
}

void CSetBoneGroupDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	DestroyContex();
	OnCancel();
}

BEGIN_MESSAGE_MAP(CSetBoneGroupDlg, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CREATE_GROUP, OnBnClickedCreateGroup)
	ON_BN_CLICKED(IDC_ASSIGN_GROUP, OnBnClickedAssignGroup)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_DUMP_TO_FILE, OnBnClickedDumpToFile)
END_MESSAGE_MAP()


// CSetBoneGroupDlg message handlers

void CSetBoneGroupDlg::GetBones(sMaxNodes_t& bones)
{
    for(size_t i = 0 ; i < m_Groups.size() ; ++i)
	{
		sBoneGroup_t& group = m_Groups[i];
		for(size_t j = 0 ; j < group.m_BoneItems.size() ; ++ j )
		{
		    sBoneItem_t& bItem = group.m_BoneItems[j];
			bones.push_back(bItem.m_Bone);
		}
	}
}

sBoneGroups_t& CSetBoneGroupDlg::GetBoneGroups()
{
	return m_Groups;
}

void CSetBoneGroupDlg::Confirm()
{
	if( ! IsWindow(CMaxEnv::m_BoneGroupDlg.GetSafeHwnd()) )
	{
		CMaxEnv::m_BoneGroupDlg.Create(CSetBoneGroupDlg::IDD);
		CMaxEnv::m_BoneGroupDlg.hide();
	}
}

void CSetBoneGroupDlg::show()
{
	BuildContex();
	this->ShowWindow(SW_SHOW);
}
void CSetBoneGroupDlg::show(sMaxNodes_t& bones)
{
    BuildContex(bones);
	this->ShowWindow(SW_SHOW);
}
void CSetBoneGroupDlg::hide()
{   
	DestroyContex();
	ShowWindow(SW_HIDE);
}

void CSetBoneGroupDlg::DestroyContex()
{
	Confirm();
	m_Groups.clear();
	m_BoneGroupTree.DeleteAllItems();
}

void CSetBoneGroupDlg::BuildContex(sMaxNodes_t& bones)
{
	Confirm();
	for(size_t i = 0 ; i < bones.size() ; ++i)
	{
		BuildContex(bones[i],false);
	}
}
void CSetBoneGroupDlg::BuildContex(INode* pNode , bool bRecursive)
{
	Confirm();
	TSTR groupName;
	wstring strGroupName;
	if( !pNode->UserPropExists(BONEGROUP_PRO) && CMaxEnv::IsBone(pNode) )
	{
		pNode->SetUserPropBool(ISABONE_PRO,TRUE);
		pNode->SetUserPropString(BONEGROUP_PRO,"DefGroup");
		strGroupName = L"DefGroup";
	}

	if(pNode->UserPropExists(ISABONE_PRO) )
	{
		pNode->SetUserPropBool(ISABONE_PRO,TRUE);
		pNode->GetUserPropString(BONEGROUP_PRO,groupName);
		strGroupName = ToWCHAR(groupName);
		InsertBone(strGroupName,pNode);
	}

	if(bRecursive == false)
		return ;

	for(int i = 0 ; i < pNode->NumberOfChildren() ; i++)
	{
		BuildContex(pNode->GetChildNode(i));
	}
}

void CSetBoneGroupDlg::BuildContex()
{
	Confirm();
	BuildContex(CMaxEnv::singleton().m_pInterace->GetRootNode() );
}



int CSetBoneGroupDlg::CreateGroup(wstring GroupName)
{
	
	for(size_t i = 0 ; i < m_Groups.size() ; i++)
	{
		if(m_Groups[i].m_GroupName == GroupName)
			return (int)i;
	}
	
	HTREEITEM hTreeItem =  m_BoneGroupTree.InsertItem( ToAnsi(GroupName).c_str(),1,1);
	sBoneGroup_t group;
	group.m_GroupName = GroupName;
	group.m_TreeItem  = hTreeItem;
	m_Groups.push_back(group);
	return (int)(m_Groups.size() ) - 1;
}

void CSetBoneGroupDlg::DeleteBone(INode* pNode)
{
   for(size_t iGroup = 0 ; iGroup < m_Groups.size() ; iGroup ++)
   {
	   sBoneGroup_t& group = m_Groups[iGroup];
	   sBoneItems_t::iterator i = group.m_BoneItems.begin();

	   for(; i != group.m_BoneItems.end() ; )
	   {
		   sBoneItem_t& bitem = *i;
		   if( bitem.m_Bone == pNode)
		   {
			   m_BoneGroupTree.DeleteItem(bitem.m_TreeItem);
			   i = group.m_BoneItems.erase(i);
		   }
		   else
		   {
			   i++;
		   }
	   }
   }
}

void CSetBoneGroupDlg::DeleteBone(HTREEITEM hItem)
{
	for(size_t iGroup = 0 ; iGroup < m_Groups.size() ; iGroup ++)
	{
		sBoneGroup_t& group = m_Groups[iGroup];
		sBoneItems_t::iterator i = group.m_BoneItems.begin();

		for(; i != group.m_BoneItems.end() ;i++)
		{
			sBoneItem_t& bitem = *i;
			if( bitem.m_TreeItem == hItem)
			{				
				m_BoneGroupTree.DeleteItem(bitem.m_TreeItem);
				i = group.m_BoneItems.erase(i);
				return;
			}
		}
	}
}

void CSetBoneGroupDlg::DeleteGroup(HTREEITEM hItem)
{
	sBoneGroups_t::iterator it = m_Groups.begin();
	for(size_t i = 0 ; i < m_Groups.size() ; i ++,it ++)
	{
		if(m_Groups[i].m_TreeItem == hItem)
		{
			sBoneGroup_t& group = m_Groups[i];
			for(size_t j = 0 ; j < group.m_BoneItems.size() ; ++j)
			{
				sBoneItem_t& bItem = group.m_BoneItems[j];
				m_BoneGroupTree.DeleteItem(bItem.m_TreeItem);
				bItem.m_Bone->SetUserPropString(BONEGROUP_PRO,"DefGroup");
				CMaxEnv::SetXEvol3DBone(bItem.m_Bone,FALSE);
			}
			m_Groups.erase(it);
			m_BoneGroupTree.DeleteItem(group.m_TreeItem);
			return ;
		}
	}
}

void CSetBoneGroupDlg::DeleteGroup(wstring name)
{
	 sBoneGroups_t::iterator it = m_Groups.begin();
     for(size_t i = 0 ; i < m_Groups.size() ; i ++,it ++)
	 {
		 if(m_Groups[i].m_GroupName == name)
		 {
			 sBoneGroup_t& group = m_Groups[i];
			 for(size_t j = 0 ; j < group.m_BoneItems.size() ; ++j)
			 {
				 sBoneItem_t& bItem = group.m_BoneItems[j];
				 m_BoneGroupTree.DeleteItem(bItem.m_TreeItem);
				 bItem.m_Bone->SetUserPropString(BONEGROUP_PRO,"DefGroup");
				 CMaxEnv::SetXEvol3DBone(bItem.m_Bone,FALSE);
			 }
			 m_Groups.erase(it);
			 m_BoneGroupTree.DeleteItem(group.m_TreeItem);
			 return ;
		 }
	 }
}

void CSetBoneGroupDlg::InsertBone(wstring GroupName, INode* pNode)
{
    DeleteBone(pNode);
	int iGroup = CreateGroup(GroupName);
	sBoneGroup_t& group = m_Groups[iGroup];
	HTREEITEM hItem = m_BoneGroupTree.InsertItem(INodeNameA(pNode),0 ,0,group.m_TreeItem);
	sBoneItem_t item ;
	item.m_Bone = pNode;
	pNode->SetUserPropBool(ISABONE_PRO,true);
	pNode->SetUserPropString(BONEGROUP_PRO, ToAnsi(GroupName).c_str());
	item.m_TreeItem = hItem;
	group.m_BoneItems.push_back(item);
}

void CSetBoneGroupDlg::OnBnClickedAssignGroup()
{
	// TODO: Add your control notification handler code here
	HTREEITEM hTreeItem = m_BoneGroupTree.GetSelectedItem();
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

	
	for(int i = 0 ;  i < CMaxEnv::singleton().m_pInterace->GetSelNodeCount() ; i++)
	{
		INode* pNode = CMaxEnv::singleton().m_pInterace->GetSelNode(i);
		pNode->SetUserPropBool(ISABONE_PRO,true);
		pNode->SetUserPropString(BONEGROUP_PRO,name.c_str());
		InsertBone(name.c_str(),pNode);
	}
}

BOOL CSetBoneGroupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_TreeImages.Create(IDB_NODE_TYPE_BITMAP,16,8,0xffffffff);
	m_BoneGroupTree.SetImageList(&m_TreeImages,TVSIL_NORMAL);

	if(m_pBones)
	{
		BuildContex(*m_pBones);
		m_BtnRefresh.EnableWindow(FALSE);
		m_BtnDelGroup.EnableWindow(FALSE);
		m_BtnCreateGroup.EnableWindow(FALSE);
		m_BtnAssignBoneGroup.EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetBoneGroupDlg::OnBnClickedCreateGroup()
{
	CBoneGroupNameDlg BGND;
	if(BGND.DoModal() == IDCANCEL)
		return;
	wstring GroupName = BGND.GetGroupName();
	CreateGroup(GroupName);
	// TODO: Add your control notification handler code here
}

void CSetBoneGroupDlg::OnBnClickedButton3()
{
	HTREEITEM hTreeItem = m_BoneGroupTree.GetSelectedItem();
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
	// TODO: Add your control notification handler code here
}

void CSetBoneGroupDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	DestroyContex();
	BuildContex();
}

#include <fstream>
using namespace std;

void CSetBoneGroupDlg::OnBnClickedDumpToFile()
{
	wofstream out;
	CMaxEnv::singleton().BeginLog(L"xrm_bone_group.dump.txt");
	for(size_t iG = 0 ; iG < m_Groups.size() ; ++iG)
	{
		sBoneGroup_t& g = m_Groups[iG];
		XEVOL_LOG(eXL_DEBUG_HIGH ,"<Begin Group" );
		XEVOL_LOG(eXL_DEBUG_HIGH ,"  name = %s\n" , g.m_GroupName.c_str() );
		for(size_t i = 0 ; i < g.m_BoneItems.size() ; ++i)
		{
			sBoneItem_t& item = g.m_BoneItems[i];
			XEVOL_LOG(eXL_DEBUG_HIGH ,"     item< %d > name=  %s \n" , i , INodeName(item.m_Bone ).c_str() );
		}
		XEVOL_LOG(eXL_DEBUG_HIGH ,"End Group>");
	}
	CMaxEnv::singleton().FinishLog();
}
