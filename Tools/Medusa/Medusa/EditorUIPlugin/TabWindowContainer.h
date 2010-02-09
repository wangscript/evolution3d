
#pragma once
#include "../Editor/xMedusaEditorImpl.h"
class CTabWindowContainer : public CWnd
{
	// Construction
public:
	CTabWindowContainer();
	virtual ~CTabWindowContainer();
	void     SetMEdDockPane(nsMedusaEditor::IMEdUIElement* pPane);
	BOOL     Create( CWnd* pWnd , int w );
protected:
	void AdjustLayout();
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
protected:
	HWND                          m_hChildWindow;
	// Implementation
};

