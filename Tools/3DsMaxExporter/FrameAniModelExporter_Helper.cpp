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
#include "FrameAniModelExpoter.h"
#include "FrameAniModelExportDlg.h"
bool  CFrameAniModelExporter::_show_expeort_dlg()
{
      CFrameAniModelExportDlg dlg(m_ExpContex);
      if(IDCANCEL == dlg.DoModal() )
          return false;
      return true;
}
//void CFrameAniModelExporter::parse_action_frame(sMaxActionInfo_t& action_raw,
//                                                int iframe,
//                                                int& p_frame_persist,
//                                                int& n_frame_persist,
//                                                int& c_frame_persist,
//                                                int& t_frame_persist,
//                                                int& u_frame_persist )
//{
//    int frame_s = action_raw.m_BaseInfo.m_iFirstFrame;
//    int frame_e = action_raw.m_BaseInfo.m_iLastFrame;
//    int frame_n = abs(frame_e - frame_s) + 1;
//    p_frame_persist = 0;
//    n_frame_persist = 0;
//    c_frame_persist = frame_n;
//    t_frame_persist = 1;
//    u_frame_persist = 0;
//    if(action_raw.m_bPositionAni == true)
//    {
//        p_frame_persist = 1;
//        n_frame_persist = 1;
//    }
//    else
//    {
//        if(iframe == frame_s) //是第一帧
//        {
//            p_frame_persist = frame_n;
//            n_frame_persist = frame_n;
//        }
//    }
//
//    //处理纹理动画
//    if(action_raw.m_bTexCoordAni == true)
//    {
//        u_frame_persist = 1;
//    }
//    else
//    {
//        if(iframe == frame_s) //是第一帧
//        {
//            u_frame_persist = frame_n;
//        }
//    }
//    //不是第一帧。不导出颜色
//    if(iframe != frame_s) c_frame_persist = 0;
//}
void CFrameAniModelExporter::__build_action_datas()
{
    //首先清除所有的数据
    m_ActionDatas.clear();
    size_t nAction  = m_ExpContex.m_Actions.size();
    for(size_t i = 0 ; i < nAction ; ++i)
    {
        sMaxActionInfo_t& act_info = m_ExpContex.m_Actions[i];
        FrameAniActionData_t act_data;
        act_data.m_ActionInfoExp = act_info;
        act_data.m_ActionInfoExp.m_BaseInfo.m_iFirstFrame = 0 ;
        act_data.m_ActionInfoExp.m_BaseInfo.m_iLastFrame = act_info.m_BaseInfo.m_iLastFrame - act_info.m_BaseInfo.m_iFirstFrame;
        m_ActionDatas.push_back(act_data);
    }

}

bool CFrameAniModelExporter::_get_saved_filename(wchar_t* file_name)
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

wstring CFrameAniModelExporter::__get_avialabale_mesh_name(std::wstring prefix, std::wstring name)
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



