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

class CActionExporter
{
	void    writeActionList(sActionInfos_t& actions, xcomdoc&  comdoc, std::wstring action_list_name);
public:
    void    export(CSkeletonExporter* pSkeleton, sActionInfos_t& actions, xcomdoc&  comdoc, std::wstring action_dir);
    Matrix3 get_bone_tm(CSkeletonExporter* pSkeleton,int boneIndex,unsigned int iMaxTime);
    Matrix3 get_locale_tm(INode* pNode , bool hasParent,unsigned int iMaxTime);
	//void    export_xml(CSkeletonExporter* pSkeleton, sActionInfos_t& actions, xcomdoc&  comdoc, std::wstring action_dir);
	bool    load_action_list(sActionInfos_t& actions,xcomdoc&  comdoc, std::wstring action_list_name);
};
#include "ActionExporter_Helper.h"
