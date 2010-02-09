
#include "stdafx.h"
#include "OutlookBar.h"
#include "..\Resource.h"
#include <BaseLib/xStringTable.h>
USING_NS_XEVOL3D;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define _MEdUI_NAME L"NavigatorBar";
#define _MEdUI_DESC L"Navigator Bar";
IMPL_OBJECT_FACTORY_NO_ARG(CNavigatorBar , IMEdUIElement, CNavigatorBarFactory, IMEdUIElementFactory , CMEdUIElFactoryMgr , _MEdUI_NAME , _MEdUI_DESC);
/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar
const wchar_t* CNavigatorBar::typeName()
{
	return _MEdUI_NAME;
}
CNavigatorBar::CNavigatorBar() 
{
	m_InitWidth = 250;
	m_wndNavigationBar.m_strPaneName = _TRANSLATE(L"Outlook Bar");
}

CNavigatorBar::~CNavigatorBar()
{

}
HWND CNavigatorBar::GetParentWnd()
{
	return m_wndNavigationBar.GetSafeHwnd();
}


bool  CNavigatorBar::onDetach()
{
	return true;
}

bool  CNavigatorBar::onAttach()
{
	return true ;
}

bool  CNavigatorBar::AttachUIElement(nsMedusaEditor::IMEdUIElement* pPane)
{
	if(pPane == NULL)
		return false;

	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;
	CMEdUIElConatiner::AttachUIElement(pPane);
	HWND hWnd = pPane->hWnd();
	int imagIdx = -1;
    IMEdDockPane* pDockPane = dynamic_cast<IMEdDockPane*> (pPane);
	if(pDockPane && pDockPane->hIcon() )
	{
		CImageList* pImageList = (CImageList*)m_pOutlookBar->GetImageList();
		int nImg = pImageList->GetImageCount();
		imagIdx = nImg;
		pImageList->Add(pDockPane->hIcon());
	}


	CTabWindowContainer* pCt = new CTabWindowContainer;
	pCt->Create(m_pOutlookBar , m_InitWidth);
	
	m_ctToElements.insert( vCtToElements::value_type(pPane , pCt));
	pCt->SetMEdDockPane(pPane);
	pCt->ShowWindow(SW_SHOW);
	pCt->UpdateWindow();
	

	//CWnd* pWnd = CWnd::FromHandle(hWnd);
	m_pOutlookBar->AddControl(pCt, pPane->title() , imagIdx , TRUE, dwStyle);
	m_pOutlookBar->ShowWindow(SW_SHOW);
	m_pOutlookBar->UpdateWindow();
	return true;
}

bool  CNavigatorBar::DetachUIElement(nsMedusaEditor::IMEdUIElement* pPane)
{
	if(pPane == NULL)
		return false;

	vCtToElements::iterator pos = m_ctToElements.find(pPane);
	CTabWindowContainer* pCt = pos->second;
	if(pCt)
	{
		pCt->DestroyWindow();
		delete pCt;
	}
	m_ctToElements.erase(pos);

	CMEdUIElConatiner::DetachUIElement(pPane);
	HWND hWnd = pPane->hWnd();
	CWnd* pWnd = CWnd::FromHandle(hWnd);
	ASSERT(0);
	return true;
}

bool  CNavigatorBar::onMEdUIEvent(eMEUIEvent _event , int param, int param2)
{
	return CMEdUIElConatiner::fireMEdUIEvent(_event , param, param2);
}

bool  CNavigatorBar::createMEdUI(HWND hMainWnd)
{
	m_CtrlID = GetMedusaEditor()->GetUI()->AllocUIID(1);
	CreateOutlookBar(hMainWnd , m_CtrlID , m_InitWidth);
	return false;
}

bool  CNavigatorBar::destroyMEdUI()
{
	m_wndNavigationBar.DestroyWindow();
	return true;
}

bool CNavigatorBar::HideMEdUI()
{
	m_wndNavigationBar.ShowWindow(SW_HIDE);
	return true;
}

bool CNavigatorBar::ShowMEdUI()
{
	m_wndNavigationBar.ShowWindow(SW_SHOW);
	m_wndNavigationBar.UpdateWindow();
	return true;
}

//
HICON CNavigatorBar::hIcon()
{
	return NULL;
}

const wchar_t* CNavigatorBar::title() 
{
	return L"Outlook bar";
}
const wchar_t* CNavigatorBar::name()
{
	return L"Outlookbar";
}
void CNavigatorBar::setID(UINT id)
{
	m_PaneID = id;
}

UINT CNavigatorBar::getID()
{
	return m_PaneID;
}

HWND CNavigatorBar::hWnd()
{
	return m_wndNavigationBar.GetSafeHwnd();
}


BOOL CNavigatorBar::CreateOutlookBar(HWND hParent , UINT uiID, int nInitialWidth)
{
	CWindowDC dc(NULL);

	CWnd* pWnd = CWnd::FromHandle(hParent);
	m_wndNavigationBar.SetMode2003();

	if (!m_wndNavigationBar.Create(_TRANSLATE(L"Shortcuts"), pWnd, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		return FALSE; // fail to create
	}

	m_pOutlookBar = (CMFCOutlookBarTabCtrl*)m_wndNavigationBar.GetUnderlyingWindow();

	if (m_pOutlookBar == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_pOutlookBar->EnableInPlaceEdit(TRUE);

	static UINT uiPageID = 1;

	DWORD dwPaneStyle = AFX_DEFAULT_TOOLBAR_STYLE | CBRS_FLOAT_MULTI;

	m_wndNavigationBar.SetPaneStyle(m_wndNavigationBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	m_pOutlookBar->SetImageList(TRUE ? IDB_PAGES_HC : IDB_PAGES, 24);
	m_pOutlookBar->SetToolbarImageList(TRUE ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	m_pOutlookBar->RecalcLayout();


	CMFCOutlookBarTabCtrl::EnableAnimation(TRUE);

	m_wndNavigationBar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}
