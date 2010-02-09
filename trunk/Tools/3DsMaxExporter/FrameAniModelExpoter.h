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

#ifndef __MAX_FRAMEANI_MODEL_EXPORT_H__
#define __MAX_FRAMEANI_MODEL_EXPORT_H__
#include <vector>
#include <string>
using namespace std;

#include "MaxNodeTree.h"
#include "xExportFunction.h"
#include "MaxMeshNode.h"
#include "DataDefine.h"
#include "MaxEnv.h"
using namespace XEvol3DModel;

typedef std::vector<sMaxActionInfo_t> MaxFrameActionInfos_t;

struct sFrameAniModelExpCtx_t
{
    MaxMeshs              m_MaxMeshs;

    bool                  m_bTextMode;
    bool                  m_bExpPackage;
    float                 m_fModelScale;

    MaxFrameActionInfos_t m_Actions;
    FrameAniModelDesc_t   m_ModelDesc;
};

struct FrameAniActionData_t
{
    vector<sBoundBox_t> m_FrameBoundBoxs;//这个动作每帧的BoundBox
    sMaxActionInfo_t    m_ActionInfoExp; //为导出做准备的Actions数据

};
class CFrameAniModelExporter : public CModelBaseExportor
{
    vector<sBoundBox_t>    m_FrameBoundBoxs;
    xcomdoc                m_doc;
    sFrameAniModelExpCtx_t m_ExpContex;
    typedef std::vector<FrameAniActionData_t> ActionDatas_t; 

    ActionDatas_t          m_ActionDatas;
	CMaxNodeTree&          m_nodeTree;

public:
    CFrameAniModelExporter(CMaxNodeTree& nodeTree);
    bool export();
protected:
    bool  _show_expeort_dlg();
    bool _get_saved_filename(wchar_t* file_name);

    bool _export_to_file(wchar_t* file_name);
    bool _export_to_package(wchar_t* file_name);
	bool __export_to_xcomdoc();

    bool __export_header(std::wstring prefix,vector<wstring>& mesh_names);
    bool __export_mesh(std::wstring prefix,wstring mesh_name,CMaxMesh& mesh);
    bool __export_mesh_faceinfo(std::wstring prefix,wstring mesh_name,CMaxMesh& mesh);

    std::wstring __get_avialabale_mesh_name(std::wstring prefix, std::wstring name);
    void    __build_action_datas();//从对话框出来后。我们要重新创建动作的数据列表，用来保存信息

    //void parse_action_frame(sMaxActionInfo_t& action_raw,
    //    int iframe,
    //    int& p_frame_persist,
    //    int& n_frame_persist,
    //    int& c_frame_persist,
    //    int& t_frame_persist,
    //    int& u_frame_persist );

};


#endif
