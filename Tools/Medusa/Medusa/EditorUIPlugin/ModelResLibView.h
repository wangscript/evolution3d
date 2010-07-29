
#pragma once

#include "../xMedusaEditor.h"
#include "../Editor/EditorEnv.h"
using namespace nsMedusaEditor;
USING_NS_XEVOL3D;

class CModelResViewTreeItem;
typedef std::vector<CModelResViewTreeItem> vModelResViewTreeItems;

enum eModelResViewTreeItem
{
	eMRVTI_Root,
	eMRVTI_Folder,
	eMRVTI_File,
};
class CModelResViewTreeItem
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

    CModelResViewTreeItem* AddChild(CModelResViewTreeItem& hTreeItem)
	{
        m_TreeItems.push_back(hTreeItem);
		return &m_TreeItems[m_TreeItems.size() - 1];
	}

	CModelResViewTreeItem* FindTreeItem(HTREEITEM hTreeItem)
	{
		if(hTreeItem == m_hTreeItem)
			return this;
		for(size_t i = 0 ; i < m_TreeItems.size() ; i ++)
		{
			CModelResViewTreeItem* pItem = m_TreeItems[i].FindTreeItem(hTreeItem);
			if(pItem)
				return pItem;
		}
		return NULL;
	}
	
	CModelResViewTreeItem()
	{
		m_ItemType = eMRVTI_Folder;
	}
public:
	vModelResViewTreeItems m_TreeItems;
	HTREEITEM              m_hTreeItem;
	eModelResViewTreeItem  m_ItemType;
	std::wstring           m_FileName;
};

class CModelResLibViewToolBar : public CMFCToolBar 
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CModelResLibView  : public CWnd ,  public nsMedusaEditor::IMEdDockPane
{
// Construction
public:
	CModelResLibView();

	void AdjustLayout();
	void OnChangeVisualStyle();
public:
    IMPL_REFCOUNT_OBJECT_INTERFACE(CPropertiesPane);
// Attributes

protected:
	void FillModelResTreeView(xResPathManager* PathManager);

// Implementation
public:
	virtual ~CModelResLibView();
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
	bool             FindModelFile(xDirEnumItem& item, CModelResViewTreeItem* pRootItem);
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnInserModel();
	afx_msg void OnRefleshLib();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()

protected:

	CTreeCtrl               m_ctrlTree;
	CImageList              m_ShaderLibViewImages;
	CModelResLibViewToolBar m_wndToolBar;
	UINT                    m_CtrlID;
	vModelResViewTreeItems  m_TreeItems;
	CModelResViewTreeItem   m_RootItem;
};

