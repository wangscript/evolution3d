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

// Medusa.h : main header file for the Medusa application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "../Resource.h"       // main symbols
#include "../Editor/xMedusaEditorImpl.h"
#include "../Editor/EditorEnv.h"
#include <vector>
#include <string>
// CMedusaApp:
// See Medusa.cpp for the implementation of this class
//

class CMedusaApp : public CWinAppEx , public CEditorEnv
{
public:  
	void              SetAppName(const wchar_t* _appName);
	void              SetUILayout(const wchar_t* layoutFile , const wchar_t* layoutName , bool bRelPath);
    void              SetInitSceneFile(const wchar_t* sceneFile);
	void              SetWorkDir(const wchar_t* _workDir);
	std::wstring      GetWorkDir(){ return m_WorkDir; }
	CMedusaEditor&    GetEditor(){ return m_MedusaEditor ; }
public:
	CMedusaApp();
	virtual BOOL      OnIdle(LONG lCount);
	bool              OnMainWindowCreate();
	bool              AddDocTemplate(CMultiDocTemplate* pDocTemplate , const wchar_t* _name);
	// Overrides   
public:			   
	virtual BOOL      InitInstance();
	virtual BOOL      RegisteAllViews();
    BOOL              InitMedusaEditor();
				   
				   
	virtual void      PreLoadState();
	virtual void      LoadCustomState();
	virtual void      SaveCustomState();
				   
	afx_msg void      OnAppAbout();
	afx_msg void      CreateFileView();
	afx_msg void      OnFileOpen();
	afx_msg void      OnFileSave();
	afx_msg void      OnFileSaveAs();
	DECLARE_MESSAGE_MAP()
protected:
	void              LoadMedusaUI(NS_XEVOL3D::xXmlNode* pNode , nsMedusaEditor::IMEdUIElement* pRootPanel , nsMedusaEditor::IMEdMainUI* pMainUI );
protected:
	
	typedef std::vector<CMultiDocTemplate*> vMultiDocTemplates;
	typedef std::vector<CString>            vMultiDocTemplateStrings;
	CMultiDocTemplate*                      m_pMainDocTemplate;
	vMultiDocTemplates                      m_vDocTemplates;
	vMultiDocTemplateStrings                m_vDocTemplateStrings;
	CMedusaEditor                           m_MedusaEditor;
	std::wstring                            m_AppName;
	std::wstring                            m_AppCfgFile ;
	std::wstring                            m_WorkDir;
	std::wstring                            m_UILayoutFile;
	std::wstring                            m_UILayoutName;
	std::wstring                            m_InitSceneFile;


public:
	UINT                                    m_nAppLook;
	BOOL                                    m_bHiColorIcons;

	virtual int ExitInstance();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
};

extern CMedusaApp theApp;
std::wstring  _MEDUSA_RESPATH(const wchar_t* _path);

