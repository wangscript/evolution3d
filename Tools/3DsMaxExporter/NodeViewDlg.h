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
#include "afxcmn.h"
#include "MaxNodeTree.h"

// CNodeViewDlg 对话框
#define IMAGE_BONE    0
#define IMAGE_MESH    1
#define IMAGE_HELPER  2
#define IMAGE_DUMMY   3
#define IMAGE_UNKNOWN 4
class CNodeViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CNodeViewDlg)
public:
    BOOL m_bSelected;
public:
	CNodeViewDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNodeViewDlg();
    void BuildRootNodeTree();
	void BuildSelNodeTree();
	CMaxNodeTree& getBuiledNodeTree(){return m_MaxNodeTree;}
	void ReleaseTree();
	void InsertNode(HTREEITEM parentItem , CMaxNodeTreeItem& Node , BOOL bSelect);
// 对话框数据
	enum { IDD = IDD_NODE_VIEW };
   
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CImageList   m_TreeImages;
    CMaxNodeTree m_MaxNodeTree;
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_TreeNodes;
	virtual BOOL OnInitDialog();
    afx_msg void OnTvnSelchangedNodeTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickNodeTree(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedOk();
    afx_msg void OnDestroy();
    afx_msg void OnTvnItemChangedNodeTree(NMHDR *pNMHDR, LRESULT *pResult);
};
