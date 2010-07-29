// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// Medusa.h : Medusa 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "../Resource.h"       // main symbols
#include "../Editor/xMedusaEditorImpl.h"
#include "../Editor/EditorEnv.h"
#include <vector>
#include <string>

class CMedusaView;
// CMedusaApp:
// 有关此类的实现，请参阅 Medusa.cpp
//

class CLocaleToolbas
{
	CMEdUiToolBarInfo m_LocaleToolBar[1];
public:
	CLocaleToolbas()
	{

	}

	void Register()
	{
		m_LocaleToolBar[0].m_funcCallback = NULL;
		m_LocaleToolBar[0].m_startID = -1;
		m_LocaleToolBar[0].m_hDll = AfxGetResourceHandle();
		m_LocaleToolBar[0].m_ResID = IDR_MAINFRAME_256;
		wcscpy(m_LocaleToolBar[0].m_name  , L"MainToolbar");
		wcscpy(m_LocaleToolBar[0].m_title , L"Standard Toolbar");	
		m_LocaleToolBar[0].m_OriginBtnIDs = NULL;
		m_LocaleToolBar[0].m_hToolbar = NULL;
		m_LocaleToolBar[0].m_nButton = 0;
		m_LocaleToolBar[0].SetType(CMEdUiToolBarInfo::eToolBar);
		GetMedusaEditor()->GetUI()->RegisteToolbar(m_LocaleToolBar);
	}
};

class CMedusaApp : public CWinAppEx  , public CEditorEnv
{
public:
	//接口
	void              SetAppName(const wchar_t* _appName);
	void              SetUILayout(const wchar_t* layoutFile , const wchar_t* layoutName , bool bRelPath);
	void              SetInitSceneFile(const wchar_t* sceneFile);
	void              SetWorkDir(const wchar_t* _workDir);
	std::wstring      GetWorkDir(){ return m_WorkDir; }
	CMedusaEditor&    GetEditor() { return m_MedusaEditor ; }

	BOOL              InitMedusaEditor();
	bool              OnMainWindowCreate(CMainFrame* pMainFrame);
	bool              OnRenderWindowCreate(CMedusaView* pView);
	void              LoadMedusaUI(NS_XEVOL3D::xXmlNode* pNode , nsMedusaEditor::IMEdUIElement* pRootPanel , nsMedusaEditor::IMEdMainUI* pMainUI );
public:
	CMedusaApp();


protected:
	CMedusaEditor                           m_MedusaEditor;
	std::wstring                            m_AppName;
	std::wstring                            m_AppCfgFile ;
	std::wstring                            m_WorkDir;
	std::wstring                            m_UILayoutFile;
	std::wstring                            m_UILayoutName;
	std::wstring                            m_InitSceneFile;
	CLocaleToolbas                          m_LocaleToolbar;
	CMedusaView*                            m_pRenderWindow;


// 重写
public:
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL InitInstance();
	virtual int  ExitInstance();


	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void CreateFileView();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();


	DECLARE_MESSAGE_MAP()

public:
	// MFC自己的属性
	UINT  m_nAppLook;
    afx_msg void OnEditUndo();
};

extern CMedusaApp theApp;
std::wstring  _MEDUSA_RESPATH(const wchar_t* _path);