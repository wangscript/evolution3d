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
#include "SkeletonModelExporter.h"
// CSkeletonModelExpDlg dialog

class CSkeletonModelExpDlg : public CDialog
{
	DECLARE_DYNAMIC(CSkeletonModelExpDlg)

    sSkeletonModelExpCtx_t& m_ExpContex;
public:
	CSkeletonModelExpDlg(sSkeletonModelExpCtx_t& expContex,CWnd* pParent = NULL);   // standard constructor
	virtual ~CSkeletonModelExpDlg();
// Dialog Data
	enum { IDD = IDD_SKELETON_MODEL_EXPROT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedExport();
    CEdit m_ctrlModelName;
    float m_fScale;
    afx_msg void OnBnClickedActionSet();
    BOOL m_bTextMode;
	afx_msg void OnBnClickedSetSkeleton();
};
