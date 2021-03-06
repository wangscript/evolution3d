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

bool On_ViewNode(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ViewSceneNode(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ExportStaticModel(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ExportFrameAniModel(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ExportSkeletonModel(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_SetBoneID(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ConfirmScene(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_SetBonesGroup(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_SetSkinGroup(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_SetSkeleton(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ExportAction(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ExportSkin(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool On_ExportWaterMesh(HWND hDlg,WPARAM wParam,LPARAM lParam);
bool PannelCommand(HWND hDlg,WPARAM wParam,LPARAM lParam)
{    

     switch(wParam)
	 {

	 case IDC_SET_BONES_GROUP:
		 return On_SetBonesGroup(hDlg,wParam,lParam);
		 break;
	 case IDC_SET_SKIN_GROUP:
		 return On_SetSkinGroup(hDlg,wParam,lParam);
		 break;
	 case IDC_SET_SKELETON:
		 return On_SetSkeleton(hDlg,wParam,lParam);
		 break;

	 case IDB_VIEWNODE:
          return On_ViewNode(hDlg,wParam,lParam);
	 case IDC_VIEWROOTSCENE_NODE:
          return On_ViewSceneNode(hDlg,wParam,lParam);
     case IDC_EXPORT_STATICMODEL:
          return On_ExportStaticModel(hDlg,wParam,lParam);
     case IDC_EXPORT_FRAMEANI_MODEL:
         return On_ExportFrameAniModel(hDlg,wParam,lParam);
     case IDC_EXPORT_SKELETON_MODEL:
         return On_ExportSkeletonModel(hDlg,wParam,lParam);
         break;
	 case IDC_EXPORT_ACTION:
		 {
			 G_MaxEnv().BeginLog();
			 bool ret =  On_ExportAction(hDlg,wParam,lParam);
			 if(ret == false)
			 {
				XEVOL_LOG(eXL_DEBUG_HIGH , L"导出动作失败\n");
			 }
			 else
			 {
                 XEVOL_LOG(eXL_DEBUG_HIGH , L"导出动作成功\n");
			 }
			 G_MaxEnv().FinishLog();
			 return ret;
		 }
		 break;

	 case IDC_EXPORT_WATER_MESH:
		 {
			 G_MaxEnv().BeginLog();
			 bool ret =  On_ExportWaterMesh(hDlg,wParam,lParam);
			 if(ret == false)
			 {
				 XEVOL_LOG(eXL_DEBUG_HIGH , L"导出水面mesh失败\r\n");
			 }
			 else
			 {
				 XEVOL_LOG(eXL_DEBUG_HIGH , L"导出水面mesh成功\r\n");
			 }
			 G_MaxEnv().FinishLog();
			 return ret;
		 }
	 case IDC_EXPORT_SKIN:
		 {
			 G_MaxEnv().BeginLog();
			 bool ret =  On_ExportSkin(hDlg,wParam,lParam);
			 if(ret == false)
			 {
				 XEVOL_LOG(eXL_DEBUG_HIGH , L"导出皮肤失败\r\n");
			 }
			 else
			 {
				 XEVOL_LOG(eXL_DEBUG_HIGH , L"导出皮肤成功\r\n");
			 }
			 G_MaxEnv().FinishLog();
			 return ret;
		 }
		 break;
     case IDC_SETBONEID:
         return On_SetBoneID(hDlg,wParam,lParam);
         break;
     case IDC_CONFIRMSCENE:
         return On_ConfirmScene(hDlg,wParam,lParam);
         break;
     case IDC_BUSE_BEFORESKELETON_BIND:
         {
             BOOL bCheck = IsDlgButtonChecked(hDlg,IDC_BUSE_BEFORESKELETON_BIND);
             G_MaxEnv().m_bUseBeforeSkeletonPose = (bCheck!=FALSE);
         }
         break;
     case IDC_CALCINITPOS_MYSEF:
         {
             BOOL bCheck = IsDlgButtonChecked(hDlg,IDC_CALCINITPOS_MYSEF);
             G_MaxEnv().m_bCalcInitPosMySelf = (bCheck!=FALSE);
         }
         break;
     case IDC_EXPORT_VERTEX_SHELL:
         {
             BOOL bCheck = IsDlgButtonChecked(hDlg,IDC_EXPORT_VERTEX_SHELL);
             G_MaxEnv().m_bExpVertexNormal = (bCheck!=FALSE);
         }
	 case IDC_INVERT_V_COORD:
		 {
			 BOOL bCheck = IsDlgButtonChecked(hDlg,IDC_INVERT_V_COORD);
			 G_MaxEnv().m_bInvertTexCoordV = (bCheck!=FALSE);
		 }
	 case IDC_INVERT_YZ_COORD:
		 {
			 BOOL bCheck = IsDlgButtonChecked(hDlg,IDC_INVERT_YZ_COORD);
			 G_MaxEnv().m_bInvertYZCoord = (bCheck!=FALSE);
		 }
         break;
     }
     return true;
}

bool On_SetBoneID(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    if(true == G_MaxEnv().InitSceneBoneID() )
    {
        MessageBox(GetActiveWindow(),"场景的BoneID已经分配！","正确",MB_OK);
    }
    else
    {
        MessageBox(GetActiveWindow(),"场景的BoneID没有正确分配！\n 请检查你的场景中的物体的命名，\
                                      \n尽量不要把不同的Node设置成相同名字","错误",MB_OK);
    }
    return true;
}
bool On_ConfirmScene(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    if(true == G_MaxEnv().ConfirmBoneIDs() )
    {
        MessageBox(GetActiveWindow(),"场景的BoneID都已经分配！","正确",MB_OK);
    }
    return true;
}

bool On_ViewNode(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    CNodeViewDlg dlg;
    dlg.BuildSelNodeTree();
    dlg.DoModal();
    dlg.ReleaseTree();
    return true;
}

bool On_ViewSceneNode(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    CNodeViewDlg dlg;
    dlg.BuildRootNodeTree();
    dlg.DoModal();
    dlg.ReleaseTree();
    return true;
}


bool LoadSkeleton(CSkeletonExporter& skeletonExporter)
{
	wchar_t skeleton_file[128];
	skeleton_file[0] = 0;

	if(G_MaxEnv().GetSaveFileName(skeleton_file,
		L"XEvol骨骼文件(*.skeleton)\0*.skeleton\0XEvol模型文件(*.xrm)\0*.xrm\0所有文件(*.*)\0*.*\0",
		L"skeleton",
		L"请选择一个骨架文件") == false)
	{
		return false;
	}

	xcomdoc skeleton_doc;
	skeleton_doc.open(skeleton_file,xcdm_read);
	if(skeleton_doc.failed())
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"打开骨架文件失败\r\n");
		skeleton_doc.close();
		return false;
	}
	xcomdocstream* pstream = skeleton_doc.open_stream(L"skeleton/skeleton");
	if(pstream == false)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"打开骨架文件失败[skeleton/skeleton流不存在]\r\n");
		skeleton_doc.close();
		return false;
	}

	if(false == skeletonExporter.load_base_info(*pstream))
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"骨架非法，你选错了一个骨架\r\n");
		skeleton_doc.close();
		return false;
	}
	G_MaxEnv().SaveLogToXComdoc(skeleton_doc);
    skeleton_doc.close();
	return true;
}

bool On_ExportAction(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	wchar_t skeleton_file[128];
	skeleton_file[0] = 0;
	
	XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
	XEVOL_LOG(eXL_DEBUG_HIGH , L"开始导出动作\r\n");
	if(G_MaxEnv().GetSaveFileName(skeleton_file,
		 L"XEvol骨骼文件(*.skeleton)\0*.skeleton\0XEvol模型文件(*.xrm)\0*.xrm\0所有文件(*.*)\0*.*\0",
		 L"skeleton",
		 L"请选择一个骨架文件") == false)
	{
          return false;
	}
    CSkeletonExporter sexporter;
	xcomdoc skeleton_doc;
	skeleton_doc.open(skeleton_file,xcdm_rw);
	if(skeleton_doc.failed())
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"打开骨架文件失败\r\n");
		skeleton_doc.close();
		return false;
	}
    xcomdocstream* pstream = skeleton_doc.open_stream(L"skeleton/skeleton");
    if(pstream == false)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"打开骨架文件失败[skeleton/skeleton流不存在]\r\n");
		skeleton_doc.close();
		return false;
	}
	
	if(false == sexporter.load_base_info(*pstream))
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"骨架非法，你选错了一个骨架\r\n");
		skeleton_doc.close();
		return false;
	}
	
	sActionInfos_t actions;
	CActionExporter action_exporter;
	action_exporter.load_action_list(actions,skeleton_doc,L"skeleton/actions.xml");
	CSkeletonActionExpDlg sAExpdlg(actions);
	sAExpdlg.DoModal();
    action_exporter.export(&sexporter,actions,skeleton_doc,L"skeleton/");

	G_MaxEnv().SaveLogToXComdoc(skeleton_doc);
	skeleton_doc.close();
	return true;
}

