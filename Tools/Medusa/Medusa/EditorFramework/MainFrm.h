// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "..\Resource.h"
#include "DockPaneContainer.h"
#include <vector>
#include "BaseLib/xIDManager.h"
#include "..\xMedusaEditor.h"
//#include "RibbonToolBar.h"


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


class CMainFrame;
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

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  
	CMFCToolBarImages m_UserImages;

	//CMFCCaptionBar              m_wndCaptionBar;
	CMFCRibbonStatusBar         m_wndStatusBar;
public:
	bool         AttachToolBar(HWND hToolBar);
	bool         DetachToolBar(HWND hToolBar);
	bool         AttachDockPane(nsMedusaEditor::IMEdDockPane* pPane);
	bool         DetachDockPane(nsMedusaEditor::IMEdDockPane* pPane);
	//BOOL         CreateCaptionBar();
public:
	std::vector<CDockPaneContainer*> m_DockPanes;
	NS_XEVOL3D::xIDManager           m_DockPaneIDMgr;
// Generated message map functions
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	//afx_msg void OnViewCaptionBar();
	//afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnDockPaneRange(UINT id);
	afx_msg void OnUpdateDockPaneRange(CCmdUI* pCmdUI);

	afx_msg void OnPluginCmdRange(UINT id);
	afx_msg void OnUpdatePluginCmdRange(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()


	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	virtual BOOL DestroyWindow();

	 IMEdUIStatusBarItem* FindItem(const wchar_t* pName);
	 bool                 DeleteItem(IMEdUIStatusBarItem* pItem);
	 IMEdUIStatusBarItem* InsertItem(const wchar_t* pName , IMEdUIStatusBarItem::StatusBarItemType _type);
	 bool                 InsertSeperator();

	 IMEdUIStatusBar*     GetStatusBarImpl(){return &m_StatusBarImpl;}
protected:
	CStatusBarInterface m_StatusBarImpl;
};


