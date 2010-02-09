
#include "stdafx.h"
#include "ToolBarElement.h"
#include "../EditorFramework/MainFrm.h"
#include "..\Resource.h"
#include <BaseLib/xStringTable.h>
USING_NS_MDED
USING_NS_XEVOL3D

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define _MEdUI_NAME L"Toolbar";
#define _MEdUI_DESC L"Toolbar";
IMPL_OBJECT_FACTORY_NO_ARG(CMEdUiToolBar , IMEdUIElement, CMEdUiToolBarFactory, IMEdUIElementFactory , CMEdUIElFactoryMgr , _MEdUI_NAME , _MEdUI_DESC);

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar
const wchar_t* CMEdUiToolBar::typeName()
{
	return _MEdUI_NAME;
}
CMEdUiToolBar::CMEdUiToolBar(CMEdUiToolBarInfo* pMEUIToolbar) 
{
	m_ToolbarInfo = pMEUIToolbar;
	m_ResID = IDR_MAINFRAME_256;
}

CMEdUiToolBar::CMEdUiToolBar(UINT ID ) 
{
	m_ResID = ID;
	m_ToolbarInfo = NULL;
}

CMEdUiToolBar::~CMEdUiToolBar()
{
	m_ToolbarInfo->m_hToolbar = NULL;
}

bool  CMEdUiToolBar::onDetach()
{
	return true;
}

bool  CMEdUiToolBar::onAttach()
{
	m_wndToolBar.ShowWindow(SW_SHOW);
	m_wndToolBar.UpdateWindow();

	return true ;
}

bool  CMEdUiToolBar::onMEdUIEvent(eMEUIEvent _event , int param, int param2)
{
     return false;
}

bool  CMEdUiToolBar::createMEdUI(HWND hMainWnd)
{
	std::wstring _toolbarTitle = _TRANSLATE(L"Standard Toolbar");
	CWnd* pWnd = CWnd::FromHandle(hMainWnd);
	
	if(m_ToolbarInfo == NULL)
	{
		if (!m_wndToolBar.CreateEx(pWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndToolBar.LoadToolBar(m_ResID))
		{
			TRACE0("Failed to create toolbar\n");
			return false;      // fail to create
		}
	}
	else
	{
		HINSTANCE hInstance = AfxGetResourceHandle();
		m_ResID = m_ToolbarInfo->m_ResID;
		AfxSetResourceHandle( m_ToolbarInfo->m_hDll );
		if (!m_wndToolBar.CreateEx(pWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndToolBar.LoadToolBar(m_ToolbarInfo->m_ResID))
		{
			TRACE0("Failed to create toolbar\n");
			AfxSetResourceHandle(hInstance );
			return false;      // fail to create
		}
		AfxSetResourceHandle(hInstance );
		_toolbarTitle = m_ToolbarInfo->m_title;
	}

	m_wndToolBar.SetWindowText(_toolbarTitle.c_str() );
	//m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, _TRANSLATE(L"Customize Toolbar"));

	// Allow user-defined toolbars operations:
	//InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	CMainFrame* pFrame = (CMainFrame*)pWnd;
	pFrame->DockPane(&m_wndToolBar);

	CMFCToolBar::EnableQuickCustomization();
    return true;
}

bool  CMEdUiToolBar::destroyMEdUI()
{
    m_wndToolBar.DestroyWindow();
	return true;
}
bool  CMEdUiToolBar::HideMEdUI()
{
	m_wndToolBar.ShowWindow(SW_HIDE);
	return true;
}

bool  CMEdUiToolBar::ShowMEdUI()
{
	m_wndToolBar.ShowWindow(SW_SHOW);
	m_wndToolBar.UpdateWindow();
	return true;
}

HWND CMEdUiToolBar::hWnd()
{
	return m_wndToolBar.GetSafeHwnd();
}

const wchar_t* CMEdUiToolBar::title()
{
	std::wstring _toolbarTitle = _TRANSLATE(L"Standard Toolbar");
	if(m_ToolbarInfo )
	{
		_toolbarTitle = m_ToolbarInfo->m_title;
	}
	return _toolbarTitle.c_str();
}

const wchar_t* CMEdUiToolBar::name() 
{
	std::wstring _toolbarName = L"Standard";
	if(m_ToolbarInfo )
	{
		_toolbarName = m_ToolbarInfo->m_name;
	}
	return _toolbarName.c_str();
}



