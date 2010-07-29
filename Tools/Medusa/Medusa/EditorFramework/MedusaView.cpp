// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MedusaView.cpp : CMedusaView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Medusa.h"
#endif

#include "MedusaDoc.h"
#include "MedusaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMedusaView

IMPLEMENT_DYNCREATE(CMedusaView, CView)

BEGIN_MESSAGE_MAP(CMedusaView, CView)
	ON_WM_CREATE()
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMedusaView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMedusaView 构造/析构

CMedusaView::CMedusaView()
{
	// TODO: 在此处添加构造代码

}

CMedusaView::~CMedusaView()
{
}

BOOL CMedusaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}
int CMedusaView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	theApp.OnRenderWindowCreate(this);

}
// CMedusaView 绘制

void CMedusaView::OnDraw(CDC* /*pDC*/)
{
	
	// TODO: 在此处为本机数据添加制代码
}


// CMedusaView 打印


void CMedusaView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMedusaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMedusaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMedusaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMedusaView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMedusaView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMedusaView 诊断

#ifdef _DEBUG
void CMedusaView::AssertValid() const
{
	CView::AssertValid();
}

void CMedusaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMedusaDoc* CMedusaView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMedusaDoc)));
	return (CMedusaDoc*)m_pDocument;
}
#endif //_DEBUG


// CMedusaView 消息处理程序
BEGIN_NAMESPACE_XEVOL3D;
bool ConvertToXEvolMsg(xWindowMsg& WinMsg , HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
END_NAMESPACE_XEVOL3D;

BOOL CMedusaView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	NS_XEVOL3D::xWindowMsg msg;
	ConvertToXEvolMsg(msg , GetSafeHwnd() , message , wParam , lParam);
	if( (msg.MsgID >= WIN_KEYBOARD_MESSAGE_BEGIN && msg.MsgID < WIN_KEYBOARD_MESSAGE_END) || (msg.MsgID >= WIN_MOUSE_MESSAGE_BEGIN && msg.MsgID < WIN_MOUSE_MESSAGE_END)             )
	{

		CMedusaMainUI* pUI = dynamic_cast<CMedusaMainUI*>( GetMedusaEditor()->GetUI() );
		if( pUI->FireWindowMsg(msg) == false)
		{
			GetEditorEnv()->onMessage(msg);
		}
	}
	else 
	{
		GetEditorEnv()->onMessage(msg);		
	}
	return CView::OnWndMsg(message, wParam, lParam, pResult);
}