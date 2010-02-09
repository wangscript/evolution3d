// FindDiffQQ.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FindDiffQQ.h"
#include "FindDiffQQDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFindDiffQQApp

BEGIN_MESSAGE_MAP(CFindDiffQQApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFindDiffQQApp construction

CFindDiffQQApp::CFindDiffQQApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CFindDiffQQApp object

CFindDiffQQApp theApp;

#include "LoginDlg.h"
// CFindDiffQQApp initialization
extern bool bUserLogin ;
CFindDiffQQDlg dlg;
BOOL CFindDiffQQApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	//CLoginDlg logindlg;
	//logindlg.DoModal();
	bUserLogin = true;
	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	//AfxOleSetUserCtrl(FALSE);

	m_pMainWnd = &dlg;
	dlg.Create(CFindDiffQQDlg::IDD,NULL);
	dlg.ShowWindow(SW_SHOW);
	//INT_PTR nResponse = dlg.DoModal();
	////if (nResponse == IDOK)
	//{
	//	// TODO: Place code here to handle when the dialog is
	//	//  dismissed with OK
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO: Place code here to handle when the dialog is
	//	//  dismissed with Cancel
	//}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return TRUE;
}

BOOL CFindDiffQQApp::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class
    CFindDiffQQDlg* pDlg = (CFindDiffQQDlg*)m_pMainWnd;
	pDlg->OnIdle();
	return CWinApp::OnIdle(lCount);
}
