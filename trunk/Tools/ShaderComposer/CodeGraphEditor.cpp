
// ChildFrm.cpp : implementation of the CCodeGraphEditor class
//

#include "stdafx.h"
#include "ShaderComposer.h"

#include "CodeGraphEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCodeGraphEditor

IMPLEMENT_DYNCREATE(CCodeGraphEditor, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CCodeGraphEditor, CMDIChildWndEx)
END_MESSAGE_MAP()

// CCodeGraphEditor construction/destruction

CCodeGraphEditor::CCodeGraphEditor()
{
	// TODO: add member initialization code here
}

CCodeGraphEditor::~CCodeGraphEditor()
{
}


BOOL CCodeGraphEditor::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CCodeGraphEditor diagnostics

#ifdef _DEBUG
void CCodeGraphEditor::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CCodeGraphEditor::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CCodeGraphEditor message handlers
