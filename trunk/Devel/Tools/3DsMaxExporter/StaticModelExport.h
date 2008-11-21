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

#ifndef __MAX_STATIC_MODEL_EXPORT_H__
#define __MAX_STATIC_MODEL_EXPORT_H__
#include <vector>
#include <string>
using namespace std;

#include "MaxNodeTree.h"
#include "xExportFunction.h"
#include "MaxMeshNode.h"
#include "xcomdoc/xcomdoc.h"
#include "DataDefine.h"

using namespace XEvol3D;
using namespace XEvol3DModel;

typedef vector<CMaxMesh> MaxMeshs;



class CStaticModelExporter : public CModelBaseExportor
{
    sBoundBox_t            m_BoundBox;
    MaxMeshs               m_MaxMeshs;
    xcomdoc                m_doc;
    bool                   m_IsExportToPackage;
    bool                   m_isTextMode;
    float                  m_fModelScale;
    StaticModelDesc_t      m_desc;
	CMaxNodeTree&          m_nodeTree;
public:
    CStaticModelExporter(CMaxNodeTree& nodeTree);

    bool export();
protected:

    bool    show_expeort_dlg();
    bool    get_saved_filename(wchar_t* file_name);
    
    
    bool    export_to_file(wchar_t* file_name);
    bool    export_to_package(wchar_t* file_name);

    bool    export_header(wstring prefix,vector<wstring>& mesh_names);
    bool    export_mesh(wstring prefix,wstring mesh_name,CMaxMesh& mesh , bool bTextMode);

	wstring get_avialabale_mesh_name(wstring prefix, wstring name);

};


#endif
