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

#include "MaxMeshNode.h"
#include "afxwin.h"
// CStaticMeshExportDlg dialog

class CStaticMeshExportDlg : public CDialog
{
	DECLARE_DYNAMIC(CStaticMeshExportDlg)

    wchar_t   m_model_name[32];
    MaxMeshs& m_MaxMeshs;
public:
	CStaticMeshExportDlg(MaxMeshs& meshs,CWnd* pParent = NULL);   // standard constructor
	virtual ~CStaticMeshExportDlg();

// Dialog Data
	enum { IDD = IDD_STATICMESH_EXPORT };

    void  GetModelName(wchar_t * model_name)
    {
        wcsncpy(model_name,m_model_name,32);
    }

    bool  IsPackage()
    {
        return m_IsPackage != FALSE;
    }
    bool  IsTextMode()
    {
        return m_bIsTextMode != FALSE;
    }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedMeshProp();
    BOOL m_IsPackage;
    CEdit m_txtModelName;
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    BOOL m_bIsTextMode;
};
