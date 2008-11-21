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

// FrameAniModelExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XEvolMaxExporter.h"
#include "FrameAniModelExportDlg.h"
#include ".\frameanimodelexportdlg.h"
#include "MeshPropertyDlg.h"


// CFrameAniModelExportDlg dialog

IMPLEMENT_DYNAMIC(CFrameAniModelExportDlg, CDialog)
CFrameAniModelExportDlg::CFrameAniModelExportDlg(sFrameAniModelExpCtx_t& ExpContex,CWnd* pParent /*=NULL*/)
	: CDialog(CFrameAniModelExportDlg::IDD, pParent)
    , m_bTexCooesAni(FALSE)
    , m_bPositionAni(FALSE)
    , m_uiActionTime(0)
    , m_uiFirstFrame(0)
    , m_uiLastFrame(0)
    , m_bExportToPackage(FALSE)
    , m_fScaleFactor(1.0)
    , m_ExpContex(ExpContex)
    , m_bTextMode(FALSE)
    , m_bIsTextMode(FALSE)
{
}

CFrameAniModelExportDlg::~CFrameAniModelExportDlg()
{
}

void CFrameAniModelExportDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDL_ACTION_LIST, m_ctrlActionList);
    DDX_Check(pDX, IDC_TEXTURECOOR_ANI, m_bTexCooesAni);
    DDX_Check(pDX, IDC_POSITION_ANI, m_bPositionAni);
    DDX_Control(pDX, IDT_ACTION_NAME, m_ctrlActionName);
    DDX_Text(pDX, IDC_DUARING_TIME, m_uiActionTime);
    DDX_Text(pDX, IDC_FIRST_FRAME, m_uiFirstFrame);
    DDX_Text(pDX, IDC_LAST_FRAME, m_uiLastFrame);
    DDX_Control(pDX, IDC_PACKAGE_PATH, m_ctrlPackagePath);
    DDX_Check(pDX, IDC_EXPORT_TO_PACKAGE, m_bExportToPackage);
    DDX_Text(pDX, IDC_SCALE_FACTOR, m_fScaleFactor);
    DDX_Control(pDX, IDT_MODEL_NAME, m_ctrlModelName);
    DDX_Check(pDX, IDC_EXPTEXTMODE, m_bTextMode);
    DDX_Check(pDX, IDC_EXPORT_TEXTMODE, m_bIsTextMode);
}


BEGIN_MESSAGE_MAP(CFrameAniModelExportDlg, CDialog)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDB_CANCEL_EXPORT, OnBnClickedCancelExport)
    ON_BN_CLICKED(IDB_EXPORT, OnBnClickedExport)
    ON_BN_CLICKED(IDB_ACTION_ADD, OnBnClickedActionAdd)
    ON_BN_CLICKED(IDB_ACTION_REMOVE, OnBnClickedActionRemove)
    ON_BN_CLICKED(IDB_MESH_PROPERTY, OnBnClickedMeshProperty)
END_MESSAGE_MAP()


// CFrameAniModelExportDlg message handlers

void CFrameAniModelExportDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}

void CFrameAniModelExportDlg::OnBnClickedCancelExport()
{
    OnCancel();
}

void CFrameAniModelExportDlg::OnBnClickedExport()
{
    UpdateData();
    m_ExpContex.m_bExpPackage = m_bExportToPackage != FALSE;
    m_ExpContex.m_bTextMode   = m_bIsTextMode  != FALSE;
    m_ExpContex.m_fModelScale = 1.0f/m_fScaleFactor;
	WGetWndText(&m_ctrlModelName , m_ExpContex.m_ModelDesc.m_Name,32);
    OnOK();
}

void CFrameAniModelExportDlg::OnBnClickedActionAdd()
{
    // TODO: Add your control notification handler code here
    sMaxActionInfo_t action;
    UpdateData();
    action.m_BaseInfo.m_ActionType = ACTION_FA_MODEL;
    action.m_BaseInfo.m_iFirstFrame = m_uiFirstFrame;
    action.m_BaseInfo.m_iLastFrame  = m_uiLastFrame;
    action.m_BaseInfo.m_lTime       = m_uiActionTime;
	WGetWndText(&m_ctrlActionName ,action.m_BaseInfo.m_Name,32);
    action.m_bPositionAni = m_bPositionAni!=FALSE;
    action.m_bTexCoordAni = m_bTexCooesAni!=FALSE;

    if(m_bPositionAni == FALSE && m_bTexCooesAni == FALSE)
    {
        ::MessageBoxA(::GetActiveWindow(),"顶点动画和纹理动画必须设置其中一个","错误",MB_OK);
        return ;
    }
    if(wcslen(action.m_BaseInfo.m_Name) == 0)
    {
        ::MessageBoxA(::GetActiveWindow(),"请填写动作名称","错误",MB_OK);
        return ;
    }
    if(m_uiActionTime <= 0)
    {
        ::MessageBoxA(::GetActiveWindow(),"动作时间不能为0","错误",MB_OK);
        return ;
    }
    InsertAction(action); 
    m_ExpContex.m_Actions.push_back(action);
}

