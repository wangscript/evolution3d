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

// MedusaView.cpp : implementation of the CMedusaView class
//

#include "stdafx.h"
#include "Medusa.h"

#include "MedusaDoc.h"
#include "MedusaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMedusaView

IMPLEMENT_DYNCREATE(CMedusaView, CView)

BEGIN_MESSAGE_MAP(CMedusaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMedusaView::OnFilePrintPreview)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMedusaView construction/destruction

CMedusaView::CMedusaView()
{
	// TODO: add construction code here

}

CMedusaView::~CMedusaView()
{
}

BOOL CMedusaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMedusaView drawing

void CMedusaView::OnDraw(CDC* /*pDC*/)
{
	CMedusaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMedusaView printing


void CMedusaView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMedusaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMedusaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMedusaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMedusaView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMedusaView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMedusaView diagnostics

#ifdef _DEBUG
void CMedusaView::AssertValid() const
{
	CView::AssertValid();
}

void CMedusaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMedusaDoc* CMedusaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMedusaDoc)));
	return (CMedusaDoc*)m_pDocument;
}
#endif //_DEBUG


// CMedusaView message handlers

BOOL CMedusaView::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CView::DestroyWindow();
}

void CMedusaView::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CView::OnClose();
}

BEGIN_NAMESPACE_XEVOL3D
bool ConvertToXEvolMsg(xWindowMsg& WinMsg , HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
END_NAMESPACE_XEVOL3D
BOOL CMedusaView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	NS_XEVOL3D::xWindowMsg msg;
	ConvertToXEvolMsg(msg , GetSafeHwnd() , message , wParam , lParam);
	if( (msg.MsgID >= WIN_KEYBOARD_MESSAGE_BEGIN && msg.MsgID < WIN_KEYBOARD_MESSAGE_END) ||
		(msg.MsgID >= WIN_MOUSE_MESSAGE_BEGIN && msg.MsgID < WIN_MOUSE_MESSAGE_END)             )
	{
		if(GetEditorEnv()->onMessage(msg) == false )
		{
			CMedusaMainUI* pUI = dynamic_cast<CMedusaMainUI*>( GetMedusaEditor()->GetUI() );
			pUI->FireWindowMsg(msg);
		}
	}
	else 
	{
		GetEditorEnv()->onMessage(msg);		
	}
	return CView::OnWndMsg(message, wParam, lParam, pResult);
}

void CMedusaView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
   // GetEditorEnv()->OnIdle();
	CView::OnTimer(nIDEvent);
}

int CMedusaView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//SetTimer(1000,20,NULL);
	// TODO:  Add your specialized creation code here

	return 0;
}
