
#pragma once
#include "../Editor/xMedusaEditorImpl.h"
class CDockPaneContainer : public CDockablePane
{
	// Construction
public:
	CDockPaneContainer();
	virtual ~CDockPaneContainer();
	HICON            hIcon();
	const wchar_t*   title();
	void             setID(UINT id);
	UINT             getID();
	HWND             hWnd();
	bool             onDetach();
	bool             onAttach();
	nsMedusaEditor::IMEdDockPane* GetMEdDockPane(){return m_pMEdDockPane ; }
	void                          SetMEdDockPane(nsMedusaEditor::IMEdDockPane* pPane);
protected:
	void AdjustLayout();
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
protected:
	HWND                          m_ChildWindow;
	nsMedusaEditor::IMEdDockPane* m_pMEdDockPane;
	// Implementation
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

