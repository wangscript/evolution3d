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

// Medusa.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Medusa.h"
#include "MainFrm.h"

#include "EditorViewFrame.h"
#include "MedusaDoc.h"
#include "MedusaView.h"
#include "Splash.h"
#include "SplashScreenEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMedusaApp

BEGIN_MESSAGE_MAP(CMedusaApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMedusaApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CMedusaApp::CreateFileView)
	ON_COMMAND(ID_FILE_OPEN, &CMedusaApp::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE   , &CMedusaApp::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMedusaApp::OnFileSaveAs)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_EDIT_UNDO, &CMedusaApp::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CMedusaApp::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CMedusaApp::OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_REDO, &CMedusaApp::OnEditRedo)
END_MESSAGE_MAP()


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
		GetMedusaEditor()->GetUI()->RegisteToolbar(m_LocaleToolBar);
	}
};
CLocaleToolbas g_LocaleToolbars;

std::wstring  _MEDUSA_RESPATH(const wchar_t* _path)    
{
	std::wstring _fullPath = std::wstring(L"./" ) + theApp.GetWorkDir() + std::wstring(L"/" ) + _path;
	std::ds_wstring _absPath = xOperationSys::singleton()->getAbsResourcePath( _fullPath.c_str() );
	std::wstring  ret = _absPath.c_str();
	return ret ;
}

