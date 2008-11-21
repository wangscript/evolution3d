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
#include "SetSkinGroupDlg.h"
#include "SkeletonModelExporter.h"
#include "xCfgParser.h"
#include <fstream>
using namespace std;
CSkeletonModelExporter::CSkeletonModelExporter(CMaxNodeTree& nodeTree)
:m_nodeTree(nodeTree)/*,
 m_SkinExporter(m_doc)*/
{
    m_nodeTree.buildFinalSelNodes(m_FinalSelNodes);
	m_Skeleton.build_modifiers(m_FinalSelNodes);
	CBuildSkinGroup               SkinGroupBuilder;
	m_SkinGroupBuilder.BuildContex(m_FinalSelNodes);
	MaxMeshs maxMeshs;
    BuildSkinMeshsFromNodes(maxMeshs,m_FinalSelNodes,&m_Skeleton);
	m_ExpContex.m_pSkeleton = &m_Skeleton;
    //m_ExpContex.m_SkinDir = "def_skin/main/";
}

bool CSkeletonModelExporter::export()
{
    wchar_t file_name[256];
    file_name[0] = 0;
    if(false == _get_saved_filename(file_name) )
    {
        MessageBoxA(GetActiveWindow(),"没有选择文件","操作被取消",MB_OK);
        return false;
    }
    if(false == _show_expeort_dlg() )
        return false;
    return _export_to_file(file_name);
}

bool CSkeletonModelExporter::__export_header(wstring prefix)
{
	sSkinGroups_t& skin_groups = m_SkinGroupBuilder.GetSkinGroups();
    m_ExpContex.m_ModelDesc.m_nSkinGroup = (int)skin_groups.size();

	//写入已经存在的skin列表
    xcomdocstream* pstream =  m_doc.create_stream((prefix + L"desc.xml").c_str());

	xCfgDocument xml;
	xCfgNode* rootNode = xml.insertNode(L"model");
	rootNode->insertValue(L"type")->setValue(L"skeleton");
	rootNode->insertValue(L"name")->setValue(m_ExpContex.m_ModelDesc.m_Name);
	xCfgNode* bbNode = rootNode->insertNode(L"BoundBox");
	bbNode->insertValue(L"min_x")->setValue(m_Boundbox.m_minx);
	bbNode->insertValue(L"min_y")->setValue(m_Boundbox.m_miny);
	bbNode->insertValue(L"min_z")->setValue(m_Boundbox.m_minz);
	bbNode->insertValue(L"max_x")->setValue(m_Boundbox.m_maxx);
	bbNode->insertValue(L"max_y")->setValue(m_Boundbox.m_maxy);
	bbNode->insertValue(L"max_z")->setValue(m_Boundbox.m_maxz);

	for(size_t i = 0 ; i < skin_groups.size() ; i++)
	{
		xCfgNode* skinGroupNode = rootNode->insertNode(L"skinGroup");
		skinGroupNode->insertValue(L"name")->setValue(skin_groups[i].m_GroupName.c_str());
	}

	pstream->stream_seekw(xcdsd_beg,0);
	xcdstream out_stream(pstream);
	xml.setXMLStyle(true);
	xml.save(out_stream);
	out_stream.close();
	m_doc.close_stream(pstream);
	xml.unload();


 //   int model_type = XRM_SKELETON_MODEL;
 //   pstream->write(model_type);
 //   pstream->write(m_ExpContex.m_ModelDesc);
 //   pstream->write(m_Boundbox);

	//for(int i = 0 ; i < skin_groups.size() ; i++)
	//{
	//	char skin_group_name[32];
	//	sprintf(skin_group_name,"%s",skin_groups[i].m_GroupName.c_str());
	//	pstream->write(skin_group_name,32);
	//}

	//写入action的数据
	m_ActionExporter.export(&m_Skeleton,m_ExpActions,m_doc,L"skeleton/");

	m_Skeleton.save_skeleton(m_doc,prefix + L"skeleton/");

    return true;
}



bool CSkeletonModelExporter::_export_to_file(wchar_t* file_name)
{
    //得到当前的时间．我们只需要导出当前那一帧
    
    DeleteFileW(file_name);
    m_doc.create(file_name);
    wstring errostring ;
    XEVOL_LOG(eXL_DEBUG_HIGH , L"Begin to export model to %s\n" , file_name );

    __export_skin(L"");

    m_Skeleton.buildHiberarchys();
    //输出文件头
    __export_header(L"");
	CMaxEnv::singleton().SaveLogToXComdoc(m_doc);
    m_doc.close();

    //清除Action的信息
    return true;
}

bool CSkeletonModelExporter::__export_skin(wstring prefix)
{
	sSkinGroups_t skin_groups =  m_SkinGroupBuilder.GetSkinGroups();
	for(size_t ig = 0 ; ig < skin_groups.size() ; ig++)
	{
         sSkinGroup_t& skin_group = skin_groups[ig];
		 MaxMeshs  maxMeshs;
		 sMaxNodes_t max_nodes;
		 skin_group.build_nodes(max_nodes);
		 BuildSkinMeshsFromNodes(maxMeshs,max_nodes,NULL);
		 CSkinExporter skinExporter(m_doc);
		 skinExporter.set_skeleton(&m_Skeleton);

		 wstring skin_dir = skin_group.m_GroupName;
		 if(skin_dir[skin_dir.length()-1] != '/')
			 skin_dir += L"/";

		 skinExporter.export_skin(L"",m_ExpContex.m_bTextMode,maxMeshs,skin_dir,m_Boundbox);
	}
    return true;
}