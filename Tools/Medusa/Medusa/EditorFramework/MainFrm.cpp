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

// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Medusa.h"

#include "MainFrm.h"
#include "../Editor/xMedusaEditorImpl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
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

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
    ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
    ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
    ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)

	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_FILE_PRINT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMainFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnUpdateFilePrintPreview)

    ON_COMMAND_RANGE          (ID_VIEW_DOCKPANE ,ID_VIEW_DOCKPANE + ID_VIEW_DOCKPANE_MAX, &CMainFrame::OnDockPaneRange)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_DOCKPANE ,ID_VIEW_DOCKPANE + ID_VIEW_DOCKPANE_MAX, &CMainFrame::OnUpdateDockPaneRange)
	ON_COMMAND_RANGE          (ID_PLUGIN_ID_0 ,ID_PLUGIN_ID_0 + ID_PLUGIN_ID_MAX, &CMainFrame::OnPluginCmdRange)
	ON_UPDATE_COMMAND_UI_RANGE(ID_PLUGIN_ID_0 ,ID_PLUGIN_ID_0 + ID_PLUGIN_ID_MAX, &CMainFrame::OnUpdatePluginCmdRange)

    // Edit
    ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnEditUndo)
    ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CMainFrame::OnUpdateEditUndo)
    ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CMainFrame::OnUpdateEditRedo)
    ON_COMMAND(ID_EDIT_REDO, &CMainFrame::OnEditRedo)

	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_AQUA);
	m_StatusBarImpl.m_pMainFrame = this;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}


    
    EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, L"配置", ID_VIEW_TOOLBAR);
	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	theApp.OnMainWindowCreate(this);


    //
    if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
    {
        TRACE0("未能创建标题栏\n");
        return FALSE;
    }
    m_wndCaptionBar.SetButton(L"选项", ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
    m_wndCaptionBar.SetButtonToolTip(L"单击此处查看其他选项");


    m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
    m_wndCaptionBar.SetImageToolTip(L"提示", L"请勿关闭");

    //-------------------------------------------------------------
    //状态栏
    CMFCRibbonButton* pButton= new CMFCRibbonButton(1001,L"查找属性");
    m_wndStatusBar.AddElement(pButton , L"Test" , true);
    m_wndStatusBar.AddSeparator();
    CMFCRibbonLabel* pLabel = new CMFCRibbonLabel(L"坐标：经度=101.0,纬度=35.0");
    m_wndStatusBar.AddElement(pLabel , L"Test" , true);
    //-------------------------------------------------------------

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序
void CMainFrame::ResetApplicationLook()
{
    switch (theApp.m_nAppLook)
    {
    case ID_VIEW_APPLOOK_WIN_2000:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
        if( GetRibbonBar() ) 
            GetRibbonBar()->SetWindows7Look(FALSE);
        break;

    case ID_VIEW_APPLOOK_OFF_XP:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
        if( GetRibbonBar() ) 
            GetRibbonBar()->SetWindows7Look(FALSE);
        break;

    case ID_VIEW_APPLOOK_WIN_XP:
        CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
        if( GetRibbonBar() ) 
            GetRibbonBar()->SetWindows7Look(FALSE);
        break;

    case ID_VIEW_APPLOOK_OFF_2003:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
        CDockingManager::SetDockingMode(DT_SMART);
        if( GetRibbonBar() ) 
            GetRibbonBar()->SetWindows7Look(FALSE);
        break;

    case ID_VIEW_APPLOOK_VS_2005:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
        CDockingManager::SetDockingMode(DT_SMART);
        if( GetRibbonBar() ) 
            GetRibbonBar()->SetWindows7Look(FALSE);
        break;

    case ID_VIEW_APPLOOK_VS_2008:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
        CDockingManager::SetDockingMode(DT_SMART);
        if( GetRibbonBar() ) 
            GetRibbonBar()->SetWindows7Look(FALSE);
        break;

    case ID_VIEW_APPLOOK_WINDOWS_7:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
        CDockingManager::SetDockingMode(DT_SMART);
        if( GetRibbonBar() ) 
            GetRibbonBar()->SetWindows7Look(TRUE);
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
        if( GetRibbonBar() ) 
            GetRibbonBar()->SetWindows7Look(FALSE);
    }

    
    RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

}
void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;
	theApp.m_nAppLook = id;
    ResetApplicationLook();
	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnFilePrint()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CMainFrame::OnFilePrintPreview()
{
	if (IsPrintPreview())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // 强制关闭“打印预览”模式
	}
}

void CMainFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(IsPrintPreview());
}

void CMainFrame::OnViewCustomize()
{
    CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
    pDlgCust->EnableUserDefinedToolbars();
    pDlgCust->Create();
}

void CMainFrame::OnViewCaptionBar()
{
    m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
    RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
    if(::IsWindow(m_wndCaptionBar.GetSafeHwnd()) == false)
    {
        return ;
    }
    pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

////////////////////////////////////////////
///自定义函数从这里开始
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
void CMainFrame::SetRibbonBar(CMFCRibbonBar* pRibbonBar)
{
    //m_Impl.setR = pRibbonBar;
    CMFCRibbonCategory* pCategoryHome = pRibbonBar->GetCategory(1);
    CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel(L"显示/隐藏" );
    CMFCRibbonButton* pBtnStatusBar = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, L"状态栏");
    pPanelView->Add(pBtnStatusBar);
    CMFCRibbonButton* pBtnCaptionBar = new CMFCRibbonCheckBox(ID_VIEW_CAPTION_BAR, L"通知栏");
    pPanelView->Add(pBtnCaptionBar);
    m_pShowHidePanelView = pPanelView;

}

