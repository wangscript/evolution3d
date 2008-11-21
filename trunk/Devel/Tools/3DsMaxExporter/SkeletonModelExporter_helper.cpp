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

#include "stdafx.h"
#include "SkeletonModelExporter.h"
#include "SkeletonModelExpDlg.h"
bool CSkeletonModelExporter::_show_expeort_dlg()
{
	m_ExpContex.m_pSkeleton = &m_Skeleton;
    CSkeletonModelExpDlg dlg(m_ExpContex);
    if(dlg.DoModal() != IDOK)
        return false;

    m_ExpActions.clear();
    m_ExpActions.resize(m_ExpContex.m_Actions.size());
    for(size_t i = 0 ; i  < m_ExpContex.m_Actions.size() ; ++i)
    {
        sActionInfo_t& action = m_ExpContex.m_Actions[i];
        m_ExpActions[i] = action;
        m_ExpActions[i].m_iFirstFrame = action.m_iFirstFrame ;
        m_ExpActions[i].m_iLastFrame  = action.m_iLastFrame  ;
    }

    //m_ExpContex.m_nFrame = m_ExpActions[m_ExpContex.m_Actions.size() - 1].m_iLastFrame + 1;
    m_Skeleton.m_fScale = m_ExpContex.m_fModelScale;
    m_Boundbox.clear(10000000000.0f);
    return true;
}


bool CSkeletonModelExporter::_get_saved_filename(wchar_t* file_name)
{
    OPENFILENAMEW ofn; 
    ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    ofn.hwndOwner = GetActiveWindow();
    ofn.lpstrFile = file_name;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"XEvol模型文件(*.xrm)\0*.xrm\0所有文件(*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrDefExt = L"xrm";
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if(GetSaveFileNameW(&ofn) == FALSE)
    {
        return false;
    }
    return true;
}