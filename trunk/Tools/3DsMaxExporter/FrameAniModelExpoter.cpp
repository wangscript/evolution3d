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
#include "xXmlDocument.h"
#include "xExportFunction.h"
CFrameAniModelExporter::CFrameAniModelExporter(CMaxNodeTree& nodeTree)
:m_nodeTree(nodeTree)
{
    sMaxNodes_t nodes;
    m_nodeTree.buildFinalSelNodes(nodes);
    BuildMeshsFromNodes(m_ExpContex.m_MaxMeshs,nodes);
}

bool CFrameAniModelExporter::export()
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

    if(m_ExpContex.m_bExpPackage == false)
        return _export_to_file(file_name);
    else
        return _export_to_package(file_name);

    return true;
}

bool CFrameAniModelExporter::__export_to_xcomdoc()
{
	__build_action_datas();
	wstring errostring ;
	vector<wstring> mesh_names;
	for(size_t i = 0 ; i < m_ExpContex.m_MaxMeshs.size(); ++i)
	{
		CMaxMesh& mesh = m_ExpContex.m_MaxMeshs[i];
        xMeshData& meshData = mesh.GetOutputData();
		//建立这个mesh
		mesh.create(0);
		mesh.set_scale(m_ExpContex.m_fModelScale);

		//简历这个Mesh的面的信息
		if(false == mesh.build_face_info(false) )
		{
			errostring = std::wstring(L"物体[")+ INodeName(mesh.m_pNode)+ L"]创建面的信息出错";
			XEVOL_LOG(eXL_DEBUG_HIGH  , L"   {Error} %s\n",errostring.c_str() );
			mesh.free();
			continue;
		}

		//为了防止重名，得到一个可用的名字
		wstring mesh_name = __get_avialabale_mesh_name(L"", INodeName(mesh.m_pNode) );
		//如果名字被重命名了，就打印警告信息
		if(mesh_name != INodeName(mesh.m_pNode))
		{
			XEVOL_LOG(eXL_DEBUG_HIGH  , L"   {Error} %s\n",errostring.c_str() );
			XEVOL_LOG(eXL_DEBUG_HIGH  , L"   {Warning}[%s] 改名为 [%s]\n",INodeName(mesh.m_pNode),mesh_name.c_str() );
		}

		bool exp_mesh_ret = false;

		//导出这个maxmesh
		if(m_ExpContex.m_bTextMode)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH  , L"   >>--Exporting<TextMode>: %s\n", mesh_name.c_str() );
		}
		else
		{
			XEVOL_LOG(eXL_DEBUG_HIGH  , L"   >>--Exporting<BinMode>: %s\n" , mesh_name.c_str() );
		}

		XEVOL_LOG(eXL_DEBUG_HIGH  , L"       nFace=%d , nSubMesh=%d\n" , meshData.m_pMeshDesc->m_nFace , meshData.m_pMeshDesc->m_nSubMesh );
		exp_mesh_ret = __export_mesh(L"",mesh_name,mesh);

		if(exp_mesh_ret)
		{
			//导出了一个，添加到导出列表中
			mesh_names.push_back(mesh_name);
		}
		mesh.free();
	}

	//输出文件头
	__export_header(L"",mesh_names);
	XEVOL_LOG(eXL_DEBUG_HIGH  , L"   >>Exporting Finshing: %d meshs exporting\r\n" , (int)mesh_names.size() );

	//清除Action的信息
	m_ActionDatas.clear();
	return true;
}

bool CFrameAniModelExporter::_export_to_file(wchar_t* file_name)
{
    //得到当前的时间．我们只需要导出当前那一帧
	XEVOL_LOG(eXL_DEBUG_HIGH , L"Begin to export model to %s \n" , file_name );
    DeleteFileW(file_name);
    m_doc.create(file_name);
    bool ret = __export_to_xcomdoc();
	CMaxEnv::singleton().SaveLogToXComdoc(m_doc);
    m_doc.close();
    return ret;
}

bool CFrameAniModelExporter::_export_to_package(wchar_t* file_name)
{
     return true;
}

