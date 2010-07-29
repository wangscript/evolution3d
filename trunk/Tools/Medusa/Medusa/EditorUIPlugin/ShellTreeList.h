
#pragma once
#include "../Editor/xMedusaEditorImpl.h"
USING_NS_MDED;
USING_NS_XEVOL3D;


class CShellTreeList : public nsMedusaEditor::IMEdDockPane
{
// Construction
public:
	IMPL_REFCOUNT_OBJECT_INTERFACE(CShellTreeList);
	CShellTreeList();
    ~CShellTreeList();
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
	HICON            hIcon() ;
	void             setID(UINT id) ;
	UINT             getID() ;
    bool             IsVisible() { return m_wndTree.IsWindowVisible() ; } 

protected:
	CMFCShellTreeCtrl       m_wndTree;
	UINT                    m_ResID;
	int                     m_CtrlID;
	HICON                   m_hICON; 
};
