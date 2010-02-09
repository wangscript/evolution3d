
#include "stdafx.h"
#include "SceneTreeView.h"
#include "../Resource.h"
#include <BaseLib/xStringTable.h>
#include "../xMedusaEditor.h"
#include "../xEvolEnviroment.h"
#include "../Editor/EditorEnv.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CSceneTreeView
#define _MEdUI_NAME L"SceneTreeView";
#define _MEdUI_DESC L"SceneTreeView";
IMPL_OBJECT_FACTORY_NO_ARG(CSceneTreeView , IMEdUIElement, CSceneTreeViewFactory, IMEdUIElementFactory , CMEdUIElFactoryMgr , _MEdUI_NAME , _MEdUI_DESC);
// CResourceViewBar
const wchar_t* CSceneTreeView::typeName()
{
	return _MEdUI_NAME;
}
CSceneTreeView::CSceneTreeView()
:m_ctrlTree(this)
{
	m_RootItem.m_hTreeItem = NULL;
}

CSceneTreeView::~CSceneTreeView()
{
}

BEGIN_MESSAGE_MAP(CSceneTreeView, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_STV_INSERTNODE , OnInsertNode)
	ON_COMMAND(ID_STV_DELETE_NODE, OnDeleteNode)
	ON_COMMAND(ID_STV_COMPILE    , OnCompile)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

bool  CSceneTreeView::onDetach()
{
	return true;
}

bool  CSceneTreeView::onAttach()
{
	return true ;
}

HICON  CSceneTreeView::hIcon()
{
	BOOL bHiColorIcons = TRUE;
	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE( IDI_SCENE_TREEVIEW ), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	return hPropertiesBarIcon;
}

const wchar_t*  CSceneTreeView::title()
{
	return NS_XEVOL3D::_TRANSLATE(L"Scene Tree View");
}

const wchar_t*  CSceneTreeView::name()
{
	return NS_XEVOL3D::_TRANSLATE(L"SceneTreeView");
}

void  CSceneTreeView::setID(UINT id)
{
	m_CtrlID = id;
}

UINT  CSceneTreeView::getID()
{
	return m_CtrlID;
}

HWND  CSceneTreeView::hWnd()
{
	return GetSafeHwnd();
}

bool  CSceneTreeView::onMEdUIEvent(eMEUIEvent _event , int param , int param2)
{
	if(_event == eMEUI_SceneChanged)
	{
		ISceneGraph* pScene = GetEditorEnv()->evol3DEnv()->scene();
		FillSceneTreeView(pScene);
		m_ctrlTree.Expand(m_RootItem.m_hTreeItem , TVE_EXPAND);
		return true;
	}
	if(_event == eMEUI_SceneInsertNode)
	{
		ISceneNode* pChlidNode = (ISceneNode*)param2;
		if(pChlidNode == NULL)
			return false;
		ISceneNode* pNode = pChlidNode->getParent();

        CSceneTreeViewItem* pRootItem = m_RootItem.FindTreeItem(pNode);
		if(pRootItem == NULL)
			return false;

		CSceneTreeViewItem _item;
		_item.m_ItemType = eMRVTI_Scene;
		_item.m_pSceneNode = pChlidNode;
		_item.m_hTreeItem = m_ctrlTree.InsertItem( pChlidNode->name(), 0, 0, pRootItem->m_hTreeItem);
		m_ctrlTree.SetItemState(_item.m_hTreeItem, TVIS_BOLD, TVIS_BOLD);
		FillSceneTreeView(pChlidNode , &_item);
		pRootItem->AddChild(_item);		
		
		return true;
	}
	if(_event == eMEUI_SceneRemoveNode)
	{
		ISceneNode* pSceneNode = (ISceneNode*)(param2);
        removeItem(pSceneNode);
		return true;
	}
	return false;
}

bool  CSceneTreeView::createMEdUI(HWND hMainWnd)
{
	CWnd* pWnd = CWnd::FromHandle(hMainWnd);
	LPCTSTR clsName = AfxRegisterWndClass(CS_DBLCLKS);
	if(FALSE !=  CWnd::Create(clsName , L"Hello" , WS_CHILD, CRect(0,0,200,200) , pWnd ,0x00f123, NULL ) )
	{
		return true;
	}
	return false;
}

bool  CSceneTreeView::destroyMEdUI()
{
	this->DestroyWindow();
	return true;
}

bool CSceneTreeView::HideMEdUI()
{
	GetParent()->ShowWindow(SW_HIDE);
	return true;
}

bool CSceneTreeView::ShowMEdUI()
{
	GetParent()->ShowWindow(SW_SHOW);
	GetParent()->UpdateWindow();
	return true;
}


int CSceneTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;

	if (!m_ctrlTree.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create file view\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_ShaderLibViewImages.Create(IDB_SCENE_TREEVIEW, 16, 0, RGB(255, 0, 255));
	m_ctrlTree.SetImageList(&m_ShaderLibViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SCENETREEVIEW);
	m_wndToolBar.LoadToolBar(IDR_SCENETREEVIEW /* Is locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
	m_wndToolBar.ShowWindow(SW_SHOW);

	( (CWinAppEx*)AfxGetApp())->GetContextMenuManager()->AddMenu(TEXT("ModelResView"),IDR_POPUP_MODELRESVIEW);
	// Fill in some static tree view data (dummy code, nothing magic here)
	AdjustLayout();
	return 0;
}

void CSceneTreeView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	AdjustLayout();
}

void  CSceneTreeView::removeItem(ISceneNode* pNode)
{
    m_RootItem.DeleteChildren(pNode,m_ctrlTree);
	if(GetSceneSelection()->GetCurSelNode() == pNode)
	{
		GetSceneSelection()->setCurSelNode(NULL);
	}
	return ;
}

void CSceneTreeView::FillSceneTreeView(ISceneGraph* pScene)
{
	m_RootItem.DeleteChildren(m_ctrlTree);
	m_RootItem.m_ItemType = eMRVTI_Scene;
	m_RootItem.m_pSceneNode = pScene;
	m_RootItem.m_hTreeItem = m_ctrlTree.InsertItem( _TRANSLATE(_T("Scene Root")), 0, 0);
	m_ctrlTree.SetItemState(m_RootItem.m_hTreeItem, TVIS_BOLD, TVIS_BOLD);
	FillSceneTreeView(pScene , &m_RootItem);

}
void CSceneTreeView::FillSceneTreeView(ISceneNode*  pRootNode , CSceneTreeViewItem* pRootItem)
{
	 int iChild = pRootNode->nChildren();
	 pRootNode->beginEnumChildren();
	 ISceneNode* pChlidNode = NULL;
	 while(pChlidNode = pRootNode->nextChild())
	 {
		 if( pChlidNode->bInScene() == false)
		 {
				 continue;
		 }
		 CSceneTreeViewItem _item;
		 _item.m_ItemType = eMRVTI_Scene;
		 _item.m_pSceneNode = pChlidNode;
		 _item.m_hTreeItem = m_ctrlTree.InsertItem( pChlidNode->name(), 0, 0, pRootItem->m_hTreeItem);
		 m_ctrlTree.SetItemState(_item.m_hTreeItem, TVIS_BOLD, TVIS_BOLD);
		 FillSceneTreeView(pChlidNode , &_item);
		 pRootItem->AddChild(_item);		 
         
	 }
	 pRootNode->endEnumChildren();
}

void CSceneTreeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_ctrlTree;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CWnd::OnContextMenu(pWnd, point);
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
	( (CWinAppEx*)AfxGetApp())->GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_MODELRESVIEW, point.x, point.y, this, TRUE);
}

void CSceneTreeView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_ctrlTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CSceneTreeView::OnInsertNode()
{
	AfxMessageBox(_T("OnInsertNode...."));
}

void CSceneTreeView::OnDeleteNode()
{
	AfxMessageBox(_T("OnDeleteNode...."));
}

void CSceneTreeView::OnCompile()
{
	AfxMessageBox(_T("OnCompile...."));
}

void CSceneTreeView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_ctrlTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CSceneTreeView::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	m_ctrlTree.SetFocus();
}

void CSceneTreeView::OnChangeVisualStyle()
{
}

bool CSceneTreeView::OnTreeSelChange()
{
	HTREEITEM hTreeItem = m_ctrlTree.GetSelectedItem();
	CSceneTreeViewItem* pItem = m_RootItem.FindTreeItem(hTreeItem);
	if(pItem == NULL)
		return false;

	ISceneNode* pCurSelNode = pItem->m_pSceneNode;
	if(pCurSelNode == NULL)
		return false;
	if(pCurSelNode == GetEvol3DEnv()->scene())
		return false;
	ISceneNode* pGizmo = GetSceneSelection()->GetGizmo();
    GetSceneSelection()->setCurSelNode(pCurSelNode);


	//ÖØÐÂÉèÖÃGizmo
	if(pCurSelNode->placement() != NULL)
		return false;

	if( pGizmo == NULL  )
	{
		GetSceneSelection()->setMoveGizmo();
	}
	else
	{
		GetSceneSelection()->setGizmoNode(pGizmo);
	}

	return true;
}
CSceneTreeCtrl::CSceneTreeCtrl(CSceneTreeView* pView)
{
	m_pView = pView;
}

BEGIN_MESSAGE_MAP(CSceneTreeCtrl, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CSceneTreeCtrl::OnTvnSelchanged)
END_MESSAGE_MAP()

void CSceneTreeCtrl::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_pView->OnTreeSelChange();
	*pResult = 0;
}
