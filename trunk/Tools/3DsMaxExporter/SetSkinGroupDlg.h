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

#include "XEvolMaxExporter.h"
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

typedef std::vector<INode*>           sMaxNodes_t;
#include "afxwin.h"
#include "afxcmn.h"

#define SKINGROUP_PRO "SkinGroup"
#define DEFAULT_SKIN  "def_skin/main"
struct sSkinMeshItem_t
{
	INode*    m_SkinMesh;
	HTREEITEM m_TreeItem;

};
typedef std::vector<sSkinMeshItem_t>  sSkinMeshItems_t;
struct sSkinGroup_t
{
	std::wstring        m_GroupName;
	HTREEITEM           m_TreeItem;
	sSkinMeshItems_t    m_SkinMeshItems;
	void                build_nodes(sMaxNodes_t& nodes)
	{
		for(int i = 0 ; i <(int)m_SkinMeshItems.size() ; i ++)
		{
			nodes.push_back(m_SkinMeshItems[i].m_SkinMesh);
		}
	}
};

typedef std::vector<sSkinGroup_t> sSkinGroups_t;

// CSetSkinGroupDlg dialog

class CBuildSkinGroup
{
	sSkinGroups_t     m_Groups;
public:

	void DestroyContex();
	void BuildContex(bool bUseAllNode = false);
	void BuildContex(sMaxNodes_t& bones);
	void BuildContex(INode* pNode , bool bRecursive = true);
	void DeleteSkinMesh(INode* pNode);
	void DeleteGroup(std::wstring name);
	void InsertSkinMesh(std::wstring GroupName, INode* pNode);
	int            CreateGroup(std::wstring GroupName);
	void           GetSkinMeshs(sMaxNodes_t& meshs);
	sSkinGroups_t& GetSkinGroups();
};


class CSetSkinGroupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetSkinGroupDlg)

	sMaxNodes_t* m_pBones;
public:
	CSetSkinGroupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetSkinGroupDlg();

// Dialog Data
	enum { IDD = IDD_SKINGROUP_DLG };

	void show();
	void show(sMaxNodes_t& bones);
	void hide();

	void DestroyContex();

	void BuildContex(bool bUseAllNode = false);
	void BuildContex(sMaxNodes_t& bones);
	void BuildContex(INode* pNode , bool bRecursive = true);

	void Confirm();
	void DeleteSkinMesh(INode* pNode);
	void DeleteSkinMesh(HTREEITEM hItem);
	void DeleteGroup(HTREEITEM hItem);
	void DeleteGroup(std::wstring name);
	void InsertSkinMesh(std::wstring GroupName, INode* pNode);

	int            CreateGroup(std::wstring GroupName);
	void           GetSkinMeshs(sMaxNodes_t& meshs);
	sSkinGroups_t& GetSkinGroups();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CImageList        m_TreeImages;
	CTreeCtrl         m_SkinGroupTree;
	sSkinGroups_t     m_Groups;
	afx_msg void OnBnClickedAssignGroup();
	afx_msg void OnBnClickedDelSkingroup();
	afx_msg void OnBnClickedCreateSkingroup();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedDumpToFile();
	BOOL m_bRefAllNodes;
	afx_msg void OnBnClickedReflesh();
};
