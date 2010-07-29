
#include "stdafx.h"
#include "ToolBarElement.h"
#include "../EditorFramework/MainFrm.h"
#include "..\Resource.h"
#include <BaseLib/xStringTable.h>
USING_NS_MDED
USING_NS_XEVOL3D

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define _MEdUI_TOOLBAR_NAME L"Toolbar";
#define _MEdUI_TOOLBAR_DESC L"Toolbar";
IMPL_OBJECT_FACTORY_NO_ARG(CMEdUiToolBar , IMEdUIElement, CMEdUiToolBarFactory, IMEdUIElementFactory , CMEdUIElFactoryMgr , _MEdUI_TOOLBAR_NAME , _MEdUI_TOOLBAR_DESC);

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar
const wchar_t* CMEdUiToolBar::typeName()
{
	return _MEdUI_TOOLBAR_NAME;
}
CMEdUiToolBar::CMEdUiToolBar(CMEdUiToolBarInfo* pMEUIToolbar) 
{
	m_ToolbarInfo = pMEUIToolbar;
	m_ResID = IDR_MAINFRAME_256;
}

CMEdUiToolBar::CMEdUiToolBar(UINT ID ) 
{
	m_ResID = ID;
	m_ToolbarInfo = NULL;
}

CMEdUiToolBar::~CMEdUiToolBar()
{
	m_ToolbarInfo->m_hToolbar = NULL;
}

bool  CMEdUiToolBar::onDetach()
{
	return true;
}

bool  CMEdUiToolBar::onAttach()
{
	m_wndToolBar.ShowWindow(SW_SHOW);
	m_wndToolBar.UpdateWindow();

	return true ;
}

bool  CMEdUiToolBar::onMEdUIEvent(eMEUIEvent _event , int param, int param2)
{
     return false;
}

