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

#pragma  once

#include <vector>
#include <string>
using namespace std;

#include "MaxNodeTree.h"
#include "xExportFunction.h"
#include "MaxMeshNode.h"
#include "SkeletonExp.h"
#include "DataDefine.h"
using namespace XEvol3DModel;


class CSkinExporter : public CModelBaseExportor
{
    xcomdoc&           m_doc;
    float              m_Scale;
    CSkeletonExporter* m_pSkeleton;
public:
    CSkinExporter(xcomdoc& doc):m_doc(doc)
    {
         m_pSkeleton = NULL;
    }
    void   set_skeleton(CSkeletonExporter* pSkeleton)
    {
        m_pSkeleton = pSkeleton;
    }
 
	bool   export_skin_header(wstring skin_group_dir,vector<wstring>& mesh_names);
	bool   export_skin_mesh(wstring skin_group_dir,wstring mesh_name,CMaxMesh& mesh , bool bTexMode);
    bool   export_skin(const wstring prefix, bool bTexMode , MaxMeshs& maxMeshs, const wstring& SkinName, sBoundBox_t& BoundBox);
public:
    
   
    bool   __export_action_data();
	bool   __remove_exist_mesh(wstring skin_group_dir,MaxMeshs& mesh_in ,MaxMeshs& mesh_out);
    wstring __get_avialabale_mesh_name(wstring skin_group_dir, wstring name);
};