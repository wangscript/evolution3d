#include "stdafx.h"
#include "ShaderComposer.h"

BOOL DoExit()
{
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
BOOL DoInit()
{
	HINSTANCE hInstance = g_Instance;
	int nReturnCode = -1;
	CWinThread* pThread = AfxGetThread();
	CWinApp* pApp = AfxGetApp();

	// AFX internal initialization
	if (!AfxWinInit(hInstance, NULL, TEXT(""), SW_SHOW))
		return DoExit();

	// App global initializations (rare)
	if (pApp != NULL && !pApp->InitApplication())
		return DoExit();

	// Perform specific initializations
	if (!pThread->InitInstance())
	{
		if (pThread->m_pMainWnd != NULL)
		{
			TRACE(traceAppMsg, 0, "Warning: Destroying non-NULL m_pMainWnd\n");
			pThread->m_pMainWnd->DestroyWindow();
		}
		nReturnCode = pThread->ExitInstance();
		return DoExit();
	}

	return TRUE;
}

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
	SHADERCOMPOSER_API IShaderComposerUI * GetShaderComposerUI()
	{
		CShaderComposerApp* pApp = (CShaderComposerApp*)AfxGetApp();
		IShaderComposerUI* pUI = dynamic_cast<IShaderComposerUI*>(pApp);
		return pUI;
	}
};

