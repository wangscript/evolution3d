// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// EditorViewFrame.cpp : implementation of the CEditorViewFrame class
//

#include "stdafx.h"
#include "Medusa.h"

#include "EditorViewFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEditorViewFrame

IMPLEMENT_DYNCREATE(CEditorViewFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CEditorViewFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_PRINT, &CEditorViewFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CEditorViewFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEditorViewFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CEditorViewFrame::OnUpdateFilePrintPreview)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CEditorViewFrame construction/destruction

CEditorViewFrame::CEditorViewFrame()
{
	// TODO: add member initialization code here
}

CEditorViewFrame::~CEditorViewFrame()
{
}


BOOL CEditorViewFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CEditorViewFrame diagnostics

#ifdef _DEBUG
void CEditorViewFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CEditorViewFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CEditorViewFrame message handlers

void CEditorViewFrame::OnFilePrint()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CEditorViewFrame::OnFilePrintPreview()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // force Print Preview mode closed
	}
}

void CEditorViewFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}

BOOL CEditorViewFrame::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CMDIChildWndEx::DestroyWindow();
}

void CEditorViewFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	//ShowWindow(SW_HIDE);
	//CMDIChildWndEx::OnClose();
}