bool  CMEdUiToolBar::createMEdUI(HWND hMainWnd)
{
	std::wstring _toolbarTitle = _TRANSLATE(L"Standard Toolbar");
	CWnd* pWnd = CWnd::FromHandle(hMainWnd);
	
	if(m_ToolbarInfo == NULL)
	{
		if (!m_wndToolBar.CreateEx(pWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndToolBar.LoadToolBar(m_ResID))
		{
			TRACE0("Failed to create toolbar\n");
			return false;      // fail to create
		}
	}
	else
	{
		HINSTANCE hInstance = AfxGetResourceHandle();
		m_ResID = m_ToolbarInfo->m_ResID;
		AfxSetResourceHandle( m_ToolbarInfo->m_hDll );
		if (!m_wndToolBar.CreateEx(pWnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndToolBar.LoadToolBar(m_ToolbarInfo->m_ResID))
		{
			TRACE0("Failed to create toolbar\n");
			AfxSetResourceHandle(hInstance );
			return false;      // fail to create
		}
		AfxSetResourceHandle(hInstance );
		_toolbarTitle = m_ToolbarInfo->m_title;
	}

	m_wndToolBar.SetWindowText(_toolbarTitle.c_str() );
	//m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, _TRANSLATE(L"Customize Toolbar"));

	// Allow user-defined toolbars operations:
	//InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	CMainFrame* pFrame = (CMainFrame*)pWnd;
	pFrame->DockPane(&m_wndToolBar);

	CMFCToolBar::EnableQuickCustomization();
    return true;
}

bool  CMEdUiToolBar::destroyMEdUI()
{
    m_wndToolBar.DestroyWindow();
	return true;
}
bool  CMEdUiToolBar::HideMEdUI()
{
	m_wndToolBar.ShowWindow(SW_HIDE);
	return true;
}

bool  CMEdUiToolBar::ShowMEdUI()
{
	m_wndToolBar.ShowWindow(SW_SHOW);
	m_wndToolBar.UpdateWindow();
	return true;
}

HWND CMEdUiToolBar::hWnd()
{
	return m_wndToolBar.GetSafeHwnd();
}

const wchar_t* CMEdUiToolBar::title()
{
	std::wstring _toolbarTitle = _TRANSLATE(L"Standard Toolbar");
	if(m_ToolbarInfo )
	{
		_toolbarTitle = m_ToolbarInfo->m_title;
	}
	return _toolbarTitle.c_str();
}

const wchar_t* CMEdUiToolBar::name() 
{
	std::wstring _toolbarName = L"Standard";
	if(m_ToolbarInfo )
	{
		_toolbarName = m_ToolbarInfo->m_name;
	}
	return _toolbarName.c_str();
}


//-------------------------------
#define _MEdUI_RIBBONBAR_NAME L"Ribbonbar";
#define _MEdUI_RIBBONBAR_DESC L"Ribbonbar";
IMPL_OBJECT_FACTORY_NO_ARG(CMEdUiRibbonBar , IMEdUIElement, CMEdUiRibbonBarFactory, IMEdUIElementFactory , CMEdUIElFactoryMgr , _MEdUI_RIBBONBAR_NAME , _MEdUI_RIBBONBAR_DESC);

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar
const wchar_t* CMEdUiRibbonBar::typeName()
{
	return _MEdUI_RIBBONBAR_NAME;
}
CMEdUiRibbonBar::CMEdUiRibbonBar(CMEdUiToolBarInfo* pMEUIToolbar) 
{
	m_ToolbarInfo = pMEUIToolbar;
	m_ResID       = IDR_MAINFRAME_256;
}

CMEdUiRibbonBar::CMEdUiRibbonBar(UINT ID ) 
{
	m_ResID = ID;
	m_ToolbarInfo = NULL;
}

CMEdUiRibbonBar::~CMEdUiRibbonBar()
{
	m_ToolbarInfo->m_hToolbar = NULL;
}

bool  CMEdUiRibbonBar::onDetach()
{
	return true;
}

bool  CMEdUiRibbonBar::onAttach()
{
	//m_wndToolBar.ShowWindow(SW_SHOW);
	//m_wndToolBar.UpdateWindow();

	return true ;
}

bool  CMEdUiRibbonBar::onMEdUIEvent(eMEUIEvent _event , int param, int param2)
{
     return false;
}

bool  CMEdUiRibbonBar::createMEdUI(HWND hMainWnd)
{
	std::wstring _toolbarTitle = _TRANSLATE(L"RibbonBar");
	CWnd* pWnd = CWnd::FromHandle(hMainWnd);
    CMainFrame* pFrame = (CMainFrame*)pWnd;
	
	if(m_ToolbarInfo != NULL)
	{
        
        const char* pUtf8Data =  (const char*)m_ToolbarInfo->ProcessRibbonBarData(-1);   
         m_ResID = m_ToolbarInfo->m_ResID;
        HINSTANCE hInstance = AfxGetResourceHandle();
        
        AfxSetResourceHandle( m_ToolbarInfo->m_hDll );
        m_wndRibbonBar.Create(pFrame);  
        //m_wndRibbonBar.LoadFromResource( m_ToolbarInfo->m_ResID);
        m_wndRibbonBar.LoadFromBuffer((const wchar_t*)(pUtf8Data  ) );//pUtf8Data , utf8Len);
        m_ToolbarInfo->FreeData((void*)pUtf8Data);

        AfxSetResourceHandle(hInstance );
        _toolbarTitle = m_ToolbarInfo->m_title;
        return true;        
	}
    return false;    
}

bool  CMEdUiRibbonBar::destroyMEdUI()
{
    //m_wndToolBar.DestroyWindow();
	return true;
}
bool  CMEdUiRibbonBar::HideMEdUI()
{
	//m_wndToolBar.ShowWindow(SW_HIDE);
	return true;
}

bool  CMEdUiRibbonBar::ShowMEdUI()
{
	//m_wndToolBar.ShowWindow(SW_SHOW);
	//m_wndToolBar.UpdateWindow();
	return true;
}

HWND CMEdUiRibbonBar::hWnd()
{
	return m_wndRibbonBar.GetSafeHwnd();
}

const wchar_t* CMEdUiRibbonBar::title()
{
	std::wstring _toolbarTitle = _TRANSLATE(L"Ribbonbar");
	if(m_ToolbarInfo )
	{
		_toolbarTitle = m_ToolbarInfo->m_title;
	}
	return _toolbarTitle.c_str();
}

const wchar_t* CMEdUiRibbonBar::name() 
{
	if(m_ToolbarInfo )
	{
		return m_ToolbarInfo->m_name;
	}
	return L"Default";
}

BOOL  CMEdUiRibbonBar::IsChecked(int ctrlIdx)
{
    int _cmdid = m_ToolbarInfo->MapOriginID2CtrlID(ctrlIdx);

    CArray<CMFCRibbonBaseElement* , CMFCRibbonBaseElement*> btns;
    m_wndRibbonBar.GetElementsByID(_cmdid , btns );
    for(int i = 0 ; i < btns.GetCount() ; i ++)
    {
        CMFCRibbonBaseElement* pElement = btns[i];
        return pElement->IsChecked();
    }
    return FALSE;

}

int  CMEdUiRibbonBar::GetSliderPos(int ctrlIdx)
{
    int _cmdid = m_ToolbarInfo->MapOriginID2CtrlID(ctrlIdx);
    CArray<CMFCRibbonBaseElement* , CMFCRibbonBaseElement*> btns;
    m_wndRibbonBar.GetElementsByID(_cmdid , btns );
    for(int i = 0 ; i < btns.GetCount() ; i ++)
    {
        CMFCRibbonSlider* pElement = dynamic_cast<CMFCRibbonSlider*>(btns[i]);
        if(pElement)
        {
           return pElement->GetPos();
        }
    }
    return 0;
}

void   CMEdUiRibbonBar::SetChecked(int ctrlIdx , BOOL bFlag)
{
    int _cmdid = m_ToolbarInfo->MapOriginID2CtrlID(ctrlIdx);
    CArray<CMFCRibbonBaseElement* , CMFCRibbonBaseElement*> btns;
    m_wndRibbonBar.GetElementsByID(_cmdid , btns );
    for(int i = 0 ; i < btns.GetCount() ; i ++)
    {
        CMFCRibbonCheckBox* pElement = dynamic_cast<CMFCRibbonCheckBox*>(btns[i]);
        if(pElement)
        {

        }
    }
}

void   CMEdUiRibbonBar::SetSliderPos(int ctrlIdx  , int pos)
{
    int _cmdid = m_ToolbarInfo->MapOriginID2CtrlID(ctrlIdx);
    CArray<CMFCRibbonBaseElement* , CMFCRibbonBaseElement*> btns;
    m_wndRibbonBar.GetElementsByID(_cmdid , btns );
    for(int i = 0 ; i < btns.GetCount() ; i ++)
    {
        CMFCRibbonSlider* pElement = dynamic_cast<CMFCRibbonSlider*>(btns[i]);
        if(pElement)
        {
            pElement->SetPos(pos);
        }
    }
}
