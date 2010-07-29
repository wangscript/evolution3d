// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "DockPaneContainer.h"
#include <vector>
#include "BaseLib/xIDManager.h"
#include "../xMedusaEditor.h"
#include "RibbonBar.h"

class CMainFrame;
///Medusa主项目的状态栏
class CStatusBarItem
{
public:
	enum StatusBarItemType
	{
		SBIT_Label,
		SBIT_Button,
		SBIT_Checkbok,
		SBIT_Combox,
	};
private:
	nsMedusaEditor::IMEdUICommandReciver* m_pCallBack;
	CMFCRibbonBaseElement*                m_pElement;
};
typedef ds_map(int , IMEdUIElement*) vID2UIElement;

class CStatusBarInterface : public IMEdUIStatusBar
{
public:
	IMEdUIStatusBarItem* FindItem(const wchar_t* pName);
	bool                 DeleteItem(IMEdUIStatusBarItem* pItem);
	IMEdUIStatusBarItem* InsertItem(const wchar_t* pName , IMEdUIStatusBarItem::StatusBarItemType _type);
	bool                 InsertSeperator();
public:
	CMainFrame*           m_pMainFrame;

};

//////////////////////////////////////////////////////////////////////////
class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
	IMEdUIStatusBarItem* FindItem(const wchar_t* pName);
	bool                 DeleteItem(IMEdUIStatusBarItem* pItem);
	IMEdUIStatusBarItem* InsertItem(const wchar_t* pName , IMEdUIStatusBarItem::StatusBarItemType _type);
	bool                 InsertSeperator();
	IMEdUIStatusBar*     GetStatusBarImpl(){return &m_StatusBarImpl;}
    void                 ResetApplicationLook();
    void                 SetRibbonBar(CMFCRibbonBar* pRibbonBar);
	//CMFCRibbonBar*       GetRibbonBar();
// 操作
public:
	bool         AttachToolBar(HWND hToolBar);
	bool         DetachToolBar(HWND hToolBar);
	bool         AttachDockPane(nsMedusaEditor::IMEdDockPane* pPane);
	bool         DetachDockPane(nsMedusaEditor::IMEdDockPane* pPane);

    bool         AddUIElementToShowHideMenu(IMEdUIElement* pUIElement , int id = - 1);
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCRibbonStatusBar              m_wndStatusBar;
	////
	std::vector<CDockPaneContainer*> m_DockPanes;
	NS_XEVOL3D::xIDManager           m_DockPaneIDMgr;
	CStatusBarInterface              m_StatusBarImpl;
    CMFCCaptionBar                   m_wndCaptionBar;
    vID2UIElement                    m_mapUIElment;
    CMFCRibbonPanel*                 m_pShowHidePanelView;
// 生成的消息映射函数
protected:
    afx_msg void OnViewCustomize();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
    afx_msg void OnViewCaptionBar();
    afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);

    afx_msg void OnEditUndo();
    afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
    afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
    afx_msg void OnEditRedo();

	afx_msg void OnDockPaneRange(UINT id);
	afx_msg void OnUpdateDockPaneRange(CCmdUI* pCmdUI);

	afx_msg void OnPluginCmdRange(UINT id);
	afx_msg void OnUpdatePluginCmdRange(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

	
};


