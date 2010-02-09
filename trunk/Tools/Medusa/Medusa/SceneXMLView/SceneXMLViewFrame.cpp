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

// EditorViewFrame.cpp : implementation of the CMEdSceneXMLViewFrame class
//

#include "stdafx.h"
#include "SceneXMLViewFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMEdSceneXMLViewFrame

IMPLEMENT_DYNCREATE(CMEdSceneXMLViewFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CMEdSceneXMLViewFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_PRINT, &CMEdSceneXMLViewFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CMEdSceneXMLViewFrame::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMEdSceneXMLViewFrame::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CMEdSceneXMLViewFrame::OnUpdateFilePrintPreview)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CMEdSceneXMLViewFrame construction/destruction

CMEdSceneXMLViewFrame::CMEdSceneXMLViewFrame()
{
	// TODO: add member initialization code here
}

CMEdSceneXMLViewFrame::~CMEdSceneXMLViewFrame()
{
}


BOOL CMEdSceneXMLViewFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CMEdSceneXMLViewFrame diagnostics

#ifdef _DEBUG
void CMEdSceneXMLViewFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CMEdSceneXMLViewFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CMEdSceneXMLViewFrame message handlers

void CMEdSceneXMLViewFrame::OnFilePrint()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
	}
}

void CMEdSceneXMLViewFrame::OnFilePrintPreview()
{
	if (m_dockManager.IsPrintPreviewValid())
	{
		PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // force Print Preview mode closed
	}
}

void CMEdSceneXMLViewFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}

BOOL CMEdSceneXMLViewFrame::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CMDIChildWndEx::DestroyWindow();
}

void CMEdSceneXMLViewFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	//ShowWindow(SW_HIDE);
	//CMDIChildWndEx::OnClose();
}
