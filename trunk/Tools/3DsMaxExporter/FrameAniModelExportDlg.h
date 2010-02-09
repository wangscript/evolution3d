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

#include "FrameAniModelExpoter.h"
// CFrameAniModelExportDlg dialog

class CFrameAniModelExportDlg : public CDialog
{
	DECLARE_DYNAMIC(CFrameAniModelExportDlg)

    sFrameAniModelExpCtx_t& m_ExpContex;
public:
	CFrameAniModelExportDlg(sFrameAniModelExpCtx_t& ExpContex,CWnd* pParent = NULL);   // standard constructor
	virtual ~CFrameAniModelExportDlg();

// Dialog Data
	enum { IDD = IDD_FRAMEANI_MODEL_EXPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    bool         InsertAction(sMaxActionInfo_t& action);
    void         RemoveAction(int index);
    int          findAction(const wchar_t* actionName);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedCancel();
    CListCtrl m_ctrlActionList;
    BOOL m_bTexCooesAni;
    BOOL m_bPositionAni;
    CEdit m_ctrlActionName;
    UINT m_uiActionTime;
    UINT m_uiFirstFrame;
    UINT m_uiLastFrame;
    CEdit m_ctrlPackagePath;
    BOOL m_bExportToPackage;
    float m_fScaleFactor;
    CEdit m_ctrlModelName;
    afx_msg void OnBnClickedCancelExport();
    afx_msg void OnBnClickedExport();
    afx_msg void OnBnClickedActionAdd();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedActionRemove();
    BOOL m_bTextMode;
    afx_msg void OnBnClickedMeshProperty();
    BOOL m_bIsTextMode;
};
