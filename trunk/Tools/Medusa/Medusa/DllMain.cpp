#include "stdafx.h"
#include "xMedusaApp.h"
#include "EditorFramework/Medusa.h"
class xMedusaApplication : public IMedusaApplication
{
public:
	bool Start();
	bool Exit();
	void SetAppName(const wchar_t* _appName);
	void SetUILayout(const wchar_t* layoutFile , const wchar_t* layoutName , bool bRelPath);
	void SetInitScene(const wchar_t* sceneFile );
	void SetWorkDir(const wchar_t*);
	void DeleteObject(){ delete this ; }
	void updateFrame();
	bool messageLoop();
};

bool xMedusaApplication::Exit()
{
    theApp.ExitInstance();
#ifdef _DEBUG    
	// Check for missing AfxLockTempMap calls
	if (AfxGetModuleThreadState()->m_nTempMapLock != 0)
	{
		TRACE(traceAppMsg, 0, "Warning: Temp map lock count non-zero (%ld).\n",
			AfxGetModuleThreadState()->m_nTempMapLock);
	}
	AfxLockTempMaps();
	AfxUnlockTempMaps(-1);
#endif

	AfxWinTerm();
	return TRUE;
}

static HINSTANCE g_Instance = NULL;
bool xMedusaApplication::Start()
{
	HINSTANCE hInstance = g_Instance;
	int nReturnCode = -1;
	CWinThread* pThread = AfxGetThread();
	CWinApp* pApp = AfxGetApp();

	// AFX internal initialization
	if (!AfxWinInit(hInstance, NULL, TEXT(""), SW_SHOW))
		return Exit();

	// App global initializations (rare)
	if (pApp != NULL && !pApp->InitApplication())
		return Exit();

	// Perform specific initializations
	if (!pThread->InitInstance())
	{
		if (pThread->m_pMainWnd != NULL)
		{
			TRACE(traceAppMsg, 0, "Warning: Destroying non-NULL m_pMainWnd\n");
			pThread->m_pMainWnd->DestroyWindow();
		}
		nReturnCode = pThread->ExitInstance();
		return Exit();
	}

	return TRUE;
}
void xMedusaApplication::SetWorkDir(const wchar_t* _workDir)
{
	theApp.SetWorkDir(_workDir);
}

void xMedusaApplication::updateFrame()
{
    GetEditorEnv()->OnIdle();
}

bool xMedusaApplication::messageLoop()
{
	//return GetEditorEnv()->messageLoop();
	MSG msg;
	//long  thisTick;
	long  lastTick;
	CEvolEnviroment* pCurrentApp = GetEditorEnv()->evol3DEnv();
	pCurrentApp->setApplicationState(IApplication::AS_RUN);
	lastTick= ::GetTickCount();
	while( 1 )
	{  
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if( !GetMessage( &msg, NULL, 0, 0 ) )//Remove The message From the Message Queue
			{
				if(pCurrentApp)
				{
					pCurrentApp->exitAppliction();
				}				
				return msg.wParam != 0;
			}

			_AFX_THREAD_STATE *pState = AfxGetThreadState();
			//if ( !TranslateAccelerator( g_AppData.hWnd, g_AppData.hAccel, &msg ) ) 
			if (msg.message != WM_KICKIDLE && !AfxPreTranslateMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else if(pCurrentApp && pCurrentApp->isRun())
		{
			long thisTickt = GetTickCount();
			long passedTime = thisTickt - lastTick;
			lastTick = thisTickt;
			if(pCurrentApp)
			{
				pCurrentApp->prevUpdateFrame(passedTime);
				pCurrentApp->updateFrame(passedTime);
				pCurrentApp->postUpdateFrame(passedTime);
			}
		}
		else
		{   
			WaitMessage();
		}
	}
}

void xMedusaApplication::SetAppName(const wchar_t* _appName)
{
	theApp.SetAppName(_appName);
}

void xMedusaApplication::SetUILayout(const wchar_t* layoutFile , const wchar_t* layoutName , bool bRelPath)
{
	theApp.SetUILayout(layoutFile , layoutName , bRelPath);
}

void xMedusaApplication::SetInitScene(const wchar_t* sceneFile )
{
	theApp.SetInitSceneFile(sceneFile);
}

xMedusaApplication DllApp;

BOOLEAN WINAPI DllMain(IN HINSTANCE hDllHandle, 	IN DWORD     nReason,  	IN LPVOID    Reserved    	)
{

	if(nReason == DLL_PROCESS_ATTACH)
	{
		g_Instance =hDllHandle;
	}
	else if(nReason == DLL_PROCESS_DETACH)
	{
	}
	return TRUE;
}

extern "C" 
{
	MEDUSA_API IMedusaApplication * GetMedusaApp()
	{
		static xMedusaApplication g_App;
		return &g_App ; //new xMedusaApplication();
	}
};

