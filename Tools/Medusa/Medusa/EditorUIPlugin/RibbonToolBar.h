// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "CalendarBar.h"
#include "..\Resource.h"
#include "DockPaneContainer.h"
#include <vector>
#include "BaseLib/xIDManager.h"

class CRbToolBarCategoryInfo
{
public:
	typedef std::pair<std::ds_wstring , CMFCRibbonPanel*> MFCRibbonPanelInfo;
	typedef std::vector< MFCRibbonPanelInfo > vMFCRibbonPanelInfos ; 

	vMFCRibbonPanelInfos          m_Panels;
	CMFCRibbonCategory*           m_pCategory;
	std::ds_wstring               m_CateName;

public:
	void             AddPanel(const wchar_t* pName , CMFCRibbonPanel* pPanel);
	CMFCRibbonPanel* FindPanel(const wchar_t* name);
};

class CRibbonToolBar
{
public:
	void                    Init(CWnd* pFrame);
	CRbToolBarCategoryInfo* AddCategory(const wchar_t* pName , CMFCRibbonCategory* pCategory);
	CRbToolBarCategoryInfo* FindCategory(const wchar_t* _name);
	CMFCRibbonPanel*        FindPanel(const wchar_t* name);
	CMFCRibbonPanel*        FindPanel(const wchar_t* catName , const wchar_t* name);

public:
	typedef std::vector<CRbToolBarCategoryInfo*>  vCRbToolBarCategoryInfo;
public:
	CMFCRibbonBar                       m_wndRibbonBar;
	CMFCRibbonApplicationButton         m_MainButton;
	vCRbToolBarCategoryInfo             m_RibbonToolBarCategorys;
	CMFCToolBarImages                   m_PanelImages;	
};