int CFrameAniModelExportDlg::findAction(const wchar_t* actionName)
{
     for(size_t i = 0 ; i < m_ExpContex.m_Actions.size() ; ++i)
     {
         sMaxActionInfo_t& action = m_ExpContex.m_Actions[i];
         if(wstring(actionName) == action.m_BaseInfo.m_Name)
         {
             return (int)i;
         }
     }
     return -1;
}
bool CFrameAniModelExportDlg::InsertAction(sMaxActionInfo_t& action)
{
     if(findAction(action.m_BaseInfo.m_Name) != -1)
     {
         ::MessageBoxA(::GetActiveWindow(),"相同名字的动作存在","错误",MB_OK);
         return false;
     }
     int index = (int)m_ExpContex.m_Actions.size();
     
     char buf[23];
     sprintf(buf,"%d",index);
     m_ctrlActionList.InsertItem(index,buf);
     m_ctrlActionList.SetItemText(index,1, ToAnsi(action.m_BaseInfo.m_Name).c_str() );

     sprintf(buf,"%d",action.m_BaseInfo.m_lTime);
     m_ctrlActionList.SetItemText(index,2,buf);

     sprintf(buf,"%d",action.m_BaseInfo.m_iFirstFrame);
     m_ctrlActionList.SetItemText(index,3,buf);

     sprintf(buf,"%d",action.m_BaseInfo.m_iLastFrame);
     m_ctrlActionList.SetItemText(index,4,buf);

     if(action.m_bPositionAni == true)
         m_ctrlActionList.SetItemText(index,5,"是");
     else
         m_ctrlActionList.SetItemText(index,5,"否");

     if(action.m_bTexCoordAni == true)
         m_ctrlActionList.SetItemText(index,6,"是");
     else
         m_ctrlActionList.SetItemText(index,6,"否");
     return true;
}

void CFrameAniModelExportDlg::RemoveAction(int index)
{
    MaxFrameActionInfos_t::iterator i = m_ExpContex.m_Actions.begin();
    for(int ii = 0  ; i != m_ExpContex.m_Actions.end() ; ++ii , ++i)
    {
        if(ii == index )
        {
            m_ExpContex.m_Actions.erase(i);
            m_ctrlActionList.DeleteItem(index);
            return ;
        }
    }
}

BOOL CFrameAniModelExportDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_ctrlActionList.InsertColumn(0,"序号",LVCFMT_LEFT,30);
    m_ctrlActionList.InsertColumn(1,"名字",LVCFMT_LEFT,130);
    m_ctrlActionList.InsertColumn(2,"时间",LVCFMT_LEFT,30);
    m_ctrlActionList.InsertColumn(3,"第一帧",LVCFMT_LEFT,30);
    m_ctrlActionList.InsertColumn(4,"最后帧",LVCFMT_LEFT,30);
    m_ctrlActionList.InsertColumn(5,"顶点动画",LVCFMT_LEFT,30);
    m_ctrlActionList.InsertColumn(6,"纹理动画",LVCFMT_LEFT,30);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CFrameAniModelExportDlg::OnBnClickedActionRemove()
{
    // TODO: Add your control notification handler code here
    POSITION pos = m_ctrlActionList.GetFirstSelectedItemPosition();
    int index = m_ctrlActionList.GetNextSelectedItem(pos);
    if(index >= 0 && index < (int)m_ExpContex.m_Actions.size())
    {
        RemoveAction(index);
    }
}

void CFrameAniModelExportDlg::OnBnClickedMeshProperty()
{
    // TODO: Add your control notification handler code here
    CMeshPropertyDlg dlg(m_ExpContex.m_MaxMeshs);
    dlg.DoModal();
}
