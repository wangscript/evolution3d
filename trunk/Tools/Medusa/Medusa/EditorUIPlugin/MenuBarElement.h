
#pragma once
#include "../Editor/xMedusaEditorImpl.h"
USING_NS_MDED;

class CMEdUiMenuBar : public nsMedusaEditor::IMEdUIElement
{
// Construction
public:
	CMEdUiMenuBar();
    ~CMEdUiMenuBar();
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

	IMPL_REFCOUNT_OBJECT_INTERFACE(CMEdUiMenuBar);
protected:
	CMFCMenuBar      m_wndMenuBar;
};

