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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Medusa.h"

#include "MainFrm.h"
#include "../Editor/xMedusaEditorImpl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
USING_NS_MDED
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	//ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)

	ON_COMMAND_RANGE          (ID_VIEW_DOCKPANE ,ID_VIEW_DOCKPANE + ID_VIEW_DOCKPANE_MAX, &CMainFrame::OnDockPaneRange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_DOCKPANE ,ID_VIEW_DOCKPANE + ID_VIEW_DOCKPANE_MAX, &CMainFrame::OnUpdateDockPaneRange)

	ON_COMMAND_RANGE          (ID_PLUGIN_ID_0 ,ID_PLUGIN_ID_0 + ID_PLUGIN_ID_MAX, &CMainFrame::OnPluginCmdRange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_PLUGIN_ID_0 ,ID_PLUGIN_ID_0 + ID_PLUGIN_ID_MAX, &CMainFrame::OnUpdatePluginCmdRange)
END_MESSAGE_MAP()

// CMainFrame construction/destruction


IMEdUIStatusBarItem* CStatusBarInterface::FindItem(const wchar_t* pName)
{
	return this->m_pMainFrame->FindItem(pName);
}

bool CStatusBarInterface::DeleteItem(IMEdUIStatusBarItem* pItem)
{
	return this->m_pMainFrame->DeleteItem(pItem);
}

IMEdUIStatusBarItem*  CStatusBarInterface::InsertItem(const wchar_t* pName , IMEdUIStatusBarItem::StatusBarItemType _type)
{
	return this->m_pMainFrame->InsertItem(pName , _type);
}

bool CStatusBarInterface::InsertSeperator()
{
	return this->m_pMainFrame->InsertSeperator();
}

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
	m_StatusBarImpl.m_pMainFrame = this;
}

CMainFrame::~CMainFrame()
{
}


IMEdUIStatusBarItem*   CMainFrame::FindItem(const wchar_t* pName)
{
	return NULL;
}

bool                   CMainFrame::DeleteItem(IMEdUIStatusBarItem* pItem)
{
	return false;
}

IMEdUIStatusBarItem*   CMainFrame::InsertItem(const wchar_t* pName , IMEdUIStatusBarItem::StatusBarItemType _type)
{
	return NULL;
}

bool                   CMainFrame::InsertSeperator()
{
	return false;
}

bool  CMainFrame::AttachToolBar(HWND hToolBar)
{
	 CMFCToolBar* toolBar =  new CMFCToolBar();
	 toolBar->Attach(hToolBar);
	 toolBar->SetWindowText(_TRANSLATE(L"Standard Toolbar"));
	 //m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, _TRANSLATE(L"Customize Toolbar"));

	 // Allow user-defined toolbars operations:
	 //InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);
	 toolBar->EnableDocking(CBRS_ALIGN_ANY);
	 DockPane(toolBar);
	 return true;
}

bool  CMainFrame::DetachToolBar(HWND hToolBar)
{

	return true;
}

bool  CMainFrame::DetachDockPane(nsMedusaEditor::IMEdDockPane* pPane)
{
	std::vector<CDockPaneContainer*>::iterator pos = m_DockPanes.begin();
	if(pPane == NULL)
	{
		for(size_t i = 0 ; i < m_DockPanes.size() ; i ++)
		{
			m_DockPanes[i]->GetMEdDockPane()->destroyMEdUI();
		}
		m_DockPanes.clear();
		return true;
	}


	for(; pos != m_DockPanes.end() ; pos ++)
	{
		if( (*pos)->GetMEdDockPane() == pPane)
		{
			pPane->destroyMEdUI();
			m_DockPanes.erase(pos);
			return true;
		}
	}
	return false;


}


bool  CMainFrame::AttachDockPane(nsMedusaEditor::IMEdDockPane* pPane)
{
	CDockPaneContainer* pPaneContainer = new CDockPaneContainer();
	pPaneContainer->SetMEdDockPane(pPane);
	UINT dockPaneID = m_DockPaneIDMgr.getID()   + ID_VIEW_DOCKPANE ;
	pPaneContainer->setID(dockPaneID);
	//---------
	// Create properties window
	if (!pPaneContainer->Create(pPaneContainer->title(), this, CRect(0, 0, 200, 200), TRUE, dockPaneID , WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		return FALSE; // failed to create
	}
	pPaneContainer->SetIcon( pPaneContainer->hIcon(), TRUE );
	EnableDocking(CBRS_ALIGN_ANY);
	pPaneContainer->EnableDocking(CBRS_ALIGN_ANY);
	DockPane(pPaneContainer);// , dockPaneID);
	m_DockPanes.push_back(pPaneContainer);

	//CMFCRibbonPanel* pVewPanel = m_Toolbar.FindPanel(L"Home");
	//if(pVewPanel)
	//{
	//	 CMFCRibbonButton* pBtnPaneViewBar = new CMFCRibbonCheckBox(dockPaneID, pPaneContainer->title() ); 
	//	 pVewPanel->Add(pBtnPaneViewBar);
	//	 pPaneContainer->SetViewBarButton(pBtnPaneViewBar);
	//}

	//m_ctRibbonView->
	PostMessage(WM_SIZE , 0 , 0 );
	return true;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = FALSE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	// Allow user-defined toolbars operations:
	//InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//-------------------------------------------------------------
	//状态栏
	CMFCRibbonButton* pButton= new CMFCRibbonButton(1001,L"查找属性");
	m_wndStatusBar.AddElement(pButton , L"Test" , true);
	m_wndStatusBar.AddSeparator();
	CMFCRibbonLabel* pLabel = new CMFCRibbonLabel(L"坐标：经度=101.0,纬度=35.0");
	m_wndStatusBar.AddElement(pLabel , L"Test" , true);
	//-------------------------------------------------------------

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);



	// Create a caption bar:
	//if (!CreateCaptionBar())
	//{
	//	TRACE0("Failed to create caption bar\n");
	//	return -1;      // fail to create
	//}


	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);


	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);
	
	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, _TRANSLATE(L"Customize"), ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();
	

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	//GetEditorEnv()->OnMainWindowCreate();
	 //m_ctRibbonView->
	return true;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}