bool On_ExportStaticModel(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    G_MaxEnv().BeginLog();
    XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , L"开始导出静态模型\r\n");
    CNodeViewDlg dlg;
    dlg.BuildSelNodeTree();
    if(IDOK == dlg.DoModal() )
    {
        CStaticModelExporter exportor(dlg.getBuiledNodeTree());
        exportor.export();
        dlg.ReleaseTree();
        XEVOL_LOG(eXL_DEBUG_HIGH , L"导出静态模型完成\r\n");
        XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
        G_MaxEnv().FinishLog();
        return true;
    }
	G_MaxEnv().FinishLog();
    return false;

}

bool On_ExportFrameAniModel(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    G_MaxEnv().BeginLog();
    XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , L"开始导出帧动画模型\r\n");
    CNodeViewDlg dlg;
    dlg.BuildSelNodeTree();
    if(IDOK == dlg.DoModal() )
    {
        CFrameAniModelExporter exportor(dlg.getBuiledNodeTree());
        exportor.export();

        dlg.ReleaseTree();
        XEVOL_LOG(eXL_DEBUG_HIGH , L"导出帧动画模型完成\r\n");
        XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
        G_MaxEnv().FinishLog();
        return true;
    }
	G_MaxEnv().FinishLog();
    return false;

}

