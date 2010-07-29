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

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DataDefine.h"
#include <set>
// CSkeletonActionExpDlg dialog
using namespace XEvol3DModel;
struct xActionXMLInfo
{
    ds_wstring m_Name;
    ds_wstring m_MaxFile;
    int        m_nFirst;
    int        m_nLast;
    int        m_DurTime;
public:
    bool operator == (const xActionXMLInfo& _info)
    {
        return  (_info.m_MaxFile == m_MaxFile &&
                 _info.m_Name    == m_Name);
    }
};

typedef std::vector<xActionXMLInfo> vActionXMLInfos;
inline void Remove(vActionXMLInfos& _List , const xActionXMLInfo& info)
{
    vActionXMLInfos::iterator pos = _List.begin();
    for(size_t i = 0 ; i < _List.size() ; i ++ , pos ++)
    {
        if(_List[i] == info)
        {
            _List.erase(pos);
            return ;
        }
    }
}

inline void Remove(vActionXMLInfos& _List , const vActionXMLInfos& _List2)
{
    vActionXMLInfos::iterator pos = _List.begin();
    for(size_t i = 0 ; i < _List2.size() ; i ++ )
    {
        Remove(_List , _List2[i]);
    }
}

class CSkeletonActionExpDlg : public CDialog
{
	DECLARE_DYNAMIC(CSkeletonActionExpDlg)
    sActionInfos_t& m_ActionList;
	typedef std::set<wstring> vOperatedActions;
	vOperatedActions  m_OperatedActions;
public:
	CSkeletonActionExpDlg(sActionInfos_t& ActionList,CWnd* pParent = NULL);   // standard constructor
	virtual ~CSkeletonActionExpDlg();
    void            GetOperatedAction(sActionInfos_t& ActionList);
    const wchar_t*  ActionListFile(){ return m_XmlFile ; }
    void            LoadActionList(const wchar_t* fileName,  bool FilterByFile);
    void            LoadActionList(bool FilterByFile );
// Dialog Data
	enum { IDD = IDD_SKELETON_ACTION_EXP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool         InsertActionToUI(sActionInfo_t& action);
    bool         InsertAction(sActionInfo_t& action,bool isInsertToUI = false);
    int          findAction(const wchar_t* actionName);
    void         RemoveAction(int index);
    int          GetActionType();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnLvnItemchangedActionList(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedActionAdd();
    afx_msg void OnBnClickedActionRemove();
    afx_msg void OnBnClickedFinishSetup();
	afx_msg void OnBnClickedExportToSignle();
    
    vActionXMLInfos m_ActionXmlList;
public:
	int       m_iStartFrame;
	int       m_EndFrame;
	int       m_iDurationTime;
	int       m_bExpToSingleFile;
protected:
	CListCtrl m_ctrlActionList;
	CEdit     m_ctrlActionName;
    bool      m_bHasUI;
    wchar_t   m_XmlFile[1024];
public:
    afx_msg void OnBnClickedActionLoad();
    afx_msg void OnBnClickedActionSave();
    BOOL         m_bKeyFrame;
};
