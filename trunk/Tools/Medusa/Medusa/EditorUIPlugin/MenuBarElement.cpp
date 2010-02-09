
#include "stdafx.h"
#include "MenuBarElement.h"
#include "../EditorFramework/MainFrm.h"
#include "..\Resource.h"
#include <BaseLib/xStringTable.h>
USING_NS_XEVOL3D;
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define _MEdUI_NAME L"MainMenubar";
#define _MEdUI_DESC L"MainMenu Bar";
IMPL_OBJECT_FACTORY_NO_ARG(CMEdUiMenuBar , IMEdUIElement, CMEdUiMenuBarFactory, IMEdUIElementFactory , CMEdUIElFactoryMgr , _MEdUI_NAME , _MEdUI_DESC);
/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar
const wchar_t* CMEdUiMenuBar::typeName()
{
	return _MEdUI_NAME;
}
CMEdUiMenuBar::CMEdUiMenuBar() 
{
}

CMEdUiMenuBar::~CMEdUiMenuBar()
{
}

bool  CMEdUiMenuBar::onDetach()
{
	return true;
}

bool  CMEdUiMenuBar::onAttach()
{
	m_wndMenuBar.ShowWindow(SW_SHOW);
	m_wndMenuBar.UpdateWindow();
	return true ;
}

bool  CMEdUiMenuBar::onMEdUIEvent(eMEUIEvent _event , int param, int param2)
{
	return false;
}

bool  CMEdUiMenuBar::createMEdUI(HWND hMainWnd)
{
	CWnd* pWnd = CWnd::FromHandle(hMainWnd);
	if (!m_wndMenuBar.Create(pWnd))
	{
		TRACE0("Failed to create menubar\n");
		return false;      // fail to create
	}
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndMenuBar.ShowPane(TRUE , FALSE , TRUE);
	CMenu* pMenu = m_wndMenuBar.GetMenu();
	CMainFrame* pFrame = (CMainFrame*)pWnd;
	pFrame->DockPane(&m_wndMenuBar);
	return true;
}

bool  CMEdUiMenuBar::destroyMEdUI()
{
	m_wndMenuBar.DestroyWindow();
	return true;
}
bool  CMEdUiMenuBar::HideMEdUI()
{
	m_wndMenuBar.ShowWindow(SW_HIDE);
	return true;
}

bool  CMEdUiMenuBar::ShowMEdUI()
{
	m_wndMenuBar.ShowWindow(SW_SHOW);
	m_wndMenuBar.UpdateWindow();
	return true;
}

HWND  CMEdUiMenuBar::hWnd()
{
	return m_wndMenuBar.GetSafeHwnd();
}

const wchar_t*  CMEdUiMenuBar::title()
{
	return _TRANSLATE(L"Menubar");
}
const wchar_t*  CMEdUiMenuBar::name()
{
	return _TRANSLATE(L"Menubar" );
}