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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RibbonToolBar.h"
#include "MainFrm.h"
#include "BaseLib/xStringTable.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
USING_NS_XEVOL3D
void CRibbonToolBar::Init(CWnd* pFrame)
{
	m_wndRibbonBar.Create(pFrame);
	// Load panel images:
	m_PanelImages.SetImageSize(CSize(16, 16));
	m_PanelImages.Load(IDB_BUTTONS);

	// Init main button:
	m_MainButton.SetImage(IDB_MAIN);
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(_TRANSLATE(L"File"));

	//初始化那个大菜单
	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));
	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(_TRANSLATE(L"File"), IDB_FILESMALL, IDB_FILELARGE);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW    , _TRANSLATE(L"&New")       , 0, 0));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN   , _TRANSLATE(L"&Open...")   , 1, 1));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE   , _TRANSLATE(L"&Save")      , 2, 2));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SAVE_AS, _TRANSLATE(L"Save &As..."), 3, 3));

	//大菜单的Print
	CMFCRibbonButton* pBtnPrint = new CMFCRibbonButton(ID_FILE_PRINT, _TRANSLATE(L"Print"), 4, 4);
	pBtnPrint->SetKeys(_T("p"), _T("w"));
	pBtnPrint->AddSubItem(new CMFCRibbonLabel(_TRANSLATE(L"Preview and print the document")));
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_PREVIEW, _TRANSLATE(L"Print Pre&view"), 5, 5, TRUE));
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_SETUP, _TRANSLATE(L"Print Set&up"), 6, 6, TRUE));
	pMainPanel->Add(pBtnPrint);

	//退出
	pMainPanel->Add(new CMFCRibbonSeparator(TRUE));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_CLOSE, _TRANSLATE(L"&Close"), 9, 9));

	//最近文件列表
	pMainPanel->AddRecentFilesList(_TRANSLATE(L"Recent Documents"));
	pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, _TRANSLATE(L"E&xit"), 15));

	// Add "Home" category with "Clipboard" panel:
	CMFCRibbonCategory* pCategoryHome = m_wndRibbonBar.AddCategory(_TRANSLATE(L"&Main"), IDB_WRITESMALL, IDB_WRITELARGE);
	CRbToolBarCategoryInfo* pCgInfo = AddCategory(L"Home" , pCategoryHome);

	// Create "Clipboard" panel:
	CMFCRibbonPanel* pPanelClipboard = pCategoryHome->AddPanel(_TRANSLATE(L"Clipboard\nd"), m_PanelImages.ExtractIcon(27));
	pCgInfo->AddPanel(L"Clipboard" , pPanelClipboard);

	CMFCRibbonButton* pBtnPaste = new CMFCRibbonButton(ID_EDIT_PASTE, _TRANSLATE(L"Paste\nv"), 0, 0);
	pPanelClipboard->Add(pBtnPaste);

	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_CUT, _TRANSLATE(L"Cut\nx"), 1));
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_COPY, _TRANSLATE(L"Copy\nc"), 2));
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_SELECT_ALL, _TRANSLATE(L"Select All\na"), -1));

	// Create and add a "View" panel:

	CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel(_TRANSLATE(L"View"), m_PanelImages.ExtractIcon (7));
	pCgInfo->AddPanel(L"View" , pPanelView);
	CMFCRibbonButton* pBtnStatusBar = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, _TRANSLATE(L"Status Bar"));
	pPanelView->Add(pBtnStatusBar);
	CMFCRibbonButton* pBtnCaptionBar = new CMFCRibbonCheckBox(ID_VIEW_CAPTION_BAR, _TRANSLATE(L"Caption Bar"));
	pPanelView->Add(pBtnCaptionBar);

	// Add elements to the right side of tabs:
	CMFCRibbonButton* pVisualStyleButton = new CMFCRibbonButton(-1, _TRANSLATE(L"Style"), -1, -1);
	pVisualStyleButton->SetMenu(IDR_THEME_MENU, FALSE /* No default command */, TRUE /* Right align */);

	pVisualStyleButton->SetToolTipText(_TRANSLATE(L"Modify Visual Style"));
	pVisualStyleButton->SetDescription(_TRANSLATE(L"Choose one of the Office 2007 themes"));
	m_wndRibbonBar.AddToTabs(pVisualStyleButton);

	// Add quick access toolbar commands:
	CList<UINT, UINT> lstQATCmds;

	lstQATCmds.AddTail(ID_FILE_NEW);
	lstQATCmds.AddTail(ID_FILE_OPEN);
	lstQATCmds.AddTail(ID_FILE_SAVE);
	lstQATCmds.AddTail(ID_FILE_PRINT_DIRECT);

	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);

	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));

}


void CRbToolBarCategoryInfo::AddPanel(const wchar_t* pName , CMFCRibbonPanel* pPanel)
{
	MFCRibbonPanelInfo info(pName , pPanel);
	m_Panels.push_back(info);
}

CMFCRibbonPanel* CRbToolBarCategoryInfo::FindPanel(const wchar_t* name)
{
	std::ds_wstring _name = name;
	for(size_t i = 0 ; i < m_Panels.size() ;  i ++)
	{
		if(m_Panels[i].first == _name)
		{
			return m_Panels[i].second;
		}
	}
	return NULL;
}


CRbToolBarCategoryInfo* CRibbonToolBar::AddCategory(const wchar_t* pName , CMFCRibbonCategory* pCategory)
{
	CRbToolBarCategoryInfo* p = new CRbToolBarCategoryInfo;
	p->m_CateName = pName;
	p->m_pCategory = pCategory;
	m_RibbonToolBarCategorys.push_back(p);
	return p;
}

CRbToolBarCategoryInfo* CRibbonToolBar::FindCategory(const wchar_t* _name)
{
	std::ds_wstring name = _name;
	for(size_t k = 0 ; k < m_RibbonToolBarCategorys.size() ; k ++)
	{
		if(m_RibbonToolBarCategorys[k]->m_CateName == name)
		{
			return m_RibbonToolBarCategorys[k];
		}
	}
	return NULL;
}


CMFCRibbonPanel* CRibbonToolBar::FindPanel(const wchar_t* name)
{
	for(size_t k = 0 ; k < m_RibbonToolBarCategorys.size() ; k ++)
	{
		CMFCRibbonPanel* p = m_RibbonToolBarCategorys[k]->FindPanel(name);
		if( p ) p;
	}
	return NULL;
}

CMFCRibbonPanel* CRibbonToolBar::FindPanel(const wchar_t* catName , const wchar_t* name)
{
	CRbToolBarCategoryInfo* pCtInfo = FindCategory(catName);
	if(pCtInfo == NULL)
		return NULL;
	return pCtInfo->FindPanel(name);
}