bool On_ExportSkeletonModel(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
    G_MaxEnv().BeginLog();
    XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , L"开始导出骨骼动画模型\r\n");
    CNodeViewDlg dlg;
    dlg.BuildSelNodeTree();
    if(IDOK == dlg.DoModal() )
    {
        CSkeletonModelExporter exportor(dlg.getBuiledNodeTree());
        exportor.export();

        dlg.ReleaseTree();
        XEVOL_LOG(eXL_DEBUG_HIGH , L"导出骨骼动画模型完成\r\n");
        XEVOL_LOG(eXL_DEBUG_HIGH , L"------------------------------------\r\n");
        G_MaxEnv().FinishLog();
        return true;
    }
	G_MaxEnv().FinishLog();
    return false;
}

bool On_SetSkeleton(HWND hDlg,WPARAM wParam,LPARAM lParam)
{  
    G_MaxEnv().m_BoneGroupDlg.DestroyContex();
	G_MaxEnv().m_BoneGroupDlg.BuildContex();

	sMaxNodes_t bones;
	G_MaxEnv().m_BoneGroupDlg.GetBones(bones);

	CSkeletonSetDlg SSDlg(1.0f);
	SSDlg.BuildNodeTree(bones);
	SSDlg.DoModal();
	return true;
}

bool On_SetBonesGroup(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	if( ! IsWindow(G_MaxEnv().m_BoneGroupDlg.GetSafeHwnd()) )
	{
		G_MaxEnv().m_BoneGroupDlg.Create(CSetBoneGroupDlg::IDD);
		G_MaxEnv().m_BoneGroupDlg.hide();
	}

	
	if(IsWindowVisible(G_MaxEnv().m_BoneGroupDlg.GetSafeHwnd()))
	{
        G_MaxEnv().m_BoneGroupDlg.hide();
	}
	else
	{
        G_MaxEnv().m_BoneGroupDlg.show();
	}
	return true;
}

bool On_SetSkinGroup(HWND hDlg,WPARAM wParam,LPARAM lParam)
{
	if( ! IsWindow(G_MaxEnv().m_SkinGroupDlg.GetSafeHwnd()) )
	{
		G_MaxEnv().m_SkinGroupDlg.Create(CSetSkinGroupDlg::IDD);
		G_MaxEnv().m_SkinGroupDlg.hide();
	}


	if(IsWindowVisible(G_MaxEnv().m_SkinGroupDlg.GetSafeHwnd()))
	{
		G_MaxEnv().m_SkinGroupDlg.hide();
	}
	else
	{
		G_MaxEnv().m_SkinGroupDlg.show();
	}
	return true;
}
