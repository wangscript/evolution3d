
#pragma once
#include "../Editor/xMedusaEditorImpl.h"
USING_NS_MDED;
USING_NS_XEVOL3D;

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;


class MEDUSA_API CMEdUiToolBar : public nsMedusaEditor::IMEdUIElement
{
// Construction
public:
	IMPL_REFCOUNT_OBJECT_INTERFACE(CMEdUiToolBar);
	CMEdUiToolBar(CMEdUiToolBarInfo* pMEUIToolbar = NULL);
	CMEdUiToolBar(UINT ID);
    ~CMEdUiToolBar();
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
    bool             IsVisible() { return m_wndToolBar.IsVisible() != FALSE; } 

protected:
	CMFCToolBar           m_wndToolBar;
	UINT                  m_ResID;
	CMEdUiToolBarInfo*    m_ToolbarInfo;
};




class MEDUSA_API CMEdUiRibbonBar : public nsMedusaEditor::CMEdUiBaseToolbarElement
{
// Construction
public:
	IMPL_REFCOUNT_OBJECT_INTERFACE(CMEdUiToolBar);
	CMEdUiRibbonBar(CMEdUiToolBarInfo* pMEUIToolbar = NULL);
	CMEdUiRibbonBar(UINT ID);
    ~CMEdUiRibbonBar();
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
    CMFCRibbonBar*   getRibbonBar(){ return & m_wndRibbonBar ; }

    BOOL             IsChecked(int ctrlIdx);
    int              GetSliderPos(int ctrlIdx);
    bool             IsVisible() { return m_wndRibbonBar.IsVisible() != FALSE ; } 
    void             SetChecked(int ctrlIdx , BOOL bFlag);
    void             SetSliderPos(int ctrlIdx  , int pos);
protected:
	CMFCRibbonBar         m_wndRibbonBar;
	UINT                  m_ResID;
	CMEdUiToolBarInfo*    m_ToolbarInfo;
    
};
