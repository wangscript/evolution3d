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
#include "MaxMeshNode.h"

// CMeshPropertyDlg dialog

class CMeshPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CMeshPropertyDlg)
    MaxMeshs& m_MaxMeshs;
public:
	CMeshPropertyDlg(MaxMeshs& meshs,CWnd* pParent = NULL);   // standard constructor
	virtual ~CMeshPropertyDlg();

// Dialog Data
	enum { IDD = IDD_MESH_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    BOOL m_bHasNormal;
    BOOL m_bHasDiffuse;
    BOOL m_bHasTangentSpace;
    UINT m_nUVLayers;
    CListCtrl m_MeshListCtrl;
};
