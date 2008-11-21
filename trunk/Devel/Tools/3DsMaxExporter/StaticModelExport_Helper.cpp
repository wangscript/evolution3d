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
#include "XEvolMaxExporter.h"
#include "StaticMeshExportDlg.h"
#include "StaticModelExport.h"

bool CStaticModelExporter::show_expeort_dlg()
{
   CStaticMeshExportDlg dlg(m_MaxMeshs);
   if(IDOK == dlg.DoModal())
   {
       dlg.GetModelName(m_desc.m_Name);
       m_IsExportToPackage = dlg.IsPackage();
       m_isTextMode        = dlg.IsTextMode();
       return true;
   }
   return false;
}

bool CStaticModelExporter::get_saved_filename(wchar_t* file_name)
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
wstring CStaticModelExporter::get_avialabale_mesh_name(wstring prefix, wstring name)
{
    wchar_t posfix[32];
    if(m_doc.open_stream( (prefix + name).c_str(),true) == NULL)
        return name;
    for( int i = 1 ; ; i++)
    {
        swprintf(posfix,L"_%d",i);
        if(m_doc.open_stream( (prefix + name + posfix).c_str(),true) == NULL)
            return name + posfix;
    }
}
