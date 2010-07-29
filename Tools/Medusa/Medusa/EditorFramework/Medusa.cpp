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

// Medusa.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Medusa.h"
#include "MainFrm.h"

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
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)

	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CMedusaApp::CreateFileView)
	ON_COMMAND(ID_FILE_OPEN, &CMedusaApp::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE   , &CMedusaApp::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMedusaApp::OnFileSaveAs)

    ON_COMMAND(ID_EDIT_UNDO, &CMedusaApp::OnEditUndo)
END_MESSAGE_MAP()


// CMedusaApp 构造

CMedusaApp::CMedusaApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Medusa.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

	m_AppName       = L"Medusa";
	m_WorkDir       = L"Medusa";
	m_UILayoutFile  = L"Configure/uidef.xml";
	m_UILayoutName  = L"Default";
	m_InitSceneFile = L"scene/demoScene.xml";
	m_pRenderWindow = NULL;

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
	if(this->bInited() == false && m_pRenderWindow == NULL )
	{
		__super::OnFileNew();

		if(m_pRenderWindow)
		{
			SetRenderWindow( m_pRenderWindow->GetSafeHwnd() );
			initRenderer(NULL);
		}
		assert(m_pRenderWindow);
	}
	else
	{
		//重新建立一个Scene。相当于把原来的Scene清空了。
	}

}

void CMedusaApp::OnFileOpen()
{

}
// 唯一的一个 CMedusaApp 对象

CMedusaApp theApp;


// CMedusaApp 初始化

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

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey( m_AppName.c_str() );
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMedusaDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CMedusaView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


    m_LocaleToolbar.Register();

	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;


	

	//--------------------------
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生


	//初始化完毕
	if( bSplash )
	{
		Splash.Hide();
	}
	if(m_InitSceneFile.length() > 0 )
	{
		//Now load the scene
		evol3DEnv()->loadScene( _MEDUSA_RESPATH(m_InitSceneFile.c_str()).c_str() );
	}
	m_pMainWnd->SetWindowText(m_AppName.c_str() );
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

bool  CMedusaApp::OnRenderWindowCreate(CMedusaView* pView)
{
	  assert(m_pRenderWindow == NULL);
	  m_pRenderWindow = pView;
	  return m_pRenderWindow != NULL;
}

bool CMedusaApp::OnMainWindowCreate(CMainFrame* pMainFrame)
{

    //--------------------------
    m_MedusaEditor.SetMainFrame(  pMainFrame );

	//加载所有插件
	xPluginMgr::singleton()->loadPluginDir( _MEDUSA_RESPATH(L"Plugin/").c_str() );
	xPluginMgr::singleton()->loadPluginDir( _XEVOL_ABSPATH_(L"./Medusa/Plugin/") );

	xXmlDocument uiDoc;
	if(false == uiDoc.load( m_UILayoutFile.c_str()  ) )
	{
		return false;
	}

    //=======================================================
    //LoadState( (CMainFrame*)m_pMainWnd , NULL);
    //this->LoadCustomState();
    //CRect _rect ;
    //int flag;
    //int shcmd;
    //this->LoadWindowPlacement(_rect , flag , shcmd);
    //=======================================================

    xXmlNode::XmlNodes toolbars;
    xXmlNode* pTestUISchema = uiDoc.root()->findNode( m_UILayoutName.c_str() );

    pTestUISchema->findNode(L"Toolbar" , toolbars);
    for(size_t i = 0 ;i < toolbars.size() ; i ++)
    {
        xXmlNode* pTbNode = toolbars[i];
        const wchar_t* strToolbarName = pTbNode->value(L"name");
        GetMedusaEditor()->GetUI()->CreateToolbar( strToolbarName );
    }
    	
	LoadMedusaUI(pTestUISchema , NULL , GetMedusaEditor()->GetUI());

    pMainFrame->SetTitle( m_AppName.c_str() );
	return true;
}

int CMedusaApp::ExitInstance()
{

	m_MedusaEditor.GetUI()->DetachUIElement();
	int _ret = __super::ExitInstance();
	m_sysConfig.setXMLStyle(true);
	m_sysConfig.save( m_AppCfgFile.c_str() );

	// TODO: Add your specialized code here and/or call the base class	
	CEditorEnv::Uninit();


	m_MedusaEditor.GetUI()->DestroyToolbar();
	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

BOOL CMedusaApp::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class
	//CEditorEnv::OnIdle();
	return __super::OnIdle(lCount);
}
// CMedusaApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CMedusaApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMedusaApp 自定义加载/保存方法

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

// CMedusaApp 消息处理程序






//////////////////////////////////////////////////////////////////////////


std::wstring  _MEDUSA_RESPATH(const wchar_t* _path)    
{
	std::wstring _fullPath = std::wstring(L"./" ) + theApp.GetWorkDir() + std::wstring(L"/" ) + _path;
	std::ds_wstring _absPath = xOperationSys::singleton()->getAbsResourcePath( _fullPath.c_str() );
	std::wstring  ret = _absPath.c_str();
	return ret ;
}

extern "C"
{
	MEDUSA_API IMedusaEditor* GetMedusaEditor()
	{
		return &theApp.GetEditor();
	}
}

void CMedusaApp::OnEditUndo()
{
    // TODO: Add your command handler code here
}
