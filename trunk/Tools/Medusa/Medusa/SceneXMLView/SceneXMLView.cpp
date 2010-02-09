// SenceXMLView.cpp : implementation file
//

#include "stdafx.h"
#include "..\Resource.h"
#include "SceneXMLView.h"
#include "SceneXMLDoc.h"
#include "SceneXMLViewFrame.h"
#include "BaseLib/xStringTable.h"
#include "BaseLib/xXmlDocument.h"
#include <string>
#include <fstream>
using namespace std;
USING_NS_XEVOL3D

#define _MEdUI_NAME L"SceneXMLView";
#define _MEdUI_DESC L"Scene XML View";
IMPL_OBJECT_FACTORY_NO_ARG(CMEdSceneXMLView , IMEdUIElement, CMEdSceneXMLViewFactory, IMEdUIElementFactory , CMEdUIElFactoryMgr , _MEdUI_NAME , _MEdUI_DESC);
/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar
const wchar_t* CMEdSceneXMLView::typeName()
{
	return _MEdUI_NAME;
}
CMEdSceneXMLView::CMEdSceneXMLView() 
{
}

CMEdSceneXMLView::~CMEdSceneXMLView()
{
}

bool  CMEdSceneXMLView::onDetach()
{
	return true;
}

bool  CMEdSceneXMLView::onAttach()
{
	m_pFrame->ShowWindow(SW_SHOW);
	m_pFrame->UpdateWindow();
	return true ;
}

bool  CMEdSceneXMLView::onMEdUIEvent(eMEUIEvent _event , int param, int param2)
{
	if(_event == eMEUIEvent::eMEUI_SceneChanged || _event == eMEUI_SceneInsertNode || _event == eMEUI_SceneRemoveNode)
	{
		xXmlDocument doc;
		doc.insertNode(L"scene");
		ISceneGraph* pScene = (ISceneGraph*)param;
		pScene->save(doc.root() );

		std::wostringstream out;
		doc.save(out);
		CSceneXMLView* pView = (CSceneXMLView*)m_pView ;
		pView->SetWindowText(  (wchar_t*) out.str().c_str() );
		return true;
	}
	return false;
}

bool  CMEdSceneXMLView::createMEdUI(HWND hMainWnd)
{
	
	//ÆäËûµÄView;
	m_pDocTemplate = new CMultiDocTemplate(IDR_MedusaDocumentTYPE,
		RUNTIME_CLASS(CSceneXMLDoc),
		RUNTIME_CLASS(CMEdSceneXMLViewFrame), // custom MDI child frame
		RUNTIME_CLASS(CSceneXMLView));
	if (!m_pDocTemplate)
		return FALSE;
	

	CDocument* m_pDocument = m_pDocTemplate->OpenDocumentFile(NULL);
	POSITION pos = m_pDocument->GetFirstViewPosition();
	m_pView      = m_pDocument->GetNextView(pos);
	m_pFrame = m_pView->GetParentFrame();
	m_pDocument->SetTitle( _TRANSLATE(L"XMLView") );
	m_pFrame->OnUpdateFrameTitle(TRUE);


	return true;
}

bool  CMEdSceneXMLView::destroyMEdUI()
{
	delete m_pDocTemplate;
	m_pDocTemplate = NULL;
	return true;
}
bool  CMEdSceneXMLView::HideMEdUI()
{
	m_pFrame->ShowWindow(SW_HIDE);
	return true;
}

bool  CMEdSceneXMLView::ShowMEdUI()
{
	m_pFrame->ShowWindow(SW_SHOW);
	m_pFrame->UpdateWindow();
	return true;
}

HWND  CMEdSceneXMLView::hWnd()
{
	return m_pFrame->GetSafeHwnd();
}

const wchar_t*  CMEdSceneXMLView::title()
{
	return _TRANSLATE(L"XMLView");
}
const wchar_t*  CMEdSceneXMLView::name()
{
	return _TRANSLATE(L"SceneXMLView" );
}

// CSceneXMLView

IMPLEMENT_DYNCREATE(CSceneXMLView, CRichEditView)

CSceneXMLView::CSceneXMLView()
{

}

CSceneXMLView::~CSceneXMLView()
{
}

BEGIN_MESSAGE_MAP(CSceneXMLView, CRichEditView)
END_MESSAGE_MAP()


// CSceneXMLView diagnostics

#ifdef _DEBUG
void CSceneXMLView::AssertValid() const
{
	CRichEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CSceneXMLView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSceneXMLView message handlers