bool  CMainFrame::AddUIElementToShowHideMenu(IMEdUIElement* pUIElement , int id)
{
    if(pUIElement->hWnd() == NULL )
        return false;

    if(id == - 1)
        id = m_DockPaneIDMgr.getID()   + ID_VIEW_DOCKPANE;
    m_mapUIElment[id] = pUIElement;

    const wchar_t* pTitle = pUIElement->title();
    CMFCRibbonButton* pBtn = new CMFCRibbonCheckBox(id , pTitle);
    m_pShowHidePanelView->Add(pBtn);
    GetRibbonBar()->ForceRecalcLayout();
    GetRibbonBar()->InvalidateRect(NULL);
    return true;
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
    AddUIElementToShowHideMenu(pPane , dockPaneID);
	//m_ctRibbonView->
	PostMessage(WM_SIZE , 0 , 0 );
	return true;
}

void  CMainFrame::OnDockPaneRange(UINT id)
{
	for(size_t i = 0 ; i < m_DockPanes.size() ; i ++)
	{
		if(m_DockPanes[i]->getID() == id)
		{
			CPane* pWnd = m_DockPanes[i] ;
            BOOL bShow = ! pWnd->IsVisible();

            ShowPane(pWnd , bShow , FALSE , bShow);
			//pWnd->ShowWindow(pWnd->IsVisible() ? SW_HIDE : SW_SHOW);
			//pWnd->UpdateWindow();
			CMainFrame::OnWndMsg(WM_SIZE , 0 , 0 , NULL);
            RecalcLayout();
            return ;
		}
	}
    if(m_mapUIElment.find(id) == m_mapUIElment.end() )
        return ;

    IMEdUIElement* pUIElement = m_mapUIElment[id];
    if(pUIElement )
    {
        if(pUIElement->IsVisible() == false ) 
            pUIElement->ShowMEdUI( );
        else
            pUIElement->HideMEdUI();
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
            return ;
		}
	}

    if(m_mapUIElment.find(pCmdUI->m_nID) == m_mapUIElment.end() )
        return ;

    IMEdUIElement* pUIElement = m_mapUIElment[pCmdUI->m_nID];
    if(pUIElement )
    {
        pCmdUI->SetCheck( pUIElement->IsVisible() );
    }
}

void CMainFrame::OnPluginCmdRange(UINT id)
{
	CMEdUiToolBarInfo* pToolbar = GetMedusaEditor()->GetUI()->FindToolbarByCmdID(id);
	if(pToolbar)
	{
		if(pToolbar->m_funcCallback)
		{
			int cmdId = pToolbar->GetOriginID(id);
			int idx = id - pToolbar->m_startID;
			if( false == pToolbar->m_funcCallback->OnCommand(cmdId ,  idx) )
			{
				OnWndMsg(WM_COMMAND , cmdId , 0 , NULL);
			}
		}
	}
}
class     CMEdUiCmdUiImp : public CMEdUiToolBarInfo::CMEdUiCmdUi
{
	CCmdUI* m_pCmdUi;
public:
	CMEdUiCmdUiImp(CCmdUI* pUI) { m_pCmdUi = pUI ; }
	virtual void Enable(BOOL bOn = TRUE){ return m_pCmdUi->Enable(bOn) ; }
	virtual void SetCheck(int nCheck = 1){ return m_pCmdUi->SetCheck(nCheck) ; }
	virtual void SetRadio(BOOL bOn = TRUE){ return m_pCmdUi->SetRadio(bOn) ; }
	virtual void SetText(LPCTSTR lpszText){ return m_pCmdUi->SetText(lpszText) ; }
};


void CMainFrame::OnUpdatePluginCmdRange(CCmdUI* pCmdUI)
{
	int id = pCmdUI->m_nID;
	CMEdUiToolBarInfo* pToolbar = GetMedusaEditor()->GetUI()->FindToolbarByCmdID(id);
	if(pToolbar)
	{
		if(pToolbar->m_funcCallback)
		{
			int cmdId = pToolbar->GetOriginID(id);
			int idx = id - pToolbar->m_startID;

			CMEdUiCmdUiImp cmdUi(pCmdUI);
			if(false == pToolbar->m_funcCallback->OnUpdateCommandUI(cmdId ,  idx , &cmdUi) )
			{
				pCmdUI->m_nID = cmdId;
				OnCmdMsg(cmdId , -1 , pCmdUI , NULL);
				pCmdUI->m_nID = id;
			}
		}
	}
}


void CMainFrame::OnEditUndo()
{
    // TODO: Add your command handler code here
    theApp.GetEditor().GetUI()->Undo();
}

void CMainFrame::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
    if(theApp.GetEditor().GetUI()->CanUndo() )
    {
        pCmdUI->Enable(TRUE);
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
    // TODO: Add your command update UI handler code here
}

void CMainFrame::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
    if(theApp.GetEditor().GetUI()->CanRedo() )
    {
        pCmdUI->Enable(TRUE);
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
    // TODO: Add your command update UI handler code here
}

void CMainFrame::OnEditRedo()
{
    theApp.GetEditor().GetUI()->Redo();
    // TODO: Add your command handler code here
}

BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == WM_COMMAND)
	{
		UINT id = LOWORD(wParam);
		wParam = wParam;
	}
	return __super::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL CMainFrame::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return __super::DestroyWindow();
}


void CMainFrame::OnDestroy()
{
	((CMedusaMainUI*)(GetMedusaEditor()->GetUI()))->OnExit();
	PostQuitMessage(0);
	__super::OnDestroy();

	// TODO: Add your message handler code here
}
