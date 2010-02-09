
#include "stdafx.h"
#include "ShellTreeList.h"
#include "../EditorFramework/MainFrm.h"
#include "../Resource.h"
#include <BaseLib/xStringTable.h>
USING_NS_MDED
USING_NS_XEVOL3D

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define _MEdUI_NAME L"ShellTreeList";
#define _MEdUI_DESC L"ShellTreeList";
IMPL_OBJECT_FACTORY_NO_ARG(CShellTreeList , IMEdUIElement, CShellTreeListFactory, IMEdUIElementFactory , CMEdUIElFactoryMgr , _MEdUI_NAME , _MEdUI_DESC);

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar
const wchar_t* CShellTreeList::typeName()
{
	return _MEdUI_NAME;
}



CShellTreeList::CShellTreeList( ) 
{

}

CShellTreeList::~CShellTreeList()
{
}

bool  CShellTreeList::onDetach()
{
	return true;
}

bool  CShellTreeList::onAttach()
{
	m_wndTree.ShowWindow(SW_SHOW);
	m_wndTree.UpdateWindow();

	return true ;
}

bool  CShellTreeList::onMEdUIEvent(eMEUIEvent _event , int param, int param2)
{
     return false;
}

bool  CShellTreeList::createMEdUI(HWND hMainWnd)
{
	CWnd* pWnd = CWnd::FromHandle(hMainWnd);
	
	m_hICON = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_SHELLTREE));

	CRect rectDummy(0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	m_wndTree.Create(dwTreeStyle, rectDummy, pWnd, 1200);
    return true;
}

bool  CShellTreeList::destroyMEdUI()
{
    m_wndTree.DestroyWindow();
	return true;
}
bool  CShellTreeList::HideMEdUI()
{
	m_wndTree.ShowWindow(SW_HIDE);
	return true;
}

bool  CShellTreeList::ShowMEdUI()
{
	m_wndTree.ShowWindow(SW_SHOW);
	m_wndTree.UpdateWindow();
	return true;
}

HWND CShellTreeList::hWnd()
{
	return m_wndTree.GetSafeHwnd();
}

const wchar_t* CShellTreeList::title()
{
	return  _TRANSLATE(L"Folders");
}

const wchar_t* CShellTreeList::name() 
{
	return _TRANSLATE(L"Folders");

}

HICON CShellTreeList::hIcon()
{
	return m_hICON;
}

void CShellTreeList::setID(UINT id)
{
	m_CtrlID = id;
}

UINT CShellTreeList::getID()
{
	return m_CtrlID;
}
