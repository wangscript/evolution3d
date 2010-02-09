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
#include "MaxMesh.h"
#include "SkeletonExp.h"
#include "ActionExporter.h"
#include "SkinExporter.h"
#include "DataDefine.h"
using namespace XEvol3DModel;
using namespace XEvol3D;
typedef std::vector<sActionInfo_t> MaxSkeletonActionInfos_t;

struct sSkeletonModelExpCtx_t
{
    bool                  m_bTextMode;
    bool                  m_bExpPackage;
    float                 m_fModelScale;
    sActionInfos_t        m_Actions;
    SkeletonModelDesc_t   m_ModelDesc;
	CSkeletonExporter*    m_pSkeleton;
    sSkeletonModelExpCtx_t()
	{
		m_bTextMode   = false;
		m_bExpPackage = false;
		m_fModelScale = 1.0f;
		m_pSkeleton   = NULL;
	}
};

class CSkeletonModelExporter : public CModelBaseExportor
{
    sSkeletonModelExpCtx_t   m_ExpContex;
    sActionInfos_t           m_ExpActions;
    CSkeletonExporter        m_Skeleton;
    xcomdoc                  m_doc;
    sBoundBox_t              m_Boundbox;
    CActionExporter          m_ActionExporter;
	sMaxNodes_t              m_FinalSelNodes;
	CBuildSkinGroup          m_SkinGroupBuilder;
	CMaxNodeTree&            m_nodeTree;
public:
    CSkeletonModelExporter(CMaxNodeTree& nodeTree);
    ~CSkeletonModelExporter()
    {
         m_Skeleton.RemoveModifiers();
    }
    bool export();
protected:
    bool   _show_expeort_dlg();
    bool   _get_saved_filename(wchar_t* file_name);
    bool   _export_to_file(wchar_t* file_name);
    bool   __export_skin(std::wstring prefix);
    bool   __export_header(std::wstring prefix);
};
