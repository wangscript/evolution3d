
#pragma once
#include "../Editor/xMedusaEditorImpl.h"
#include "TabWindowContainer.h"
USING_NS_MDED;

class COutlookBar : public CMFCOutlookBar
{
public:
	virtual BOOL AllowShowOnPaneMenu() const 
	{
		return TRUE; 
	}

	virtual void GetPaneName(CString& strName) const
	{ 
		strName = m_strPaneName;
	}
public:
	CString m_strPaneName;
};


class CNavigatorBar :  public CMEdUIElConatiner
{
	// Construction
public:
	CNavigatorBar();
	~CNavigatorBar();

	IMPL_REFCOUNT_OBJECT_INTERFACE(CNavigatorBar);

	// Attributes
public:
	bool             onDetach();
	bool             onAttach();
	bool             onMEdUIEvent(eMEUIEvent _event , int param, int param2);
	bool             createMEdUI(HWND hMainWnd) ;
	bool             destroyMEdUI() ; 
	HWND             GetParentWnd();
	bool             HideMEdUI();	
	bool             ShowMEdUI();	 //
	HICON            hIcon();
	const wchar_t*   title() ;
	const wchar_t*   name();
	void             setID(UINT id);
	UINT             getID();
	HWND             hWnd();
	bool             AttachUIElement(nsMedusaEditor::IMEdUIElement* pPane);
	bool             DetachUIElement(nsMedusaEditor::IMEdUIElement* pPane);
	const wchar_t*   typeName();


	typedef std::map<IMEdUIElement* , CTabWindowContainer*> vCtToElements;
protected:
	COutlookBar                 m_wndNavigationBar;

	int                         m_CtrlID;
	int                         m_PaneID;
	int                         m_InitWidth;
	CMFCOutlookBarTabCtrl*      m_pOutlookBar ;
	vCtToElements               m_ctToElements;
public:	
	BOOL                        CreateOutlookBar(HWND hParent , UINT uiID, int nInitialWidth);
};

