
#pragma once

#include "xMedusaEditor.h"
using namespace nsMedusaEditor;
USING_NS_XEVOL3D;
class CShaderLibViewToolBar : public CMFCToolBar 
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CShaderLibView  : public CWnd ,  public nsMedusaEditor::IMEdDockPane
{
// Construction
public:
	CShaderLibView();

	void AdjustLayout();
	void OnChangeVisualStyle();
public:
    IMPL_REFCOUNT_OBJECT_INTERFACE(CPropertiesPane);
// Attributes
protected:

	CTreeCtrl             m_wndShaderLibView;
	CImageList            m_ShaderLibViewImages;
	CShaderLibViewToolBar m_wndToolBar;

protected:
	void FillShaderLibView();

// Implementation
public:
	virtual ~CShaderLibView();
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
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()

protected:
	UINT          m_CtrlID;
};

