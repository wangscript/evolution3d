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
#include "xXmlDocument.h"
#include "xExportFunction.h"
CStaticModelExporter::CStaticModelExporter(CMaxNodeTree& nodeTree)
:m_nodeTree(nodeTree)
{
    m_fModelScale = 1.0;
    sMaxNodes_t nodes;
    m_nodeTree.buildFinalSelNodes(nodes);
    BuildMeshsFromNodes(m_MaxMeshs,nodes);
}

bool CStaticModelExporter::export()
{
    wchar_t file_name[256];
    file_name[0] = 0;
    if(false == get_saved_filename(file_name) )
    {
        MessageBoxA(GetActiveWindow(),"没有选择文件","操作被取消",MB_OK);
        return false;
    }
    show_expeort_dlg();
    if(m_IsExportToPackage == false)
         return export_to_file(file_name);
    else
         return export_to_package(file_name);
}

bool CStaticModelExporter::export_to_package(wchar_t * file_name)
{
    return true;
}
bool CStaticModelExporter::export_header(wstring prefix,vector<wstring>& mesh_names)
{
    m_desc.m_nMesh = (int)mesh_names.size();
    //write the model desc
    //m_doc.create_stream( (prefix + MODEL_TYPE_STRING(XRM_STATIC_MODEL) ).c_str());
    xcomdocstream* pstream =  m_doc.create_stream((prefix + L"desc.xml").c_str(),(xcd_data_type)cddt_static_model_desc);


	xXmlDocument xml;
	xXmlNode* rootNode = xml.insertNode(L"model");
	rootNode->insertValue(L"type")->setValue(L"static");
	rootNode->insertValue(L"name")->setValue(m_desc.m_Name);
	xXmlNode* bbNode = rootNode->insertNode(L"BoundBox");
	bbNode->insertValue(L"min_x")->setValue(m_BoundBox.m_minx);
	bbNode->insertValue(L"min_y")->setValue(m_BoundBox.m_miny);
	bbNode->insertValue(L"min_z")->setValue(m_BoundBox.m_minz);
	bbNode->insertValue(L"max_x")->setValue(m_BoundBox.m_maxx);
	bbNode->insertValue(L"max_y")->setValue(m_BoundBox.m_maxy);
	bbNode->insertValue(L"max_z")->setValue(m_BoundBox.m_maxz);

	for(size_t i = 0 ; i<mesh_names.size();++i)
	{
		xXmlNode* meshNode = rootNode->insertNode(L"mesh");
		meshNode->insertValue(L"name")->setValue(mesh_names[i].c_str());
	}

	stringstream out_stream;
	xml.setXMLStyle(true);
	xml.save(out_stream);
	pstream->stream_seekw(xcdsd_beg,0);
	pstream->write(out_stream.str().c_str(),out_stream.str().length());
	m_doc.close_stream(pstream);


    //int model_type = XRM_STATIC_MODEL;
    //pstream->write(model_type);
    //pstream->write(m_desc);
    //pstream->write(m_BoundBox);
    //for(size_t i = 0 ; i<mesh_names.size();++i)
    //{
    //    strncpy(mesh_name,mesh_names[i].c_str(),64);
    //    mesh_name[63] = 0;
    //    pstream->write(mesh_name,64);
    //}
    return true;
}


bool CStaticModelExporter::export_mesh(wstring prefix,wstring mesh_name,CMaxMesh& mesh , bool bTextMode)
{
	xMeshData& meshData = mesh.GetOutputData();

    

    wstring mesh_dir  = prefix + mesh_name;
    m_doc.create_stream(mesh_dir.c_str(),(xcd_data_type)cddt_static_mesh_dir);

	//export mesh description
	sSkeletonID_t sid;
	sid.m_HiWord = 0;
	sid.m_LoWord = 0;
    
	CModelBaseExportor::export_mesh_data(meshData,mesh_dir,m_doc, sid , false , bTextMode);

    return true;
}

bool CStaticModelExporter::export_to_file(wchar_t* file_name)
{
    //得到当前的时间．我们只需要导出当前那一帧
    unsigned int cur_time = CMaxEnv::singleton().m_pInterace->GetTime();
    DeleteFileW(file_name);
    m_doc.create(file_name);
    m_BoundBox.clear(100000000000.0f);
    wstring errostring ;
    XEVOL_LOG(eXL_DEBUG_HIGH , L"Begin to export model to %s\n",file_name );
    vector<wstring> mesh_names;
    for(size_t i = 0 ; i < m_MaxMeshs.size(); ++i)
    {
         CMaxMesh& mesh = m_MaxMeshs[i];
         xMeshData& meshData = mesh.GetOutputData();
         //建立这个mesh
         mesh.create(cur_time);
         mesh.set_scale(m_fModelScale);

         //简历这个Mesh的面的信息
         if(false == mesh.build_face_info(false) )
         {
             errostring = wstring(L"物体[")+INodeName(mesh.m_pNode)+L"]创建面的信息出错";
             XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errostring.c_str() );
             mesh.free();
             continue;
         }

         //建立这个mesh的数据信息，找回顶点纹理等信息
         if(false == mesh.build_data_info() )
         {
             errostring = wstring(L"物体[")+INodeName(mesh.m_pNode)+L"]创建顶点数据信息出错";
             XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} %s\n" , errostring.c_str() );
             mesh.free();
             continue;
         }

		 if(true == mesh.build_morph_target())
		 {
			 errostring = wstring(L"物体[")+INodeName(mesh.m_pNode)+L"] 有Morpher修改器，启用表情系统";
			 XEVOL_LOG(eXL_DEBUG_HIGH , L"   {提示} %s \n", errostring.c_str()	);
		 }

         //为了防止重名，得到一个可用的名字
         wstring mesh_name = get_avialabale_mesh_name(L"",INodeName(mesh.m_pNode));
         //把当前的包围盒添加到模型的包围盒中
         m_BoundBox.combine(meshData.m_BoundBox);
         //如果名字被重命名了，就打印警告信息
         if(mesh_name != INodeName(mesh.m_pNode))
         {
			 XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Warning}[%s] 改名为[%s]\n", INodeName(mesh.m_pNode) , mesh_name.c_str());
         }


         //导出这个maxmesh
         if(m_isTextMode)
         {
              XEVOL_LOG(eXL_DEBUG_HIGH , L"   >>--Exporting<TextMode>: %s\n" , mesh_name.c_str() );
              XEVOL_LOG(eXL_DEBUG_HIGH , L"       nFace =%d ;  nSubMesh = %d",meshData.m_pMeshDesc->m_nFace , meshData.m_pMeshDesc->m_nSubMesh );
         }
         else
         {
              XEVOL_LOG(eXL_DEBUG_HIGH , L"   >>--Exporting<BinMode>: %s \n", mesh_name.c_str() );
              XEVOL_LOG(eXL_DEBUG_HIGH , L"       nFace=%d ,nSubMesh=%d\n ",meshData.m_pMeshDesc->m_nFace,meshData.m_pMeshDesc->m_nSubMesh );
              
         }
		 export_mesh(L"",mesh_name,mesh , m_isTextMode);         
         //导出了一个，添加到导出列表中
         mesh_names.push_back(mesh_name);
         mesh.free();
    }

    //输出文件头
    export_header(L"",mesh_names);

    XEVOL_LOG(eXL_DEBUG_HIGH , L"   >>Exporting Finshing:  %d meshs exporting \n" , (int)mesh_names.size() );
	CMaxEnv::singleton().SaveLogToXComdoc(m_doc);
    m_doc.close();
    return true;
}