CMedusaApp::CMedusaApp()
{
	m_bHiColorIcons = TRUE;
    m_AppName       = L"Medusa";
	m_WorkDir       = L"Medusa";
	m_UILayoutFile  = L"Configure/uidef.xml";
	m_UILayoutName  = L"Default";
	m_InitSceneFile = L"scene/demoScene.xml";
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
void CMedusaApp::SetWorkDir(const wchar_t* _workDir)
{
	m_WorkDir = _workDir;
}

void CMedusaApp::SetInitSceneFile(const wchar_t* sceneFile)
{
	m_InitSceneFile = sceneFile;
}

void CMedusaApp::SetAppName(const wchar_t* _appName)
{
	m_AppName = _appName;
}

void CMedusaApp::SetUILayout(const wchar_t* layoutFile , const wchar_t* layoutName , bool bRelPath)
{
	if(bRelPath == false )
		m_UILayoutFile = layoutFile;
	else
		m_UILayoutFile = _MEDUSA_RESPATH(layoutFile);
	m_UILayoutName = layoutName;
}


// The one and only CMedusaApp object

CMedusaApp theApp;

CEditorEnv * GetEditorEnv()
{
	return dynamic_cast<CEditorEnv *>(&theApp);
}

CEvolEnviroment* GetEvol3DEnv()
{
	return GetEditorEnv()->evol3DEnv();
}

xSceneSelection* GetSceneSelection()
{
	return GetEvol3DEnv()->GetSelection();
}

void CMedusaApp::OnFileSave()
{

	return ;
}

void CMedusaApp::OnFileSaveAs()
{

}

// CMedusaApp initialization
void CMedusaApp::CreateFileView()
{
	if(this->bInited() == false )
	{
		for(size_t i = 0 ; i < m_vDocTemplates.size() ; i ++)
		{
			CDocument* pDoc = m_vDocTemplates[i]->OpenDocumentFile(NULL);
			POSITION pos = pDoc->GetFirstViewPosition();
			CView* pView = pDoc->GetNextView(pos);
			CFrameWnd* pFrame = pView->GetParentFrame();
			pFrame->SetTitle(m_AppName.c_str());
			pDoc->SetTitle( m_vDocTemplateStrings[i] );
			pFrame->OnUpdateFrameTitle(TRUE);

			if(m_pMainDocTemplate == m_vDocTemplates[i])
			{
				this->SetRenderWindow( pView->GetSafeHwnd() );
			}
			this->AddDocument(pDoc);
		}

		this->initRenderer(NULL);
	}

}

void CMedusaApp::OnFileOpen()
{

}
bool  CMedusaApp::AddDocTemplate(CMultiDocTemplate* pDocTemplate , const wchar_t* _name)
{
	CWinAppEx::AddDocTemplate(pDocTemplate);
	//pDocTemplate->OpenDocumentFile(_name);
	m_vDocTemplates.push_back(pDocTemplate);
	m_vDocTemplateStrings.push_back(_name);
	return true;
}
BOOL  CMedusaApp::RegisteAllViews()
{
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MedusaDocumentTYPE,
		RUNTIME_CLASS(CMedusaDoc),
		RUNTIME_CLASS(CEditorViewFrame), // custom MDI child frame
		RUNTIME_CLASS(CMedusaView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate,_TRANSLATE(L"PreviewWindow"));
	m_pMainDocTemplate = pDocTemplate;


	return TRUE;
}

BOOL CMedusaApp::InitInstance()
{
	xXmlDocument uiDoc;
	if( false == uiDoc.load(m_UILayoutFile.c_str() )  )
	{
		MessageBox(GetActiveWindow() , L"Medusa Init failed : Missing UI configuration\n", L"Error" , MB_OK);
		return FALSE;
	}

	xXmlNode* pUISchema = uiDoc.root()==NULL?NULL : uiDoc.root()->findNode( m_UILayoutName.c_str() );
    if(pUISchema == NULL)
	{
		MessageBox(GetActiveWindow() , L"Medusa Init failed : Missing UI Schema\n", L"Error" , MB_OK);
		return FALSE;
	}
	uiDoc.unload();

	xI18N i18N;
	std::ds_wstring langFile  = std::ds_wstring(L"Language/") + i18N.getCurrentLocaleInfor()->_locale + L".dll";
	std::ds_wstring resDllName = _MEDUSA_RESPATH(langFile.c_str()).c_str() ;
	HINSTANCE hResDll  = ::LoadLibrary(resDllName.c_str() );
	if(hResDll == NULL)
	{
		langFile   = std::ds_wstring(L"Medusa/Language/") + i18N.getCurrentLocaleInfor()->_locale + L".dll";
		resDllName = _XEVOL_ABSPATH_(langFile.c_str()) ;
        hResDll  = ::LoadLibrary(resDllName.c_str() );
	}

	if(hResDll)
	{
		//m_hLangResourceDLL = hResDll;
		AfxSetResourceHandle(hResDll);
	}
	
	//LCID lcId = MAKELCID(i18N.getCurrentLocaleInfor()->_langID , SORT_DEFAULT);
	//::SetThreadLocale( lcId );
	//::SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE_SIMPLIFIED, SUBLANG_DEFAULT),SORT_DEFAULT));


	//加载引擎
	CEditorEnv::init();
	m_AppCfgFile =  std::wstring(L"Configure/") + m_AppName.c_str() + L".xml";
	m_AppCfgFile = _MEDUSA_RESPATH(m_AppCfgFile.c_str() );
    m_sysConfig.load(m_AppCfgFile.c_str() );

	CSplashScreenEx Splash ;
	bool bSplash = GetEditorEnv()->sysValue(L"splash" , L"visible" , true);
	if( bSplash )
	{
	    Splash.Create(NULL,L"CSplashScreenEx dynamic text:",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	    Splash.SetBitmap(_MEDUSA_RESPATH(L"splash.bmp").c_str(),255,0,255);
	    Splash.SetTextFont(L"Impact",100,CSS_TEXT_NORMAL);
	    Splash.SetTextRect(CRect(125,60,291,104));
	    Splash.SetTextColor(RGB(255,255,255));
	    Splash.SetTextFormat(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		Splash.Show();
        Splash.SetText(TEXT("Medusa 3D Studio ......"));
	}
    
	// your start up initialization code goes here

	// Close the splash screen
	

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	//m_pszProfileName = m_AppName.c_str();
	SetRegistryKey( m_AppName.c_str() );

	LoadStdProfileSettings(0);  // Load standard INI file options

	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	RegisteAllViews();
   
     
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	//在窗口开始显示之前。初始化编辑器的插件系统，界面配置系统等
	m_MedusaEditor.SetMainFrame(pMainFrame);
    g_LocaleToolbars.Register();
    
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	pMainFrame->SetTitle(m_AppName.c_str());
	m_pMainWnd = pMainFrame;
	//m_pMainWnd->ShowWindow(SW_HIDE);
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
   

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


    OnMainWindowCreate();
    //this->LoadState(pMainFrame);
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();


	if( bSplash )
	{
		Splash.Hide();
	}

	if(m_InitSceneFile.length() > 0 )
	{
		//Now load the scene
		evol3DEnv()->loadScene( _MEDUSA_RESPATH(m_InitSceneFile.c_str()).c_str() );
	}


	pMainFrame->SetWindowText(m_AppName.c_str() );

	//切回到系统默认语言
	//::SetThreadLocale( lcId );
	return TRUE;
}


void  CMedusaApp::LoadMedusaUI(xXmlNode* pNode , IMEdUIElement* pRootPanel , IMEdMainUI* pMainUI )
{
	xXmlNode::XmlNodes elments;
	pNode->findNode(L"Element" , elments);
	for(size_t i = 0 ;i < elments.size() ; i ++)
	{
		xXmlNode* pElNode = elments[i];
		const wchar_t* strTypeName = pElNode->value(L"type");
		IMEdUIElement* pPanel = CMEdUIElFactoryMgr::singleton()->createInstance(strTypeName);
		if(pRootPanel)
		{
			pRootPanel->AttachUIElement(pPanel);
		}
		else
		{
			pMainUI->AttachUIElement(pPanel);
		}
		LoadMedusaUI(pElNode , pPanel, pMainUI);
		pPanel->ReleaseObject();
	}
}

bool CMedusaApp::OnMainWindowCreate()
{

	//加载所有插件
	xPluginMgr::singleton()->loadPluginDir( _MEDUSA_RESPATH(L"Plugin/").c_str() );
	xPluginMgr::singleton()->loadPluginDir( _XEVOL_ABSPATH_(L"./Medusa/Plugin/") );

	xXmlDocument uiDoc;
	if(false == uiDoc.load( m_UILayoutFile.c_str()  ) )
	{
		return false;
	}
	xXmlNode* pTestUISchema = uiDoc.root()->findNode( m_UILayoutName.c_str() );
	LoadMedusaUI(pTestUISchema , NULL , GetMedusaEditor()->GetUI());
    //=======================================================
	LoadState( (CMainFrame*)m_pMainWnd , NULL);
	this->LoadCustomState();
	//CRect _rect ;
	//int flag;
	//int shcmd;
	//this->LoadWindowPlacement(_rect , flag , shcmd);
	//=======================================================
	xXmlNode::XmlNodes toolbars;
	pTestUISchema->findNode(L"Toolbar" , toolbars);
	for(size_t i = 0 ;i < toolbars.size() ; i ++)
	{
		xXmlNode* pTbNode = toolbars[i];
		const wchar_t* strToolbarName = pTbNode->value(L"name");
		GetMedusaEditor()->GetUI()->CreateToolbar( strToolbarName );
	}
	return true;
}
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CMedusaApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMedusaApp customization load/save methods

void CMedusaApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMedusaApp::LoadCustomState()
{
}

void CMedusaApp::SaveCustomState()
{
}

// CMedusaApp message handlers




BOOL CMedusaApp::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class
	//CEditorEnv::OnIdle();
	return __super::OnIdle(lCount);
}


// CMedusaApp construction
extern "C"
{
	MEDUSA_API IMedusaEditor* GetMedusaEditor()
	{
		return &theApp.GetEditor();
	}
}

int CMedusaApp::ExitInstance()
{
	
	m_sysConfig.setXMLStyle(true);
	m_sysConfig.save( m_AppCfgFile.c_str() );
	m_MedusaEditor.GetUI()->DetachUIElement();
	// TODO: Add your specialized code here and/or call the base class	
	CEditorEnv::Uninit();
	m_MedusaEditor.GetUI()->DestroyToolbar();
	return __super::ExitInstance();
}

void CMedusaApp::OnEditUndo()
{
	// TODO: Add your command handler code here
	GetEditor().GetUI()->Undo();
}

void CMedusaApp::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	if(GetEditor().GetUI()->CanUndo() )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
	// TODO: Add your command update UI handler code here
}

void CMedusaApp::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	if(GetEditor().GetUI()->CanRedo() )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
	// TODO: Add your command update UI handler code here
}

void CMedusaApp::OnEditRedo()
{
	GetEditor().GetUI()->Redo();
	// TODO: Add your command handler code here
}
