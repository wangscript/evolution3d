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
#include <Windows.h>
#include <string>
using namespace std;
#include "XEvolMaxExporter.h"
#include "Resource.h"
#include "XEvolMaxExporter.h"
#include "NodeViewDlg.h"
#include "LogTextDlg.h"
#include "SkeletonSetDlg.h"


//


//各种类型的exporter.
#include "StaticModelExport.h"
#include "FrameAniModelExpoter.h"
#include "SkeletonModelExporter.h"


#include "SkeletonActionExpDlg.h"
//所有的对话框消息都在这里处理
bool LoadSkeleton(CSkeletonExporter& skeletonExporter);
template <typename T> bool IsInVec(const vector<T>& v, const T& v2)
{
     for(int i = 0 ;i  < v.size() ; i ++)
	 {
		 if(v[i] == v2)
			 return true;
	 }
	 return false;
}
void AppandSkins(sSkinGroups_t& groups,xcomdocstream* pDescStream)
{
	if(pDescStream == NULL)
		return ;

	int model_type = XRM_SKELETON_MODEL;

	pDescStream->stream_seekr(xcdsd_beg,0);
	char* buf = new char[pDescStream->data_len() + 1];
	pDescStream->read(buf,pDescStream->data_len());
	buf[pDescStream->data_len() ] = 0;
	xXmlDocument xml;
	xml.load( buf, (int)pDescStream->data_len() ,true);
    xXmlNode* rootNode = xml.root();
	if(rootNode == NULL)
	{
		return ;
	}
	xXmlNode::XmlNodes _oldNodes;
	rootNode->findNode(L"skinGroup",_oldNodes);
	for(size_t i =  0 ; i < groups.size() ; ++i)
	{
		bool bFinded = false;
		for(size_t j = 0 ; j < _oldNodes.size() ; j ++)
		{
			xXmlNode* pNode = _oldNodes[j];
			if( std::wstring(pNode->value(L"name")) == groups[i].m_GroupName)
			{
				bFinded = true;
			}
		}

		if(bFinded == false)
		{
            rootNode->insertNode(L"skinGroup")->insertValue(L"name")->setValue(groups[i].m_GroupName.c_str() );
		}
	}

	pDescStream->stream_seekw(xcdsd_beg,0);
	xcdstream out_stream(pDescStream);
	xml.setXMLStyle(true);
	xml.save(out_stream);
	out_stream.close();
	xml.unload();
}

bool On_ExportSkinToSignleFile(wchar_t *skin_file,CSkeletonExporter& skeExporter)
{
	CNodeViewDlg dlg;
	dlg.BuildSelNodeTree();
	if(IDOK == dlg.DoModal() )
	{  
		xcomdoc skin_doc;
		skin_doc.open(skin_file,xcdm_rw,0,true,false);
		if(skin_doc.failed())
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"打开皮肤文件失败 %s \n" , skin_file );
			skin_doc.close();
			return false;
		}

		sMaxNodes_t FinalSelNodes;
		dlg.getBuiledNodeTree().buildFinalSelNodes(FinalSelNodes);
		CBuildSkinGroup               SkinGroupBuilder;
		SkinGroupBuilder.BuildContex(FinalSelNodes);
		MaxMeshs maxMeshs;
		BuildSkinMeshsFromNodes(maxMeshs,FinalSelNodes,&skeExporter);

		CSkinExporter skinExporter(skin_doc);
		skinExporter.set_skeleton(&skeExporter);

		sBoundBox_t Boundbox;
		skinExporter.export_skin(L"",false,maxMeshs,L"",Boundbox);

		G_MaxEnv().SaveLogToXComdoc(skin_doc);
		skin_doc.close();
		ReleaseMaxMeshs(maxMeshs);
		return true;
	}
	return false;
}

bool On_ExportSkin(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	CSkeletonExporter skeExporter;
	if(LoadSkeleton(skeExporter) == false)
		return false;

	wchar_t skin_file[128];
	skin_file[0] = 0;

	XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\n");
	XEVOL_LOG(eXL_DEBUG_HIGH , L"开始导出皮肤文件 \n");
	if(G_MaxEnv().GetSaveFileName(skin_file,
		L"XEvol模型文件(*.xrm)\0*.xrm\0 XEvol皮肤组文件(*.xsg)\0*.xsg\0XEvol皮肤文件(*.xskin)\0*.xskin\0所有文件(*.*)\0*.*\0",
		L"xrm",
		L"请选择一个皮肤文件") == false)
	{
		return false;
	}


	//如果导出到一个.xskin文件。那么导出将覆盖这个文件。所以先删除它。
	if(std::wstring(skin_file).find(L".xskin") != std::wstring::npos )
	{
		DeleteFileW(skin_file);
        return On_ExportSkinToSignleFile(skin_file,skeExporter);
	}

	//如果导出到一个.xrm或者一个.xsg文件里。说明是把一个mesh追加进去。所以不需要删除
	CNodeViewDlg dlg;
	dlg.BuildSelNodeTree();
	if(IDOK == dlg.DoModal() )
	{  
		xcomdoc skin_doc;
		skin_doc.open(skin_file,xcdm_rw,0,true,false);
		if(skin_doc.failed())
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"打开皮肤文件失败 [%s]\n" , skin_file );
			skin_doc.close();
			return false;
		}

        

		sMaxNodes_t FinalSelNodes;
		dlg.getBuiledNodeTree().buildFinalSelNodes(FinalSelNodes);
		CBuildSkinGroup               SkinGroupBuilder;
		SkinGroupBuilder.BuildContex(FinalSelNodes);
		
		sSkinGroups_t skin_groups =  SkinGroupBuilder.GetSkinGroups();

		AppandSkins(skin_groups,skin_doc.open_stream(L"desc.xml"));
		bool ret = true;
		for(size_t ig = 0 ; ig < skin_groups.size() ; ig++)
		{
			sSkinGroup_t& skin_group = skin_groups[ig];
			MaxMeshs  maxMeshs;
			sMaxNodes_t max_nodes;
			skin_group.build_nodes(max_nodes);
			BuildSkinMeshsFromNodes(maxMeshs,max_nodes,&skeExporter);
			CSkinExporter skinExporter(skin_doc);
			skinExporter.set_skeleton(&skeExporter);

			wstring skin_dir = skin_group.m_GroupName;

			if(skin_dir[skin_dir.length()-1] != '/')
				skin_dir += L"/";

			sBoundBox_t Boundbox;
			bool ret2 = skinExporter.export_skin(L"",false,maxMeshs,skin_dir,Boundbox);
			ret = ret2 & ret2;
			ReleaseMaxMeshs(maxMeshs);
		}
		G_MaxEnv().SaveLogToXComdoc(skin_doc);
		skin_doc.close();
		return true;
	}
	return false;
}