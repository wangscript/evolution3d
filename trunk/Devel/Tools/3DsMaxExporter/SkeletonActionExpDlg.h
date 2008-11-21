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

// CSkeletonActionExpDlg dialog
using namespace XEvol3DModel;
class CSkeletonActionExpDlg : public CDialog
{
	DECLARE_DYNAMIC(CSkeletonActionExpDlg)
    sActionInfos_t& m_ActionList;
public:
	CSkeletonActionExpDlg(sActionInfos_t& ActionList,CWnd* pParent = NULL);   // standard constructor
	virtual ~CSkeletonActionExpDlg();

// Dialog Data
	enum { IDD = IDD_SKELETON_ACTION_EXP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    bool         InsertAction(sActionInfo_t& action,bool isInsertToUI = false);
    int          findAction(const wchar_t* actionName);
    void         RemoveAction(int index);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnLvnItemchangedActionList(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL OnInitDialog();
    CListCtrl m_ctrlActionList;
    int m_iStartFrame;
    int m_EndFrame;
    int m_iDurationTime;
    CEdit m_ctrlActionName;
    afx_msg void OnBnClickedActionAdd();
    afx_msg void OnBnClickedActionRemove();
    afx_msg void OnBnClickedFinishSetup();
};
