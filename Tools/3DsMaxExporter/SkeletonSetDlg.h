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
#include "afxwin.h"
#include "DataDefine.h"
#include "MaxEnv.h"
#include "MaxNodeTree.h"
// CSkeletonSetDlg dialog
class CSkeletonExporter;
class CSkeletonSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CSkeletonSetDlg)

	CSkeletonExporter* m_pSkeleton;
public:
	CSkeletonSetDlg(float fScale,CWnd* pParent = NULL);   // standard constructor
	virtual ~CSkeletonSetDlg();
	void setSkeleton(CSkeletonExporter*  pSkeleton)
	{
		m_pSkeleton = pSkeleton;
	}
    void BuildRootNodeTree();
    void BuildSelNodeTree();
    void BuildNodeTree(sMaxNodes_t& nodes);
    CMaxNodeTree& getBuiledNodeTree(){return m_MaxNodeTree;}
    void ReleaseTree();
    void InsertNode(HTREEITEM parentItem , CMaxNodeTreeItem& Node);
// Dialog Data
	enum { IDD = IDD_SKELETON_DLG };

    CImageList   m_TreeImages;
    CMaxNodeTree m_MaxNodeTree;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CTreeCtrl m_TreeNodes;
    afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedViewGroup();
	afx_msg void OnBnClickedSaveSkeleton();
	float m_fScale;
	afx_msg void OnBnClickedAllBones();
	afx_msg void OnBnClickedAddBones();
    afx_msg void OnBnClickedAddBoneH();
};