bool CFrameAniModelExporter::__export_header(std::wstring prefix,vector<wstring>& mesh_names)
{
	m_ExpContex.m_ModelDesc.m_nSubMesh = (int)mesh_names.size();
	//写入每一帧对应的信息，现在暂时只有包围盒
	int nFrame = (int)m_FrameBoundBoxs.size();
	sBoundBox_t _bb;
	_bb.clear(100000000000.0f);
	for(int i = 0 ; i < nFrame ; ++ i)
	{
		_bb.combine(m_FrameBoundBoxs[i]);
	}


    
    //write the model desc
    //m_doc.create_stream( (prefix + MODEL_TYPE_STRING(XRM_STATIC_MODEL) ).c_str());
    xcomdocstream* pstream =  m_doc.create_stream((prefix + L"desc.xml").c_str(),(xcd_data_type)cddt_frame_ani_model_desc);
	xXmlDocument xml;
	xXmlNode* rootNode = xml.insertNode(L"model");
	rootNode->insertValue(L"type")->setValue(L"frame");
	rootNode->insertValue(L"name")->setValue(m_ExpContex.m_ModelDesc.m_Name);
	xXmlNode* bbNode = rootNode->insertNode(L"BoundBox");
	bbNode->insertValue(L"min_x")->setValue(_bb.m_minx);
	bbNode->insertValue(L"min_y")->setValue(_bb.m_miny);
	bbNode->insertValue(L"min_z")->setValue(_bb.m_minz);
	bbNode->insertValue(L"max_x")->setValue(_bb.m_maxx);
	bbNode->insertValue(L"max_y")->setValue(_bb.m_maxy);
	bbNode->insertValue(L"max_z")->setValue(_bb.m_maxz);
	for(size_t i = 0 ; i<mesh_names.size();++i)
	{
		xXmlNode* meshNode = rootNode->insertNode(L"mesh");
		meshNode->insertValue(L"name")->setValue(mesh_names[i].c_str());
	}

	pstream->stream_seekw(xcdsd_beg,0);
	xcdstream out_stream(pstream);
	xml.setXMLStyle(true);
	xml.save(out_stream);
	out_stream.close();
	m_doc.close_stream(pstream);
	xml.unload();

    //写入每一动作的数据
    xcomdocstream* pActstream =  m_doc.create_stream((prefix + L"actions.xml").c_str());

	xXmlDocument xmlAct;
	xXmlNode* rootActNode = xmlAct.insertNode(L"actions");
	int nAct = (int)m_ExpContex.m_Actions.size();
    nFrame = 0;
    for(int i = 0 ; i < nAct ; ++i)
	{
		xXmlNode* actNode = rootActNode->insertNode(L"action");
		sActionInfo_t& action = m_ActionDatas[i].m_ActionInfoExp.m_BaseInfo ;
		action.m_nFrame = GetActNumFrame(action);
		actNode->insertValue(L"Name")->setValue(action.m_Name );
		actNode->insertValue(L"nFrame")->setValue(action.m_nFrame);
		actNode->insertValue(L"DurTime")->setValue(action.m_lTime);
		actNode->insertValue(L"FirstFrame")->setValue(action.m_iFirstFrame);
		actNode->insertValue(L"LastFrame")->setValue(action.m_iLastFrame);
		actNode->insertValue(L"ActionType")->setValue(action.m_ActionType);
        nFrame += (action.m_ActionType - action.m_iLastFrame);
	}

	xcdstream out_actstream(pActstream);
	pActstream->stream_seekw(xcdsd_beg,0);
	xmlAct.setXMLStyle(true);
	xmlAct.save(out_actstream);
	out_actstream.close();
	m_doc.close_stream(pActstream);
	xmlAct.unload();



    return false;
}

bool CFrameAniModelExporter::__export_mesh(std::wstring prefix,wstring mesh_name,CMaxMesh& mesh)
{
    //先写入面的信息


	vMaxTimes exportTimes;
    for(int i = 0 ; i < int(m_ActionDatas.size()) ; ++i)
    {

		sMaxActionInfo_t& action = m_ActionDatas[i].m_ActionInfoExp;
		sMaxActionInfo_t& action_raw = m_ExpContex.m_Actions[i]; 
		xMeshData&   meshData = mesh.GetOutputData();
		int frame_s = action_raw.m_BaseInfo.m_iFirstFrame;
		int frame_e = action_raw.m_BaseInfo.m_iLastFrame;
		int frame_n = abs(frame_e - frame_s) + 1;
		int frame_delta = 1;
		unsigned int iMaxTime = frame_s * GetTicksPerFrame();
		if(frame_e < frame_s) frame_delta = -1;

		for(int iframe = frame_s ; iframe != frame_e ; iframe += frame_delta)
		{
			iMaxTime = iframe * GetTicksPerFrame();
			exportTimes.push_back(iMaxTime);
		}
    }

    if(false == export_mesh_action(prefix,m_doc , m_FrameBoundBoxs , mesh_name,mesh, exportTimes) )
		return false;
	if( __export_mesh_faceinfo (prefix,mesh_name,mesh) == false)
		return false;

	return true;
}


bool CFrameAniModelExporter::__export_mesh_faceinfo(std::wstring prefix,wstring mesh_name,CMaxMesh& mesh)
{
	xMeshData& meshData = mesh.GetOutputData();
    std::wstring mesh_dir  = prefix + mesh_name;
    m_doc.create_stream(mesh_dir.c_str(),(xcd_data_type)cddt_frame_ani_mesh_dir);

	sSkeletonID_t sid;
	sid.m_HiWord = 0;
	sid.m_LoWord = 0;


    //用来描述一个mesh的frame信息的
    std::wstring frame_desc_file = mesh_dir + L"/frame";
    int nFrame = 0 ;
    for(size_t i = 0 ; i < m_ActionDatas.size() ; i ++)
    {
        FrameAniActionData_t& acdata=m_ActionDatas[i];
        sMaxActionInfo_t& acinfo = acdata.m_ActionInfoExp;
        nFrame += (acinfo.m_BaseInfo.m_iLastFrame - acinfo.m_BaseInfo.m_iFirstFrame + 1); 
    }

    //创建数据流
    xcomdocstream* pFrameStream = m_doc.create_stream(frame_desc_file.c_str(),(xcd_data_type)cddt_frame_ani_mesh_desc);
    int nAction = (int)m_ActionDatas.size();
    //一共有几个frame.
    pFrameStream->write(nFrame);
    pFrameStream->write(nAction);
    for(size_t i = 0 ; i < m_ActionDatas.size() ; i ++)
    {
        FrameAniActionData_t& acdata=m_ActionDatas[i];
        sMaxActionInfo_t& acinfo = acdata.m_ActionInfoExp;
        pFrameStream->write(acinfo);   
    }

    m_doc.close_stream(pFrameStream);
    //写入Mesh的Material
    exportMaterialToStream(mesh_dir , m_doc ,meshData );
    //写入face Index. Index buffer
	exportFaceToStream(mesh_dir , m_doc , meshData);

	//exportMeshDescToStream(mesh_dir , m_doc , meshData,nFrame,sid);
	exportMeshDescToStreamAsXML(mesh_dir , m_doc , meshData,nFrame,sid);

    return true;
}


