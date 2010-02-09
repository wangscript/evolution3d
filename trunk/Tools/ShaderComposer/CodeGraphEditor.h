
// ChildFrm.h : interface of the CCodeGraphEditor class
//

#pragma once

class CCodeGraphEditor : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CCodeGraphEditor)
public:
	CCodeGraphEditor();

// Attributes
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CCodeGraphEditor();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
