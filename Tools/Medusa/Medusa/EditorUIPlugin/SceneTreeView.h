
#pragma once

#include "../xMedusaEditor.h"
#include "../Editor/EditorEnv.h"
using namespace nsMedusaEditor;
USING_NS_XEVOL3D;

class CSceneTreeViewItem;
typedef std::vector<CSceneTreeViewItem> vModelResViewTreeItems;

enum eModelResViewTreeItem
{
	eMRVTI_Scene,
	eMRVTI_Node,
	eMRVTI_Dummy,
};

class CSceneTreeViewItem
{
public:
	void DeleteChildren(CTreeCtrl& treeCtrl)
	{
		for(size_t i = 0 ; i < m_TreeItems.size() ; i ++)
		{
			m_TreeItems[i].DeleteChildren(treeCtrl);
			
		}
		treeCtrl.DeleteItem(m_hTreeItem);
		m_hTreeItem = NULL;
		m_TreeItems.clear();
	}

	void DeleteChildren(ISceneNode* pNode , CTreeCtrl& treeCtrl)
	{
		vModelResViewTreeItems::iterator it = m_TreeItems.begin();
		for(; it != m_TreeItems.end() ; it ++)
		{
			CSceneTreeViewItem& item = *it;
			if(item.m_pSceneNode == pNode)
			{
				GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_SceneSelChanged , (int)pNode->sceneGraph() , (int)pNode->getParent() );
				treeCtrl.DeleteItem(item.m_hTreeItem);
				m_TreeItems.erase(it);
				return ;
			}
		}

		it = m_TreeItems.begin();
		for(; it != m_TreeItems.end() ; it ++)
		{
			CSceneTreeViewItem& item = *it;
			item.DeleteChildren(pNode , treeCtrl);
		}
	}

    CSceneTreeViewItem* AddChild(CSceneTreeViewItem& hTreeItem)
	{
        m_TreeItems.push_back(hTreeItem);
		return &m_TreeItems[m_TreeItems.size() - 1];
	}
	CSceneTreeViewItem* FindTreeItem(HTREEITEM hTreeItem)
	{
		if(hTreeItem == m_hTreeItem)
			return this;
		for(size_t i = 0 ; i < m_TreeItems.size() ; i ++)
		{
			CSceneTreeViewItem* pItem = m_TreeItems[i].FindTreeItem(hTreeItem);
			if(pItem)
				return pItem;
		}
		return NULL;
	}

	CSceneTreeViewItem* FindTreeItem(ISceneNode* pSceneNode)
	{
		if(pSceneNode == m_pSceneNode)
			return this;
		for(size_t i = 0 ; i < m_TreeItems.size() ; i ++)
		{
			CSceneTreeViewItem* pItem = m_TreeItems[i].FindTreeItem(pSceneNode);
			if(pItem)
				return pItem;
		}
		return NULL;
	}
	CSceneTreeViewItem()
	{
		m_ItemType = eMRVTI_Dummy;
	}
public:
	vModelResViewTreeItems m_TreeItems;
	HTREEITEM              m_hTreeItem;
	eModelResViewTreeItem  m_ItemType;
	ISceneNode*            m_pSceneNode;

};

class CModelResLibViewToolBar : public CMFCToolBar 
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};
class CSceneTreeView;
class CSceneTreeCtrl  : public CTreeCtrl
{
public:	
	CSceneTreeCtrl(CSceneTreeView* pView);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	CSceneTreeView* m_pView;
};
class CSceneTreeView  : public CWnd ,  public nsMedusaEditor::IMEdDockPane
{
// Construction
public:
	CSceneTreeView();

	void AdjustLayout();
	void OnChangeVisualStyle();
public:
    IMPL_REFCOUNT_OBJECT_INTERFACE(CPropertiesPane);
// Attributes

protected:
	void FillSceneTreeView(ISceneGraph* pScene);
	void FillSceneTreeView(ISceneNode*  pRootNode , CSceneTreeViewItem* pRootItem);

// Implementation
public:
	virtual ~CSceneTreeView();
	HICON            hIcon();
	const wchar_t*   title();
	const wchar_t*   name();
	void             setID(UINT id);
	UINT             getID();
	HWND             hWnd();
	bool             onDetach();
	bool             onAttach();
	bool             onMEdUIEvent(eMEUIEvent _event , int param, int param2);
	bool             createMEdUI(HWND hMainWnd) ;
	bool             destroyMEdUI() ; 	
	bool             HideMEdUI();
	bool             ShowMEdUI();
	const wchar_t*   typeName();
    bool             IsVisible() { return IsWindowVisible() ; } 

public:
	bool  OnTreeSelChange();
	void                removeItem(ISceneNode* pNode);
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnInsertNode();
	afx_msg void OnDeleteNode();
	afx_msg void OnCompile();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()

protected:

	CSceneTreeCtrl          m_ctrlTree;
	CImageList              m_ShaderLibViewImages;
	CModelResLibViewToolBar m_wndToolBar;
	UINT                    m_CtrlID;
	CSceneTreeViewItem   m_RootItem;
};

