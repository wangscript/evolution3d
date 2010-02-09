
// ShaderComposerView.cpp : implementation of the CShaderComposerView class
//

#include "stdafx.h"
#include "ShaderComposer.h"

#include "ShaderComposerDoc.h"
#include "ShaderComposerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CShaderComposerView

IMPLEMENT_DYNCREATE(CShaderComposerView, CView)

BEGIN_MESSAGE_MAP(CShaderComposerView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CShaderComposerView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CShaderComposerView construction/destruction

CShaderComposerView::CShaderComposerView()
{
	// TODO: add construction code here

}

CShaderComposerView::~CShaderComposerView()
{
}

BOOL CShaderComposerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CShaderComposerView drawing

void CShaderComposerView::OnDraw(CDC* /*pDC*/)
{
	CShaderComposerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CShaderComposerView printing


void CShaderComposerView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CShaderComposerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CShaderComposerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CShaderComposerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CShaderComposerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CShaderComposerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CShaderComposerView diagnostics

#ifdef _DEBUG
void CShaderComposerView::AssertValid() const
{
	CView::AssertValid();
}

void CShaderComposerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CShaderComposerDoc* CShaderComposerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CShaderComposerDoc)));
	return (CShaderComposerDoc*)m_pDocument;
}
#endif //_DEBUG


// CShaderComposerView message handlers
