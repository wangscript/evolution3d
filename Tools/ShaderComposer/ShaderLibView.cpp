
#include "stdafx.h"
#include "mainfrm.h"
#include "ShaderLibView.h"
#include "Resource.h"
#include "ShaderComposer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CShaderLibView

CShaderLibView::CShaderLibView()
{
}

CShaderLibView::~CShaderLibView()
{
}

BEGIN_MESSAGE_MAP(CShaderLibView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CShaderLibView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndShaderLibView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_ShaderLibViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndShaderLibView.SetImageList(&m_ShaderLibViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillShaderLibView();
	AdjustLayout();

	return 0;
}

void CShaderLibView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CShaderLibView::FillShaderLibView()
{
	HTREEITEM hRoot = m_wndShaderLibView.InsertItem(_T("FakeApp files"), 0, 0);
	m_wndShaderLibView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hSrc = m_wndShaderLibView.InsertItem(_T("FakeApp Source Files"), 0, 0, hRoot);

	m_wndShaderLibView.InsertItem(_T("FakeApp.cpp"), 1, 1, hSrc);
	m_wndShaderLibView.InsertItem(_T("FakeApp.rc"), 1, 1, hSrc);
	m_wndShaderLibView.InsertItem(_T("FakeAppDoc.cpp"), 1, 1, hSrc);
	m_wndShaderLibView.InsertItem(_T("FakeAppView.cpp"), 1, 1, hSrc);
	m_wndShaderLibView.InsertItem(_T("MainFrm.cpp"), 1, 1, hSrc);
	m_wndShaderLibView.InsertItem(_T("StdAfx.cpp"), 1, 1, hSrc);

	HTREEITEM hInc = m_wndShaderLibView.InsertItem(_T("FakeApp Header Files"), 0, 0, hRoot);

	m_wndShaderLibView.InsertItem(_T("FakeApp.h"), 2, 2, hInc);
	m_wndShaderLibView.InsertItem(_T("FakeAppDoc.h"), 2, 2, hInc);
	m_wndShaderLibView.InsertItem(_T("FakeAppView.h"), 2, 2, hInc);
	m_wndShaderLibView.InsertItem(_T("Resource.h"), 2, 2, hInc);
	m_wndShaderLibView.InsertItem(_T("MainFrm.h"), 2, 2, hInc);
	m_wndShaderLibView.InsertItem(_T("StdAfx.h"), 2, 2, hInc);

	HTREEITEM hRes = m_wndShaderLibView.InsertItem(_T("FakeApp Resource Files"), 0, 0, hRoot);

	m_wndShaderLibView.InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
	m_wndShaderLibView.InsertItem(_T("FakeApp.rc2"), 2, 2, hRes);
	m_wndShaderLibView.InsertItem(_T("FakeAppDoc.ico"), 2, 2, hRes);
	m_wndShaderLibView.InsertItem(_T("FakeToolbar.bmp"), 2, 2, hRes);

	m_wndShaderLibView.Expand(hRoot, TVE_EXPAND);
	m_wndShaderLibView.Expand(hSrc, TVE_EXPAND);
	m_wndShaderLibView.Expand(hInc, TVE_EXPAND);
}

void CShaderLibView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndShaderLibView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CShaderLibView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndShaderLibView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CShaderLibView::OnProperties()
{
	AfxMessageBox(_T("Properties...."));

}

void CShaderLibView::OnFileOpen()
{
	// TODO: Add your command handler code here
}

void CShaderLibView::OnFileOpenWith()
{
	// TODO: Add your command handler code here
}

void CShaderLibView::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CShaderLibView::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CShaderLibView::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CShaderLibView::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CShaderLibView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndShaderLibView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CShaderLibView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndShaderLibView.SetFocus();
}

void CShaderLibView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);

	m_ShaderLibViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ShaderLibViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ShaderLibViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndShaderLibView.SetImageList(&m_ShaderLibViewImages, TVSIL_NORMAL);
}


