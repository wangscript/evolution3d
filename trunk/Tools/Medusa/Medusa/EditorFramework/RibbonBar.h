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
#include "../Resource.h"
#include <map>
#include <vector>
#include <string>
class CRibbonCategory;
class CRibbonPanel;
class CRibbonBarWrapper ;
class CRibbonPanel
{
public:
	CRibbonPanel(CRibbonCategory* pCategory , const wchar_t* _name);
	CMFCRibbonPanel* RibbonPtr();
public:
	std::wstring      m_Name;
	CMFCRibbonPanel*  m_pPanel;
	CRibbonCategory*  m_pCategory;
};


class CRibbonCategory
{
public:
	typedef std::vector<CRibbonPanel*> vRibbonPanels;
public:
	CRibbonCategory(CMFCRibbonBar* pRibbonBar, const wchar_t* _name);
    CRibbonPanel*   findPanel(const wchar_t* _name);
	CRibbonPanel*   createPanel(const wchar_t* _name);
	CMFCRibbonCategory* RibbonCategoryPtr();
public:	
	std::wstring                 m_Name;
	vRibbonPanels                m_vPanels;
	CMFCRibbonBar*               m_pRibbonBars;
	CMFCRibbonCategory*          m_RibbonCategory;
};

class CRibbonBarWrapper 
{

protected:  // control bar embedded members
	CMFCRibbonBar               m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages           m_PanelImages;
	CMFCRibbonStatusBar         m_wndStatusBar;
	CRibbonPanel*               m_pMainPanel;
	               
	// Generated message map functions
public:
	void             InitializeRibbon(CWnd* pParent);
	CRibbonCategory* findCategory(const wchar_t* _name);
	CRibbonCategory* createCategory(const wchar_t* _name);
public:
	typedef std::vector<CRibbonCategory*> vRibbonCategorys;

public:
	vRibbonCategorys        m_Categorys;

};


