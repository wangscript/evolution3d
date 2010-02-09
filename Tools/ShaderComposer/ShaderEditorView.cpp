
#include "stdafx.h"
#include "MainFrm.h"
#include "ShaderEditorView.h"
#include "Resource.h"
#include "ShaderComposer.h"

class CShaderEditorViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CShaderEditorView;

	DECLARE_SERIAL(CShaderEditorViewMenuButton)

public:
	CShaderEditorViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CShaderEditorViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShaderEditorView::CShaderEditorView()
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CShaderEditorView::~CShaderEditorView()
{
}

BEGIN_MESSAGE_MAP(CShaderEditorView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShaderEditorView message handlers

int CShaderEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create views:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndShaderEditorView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Failed to create Class View\n");
		return -1;      // fail to create
	}

	// Load images:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CShaderEditorViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CShaderEditorViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CShaderEditorViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// Fill in some static tree view data (dummy code, nothing magic here)
	FillShaderEditorView();

	return 0;
}

void CShaderEditorView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CShaderEditorView::FillShaderEditorView()
{
	HTREEITEM hRoot = m_wndShaderEditorView.InsertItem(_T("FakeApp classes"), 0, 0);
	m_wndShaderEditorView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = m_wndShaderEditorView.InsertItem(_T("CFakeAboutDlg"), 1, 1, hRoot);
	m_wndShaderEditorView.InsertItem(_T("CFakeAboutDlg()"), 3, 3, hClass);

	m_wndShaderEditorView.Expand(hRoot, TVE_EXPAND);

	hClass = m_wndShaderEditorView.InsertItem(_T("CFakeApp"), 1, 1, hRoot);
	m_wndShaderEditorView.InsertItem(_T("CFakeApp()"), 3, 3, hClass);
	m_wndShaderEditorView.InsertItem(_T("InitInstance()"), 3, 3, hClass);
	m_wndShaderEditorView.InsertItem(_T("OnAppAbout()"), 3, 3, hClass);

	hClass = m_wndShaderEditorView.InsertItem(_T("CFakeAppDoc"), 1, 1, hRoot);
	m_wndShaderEditorView.InsertItem(_T("CFakeAppDoc()"), 4, 4, hClass);
	m_wndShaderEditorView.InsertItem(_T("~CFakeAppDoc()"), 3, 3, hClass);
	m_wndShaderEditorView.InsertItem(_T("OnNewDocument()"), 3, 3, hClass);

	hClass = m_wndShaderEditorView.InsertItem(_T("CFakeAppView"), 1, 1, hRoot);
	m_wndShaderEditorView.InsertItem(_T("CFakeAppView()"), 4, 4, hClass);
	m_wndShaderEditorView.InsertItem(_T("~CFakeAppView()"), 3, 3, hClass);
	m_wndShaderEditorView.InsertItem(_T("GetDocument()"), 3, 3, hClass);
	m_wndShaderEditorView.Expand(hClass, TVE_EXPAND);

	hClass = m_wndShaderEditorView.InsertItem(_T("CFakeAppFrame"), 1, 1, hRoot);
	m_wndShaderEditorView.InsertItem(_T("CFakeAppFrame()"), 3, 3, hClass);
	m_wndShaderEditorView.InsertItem(_T("~CFakeAppFrame()"), 3, 3, hClass);
	m_wndShaderEditorView.InsertItem(_T("m_wndMenuBar"), 6, 6, hClass);
	m_wndShaderEditorView.InsertItem(_T("m_wndToolBar"), 6, 6, hClass);
	m_wndShaderEditorView.InsertItem(_T("m_wndStatusBar"), 6, 6, hClass);

	hClass = m_wndShaderEditorView.InsertItem(_T("Globals"), 2, 2, hRoot);
	m_wndShaderEditorView.InsertItem(_T("theFakeApp"), 5, 5, hClass);
	m_wndShaderEditorView.Expand(hClass, TVE_EXPAND);
}

void CShaderEditorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndShaderEditorView;
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
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CShaderEditorView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndShaderEditorView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CShaderEditorView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CShaderEditorView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CShaderEditorViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CShaderEditorViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CShaderEditorView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CShaderEditorView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("Add member function..."));
}

void CShaderEditorView::OnClassAddMemberVariable()
{
	// TODO: Add your command handler code here
}

void CShaderEditorView::OnClassDefinition()
{
	// TODO: Add your command handler code here
}

void CShaderEditorView::OnClassProperties()
{
	// TODO: Add your command handler code here
}

void CShaderEditorView::OnNewFolder()
{
	AfxMessageBox(_T("New Folder..."));
}

void CShaderEditorView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndShaderEditorView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CShaderEditorView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndShaderEditorView.SetFocus();
}

void CShaderEditorView::OnChangeVisualStyle()
{
	m_ShaderEditorViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

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

	m_ShaderEditorViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ShaderEditorViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndShaderEditorView.SetImageList(&m_ShaderEditorViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* Locked */);
}
