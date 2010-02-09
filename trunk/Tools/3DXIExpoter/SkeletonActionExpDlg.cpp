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

// SkeletonActionExpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XEvolMaxExporter.h"
#include "SkeletonActionExpDlg.h"
#include ".\skeletonactionexpdlg.h"
#include "MaxEnv.h"
// CSkeletonActionExpDlg dialog

IMPLEMENT_DYNAMIC(CSkeletonActionExpDlg, CDialog)
CSkeletonActionExpDlg::CSkeletonActionExpDlg(sActionInfos_t& ActionList,CWnd* pParent /*=NULL*/)
	: CDialog(CSkeletonActionExpDlg::IDD, pParent)
    , m_iStartFrame(0)
    , m_EndFrame(0)
    , m_iDurationTime(0)
    , m_ActionList(ActionList)
{
}

CSkeletonActionExpDlg::~CSkeletonActionExpDlg()
{
}

void CSkeletonActionExpDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDL_ACTION_LIST, m_ctrlActionList);
    DDX_Text(pDX, IDC_FIRST_FRAME, m_iStartFrame);
    DDX_Text(pDX, IDC_LAST_FRAME, m_EndFrame);
    DDX_Text(pDX, IDC_DUARING_TIME, m_iDurationTime);
    DDX_Control(pDX, IDT_ACTION_NAME, m_ctrlActionName);
}


BEGIN_MESSAGE_MAP(CSkeletonActionExpDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_NOTIFY(LVN_ITEMCHANGED, IDL_ACTION_LIST, OnLvnItemchangedActionList)
    ON_BN_CLICKED(IDB_ACTION_ADD, OnBnClickedActionAdd)
    ON_BN_CLICKED(IDB_ACTION_REMOVE, OnBnClickedActionRemove)
    ON_BN_CLICKED(IDB_FINISH_SETUP, OnBnClickedFinishSetup)
END_MESSAGE_MAP()


// CSkeletonActionExpDlg message handlers

void CSkeletonActionExpDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    OnOK();
}

void CSkeletonActionExpDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}
int CSkeletonActionExpDlg::findAction(const wchar_t* actionName)
{
    for(size_t i = 0 ; i < m_ActionList.size() ; ++i)
    {
        sActionInfo_t& action = m_ActionList[i];
        if(std::wstring(actionName) == action.m_Name)
        {
            return (int)i;
        }
    }
    return -1;
}

void CSkeletonActionExpDlg::RemoveAction(int index)
{
    sActionInfos_t::iterator i = m_ActionList.begin();
    for(int ii = 0  ; i != m_ActionList.end() ; ++ii , ++i)
    {
        if(ii == index )
        {
            m_ActionList.erase(i);
            m_ctrlActionList.DeleteItem(index);
            return ;
        }
    }
}
BOOL CSkeletonActionExpDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    m_ctrlActionList.InsertColumn(0,"序号",LVCFMT_LEFT,30);
    m_ctrlActionList.InsertColumn(1,"名字",LVCFMT_LEFT,130);
    m_ctrlActionList.InsertColumn(2,"时间",LVCFMT_LEFT,30);
    m_ctrlActionList.InsertColumn(3,"第一帧",LVCFMT_LEFT,30);
    m_ctrlActionList.InsertColumn(4,"最后帧",LVCFMT_LEFT,30);

    for(size_t i = 0 ; i < m_ActionList.size() ; ++i)
    {
        InsertAction(m_ActionList[i],true);
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
bool CSkeletonActionExpDlg::InsertAction(sActionInfo_t& action,bool isInsertToUI)
{
    int index = findAction(action.m_Name);
    if(index != -1 && isInsertToUI == false)
    {
        ::MessageBoxA(::GetActiveWindow(),"相同名字的动作存在","错误",MB_OK);
        return false;
    }
    if(index == -1)
    {
       index = (int)m_ActionList.size();
       m_ActionList.push_back(action);
    }
    char buf[23];
    sprintf(buf,"%d",index);
    m_ctrlActionList.InsertItem(index,buf);
    m_ctrlActionList.SetItemText(index,1,ToAnsi(action.m_Name).c_str() );

    sprintf(buf,"%d",action.m_lTime);
    m_ctrlActionList.SetItemText(index,2,buf);

    sprintf(buf,"%d",action.m_iFirstFrame);
    m_ctrlActionList.SetItemText(index,3,buf);

    sprintf(buf,"%d",action.m_iLastFrame);
    m_ctrlActionList.SetItemText(index,4,buf);

    return true;
}
void CSkeletonActionExpDlg::OnLvnItemchangedActionList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
}

void CSkeletonActionExpDlg::OnBnClickedActionAdd()
{
    // TODO: Add your control notification handler code here
    sActionInfo_t action;
    UpdateData();
    action.m_ActionType  = ACTION_SKELENTON;
    action.m_iFirstFrame = m_iStartFrame;
    action.m_iLastFrame  = m_EndFrame;
    action.m_lTime       = m_iDurationTime;
	WGetWndText(&m_ctrlActionName , action.m_Name,32);

    if(wcslen(action.m_Name) == 0)
    {
        ::MessageBoxA(::GetActiveWindow(),"请填写动作名字","错误",MB_OK);
        return ;
    }
    if(m_iDurationTime <= 0)
    {
        ::MessageBoxA(::GetActiveWindow(),"动作时间不能为0","错误",MB_OK);
        return ;
    }
    InsertAction(action); 
    
}

void CSkeletonActionExpDlg::OnBnClickedActionRemove()
{
    POSITION pos = m_ctrlActionList.GetFirstSelectedItemPosition();
    int index = m_ctrlActionList.GetNextSelectedItem(pos);
    if(index >= 0 && index < (int)m_ActionList.size())
    {
        RemoveAction(index);
    }
}

void CSkeletonActionExpDlg::OnBnClickedFinishSetup()
{
    // TODO: Add your control notification handler code here
    OnOK();
}
