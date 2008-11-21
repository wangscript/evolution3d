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

#pragma once

#include <vector>
#include <string>

#include "afxcmn.h"
using namespace std;
#define BONEGROUP_PRO  "BoneGroup"
#define ISABONE_PRO    "IsBone"
// CSetBoneGroupDlg dialog
#include "MaxEnv.h"
#include "afxwin.h"

typedef vector<INode*>           sMaxNodes_t;

struct sBoneItem_t
{
	INode*    m_Bone;
	HTREEITEM m_TreeItem;

};

typedef vector<sBoneItem_t>  sBoneItems_t;
struct sBoneGroup_t
{
	wstring           m_GroupName;
	HTREEITEM         m_TreeItem;
	sBoneItems_t      m_BoneItems;
	void                build_nodes(sMaxNodes_t& nodes)
	{
		for(int i = 0 ; i <(int)m_BoneItems.size() ; i ++)
		{
			nodes.push_back(m_BoneItems[i].m_Bone);
		}
	}
};

typedef vector<sBoneGroup_t> sBoneGroups_t;


class CSetBoneGroupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetBoneGroupDlg)

	sMaxNodes_t* m_pBones;

public:
	CSetBoneGroupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetBoneGroupDlg();

// Dialog Data
	enum { IDD = IDD_BONES_GROUP };

	void SetContexs(sMaxNodes_t* pBones){m_pBones = pBones;}
	void show();
	void show(sMaxNodes_t& bones);
	void hide();

	void DestroyContex();
	void BuildContex();
	void BuildContex(sMaxNodes_t& bones);
	void BuildContex(INode* pNode , bool bRecursive = true);
	void Confirm();
	

	void DeleteBone(INode* pNode);
	void DeleteBone(HTREEITEM hItem);

	void DeleteGroup(HTREEITEM hItem);
	void DeleteGroup(wstring name);
	void InsertBone(wstring GroupName, INode* pNode);

	int  CreateGroup(wstring GroupName);
	
	void GetBones(sMaxNodes_t& bones);
	sBoneGroups_t& GetBoneGroups();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCreateGroup();


public:

	CImageList        m_TreeImages;
	sBoneGroups_t     m_Groups;
	CTreeCtrl         m_BoneGroupTree;
	afx_msg void OnBnClickedAssignGroup();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	CButton m_BtnRefresh;
	CButton m_BtnDelGroup;
	CButton m_BtnCreateGroup;
	CButton m_BtnAssignBoneGroup;
	afx_msg void OnBnClickedDumpToFile();
};
