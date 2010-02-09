#pragma once
#include "../Editor/xMedusaEditorImpl.h"
USING_NS_MDED;

// CSceneXMLView view
class CSceneXMLDoc;
class CSceneXMLView : public CRichEditView
{
	DECLARE_DYNCREATE(CSceneXMLView)

protected:
	CSceneXMLView();           // protected constructor used by dynamic creation
	virtual ~CSceneXMLView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()


};

class CMEdSceneXMLView : public nsMedusaEditor::IMEdUIElement
{
	// Construction
public:
	CMEdSceneXMLView();
	~CMEdSceneXMLView();
	// Attributes
public:
	bool             onDetach();
	bool             onAttach();
	bool             onMEdUIEvent(eMEUIEvent _event , int param, int param2);
	bool             createMEdUI(HWND hMainWnd);
	bool             destroyMEdUI() ;
	bool             HideMEdUI();
	bool             ShowMEdUI();
	HWND             hWnd() ;
	const wchar_t*   title();
	const wchar_t*   name();
	const wchar_t*   typeName();

	IMPL_REFCOUNT_OBJECT_INTERFACE(CMEdSceneXMLView);
protected:
	CMultiDocTemplate* m_pDocTemplate;
	CView*     m_pView  ;
	CFrameWnd* m_pFrame ;
	CSceneXMLDoc* m_pDocument;
	
};

