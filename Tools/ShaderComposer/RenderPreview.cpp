
// ChildFrm.cpp : implementation of the CCodeGraphEditor class
//

#include "stdafx.h"
#include "ShaderComposer.h"

#include "RenderPreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCodeGraphEditor


BEGIN_MESSAGE_MAP(CPreviewWindow, CDockablePane)
END_MESSAGE_MAP()

// CCodeGraphEditor construction/destruction

CPreviewWindow::CPreviewWindow()
{
	// TODO: add member initialization code here
}

CPreviewWindow::~CPreviewWindow()
{
}

void CPreviewWindow::OnDraw(CDC* pDC)
{

}

BOOL CPreviewWindow::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::DestroyWindow();
}
#define LOSHORT(l)           ((short)(((DWORD_PTR)(l)) & 0xffff))
#define HISHORT(l)           ((short)((((DWORD_PTR)(l)) >> 16) & 0xffff))
BOOL CPreviewWindow::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
    GetShaderComposerUIImpl()->fireRenderPreviewEvent(GetSafeHwnd() , message , lParam , wParam);
	if(message == WM_MOUSEMOVE)
	{
		int x, y;
		y=HISHORT(lParam);
		x=LOSHORT(lParam);
		if(x >  0 && y > 0)
			return TRUE;
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}
