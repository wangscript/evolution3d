
// ChildFrm.h : interface of the CCodeGraphEditor class
//

#pragma once

class CPreviewWindow : public CDockablePane
{
public:
	CPreviewWindow();
public:

// Implementation
public:
	virtual ~CPreviewWindow();
	void OnDraw(CDC* pDC);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
