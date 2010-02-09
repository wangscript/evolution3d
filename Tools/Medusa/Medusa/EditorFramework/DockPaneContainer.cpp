
#include "stdafx.h"
#include "DockPaneContainer.h"
#include "..\Resource.h"
#include "MainFrm.h"
#include "Medusa.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CDockPaneContainer::CDockPaneContainer()
{
	m_ChildWindow = NULL;
	m_pMEdDockPane = NULL;
	m_pViewBarButton = NULL;
}

void CDockPaneContainer::SetMEdDockPane(nsMedusaEditor::IMEdDockPane* pPane)
{
	HWND hWnd = pPane->hWnd();
	m_ChildWindow = hWnd;
	m_pMEdDockPane = pPane;
	m_pViewBarButton = NULL;
}

HICON CDockPaneContainer::hIcon()
{
	return m_pMEdDockPane->hIcon();
}

bool CDockPaneContainer::onDetach()
{
return m_pMEdDockPane->onDetach();
}

bool CDockPaneContainer::onAttach()
{
return m_pMEdDockPane->onAttach();
}

const wchar_t* CDockPaneContainer::title()
{
	return m_pMEdDockPane->title();
}

void CDockPaneContainer::setID(UINT id)
{
	return m_pMEdDockPane->setID(id);
}

UINT CDockPaneContainer::getID()
{
	return m_pMEdDockPane->getID();
}

HWND CDockPaneContainer::hWnd()
{
	return m_pMEdDockPane->hWnd();
}

CDockPaneContainer::~CDockPaneContainer()
{

}

BEGIN_MESSAGE_MAP(CDockPaneContainer, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CDockPaneContainer::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	::MoveWindow(m_ChildWindow ,  rectClient.left, rectClient.top , rectClient.Width(), rectClient.Height() , TRUE);
	//::SetWindowPos(m_pWnd->GetSafeHwnd() ,  NULL, rectClient.left, rectClient.top , rectClient.Width(), rectClient.Height()  , SWP_NOACTIVATE | SWP_NOZORDER);
}

int CDockPaneContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	::SetParent(m_ChildWindow , this->GetSafeHwnd() );
	::ShowWindow(m_ChildWindow , SW_SHOW);
	::UpdateWindow(m_ChildWindow);
	AdjustLayout();
	return 0;
}

void CDockPaneContainer::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}



void CDockPaneContainer::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	::SetFocus(m_ChildWindow);
}


BOOL CDockPaneContainer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDockablePane::PreTranslateMessage(pMsg);
}