//BOOL CMainFrame::CreateCaptionBar()
//{
//	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
//	{
//		TRACE0("Failed to create caption bar\n");
//		return FALSE;
//	}
//
//	BOOL bNameValid;
//
//	CString strTemp, strTemp2;
//	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
//	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetButtonToolTip(strTemp);
//
//	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);
//
//	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
//	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
//	ASSERT(bNameValid);
//	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
//	ASSERT(bNameValid);
//	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);
//
//	return TRUE;
//}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

//void CMainFrame::OnViewCaptionBar()
//{
//	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
//	RecalcLayout(FALSE);
//}
//
//void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
//{
//	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
//}

void  CMainFrame::OnDockPaneRange(UINT id)
{
    for(size_t i = 0 ; i < m_DockPanes.size() ; i ++)
	{
		if(m_DockPanes[i]->getID() == id)
		{
			CPane* pWnd = m_DockPanes[i] ;
			pWnd->ShowWindow(pWnd->IsVisible() ? SW_HIDE : SW_SHOW);
			pWnd->UpdateWindow();
			CMainFrame::OnWndMsg(WM_SIZE , 0 , 0 , NULL);
		}
	}
}

void  CMainFrame::OnUpdateDockPaneRange(CCmdUI* pCmdUI)
{
	for(size_t i = 0 ; i < m_DockPanes.size() ; i ++)
	{
		if(m_DockPanes[i]->getID() == pCmdUI->m_nID )
		{
			CPane* pWnd = m_DockPanes[i] ;
			pCmdUI->SetCheck(pWnd->IsVisible());
		}
	}
	
}

void CMainFrame::OnPluginCmdRange(UINT id)
{
    CMEdUiToolBarInfo* pToolbar = GetMedusaEditor()->GetUI()->FindToolbarByCmdID(id);
	if(pToolbar)
	{
		if(pToolbar->m_funcCallback)
		{
			int cmdId = pToolbar->GetCommandID(id);
			int idx = id - pToolbar->m_startID;
			pToolbar->m_funcCallback->OnCommand(cmdId ,  idx);
		}
	}
}

void CMainFrame::OnUpdatePluginCmdRange(CCmdUI* pCmdUI)
{
	int id = pCmdUI->m_nID;
	CMEdUiToolBarInfo* pToolbar = GetMedusaEditor()->GetUI()->FindToolbarByCmdID(id);
	if(pToolbar)
	{
		if(pToolbar->m_funcCallback)
		{
			int cmdId = pToolbar->GetCommandID(id);
			int idx = id - pToolbar->m_startID;

			CMEdUiToolBarInfo::CommandUIStatus _status = pToolbar->m_funcCallback->OnUpdateCommandUI(cmdId ,  idx);
			switch(_status)
			{
			case CMEdUiToolBarInfo::CUS_CHECKED   :pCmdUI->SetCheck(TRUE) ; break;
			case CMEdUiToolBarInfo::CUS_UNCHECKED :pCmdUI->SetCheck(FALSE) ; break;
			case CMEdUiToolBarInfo::CUS_ENABLE    :pCmdUI->Enable(TRUE)  ; pCmdUI->SetCheck(FALSE) ; break;
			case CMEdUiToolBarInfo::CUS_DISABLE   :pCmdUI->Enable(FALSE) ; pCmdUI->SetCheck(FALSE) ; break;
			}
		}
	}
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, _TRANSLATE(L"Customize"));
		}
	}

	return TRUE;
}

BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
    if(message == WM_COMMAND)
	{
		UINT id = LOWORD(wParam);
		wParam = wParam;
	}
	return CMDIFrameWndEx::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL CMainFrame::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
    PostQuitMessage(0);
	return CMDIFrameWndEx::DestroyWindow();
}
