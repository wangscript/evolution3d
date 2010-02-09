
#include "stdafx.h"
#include "TabWindowContainer.h"
#include "..\Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CTabWindowContainer::CTabWindowContainer()
{
	m_hChildWindow = NULL;
}

void CTabWindowContainer::SetMEdDockPane(nsMedusaEditor::IMEdUIElement* pPane)
{
	HWND hWnd = pPane->hWnd();
	m_hChildWindow = hWnd;
	::SetParent(m_hChildWindow , this->GetSafeHwnd() );
}


CTabWindowContainer::~CTabWindowContainer()
{

}

BEGIN_MESSAGE_MAP(CTabWindowContainer, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CTabWindowContainer::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	::MoveWindow(m_hChildWindow ,  rectClient.left, rectClient.top , rectClient.Width(), rectClient.Height() , TRUE);
	//::SetWindowPos(m_pWnd->GetSafeHwnd() ,  NULL, rectClient.left, rectClient.top , rectClient.Width(), rectClient.Height()  , SWP_NOACTIVATE | SWP_NOZORDER);
}
BOOL CTabWindowContainer::Create( CWnd* pParentWnd , int w  )
{
	CRect _clientRect;
	pParentWnd->GetClientRect(_clientRect);
	LPCTSTR clsName = AfxRegisterWndClass(CS_DBLCLKS);
	_clientRect.right = w;
	_clientRect.bottom = 1000;
	if(FALSE ==  CWnd::Create(clsName , L"Hello" , WS_CHILD, _clientRect , pParentWnd ,0x00f123, NULL ) )
	{
		return FALSE;
	}
	ShowWindow(SW_SHOW);
	UpdateWindow();
	::SetParent(m_hChildWindow , pParentWnd->GetSafeHwnd() );
	AdjustLayout();
	return TRUE;
}
int CTabWindowContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	::SetParent(m_hChildWindow , this->GetSafeHwnd() );
	::ShowWindow(m_hChildWindow , SW_SHOW);
	::UpdateWindow(m_hChildWindow);
	AdjustLayout();
	return 0;
}

void CTabWindowContainer::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	AdjustLayout();
}



void CTabWindowContainer::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	::SetFocus(m_hChildWindow);
}

